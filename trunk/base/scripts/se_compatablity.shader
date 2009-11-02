// Shaders that are not used by TMNT Arena,
//  but we have them for "quake3 engine compatablity"... They may be removed leter.

// Turtle Man: TMNT Arena uses shader name set in model, "models/misc/telep"
//             While Q3 uses hardcoded name.
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

// TMNT Arena no longer uses "no logo" version
menubacknologo
{
	nopicmip
	nomipmaps
	{
		map menu/art/menuback
		//blendFunc GL_ONE GL_ZERO
		tcMod scroll .04  .02
		//tcMod scale 1.2 1.2
	}
	//{
	//	map menu/art/menuback
	//	blendFunc add
	//	tcMod scroll .02  .01
	//	tcMod scale .9 .9
	//	tcMod turb 0 .1 0 .1
	//}
}

// Turtle Man: Removed HI_TELEPORTER
icons/teleporter
{
	nopicmip
	{
		map icons/teleporter.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Turtle Man: So Quake3 finds our powerup icons...
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

// Medals not used by TMNT.
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


// Turtle Man: Renamed 3 (of 4) health icons;
// Q3: green - TMNT: small
// Q3: yellow - TMNT: medium
// Q3: red - TMNT: large
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

// icons/iconh_mega -- same as in q3.

// Turtle Man: Renamed to sprites/talkBalloon ...
sprites/balloon3
{
	{
		map sprites/talkBalloon.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Turtle Man: Used in Team modes, it is above your teammate's head.
sprites/foe
{
	nomipmaps
	{
		map sprites/friend_r.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

