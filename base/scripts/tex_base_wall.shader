// From Quake3's base_wall.shader
textures/base_wall/glass01
{
	surfaceparm trans	
	cull none
	qer_trans 	0.5

	{
		// Turtle Man: FIXME: This is the only shader [that I currently have] that uses tinfx.tga, so...
		map textures/effects/tinfx.tga
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
}
