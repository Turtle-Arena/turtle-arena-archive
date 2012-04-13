// Sprite shaders.
// A sprite is a image in the 3D world that always faces the camera.

// ZTM: Drawn over player's head when in console or chat mode.
sprites/talkBalloon
{
	nomipmaps
	entityMergable
	{
		map sprites/talkBalloon.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

waterBubble
{
	sort underwater
	cull none
	nomipmaps
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
	nomipmaps
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

// ZTM: Drawn at player's feet when player has speed powerups
hasteSmokePuff
{
	cull none
	nomipmaps
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
                //blendfunc GL_ONE GL_ONE
		rgbGen		vertex
		alphaGen	vertex
	}
}

// ZTM: Empty transparent image.
smokePuffRagePro
{
	cull none
	nomipmaps
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuffragepro.png
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// ZTM:
// Used for breath, g_enableBreath 1 in console
// or "enableBreath" "1" in worldspawn
shotgunSmokePuff
{
	nomipmaps
	entityMergable // ZTM: Should be okay.
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
	nomipmaps
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
	nomipmaps
	//entityMergable // ZTM: Should be okay?
	cull none
	{
		map gfx/misc/flare.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

sun
{
	nomipmaps
	//entityMergable // ZTM: Should be okay?
	cull none
	{
		map gfx/misc/sun.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// ZTM: Used in Team modes, it is above your teammate's head.
sprites/team_red
{
	entityMergable
	nomipmaps
	{
		map sprites/friend_r.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/team_blue
{
	entityMergable
	nomipmaps
	{
		map sprites/friend_b.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Shows over the target entity
sprites/target
{
	entityMergable
	nomipmaps
	{
		map sprites/target.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// note: plasma trail uses railDisc
// Can't use "entityMergable" each rotates seperate.
sprites/plasma1
{
	nomipmaps
	cull disable
	{
		clampmap sprites/plasmaa.png
		blendfunc GL_ONE GL_ONE
		tcMod rotate 931
	}

	// ZTM: Looks cool.
	{
		clampmap sprites/plasmaa.png
		blendfunc GL_ONE GL_ONE
		tcMod rotate 460
	}
}

