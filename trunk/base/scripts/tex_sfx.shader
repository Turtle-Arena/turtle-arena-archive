
textures/sfx/portal_sfx
{
	portal
	surfaceparm nolightmap
}

// From quake3
textures/sfx/beam
{
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps

	{
		map textures/sfx/beam.jpg
		tcMod Scroll .3 0
		blendFunc add
	}
}
