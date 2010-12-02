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
//
// cg_draw.c -- draw all of the graphical elements during
// active (after loading) gameplay

#include "cg_local.h"

#ifdef MISSIONPACK_HUD2
#include "../ui/ui_shared.h"
#endif

#ifdef MISSIONPACK_HUD
// used for scoreboard
extern displayContextDef_t cgDC;
menuDef_t *menuScoreboard = NULL;
#else
int drawTeamOverlayModificationCount = -1;
#endif

#ifdef TA_HUD
#define HUD_X 40
#define HUD_Y 55 // letterbox view shouldn't overlap hud
#define HUD_WIDTH 150
#define HUD_HEIGHT 2+(CHAR_HEIGHT/2)+2+(CHAR_HEIGHT/2)+2+ICON_SIZE // was 60
#define HUD_HEAD_OFFSET_X -20
#endif

int sortedTeamPlayers[TEAM_MAXOVERLAY];
int	numSortedTeamPlayers;

char systemChat[256];
char teamChat1[256];
char teamChat2[256];

#ifdef MISSIONPACK_HUD2

#ifdef IOQ3ZTM // FONT_REWRITE
font_t *CG_FontForScale(float scale) {
	font_t *font = &cgs.media.fontSmall;
	if (scale <= cg_smallFont.value) {
		font = &cgs.media.fontTiny;
	} else if (scale > cg_bigFont.value) {
		font = &cgs.media.fontGiant;
	}

	return font;
}

int CG_Text_Width(const char *text, float scale, int limit) {
	float useScale;
	font_t *font = CG_FontForScale(scale);

	if (font->fontInfo.name[0]) {
		useScale = scale * font->fontInfo.glyphScale;
	} else {
		//float dpi = 72.0f;
		//float dpiScale = 72.0f / dpi;

		useScale = scale * (48.0f / font->pointSize);// * dpiScale;
	}

	return Com_FontStringWidthExt(font, text, limit, qtrue) * useScale;
}

int CG_Text_Height(const char *text, float scale, int limit) {
	float useScale;
	font_t *font = CG_FontForScale(scale);

	if (font->fontInfo.name[0]) {
		useScale = scale * font->fontInfo.glyphScale;
	} else {
		//float dpi = 72.0f;
		//float dpiScale = 72.0f / dpi;

		useScale = scale * (48.0f / font->pointSize);// * dpiScale;
	}

	return Com_FontStringHeight(font, text, limit) * useScale;
}

void CG_Text_Paint(float x, float y, float scale, vec4_t color, const char *text, float adjust, int limit, int style) {
	font_t *font = CG_FontForScale(scale);
	int shadowofs;

	if (style == ITEM_TEXTSTYLE_SHADOWED || style == ITEM_TEXTSTYLE_SHADOWEDMORE) {
		shadowofs = style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;
	} else {
		shadowofs = 0;
	}

	CG_DrawFontStringExt( font, scale, x, y, text, color, qfalse, qfalse, shadowofs, qtrue, adjust, limit, NULL );
}

void CG_Text_Paint_Limit(float *maxX, float x, float y, float scale, vec4_t color, const char* text, float adjust, int limit) {
	font_t *font = CG_FontForScale(scale);

	CG_DrawFontStringExt( font, scale, x, y, text, color, qfalse, qfalse, 0, qtrue, adjust, limit, maxX );
}
#else
int CG_Text_Width(const char *text, float scale, int limit) {
	int count,len;
	float out;
	glyphInfo_t *glyph;
	float useScale;
// FIXME: see ui_main.c, same problem
//	const unsigned char *s = text;
	const char *s = text;
	fontInfo_t *font = &cgDC.Assets.textFont;
	if (scale <= cg_smallFont.value) {
		font = &cgDC.Assets.smallFont;
	} else if (scale > cg_bigFont.value) {
		font = &cgDC.Assets.bigFont;
	}
	useScale = scale * font->glyphScale;
	out = 0;
	if (text) {
		len = strlen(text);
		if (limit > 0 && len > limit) {
			len = limit;
		}
		count = 0;
		while (s && *s && count < len) {
			if ( Q_IsColorString(s) ) {
				s += 2;
				continue;
			} else {
				glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
				out += glyph->xSkip;
				s++;
				count++;
			}
		}
	}
	return out * useScale;
}

int CG_Text_Height(const char *text, float scale, int limit) {
	int len, count;
	float max;
	glyphInfo_t *glyph;
	float useScale;
// TTimo: FIXME
//	const unsigned char *s = text;
	const char *s = text;
	fontInfo_t *font = &cgDC.Assets.textFont;
	if (scale <= cg_smallFont.value) {
		font = &cgDC.Assets.smallFont;
	} else if (scale > cg_bigFont.value) {
		font = &cgDC.Assets.bigFont;
	}
	useScale = scale * font->glyphScale;
	max = 0;
	if (text) {
		len = strlen(text);
		if (limit > 0 && len > limit) {
			len = limit;
		}
		count = 0;
		while (s && *s && count < len) {
			if ( Q_IsColorString(s) ) {
				s += 2;
				continue;
			} else {
				glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
				if (max < glyph->height) {
					max = glyph->height;
				}
				s++;
				count++;
			}
		}
	}
	return max * useScale;
}

void CG_Text_PaintChar(float x, float y, float width, float height, float scale, float s, float t, float s2, float t2, qhandle_t hShader) {
  float w, h;
  w = width * scale;
  h = height * scale;
  CG_AdjustFrom640( &x, &y, &w, &h );
  trap_R_DrawStretchPic( x, y, w, h, s, t, s2, t2, hShader );
}

void CG_Text_Paint(float x, float y, float scale, vec4_t color, const char *text, float adjust, int limit, int style) {
  int len, count;
	vec4_t newColor;
	glyphInfo_t *glyph;
	float useScale;
	fontInfo_t *font = &cgDC.Assets.textFont;
	if (scale <= cg_smallFont.value) {
		font = &cgDC.Assets.smallFont;
	} else if (scale > cg_bigFont.value) {
		font = &cgDC.Assets.bigFont;
	}
	useScale = scale * font->glyphScale;
	if (x == CENTER_X) {
		// center aligned
		float w;

		w = CG_Text_Width(text, scale, limit);
		x = (SCREEN_WIDTH - w) * 0.5f;
	} else if (x < 0) {
		// right aligned, offset by x
		float w;

		w = CG_Text_Width(text, scale, limit);
		x = SCREEN_WIDTH + x - w;
	}
  if (text) {
// TTimo: FIXME
//		const unsigned char *s = text;
		const char *s = text;
		trap_R_SetColor( color );
		memcpy(&newColor[0], &color[0], sizeof(vec4_t));
		len = strlen(text);
		if (limit > 0 && len > limit) {
			len = limit;
		}
		count = 0;
		while (s && *s && count < len) {
			glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
			//int yadj = Assets.textFont.glyphs[text[i]].bottom + Assets.textFont.glyphs[text[i]].top;
			//float yadj = scale * (Assets.textFont.glyphs[text[i]].imageHeight - Assets.textFont.glyphs[text[i]].height);
			if ( Q_IsColorString( s ) ) {
				memcpy( newColor, g_color_table[ColorIndex(*(s+1))], sizeof( newColor ) );
				newColor[3] = color[3];
				trap_R_SetColor( newColor );
				s += 2;
				continue;
			} else {
				float yadj = useScale * glyph->top;
				if (style == ITEM_TEXTSTYLE_SHADOWED || style == ITEM_TEXTSTYLE_SHADOWEDMORE) {
					int ofs = style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;
					colorBlack[3] = newColor[3];
					trap_R_SetColor( colorBlack );
					CG_Text_PaintChar(x + ofs, y - yadj + ofs, 
														glyph->imageWidth,
														glyph->imageHeight,
														useScale, 
														glyph->s,
														glyph->t,
														glyph->s2,
														glyph->t2,
														glyph->glyph);
					colorBlack[3] = 1.0;
					trap_R_SetColor( newColor );
				}
				CG_Text_PaintChar(x, y - yadj, 
													glyph->imageWidth,
													glyph->imageHeight,
													useScale, 
													glyph->s,
													glyph->t,
													glyph->s2,
													glyph->t2,
													glyph->glyph);
				// CG_DrawPic(x, y - yadj, scale * cgDC.Assets.textFont.glyphs[text[i]].imageWidth, scale * cgDC.Assets.textFont.glyphs[text[i]].imageHeight, cgDC.Assets.textFont.glyphs[text[i]].glyph);
				x += (glyph->xSkip * useScale) + adjust;
				s++;
				count++;
			}
		}
		trap_R_SetColor( NULL );
	}
}
#endif


#endif

/*
==============
CG_DrawField

Draws large numbers for status bar and powerups
==============
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawField (int x, int y, int width, int value) {
	char	num[16], *ptr;
	int		l;
	int		frame;

	if ( width < 1 ) {
		return;
	}

	// draw number string
	if ( width > 5 ) {
		width = 5;
	}

	switch ( width ) {
	case 1:
		value = value > 9 ? 9 : value;
		value = value < 0 ? 0 : value;
		break;
	case 2:
		value = value > 99 ? 99 : value;
		value = value < -9 ? -9 : value;
		break;
	case 3:
		value = value > 999 ? 999 : value;
		value = value < -99 ? -99 : value;
		break;
	case 4:
		value = value > 9999 ? 9999 : value;
		value = value < -999 ? -999 : value;
		break;
	}

	Com_sprintf (num, sizeof(num), "%i", value);
	l = strlen(num);
	if (l > width)
		l = width;
	x += 2 + CHAR_WIDTH*(width - l);

	ptr = num;
	while (*ptr && l)
	{
		if (*ptr == '-')
			frame = STAT_MINUS;
		else
			frame = *ptr -'0';

		CG_DrawPic( x,y, CHAR_WIDTH, CHAR_HEIGHT, cgs.media.numberShaders[frame] );
		x += CHAR_WIDTH;
		ptr++;
		l--;
	}
}
#endif // MISSIONPACK_HUD

/*
================
CG_Draw3DModel

================
*/
void CG_Draw3DModel( float x, float y, float w, float h, qhandle_t model, qhandle_t skin, vec3_t origin, vec3_t angles )
{
	refdef_t		refdef;
	refEntity_t		ent;

	if ( !cg_draw3dIcons.integer || !cg_drawIcons.integer ) {
		return;
	}

	CG_AdjustFrom640( &x, &y, &w, &h );

	memset( &refdef, 0, sizeof( refdef ) );

	memset( &ent, 0, sizeof( ent ) );
	AnglesToAxis( angles, ent.axis );
	VectorCopy( origin, ent.origin );
	ent.hModel = model;
	ent.customSkin = skin;
	ent.renderfx = RF_NOSHADOW;		// no stencil shadows

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	refdef.fov_x = 30;
	refdef.fov_y = 30;

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.time = cg.time;

	trap_R_ClearScene();
	trap_R_AddRefEntityToScene( &ent );
	trap_R_RenderScene( &refdef );
}

#if defined IOQ3ZTM || defined IOQ3ZTM_NO_COMPAT // DAMAGE_SKINS
/*
================
CG_Draw3DHeadModel
================
*/
void CG_Draw3DHeadModel( int clientNum, float x, float y, float w, float h, vec3_t origin, vec3_t angles )
{
	refdef_t		refdef;
	refEntity_t		ent;
	clientInfo_t	*ci;
	qhandle_t		model;
	qhandle_t		skin;

	ci = &cgs.clientinfo[ clientNum ];

	model = ci->headModel;
	skin = ci->headSkin;

	CG_AdjustFrom640( &x, &y, &w, &h );

	memset( &refdef, 0, sizeof( refdef ) );

	memset( &ent, 0, sizeof( ent ) );
	AnglesToAxis( angles, ent.axis );
	VectorCopy( origin, ent.origin );
	ent.hModel = model;
	ent.customSkin = skin;
#ifdef IOQ3ZTM_NO_COMPAT // DAMAGE_SKINS
	ent.skinFraction = cg_entities[clientNum].currentState.skinFraction;
#endif
	ent.renderfx = RF_NOSHADOW;		// no stencil shadows

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	refdef.fov_x = 30;
	refdef.fov_y = 30;

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.time = cg.time;

	trap_R_ClearScene();
#ifdef IOQ3ZTM // ZTM: Show powerups on statusbar/scoreboard head models!
	CG_AddRefEntityWithPowerups( &ent, &cg_entities[clientNum].currentState, ci->team );
#else
	trap_R_AddRefEntityToScene( &ent );
#endif
	trap_R_RenderScene( &refdef );
}
#endif

