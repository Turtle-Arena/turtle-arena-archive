// Shaders that are not used by Turtle Arena,
//  but we have them for "quake3 engine compatablity"... They may be removed leter.

// ZTM: Turtle Arena uses the shader name set in model, "models/misc/telep"
//             While Q3 uses a hardcoded name.
teleportEffect
{
	cull disable
	{
		map models/misc/telep.png
		tcGen environment
		//tcMod turb 0 0.1 0 0.1
		//tcmod rotate 311
		//tcmod scroll 1 1
		blendfunc Add
		rgbGen entity
	}
}

// Turtle Arena no longer uses "no logo" version
menubacknologo
{
	nomipmaps
	{
		map menu/art/menuback
		tcMod scroll 0.04 0.02
	}
}

// ZTM: Removed HI_TELEPORTER
icons/teleporter
{
	nopicmip
	{
		map icons/teleporter.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// ZTM: Removed PW_REGEN
icons/regen
{
	nopicmip
	{
		map icons/regen.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

powerups/regen
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/regenmap2.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
	}
}

// ZTM: So Quake3 finds our powerup icons...
icons/envirosuit
{
	nopicmip
	{
		map icons/defense.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/quad
{
	nopicmip
	{
		map icons/strength.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/haste
{
	nopicmip
	{
		map icons/speed.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Medals that are not used by Turtle Arena.
medal_excellent
{
	nopicmip
	{
		clampmap menu/medals/medal_excellent.tga
		blendFunc blend
	}
}

medal_impressive
{
	nopicmip
	{
		clampmap menu/medals/medal_impressive.tga
		blendFunc blend
	}
}

medal_frags
{
	nopicmip
	{
		clampmap menu/medals/medal_frags.tga
		blendFunc blend
	}
}

medal_gauntlet
{
	nopicmip
	{
		clampmap menu/medals/medal_gauntlet.tga
		blendFunc blend
	}
}


// ZTM: Renamed 3 (of 4) health icons;
// Q3: green - Turtle Arena: small
// Q3: yellow - Turtle Arena: medium
// Q3: red - Turtle Arena: large
icons/iconh_green
{
	nopicmip
	{
		map icons/iconh_small.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconh_yellow
{
	nopicmip
	{
		map icons/iconh_medium.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconh_red
{
	nopicmip
	{
		map icons/iconh_large.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}

}

// icons/iconh_mega -- same as in Q3.

// ZTM: Renamed to sprites/talkBalloon ...
sprites/balloon3
{
	{
		map sprites/talkBalloon.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// ZTM: Used in Team modes, it is above your teammate's head.
sprites/foe
{
	nomipmaps
	{
		map sprites/friend_r.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// ZTM: Not used by Turtle Arena
gfx/2d/crosshaire
{
	nopicmip
	{
		map gfx/2d/crosshaire.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairf
{
	nopicmip
	{
		map gfx/2d/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairg
{
	nopicmip
	{
		map gfx/2d/crosshairg.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairh
{
	nopicmip
	{
		map gfx/2d/crosshairh.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/2d/crosshairi
{
	nopicmip
	{
		map gfx/2d/crosshairi.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}

}
gfx/2d/crosshairj
{
	nopicmip
	{
		map gfx/2d/crosshairj.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}
gfx/2d/crosshairk
{
	nopicmip
	{
		map gfx/2d/crosshairk.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

