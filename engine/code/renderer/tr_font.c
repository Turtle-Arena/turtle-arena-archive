/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// tr_font.c
// 
//
// The font system uses FreeType 2.x to render TrueType fonts for use within the game.
// Quake III: Team Arena uses these fonts for all of the ui and about 90% of the cgame
// presentation. A few areas of the CGAME were left uses the old fonts since the code
// is shared with standard Q3A. Turtle Arena replaced the last bits so
// all text uses FreeType.
//
// Q3A 1.25+ and Team Arena were shipped with the font rendering code disabled, there was
// a patent held by Apple at the time which FreeType MIGHT infringe on.
// This removed any potential patent issues and it kept us (id Software) from having to 
// distribute an actual TrueTrype font which is 1. expensive to do and 2. seems to require
// an act of god to accomplish.
//
// What we did was pre-render the fonts using FreeType ( which is why we leave the FreeType
// credit in the credits ) and then saved off the glyph data and then hand touched up the 
// font bitmaps so they scale a bit better in GL.
//
// In the UI Scripting code, a scale of 1.0 is equal to a 48 point font. In Team Arena, we
// use three or four scales, most of them exactly equaling the specific rendered size. We 
// rendered three sizes in Team Arena, 12, 16, and 20.


#include "tr_local.h"
#include "../qcommon/qcommon.h"

#include <ft2build.h>
#include <freetype/fterrors.h>
#include <freetype/ftsystem.h>
#include <freetype/ftimage.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>

#define _FLOOR(x)  ((x) & -64)
#define _CEIL(x)   (((x)+63) & -64)
#define _TRUNC(x)  ((x) >> 6)

FT_Library ftLibrary = NULL;  

#ifdef IOQ3ZTM // USE_FREETYPE
#define MAX_FONTS 12
#else
#define MAX_FONTS 6
#endif
static int registeredFontCount = 0;
static fontInfo_t registeredFont[MAX_FONTS];

void R_GetGlyphInfo(FT_GlyphSlot glyph, int *left, int *right, int *width, int *top, int *bottom, int *height, int *pitch) {
	*left  = _FLOOR( glyph->metrics.horiBearingX );
	*right = _CEIL( glyph->metrics.horiBearingX + glyph->metrics.width );
	*width = _TRUNC(*right - *left);

	*top    = _CEIL( glyph->metrics.horiBearingY );
	*bottom = _FLOOR( glyph->metrics.horiBearingY - glyph->metrics.height );
	*height = _TRUNC( *top - *bottom );
	*pitch  = ( qtrue ? (*width+3) & -4 : (*width+7) >> 3 );
}


FT_Bitmap *R_RenderGlyph(FT_GlyphSlot glyph, glyphInfo_t* glyphOut) {
	FT_Bitmap  *bit2;
	int left, right, width, top, bottom, height, pitch, size;

	R_GetGlyphInfo(glyph, &left, &right, &width, &top, &bottom, &height, &pitch);

	if ( glyph->format == ft_glyph_format_outline ) {
		size   = pitch*height; 

		bit2 = ri.Malloc(sizeof(FT_Bitmap));

		bit2->width      = width;
		bit2->rows       = height;
		bit2->pitch      = pitch;
		bit2->pixel_mode = ft_pixel_mode_grays;
		//bit2->pixel_mode = ft_pixel_mode_mono;
		bit2->buffer     = ri.Malloc(pitch*height);
		bit2->num_grays = 256;

		Com_Memset( bit2->buffer, 0, size );

		FT_Outline_Translate( &glyph->outline, -left, -bottom );

		FT_Outline_Get_Bitmap( ftLibrary, &glyph->outline, bit2 );

		glyphOut->height = height;
		glyphOut->pitch = pitch;
		glyphOut->top = _TRUNC(glyph->metrics.horiBearingY) + 1;
		glyphOut->bottom = bottom;

		return bit2;
	} else {
		ri.Printf(PRINT_ALL, "Non-outline fonts are not supported\n");
	}
	return NULL;
}

