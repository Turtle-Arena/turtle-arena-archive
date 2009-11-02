// Shaders that are not need (and shouldn't be used by TMNT Arena maps),
//  but we have them for "quake3 map compatablity"... They may be removed leter.

// See textures/common/mirror1 in tex_common.shader
textures/common/mirror
{
	qer_editorimage textures/common/mirror1.jpg
	q3map_nolightmap
	portal
    {
        map textures/common/invisible.tga
        blendFunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
        depthWrite
    }
}

// See textures/common/donotenter in tex_common.shader
// acts as player clip only for the bots
// can keep them from being pushed into voids
// do not use, use donotenter instead :-P
textures/common/botclip
{
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm botclip
}
