// Shaders for the new (missionpack based) UI

// See menu/art/titlebanner (se_menu.shader)
ui/assets/titlebanner
{
	nomipmaps
	{
		map ui/assets/titlebanner.png
		blendfunc blend
	}
}

// Copied from se_menu.shader "menuback[RagePro]"
ui/assets/out_menuback
{
	nomipmaps
	{
		map menu/art/menuback
		tcMod scroll .04  .02
	}
}

ui/assets/3_cursor3
{
	nomipmaps
	{
		map menu/art/3_cursor2
	}
}

// Used by Team Arena scriptable UI HUD
ui/assets/selectcursor
{
	nomipmaps
	{
		map menu/art/3_cursor2
	}
}

ui/assets/sizecursor
{
	nomipmaps
	{
		// TODO: Add the correct image.
		map menu/art/3_cursor2
	}
}

// MISSIONPACK shaders (Used by "statusbar"/hud)
ui/assets/statusbar/flag_in_base
{
	nomipmaps
	{
		map icons/iconf_neutral1
	}
}

// Colormapping "flag_capture" doesn't work if there is a shader?
//ui/assets/statusbar/flag_capture
//{
	//nomipmaps
	//{
	//	map icons/iconf_neutral2
	//}
//}

ui/assets/statusbar/flag_missing
{
	nomipmaps
	{
		map icons/iconf_neutral3
	}
}