static void WriteTGA (char *filename, byte *data, int width, int height) {
	byte			*buffer;
	int				i, c;
	int             row;
	unsigned char  *flip;
	unsigned char  *src, *dst;

	buffer = ri.Malloc(width*height*4 + 18);
	Com_Memset (buffer, 0, 18);
	buffer[2] = 2;		// uncompressed type
	buffer[12] = width&255;
	buffer[13] = width>>8;
	buffer[14] = height&255;
	buffer[15] = height>>8;
	buffer[16] = 32;	// pixel size

	// swap rgb to bgr
	c = 18 + width * height * 4;
	for (i=18 ; i<c ; i+=4)
	{
		buffer[i] = data[i-18+2];		// blue
		buffer[i+1] = data[i-18+1];		// green
		buffer[i+2] = data[i-18+0];		// red
		buffer[i+3] = data[i-18+3];		// alpha
	}

	// flip upside down
	flip = (unsigned char *)ri.Malloc(width*4);
	for(row = 0; row < height/2; row++)
	{
		src = buffer + 18 + row * 4 * width;
		dst = buffer + 18 + (height - row - 1) * 4 * width;

		Com_Memcpy(flip, src, width*4);
		Com_Memcpy(src, dst, width*4);
		Com_Memcpy(dst, flip, width*4);
	}
	ri.Free(flip);

	ri.FS_WriteFile(filename, buffer, c);

	//f = fopen (filename, "wb");
	//fwrite (buffer, 1, c, f);
	//fclose (f);

	ri.Free (buffer);
}

static glyphInfo_t *RE_ConstructGlyphInfo(int imageSize, unsigned char *imageOut, int *xOut, int *yOut, int *maxHeight, FT_Face face, const unsigned char c, qboolean calcHeight) {
	int i;
	static glyphInfo_t glyph;
	unsigned char *src, *dst;
	float scaled_width, scaled_height;
	FT_Bitmap *bitmap = NULL;

	Com_Memset(&glyph, 0, sizeof(glyphInfo_t));
	// make sure everything is here
	if (face != NULL) {
		FT_Load_Glyph(face, FT_Get_Char_Index( face, c), FT_LOAD_DEFAULT );
		bitmap = R_RenderGlyph(face->glyph, &glyph);
		if (bitmap) {
			glyph.xSkip = (face->glyph->metrics.horiAdvance >> 6) + 1;
		} else {
			return &glyph;
		}

		if (glyph.height > *maxHeight) {
			*maxHeight = glyph.height;
		}

		if (calcHeight) {
			ri.Free(bitmap->buffer);
			ri.Free(bitmap);
			return &glyph;
		}

/*
		// need to convert to power of 2 sizes so we do not get 
		// any scaling from the gl upload
		for (scaled_width = 1 ; scaled_width < glyph.pitch ; scaled_width<<=1)
			;
		for (scaled_height = 1 ; scaled_height < glyph.height ; scaled_height<<=1)
			;
*/

		scaled_width = glyph.pitch;
		scaled_height = glyph.height;

		// we need to make sure we fit
		if (*xOut + scaled_width + 1 >= imageSize-1) {
			*xOut = 0;
			*yOut += *maxHeight + 1;
		}

		if (*yOut + *maxHeight + 1 >= imageSize-1) {
			*yOut = -1;
			*xOut = -1;
			ri.Free(bitmap->buffer);
			ri.Free(bitmap);
			return &glyph;
		}


		src = bitmap->buffer;
		dst = imageOut + (*yOut * imageSize) + *xOut;

		if (bitmap->pixel_mode == ft_pixel_mode_mono) {
			for (i = 0; i < glyph.height; i++) {
				int j;
				unsigned char *_src = src;
				unsigned char *_dst = dst;
				unsigned char mask = 0x80;
				unsigned char val = *_src;
				for (j = 0; j < glyph.pitch; j++) {
					if (mask == 0x80) {
						val = *_src++;
					}
					if (val & mask) {
						*_dst = 0xff;
					}
					mask >>= 1;

					if ( mask == 0 ) {
						mask = 0x80;
					}
					_dst++;
				}

				src += glyph.pitch;
				dst += imageSize;
			}
		} else {
			for (i = 0; i < glyph.height; i++) {
				Com_Memcpy(dst, src, glyph.pitch);
				src += glyph.pitch;
				dst += imageSize;
			}
		}

		// we now have an 8 bit per pixel grey scale bitmap 
		// that is width wide and pf->ftSize->metrics.y_ppem tall

		glyph.imageHeight = scaled_height;
		glyph.imageWidth = scaled_width;
		glyph.s = (float)*xOut / imageSize;
		glyph.t = (float)*yOut / imageSize;
		glyph.s2 = glyph.s + (float)scaled_width / imageSize;
		glyph.t2 = glyph.t + (float)scaled_height / imageSize;

		*xOut += scaled_width + 1;
	}

	ri.Free(bitmap->buffer);
	ri.Free(bitmap);

	return &glyph;
}

