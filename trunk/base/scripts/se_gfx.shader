// Based on OpenArena's /scripts/decals.shader

// Snow!
gfx/misc/snow
{
	cull disable
	{
		map gfx/misc/snow
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// raindrop
gfx/misc/raindrop
{
	cull disable
	{
		map gfx/misc/raindrop
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Melee Weapon Trail
weaponTrail
{
	cull disable
	{
		map gfx/misc/weapontrail
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Used by Plasm trail (when cg_oldPlasm is 0) and Rail trail
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
		// tcMod rotate 931 // ZTM
	}
}

// Rail trail
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

// Grappling Hook Cable, grapple trail shader --ZTM
GrappleCable
{
	cull disable
	{
		map models/weapons2/grapple/trail.png
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
		blendFunc blend
		tcMod scroll -0.01 0
	}
	{
		map menu/art/menuback2.png
		blendFunc blend
		tcMod scale 1 1
		tcMod scroll -0.02 -0.01
		alphaGen const 0.4
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

// ZTM: In OpenArena this is called "oldwake", see OA for their new wake.
wake
{
	{
		clampmap sprites/splash.png
		blendfunc add
		rgbGen wave sin 0.7 0.3 0.25 0.5
		tcMod rotate 250
		tcMod stretch sin 0.9 0.1 0 0.7
	}
	{
		clampmap sprites/splash.png
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

// ZTM: Used by unfinished WP_GUN
gfx/damage/bullet_mrk
{
	polygonoffset
	{
		map gfx/damage/bullet_mrk.png
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// ZTM: Used by rocket launcher and homing-rocket launcher
gfx/damage/burn_med_mrk
{
	polygonoffset
	{
		map gfx/damage/burn_med_mrk.tga
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// ZTM: Used by grapple and melee weapons.
gfx/damage/hole_lg_mrk
{
	polygonoffset
	{
		map gfx/damage/hole_lg_mrk.png
		blendfunc gl_zero gl_one_minus_src_color
		rgbGen Vertex
	}
}

// ZTM: Used by the electric launcher
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

// ZTM: Used by hammer melee weapon
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
// Player damage hit effects
//
// Based on bloodExplosion
//

meleeHit1
{
	cull disable
	{
		animmap 5 models/weaphits/meleehit1_1.png models/weaphits/meleehit1_2.png models/weaphits/meleehit1_3.png models/weaphits/meleehit1_4.png models/weaphits/meleehit1_5.png
		blendfunc blend
	}
}

//meleeHit2
//{
//	cull disable
//	{
//		animmap 5 models/weaphits/meleehit2_1.png models/weaphits/meleehit2_2.png models/weaphits/meleehit2_3.png models/weaphits/meleehit2_4.png models/weaphits/meleehit2_5.png
//		blendfunc blend
//	}
//}

missileHit1
{
	cull disable
	{
		// ZTM: TODO: Create images for missileHit1
//		animmap 5 models/weaphits/missilehit1_1.png models/weaphits/missilehit1_2.png models/weaphits/missilehit1_3.png models/weaphits/missilehit1_4.png models/weaphits/missilehit1_5.png

		// Temporarily use secondary melee hit images
		animmap 5 models/weaphits/meleehit2_1.png models/weaphits/meleehit2_2.png models/weaphits/meleehit2_3.png models/weaphits/meleehit2_4.png models/weaphits/meleehit2_5.png
		blendfunc blend
	}
}

//missileHit2
//{
//	cull disable
//	{
//		animmap 5 models/weaphits/missilehit2_1.png models/weaphits/missilehit2_2.png models/weaphits/missilehit2_3.png models/weaphits/missilehit2_4.png models/weaphits/missilehit2_5.png
//		blendfunc blend
//	}
//}


//
// Explosions
//

plasmaExplosion
{
	cull disable
	{
		clampmap models/weaphits/plasmaboom.png
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
		animmap 5 models/weaphits/bullet1.png  models/weaphits/bullet2.png  models/weaphits/bullet3.png gfx/colors/black.png
		//rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaphits/bullet2.png  models/weaphits/bullet3.png  gfx/colors/black.png gfx/colors/black.png
		//rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}

bulletExplosionColorize
{
	cull disable
	{
		animmap 5 models/weaphits/bulletc1.png  models/weaphits/bulletc2.png  models/weaphits/bulletc3.png gfx/colors/black.png
		rgbGen entity // Colorize
		blendfunc add
	}
	{
		animmap 5 models/weaphits/bulletc2.png  models/weaphits/bulletc3.png  gfx/colors/black.png gfx/colors/black.png
		rgbGen entity // Colorize
		blendfunc add
	}
}

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