/*
================
CG_DrawHead

Used for both the status bar and the scoreboard
================
*/
void CG_DrawHead( float x, float y, float w, float h, int clientNum, vec3_t headAngles ) {
	clipHandle_t	cm;
	clientInfo_t	*ci;
	float			len;
	vec3_t			origin;
	vec3_t			mins, maxs;

	ci = &cgs.clientinfo[ clientNum ];

	if ( cg_draw3dIcons.integer ) {
		cm = ci->headModel;
		if ( !cm ) {
			return;
		}

		// offset the origin y and z to center the head
		trap_R_ModelBounds( cm, mins, maxs );

		origin[2] = -0.5 * ( mins[2] + maxs[2] );
		origin[1] = 0.5 * ( mins[1] + maxs[1] );

		// calculate distance so the head nearly fills the box
		// assume heads are taller than wide
		len = 0.7 * ( maxs[2] - mins[2] );		
		origin[0] = len / 0.268;	// len / tan( fov/2 )

		// allow per-model tweaking
#ifdef TA_PLAYERSYS
		VectorAdd( origin, ci->playercfg.headOffset, origin );
#else
		VectorAdd( origin, ci->headOffset, origin );
#endif

#if defined IOQ3ZTM || defined IOQ3ZTM_NO_COMPAT // DAMAGE_SKINS
		CG_Draw3DHeadModel( clientNum, x, y, w, h, origin, headAngles );
#else
		CG_Draw3DModel( x, y, w, h, ci->headModel, ci->headSkin, origin, headAngles );
#endif

	} else if ( cg_drawIcons.integer ) {
		CG_DrawPic( x, y, w, h, ci->modelIcon );
	}

	// if they are deferred, draw a cross out
	if ( ci->deferred ) {
		CG_DrawPic( x, y, w, h, cgs.media.deferShader );
	}
}

/*
================
CG_DrawFlagModel

Used for both the status bar and the scoreboard
================
*/
void CG_DrawFlagModel( float x, float y, float w, float h, int team, qboolean force2D ) {
#ifdef TA_DATA // FLAG_MODEL
	gitem_t *item;
	int itemIndex;

	if( team == TEAM_RED ) {
		item = BG_FindItemForPowerup( PW_REDFLAG );
	} else if( team == TEAM_BLUE ) {
		item = BG_FindItemForPowerup( PW_BLUEFLAG );
	} else if( team == TEAM_FREE ) {
		item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
	} else {
		return;
	}
	if (!item) {
		return;
	}
	itemIndex = ITEM_INDEX(item);

	if ( !force2D && cg_draw3dIcons.integer )
	{
		float			len;
		vec3_t			origin, angles;
		vec3_t			mins, maxs;
		qhandle_t		model;
		qhandle_t		skin;

		VectorClear( angles );
		angles[YAW] = 60 * sin( cg.time / 2000.0 );

		model = cg_items[ itemIndex ].models[0];
		skin = cg_items[ itemIndex ].skin;

		// offset the origin y and z to center the flag
		trap_R_ModelBounds( model, mins, maxs );

		origin[2] = -0.5 * ( mins[2] + maxs[2] );
		origin[1] = 0.5 * ( mins[1] + maxs[1] );

		// calculate distance so the flag nearly fills the box
		// assume heads are taller than wide
		len = 0.5 * ( maxs[2] - mins[2] );
		origin[0] = len / 0.268;	// len / tan( fov/2 )

		CG_Draw3DModel( x, y, w, h, model, skin, origin, angles );

		// Draw flag flap
		model = cg_items[ itemIndex ].models[1];

		CG_Draw3DModel( x, y, w, h, model, skin, origin, angles );
	}
	else
	{
		CG_DrawPic( x, y, w, h, cg_items[ itemIndex ].icon );
	}
#else
	qhandle_t		cm;
	float			len;
	vec3_t			origin, angles;
	vec3_t			mins, maxs;
	qhandle_t		handle;

	if ( !force2D && cg_draw3dIcons.integer ) {

		VectorClear( angles );

		cm = cgs.media.redFlagModel;

		// offset the origin y and z to center the flag
		trap_R_ModelBounds( cm, mins, maxs );

		origin[2] = -0.5 * ( mins[2] + maxs[2] );
		origin[1] = 0.5 * ( mins[1] + maxs[1] );

		// calculate distance so the flag nearly fills the box
		// assume heads are taller than wide
		len = 0.5 * ( maxs[2] - mins[2] );		
		origin[0] = len / 0.268;	// len / tan( fov/2 )

		angles[YAW] = 60 * sin( cg.time / 2000.0 );

		if( team == TEAM_RED ) {
			handle = cgs.media.redFlagModel;
		} else if( team == TEAM_BLUE ) {
			handle = cgs.media.blueFlagModel;
		} else if( team == TEAM_FREE ) {
			handle = cgs.media.neutralFlagModel;
		} else {
			return;
		}
		CG_Draw3DModel( x, y, w, h, handle, 0, origin, angles );
	} else if ( cg_drawIcons.integer ) {
		gitem_t *item;

		if( team == TEAM_RED ) {
			item = BG_FindItemForPowerup( PW_REDFLAG );
		} else if( team == TEAM_BLUE ) {
			item = BG_FindItemForPowerup( PW_BLUEFLAG );
		} else if( team == TEAM_FREE ) {
			item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
		} else {
			return;
		}
		if (item) {
		  CG_DrawPic( x, y, w, h, cg_items[ ITEM_INDEX(item) ].icon );
		}
	}
#endif
}

/*
================
CG_DrawStatusBarHead

================
*/
#ifndef MISSIONPACK_HUD