static int fdOffset;
static byte	*fdFile;

int readInt( void ) {
	int i = fdFile[fdOffset]+(fdFile[fdOffset+1]<<8)+(fdFile[fdOffset+2]<<16)+(fdFile[fdOffset+3]<<24);
	fdOffset += 4;
	return i;
}

typedef union {
	byte	fred[4];
	float	ffred;
} poor;

float readFloat( void ) {
	poor	me;
#if defined Q3_BIG_ENDIAN
	me.fred[0] = fdFile[fdOffset+3];
	me.fred[1] = fdFile[fdOffset+2];
	me.fred[2] = fdFile[fdOffset+1];
	me.fred[3] = fdFile[fdOffset+0];
#elif defined Q3_LITTLE_ENDIAN
	me.fred[0] = fdFile[fdOffset+0];
	me.fred[1] = fdFile[fdOffset+1];
	me.fred[2] = fdFile[fdOffset+2];
	me.fred[3] = fdFile[fdOffset+3];
#endif
	fdOffset += 4;
	return me.ffred;
}

/*
==================
R_GetCachedFont

Get already registered font or load pre-rendered font.
==================
*/
static qboolean R_GetCachedFont(const char *name, fontInfo_t *font) {
	int		i;
	int		len;
	void	*faceData;

	for (i = 0; i < registeredFontCount; i++) {
		if (Q_stricmp(name, registeredFont[i].name) == 0) {
			Com_Memcpy(font, &registeredFont[i], sizeof(fontInfo_t));
			return qtrue;
		}
	}

	if (registeredFontCount >= MAX_FONTS) {
		return qfalse;
	}

	len = ri.FS_ReadFile(name, NULL);
	if (len == sizeof(fontInfo_t)) {
		ri.FS_ReadFile(name, &faceData);
		fdOffset = 0;
		fdFile = faceData;
		for(i=0; i<GLYPHS_PER_FONT; i++) {
			font->glyphs[i].height		= readInt();
			font->glyphs[i].top			= readInt();
			font->glyphs[i].bottom		= readInt();
			font->glyphs[i].pitch		= readInt();
			font->glyphs[i].xSkip		= readInt();
			font->glyphs[i].imageWidth	= readInt();
			font->glyphs[i].imageHeight = readInt();
			font->glyphs[i].s			= readFloat();
			font->glyphs[i].t			= readFloat();
			font->glyphs[i].s2			= readFloat();
			font->glyphs[i].t2			= readFloat();
			font->glyphs[i].glyph		= readInt();
			Q_strncpyz(font->glyphs[i].shaderName, (const char *)&fdFile[fdOffset], sizeof(font->glyphs[i].shaderName));
			fdOffset += sizeof(font->glyphs[i].shaderName);
		}
		font->glyphScale = readFloat();
		Com_Memcpy(font->name, &fdFile[fdOffset], MAX_QPATH);

//		Com_Memcpy(font, faceData, sizeof(fontInfo_t));
		Q_strncpyz(font->name, name, sizeof(font->name));
		for (i = GLYPH_START; i < GLYPH_END; i++) {
			font->glyphs[i].glyph = RE_RegisterShaderNoMip(font->glyphs[i].shaderName);
		}
		Com_Memcpy(&registeredFont[registeredFontCount++], font, sizeof(fontInfo_t));
		return qtrue;
	}

	return qfalse;
}

