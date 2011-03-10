// Shaders for the new (missionpack based) UI

// See menu/art/titlebanner (se_menu.shader)
ui/assets/titlebanner
{
	nomipmaps
	{
		map menu/art/titlebanner.png
		blendfunc blend
	}
}

// Copied from se_menu.shader "menuback[RagePro]"
ui/assets/out_menuback
{
	nomipmaps
	{
		map menu/art/menuback.png
		tcMod scroll .04  .02
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

