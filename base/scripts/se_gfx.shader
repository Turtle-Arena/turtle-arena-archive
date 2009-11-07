// Based on OpenArena's /scripts/decals.shader

// Melee Weapon Trail
weaponTrail
{
	cull disable
	{
		map models/misc/telep.png
		blendfunc GL_ONE GL_ONE
		tcGen environment
	}
}

// Plasm trail (\cg_oldPlasm 0) and q3 rail trail
railDisc
{
	sort nearest
	cull none
	deformVertexes wave 100 sin 0 .5 0 2.4
	{
		//clampmap sprites/plasmaa.png
		clampmap gfx/misc/raildisc_mono2
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod rotate -30
		// tcMod rotate 931 // Turtle Man
	}
}

// q3 rail trail
railCore
{
	sort nearest
	cull none
	{
		map gfx/misc/railcorethin_mono
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod scroll -1 0
	}
}

// Lightning trail from Q3's pak6.pk3
lightningBoltNew
{
	cull none
	{
		map gfx/misc/lightning3new
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 1 0.5 0 7.1
		tcmod scale  2 1
		tcMod scroll -5 0
	}
	{
		map gfx/misc/lightning3new
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 1 0.8 0 8.1
		tcmod scale  -1.3 -1
		tcMod scroll -7.2 0
	}
}

// Grappling Hook Cable, grapple trail shader --Turtle Man
GrappleCable
{
	cull disable
	{
		map models/weapons2/grapple/trail
		//blendfunc blend
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen Vertex
		alphaGen Vertex
	}
}

sparkTrail
{
	sort nearest
	cull none
	//deformVertexes wave 100 sin 0 .5 0 2.4
	{
		clampmap sprites/lasertrail.png
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod rotate 250
	}
}