static void CG_DrawStatusBarHead( float x ) {
	vec3_t		angles;
	float		size, stretch;
	float		frac;

	VectorClear( angles );

	if ( cg.damageTime && cg.time - cg.damageTime < DAMAGE_TIME ) {
		frac = (float)(cg.time - cg.damageTime ) / DAMAGE_TIME;
		size = ICON_SIZE * 1.25 * ( 1.5 - frac * 0.5 );

		stretch = size - ICON_SIZE * 1.25;
		// kick in the direction of damage
		x -= stretch * 0.5 + cg.damageX * stretch * 0.5;

		cg.headStartYaw = 180 + cg.damageX * 45;

		cg.headEndYaw = 180 + 20 * cos( crandom()*M_PI );
		cg.headEndPitch = 5 * cos( crandom()*M_PI );

		cg.headStartTime = cg.time;
		cg.headEndTime = cg.time + 100 + random() * 2000;
	} else {
		if ( cg.time >= cg.headEndTime ) {
			// select a new head angle
			cg.headStartYaw = cg.headEndYaw;
			cg.headStartPitch = cg.headEndPitch;
			cg.headStartTime = cg.headEndTime;
			cg.headEndTime = cg.time + 100 + random() * 2000;

			cg.headEndYaw = 180 + 20 * cos( crandom()*M_PI );
			cg.headEndPitch = 5 * cos( crandom()*M_PI );
		}

		size = ICON_SIZE * 1.25;
	}

	// if the server was frozen for a while we may have a bad head start time
	if ( cg.headStartTime > cg.time ) {
		cg.headStartTime = cg.time;
	}

	frac = ( cg.time - cg.headStartTime ) / (float)( cg.headEndTime - cg.headStartTime );
	frac = frac * frac * ( 3 - 2 * frac );
	angles[YAW] = cg.headStartYaw + ( cg.headEndYaw - cg.headStartYaw ) * frac;
	angles[PITCH] = cg.headStartPitch + ( cg.headEndPitch - cg.headStartPitch ) * frac;

#ifdef TA_HUD
	CG_DrawHead( x,  (HUD_Y+(ICON_SIZE * 1.25)) - size, size, size,
#else
	CG_DrawHead( x, 480 - size, size, size, 
#endif
				cg.snap->ps.clientNum, angles );
}
#endif // MISSIONPACK_HUD

/*
================
CG_DrawStatusBarFlag

================
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawStatusBarFlag( float x, int team ) {
#ifdef TA_HUD
	CG_DrawFlagModel( x, HUD_Y, ICON_SIZE, ICON_SIZE, team, qfalse );
#else
	CG_DrawFlagModel( x, 480 - ICON_SIZE, ICON_SIZE, ICON_SIZE, team, qfalse );
#endif
}
#endif // MISSIONPACK_HUD

/*
================
CG_DrawTeamBackground

================
*/
void CG_DrawTeamBackground( int x, int y, int w, int h, float alpha, int team, int clientNum )
{
	vec4_t		hcolor;

	hcolor[3] = alpha;
	if ( team == TEAM_RED ) {
		hcolor[0] = 1;
		hcolor[1] = 0;
		hcolor[2] = 0;
	} else if ( team == TEAM_BLUE ) {
		hcolor[0] = 0;
		hcolor[1] = 0;
		hcolor[2] = 1;
	} else {
#ifdef TA_HUD
		if (clientNum < 0 || clientNum >= MAX_CLIENTS) {
			clientNum = 0;
		}
		// Use client's effect color1
		VectorCopy(cgs.clientinfo[clientNum].color1, hcolor);
#else
		return;
#endif
	}
	trap_R_SetColor( hcolor );
#ifdef IOQ3ZTM // HUD_ASPECT_CORRECT
	// ZTM: Hacky (Don't have Turtle Arena status fit screen)
	if (w == SCREEN_WIDTH)
	{
		CG_DrawPicFit( x, y, w, h, cgs.media.teamStatusBar );
	}
	else
#endif
	CG_DrawPic( x, y, w, h, cgs.media.teamStatusBar );
	trap_R_SetColor( NULL );
}

#ifdef TA_HUD
void CG_DrawFieldSmall(int x, int y, int width, int value)
{
	char	num[16], *ptr;
	int		l;
	int		frame;
	int		char_width = CHAR_WIDTH/2;
	int		char_height = CHAR_HEIGHT/2;

	if ( width < 1 ) {
		return;
	}

	// draw number string
	if ( width > 5 ) {
		width = 5;
	}

	switch ( width ) {
	case 1:
		value = value > 9 ? 9 : value;
		value = value < 0 ? 0 : value;
		break;
	case 2:
		value = value > 99 ? 99 : value;
		value = value < -9 ? -9 : value;
		break;
	case 3:
		value = value > 999 ? 999 : value;
		value = value < -99 ? -99 : value;
		break;
	case 4:
		value = value > 9999 ? 9999 : value;
		value = value < -999 ? -999 : value;
		break;
	}

	Com_sprintf (num, sizeof(num), "%i", value);
	l = strlen(num);
	if (l > width)
		l = width;
	x += 2 + char_width*(width - l);

	ptr = num;
	while (*ptr && l)
	{
		if (*ptr == '-')
			frame = STAT_MINUS;
		else
			frame = *ptr -'0';

		CG_DrawPic( x,y, char_width, char_height, cgs.media.numberShaders[frame] );
		x += char_width;
		ptr++;
		l--;
	}
}
#endif

/*
================
CG_DrawStatusBar

================
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawStatusBar( void ) {
	int			color;
	centity_t	*cent;
	playerState_t	*ps;
	int			value;
#ifndef TA_HUD
	vec4_t		hcolor;
#endif
	vec3_t		angles;
#ifdef TA_HUD
	int			start_x;
	int			x;
	int			y;
#else
	vec3_t		origin;
#endif

	static float colors[4][4] = { 
//		{ 0.2, 1.0, 0.2, 1.0 } , { 1.0, 0.2, 0.2, 1.0 }, {0.5, 0.5, 0.5, 1} };
		{ 1.0f, 0.69f, 0.0f, 1.0f },    // normal
		{ 1.0f, 0.2f, 0.2f, 1.0f },     // low health
		{ 0.5f, 0.5f, 0.5f, 1.0f },     // weapon firing
		{ 1.0f, 1.0f, 1.0f, 1.0f } };   // health > 100

	if ( cg_drawStatus.integer == 0 ) {
		return;
	}

#ifdef IOQ3ZTM // LASERTAG
	if (cg_laserTag.integer)
	{
		cent = &cg_entities[cg.snap->ps.clientNum];
		ps = &cg.snap->ps;

		// Health
		if (cg_laserTag.integer == 1)
		{
			// unlimited
			CG_DrawSmallString( 100, 320, "Unlimited Health", 1.0F );
		}
		else
		{
			value = (int)((ps->stats[STAT_HEALTH])/40)+1; // wp_lasergun damage
			CG_DrawSmallString( 100, 320, va("Health: %d hits", value), 1.0F );
		}
		return;
	}
#endif

#ifdef TA_HUD
	CG_HudPlacement(HUD_LEFT);

	start_x = x = HUD_X;
	y = HUD_Y;

	// draw hud background
	CG_DrawTeamBackground( x, y, HUD_WIDTH, HUD_HEIGHT, 0.33f, cg.snap->ps.persistant[PERS_TEAM], cg.snap->ps.clientNum );

	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	VectorClear( angles );

	if( cg.predictedPlayerState.powerups[PW_REDFLAG] ) {
		CG_DrawStatusBarFlag( x + HUD_WIDTH + TEXT_ICON_SPACE, TEAM_RED );
	} else if( cg.predictedPlayerState.powerups[PW_BLUEFLAG] ) {
		CG_DrawStatusBarFlag( x + HUD_WIDTH + TEXT_ICON_SPACE, TEAM_BLUE );
	} else if( cg.predictedPlayerState.powerups[PW_NEUTRALFLAG] ) {
		CG_DrawStatusBarFlag( x + HUD_WIDTH + TEXT_ICON_SPACE, TEAM_FREE );
	}

	CG_DrawStatusBarHead( x + HUD_HEAD_OFFSET_X);

	y += 2;

	// LINE 1: Score
	value = ps->persistant[PERS_SCORE];
	CG_DrawFieldSmall(x+25, y, 5, value);
	x += (3 * (CHAR_WIDTH/2));

	// Space between line 1 and 3
	y += CHAR_HEIGHT/2;
	x = start_x;

	// LINE2: Health
	value = ps->stats[STAT_HEALTH];
	if ( value > 100 ) {
		trap_R_SetColor( colors[3] );		// white
	} else if (value > 25) {
		trap_R_SetColor( colors[0] );	// green
	} else if (value > 0) {
		color = (cg.time >> 8) & 1;	// flash
		trap_R_SetColor( colors[color] );
	} else {
		trap_R_SetColor( colors[1] );	// red
	}

	// stretch the health up when taking damage
	CG_DrawFieldSmall(x + 25, y, 5, value);
	x += (3 * (CHAR_WIDTH/2));

	//CG_ColorForHealth( hcolor );
	//trap_R_SetColor( hcolor );
	trap_R_SetColor( NULL );

	// Space between line 2 and 3
	y += CHAR_HEIGHT/2 + (CHAR_HEIGHT/2)/8;
	x = start_x;

	// LINE3: Weapon
	if ( cent->currentState.weapon ) {
#ifdef TA_WEAPSYS_EX
		value = ps->stats[STAT_AMMO];
#else
		value = ps->ammo[cent->currentState.weapon];
#endif
		// Draw weapon icon
#ifdef TA_WEAPSYS
		if ( cg_weapongroups[cent->currentState.weapon].weaponIcon ) {
			CG_DrawPic( x, y, ICON_SIZE, ICON_SIZE,
				cg_weapongroups[cent->currentState.weapon].weaponIcon );
		}
#else
		if ( cg_weapons[cent->currentState.weapon].weaponIcon ) {
			CG_DrawPic( x, y, ICON_SIZE, ICON_SIZE,
				cg_weapons[cent->currentState.weapon].weaponIcon );
		}
#endif
		if (value == 0) {
			CG_DrawPic( x, y, ICON_SIZE, ICON_SIZE,
				cgs.media.noammoShader );
		}

		x += ICON_SIZE;

		// ammo
		if ( value > -1 ) {
			if ( cg.predictedPlayerState.weaponstate == WEAPON_FIRING
				&& cg.predictedPlayerState.weaponTime > 100 ) {
				// draw as dark grey when reloading
				color = 2;	// dark grey
			} else {
				if ( value >= 0 ) {
					color = 0;	// green
				} else {
					color = 1;	// red
				}
			}
			trap_R_SetColor( colors[color] );

			CG_DrawFieldSmall(x-CHAR_WIDTH/2, y, 3, value);
			trap_R_SetColor( NULL );
		}
			x += (2 * (CHAR_WIDTH/2));
	}

	// LINE3: Holdable item
#ifdef TA_HOLDSYS
	value = BG_ItemNumForHoldableNum(cg.snap->ps.holdableIndex);
#else
	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];
#endif
	if ( value ) {
		CG_RegisterItemVisuals( value );
		if ( cg_items[ value ].icon ) {
			CG_DrawPic( x, y, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
		}
		x += ICON_SIZE;

#ifdef TA_HOLDSYS
		// draw value

#ifdef TA_HOLDABLE // Grappling shuriken use no ammo.
		if (!bg_projectileinfo[BG_ProjectileIndexForHoldable(cg.snap->ps.holdableIndex)].grappling)
#endif
		{
#ifdef TA_ITEMSYS
			int giveQuantity = BG_ItemForItemNum(value)->quantity;
#else
			int giveQuantity = bg_itemlist[ value ].quantity;
#endif
			int useCount = cg.snap->ps.holdable[cg.snap->ps.holdableIndex];

			if ((giveQuantity > 0 && useCount > 0)
				|| (giveQuantity == 0 && useCount > 1)) // Only happens with give command.
			{
				if (cg.predictedPlayerState.holdableTime > 100
#ifdef TA_ENTSYS // FUNC_USE
					&& !(cg.predictedPlayerState.eFlags & EF_USE_ENT)
#endif
					)
				{
					// draw as dark grey when reloading
					color = 2;	// dark grey
				} else {
					color = 0;	// green
				}
				trap_R_SetColor( colors[color] );

				CG_DrawFieldSmall(x, y, 2, useCount);
				trap_R_SetColor( NULL );
				x += (2 * (CHAR_WIDTH/2));
			}
		}
#endif
	}
	else
	{
		x += ICON_SIZE;
		x += (2 * (CHAR_WIDTH/2));
	}

#ifdef TA_SP
	// LINE4: Lives (Single Player and Co-op only)
	if (cgs.gametype == GT_SINGLE_PLAYER)
	{
		// Space between line 3 and 4
		y += GIANT_HEIGHT;
		x = start_x;

		CG_DrawSmallString( x, y, va("Lives %d", ps->persistant[PERS_LIVES]), 1.0F );

		value = ps->persistant[PERS_CONTINUES];
		if (value) {
			y += SMALLCHAR_HEIGHT;
			CG_DrawSmallString( x, y, va("Continues %d", value), 1.0F );
		}
	}
#endif
#else
	CG_HudPlacement(HUD_CENTER);

	// draw the team background
	CG_DrawTeamBackground( 0, 420, 640, 60, 0.33f, cg.snap->ps.persistant[PERS_TEAM], cg.snap->ps.clientNum );

	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	VectorClear( angles );

	// draw any 3D icons first, so the changes back to 2D are minimized
#ifdef TA_WEAPSYS
	if ( cent->currentState.weapon && cg_weapongroups[ cent->currentState.weapon ].ammoModel )
#else
	if ( cent->currentState.weapon && cg_weapons[ cent->currentState.weapon ].ammoModel )
#endif
	{
		origin[0] = 70;
		origin[1] = 0;
		origin[2] = 0;
		angles[YAW] = 90 + 20 * sin( cg.time / 1000.0 );
		CG_Draw3DModel( CHAR_WIDTH*3 + TEXT_ICON_SPACE, 432, ICON_SIZE, ICON_SIZE,
#ifdef TA_WEAPSYS
					   cg_weapongroups[ cent->currentState.weapon ].ammoModel, 0, origin, angles );
#else
					   cg_weapons[ cent->currentState.weapon ].ammoModel, 0, origin, angles );
#endif
	}

	CG_DrawStatusBarHead( 185 + CHAR_WIDTH*3 + TEXT_ICON_SPACE );

	if( cg.predictedPlayerState.powerups[PW_REDFLAG] ) {
		CG_DrawStatusBarFlag( 185 + CHAR_WIDTH*3 + TEXT_ICON_SPACE + ICON_SIZE, TEAM_RED );
	} else if( cg.predictedPlayerState.powerups[PW_BLUEFLAG] ) {
		CG_DrawStatusBarFlag( 185 + CHAR_WIDTH*3 + TEXT_ICON_SPACE + ICON_SIZE, TEAM_BLUE );
	} else if( cg.predictedPlayerState.powerups[PW_NEUTRALFLAG] ) {
		CG_DrawStatusBarFlag( 185 + CHAR_WIDTH*3 + TEXT_ICON_SPACE + ICON_SIZE, TEAM_FREE );
	}

#ifndef TURTLEARENA // NOARMOR
	if ( ps->stats[ STAT_ARMOR ] ) {
		origin[0] = 90;
		origin[1] = 0;
		origin[2] = -10;
		angles[YAW] = ( cg.time & 2047 ) * 360 / 2048.0;
		CG_Draw3DModel( 370 + CHAR_WIDTH*3 + TEXT_ICON_SPACE, 432, ICON_SIZE, ICON_SIZE,
					   cgs.media.armorModel, 0, origin, angles );
	}
#endif
	//
	// ammo
	//
	if ( cent->currentState.weapon ) {
		value = ps->ammo[cent->currentState.weapon];
		if ( value > -1 ) {
			if ( cg.predictedPlayerState.weaponstate == WEAPON_FIRING
				&& cg.predictedPlayerState.weaponTime > 100 ) {
				// draw as dark grey when reloading
				color = 2;	// dark grey
			} else {
				if ( value >= 0 ) {
					color = 0;	// green
				} else {
					color = 1;	// red
				}
			}
			trap_R_SetColor( colors[color] );
			
			CG_DrawField (0, 432, 3, value);
			trap_R_SetColor( NULL );

			// if we didn't draw a 3D icon, draw a 2D icon for ammo
			if ( !cg_draw3dIcons.integer && cg_drawIcons.integer ) {
				qhandle_t	icon;

#ifdef TA_WEAPSYS
				icon = cg_weapongroups[ cg.predictedPlayerState.weapon ].ammoIcon;
#else
				icon = cg_weapons[ cg.predictedPlayerState.weapon ].ammoIcon;
#endif
				if ( icon ) {
					CG_DrawPic( CHAR_WIDTH*3 + TEXT_ICON_SPACE, 432, ICON_SIZE, ICON_SIZE, icon );
				}
			}
		}
	}

	//
	// health
	//
	value = ps->stats[STAT_HEALTH];
	if ( value > 100 ) {
		trap_R_SetColor( colors[3] );		// white
	} else if (value > 25) {
		trap_R_SetColor( colors[0] );	// green
	} else if (value > 0) {
		color = (cg.time >> 8) & 1;	// flash
		trap_R_SetColor( colors[color] );
	} else {
		trap_R_SetColor( colors[1] );	// red
	}

	// stretch the health up when taking damage
	CG_DrawField ( 185, 432, 3, value);
	CG_ColorForHealth( hcolor );
	trap_R_SetColor( hcolor );


#ifndef TURTLEARENA // NOARMOR
	//
	// armor
	//
	value = ps->stats[STAT_ARMOR];
	if (value > 0 ) {
		trap_R_SetColor( colors[0] );
		CG_DrawField (370, 432, 3, value);
		trap_R_SetColor( NULL );
		// if we didn't draw a 3D icon, draw a 2D icon for armor
		if ( !cg_draw3dIcons.integer && cg_drawIcons.integer ) {
			CG_DrawPic( 370 + CHAR_WIDTH*3 + TEXT_ICON_SPACE, 432, ICON_SIZE, ICON_SIZE, cgs.media.armorIcon );
		}

	}
#endif
#endif // TA_HUD
}
#endif // MISSIONPACK_HUD

#if defined TA_WEAPSYS || defined IOQ3ZTM // SHOW_SPEED
/*
================
CG_DrawMiddleLeft
================
*/
void CG_DrawMiddleLeft(void)
{
	int y;

	CG_HudPlacement(HUD_LEFT);

	y = SCREEN_HEIGHT/2;

	if (cg_drawSpeed.integer) {
		CG_DrawSmallString( SCREEN_WIDTH/32, y, va("Speed %f", VectorLength(cg.snap->ps.velocity)), 1.0F );
		y += SMALLCHAR_HEIGHT;
	}

	// Index of the attack animation, not how many things we've hit.
	//if (cg.snap->ps.meleeAttack > 0) {
	//	CG_DrawSmallString( SCREEN_WIDTH/32, y, va("meleeAttack %d", cg.snap->ps.meleeAttack), 1.0F );
	//	y += SMALLCHAR_HEIGHT;
	//}
}
#endif

#ifdef TURTLEARENA // NIGHTS_ITEMS
/*
================
CG_DrawScoreChain

Draw NiGHTS Link (score chain) count
================
*/
void CG_DrawScoreChain(void)
{
	float	*fadeColor;
	float	frac;
	float	color[4];
	char	*s;
	int		y;

	if (cg.snap->ps.chain <= 1) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	fadeColor = CG_FadeColor( cg.scorePickupTime, 2000 );
	frac = fadeColor[3];

	CG_ColorForChain(cg.snap->ps.chain, color);
	color[3] = fadeColor[3];

	s = va("%d Link", cg.snap->ps.chain); // ZTM: FIXME: Should be chain-1, but then colors don't make sense?

	y = SCREEN_HEIGHT - 32 - CG_Text_Height(s, frac, 0);
	CG_Text_Paint(CENTER_X, y, frac, color, s, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
}
#endif

/*
===========================================================================================

  UPPER RIGHT CORNER

===========================================================================================
*/

/*
================
CG_DrawAttacker

================
*/
static float CG_DrawAttacker( float y ) {
	int			t;
	float		size;
	vec3_t		angles;
	const char	*info;
	const char	*name;
	int			clientNum;

	if ( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 ) {
		return y;
	}

	if ( !cg.attackerTime ) {
		return y;
	}

	clientNum = cg.predictedPlayerState.persistant[PERS_ATTACKER];
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS || clientNum == cg.snap->ps.clientNum ) {
		return y;
	}

	t = cg.time - cg.attackerTime;
	if ( t > ATTACKER_HEAD_TIME ) {
		cg.attackerTime = 0;
		return y;
	}

	size = ICON_SIZE * 1.25;

	angles[PITCH] = 0;
	angles[YAW] = 180;
	angles[ROLL] = 0;
	CG_DrawHead( 640 - size, y, size, size, clientNum, angles );

	info = CG_ConfigString( CS_PLAYERS + clientNum );
	name = Info_ValueForKey(  info, "n" );
	y += size;
	CG_DrawBigString( -1, y, name, 0.5 );

	return y + BIGCHAR_HEIGHT + 2;
}

/*
==================
CG_DrawSnapshot
==================
*/
static float CG_DrawSnapshot( float y ) {
	char		*s;

	s = va( "time:%i snap:%i cmd:%i", cg.snap->serverTime, 
		cg.latestSnapshotNum, cgs.serverCommandSequence );

	CG_DrawBigString( -5, y + 2, s, 1.0F);

	return y + BIGCHAR_HEIGHT + 4;
}

/*
==================
CG_DrawFPS
==================
*/
#define	FPS_FRAMES	4
static float CG_DrawFPS( float y ) {
	char		*s;
	static int	previousTimes[FPS_FRAMES];
	static int	index;
	int		i, total;
	int		fps;
	static	int	previous;
	int		t, frameTime;

	// don't use serverTime, because that will be drifting to
	// correct for internet lag changes, timescales, timedemos, etc
	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;

	previousTimes[index % FPS_FRAMES] = frameTime;
	index++;
	if ( index > FPS_FRAMES ) {
		// average multiple frames together to smooth changes out a bit
		total = 0;
		for ( i = 0 ; i < FPS_FRAMES ; i++ ) {
			total += previousTimes[i];
		}
		if ( !total ) {
			total = 1;
		}
		fps = 1000 * FPS_FRAMES / total;

		s = va( "%ifps", fps );

		CG_DrawBigString( -5, y + 2, s, 1.0F);
	}

	return y + BIGCHAR_HEIGHT + 4;
}

/*
=================
CG_DrawTimer
=================
*/
static float CG_DrawTimer( float y ) {
	char		*s;
	int			mins, seconds, tens;
	int			msec;

	msec = cg.time - cgs.levelStartTime;

	seconds = msec / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;
	tens = seconds / 10;
	seconds -= tens * 10;

	s = va( "%i:%i%i", mins, tens, seconds );

	CG_DrawBigString( -5, y + 2, s, 1.0F);

	return y + BIGCHAR_HEIGHT + 4;
}


/*
=================
CG_DrawTeamOverlay
=================
*/

static float CG_DrawTeamOverlay( float y, qboolean right, qboolean upper ) {
	int x, w, h, xx;
	int i, j, len;
	const char *p;
	vec4_t		hcolor;
	int pwidth, lwidth;
	int plyrs;
	char st[16];
	clientInfo_t *ci;
	gitem_t	*item;
	int ret_y, count;

	if ( !cg_drawTeamOverlay.integer ) {
		return y;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] != TEAM_RED && cg.snap->ps.persistant[PERS_TEAM] != TEAM_BLUE ) {
		return y; // Not on any team
	}

	plyrs = 0;

	// max player name width
	pwidth = 0;
	count = (numSortedTeamPlayers > 8) ? 8 : numSortedTeamPlayers;
	for (i = 0; i < count; i++) {
		ci = cgs.clientinfo + sortedTeamPlayers[i];
		if ( ci->infoValid && ci->team == cg.snap->ps.persistant[PERS_TEAM]) {
			plyrs++;
#ifdef IOQ3ZTM // FONT_REWRITE
			len = Com_FontStringWidth(&cgs.media.fontTiny, ci->name, 0);
#else
			len = CG_DrawStrlen(ci->name);
#endif
			if (len > pwidth)
				pwidth = len;
		}
	}

	if (!plyrs)
		return y;

#ifdef IOQ3ZTM // FONT_REWRITE
	if (pwidth > TEAM_OVERLAY_MAXNAME_WIDTH*TINYCHAR_WIDTH)
		pwidth = TEAM_OVERLAY_MAXNAME_WIDTH*TINYCHAR_WIDTH;
#else
	if (pwidth > TEAM_OVERLAY_MAXNAME_WIDTH)
		pwidth = TEAM_OVERLAY_MAXNAME_WIDTH;
#endif

	// max location name width
	lwidth = 0;
	for (i = 1; i < MAX_LOCATIONS; i++) {
		p = CG_ConfigString(CS_LOCATIONS + i);
		if (p && *p) {
#ifdef IOQ3ZTM // FONT_REWRITE
			len = Com_FontStringWidth(&cgs.media.fontTiny, p, 0);
#else
			len = CG_DrawStrlen(p);
#endif
			if (len > lwidth)
				lwidth = len;
		}
	}

#ifdef IOQ3ZTM // FONT_REWRITE
	if (lwidth > TEAM_OVERLAY_MAXLOCATION_WIDTH*TINYCHAR_WIDTH)
		lwidth = TEAM_OVERLAY_MAXLOCATION_WIDTH*TINYCHAR_WIDTH;

	w = pwidth + lwidth + (4 + 7) * TINYCHAR_WIDTH;
#else
	if (lwidth > TEAM_OVERLAY_MAXLOCATION_WIDTH)
		lwidth = TEAM_OVERLAY_MAXLOCATION_WIDTH;

	w = (pwidth + lwidth + 4 + 7) * TINYCHAR_WIDTH;
#endif

	if ( right )
		x = 640 - w;
	else
		x = 0;

	h = plyrs * TINYCHAR_HEIGHT;

	if ( upper ) {
		ret_y = y + h;
	} else {
		y -= h;
		ret_y = y;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED ) {
		hcolor[0] = 1.0f;
		hcolor[1] = 0.0f;
		hcolor[2] = 0.0f;
		hcolor[3] = 0.33f;
	} else { // if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE )
		hcolor[0] = 0.0f;
		hcolor[1] = 0.0f;
		hcolor[2] = 1.0f;
		hcolor[3] = 0.33f;
	}
	trap_R_SetColor( hcolor );
	CG_DrawPic( x, y, w, h, cgs.media.teamStatusBar );
	trap_R_SetColor( NULL );

	for (i = 0; i < count; i++) {
		ci = cgs.clientinfo + sortedTeamPlayers[i];
		if ( ci->infoValid && ci->team == cg.snap->ps.persistant[PERS_TEAM]) {

			hcolor[0] = hcolor[1] = hcolor[2] = hcolor[3] = 1.0;

			xx = x + TINYCHAR_WIDTH;

			CG_DrawStringExt( xx, y,
				ci->name, hcolor, qfalse, qfalse,
				TINYCHAR_WIDTH, TINYCHAR_HEIGHT, TEAM_OVERLAY_MAXNAME_WIDTH);

			if (lwidth) {
				p = CG_ConfigString(CS_LOCATIONS + ci->location);
				if (!p || !*p)
					p = "unknown";
				len = CG_DrawStrlen(p);
				if (len > lwidth)
					len = lwidth;

#ifdef IOQ3ZTM // FONT_REWRITE
				xx = x + TINYCHAR_WIDTH * 2 + pwidth;
#else
//				xx = x + TINYCHAR_WIDTH * 2 + TINYCHAR_WIDTH * pwidth + 
//					((lwidth/2 - len/2) * TINYCHAR_WIDTH);
				xx = x + TINYCHAR_WIDTH * 2 + TINYCHAR_WIDTH * pwidth;
#endif
				CG_DrawStringExt( xx, y,
					p, hcolor, qfalse, qfalse, TINYCHAR_WIDTH, TINYCHAR_HEIGHT,
					TEAM_OVERLAY_MAXLOCATION_WIDTH);
			} else {
				p = "";
			}

#ifdef TURTLEARENA // NOARMOR
			CG_GetColorForHealth( ci->health, hcolor );

			Com_sprintf (st, sizeof(st), "%3i", ci->health);
#else
			CG_GetColorForHealth( ci->health, ci->armor, hcolor );

			Com_sprintf (st, sizeof(st), "%3i %3i", ci->health,	ci->armor);
#endif

#ifdef IOQ3ZTM // FONT_REWRITE
			xx = x + TINYCHAR_WIDTH * 3 + pwidth + lwidth;
#else
			xx = x + TINYCHAR_WIDTH * 3 + 
				TINYCHAR_WIDTH * pwidth + TINYCHAR_WIDTH * lwidth;
#endif

			CG_DrawStringExt( xx, y,
				st, hcolor, qfalse, qfalse,
				TINYCHAR_WIDTH, TINYCHAR_HEIGHT, 0 );

			// draw weapon icon
			xx += TINYCHAR_WIDTH * 3;

#ifdef TA_WEAPSYS
			if ( cg_weapongroups[ci->curWeapon].weaponIcon ) {
				CG_DrawPic( xx, y, TINYCHAR_WIDTH, TINYCHAR_HEIGHT,
					cg_weapongroups[ci->curWeapon].weaponIcon );
			}
#else
			if ( cg_weapons[ci->curWeapon].weaponIcon ) {
				CG_DrawPic( xx, y, TINYCHAR_WIDTH, TINYCHAR_HEIGHT, 
					cg_weapons[ci->curWeapon].weaponIcon );
			}
#endif
			else {
				CG_DrawPic( xx, y, TINYCHAR_WIDTH, TINYCHAR_HEIGHT, 
					cgs.media.deferShader );
			}

			// Draw powerup icons
			if (right) {
				xx = x;
			} else {
				xx = x + w - TINYCHAR_WIDTH;
			}
			for (j = 0; j <= PW_NUM_POWERUPS; j++) {
				if (ci->powerups & (1 << j)) {

					item = BG_FindItemForPowerup( j );

					if (item) {
						CG_DrawPic( xx, y, TINYCHAR_WIDTH, TINYCHAR_HEIGHT, 
						trap_R_RegisterShader( item->icon ) );
						if (right) {
							xx -= TINYCHAR_WIDTH;
						} else {
							xx += TINYCHAR_WIDTH;
						}
					}
				}
			}

			y += TINYCHAR_HEIGHT;
		}
	}

	return ret_y;
//#endif
}


