// shaders for the q3_ui menus

menu/art/titlebanner
{
	nomipmaps
	{
		map menu/art/titlebanner.png
		blendfunc blend
	}
}

gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/bigchars.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/2d/select
{
	nopicmip
	{
		map gfx/2d/select.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
		rgbgen vertex
	}
}

// menu background
menuback
{
	nomipmaps
	{
		map menu/art/menuback.png
		tcMod scroll 0.04 0.02
	}
	{
		map menu/art/menuback2.png
		blendFunc blend
		tcMod scale 0.5 0.5
		tcMod scroll -0.02 -0.01
		rgbGen const ( 0.35 0.35 0.35 )
		alphaGen const 0.95
	}
}

// menu background without blending
menubackRagePro
{
	nomipmaps
	{
		map menu/art/menuback.png
		tcMod scroll 0.04 0.02
	}
}

levelShotDetail
{
	nopicmip
	{
		map textures/sfx/detail.jpg
		blendFunc GL_DST_COLOR GL_SRC_COLOR
		rgbgen identity
	}
}