console
{
	nopicmip
	nomipmaps
	{
		map gfx/misc/console01.png
		blendFunc GL_ONE GL_ZERO
		tcMod scroll .02  0
		tcmod scale 2 1
	}
	{
		map gfx/misc/console02.png
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

lagometer
{
	nopicmip
	{
		map gfx/2d/lag.png
	}
}

disconnected
{
	nopicmip
	{
		map gfx/2d/net.png
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
		alphaGen const 0.5
	}
}

// Turtle Man: In OpenArena this is called "oldwake" ...
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

//
// gfx shaders
//

gfx/2d/numbers/zero_32b
{
	nopicmip
	{
		map gfx/2d/numbers/zero_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/one_32b
{
	nopicmip
	{
		map gfx/2d/numbers/one_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/two_32b
{
	nopicmip
	{
		map gfx/2d/numbers/two_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/three_32b
{
	nopicmip
	{
		map gfx/2d/numbers/three_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/four_32b
{
	nopicmip
	{
		map gfx/2d/numbers/four_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/five_32b
{
	nopicmip
	{
		map gfx/2d/numbers/five_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/six_32b
{
	nopicmip
	{
		map gfx/2d/numbers/six_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/seven_32b
{
	nopicmip
	{
		map gfx/2d/numbers/seven_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/eight_32b
{
	nopicmip
	{
		map gfx/2d/numbers/eight_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/nine_32b
{
	nopicmip
	{
		map gfx/2d/numbers/nine_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
gfx/2d/numbers/minus_32b
{
	nopicmip
	{
		map gfx/2d/numbers/minus_32b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}


gfx/2d/crosshair
{
	nopicmip
	{
		map gfx/2d/crosshair.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen identity
	}
}

gfx/2d/crosshairb
{
	nopicmip
	{
		map gfx/2d/crosshairb.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairc
{
	nopicmip
	{
		map gfx/2d/crosshairc.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshaird
{
	nopicmip
	{
		map gfx/2d/crosshaird.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}


gfx/misc/tracer
{
	cull disable
	{
		map gfx/misc/tracer2.jpg
		blendfunc add
		//blendFunc GL_ONE GL_ONE
	}
}

// Turtle Man: Used by unfinished WP_GUN
gfx/damage/bullet_mrk
{
	polygonoffset
	{
		map gfx/damage/bullet_mrk.png
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// Turtle Man: Used by rocket launcher and homing-rocket launcher
gfx/damage/burn_med_mrk
{
	polygonoffset
	{
		map gfx/damage/burn_med_mrk.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// Turtle Man: Used by grapple and melee weapons.
gfx/damage/hole_lg_mrk
{
	polygonoffset
	{
		map gfx/damage/hole_lg_mrk.png
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// Turtle Man: Used by the electric launcher
gfx/damage/plasma_mrk
{
	polygonoffset
	{
		map gfx/damage/plasma_mrk.png
		blendfunc blend
		rgbGen Vertex
		alphaGen Vertex
	}
}

// Turtle Man: Used by hammer melee weapon
gfx/damage/hammer_mrk
{
	polygonoffset
	{
		map gfx/damage/hammer_mrk.png
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

//
// Melee damage hit effects
//

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
//	{
//		animmap 5 models/weaphits/meleehit2_1.png models/weaphits/meleehit2_2.png models/weaphits/meleehit2_3.png models/weaphits/meleehit2_4.png models/weaphits/meleehit2_5.png
//		blendfunc blend
//	}
	// meleeHit1
	{
		animmap 5 models/weaphits/meleehit1_1.png models/weaphits/meleehit1_2.png models/weaphits/meleehit1_3.png models/weaphits/meleehit1_4.png models/weaphits/meleehit1_5.png
		blendfunc blend
	}
}

meleeHit3
{
	cull disable
//	{
//		animmap 5 models/weaphits/meleehit3_1.png models/weaphits/meleehit3_2.png models/weaphits/meleehit3_3.png models/weaphits/meleehit3_4.png models/weaphits/meleehit3_5.png
//		blendfunc blend
//	}
	// meleeHit1
	{
		animmap 5 models/weaphits/meleehit1_1.png models/weaphits/meleehit1_2.png models/weaphits/meleehit1_3.png models/weaphits/meleehit1_4.png models/weaphits/meleehit1_5.png
		blendfunc blend
	}
}


//
// Explosions
//

/*
plasmaExplosion
{
	cull disable
	{
		clampmap models/weaphits/plasmaboom.tga
		blendfunc add

		tcMod stretch triangle .6 0.1 0 8
		tcmod rotate 999
		rgbGen wave inversesawtooth 0 1 0 1.5
	}
}

bulletExplosion
{
	cull disable
	{
		animmap 5 models/weaphits/bullet1.tga  models/weaphits/bullet2.tga  models/weaphits/bullet3.tga gfx/colors/black.tga
		rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaphits/bullet2.tga  models/weaphits/bullet3.tga  gfx/colors/black.tga gfx/colors/black.tga
		rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}
*/

rocketExplosion
{
	cull disable
	{
		animmap 8 models/weaphits/rlboom/rlboom_1.jpg  models/weaphits/rlboom/rlboom_2.jpg models/weaphits/rlboom/rlboom_3.jpg models/weaphits/rlboom/rlboom_4.jpg models/weaphits/rlboom/rlboom_5.jpg models/weaphits/rlboom/rlboom_6.jpg models/weaphits/rlboom/rlboom_7.jpg models/weaphits/rlboom/rlboom_8.jpg
		rgbGen wave inversesawtooth 0 1 0 8
		blendfunc add
	}
	{
		animmap 8 models/weaphits/rlboom/rlboom_2.jpg models/weaphits/rlboom/rlboom_3.jpg models/weaphits/rlboom/rlboom_4.jpg models/weaphits/rlboom/rlboom_5.jpg models/weaphits/rlboom/rlboom_6.jpg models/weaphits/rlboom/rlboom_7.jpg models/weaphits/rlboom/rlboom_8.jpg gfx/colors/black.png
		rgbGen wave sawtooth 0 1 0 8
		blendfunc add
	}
}

//explode1 (explode11 to explode122) See id software's quake3 pak7.pk3 ... and cg_oldRocket 0

//
// blood shaders
//

bloodExplosion		// spurt of blood at point of impact
{
	cull disable
	{
		animmap 5 models/weaphits/blood201.tga models/weaphits/blood202.tga models/weaphits/blood203.tga models/weaphits/blood204.tga models/weaphits/blood205.tga
		blendfunc blend
	}
}

viewBloodBlend
{
	sort	nearest
	{
		//map models/weaphits/blood201.tga
                map gfx/damage/blood_screen.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identityLighting
		alphaGen vertex
	}
}

// gibs hit ground
bloodMark
{
	nopicmip
	polygonOffset
	{
		clampmap gfx/damage/blood_stain.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identityLighting
		alphaGen vertex
	}
}

// gibs trail
bloodTrail
{
	nopicmip
	entityMergable
	{
		//clampmap gfx/misc/blood.tga
                clampmap gfx/damage/blood_spurt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

//
// Unused shaders from OpenArena
//

Grareflaader
{
	cull none
	{
		map gfx/misc/flare.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

boens
{
	cull none
	{
		map gfx/misc/sun.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

// Turtle Man: In OpenArena this is "wake"
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


