// Shaders for the new (missionpack based) UI

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
		//blendFunc GL_ONE GL_ZERO
		tcMod scroll .04  .02
		//tcMod scale 1.2 1.2
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

