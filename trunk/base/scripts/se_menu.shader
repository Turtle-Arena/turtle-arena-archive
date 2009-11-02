// shaders for the Q3 menu.

gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/bigchars
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/2d/select
{
	nopicmip
	{
		map gfx/2d/select
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
		map menu/art/menuback
		//blendFunc GL_ONE GL_ZERO
		tcMod scroll .04  .02
		//tcMod scale 1.2 1.2
	}
	//{
	//	map menu/art/menuback
	//	blendFunc add
	//	tcMod scroll .02  .01
	//	tcMod scale .9 .9
	//	tcMod turb 0 .1 0 .1
	//}
}

// menu background with out blending
menubackRagePro
{
	nomipmaps
	{
		map menu/art/menuback
		//blendFunc GL_ONE GL_ZERO
		tcMod scroll .04  .02
		//tcMod scale 1.2 1.2
	}
}

levelShotDetail
{
	nopicmip
	{
		map textures/sfx/detail
		blendFunc GL_DST_COLOR GL_SRC_COLOR
		rgbgen identity
	}
}

