// From Team Arena demo's ctf2.shader

textures/ctf2/redteam02
{
	qer_editorimage textures/ctf2/redteam01.tga 	
	cull none
	surfaceparm alphashadow
	surfaceparm nomarks
	{
		map textures/ctf2/redteam01.tga
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/ctf2/blueteam02
{
	qer_editorimage textures/ctf2/blueteam01.tga 	
	cull none
	surfaceparm alphashadow
	surfaceparm nomarks
	{
		map textures/ctf2/blueteam01.tga
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/ctf2/redteam01
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map textures/ctf2/redteam01.tga
		blendFunc Add
		//rgbgen wave triangle 0.2 0.5 0 0.2
		rgbGen wave sin 1 .5 0 .2
	}
}

textures/ctf2/blueteam01
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map textures/ctf2/blueteam01.tga
		blendFunc Add
		//rgbgen wave triangle 0.2 0.5 0 0.2
		rgbGen wave sin 1 .8 0 .2
	}
}

textures/ctf2/red_banner02
{       	
	surfaceparm alphashadow
	cull none
       
	{
		map textures/ctf2/red_banner02.tga
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/ctf2/blue_banner02
{
	surfaceparm alphashadow
	cull none

	{
		map textures/ctf2/blue_banner02.tga
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
        
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