/*
=====================
CG_DrawUpperRight

=====================
*/
static void CG_DrawUpperRight(stereoFrame_t stereoFrame)
{
	float	y;

	y = 0;

	CG_HudPlacement(HUD_RIGHT);

	if ( cgs.gametype >= GT_TEAM && cg_drawTeamOverlay.integer == 1 ) {
		y = CG_DrawTeamOverlay( y, qtrue, qtrue );
	} 
	if ( cg_drawSnapshot.integer ) {
		y = CG_DrawSnapshot( y );
	}
	if (cg_drawFPS.integer && (stereoFrame == STEREO_CENTER || stereoFrame == STEREO_RIGHT)) {
		y = CG_DrawFPS( y );
	}
	if ( cg_drawTimer.integer ) {
		y = CG_DrawTimer( y );
	}
	if ( cg_drawAttacker.integer ) {
		y = CG_DrawAttacker( y );
	}

}

/*
===========================================================================================

  LOWER RIGHT CORNER

===========================================================================================
*/

/*
=================
CG_DrawScores

Draw the small two score display
=================
*/
#ifndef MISSIONPACK_HUD
static float CG_DrawScores( float y ) {
	const char	*s;
	int			s1, s2, score;
	int			x, w;
	int			v;
	vec4_t		color;
	float		y1;
	gitem_t		*item;

	s1 = cgs.scores1;
	s2 = cgs.scores2;

	y -=  BIGCHAR_HEIGHT + 8;

	y1 = y;

	// draw from the right side to left
	if ( cgs.gametype >= GT_TEAM ) {
		x = 640;
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
		color[3] = 0.33f;
		s = va( "%2i", s2 );
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
		x -= w;
		CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE ) {
			CG_DrawPic( x, y-4, w, BIGCHAR_HEIGHT+8, cgs.media.selectShader );
		}
		CG_DrawBigString( x + 4, y, s, 1.0F);

		if ( cgs.gametype == GT_CTF ) {
			// Display flag status
			item = BG_FindItemForPowerup( PW_BLUEFLAG );

			if (item) {
				y1 = y - BIGCHAR_HEIGHT - 8;
				if( cgs.blueflag >= 0 && cgs.blueflag <= 2 ) {
					CG_DrawPic( x, y1-4, w, BIGCHAR_HEIGHT+8, cgs.media.blueFlagShader[cgs.blueflag] );
				}
			}
		}
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 0.33f;
		s = va( "%2i", s1 );
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
		x -= w;
		CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED ) {
			CG_DrawPic( x, y-4, w, BIGCHAR_HEIGHT+8, cgs.media.selectShader );
		}
		CG_DrawBigString( x + 4, y, s, 1.0F);

		if ( cgs.gametype == GT_CTF ) {
			// Display flag status
			item = BG_FindItemForPowerup( PW_REDFLAG );

			if (item) {
				y1 = y - BIGCHAR_HEIGHT - 8;
				if( cgs.redflag >= 0 && cgs.redflag <= 2 ) {
					CG_DrawPic( x, y1-4, w, BIGCHAR_HEIGHT+8, cgs.media.redFlagShader[cgs.redflag] );
				}
			}
		}

		if ( cgs.gametype >= GT_CTF ) {
			v = cgs.capturelimit;
		} else {
			v = cgs.fraglimit;
		}
		if ( v ) {
			s = va( "%2i", v );
			w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
			x -= w;
			CG_DrawBigString( x + 4, y, s, 1.0F);
		}
#ifdef MISSIONPACK // ZTM: Support missionpack with old hud; oddly flagShader wasn't used
		if ( cgs.gametype == GT_1FCTF ) {
			// Display flag status
			item = BG_FindItemForPowerup( PW_NEUTRALFLAG );

			if (item) {
				y1 = y - BIGCHAR_HEIGHT - 8;
				if( cgs.flagStatus >= 0 && cgs.flagStatus <= 4 ) {
					vec4_t color = {1, 1, 1, 1};
					int index = 0;
					if (cgs.flagStatus == FLAG_TAKEN_RED) {
						color[1] = color[2] = 0;
						index = 1;
					} else if (cgs.flagStatus == FLAG_TAKEN_BLUE) {
						color[0] = color[1] = 0;
						index = 1;
					} else if (cgs.flagStatus == FLAG_DROPPED) {
						index = 2;
					}
					trap_R_SetColor(color);
					CG_DrawPic( x, y1-4, w, BIGCHAR_HEIGHT+8, cgs.media.flagShaders[index] );
					trap_R_SetColor(NULL);
				}
			}
		}
