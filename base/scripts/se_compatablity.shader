// Shaders that are not used by TMNT Arena,
//  but we have them for "quake3 engine compatablity"... They may be removed leter.

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
		map icons/defense.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/quad
{
	nopicmip
	{
		map icons/strength.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
icons/haste
{
	nopicmip
	{
		map icons/speed.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Medals not used by TMNT.
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
		map icons/iconh_small.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconh_yellow
{
	nopicmip
	{
		map icons/iconh_medium.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconh_red
{
	nopicmip
	{
		map icons/iconh_large.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}

}

// icons/iconh_mega -- same as in q3.

// missionpack icons (That are not used by TMNT Arena, delete?)

icons/iconw_chaingun
{
	nopicmip
	{
		map icons/iconw_chaingun.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconw_chaingun_cl1
{
	nopicmip
	{
		map icons/iconw_chaingun_cl1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconw_chaingun_cl2
{
	nopicmip
	{
		map icons/iconw_chaingun_cl2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconw_kamikaze
{
	nopicmip
	{
		map icons/iconw_kamikaze.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconw_nailgun
{
	nopicmip
	{
		map icons/iconw_nailgun.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconw_proxlauncher
{
	nopicmip
	{
		map icons/iconw_proxlauncher.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_chaingun
{
	nopicmip
	{
		map icons/icona_chaingun.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_proxlauncher
{
	nopicmip
	{
		map icons/icona_proxlauncher.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_nailgun
{
	nopicmip
	{
		map icons/icona_nailgun.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// openarena icons --domination icons ???

icons/icona_red
{
	nopicmip
	{
		map icons/icona_red.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_blue
{
	nopicmip
	{
		map icons/icona_blue.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_white
{
	nopicmip
	{
		map icons/icona_white.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconb_red
{
	nopicmip
	{
		map icons/iconb_red.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconb_blue
{
	nopicmip
	{
		map icons/iconb_blue.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/iconb_white
{
	nopicmip
	{
		map icons/iconb_white.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_red
{
	nopicmip
	{
		map icons/icona_red.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/icona_blue
{
	nopicmip
	{
		map icons/icona_blue.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// Turtle Man: Renamed to sprites/talkBalloon ...
sprites/balloon3
{
	{
		map sprites/talkBalloon.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}



// Q3 shaders that are not used by Q3. (remove???)
gfx/2d/menuinfo
{
	nopicmip
	{
		map gfx/2d/menuinfo.tga
	}
}

gfx/2d/menuinfo2
{
	nopicmip
	{
		map gfx/2d/menuinfo2.tga
	}
}

gfx/2d/quit
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/quit.tga
	}
}

gfx/2d/cursor
{
    nopicmip
	nomipmaps
    {
        map gfx/2d/cursor.tga
    }
}

gfx/2d/assault1d
{
	nopicmip
	{
		map gfx/2d/assault1d.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/armor1h
{
	nopicmip
	{
		map gfx/2d/armor1h.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/health
{
	nopicmip
	{
		map gfx/2d/health.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
gfx/2d/blank
{
	nopicmip
	{
		map gfx/2d/blank.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
