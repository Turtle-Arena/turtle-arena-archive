// base_floor.shader

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