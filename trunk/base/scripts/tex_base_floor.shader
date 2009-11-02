// base_floor.shader

textures/base_floor/sewer_floor
{
	surfaceparm dust
	surfaceparm dirt
	{
		map textures/base_floor/sewer_floor.png
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

// Taken from Q3A shader manual, textures/base_floor/pjgrate1
textures/base_floor/grate_rust
{
	qer_editorimage textures/base_floor/grate_rust.tga

	surfaceparm trans
	surfaceparm metalsteps
	cull none

      // A GRATE OR GRILL THAT CAN BE SEEN FROM BOTH SIDES
      {
            map textures/base_floor/grate_rust.tga
            blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
            alphaFunc GT0
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

textures/base_floor/grate
{
	qer_editorimage textures/base_floor/grate.tga

	surfaceparm trans
	surfaceparm metalsteps
	cull none

      // A GRATE OR GRILL THAT CAN BE SEEN FROM BOTH SIDES
      {
            map textures/base_floor/grate.tga
            blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
            alphaFunc GT0
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

// metal
textures/base_floor/metal
{
	surfaceparm metalsteps
	{
		map textures/base_floor/metal
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/base_floor/metal2
{
	surfaceparm metalsteps
	{
		map textures/base_floor/metal2
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/base_floor/metal_textured
{
	surfaceparm metalsteps
	{
		map textures/base_floor/metal_textured
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

// stone
textures/base_floor/concrete
{
	surfaceparm stone
	{
		map textures/base_floor/concrete
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/base_floor/concrete2
{
	surfaceparm stone
	{
		map textures/base_floor/concrete2
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/base_floor/concrete_old
{
	surfaceparm stone
	{
		map textures/base_floor/concrete_old
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

// grass
textures/base_floor/grass
{
	surfaceparm grass
	{
		map textures/base_floor/grass
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/base_floor/grass2
{
	surfaceparm grass
	{
		map textures/base_floor/grass2
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
