// Sprite shaders.
// A sprite is a image in the 3D world that always faces the camera.

// Talk balloon
// Q3: sprites/balloon3 -> map sprites/balloon4.tga
sprites/talkBalloon
{
	{
		map sprites/talkBalloon.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// works with Q3...
sprites/balloon3
{
	{
		map sprites/talkBalloon.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

waterBubble
{
	sort underwater
	cull none
	entityMergable
	{
		map sprites/bubble.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

smokePuff
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

// Draw at player's feet when player has speed powerups
hasteSmokePuff
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuff3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
                //blendfunc GL_ONE GL_ONE
		rgbGen		vertex
		alphaGen	vertex
	}
}

// Empty transparent image.
smokePuffRagePro
{
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuffragepro.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Used for breath, g_enableBreath 1 in console
// or "enableBreath" "1" in worldspawn
shotgunSmokePuff
{
	cull none
	{
		map gfx/misc/smokepuff2b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen entity		
		rgbGen entity
	}
}

flareShader
{
	cull none
	{
		map gfx/misc/flare.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}
sun
{
	cull none
	{
		map gfx/misc/sun.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

// Used in Team modes, it is above your teammate's head.
//  This should be "sprites/friend" not "sprites/foe".
/////              "map sprites/friend1.tga"
sprites/foe
{
	nomipmaps
	nopicmip
	{
		map sprites/foe2.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
