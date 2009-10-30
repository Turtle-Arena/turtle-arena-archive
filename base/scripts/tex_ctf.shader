// From Q3s pak0.pk3/scripts/ctf.shader

textures/ctf/ctf_blueflag
{
	tessSize 64
	deformVertexes wave 194 sin 0 3 0 .4
	deformVertexes normal .5 .1
	surfaceparm nomarks
	cull none

	{
		map textures/ctf/ctf_blueflag.tga
		rgbGen identity
	}
	{
		map textures/effects/redflagmap.tga
		tcGen environment
		tcmod scale 9 3
		tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map textures/ctf/ctf_blueflag.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}
textures/ctf/ctf_redflag
{
	tessSize 64
	deformVertexes wave 194 sin 0 3 0 .4
	deformVertexes normal .3 .2
	surfaceparm nomarks
	cull none

	{
		map textures/ctf/ctf_redflag.tga
		rgbGen identity
	}
	{
		map textures/effects/blueflagmap.tga
		tcGen environment
		tcmod scale 9 3
		tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map textures/ctf/ctf_redflag.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}
