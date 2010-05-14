// Shaders for the models.

//
// Player shaders
//

// Raph's eyes/mouth should be lighter then normal textures
//   and TODO: the brightness should change base on how close we are, I think they are whiter farer away.
models/players/raph/white
{
	//cull disable
	{
		map $whiteimage
		rgbGen const ( 0.9 0.9 0.9 )
	}
	//{
		//map $whiteimage
		//rgbGen const ( 0.0 0.0 0.0 )
		//AlphaGen portal 256
	//}
}

models/players/raph/belt
{
	{
		map models/players/raph/belt.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/body
{
	{
		map models/players/raph/body.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/head
{
	{
		map models/players/raph/head.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/legs
{
	{
		map models/players/raph/legs.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_blue
{
	{
		map models/players/raph/belt_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/body_blue
{
	{
		map models/players/raph/body_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/head_blue
{
	{
		map models/players/raph/head_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_red
{
	{
		map models/players/raph/belt_red.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/body_red
{
	{
		map models/players/raph/body_red.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/head_red
{
	{
		map models/players/raph/head_red.png
		rgbGen lightingDiffuse
	}
}

// The Fugitoid!
// green eyes and mouth
models/players/fugitoid/eyes_mouth
{
	{
		map $whiteimage
		rgbGen const ( 0.0 0.9 0.0 )
	}
}

models/players/fugitoid/eyes_mouth_red
{
	{
		map $whiteimage
		rgbGen const ( 0.9 0.0 0.0 )
	}
}

models/players/fugitoid/eyes_mouth_blue
{
	{
		map $whiteimage
		rgbGen const ( 0.0 0.0 0.9 )
	}
}

models/players/fugitoid/body
{
	{
		map textures/effects/tinfx_lighter.png
		tcGen environment
		rgbGen lightingDiffuse
	}
}

//
// TELEPORT
//

// teleport model shader
models/misc/telep
{
	cull disable
	{
		map models/misc/telep.png
		tcGen environment
		//tcMod turb 0 0.1 0 0.1
		//tcmod rotate 311
		//tcmod scroll 1 1
		blendfunc add
		rgbGen lightingDiffuse
	}
}

//
// SHURIKENS
//

models/powerups/holdable/shuriken
{
	{
		map models/powerups/holdable/shuriken_1.png
	}
}

models/powerups/holdable/shurikenfire
{
	{
		map models/powerups/holdable/shurikenfire_1.png
	}
}

models/powerups/holdable/shurikenelectric
{
	{
		map models/powerups/holdable/shurikenelectric_1.png
	}
}

models/powerups/holdable/shurikenlaser
{
	{
		map models/powerups/holdable/shurikenlaser_1.png
	}
}

models/shurikens/shuriken
{
	{
		map models/powerups/holdable/shuriken_1.png
	}
}

models/shurikens/shurikenfire
{
	{
		map models/powerups/holdable/shurikenfire_1.png
	}
}

models/shurikens/shurikenelectric
{
	{
		map models/powerups/holdable/shurikenelectric_1.png
	}
}

models/shurikens/shurikenlaser_1
{
	{
		map models/powerups/holdable/shurikenlaser_1.png
	}
}

// sparkTrail
models/shurikens/laserglow
{
	sort nearest
	cull none
	deformVertexes autosprite
	{
		clampmap sprites/laserglow.png
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod rotate 500
	}
}

// Must disable culling else the model can't be seen.
models/mapobjects/traffic_cone/traffic_cone
{
	cull disable
	{
		map models/mapobjects/traffic_cone/traffic_cone.jpg
		rgbGen lightingDiffuse
	}
}

models/powerups/health/health
{
	{
		map models/powerups/health/health.png
		rgbGen lightingDiffuse
	}
}

// Based on "powerups/invisibility" (se_powerups.shader)
models/powerups/instant/invis
{
	{
		//map models/powerups/instant/invis.png
		map textures/effects/invismap.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

models/powerups/instant/defense
{
	{
		map models/powerups/instant/defense.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

models/powerups/instant/invul
{
	{
		map models/powerups/instant/invul.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

models/powerups/instant/speed
{
	{
		map models/powerups/instant/speed.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

models/powerups/instant/strength
{
	{
		map models/powerups/instant/strength.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .5 .4 0 .6
	}
}

//
// Powerup rings
//
//models/powerups/instant/defense
models/powerups/shield/yellow
{
	{
		map models/powerups/instant/defense.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

//models/powerups/instant/invul
models/powerups/shield/white
{
	{
		map models/powerups/instant/invul.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

//models/powerups/instant/speed
models/powerups/shield/blue
{
	{
		map models/powerups/instant/speed.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

//models/powerups/instant/strength
models/powerups/shield/red
{
	{
		map models/powerups/instant/strength.png
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .5 .4 0 .6
	}
}

//
// Weapons
//

models/weapons2/katana_leo/w_katana
{
	{
		map models/weapons2/katana_leo/w_katana.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
	{
		map models/weapons2/katana_leo/w_katana_spec.png
		blendFunc GL_SRC_ALPHA GL_ONE
		detail
		alphaGen lightingSpecular
	}
	//{
	//	map models/weapons2/blade_enviro.jpg
	//	blendFunc GL_DST_COLOR GL_SRC_COLOR
	//	tcGen environment
	//}
}

models/weapons2/sai/w_sai
{
	{
		map models/weapons2/sai/w_sai.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
	//{
	//	map models/weapons2/sai/w_sai_spec.png
	//	blendFunc GL_SRC_ALPHA GL_ONE
	//	detail
	//	alphaGen lightingSpecular
	//}
	//{
	//	map models/weapons2/blade_enviro.jpg
	//	blendFunc GL_DST_COLOR GL_SRC_COLOR
	//	tcGen environment
	//}
}

models/weapons2/nunchuck/w_nunchuck
{
	{
		map models/weapons2/nunchuck/w_nunchuck.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/weapons2/nunchuck/chain
{
	{
		map $whiteimage
		rgbGen const ( 0.4 0.4 0.4 )
	}
	{
		map textures/effects/tinfx_lighter.png
		tcGen environment
		rgbGen lightingDiffuse
		//alphaGen wave sin .6 .2 0 .6
	}
}

models/weapons2/gun/flash
{
	cull disable
	deformVertexes autosprite

	{
		clampmap models/weapons2/gun/flash
		blendfunc add

		tcMod stretch triangle .6 0.1 0 8
		tcmod rotate 999
		rgbGen entity
	}
}

//
// Projectiles
//
models/ammo/pulse/pulse
{
	sort nearest
	cull none
	deformVertexes autosprite2

	{
		map models/ammo/pulse/pulse
		blendFunc blend
		rgbGen identity
	}
}

//
// MATERIALS
//
models/materials/dirt
{
	celoutline 2

	{
		map models/materials/dirt.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/materials/grass
{
	celoutline 2

	{
		map models/materials/grass.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/materials/wood
{
	celoutline 2

	{
		map models/materials/wood.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/materials/stone
{
	celoutline 2

	{
		map models/materials/stone.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/materials/metal
{
	celoutline 2

	{
		map models/materials/metal.png
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

//models/materials/sparks

//Based on textures/base_wall/glass01
models/materials/glass
{
	surfaceparm trans
	cull none
	celoutline 2

	{
		map textures/effects/tinfx.png
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
}


// CTF FLAGS / MISSIONPACK CTF FLAGS
// Below CTF flay shaders from MISSIONPACK demo pak0.pk3/scripts/models.shader
models/flags/pole
{
	{
		// ZTM: Changed image.
		map textures/effects/tinfx.png
		//map models/flags/pole.png
		tcGen environment
		rgbGen lightingDiffuse
	}
}

models/flags/skull
{
	{
		// ZTM: Changed image.
		map textures/effects/envmapmach.png
		//map models/flags/skull.png
		tcGen environment
		rgbGen lightingDiffuse
	}
}

models/flags/r_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/r_flag2.png
		rgbGen identity
	}
	{
		map models/flags/red_fx.png
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/r_flag2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flags/b_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/b_flag2.png
		rgbGen identity
	}
	{
		map models/flags/blue_fx.png
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/b_flag2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


// Below CTF flag shaders from MISSIONPACK demo pak0.pk3/scripts/models2.shader
models/flags/n_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/n_flag2.png
		rgbGen identity
	}
	{
		map models/flags/neutral_fx.png
		tcGen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/n_flag2.png
		blendFunc blend
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/r_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/r_flag2.png
		rgbGen identity
	}
	{
		map models/flag2/red_fx.png
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/r_flag2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/b_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/b_flag2.png
		rgbGen identity
	}
	{
		map models/flag2/blue_fx.png
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/b_flag2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/w_flag2
{
	//deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/w_flag2.png
		rgbGen identity
	}
	{
		map models/flag2/neutral_fx.png
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/w_flag2.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.jpg
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

//
// Below are from missionpack's models2.shader
//

// overload_base.md3
models/powerups/ob2_1
{
	{
		//map models/powerups/overload_map.tga
		//blendFunc GL_ONE GL_ZERO`
		//tcGen environment
		//rgbGen identity

		map models/powerups/overload_base.png
		//rgbGen lightingDiffuse
	}
}

// overload_target.md3
models/powerups/ob2_2
{
	{
		//map models/powerups/overload_mapbw.tga
		map textures/effects/tinfx.png
		//blendFunc GL_ONE GL_ZERO
	//	tcGen environment
		rgbGen entity

		// ZTM: Invul powerup shader
		//map models/powerups/instant/invul.png
		blendfunc GL_ONE GL_ONE
		//rgbGen lightingDiffuse
		//tcMod turb 0 0.15 0 0.25
		tcMod scroll .3 .3
		//tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}

// overload_lights.md3
models/powerups/ob2_3
{
	{
		map models/powerups/overload_lights.png
		tcmod scroll 1 1
		rgbGen entity
	}
}

// overload_energy.md3
models/powerups/ob2_4
{
	cull disable
	{
		// ZTM: Don't have pop_bw
		//map models/powerups/pop_bw.tga
		map textures/effects/tinfx.png
		tcMod turb 0 0.1 0 0.1
		tcmod rotate 311
		tcmod scroll 1 1
		blendfunc Add
		rgbGen entity
	}
}


// Based on id's Team Arena 'models/mapobjects/flagbase/swirl_blue'
models/mapobjects/flagbase/base_blue
{
	{
		map models/mapobjects/flagbase/swirl_blue.png
		blendFunc GL_ONE GL_ZERO
		tcmod rotate 130
		tcMod stretch sin .8 0.2 0 .2
		rgbGen identity
	}
	{
		map models/mapobjects/flagbase/swirl_blue.png
		blendFunc Add
		tcmod rotate 80
		tcMod stretch sin .8 0.2 0 .1
		rgbGen identity
	}

	{
		map models/mapobjects/flagbase/base_blue.png
		blendFunc blend
		rgbGen lightingdiffuse
	}
}

models/mapobjects/flagbase/base_red
{
	{
		map models/mapobjects/flagbase/swirl_red.png
		blendFunc GL_ONE GL_ZERO
		tcmod rotate 130
		tcMod stretch sin .8 0.2 0 .2
		rgbGen identity
	}
	{
		map models/mapobjects/flagbase/swirl_red.png
		blendFunc Add
		tcmod rotate 80
		tcMod stretch sin .8 0.2 0 .1
		rgbGen identity
	}
	{
		map models/mapobjects/flagbase/base_red.png
		blendFunc blend
		rgbGen lightingdiffuse
	}
}

models/mapobjects/flagbase/base_ntrl
{
	{
		map models/mapobjects/flagbase/swirl_ntrl.png
		blendFunc GL_ONE GL_ZERO
		tcmod rotate 130
		tcMod stretch sin .8 0.2 0 .2
		rgbGen identity
	}
	{
		map models/mapobjects/flagbase/swirl_ntrl.png
		blendFunc Add
		tcmod rotate 80
		tcMod stretch sin .8 0.2 0 .1
		rgbGen identity
	}
	{
		map models/mapobjects/flagbase/base_ntrl.png
		blendFunc blend
		rgbGen lightingdiffuse
	}
}

models/mapobjects/flagbase/beam_blue
{
	surfaceparm	trans
	nomipmaps
	cull disable
	{
		map models/mapobjects/flagbase/beam_blue.png
		blendFunc add
		tcmod scroll -.5 0
		rgbGen wave sin .5 .5 0.5 0.09
	}
}
models/mapobjects/flagbase/beam_ntrl
{
	surfaceparm trans
	nomipmaps
	cull disable
	{
		map models/mapobjects/flagbase/beam_ntrl.png
		blendFunc add
		tcmod scroll -.5 0
		rgbGen wave sin .5 .5 0.5 0.09
	}
}
models/mapobjects/flagbase/beam_red
{
	surfaceparm	trans
	nomipmaps
	cull disable
	{
		map models/mapobjects/flagbase/beam_red.png
		blendFunc add
		tcmod scroll -.5 0
		rgbGen wave sin .5 .5 0.5 0.09
	}
}


//
// Missionpack powerups
//

// Note: Yallow/orange in Q3/TA
models/powerups/ammo1
{
	{
		map textures/effects/envmapgreen.png
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo2
{
    cull disable
	{
		map models/powerups/ammo2.png
		blendfunc Add
		// ZTM: Go the same speed as others.
		//tcmod Scroll 1.3 0
		tcmod Scroll .9 0
		rgbGen identity
	}
}


// Note: Green in Q3/TA
models/powerups/scout1
{
	{
		map textures/effects/envmapblue.png
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}

models/powerups/scout2
{
	cull disable
	{
		map models/powerups/scout2.png
		blendfunc Add
		tcmod Scroll .9 0
		rgbGen identity
	}
}

// Note: Blue in Q3/TA
models/powerups/guard1
{
	{
		map textures/effects/envmapmach.png
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}
models/powerups/guard2
{
	cull disable
	{
		map models/powerups/guard2.png
		blendfunc Add
		tcmod Scroll .9 0
		rgbGen identity
	}
}

models/powerups/doubler1
{
	{
		map textures/effects/envmaproc.png
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}
models/powerups/doubler2
{
	cull disable
	{
		map models/powerups/doubler2.png
		blendfunc Add
		tcmod Scroll .9 0
		rgbGen identity
	}
}

// When player uses HI_MEDKIT
models/powerups/regen1
{
	cull disable
	{
		clampmap models/powerups/regen1.png
		tcmod rotate -420
		blendfunc Add
		rgbGen entity
	}
	{
		clampmap models/powerups/regen2.png
		tcmod rotate -640
		blendfunc Add
		rgbGen entity
	}
}

