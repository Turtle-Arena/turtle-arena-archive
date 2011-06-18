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

MULTIPLAYER MENU

=======================================================================
*/

#include "ui_local.h"

#ifdef TA_MISC

#define SETUP_MENU_VERTICAL_SPACING		34

#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"
#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define ID_CREATEGAME			10
#define ID_JOINSEARCH			11
#define ID_JOINSPECIFY			12
#define ID_CUSTOMIZEPLAYER		13
#ifdef TA_SPLITVIEW
#define ID_CUSTOMIZEPLAYER2		14
#define ID_CUSTOMIZEPLAYER3		15
#define ID_CUSTOMIZEPLAYER4		16
#endif
#define ID_BACK					17


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;

	menutext_s		creategame;
	menutext_s		joinsearch;
	menutext_s		joinspecify;
	menutext_s		setupplayer;
#ifdef TA_SPLITVIEW
	menutext_s		setupplayer2;
	menutext_s		setupplayer3;
	menutext_s		setupplayer4;
#endif

	menubitmap_s	back;
} multiplayerMenu_t;

static multiplayerMenu_t	multiplayerMenu;


/*
===============
UI_MultiplayerMenu_Event
===============
*/
static void UI_MultiplayerMenu_Event( void *ptr, int event ) {
#ifdef TA_SPLITVIEW
	int lc;
#endif

	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {

	case ID_CREATEGAME:
		UI_StartServerMenu( qtrue );
		break;

	case ID_JOINSEARCH:
		UI_ArenaServersMenu();
		break;

	case ID_JOINSPECIFY:
		UI_SpecifyServerMenu(qfalse);
		break;

	case ID_CUSTOMIZEPLAYER:
#ifdef TA_SPLITVIEW
	case ID_CUSTOMIZEPLAYER2:
	case ID_CUSTOMIZEPLAYER3:
	case ID_CUSTOMIZEPLAYER4:
		lc = ((menucommon_s*)ptr)->id - ID_CUSTOMIZEPLAYER;
		UI_PlayerSettingsMenu(lc);
#else
		UI_PlayerSettingsMenu();
#endif
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_Multiplayer_MenuInit
===============
*/
static void UI_Multiplayer_MenuInit( void ) {
	int				y;

	UI_Multiplayer_Cache();

	memset( &multiplayerMenu, 0, sizeof(multiplayerMenu) );
	multiplayerMenu.menu.wrapAround = qtrue;
	multiplayerMenu.menu.fullscreen = qtrue;

	multiplayerMenu.banner.generic.type				= MTYPE_BTEXT;
	multiplayerMenu.banner.generic.x				= 320;
	multiplayerMenu.banner.generic.y				= 16;
	multiplayerMenu.banner.string					= "MULTIPLAYER";
	multiplayerMenu.banner.color					= text_banner_color;
	multiplayerMenu.banner.style					= UI_CENTER;

	multiplayerMenu.framel.generic.type				= MTYPE_BITMAP;
	multiplayerMenu.framel.generic.name				= ART_FRAMEL;
	multiplayerMenu.framel.generic.flags			= QMF_INACTIVE;
	multiplayerMenu.framel.generic.x				= 0;
	multiplayerMenu.framel.generic.y				= 78;
	multiplayerMenu.framel.width  					= 256;
	multiplayerMenu.framel.height  					= 329;

	multiplayerMenu.framer.generic.type				= MTYPE_BITMAP;
	multiplayerMenu.framer.generic.name				= ART_FRAMER;
	multiplayerMenu.framer.generic.flags			= QMF_INACTIVE;
	multiplayerMenu.framer.generic.x				= 376;
	multiplayerMenu.framer.generic.y				= 76;
	multiplayerMenu.framer.width  					= 256;
	multiplayerMenu.framer.height  					= 334;

#ifdef TA_SPLITVIEW
	y = (SCREEN_HEIGHT - 7*SETUP_MENU_VERTICAL_SPACING) * 0.5f;
#else
	y = (SCREEN_HEIGHT - 4*SETUP_MENU_VERTICAL_SPACING) * 0.5f;
#endif
	multiplayerMenu.creategame.generic.type			= MTYPE_PTEXT;
	multiplayerMenu.creategame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.creategame.generic.x			= 320;
	multiplayerMenu.creategame.generic.y			= y;
	multiplayerMenu.creategame.generic.id			= ID_CREATEGAME;
	multiplayerMenu.creategame.generic.callback		= UI_MultiplayerMenu_Event;
	multiplayerMenu.creategame.string				= "Create";
	multiplayerMenu.creategame.color				= text_big_color;
	multiplayerMenu.creategame.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	multiplayerMenu.joinsearch.generic.type			= MTYPE_PTEXT;
	multiplayerMenu.joinsearch.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.joinsearch.generic.x			= 320;
	multiplayerMenu.joinsearch.generic.y			= y;
	multiplayerMenu.joinsearch.generic.id			= ID_JOINSEARCH;
	multiplayerMenu.joinsearch.generic.callback		= UI_MultiplayerMenu_Event;
	multiplayerMenu.joinsearch.string				= "Search";
	multiplayerMenu.joinsearch.color				= text_big_color;
	multiplayerMenu.joinsearch.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	multiplayerMenu.joinspecify.generic.type		= MTYPE_PTEXT;
	multiplayerMenu.joinspecify.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.joinspecify.generic.x			= 320;
	multiplayerMenu.joinspecify.generic.y			= y;
	multiplayerMenu.joinspecify.generic.id			= ID_JOINSPECIFY;
	multiplayerMenu.joinspecify.generic.callback	= UI_MultiplayerMenu_Event;
	multiplayerMenu.joinspecify.string				= "Specify IP";
	multiplayerMenu.joinspecify.color				= text_big_color;
	multiplayerMenu.joinspecify.style				= UI_CENTER;

#ifdef TA_SPLITVIEW
	y += 2*SETUP_MENU_VERTICAL_SPACING;
#else
	y += SETUP_MENU_VERTICAL_SPACING;
#endif
	multiplayerMenu.setupplayer.generic.type		= MTYPE_PTEXT;
	multiplayerMenu.setupplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.setupplayer.generic.x			= 320;
	multiplayerMenu.setupplayer.generic.y			= y;
	multiplayerMenu.setupplayer.generic.id			= ID_CUSTOMIZEPLAYER;
	multiplayerMenu.setupplayer.generic.callback	= UI_MultiplayerMenu_Event;
#ifdef TA_SPLITVIEW
	multiplayerMenu.setupplayer.string				= "Setup Player 1";
#else
	multiplayerMenu.setupplayer.string				= "Setup Player";
#endif
	multiplayerMenu.setupplayer.color				= text_big_color;
	multiplayerMenu.setupplayer.style				= UI_CENTER;

#ifdef TA_SPLITVIEW
	y += SETUP_MENU_VERTICAL_SPACING;
	multiplayerMenu.setupplayer2.generic.type		= MTYPE_PTEXT;
	multiplayerMenu.setupplayer2.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.setupplayer2.generic.x			= 320;
	multiplayerMenu.setupplayer2.generic.y			= y;
	multiplayerMenu.setupplayer2.generic.id			= ID_CUSTOMIZEPLAYER2;
	multiplayerMenu.setupplayer2.generic.callback	= UI_MultiplayerMenu_Event;
	multiplayerMenu.setupplayer2.string				= "Setup Player 2";
	multiplayerMenu.setupplayer2.color				= text_big_color;
	multiplayerMenu.setupplayer2.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	multiplayerMenu.setupplayer3.generic.type		= MTYPE_PTEXT;
	multiplayerMenu.setupplayer3.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.setupplayer3.generic.x			= 320;
	multiplayerMenu.setupplayer3.generic.y			= y;
	multiplayerMenu.setupplayer3.generic.id			= ID_CUSTOMIZEPLAYER3;
	multiplayerMenu.setupplayer3.generic.callback	= UI_MultiplayerMenu_Event;
	multiplayerMenu.setupplayer3.string				= "Setup Player 3";
	multiplayerMenu.setupplayer3.color				= text_big_color;
	multiplayerMenu.setupplayer3.style				= UI_CENTER;

	y += SETUP_MENU_VERTICAL_SPACING;
	multiplayerMenu.setupplayer4.generic.type		= MTYPE_PTEXT;
	multiplayerMenu.setupplayer4.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.setupplayer4.generic.x			= 320;
	multiplayerMenu.setupplayer4.generic.y			= y;
	multiplayerMenu.setupplayer4.generic.id			= ID_CUSTOMIZEPLAYER4;
	multiplayerMenu.setupplayer4.generic.callback	= UI_MultiplayerMenu_Event;
	multiplayerMenu.setupplayer4.string				= "Setup Player 4";
	multiplayerMenu.setupplayer4.color				= text_big_color;
	multiplayerMenu.setupplayer4.style				= UI_CENTER;
#endif

	multiplayerMenu.back.generic.type				= MTYPE_BITMAP;
	multiplayerMenu.back.generic.name				= ART_BACK0;
	multiplayerMenu.back.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	multiplayerMenu.back.generic.id					= ID_BACK;
	multiplayerMenu.back.generic.callback			= UI_MultiplayerMenu_Event;
	multiplayerMenu.back.generic.x					= 0;
	multiplayerMenu.back.generic.y					= 480-64;
	multiplayerMenu.back.width						= 128;
	multiplayerMenu.back.height						= 64;
	multiplayerMenu.back.focuspic					= ART_BACK1;

	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.banner );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.framel );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.framer );

	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.creategame );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.joinsearch );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.joinspecify );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.setupplayer );
#ifdef TA_SPLITVIEW
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.setupplayer2 );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.setupplayer3 );
	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.setupplayer4 );
#endif

	Menu_AddItem( &multiplayerMenu.menu, &multiplayerMenu.back );
}

/*
=================
UI_Multiplayer_Cache
=================
*/
void UI_Multiplayer_Cache( void )
{
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
}

/*
===============
UI_MultiplayerMenu
===============
*/
void UI_MultiplayerMenu( void )
{
	UI_Multiplayer_MenuInit();
	UI_PushMenu( &multiplayerMenu.menu );
}

#endif // TA_MISC
