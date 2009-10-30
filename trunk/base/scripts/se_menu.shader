// shaders for the q3 menu.

gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/bigchars.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/select
{
	nopicmip
	{
		map gfx/2d/select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
		rgbgen vertex
	}
}

menuback
{
	nopicmip
	nomipmaps
	{
		map textures/sfx/logo512.tga
		rgbgen identity
	}
}

menubacknologo
{
	nopicmip
	nomipmaps
	{
		map gfx/colors/black.tga
	} 
}

menubackRagePro
{
	nopicmip
	nomipmaps
	{
		map textures/sfx/logo512.tga
	}
}

levelShotDetail
{
	nopicmip
	{
		map textures/sfx/detail.tga
	        blendFunc GL_DST_COLOR GL_SRC_COLOR
		rgbgen identity
	}
}
