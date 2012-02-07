// Shaders used by the client

// Drawn by the client while the engine is starting up.
clientLoading
{
	nomipmaps
	{
		map gfx/clientLoading.png
	}

	// ZTM: HACK: For some reason it needs two stages, otherwise it just shows white...
	// so I added an invisable stage.
	{
		map textures/common/invisible.tga
		blendFunc blend
	}
}

console
{
	nomipmaps
	{
		map gfx/misc/console01.png
		blendFunc blend
		tcMod scroll -0.01 -0.005
	}
}

white
{
	{
		map *white
		blendfunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

