/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2010-2011 by Zack "ZTurtleMan" Middleton

This file is part of Turtle Arena source code.

Turtle Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Turtle Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Turtle Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

CONTROLS MAIN MENU

=======================================================================
*/

#include "ui_local.h"

#if defined TA_MISC || defined TA_SPLITVIEW

#define SETUP_MENU_VERTICAL_SPACING		34

#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"
#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define ID_CUSTOMIZECONTROLS	10
#ifdef TA_SPLITVIEW
#define ID_CUSTOMIZECONTROLS2	11
#define ID_CUSTOMIZECONTROLS3	12
#define ID_CUSTOMIZECONTROLS4	13
#endif
#define ID_BACK					14


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;

	menutext_s		player;
#ifdef TA_SPLITVIEW
	menutext_s		player2;
	menutext_s		player3;
	menutext_s		player4;
#endif

	menubitmap_s	back;
} controlsMainMenu_t;

static controlsMainMenu_t	controlsMainMenu;


/*
===============
UI_ControlsMainMenu_Event
===============
*/
static void UI_ControlsMainMenu_Event( void *ptr, int event ) {
#ifdef TA_SPLITVIEW
	int lc;
#endif

	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {

	case ID_CUSTOMIZECONTROLS:
#ifdef TA_SPLITVIEW
	case ID_CUSTOMIZECONTROLS2:
	case ID_CUSTOMIZECONTROLS3:
	case ID_CUSTOMIZECONTROLS4:
		lc = ((menucommon_s*)ptr)->id - ID_CUSTOMIZECONTROLS;
		UI_ControlsMenu(lc);
#else
		UI_ControlsMenu();
#endif
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_ControlsMain_MenuInit
===============
*/
static void UI_ControlsMain_MenuInit( void ) {
	int				y;

	UI_ControlsMain_Cache();

	memset( &controlsMainMenu, 0, sizeof(controlsMainMenu) );
	controlsMainMenu.menu.wrapAround = qtrue;
	controlsMainMenu.menu.fullscreen = qtrue;

	controlsMainMenu.banner.generic.type			= MTYPE_BTEXT;
	controlsMainMenu.banner.generic.x				= 320;
	controlsMainMenu.banner.generic.y				= 16;
	controlsMainMenu.banner.string					= "CONTROLS";
	controlsMainMenu.banner.color					= text_banner_color;
	controlsMainMenu.banner.style					= UI_CENTER;

	controlsMainMenu.framel.generic.type			= MTYPE_BITMAP;
	controlsMainMenu.framel.generic.name			= ART_FRAMEL;
	controlsMainMenu.framel.generic.flags			= QMF_INACTIVE;
	controlsMainMenu.framel.generic.x				= 0;
	controlsMainMenu.framel.generic.y				= 78;
	controlsMainMenu.framel.width  					= 256;
	controlsMainMenu.framel.height  				= 329;

	controlsMainMenu.framer.generic.type			= MTYPE_BITMAP;
	controlsMainMenu.framer.generic.name			= ART_FRAMER;
	controlsMainMenu.framer.generic.flags			= QMF_INACTIVE;
	controlsMainMenu.framer.generic.x				= 376;
	controlsMainMenu.framer.generic.y				= 76;
	controlsMainMenu.framer.width  					= 256;
	controlsMainMenu.framer.height  				= 334;

#ifdef TA_SPLITVIEW
	y = (SCREEN_HEIGHT - 4*SETUP_MENU_VERTICAL_SPACING) * 0.5f;
#else
	y = (SCREEN_HEIGHT - SETUP_MENU_VERTICAL_SPACING) * 0.5f;
#endif
	controlsMainMenu.player.generic.type		= MTYPE_PTEXT;
	controlsMainMenu.player.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	controlsMainMenu.player.generic.x			= 320;
	controlsMainMenu.player.generic.y			= y;
	controlsMainMenu.player.generic.id			= ID_CUSTOMIZECONTROLS;
	controlsMainMenu.player.generic.callback	= UI_ControlsMainMenu_Event;
#ifdef TA_SPLITVIEW
	controlsMainMenu.player.string				= "Player 1";
#else
	controlsMainMenu.player.string				= "Player";
#endif
	controlsMainMenu.player.color				= text_big_color;
	controlsMainMenu.player.style				= UI_CENTER;

#ifdef TA_SPLITVIEW
	y += SETUP_MENU_VERTICAL_SPACING;
	controlsMainMenu.player2.generic.type		= MTYPE_PTEXT;
	controlsMainMenu.player2.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	controlsMainMenu.player2.generic.x			= 320;
	controlsMainMenu.player2.generic.y			= y;
	controlsMainMenu.player2.generic.id			= ID_CUSTOMIZECONTROLS2;
	controlsMainMenu.player2.generic.callback	= UI_ControlsMainMenu_Event;
	controlsMainMenu.player2.string				= "Player 2";
	controlsMainMenu.player2.color				= text_big_color;
	controlsMainMenu.player2.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	controlsMainMenu.player3.generic.type		= MTYPE_PTEXT;
	controlsMainMenu.player3.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	controlsMainMenu.player3.generic.x			= 320;
	controlsMainMenu.player3.generic.y			= y;
	controlsMainMenu.player3.generic.id			= ID_CUSTOMIZECONTROLS3;
	controlsMainMenu.player3.generic.callback	= UI_ControlsMainMenu_Event;
	controlsMainMenu.player3.string				= "Player 3";
	controlsMainMenu.player3.color				= text_big_color;
	controlsMainMenu.player3.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	controlsMainMenu.player4.generic.type		= MTYPE_PTEXT;
	controlsMainMenu.player4.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	controlsMainMenu.player4.generic.x			= 320;
	controlsMainMenu.player4.generic.y			= y;
	controlsMainMenu.player4.generic.id			= ID_CUSTOMIZECONTROLS4;
	controlsMainMenu.player4.generic.callback	= UI_ControlsMainMenu_Event;
	controlsMainMenu.player4.string				= "Player 4";
	controlsMainMenu.player4.color				= text_big_color;
	controlsMainMenu.player4.style				= UI_CENTER;
#endif

	controlsMainMenu.back.generic.type				= MTYPE_BITMAP;
	controlsMainMenu.back.generic.name				= ART_BACK0;
	controlsMainMenu.back.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	controlsMainMenu.back.generic.id				= ID_BACK;
	controlsMainMenu.back.generic.callback			= UI_ControlsMainMenu_Event;
	controlsMainMenu.back.generic.x					= 0;
	controlsMainMenu.back.generic.y					= 480-64;
	controlsMainMenu.back.width						= 128;
	controlsMainMenu.back.height					= 64;
	controlsMainMenu.back.focuspic					= ART_BACK1;

	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.banner );
	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.framel );
	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.framer );

	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.player );
#ifdef TA_SPLITVIEW
	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.player2 );
	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.player3 );
	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.player4 );
#endif

	Menu_AddItem( &controlsMainMenu.menu, &controlsMainMenu.back );
}

/*
=================
UI_ControlsMain_Cache
=================
*/
void UI_ControlsMain_Cache( void )
{
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
}

/*
===============
UI_ControlsMainMenu
===============
*/
void UI_ControlsMainMenu( void )
{
	UI_ControlsMain_MenuInit();
	UI_PushMenu( &controlsMainMenu.menu );
}

#endif // TA_MISC || TA_SPLITVIEW
