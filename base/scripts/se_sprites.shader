// Sprite shaders.
// A sprite is a image in the 3D world that always faces the camera.

// Turtle Man: Drawn over player's head when in console or chat mode.
sprites/talkBalloon
{
	entityMergable // Turtle Man: Should be okay.
	{
		map sprites/talkBalloon.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

waterBubble
{
	sort underwater
	cull none
	entityMergable
	{
		map sprites/bubble.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

// Rocket trails and stuff.
smokePuff
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

// Turtle Man: Drawn at player's feet when player has speed powerups
hasteSmokePuff
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
                //blendfunc GL_ONE GL_ONE
		rgbGen		vertex
		alphaGen	vertex
	}
}

// Turtle Man: Empty transparent image.
smokePuffRagePro
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuffragepro.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Turtle Man:
// Used for breath, g_enableBreath 1 in console
// or "enableBreath" "1" in worldspawn
shotgunSmokePuff
{
	entityMergable // Turtle Man: Should be okay.
	cull none
	{
		map gfx/misc/smokepuff2b.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen entity
		rgbGen entity
	}
}

// From Team Arena's pak0.pk3/scripts/gfx2.shader
nailtrail
{
	sort nearest
	cull none
	{
		clampmap models/weaphits/nailtrail.png
		blendFunc Add
		rgbGen vertex
		tcMod rotate -30
	}
}

flareShader
{
	//entityMergable // Turtle Man: Should be okay?
	cull none
	{
		map gfx/misc/flare.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

sun
{
	//entityMergable // Turtle Man: Should be okay?
	cull none
	{
		map gfx/misc/sun.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

// Turtle Man: Used in Team modes, it is above your teammate's head.
sprites/team_red
{
	entityMergable // Turtle Man: Should be okay.
	nomipmaps
	nopicmip
	{
		map sprites/friend_r.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/team_blue
{
	entityMergable // Turtle Man: Should be okay.
	nomipmaps
	nopicmip
	{
		map sprites/friend_b.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// note: plasma trail uses railDisc
// Can't use "entityMergable" each rotates seperate.
sprites/plasma1
{
	cull disable
	{
		clampmap sprites/plasmaa.png
		blendfunc GL_ONE GL_ONE
		tcMod rotate 931
	}

	// Turtle Man: Looks cool.
	{
		clampmap sprites/plasmaa.png
		blendfunc GL_ONE GL_ONE
		tcMod rotate 460
	}
}

