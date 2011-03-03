// Shaders for the missionpack based UI

// See menu/art/titlebanner (se_menu.shader)
ui/assets/titlebanner
{
	nomipmaps
	{
		map menu/art/titlebanner.png
		blendfunc blend
	}
}

// See menuback (se_menu.shader)
ui/assets/out_menuback
{
	nomipmaps
	{
		map menu/art/menuback.png
		tcMod scroll 0.04 0.02
	}
	{
		map menu/art/menuback2.png
		blendFunc blend
		tcMod scale 0.5 0.5
		tcMod scroll -0.02 -0.01
		rgbGen const ( 0.35 0.35 0.35 )
		alphaGen const 0.95
	}
}

ui/assets/dialog_back
{
	nomipmaps
	{
		map menu/art/cut_frame.png
		blendfunc blend
	}
}

ui/assets/3_cursor2
{
	nomipmaps
	{
		map menu/art/3_cursor2.png
	}
}

ui/assets/3_cursor3
{
	nomipmaps
	{
		map menu/art/3_cursor2.png
	}
}

// Used by Team Arena scriptable UI HUD
ui/assets/selectcursor
{
	nomipmaps
	{
		map menu/art/3_cursor2.png
	}
}

ui/assets/sizecursor
{
	nomipmaps
	{
		// TODO: Add the correct image.
		map menu/art/3_cursor2.png
	}
}

// MISSIONPACK shaders (Used by "statusbar"/hud)
ui/assets/statusbar/flag_in_base
{
	nomipmaps
	{
		map icons/iconf_neutral1.png
	}
}

// Colormapping "flag_capture" doesn't work if there is a shader?
//ui/assets/statusbar/flag_capture
//{
	//nomipmaps
	//{
	//	map icons/iconf_neutral2.png
	//}
//}

ui/assets/statusbar/flag_missing
{
	nomipmaps
	{
		map icons/iconf_neutral3.png
	}
}

