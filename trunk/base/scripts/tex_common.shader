// textures/common shader file.

// While this isn't "textures/common/*" it doesn't need its own file...
textures/REGION
{
	surfaceparm nolightmap
}

// ZTM: Added for terrain. (http://www.simonoc.com/pages/articles.htm)
// ======================================================================
// alpha fade shaders
// (c) 2004 randy reddig
// http://www.shaderlab.com
// distribution, in part or in whole, in any medium, permitted
// ======================================================================
//
// These shaders are not fixed to this directory location, they can
// be moved around. They exist here for convenience only.
//
textures/common/alpha_000	// Primary texture ONLY
{
	q3map_alphaMod volume
	q3map_alphaMod set 0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

textures/common/alpha_025
{
	q3map_alphaMod volume
	q3map_alphaMod set 0.25
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

textures/common/alpha_050	// Perfect mix of both Primary + Secondary
{
	q3map_alphaMod volume
	q3map_alphaMod set 0.50
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

textures/common/alpha_075
{
	q3map_alphaMod volume
	q3map_alphaMod set 0.75
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

textures/common/alpha_085
{
	q3map_alphaMod volume
	q3map_alphaMod set 0.85
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

textures/common/alpha_100	// Secondary texture ONLY
{
	q3map_alphaMod volume
	q3map_alphaMod set 1.0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	qer_trans 0.75
}

// FROM OPEN ARENA : scripts/common.shader

//12-12-06 removed redundant nodrop
//12-23-06 fixed the b0rked invisible shader
//01-25-07 removed redundant clip + added nodrawnonsolid, clusterportal
//01-27-07 moved portal from clown.shader to here
//01-31-07 added mirror shader doesnt work, gave other shaders transparency in map editor.-kit89 
//02-14-07 added timportal + mirror1, mirror 2, terrain, terrain 2, metalclip 
//02-27-07 added botclip, missleclip, remapped certain mirrors to point to invisible.tga
//need this or maps FTBFS
//for the idiot bots out there use instead of botclip!!!!
textures/common/donotenter
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm donotenter
}

textures/common/clip
{
    qer_trans 0.5
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm noimpact
}

textures/common/caulk
{
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nolightmap
}

textures/common/invisible
{
	qer_editorimage textures/common/invisible.jpg
    qer_trans 0.5
	surfaceparm nolightmap
	{
		map textures/common/invisible.tga
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
}

//use this near the trigger hurts, lava, death fogs, etc.
// to keep weapons and powerups from piling up...
textures/common/nodrop
{
    qer_trans 0.5
	surfaceparm 	trans
	surfaceparm	nonsolid
	surfaceparm	nomarks
	surfaceparm     nodrop
	surfaceparm 	nolightmap
	surfaceparm 	nodraw
	cull		none
}
//need this for the teleporters in cbctf1
textures/common/trigger
{
    qer_trans 0.5
	surfaceparm nodraw
}

//also needed for for cbctf1
textures/common/origin
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm origin
}
//aids in VIS compiles
textures/common/hint
{
    qer_trans 0.5
	surfaceparm hint
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
}

textures/common/nodraw
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nomarks
	surfaceparm trans
}
//for an icy effect
textures/common/slick
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm slick
}

textures/common/cushion
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nodamage
	surfaceparm trans
}

//to keep certain textures from being shot up
textures/common/weapclip
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
}

//for every stupid q3dm17 remake
textures/common/nodrawnonsolid
{
    qer_trans 0.5
	surfaceparm	nonsolid
	surfaceparm	nodraw
}

//hint for the bots
textures/common/clusterportal
{
    qer_trans 0.5
	qer_nocarve
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm clusterportal
}

//can also be used as a mirror
// Same as textures/common/mirror1
textures/common/portal
{
	qer_editorimage textures/common/portal.jpg
	surfaceparm nolightmap
	portal
	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}

//Added for Mirrors
textures/common/timportal
{
	qer_editorimage textures/common/portal.jpg
	surfaceparm nolightmap
	portal
	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite

	}

	//
	{
		//map textures/oa_fogs/kc_fogcloud3.jpg
		map textures/skies/testsky_clouds.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphagen portal 512
		rgbGen identity	
		tcmod rotate .1
		tcmod scroll .04 .01
	}
}

//nicked from nexuiz for backwards compat 
textures/common/mirror1
{
	qer_editorimage textures/common/mirror1.jpg
	surfaceparm nolightmap
	surfaceparm glass
	portal
  	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}

//nicked from nexuiz w/added turb. for backwards compat 
textures/common/mirror2
{
	qer_editorimage textures/common/mirror1.jpg
	surfaceparm nolightmap
	surfaceparm glass
	portal
	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
	{
		map textures/sfx/mirror.png
		//tcMod turb 0 0.25 0 0.05
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
	}
}

//nicked from nexuiz
textures/common/terrain
{
	q3map_terrain
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nomarks
}

//nicked from nexuiz
textures/common/terrain2
{
	qer_editorimage textures/common/common.tga
	q3map_terrain
	surfaceparm dust
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nolightmap
}

//nicked from nexuiz
textures/common/metalclip
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm metalsteps
}

textures/common/missileclip
{
    qer_trans 0.5
	surfaceparm nodamage
	surfaceparm nomarks
	surfaceparm nodraw
	surfaceparm playerclip
	surfaceparm trans
}

textures/common/full_clip
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm playerclip
}

textures/common/antiportal
{
    qer_trans 0.5
	qer_nocarve
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm antiportal
}

textures/common/areaportal
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm areaportal
}

textures/common/lightgrid
{
    qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm detail
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm lightgrid
}


//
// Ladder shader (Based on shader from Smokin' Guns)
//
textures/common/ladder
{
	qer_editorimage textures/common/ladder.png
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nomarks
	surfaceparm ladder
	surfaceparm playerclip
	qer_trans 0.40
	surfaceparm nolightmap
	surfaceparm noimpact
}