#endif

	} else
#ifdef TA_HUD // Not ffa or sp, but still in tournament?
	if (cgs.gametype == GT_TOURNAMENT)
#endif
	{
		qboolean	spectator;

		x = 640;
		score = cg.snap->ps.persistant[PERS_SCORE];
		spectator = ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR );

		// always show your score in the second box if not in first place
		if ( s1 != score ) {
			s2 = score;
		}
		if ( s2 != SCORE_NOT_PRESENT ) {
			s = va( "%2i", s2 );
			w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
			x -= w;
			if ( !spectator && score == s2 && score != s1 ) {
				color[0] = 1.0f;
				color[1] = 0.0f;
				color[2] = 0.0f;
				color[3] = 0.33f;
				CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
				CG_DrawPic( x, y-4, w, BIGCHAR_HEIGHT+8, cgs.media.selectShader );
			} else {
				color[0] = 0.5f;
				color[1] = 0.5f;
				color[2] = 0.5f;
				color[3] = 0.33f;
				CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
			}	
			CG_DrawBigString( x + 4, y, s, 1.0F);
		}

		// first place
		if ( s1 != SCORE_NOT_PRESENT ) {
			s = va( "%2i", s1 );
			w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
			x -= w;
			if ( !spectator && score == s1 ) {
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
				color[3] = 0.33f;
				CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
				CG_DrawPic( x, y-4, w, BIGCHAR_HEIGHT+8, cgs.media.selectShader );
			} else {
				color[0] = 0.5f;
				color[1] = 0.5f;
				color[2] = 0.5f;
				color[3] = 0.33f;
				CG_FillRect( x, y-4,  w, BIGCHAR_HEIGHT+8, color );
			}	
			CG_DrawBigString( x + 4, y, s, 1.0F);
		}

		if ( cgs.fraglimit ) {
			s = va( "%2i", cgs.fraglimit );
			w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH + 8;
			x -= w;
			CG_DrawBigString( x + 4, y, s, 1.0F);
		}

	}

	return y1 - 8;
}
#endif // MISSIONPACK_HUD

/*
================
CG_DrawPowerups
================
*/
#ifndef MISSIONPACK_HUD
static float CG_DrawPowerups( float y ) {
	int		sorted[MAX_POWERUPS];
	int		sortedTime[MAX_POWERUPS];
	int		i, j, k;
	int		active;
	playerState_t	*ps;
	int		t;
	gitem_t	*item;
	int		x;
	int		color;
	float	size;
	float	f;
	static float colors[2][4] = { 
    { 0.2f, 1.0f, 0.2f, 1.0f } , 
    { 1.0f, 0.2f, 0.2f, 1.0f } 
  };

	ps = &cg.snap->ps;

	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		return y;
	}

	// sort the list by time remaining
	active = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( !ps->powerups[ i ] ) {
			continue;
		}
#ifdef MISSIONPACK // IOQ3ZTM // ZTM: Skip persistant powerups!
		if (i == PW_SCOUT
			|| i == PW_GUARD
			|| i == PW_DOUBLER
			|| i == PW_AMMOREGEN)
		{
			continue;
		}
#endif
		t = ps->powerups[ i ] - cg.time;
		// ZOID--don't draw if the power up has unlimited time (999 seconds)
		// This is true of the CTF flags
		if ( t < 0 || t > 999000) {
			continue;
		}

		// insert into the list
		for ( j = 0 ; j < active ; j++ ) {
			if ( sortedTime[j] >= t ) {
				for ( k = active - 1 ; k >= j ; k-- ) {
					sorted[k+1] = sorted[k];
					sortedTime[k+1] = sortedTime[k];
				}
				break;
			}
		}
		sorted[j] = i;
		sortedTime[j] = t;
		active++;
	}

	// draw the icons and timers
	x = 640 - ICON_SIZE - CHAR_WIDTH * 2;
	for ( i = 0 ; i < active ; i++ ) {
		item = BG_FindItemForPowerup( sorted[i] );

    if (item) {

		  color = 1;

		  y -= ICON_SIZE;

		  trap_R_SetColor( colors[color] );
		  CG_DrawField( x, y, 2, sortedTime[ i ] / 1000 );

		  t = ps->powerups[ sorted[i] ];
		  if ( t - cg.time >= POWERUP_BLINKS * POWERUP_BLINK_TIME ) {
			  trap_R_SetColor( NULL );
		  } else {
			  vec4_t	modulate;

			  f = (float)( t - cg.time ) / POWERUP_BLINK_TIME;
			  f -= (int)f;
			  modulate[0] = modulate[1] = modulate[2] = modulate[3] = f;
			  trap_R_SetColor( modulate );
		  }

		  if ( cg.powerupActive == sorted[i] && 
			  cg.time - cg.powerupTime < PULSE_TIME ) {
			  f = 1.0 - ( ( (float)cg.time - cg.powerupTime ) / PULSE_TIME );
			  size = ICON_SIZE * ( 1.0 + ( PULSE_SCALE - 1.0 ) * f );
		  } else {
			  size = ICON_SIZE;
		  }

		  CG_DrawPic( 640 - size, y + ICON_SIZE / 2 - size / 2, 
			  size, size, trap_R_RegisterShader( item->icon ) );
    }
	}
	trap_R_SetColor( NULL );

	return y;
}
#endif // MISSIONPACK_HUD

/*
=====================
CG_DrawLowerRight

=====================
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawLowerRight( void ) {
	float	y;

	y = 480 - ICON_SIZE;

	CG_HudPlacement(HUD_RIGHT);

	if ( cgs.gametype >= GT_TEAM && cg_drawTeamOverlay.integer == 2 ) {
		y = CG_DrawTeamOverlay( y, qtrue, qfalse );
	} 

	y = CG_DrawScores( y );
	y = CG_DrawPowerups( y );
}
#endif // MISSIONPACK_HUD

/*
===================
CG_DrawPickupItem
===================
*/
#ifndef MISSIONPACK_HUD
static int CG_DrawPickupItem( int y ) {
	int		value;
	float	*fadeColor;
#ifdef TA_ITEMSYS
	gitem_t *item;
#endif

	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 ) {
		return y;
	}

	y -= ICON_SIZE;

	value = cg.itemPickup;
#ifdef TA_ITEMSYS
	item = BG_ItemForItemNum(value);
#endif
#ifdef TA_WEAPSYS
	if (item && item->giType == IT_WEAPON
		&& item->giTag == WP_DEFAULT)
	{
		item = BG_FindItemForWeapon(cgs.clientinfo[cg.snap->ps.clientNum].playercfg.default_weapon);
		value = ITEM_INDEX(item);
	}
#endif
	if ( value ) {
		fadeColor = CG_FadeColor( cg.itemPickupTime, 3000 );
		if ( fadeColor ) {
			CG_RegisterItemVisuals( value );
			trap_R_SetColor( fadeColor );
			CG_DrawPic( 8, y, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
#ifdef TA_ITEMSYS
			CG_DrawBigString( ICON_SIZE + 16, y + (ICON_SIZE/2 - BIGCHAR_HEIGHT/2), item->pickup_name, fadeColor[0] );
#else
			CG_DrawBigString( ICON_SIZE + 16, y + (ICON_SIZE/2 - BIGCHAR_HEIGHT/2), bg_itemlist[ value ].pickup_name, fadeColor[0] );
#endif
			trap_R_SetColor( NULL );
		}
	}
	
	return y;
}
#endif // MISSIONPACK_HUD

/*
=====================
CG_DrawLowerLeft

=====================
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawLowerLeft( void ) {
	float	y;

	CG_HudPlacement(HUD_LEFT);

	y = 480 - ICON_SIZE;

	if ( cgs.gametype >= GT_TEAM && cg_drawTeamOverlay.integer == 3 ) {
		y = CG_DrawTeamOverlay( y, qfalse, qfalse );
	} 


	y = CG_DrawPickupItem( y );
}
#endif // MISSIONPACK_HUD


//===========================================================================================

/*
=================
CG_DrawTeamInfo
=================
*/
#ifndef MISSIONPACK_HUD
static void CG_DrawTeamInfo( void ) {
	int w, h;
	int i, len;
	vec4_t		hcolor;
	int		chatHeight;

#define CHATLOC_Y 420 // bottom end
#define CHATLOC_X 0

	if (cg_teamChatHeight.integer < TEAMCHAT_HEIGHT)
		chatHeight = cg_teamChatHeight.integer;
	else
		chatHeight = TEAMCHAT_HEIGHT;
	if (chatHeight <= 0)
		return; // disabled

	if (cgs.teamLastChatPos != cgs.teamChatPos) {
		if (cg.time - cgs.teamChatMsgTimes[cgs.teamLastChatPos % chatHeight] > cg_teamChatTime.integer
#ifdef IOQ3ZTM // TEAM_CHAT_CON
			*1000
#endif
		) {
			cgs.teamLastChatPos++;
		}

		h = (cgs.teamChatPos - cgs.teamLastChatPos) * TINYCHAR_HEIGHT;

		w = 0;

		for (i = cgs.teamLastChatPos; i < cgs.teamChatPos; i++) {
			len = CG_DrawStrlen(cgs.teamChatMsgs[i % chatHeight]);
			if (len > w)
				w = len;
		}
		w *= TINYCHAR_WIDTH;
		w += TINYCHAR_WIDTH * 2;

		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED ) {
			hcolor[0] = 1.0f;
			hcolor[1] = 0.0f;
			hcolor[2] = 0.0f;
			hcolor[3] = 0.33f;
		} else if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE ) {
			hcolor[0] = 0.0f;
			hcolor[1] = 0.0f;
			hcolor[2] = 1.0f;
			hcolor[3] = 0.33f;
		} else {
			hcolor[0] = 0.0f;
			hcolor[1] = 1.0f;
			hcolor[2] = 0.0f;
			hcolor[3] = 0.33f;
		}

		trap_R_SetColor( hcolor );
		CG_DrawPic( CHATLOC_X, CHATLOC_Y - h, 640, h, cgs.media.teamStatusBar );
		trap_R_SetColor( NULL );

		hcolor[0] = hcolor[1] = hcolor[2] = 1.0f;
		hcolor[3] = 1.0f;

		for (i = cgs.teamChatPos - 1; i >= cgs.teamLastChatPos; i--) {
			CG_DrawStringExt( CHATLOC_X + TINYCHAR_WIDTH, 
				CHATLOC_Y - (cgs.teamChatPos - i)*TINYCHAR_HEIGHT, 
				cgs.teamChatMsgs[i % chatHeight], hcolor, qfalse, qfalse,
				TINYCHAR_WIDTH, TINYCHAR_HEIGHT, 0 );
		}
	}
}
#endif // MISSIONPACK_HUD

/*
===================
CG_DrawHoldableItem
===================
*/
#ifndef MISSIONPACK_HUD
#ifndef TA_HUD
static void CG_DrawHoldableItem( void ) { 
	int		value;

#ifdef TA_HOLDSYS
	value = BG_ItemNumForHoldableNum(cg.snap->ps.holdableIndex);
#else
	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];
#endif
	if ( value ) {
		CG_RegisterItemVisuals( value );
		CG_DrawPic( 640-ICON_SIZE, (SCREEN_HEIGHT-ICON_SIZE)/2, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
	}

}
#endif
#endif // MISSIONPACK_HUD