void RE_RegisterFont(const char *_fontName, int pointSize, fontInfo_t *font) {
	FT_Face		face;
	int			j, k, xOut, yOut, lastStart, imageNumber;
	int			scaledSize, newSize, maxHeight, left;
	unsigned char *out, *imageBuff;
	glyphInfo_t *glyph;
	image_t		*image;
	qhandle_t	h;
	float		max;
	int			imageSize;
	float		dpi;
	float		dpiScale;
	float		glyphScale;
	void		*faceData;
	int			i, len;
	char		fontName[MAX_QPATH];
	char		strippedName[MAX_QPATH];
	char		name[MAX_QPATH];

	if (!_fontName) {
		ri.Printf(PRINT_ALL, "RE_RegisterFont: called with empty name\n");
		return;
	}

	if (pointSize <= 0) {
		pointSize = 12;
	}

	// make sure the render thread is stopped
	R_SyncRenderThread();

	Q_strncpyz(fontName, _fontName, sizeof (fontName));
	COM_StripExtension(fontName, strippedName, sizeof(strippedName));

	//Compatibility: If there is no extension, assume this is loading one of the legacy fonts
	if(!Q_stricmpn(strippedName, fontName, strlen(fontName))) {
		Com_sprintf(name, sizeof(name), "fonts/fontImage_%i.dat", pointSize);
		if (R_GetCachedFont(name, font))
			return;

		// Try default font
#ifdef TA_DATA
		Q_strncpyz(fontName, "fonts/mplus-1c-regular.ttf", sizeof (fontName));
#else
		Q_strncpyz(fontName, "fonts/fontImage.ttf", sizeof (fontName));
#endif
		COM_StripExtension(fontName, strippedName, sizeof(strippedName));
	}

	Com_sprintf(name, sizeof(name), "%s_%i.dat", strippedName, pointSize);
	if (R_GetCachedFont(name, font))
		return;

	if (registeredFontCount >= MAX_FONTS) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: Too many fonts registered already.\n");
		return;
	}

	if (ftLibrary == NULL) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: FreeType not initialized.\n");
		return;
	}

	len = ri.FS_ReadFile(fontName, &faceData);
	if (len <= 0) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: Unable to read font file '%s'\n", fontName);
		return;
	}

	// allocate on the stack first in case we fail
	if (FT_New_Memory_Face( ftLibrary, faceData, len, 0, &face )) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: FreeType, unable to allocate new face.\n");
		return;
	}

	// scale dpi based on screen height
	dpi = 72.0f * (glConfig.vidHeight / SCREEN_HEIGHT);

	if (FT_Set_Char_Size( face, pointSize << 6, pointSize << 6, dpi, dpi)) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: FreeType, unable to set face char size.\n");
		return;
	}

	//*font = &registeredFonts[registeredFontCount++];

	// scale image size based on screen height, use the next higher power of two
	for (imageSize = 1; imageSize < 256.0f * (glConfig.vidHeight / SCREEN_HEIGHT); imageSize<<=1);

	// do not exceed maxTextureSize
	if (imageSize > glConfig.maxTextureSize) {
		imageSize = glConfig.maxTextureSize;
	}

	// make a 256x256 image buffer, once it is full, register it, clean it and keep going 
	// until all glyphs are rendered
	out = ri.Malloc(imageSize*imageSize*4);
	if (out == NULL) {
		ri.Printf(PRINT_WARNING, "RE_RegisterFont: ri.Malloc failure during output image creation.\n");
		return;
	}
	Com_Memset(out, 0, imageSize*imageSize*4);

	maxHeight = 0;

	for (i = GLYPH_START; i < GLYPH_END; i++) {
		glyph = RE_ConstructGlyphInfo(imageSize, out, &xOut, &yOut, &maxHeight, face, (unsigned char)i, qtrue);
	}

	xOut = 0;
	yOut = 0;
	i = GLYPH_START;
	lastStart = i;
	imageNumber = 0;

	// change the scale to be relative to 1 based on 72 dpi ( so dpi of 144 means a scale of .5 )
	dpiScale = 72.0f / dpi;

	// we also need to adjust the scale based on point size relative to 48 points as the ui scaling is based on a 48 point font
	glyphScale = 48.0f / pointSize;

	while ( i <= GLYPH_END ) {

		glyph = RE_ConstructGlyphInfo(imageSize, out, &xOut, &yOut, &maxHeight, face, (unsigned char)i, qfalse);

		// Scale to compensate for DPI
		glyph->top *= dpiScale;
		glyph->xSkip *= dpiScale;
		glyph->imageHeight *= dpiScale;
		glyph->imageWidth *= dpiScale;

		if (xOut == -1 || yOut == -1 || i == GLYPH_END)  {
			// ran out of room
			// we need to create an image from the bitmap, set all the handles in the glyphs to this point
			// 

			scaledSize = imageSize*imageSize;
			newSize = scaledSize * 4;
			imageBuff = ri.Malloc(newSize);
			left = 0;
			max = 0;
			for ( k = 0; k < (scaledSize) ; k++ ) {
				if (max < out[k]) {
					max = out[k];
				}
			}

			if (max > 0) {
				max = 255/max;
			}

			for ( k = 0; k < (scaledSize) ; k++ ) {
				imageBuff[left++] = 255;
				imageBuff[left++] = 255;
				imageBuff[left++] = 255;

				imageBuff[left++] = ((float)out[k] * max);
			}

			Com_sprintf(name, sizeof(name), "%s_%i_%i.tga", strippedName, imageNumber++, pointSize);
			if(r_saveFontData->integer && !ri.FS_FileExists(name)) {
				WriteTGA(name, imageBuff, imageSize, imageSize);
			}

			image = R_CreateImage(name, imageBuff, imageSize, imageSize, qfalse, qfalse, GL_CLAMP_TO_EDGE);
			h = RE_RegisterShaderFromImage(name, LIGHTMAP_2D, image, qfalse);
			Com_Memset(out, 0, imageSize*imageSize*4);
			xOut = 0;
			yOut = 0;
			ri.Free(imageBuff);

			if(i == GLYPH_END)
			{
				for(j = lastStart; j <= GLYPH_END; j++)
				{
					font->glyphs[j].glyph = h;
					COM_StripExtension(name, font->glyphs[j].shaderName, sizeof(font->glyphs[j].shaderName));
				}
				break;
			}
			else
			{
				for(j = lastStart; j < i; j++)
				{
					font->glyphs[j].glyph = h;
					COM_StripExtension(name, font->glyphs[j].shaderName, sizeof(font->glyphs[j].shaderName));
				}
				lastStart = i;
			}
		} else {
			Com_Memcpy(&font->glyphs[i], glyph, sizeof(glyphInfo_t));
			i++;
		}
	}

	registeredFont[registeredFontCount].glyphScale = glyphScale;
	font->glyphScale = glyphScale;
	Com_sprintf(name, sizeof(name), "%s_%i.dat", strippedName, pointSize);
	Q_strncpyz(font->name, name, sizeof(font->name));
	Com_Memcpy(&registeredFont[registeredFontCount++], font, sizeof(fontInfo_t));

	if(r_saveFontData->integer && !ri.FS_FileExists(name)) {
        ri.FS_WriteFile(name, font, sizeof(fontInfo_t));
 	}

	ri.Free(out);

	ri.FS_FreeFile(faceData);
}



void R_InitFreeType(void) {
	if (FT_Init_FreeType( &ftLibrary )) {
		ri.Printf(PRINT_WARNING, "R_InitFreeType: Unable to initialize FreeType.\n");
	}

	registeredFontCount = 0;
}


void R_DoneFreeType(void) {
	if (ftLibrary) {
		FT_Done_FreeType( ftLibrary );
		ftLibrary = NULL;
	}

	registeredFontCount = 0;
}

