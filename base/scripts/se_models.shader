// Shaders for the models.

// Player shaders
// Raph's eyes should always be fullwhite, currently unused
models/players/raph/eyes
{
	cull disable
	{
		map $whiteimage
	}
}

models/players/raph/belt
{
	//cull disable
	{
		map models/players/raph/belt.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/body
{
	//cull disable
	{
		map models/players/raph/body.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/head
{
	//cull disable
	{
		map models/players/raph/head.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/legs
{
	//cull disable
	{
		map models/players/raph/legs.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_blue
{
	//cull disable
	{
		map models/players/raph/belt_blue.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/body_blue
{
	//cull disable
	{
		map models/players/raph/body_blue.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/head_blue
{
	//cull disable
	{
		map models/players/raph/head_blue.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_red
{
	//cull disable
	{
		map models/players/raph/belt_red.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/body_red
{
	//cull disable
	{
		map models/players/raph/body_red.png
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

models/players/raph/head_red
{
	//cull disable
	{
		map models/players/raph/head_red.png
		//tcGen environment
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

models/shurikens/shurikenlaser
{
	{
		map models/powerups/holdable/shurikenlaser_1.png
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
	cull disable
	{
		map models/powerups/health/health.png
		rgbGen lightingDiffuse
	}
}

models/powerups/instant/defense
{
	//cull disable
	{
		map models/powerups/instant/defense.png
		rgbGen lightingDiffuse
	}
}

// Based on "powerups/invisibility" (se_powerups.shader)
models/powerups/instant/invis
{
	//cull disable
	{
		//map models/powerups/instant/invis.png
		map textures/effects/invismap.tga
			//map textures/sfx/specular.tga
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 0.15 0 0.25
		tcGen environment
	}
}

models/powerups/instant/invul
{
	//cull disable
	{
		map models/powerups/instant/invul.png
		rgbGen lightingDiffuse
	}
}

models/powerups/instant/speed
{
	//cull disable
	{
		map models/powerups/instant/speed.png
		rgbGen lightingDiffuse
	}
}

models/powerups/instant/strength
{
	//cull disable
	{
		map models/powerups/instant/strength.png
		rgbGen lightingDiffuse
	}
}

// CTF FLAGS / MISSIONPACK CTF FLAGS
// Below CTF flay shaders from MISSIONPACK demo pak0.pk3/scripts/models.shader
models/flags/pole
{
	{
		// Turtle Man: Changed image.
		//map models/flags/skull.tga
		//map textures/effects/tinfx.tga
		map models/flags/pole.tga
		tcGen environment
		rgbGen lightingDiffuse
	}
}

models/flags/skull
{
	{
		map models/flags/skull.tga
		//tcGen environment
		rgbGen lightingDiffuse
	}
}

//models/flags/r_flag
//{
//	cull disable
//	deformVertexes wave 100 sin 0 3 0 .7
//
//	{
//		map models/flags/r_flag.tga
//	}
//}

//models/flags/b_flag
//{
//	cull disable
//	deformVertexes wave 100 sin 0 3 0 .7
//
//	{
//		map models/flags/b_flag.tga
//	}
//}

models/flags/r_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/r_flag2.tga
		rgbGen identity
	}
	{
		map models/flags/red_fx.tga
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/r_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flags/b_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/b_flag2.tga
		rgbGen identity
	}
	{
		map models/flags/blue_fx.tga
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/b_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


// Below CTF flag shaders from MISSIONPACK demo pak0.pk3/scripts/models2.shader
models/flags/n_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/n_flag2.tga
		rgbGen identity
	}
	{
		map models/flags/neutral_fx.tga
		tcGen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/n_flag2.tga
		blendFunc blend
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/r_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/r_flag2.tga
		rgbGen identity
	}
	{
		map models/flag2/red_fx.tga
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/r_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/b_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/b_flag2.tga
		rgbGen identity
	}
	{
		map models/flag2/blue_fx.tga
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/b_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flag2/w_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flag2/w_flag2.tga
		rgbGen identity
	}
	{
		map models/flag2/neutral_fx.tga
		tcGen environment
		//tcmod scale 9 3
		//tcmod scroll .1 .7

		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flag2/w_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}