#ifdef MISSIONPACK
/*
===================
CG_DrawPersistantPowerup
===================
*/
//#if 0 // sos001208 - DEAD
#ifndef MISSIONPACK_HUD // IOQ3ZTM // ZTM: For playing MISSIONPACK without new HUD.
static void CG_DrawPersistantPowerup( void ) { 
	int		value;

	CG_HudPlacement(HUD_LEFT);

	value = cg.snap->ps.stats[STAT_PERSISTANT_POWERUP];
	if ( value ) {
		CG_RegisterItemVisuals( value );
#ifdef TA_HUD
		CG_DrawPic( HUD_X, (SCREEN_HEIGHT-ICON_SIZE)/2 - ICON_SIZE, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
#else
		CG_DrawPic( 640-ICON_SIZE, (SCREEN_HEIGHT-ICON_SIZE)/2 - ICON_SIZE, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
#endif
	}
}
#endif
#endif // MISSIONPACK


/*
===================
CG_DrawReward
===================
*/
static void CG_DrawReward( void ) { 
	float	*color;
	int		i, count;
	float	x, y;
	char	buf[32];

	CG_HudPlacement(HUD_CENTER);

	if ( !cg_drawRewards.integer ) {
		return;
	}

	color = CG_FadeColor( cg.rewardTime, REWARD_TIME );
	if ( !color ) {
		if (cg.rewardStack > 0) {
			for(i = 0; i < cg.rewardStack; i++) {
				cg.rewardSound[i] = cg.rewardSound[i+1];
				cg.rewardShader[i] = cg.rewardShader[i+1];
				cg.rewardCount[i] = cg.rewardCount[i+1];
			}
			cg.rewardTime = cg.time;
			cg.rewardStack--;
			color = CG_FadeColor( cg.rewardTime, REWARD_TIME );
			trap_S_StartLocalSound(cg.rewardSound[0], CHAN_ANNOUNCER);
		} else {
			return;
		}
	}

	trap_R_SetColor( color );

	/*
	count = cg.rewardCount[0]/10;				// number of big rewards to draw

	if (count) {
		y = 4;
		x = 320 - count * ICON_SIZE;
		for ( i = 0 ; i < count ; i++ ) {
			CG_DrawPic( x, y, (ICON_SIZE*2)-4, (ICON_SIZE*2)-4, cg.rewardShader[0] );
			x += (ICON_SIZE*2);
		}
	}

	count = cg.rewardCount[0] - count*10;		// number of small rewards to draw
	*/

	if ( cg.rewardCount[0] >= 0 ) {
		y = 56;
		x = 320 - ICON_SIZE/2;
		CG_DrawPic( x, y, ICON_SIZE-4, ICON_SIZE-4, cg.rewardShader[0] );
		Com_sprintf(buf, sizeof(buf), "%d", cg.rewardCount[0]);
#ifdef IOQ3ZTM // FONT_REWRITE
		CG_DrawBigStringColor(CENTER_X, y+ICON_SIZE, buf, color);
#else
		CG_DrawStringExt( CENTER_X, y+ICON_SIZE, buf, color, qfalse, qtrue,
								SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0 );
#endif
	}
	else {

		count = cg.rewardCount[0];

		y = 56;
		x = 320 - count * ICON_SIZE/2;
		for ( i = 0 ; i < count ; i++ ) {
			CG_DrawPic( x, y, ICON_SIZE-4, ICON_SIZE-4, cg.rewardShader[0] );
			x += ICON_SIZE;
		}
	}
	trap_R_SetColor( NULL );
}


/*
===============================================================================

LAGOMETER

===============================================================================
*/

#define	LAG_SAMPLES		128


typedef struct {
	int		frameSamples[LAG_SAMPLES];
	int		frameCount;
	int		snapshotFlags[LAG_SAMPLES];
	int		snapshotSamples[LAG_SAMPLES];
	int		snapshotCount;
} lagometer_t;

lagometer_t		lagometer;

/*
==============
CG_AddLagometerFrameInfo

Adds the current interpolate / extrapolate bar for this frame
==============
*/
void CG_AddLagometerFrameInfo( void ) {
	int			offset;

	offset = cg.time - cg.latestSnapshotTime;
	lagometer.frameSamples[ lagometer.frameCount & ( LAG_SAMPLES - 1) ] = offset;
	lagometer.frameCount++;
}

/*
==============
CG_AddLagometerSnapshotInfo

Each time a snapshot is received, log its ping time and
the number of snapshots that were dropped before it.

Pass NULL for a dropped packet.
==============
*/
void CG_AddLagometerSnapshotInfo( snapshot_t *snap ) {
	// dropped packet
	if ( !snap ) {
		lagometer.snapshotSamples[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = -1;
		lagometer.snapshotCount++;
		return;
	}

	// add this snapshot's info
	lagometer.snapshotSamples[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->ping;
	lagometer.snapshotFlags[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->snapFlags;
	lagometer.snapshotCount++;
}

/*
==============
CG_DrawDisconnect

Should we draw something differnet for long lag vs no packets?
==============
*/
static void CG_DrawDisconnect( void ) {
	float		x, y;
	int			cmdNum;
	usercmd_t	cmd;
	const char		*s;

	CG_HudPlacement(HUD_CENTER);

	// draw the phone jack if we are completely past our buffers
	cmdNum = trap_GetCurrentCmdNumber() - CMD_BACKUP + 1;
	trap_GetUserCmd( cmdNum, &cmd );
	if ( cmd.serverTime <= cg.snap->ps.commandTime
		|| cmd.serverTime > cg.time ) {	// special check for map_restart
		return;
	}

	// also add text in center of screen
	s = "Connection Interrupted";
	CG_DrawBigString( CENTER_X, 100, s, 1.0F);

	// blink the icon
	if ( ( cg.time >> 9 ) & 1 ) {
		return;
	}

	CG_HudPlacement(HUD_RIGHT);

	x = 640 - 48;
	y = 480 - 48;

	CG_DrawPic( x, y, 48, 48, trap_R_RegisterShader("gfx/2d/net.tga" ) );
}


#define	MAX_LAGOMETER_PING	900
#define	MAX_LAGOMETER_RANGE	300

/*
==============
CG_DrawLagometer
==============
*/
static void CG_DrawLagometer( void ) {
	int		a, x, y, i;
	float	v;
	float	ax, ay, aw, ah, mid, range;
	int		color;
	float	vscale;

	if ( !cg_lagometer.integer || cgs.localServer ) {
		CG_DrawDisconnect();
		return;
	}

	CG_HudPlacement(HUD_RIGHT);

	//
	// draw the graph
	//
#ifdef MISSIONPACK_HUD
	x = 640 - 48;
	y = 480 - 144;
#else
	x = 640 - 48;
	y = 480 - 48;
#endif

	trap_R_SetColor( NULL );
	CG_DrawPic( x, y, 48, 48, cgs.media.lagometerShader );

	ax = x;
	ay = y;
	aw = 48;
	ah = 48;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	color = -1;
	range = ah / 3;
	mid = ay + range;

	vscale = range / MAX_LAGOMETER_RANGE;

	// draw the frame interpoalte / extrapolate graph
	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.frameCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.frameSamples[i];
		v *= vscale;
		if ( v > 0 ) {
			if ( color != 1 ) {
				color = 1;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
			}
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic ( ax + aw - a, mid - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 2 ) {
				color = 2;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_BLUE)] );
			}
			v = -v;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, mid, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	// draw the snapshot latency / drop graph
	range = ah / 2;
	vscale = range / MAX_LAGOMETER_PING;

	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.snapshotCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.snapshotSamples[i];
		if ( v > 0 ) {
			if ( lagometer.snapshotFlags[i] & SNAPFLAG_RATE_DELAYED ) {
				if ( color != 5 ) {
					color = 5;	// YELLOW for rate delay
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
				}
			} else {
				if ( color != 3 ) {
					color = 3;
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_GREEN)] );
				}
			}
			v = v * vscale;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 4 ) {
				color = 4;		// RED for dropped snapshots
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_RED)] );
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - range, 1, range, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	trap_R_SetColor( NULL );

	if ( cg_nopredict.integer || cg_synchronousClients.integer ) {
		CG_DrawBigString( ax, ay, "snc", 1.0 );
	}

	CG_DrawDisconnect();
}



/*
===============================================================================

CENTER PRINTING

===============================================================================
*/


/*
==============
CG_CenterPrint

Called for important messages that should stay in the center of the screen
for a few moments
==============
*/
void CG_CenterPrint( const char *str, int y, int charWidth ) {
	char	*s;

	Q_strncpyz( cg.centerPrint, str, sizeof(cg.centerPrint) );

	cg.centerPrintTime = cg.time;
	cg.centerPrintY = y;
#ifndef MISSIONPACK_HUD2
	cg.centerPrintCharWidth = charWidth;
#endif

	// count the number of lines for centering
	cg.centerPrintLines = 1;
	s = cg.centerPrint;
	while( *s ) {
		if (*s == '\n')
			cg.centerPrintLines++;
		s++;
	}
}


