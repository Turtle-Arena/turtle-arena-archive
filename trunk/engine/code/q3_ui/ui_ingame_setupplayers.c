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

SETUP PLAYERS MENU

This is a general select local client menu. Used for accessing menus for a specific local client.
It runs a function, passing the selected client to the function.
Automaticly disables players not in the game, so the function can expect that the client is ingame.
If there is only one local client simply runs the function.

=======================================================================
*/


#include "ui_local.h"

#ifdef TA_SPLITVIEW

//#define INGAME_FRAME					"menu/art/addbotframe"
#define INGAME_FRAME					"menu/art/cut_frame"
#define INGAME_MENU_VERTICAL_SPACING	28

#define ID_BACK					10
#define ID_CUSTOMIZEPLAYER		11 // + MAX_SPLITVIEW



typedef struct {
	menuframework_s	menu;

	menubitmap_s	frame;
	menutext_s		player[MAX_SPLITVIEW];

	menutext_s		back;

	char			bannerString[32];
	char			playerString[MAX_SPLITVIEW][12];
	void 			(*playerfunc)(int);
} setupplayersmenu_t;

static setupplayersmenu_t	s_setupplayers;


/*
=================
SetupPlayers_Event
=================
*/
void SetupPlayers_Event( void *ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	if (((menucommon_s*)ptr)->id >= ID_CUSTOMIZEPLAYER && ((menucommon_s*)ptr)->id < ID_CUSTOMIZEPLAYER+MAX_SPLITVIEW) {
		s_setupplayers.playerfunc( ((menucommon_s*)ptr)->id - ID_CUSTOMIZEPLAYER );
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
=================
UI_SetupPlayersMenu_Draw
=================
*/
static void UI_SetupPlayersMenu_Draw( void ) {
	UI_DrawBannerString( 320, 16, s_setupplayers.bannerString, UI_CENTER, text_banner_color );

	// standard menu drawing
	Menu_Draw( &s_setupplayers.menu );
}


/*
=================
SetupPlayers_MenuInit
=================
*/
void SetupPlayers_MenuInit( uiClientState_t *cs, const char *banner, qboolean disableMissingPlayers ) {
	int		y;
	int		i;

	memset( &s_setupplayers, 0, sizeof(setupplayersmenu_t) );

	SetupPlayers_Cache();

	Q_strncpyz(s_setupplayers.bannerString, banner, sizeof (s_setupplayers.bannerString));
	s_setupplayers.menu.draw = UI_SetupPlayersMenu_Draw;
	s_setupplayers.menu.wrapAround = qtrue;
	s_setupplayers.menu.fullscreen = qfalse;

	s_setupplayers.frame.generic.type		= MTYPE_BITMAP;
	s_setupplayers.frame.generic.flags		= QMF_INACTIVE;
	s_setupplayers.frame.generic.name		= INGAME_FRAME;
	s_setupplayers.frame.generic.x			= 320-233;//142;
	s_setupplayers.frame.generic.y			= 240-166;//118;
	s_setupplayers.frame.width				= 466;//359;
	s_setupplayers.frame.height				= 332;//256;

	y = 96;
	//y = 88;

	y += INGAME_MENU_VERTICAL_SPACING*2;

	for (i = 0; i < MAX_SPLITVIEW; i++) {
		Q_snprintf(s_setupplayers.playerString[i], sizeof (s_setupplayers.playerString[i]), "Player %d", i+1);

		s_setupplayers.player[i].generic.type		= MTYPE_PTEXT;
		s_setupplayers.player[i].generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		s_setupplayers.player[i].generic.x			= 320;
		s_setupplayers.player[i].generic.y			= y;
		s_setupplayers.player[i].generic.id			= ID_CUSTOMIZEPLAYER + i;
		s_setupplayers.player[i].generic.callback	= SetupPlayers_Event;
		s_setupplayers.player[i].string				= s_setupplayers.playerString[i];
		s_setupplayers.player[i].color				= text_big_color;
		s_setupplayers.player[i].style				= UI_CENTER|UI_SMALLFONT;

		if (disableMissingPlayers && cs->lcIndex[i] == -1) {
			s_setupplayers.player[i].generic.flags |= QMF_GRAYED;
		}

		y += INGAME_MENU_VERTICAL_SPACING;
	}

	y += INGAME_MENU_VERTICAL_SPACING*2;
	s_setupplayers.back.generic.type		= MTYPE_PTEXT;
	s_setupplayers.back.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_setupplayers.back.generic.x			= 320;
	s_setupplayers.back.generic.y			= y;
	s_setupplayers.back.generic.id			= ID_BACK;
	s_setupplayers.back.generic.callback	= SetupPlayers_Event;
	s_setupplayers.back.string				= "Back";
	s_setupplayers.back.color				= text_big_color;
	s_setupplayers.back.style				= UI_CENTER|UI_SMALLFONT;

	Menu_AddItem( &s_setupplayers.menu, &s_setupplayers.frame );

	for (i = 0; i < MAX_SPLITVIEW; i++) {
		Menu_AddItem( &s_setupplayers.menu, &s_setupplayers.player[i] );
	}

	Menu_AddItem( &s_setupplayers.menu, &s_setupplayers.back );
}


/*
=================
SetupPlayers_Cache
=================
*/
void SetupPlayers_Cache( void ) {
	trap_R_RegisterShaderNoMip( INGAME_FRAME );
}


/*
=================
UI_SetupPlayersMenu
=================
*/
void UI_SetupPlayersMenu( void (*playerfunc)(int), const char *banner, qboolean disableMissingPlayers ) {
	uiClientState_t	cs;

	trap_GetClientState( &cs );

	// If there is only one local client skip this menu.
	if (cs.numLocalClients <= 1 && disableMissingPlayers) {
		playerfunc(0);
		return;
	}

	SetupPlayers_MenuInit( &cs, banner, disableMissingPlayers );
	s_setupplayers.playerfunc = playerfunc;
	UI_PushMenu( &s_setupplayers.menu );
}
#endif

