// Grappling Hook Cable, grapple trail shader --Turtle Man
GrappleCable
{
	cull disable
	{
		map models/weapons2/grapple/trail.png
		//blendfunc blend
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen Vertex
		alphaGen Vertex
	}
}

console
{
	nopicmip
	nomipmaps
	{
		map gfx/misc/console01.tga
		blendFunc GL_ONE GL_ZERO
		tcMod scroll .02  0
		tcmod scale 2 1
	}
	{
		map gfx/misc/console02.tga
		blendFunc add
		tcMod turb 0 .1 0 .1
		tcMod scale 2 1
		tcmod scroll 0.2  .1
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


//bloodExplosion		// spurt of blood at point of impact
//{
//	cull disable
//	{
//		animmap 5 models/weaphits/blood201.tga models/weaphits/blood202.tga models/weaphits/blood203.tga models/weaphits/blood204.tga models/weaphits/blood205.tga
//		blendfunc blend
//	}
//}

meleeHit1
{
	cull disable
	{
		animmap 5 models/weaphits/meleehit1_1.png models/weaphits/meleehit1_2.png models/weaphits/meleehit1_3.png models/weaphits/meleehit1_4.png models/weaphits/meleehit1_5.png
		blendfunc blend
	}
}

meleeHit2
{
	cull disable
	{
		animmap 5 models/weaphits/meleehit2_1.png models/weaphits/meleehit2_2.png models/weaphits/meleehit2_3.png models/weaphits/meleehit2_4.png models/weaphits/meleehit2_5.png
		blendfunc blend
	}
}

meleeHit3
{
	cull disable
	{
		animmap 5 models/weaphits/meleehit3_1.png models/weaphits/meleehit3_2.png models/weaphits/meleehit3_3.png models/weaphits/meleehit3_4.png models/weaphits/meleehit3_5.png
		blendfunc blend
	}
}


// OPEN ARENA : /scripts/decals.shader //
// In OpenArena this is called "wake"
newwake
{
	cull disable
	{
		animmap 30 sprites/multwake1.tga sprites/multwake2.tga sprites/multwake3.tga sprites/multwake4.tga sprites/multwake5.tga sprites/multwake6.tga sprites/multwake7.tga sprites/multwake8.tga 
		blendfunc gl_dst_color gl_src_color
	}
}

newishwake
{
	{
		animmap 8 sprites/splash6.tga sprites/splash5.tga sprites/splash4.tga sprites/splash3.tga sprites/splash2.tga 
		blendfunc gl_src_alpha gl_one
		alphaGen wave inversesawtooth 0 1 0 8 
	}
	{
		animmap 8 sprites/splash5.tga sprites/splash4.tga sprites/splash3.tga sprites/splash2.tga sprites/splash6.tga 
		blendfunc gl_src_alpha gl_one
		alphaGen wave sawtooth 0 1 0 8 
	}
}

gfx/misc/tracer
{
	cull disable
	{
		map gfx/misc/tracer2.tga
		blendfunc add
	}
}


gfx/damage/bullet_mrk
{
	polygonoffset
	{
		map gfx/damage/bulletmult.tga
		blendfunc gl_dst_color gl_src_color
		alphaGen Vertex
	}
}

gfx/damage/bullet_mrk
{
	polygonoffset
	{
		map gfx/damage/bullet_mrk.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

gfx/damage/burn_med_mrk
{
	polygonoffset
	{
		map gfx/damage/burn_med_mrk.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

gfx/damage/hole_lg_mrk
{
	polygonoffset
	{
		map gfx/damage/hole_lg_mrk.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

gfx/damage/plasma_mrk
{
	polygonoffset
	{
		map gfx/damage/plasma_mrk.tga
		blendfunc blend
		rgbGen Vertex
		alphaGen Vertex
	}
}

markShadow
{
	polygonoffset
	{
		map gfx/damage/shadow.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

projectionShadow
{
	polygonoffset
	deformvertexes projectionshadow		//need this
	{
		map $whiteimage
		blendfunc filter
		rgbGen wave square 0 0 0 0 
	}
}

// In OpenArena this is called "oldwake"
wake
{
	{
		clampmap sprites/splash.tga
		blendfunc add
		rgbGen wave sin 0.7 0.3 0.25 0.5 
		tcMod rotate 250
		tcMod stretch sin 0.9 0.1 0 0.7 
	}
	{
		clampmap sprites/splash.tga
		blendfunc add
		rgbGen wave sin 0.7 0.3 0.25 0.4 
		tcMod rotate -230
		tcMod stretch sin 0.9 0.05 0 0.9 
	}
}