/*
===================
CG_DrawCenterString
===================
*/
static void CG_DrawCenterString( void ) {
	char	*start;
	int		l;
	int		y;
#ifdef MISSIONPACK_HUD2
	int		h;
#endif
	float	*color;

	if ( !cg.centerPrintTime ) {
		return;
	}

	color = CG_FadeColor( cg.centerPrintTime, 1000 * cg_centertime.value );
	if ( !color ) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	trap_R_SetColor( color );

	start = cg.centerPrint;

	y = cg.centerPrintY - cg.centerPrintLines * BIGCHAR_HEIGHT / 2;

	while ( 1 ) {
		char linebuffer[1024];

		for ( l = 0; l < 50; l++ ) {
			if ( !start[l] || start[l] == '\n' ) {
				break;
			}
			linebuffer[l] = start[l];
		}
		linebuffer[l] = 0;

#ifdef MISSIONPACK_HUD2
		h = CG_Text_Height(linebuffer, 0.5, 0);
		CG_Text_Paint(CENTER_X, y + h, 0.5, color, linebuffer, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
		y += h + 6;
#else
		CG_DrawStringExt( CENTER_X, y, linebuffer, color, qfalse, qtrue,
			cg.centerPrintCharWidth, (int)(cg.centerPrintCharWidth * 1.5), 0 );

		y += cg.centerPrintCharWidth * 1.5;
#endif
		while ( *start && ( *start != '\n' ) ) {
			start++;
		}
		if ( !*start ) {
			break;
		}
		start++;
	}

	trap_R_SetColor( NULL );
}



/*
================================================================================

CROSSHAIR

================================================================================
*/


/*
=================
CG_DrawCrosshair
=================
*/
static void CG_DrawCrosshair(void)
{
	float		w, h;
	qhandle_t	hShader;
	float		f;
	float		x, y;
	int			ca;

	if ( !cg_drawCrosshair.integer ) {
		return;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		return;
	}

	if ( cg.renderingThirdPerson ) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	// set color based on health
	if ( cg_crosshairHealth.integer ) {
		vec4_t		hcolor;

		CG_ColorForHealth( hcolor );
		trap_R_SetColor( hcolor );
	} else {
		trap_R_SetColor( NULL );
	}

	w = h = cg_crosshairSize.value;

	// pulse the size of the crosshair when picking up items
	f = cg.time - cg.itemPickupBlendTime;
	if ( f > 0 && f < ITEM_BLOB_TIME ) {
		f /= ITEM_BLOB_TIME;
		w *= ( 1 + f );
		h *= ( 1 + f );
	}

	x = cg_crosshairX.integer;
	y = cg_crosshairY.integer;
#ifdef IOQ3ZTM // HUD_ASPECT_CORRECT
	CG_AdjustFrom640Fit( &x, &y, &w, &h );
#else
	CG_AdjustFrom640( &x, &y, &w, &h );
#endif

	ca = cg_drawCrosshair.integer;
	if (ca < 0) {
		ca = 0;
	}
	hShader = cgs.media.crosshairShader[ ca % NUM_CROSSHAIRS ];

	trap_R_DrawStretchPic( x + cg.refdef.x + 0.5 * (cg.refdef.width - w), 
		y + cg.refdef.y + 0.5 * (cg.refdef.height - h), 
		w, h, 0, 0, 1, 1, hShader );
}

/*
=================
CG_DrawCrosshair3D
=================
*/
static void CG_DrawCrosshair3D(void)
{
	float		w, h;
	qhandle_t	hShader;
	float		f;
	int			ca;

	trace_t trace;
	vec3_t endpos;
	float stereoSep, zProj, maxdist, xmax;
	char rendererinfos[128];
	refEntity_t ent;

	if ( !cg_drawCrosshair.integer ) {
		return;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		return;
	}

	if ( cg.renderingThirdPerson ) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	w = h = cg_crosshairSize.value;

	// pulse the size of the crosshair when picking up items
	f = cg.time - cg.itemPickupBlendTime;
	if ( f > 0 && f < ITEM_BLOB_TIME ) {
		f /= ITEM_BLOB_TIME;
		w *= ( 1 + f );
		h *= ( 1 + f );
	}

	ca = cg_drawCrosshair.integer;
	if (ca < 0) {
		ca = 0;
	}
	hShader = cgs.media.crosshairShader[ ca % NUM_CROSSHAIRS ];

	// Use a different method rendering the crosshair so players don't see two of them when
	// focusing their eyes at distant objects with high stereo separation
	// We are going to trace to the next shootable object and place the crosshair in front of it.

	// first get all the important renderer information
	trap_Cvar_VariableStringBuffer("r_zProj", rendererinfos, sizeof(rendererinfos));
	zProj = atof(rendererinfos);
	trap_Cvar_VariableStringBuffer("r_stereoSeparation", rendererinfos, sizeof(rendererinfos));
	stereoSep = zProj / atof(rendererinfos);
	
	xmax = zProj * tan(cg.refdef.fov_x * M_PI / 360.0f);
	
	// let the trace run through until a change in stereo separation of the crosshair becomes less than one pixel.
	maxdist = cgs.glconfig.vidWidth * stereoSep * zProj / (2 * xmax);
	VectorMA(cg.refdef.vieworg, maxdist, cg.refdef.viewaxis[0], endpos);
	CG_Trace(&trace, cg.refdef.vieworg, NULL, NULL, endpos, 0, MASK_SHOT);
	
	memset(&ent, 0, sizeof(ent));
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_DEPTHHACK | RF_CROSSHAIR;
	
	VectorCopy(trace.endpos, ent.origin);
	
	// scale the crosshair so it appears the same size for all distances
	ent.radius = w / 640 * xmax * trace.fraction * maxdist / zProj;
	ent.customShader = hShader;

	trap_R_AddRefEntityToScene(&ent);
}



/*
=================
CG_ScanForCrosshairEntity
=================
*/
static void CG_ScanForCrosshairEntity( void ) {
	trace_t		trace;
	vec3_t		start, end;
	int			content;

	VectorCopy( cg.refdef.vieworg, start );
	VectorMA( start, 131072, cg.refdef.viewaxis[0], end );

	CG_Trace( &trace, start, vec3_origin, vec3_origin, end, 
		cg.snap->ps.clientNum, CONTENTS_SOLID|CONTENTS_BODY );
	if ( trace.entityNum >= MAX_CLIENTS ) {
		return;
	}

	// if the player is in fog, don't show it
	content = trap_CM_PointContents( trace.endpos, 0 );
	if ( content & CONTENTS_FOG ) {
		return;
	}

	// if the player is invisible, don't show it
	if ( cg_entities[ trace.entityNum ].currentState.powerups & ( 1 << PW_INVIS ) ) {
		return;
	}

	// update the fade timer
	cg.crosshairClientNum = trace.entityNum;
	cg.crosshairClientTime = cg.time;
}


/*
=====================
CG_DrawCrosshairNames
=====================
*/
static void CG_DrawCrosshairNames( void ) {
	float		*color;
	char		*name;

	if ( !cg_drawCrosshair.integer ) {
		return;
	}
	if ( !cg_drawCrosshairNames.integer ) {
		return;
	}
	if ( cg.renderingThirdPerson ) {
		return;
	}

	// scan the known entities to see if the crosshair is sighted on one
	CG_ScanForCrosshairEntity();

	// draw the name of the player being looked at
	color = CG_FadeColor( cg.crosshairClientTime, 1000 );
	if ( !color ) {
		trap_R_SetColor( NULL );
		return;
	}

	name = cgs.clientinfo[ cg.crosshairClientNum ].name;
#ifdef MISSIONPACK_HUD2
	color[3] *= 0.5f;
	CG_Text_Paint( CENTER_X, 190, 0.3f, color, name, 0, 0, ITEM_TEXTSTYLE_SHADOWED);
#else
	CG_DrawBigString( CENTER_X, 170, name, color[3] * 0.5f );
#endif
	trap_R_SetColor( NULL );
}


//==============================================================================

/*
=================
CG_DrawSpectator
=================
*/
static void CG_DrawSpectator(void) {
	CG_HudPlacement(HUD_CENTER);
	CG_DrawBigString(CENTER_X, 440, "SPECTATOR", 1.0F);
	if ( cgs.gametype == GT_TOURNAMENT ) {
		CG_DrawBigString(CENTER_X, 460, "waiting to play", 1.0F);
	}
	else if ( cgs.gametype >= GT_TEAM ) {
#ifdef TA_MISC // SMART_JOIN_MENU
		CG_DrawBigString(CENTER_X, 460, "press ESC and use the TEAM menu to play", 1.0F);
#elif defined IOQ3ZTM
		CG_DrawBigString(CENTER_X, 460, "press ESC and use the START menu to play", 1.0F);
#else
		CG_DrawBigString(CENTER_X, 460, "press ESC and use the JOIN menu to play", 1.0F);
#endif
	}
}

/*
=================
CG_DrawVote
=================
*/
static void CG_DrawVote(void) {
	char	*s;
	int		sec;
	float	y;

	if ( !cgs.voteTime ) {
		return;
	}

	CG_HudPlacement(HUD_LEFT);

	// play a talk beep whenever it is modified
	if ( cgs.voteModified ) {
		cgs.voteModified = qfalse;
		trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
	}

	sec = ( VOTE_TIME - ( cg.time - cgs.voteTime ) ) / 1000;
	if ( sec < 0 ) {
		sec = 0;
	}
#ifdef TURTLEARENA
	y = 304;
#else
	y = 58;
#endif
#ifdef MISSIONPACK_HUD // ZTM: TODO: Add vote to Q3 ingame menu?
	s = va("VOTE(%i):%s yes:%i no:%i", sec, cgs.voteString, cgs.voteYes, cgs.voteNo);
	CG_DrawSmallString( 0, y, s, 1.0F );
	s = "or press ESC then click Vote";
	CG_DrawSmallString( 0, y + SMALLCHAR_HEIGHT + 2, s, 1.0F );
#else
	s = va("VOTE(%i):%s yes:%i no:%i", sec, cgs.voteString, cgs.voteYes, cgs.voteNo );
	CG_DrawSmallString( 0, y, s, 1.0F );
#endif
}

/*
=================
CG_DrawTeamVote
=================
*/
static void CG_DrawTeamVote(void) {
	char	*s;
	int		sec, cs_offset;
	float	y;

	if ( cgs.clientinfo->team == TEAM_RED )
		cs_offset = 0;
	else if ( cgs.clientinfo->team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !cgs.teamVoteTime[cs_offset] ) {
		return;
	}

	CG_HudPlacement(HUD_LEFT);

	// play a talk beep whenever it is modified
	if ( cgs.teamVoteModified[cs_offset] ) {
		cgs.teamVoteModified[cs_offset] = qfalse;
		trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
	}

	sec = ( VOTE_TIME - ( cg.time - cgs.teamVoteTime[cs_offset] ) ) / 1000;
	if ( sec < 0 ) {
		sec = 0;
	}
#ifdef TURTLEARENA
	y = 336;
#else
	y = 90;
#endif
	s = va("TEAMVOTE(%i):%s yes:%i no:%i", sec, cgs.teamVoteString[cs_offset],
							cgs.teamVoteYes[cs_offset], cgs.teamVoteNo[cs_offset] );
	CG_DrawSmallString( 0, y, s, 1.0F );
}


static qboolean CG_DrawScoreboard( void ) {
#ifdef MISSIONPACK_HUD
	static qboolean firstTime = qtrue;
	float fade, *fadeColor;

	CG_HudPlacement(HUD_CENTER);

	if (menuScoreboard) {
		menuScoreboard->window.flags &= ~WINDOW_FORCED;
	}
	if (cg_paused.integer) {
		cg.deferredPlayerLoading = 0;
		firstTime = qtrue;
		return qfalse;
	}

	// should never happen in Team Arena
	if (cgs.gametype == GT_SINGLE_PLAYER && cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		cg.deferredPlayerLoading = 0;
		firstTime = qtrue;
		return qfalse;
	}

	// don't draw scoreboard during death while warmup up
	if ( cg.warmup && !cg.showScores ) {
		return qfalse;
	}

	if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD || cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		fade = 1.0;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor( cg.scoreFadeTime, FADE_TIME );
		if ( !fadeColor ) {
			// next time scoreboard comes up, don't print killer
			cg.deferredPlayerLoading = 0;
			cg.killerName[0] = 0;
			firstTime = qtrue;
			return qfalse;
		}
		fade = *fadeColor;
	}																					  


	if (menuScoreboard == NULL) {
		if ( cgs.gametype >= GT_TEAM ) {
			menuScoreboard = Menus_FindByName("teamscore_menu");
		} else {
			menuScoreboard = Menus_FindByName("score_menu");
		}
	}

	if (menuScoreboard) {
		if (firstTime) {
			CG_SetScoreSelection(menuScoreboard);
			firstTime = qfalse;
		}
		Menu_Paint(menuScoreboard, qtrue);
	}

	// load any models that have been deferred
	if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}

	return qtrue;
#else
	return CG_DrawOldScoreboard();
#endif
}

#ifdef TA_SP
/*
=================
CG_DrawIntermission
=================
*/
static void CG_DrawSPIntermission( void ) {
	char		str[1024];
	char		*name;
	vec4_t		color;

	if (!cg.snap->ps.persistant[PERS_LIVES] && !cg.snap->ps.persistant[PERS_CONTINUES])
	{
		return;
	}

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	CG_HudPlacement(HUD_CENTER);

	if (cg_singlePlayerActive.integer) {
		name = cgs.clientinfo[ cg.snap->ps.clientNum ].headModelName;
		if (name[0] == '*') {
			name++;
		}
	} else {
		name = cgs.clientinfo[ cg.snap->ps.clientNum ].name;
	}

	Q_snprintf(str, sizeof (str), "%s got though the level", name);

#ifdef MISSIONPACK_HUD2
	CG_Text_Paint( CENTER_X, 210, 0.3f, color, str, 0, 0, ITEM_TEXTSTYLE_SHADOWED);
#else
	CG_DrawBigString( CENTER_X, 210, str, color[3] );
#endif
	trap_R_SetColor( NULL );
}
#endif

/*
=================
CG_DrawIntermission
=================
*/
static void CG_DrawIntermission( void ) {
//	int key;
#if defined MISSIONPACK_HUD && !defined TA_SP
	//if (cg_singlePlayer.integer) {
	//	CG_DrawCenterString();
	//	return;
	//}
#else
	if ( cgs.gametype == GT_SINGLE_PLAYER ) {
#ifdef TA_SP
		CG_DrawSPIntermission();
#endif
		CG_DrawCenterString();
		return;
	}
#endif
	cg.scoreFadeTime = cg.time;
	cg.scoreBoardShowing = CG_DrawScoreboard();
}

/*
=================
CG_DrawFollow
=================
*/
static qboolean CG_DrawFollow( void ) {
	vec4_t		color;
	const char	*name;

	if ( !(cg.snap->ps.pm_flags & PMF_FOLLOW) ) {
		return qfalse;
	}
	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	CG_HudPlacement(HUD_CENTER);

	CG_DrawBigString( CENTER_X, 24, "following", 1.0F );

	name = cgs.clientinfo[ cg.snap->ps.clientNum ].name;

	CG_DrawGiantString(CENTER_X, 40, name, 1.0F );

	return qtrue;
}

#ifdef TA_ENTSYS // FUNC_USE
/*
=================
CG_DrawUseEntity
=================
*/
static qboolean CG_DrawUseEntity(void)
{
	const char *s;
	float		h;
	float		w;
	float		x;
	float		y;

	if (!(cg.snap->ps.eFlags & EF_USE_ENT))
		return qfalse;

	CG_HudPlacement(HUD_CENTER);

	s = "Use Entity!";

#ifdef IOQ3ZTM // FONT_REWRITE
	h = Com_FontStringHeight(&cgs.media.fontBig, s, 0);
	w = Com_FontStringWidth(&cgs.media.fontBig, s, 0);
#else
	h = BIGCHAR_HEIGHT
	w = BIGCHAR_WIDTH * CG_DrawStrlen( s );
#endif
	x = ( SCREEN_WIDTH - w ) * 0.5f;
	y = SCREEN_HEIGHT-h-12;

	CG_DrawTeamBackground(x - 6, y - 6, w + 6*2, h + 6*2, 0.33f, cg.snap->ps.persistant[PERS_TEAM], cg.snap->ps.clientNum);

	CG_DrawBigString(x, y, s, 1);

	return qtrue;
}
#endif



#ifndef TURTLEARENA // NO_AMMO_WARNINGS
/*
=================
CG_DrawAmmoWarning
=================
*/
static void CG_DrawAmmoWarning( void ) {
	const char	*s;

	if ( cg_drawAmmoWarning.integer == 0 ) {
		return;
	}

	if ( !cg.lowAmmoWarning ) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	if ( cg.lowAmmoWarning == 2 ) {
		s = "OUT OF AMMO";
	} else {
		s = "LOW AMMO WARNING";
	}
	CG_DrawBigString(CENTER_X, 64, s, 1.0F);
}
#endif

#if defined MISSIONPACK && !defined TURTLEARENA // WEAPONS
/*
=================
CG_DrawProxWarning
=================
*/
static void CG_DrawProxWarning( void ) {
	char s [32];
  static int proxTime;
  static int proxCounter;
  static int proxTick;

	if( !(cg.snap->ps.eFlags & EF_TICKING ) ) {
    proxTime = 0;
		return;
	}

	CG_HudPlacement(HUD_CENTER);

  if (proxTime == 0) {
    proxTime = cg.time + 5000;
    proxCounter = 5;
    proxTick = 0;
  }

  if (cg.time > proxTime) {
    proxTick = proxCounter--;
    proxTime = cg.time + 1000;
  }

  if (proxTick != 0) {
    Com_sprintf(s, sizeof(s), "INTERNAL COMBUSTION IN: %i", proxTick);
  } else {
    Com_sprintf(s, sizeof(s), "YOU HAVE BEEN MINED");
  }

	CG_DrawBigStringColor( CENTER_X, 64 + BIGCHAR_HEIGHT, s, g_color_table[ColorIndex(COLOR_RED)] );
}
#endif


/*
=================
CG_DrawWarmup
=================
*/
static void CG_DrawWarmup( void ) {
	int			sec;
	int			i;
	float scale;
	clientInfo_t	*ci1, *ci2;
#ifndef MISSIONPACK_HUD2
	int			w;
#endif
	int			cw;
	const char	*s;

	sec = cg.warmup;
	if ( !sec ) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	if ( sec < 0 ) {
		s = "Waiting for players";
		CG_DrawBigString(CENTER_X, 24, s, 1.0F);
		cg.warmupCount = 0;
		return;
	}

	if (cgs.gametype == GT_TOURNAMENT) {
		// find the two active players
		ci1 = NULL;
		ci2 = NULL;
		for ( i = 0 ; i < cgs.maxclients ; i++ ) {
			if ( cgs.clientinfo[i].infoValid && cgs.clientinfo[i].team == TEAM_FREE ) {
				if ( !ci1 ) {
					ci1 = &cgs.clientinfo[i];
				} else {
					ci2 = &cgs.clientinfo[i];
				}
			}
		}

		if ( ci1 && ci2 ) {
			s = va( "%s vs %s", ci1->name, ci2->name );
#ifdef MISSIONPACK_HUD2
			CG_Text_Paint(CENTER_X, 60, 0.6f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
#else
			w = CG_DrawStrlen( s );
			if ( w > 640 / GIANT_WIDTH ) {
				cw = 640 / w;
			} else {
				cw = GIANT_WIDTH;
			}
			CG_DrawStringExt( 320 - w * cw/2, 20,s, colorWhite, 
					qfalse, qtrue, cw, (int)(cw * 1.5f), 0 );
#endif
		}
	} else {
		if ( cgs.gametype == GT_FFA ) {
			s = "Free For All";
		} else if ( cgs.gametype == GT_TEAM ) {
			s = "Team Deathmatch";
		} else if ( cgs.gametype == GT_CTF ) {
			s = "Capture the Flag";
#ifdef MISSIONPACK
		} else if ( cgs.gametype == GT_1FCTF ) {
			s = "One Flag CTF";
		} else if ( cgs.gametype == GT_OBELISK ) {
			s = "Overload";
#ifdef MISSIONPACK_HARVESTER
		} else if ( cgs.gametype == GT_HARVESTER ) {
			s = "Harvester";
#endif
#endif
		} else {
			s = "";
		}
#ifdef MISSIONPACK_HUD2
		CG_Text_Paint(CENTER_X, 90, 0.6f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
#else
		w = CG_DrawStrlen( s );
		if ( w > 640 / GIANT_WIDTH ) {
			cw = 640 / w;
		} else {
			cw = GIANT_WIDTH;
		}
		CG_DrawStringExt( CENTER_X, 25,s, colorWhite, 
				qfalse, qtrue, cw, (int)(cw * 1.1f), 0 );
#endif
	}

	sec = ( sec - cg.time ) / 1000;
	if ( sec < 0 ) {
		cg.warmup = 0;
		sec = 0;
	}
	s = va( "Starts in: %i", sec + 1 );
	if ( sec != cg.warmupCount ) {
		cg.warmupCount = sec;
		switch ( sec ) {
		case 0:
			trap_S_StartLocalSound( cgs.media.count1Sound, CHAN_ANNOUNCER );
			break;
		case 1:
			trap_S_StartLocalSound( cgs.media.count2Sound, CHAN_ANNOUNCER );
			break;
		case 2:
			trap_S_StartLocalSound( cgs.media.count3Sound, CHAN_ANNOUNCER );
			break;
		default:
			break;
		}
	}
	scale = 0.45f;
	switch ( cg.warmupCount ) {
	case 0:
		cw = 28;
		scale = 0.54f;
		break;
	case 1:
		cw = 24;
		scale = 0.51f;
		break;
	case 2:
		cw = 20;
		scale = 0.48f;
		break;
	default:
		cw = 16;
		scale = 0.45f;
		break;
	}

#ifdef MISSIONPACK_HUD2
	CG_Text_Paint(CENTER_X, 125, scale, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
#else
	CG_DrawStringExt( CENTER_X, 70, s, colorWhite, 
			qfalse, qtrue, cw, (int)(cw * 1.5), 0 );
#endif
}

//==================================================================================
#ifdef MISSIONPACK_HUD
/* 
=================
CG_DrawTimedMenus
=================
*/
void CG_DrawTimedMenus( void ) {
	if (cg.voiceTime) {
		int t = cg.time - cg.voiceTime;
		if ( t > 2500 ) {
			Menus_CloseByName("voiceMenu");
#ifdef IOQ3ZTM // USE_FREETYPE
			trap_Cvar_Set("cl_conXOffset", "10");
#else
			trap_Cvar_Set("cl_conXOffset", "0");
#endif
			cg.voiceTime = 0;
		}
	}
}
#endif
#ifdef TA_SP
/*
=================
CG_DrawGameOver
=================
*/
void CG_DrawGameOver(void)
{
	playerState_t	*ps;

	if (cgs.gametype != GT_SINGLE_PLAYER) {
		return;
	}

	CG_HudPlacement(HUD_CENTER);

	ps = &cg.snap->ps;

	// Show "Game Over"!
	if (!ps->persistant[PERS_LIVES] && !ps->persistant[PERS_CONTINUES])
	{
		int	y;
#ifndef MISSIONPACK_HUD2
		int charWidth;
#endif
		vec4_t color = {1,1,1,1};
		const char *str = "Game Over";

#ifdef MISSIONPACK_HUD2
		y = 120 + CG_Text_Height(str, 0.5, 0) / 2;
		CG_Text_Paint(CENTER_X, y, 0.5, color, str, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
#else
		charWidth = GIANTCHAR_WIDTH*2;
		y = 120 + GIANTCHAR_HEIGHT / 2;

		CG_DrawStringExt( CENTER_X, y, str, color, qfalse, qtrue,
			charWidth, (int)(charWidth * 1.5), 0 );
#endif
	}
}
#endif
/*
=================
CG_Draw2D
=================
*/
static void CG_Draw2D(stereoFrame_t stereoFrame)
{
#ifdef MISSIONPACK
	if (cgs.orderPending && cg.time > cgs.orderTime) {
		CG_CheckOrderPending();
	}
#endif
	// if we are taking a levelshot for the menu, don't draw anything
	if ( cg.levelShot ) {
		return;
	}

	if ( cg_draw2D.integer == 0 ) {
		return;
	}

	if ( cg.snap->ps.pm_type == PM_INTERMISSION
#ifdef TA_SP
		|| cg.snap->ps.pm_type == PM_SPINTERMISSION
#endif
	) {
		CG_DrawIntermission();
		return;
	}

#ifdef CAMERASCRIPT
	if (cg.cameraMode) {
		return;
	}
#else
/*
	if (cg.cameraMode) {
		return;
	}
*/
#endif
	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
		CG_DrawSpectator();

		if(stereoFrame == STEREO_CENTER)
			CG_DrawCrosshair();

		CG_DrawCrosshairNames();
	} else {
		// don't draw any status if dead or the scoreboard is being explicitly shown
		if (
#ifndef TA_HUD
		!cg.showScores &&
#endif
		cg.snap->ps.stats[STAT_HEALTH] > 0 ) {

#ifdef MISSIONPACK_HUD
			if ( cg_drawStatus.integer ) {
				Menu_PaintAll();
				CG_DrawTimedMenus();
			}
#else
			CG_DrawStatusBar();
#endif
      
#if defined TA_WEAPSYS || defined IOQ3ZTM // SHOW_SPEED
			CG_DrawMiddleLeft();
#endif

#ifdef TURTLEARENA // NIGHTS_ITEMS
			CG_DrawScoreChain();
#endif

#ifndef TURTLEARENA // NO_AMMO_WARNINGS
			CG_DrawAmmoWarning();
#endif

#if defined MISSIONPACK && !defined TURTLEARENA // WEAPONS
			CG_DrawProxWarning();
#endif      
			if(stereoFrame == STEREO_CENTER)
				CG_DrawCrosshair();
			CG_DrawCrosshairNames();
#ifndef TA_WEAPSYS_EX
			CG_DrawWeaponSelect();
#endif
#ifndef MISSIONPACK_HUD
#ifndef TA_HUD
			CG_DrawHoldableItem();
#endif
#ifdef MISSIONPACK // IOQ3ZTM // ZTM: For playing MISSIONPACK without new HUD.
			CG_DrawPersistantPowerup();
#endif
#endif
			CG_DrawReward();

#ifdef TA_ENTSYS // FUNC_USE
			CG_DrawUseEntity();
#endif
		}
    
#ifndef MISSIONPACK_HUD
		if ( cgs.gametype >= GT_TEAM ) {
			CG_DrawTeamInfo();
		}
#endif
	}

	CG_DrawVote();
	CG_DrawTeamVote();

	CG_DrawLagometer();

#ifdef MISSIONPACK
	if (!cg_paused.integer) {
		CG_DrawUpperRight(stereoFrame);
	}
#else
	CG_DrawUpperRight(stereoFrame);
#endif

#ifndef MISSIONPACK_HUD
	CG_DrawLowerRight();
	CG_DrawLowerLeft();
#endif

	if ( !CG_DrawFollow() ) {
		CG_DrawWarmup();
	}

	// don't draw center string if scoreboard is up
	cg.scoreBoardShowing = CG_DrawScoreboard();
	if ( !cg.scoreBoardShowing) {
#ifdef TA_SP
		CG_DrawGameOver();
#endif
		CG_DrawCenterString();
	}
#ifdef CAMERASCRIPT
	CG_DrawFlashFade();
#endif
}


static void CG_DrawTourneyScoreboard( void ) {
#ifdef MISSIONPACK_HUD
#else
	CG_DrawOldTourneyScoreboard();
#endif
}

/*
=====================
CG_DrawActive

Perform all drawing needed to completely fill the screen
=====================
*/
void CG_DrawActive( stereoFrame_t stereoView ) {
	// optionally draw the info screen instead
	if ( !cg.snap ) {
		CG_DrawInformation();
		return;
	}

	// optionally draw the tournement scoreboard instead
	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR &&
		( cg.snap->ps.pm_flags & PMF_SCOREBOARD ) ) {
		CG_DrawTourneyScoreboard();
		return;
	}

	// clear around the rendered view if sized down
	CG_TileClear();

	if(stereoView != STEREO_CENTER)
		CG_DrawCrosshair3D();

#ifdef WOLFET
	CG_PB_RenderPolyBuffers();
#endif

	// draw 3D view
	trap_R_RenderScene( &cg.refdef );

	// draw status bar and other floating elements
 	CG_Draw2D(stereoView);

#ifdef IOQ3ZTM // LETTERBOX
	// Draw black bars if needed.
	CG_DrawLetterbox();
#endif
}

#ifdef CAMERASCRIPT
/*
=================
CG_Fade
=================
*/
void CG_Fade( int a, int time, int duration ) {
	cgs.scrFadeAlpha = (float)a / 255.0f;
	cgs.scrFadeStartTime = time;
	cgs.scrFadeDuration = duration;
	if (cgs.scrFadeStartTime + cgs.scrFadeDuration <= cg.time) {
		cgs.scrFadeAlphaCurrent = cgs.scrFadeAlpha;
	}
	return;
}

void CG_DrawFlashFade( void ) {
	static int lastTime;
	int elapsed, time;
	vec4_t col;
	if (cgs.scrFadeStartTime + cgs.scrFadeDuration < cg.time) {
		cgs.scrFadeAlphaCurrent = cgs.scrFadeAlpha;
	} else if (cgs.scrFadeAlphaCurrent != cgs.scrFadeAlpha) {
     elapsed = (time = trap_Milliseconds()) - lastTime;
		lastTime = time;
		if (elapsed < 500 && elapsed > 0) {
			if (cgs.scrFadeAlphaCurrent > cgs.scrFadeAlpha) {
				cgs.scrFadeAlphaCurrent -= ((float)elapsed/(float)cgs.scrFadeDuration);
				if (cgs.scrFadeAlphaCurrent < cgs.scrFadeAlpha)
					cgs.scrFadeAlphaCurrent = cgs.scrFadeAlpha;
			} else {
				cgs.scrFadeAlphaCurrent += ((float)elapsed/(float)cgs.scrFadeDuration);
				if (cgs.scrFadeAlphaCurrent > cgs.scrFadeAlpha)
					cgs.scrFadeAlphaCurrent = cgs.scrFadeAlpha;
			}
		}
	}
	// now draw the fade
	if (cgs.scrFadeAlphaCurrent > 0.0) {
		VectorClear( col );
		col[3] = cgs.scrFadeAlphaCurrent;
		CG_FillRectFit( 0, 0, 640, 480, col );
	}
}
#endif

#ifdef IOQ3ZTM // LETTERBOX
/*
=============
CG_ToggleLetterbox
=============
*/
void CG_ToggleLetterbox(qboolean onscreen, qboolean instant)
{
	if (instant)
	{
		cg.letterbox = onscreen;
		cg.letterboxTime = -1;
		return;
	}

	// Don't restart current move type.
	if (cg.letterbox == onscreen)
	{
		return;
	}

	cg.letterbox = onscreen;
	cg.letterboxTime = cg.time;

	// ZTM: Play sounds like in LOZ:TP
	if (cg.letterbox)
	{
		// move on screen
		trap_S_StartLocalSound( cgs.media.letterBoxOnSound, CHAN_LOCAL_SOUND );
	}
	else
	{
		// move off screen
		trap_S_StartLocalSound( cgs.media.letterBoxOffSound, CHAN_LOCAL_SOUND );
	}
}

/*
=============
CG_DrawLetterbox
=============
*/
void CG_DrawLetterbox(void)
{
	const int letterbox_movetime = 150; // Time in msec to move the letterbox on or off of the screen.
	const int letterbox_height = 50;
	float color[4] = {0,0,0,1}; // letterbox color
	int pixels; // height of the black bar, in pixels
	int movetime;

	// 0 = new game default, -1 = instant was used.
	if (!cg.letterbox && cg.letterboxTime <= 0)
	{
		return;
	}

	// Get time since move started.
	movetime = cg.time - cg.letterboxTime;
	if (movetime < letterbox_movetime)
	{
		pixels = ( ( letterbox_height / (float)letterbox_movetime ) * movetime );
		if (pixels > letterbox_height)
			pixels = letterbox_height;
	}
	else
	{
		// moving is done
		pixels = letterbox_height;
	}

	// If moving off screen, draw according.
	if (!cg.letterbox) {
		pixels = letterbox_height - pixels;
	}

	// Check if there is anything to draw.
	if (pixels <= 0) {
		return;
	}

	CG_FillRectFit(0, 0, 640, pixels, color);
	CG_FillRectFit(0, 480-pixels, 640, pixels, color);
}
#endif

