/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

INGAME MENU

=======================================================================
*/


#include "ui_local.h"


#define INGAME_FRAME					"menu/art/addbotframe"
//#define INGAME_FRAME					"menu/art/cut_frame"
#define INGAME_MENU_VERTICAL_SPACING	28

#define ID_TEAM					10
#ifdef TA_MISC // INGAME_SERVER_MENU
#define ID_SERVER				11
#else
#define ID_ADDBOTS				11
#define ID_REMOVEBOTS			12
#endif
#define ID_SETUP				13
#define ID_SERVERINFO			14
#define ID_LEAVEARENA			15
#ifndef TA_MISC // INGAME_SERVER_MENU
#define ID_RESTART				16
#endif
#define ID_QUIT					17
#define ID_RESUME				18
#define ID_TEAMORDERS			19
#ifdef TA_MISC // SMART_JOIN_MENU
#define ID_JOINGAME				20
#define ID_SPECTATE				21
#endif
#ifdef TA_MISC
#define ID_CUSTOMIZEPLAYER		22
#endif


typedef struct {
	menuframework_s	menu;

	menubitmap_s	frame;
	menutext_s		team;
	menutext_s		setup;
	menutext_s		server;
	menutext_s		leave;
#ifdef TA_MISC // INGAME_SERVER_MENU
	menutext_s		inserver;
#else
	menutext_s		restart;
	menutext_s		addbots;
	menutext_s		removebots;
#endif
#ifdef TA_MISC
	menutext_s		setupplayer;
#endif
	menutext_s		teamorders;
	menutext_s		quit;
	menutext_s		resume;
} ingamemenu_t;

static ingamemenu_t	s_ingame;


#ifndef TA_MISC // INGAME_SERVER_MENU
/*
=================
InGame_RestartAction
=================
*/
static void InGame_RestartAction( qboolean result ) {
	if( !result ) {
		return;
	}

	UI_PopMenu();
	trap_Cmd_ExecuteText( EXEC_APPEND, "map_restart 0\n" );
}
#endif


/*
=================
InGame_QuitAction
=================
*/
static void InGame_QuitAction( qboolean result ) {
	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}


/*
=================
InGame_Event
=================
*/
void InGame_Event( void *ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_TEAM:
		UI_TeamMainMenu();
		break;

#ifdef TA_MISC // SMART_JOIN_MENU
	case ID_JOINGAME:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team free\n" );
		UI_ForceMenuOff();
		break;

	case ID_SPECTATE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team spectator\n" );
		UI_ForceMenuOff();
		break;
#endif

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_LEAVEARENA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
		break;

#ifndef TA_MISC // INGAME_SERVER_MENU
	case ID_RESTART:
		UI_ConfirmMenu( "RESTART ARENA?", 0, InGame_RestartAction );
		break;
#endif

	case ID_QUIT:
		UI_ConfirmMenu( "EXIT GAME?",  0, InGame_QuitAction );
		break;

	case ID_SERVERINFO:
		UI_ServerInfoMenu();
		break;

#ifdef TA_MISC // INGAME_SERVER_MENU
	case ID_SERVER:
		UI_InServerMenu();
		break;
#else
	case ID_ADDBOTS:
		UI_AddBotsMenu();
		break;

	case ID_REMOVEBOTS:
		UI_RemoveBotsMenu();
		break;
#endif

#ifdef TA_MISC
	case ID_CUSTOMIZEPLAYER:
		UI_PlayerSettingsMenu();
		break;
#endif

	case ID_TEAMORDERS:
		UI_TeamOrdersMenu();
		break;

	case ID_RESUME:
		UI_PopMenu();
		break;
	}
}


/*
=================
InGame_MenuInit
=================
*/
void InGame_MenuInit( void ) {
	int		y;
	uiClientState_t	cs;
	char	info[MAX_INFO_STRING];
	int		team;

	memset( &s_ingame, 0 ,sizeof(ingamemenu_t) );

	InGame_Cache();

	s_ingame.menu.wrapAround = qtrue;
	s_ingame.menu.fullscreen = qfalse;

	s_ingame.frame.generic.type			= MTYPE_BITMAP;
	s_ingame.frame.generic.flags		= QMF_INACTIVE;
	s_ingame.frame.generic.name			= INGAME_FRAME;
	s_ingame.frame.generic.x			= 320-233;//142;
	s_ingame.frame.generic.y			= 240-166;//118;
	s_ingame.frame.width				= 466;//359;
	s_ingame.frame.height				= 332;//256;

#ifdef TA_MISC // INGAME_SERVER_MENU
	y = 88+INGAME_MENU_VERTICAL_SPACING;
#else
	//y = 96;
	y = 88;
#endif
#ifdef TA_MISC // SMART_JOIN_MENU
	if( (trap_Cvar_VariableValue( "g_gametype" ) >= GT_TEAM) ) {
		s_ingame.team.generic.type			= MTYPE_PTEXT;
		s_ingame.team.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		s_ingame.team.generic.x				= 320;
		s_ingame.team.generic.y				= y;
		s_ingame.team.generic.id			= ID_TEAM;
		s_ingame.team.generic.callback		= InGame_Event;
		s_ingame.team.string				= "CHANGE TEAM";
		s_ingame.team.color					= text_big_color;
		s_ingame.team.style					= UI_CENTER|UI_SMALLFONT;
	}
	else {
		trap_GetClientState( &cs );
		trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
		team = atoi( Info_ValueForKey( info, "t" ) );
		if( team == TEAM_SPECTATOR ) {
			s_ingame.team.generic.type			= MTYPE_PTEXT;
			s_ingame.team.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
			s_ingame.team.generic.x				= 320;
			s_ingame.team.generic.y				= y;
			s_ingame.team.generic.id			= ID_JOINGAME;
			s_ingame.team.generic.callback		= InGame_Event;
			s_ingame.team.string				= "JOIN GAME";
			s_ingame.team.color					= text_big_color;
			s_ingame.team.style					= UI_CENTER|UI_SMALLFONT;
		}
		else
		{
			s_ingame.team.generic.type			= MTYPE_PTEXT;
			s_ingame.team.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
			s_ingame.team.generic.x				= 320;
			s_ingame.team.generic.y				= y;
			s_ingame.team.generic.id			= ID_SPECTATE;
			s_ingame.team.generic.callback		= InGame_Event;
			s_ingame.team.string				= "SPECTATE";
			s_ingame.team.color					= text_big_color;
			s_ingame.team.style					= UI_CENTER|UI_SMALLFONT;
		}
	}
#else
	s_ingame.team.generic.type			= MTYPE_PTEXT;
	s_ingame.team.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.team.generic.x				= 320;
	s_ingame.team.generic.y				= y;
	s_ingame.team.generic.id			= ID_TEAM;
	s_ingame.team.generic.callback		= InGame_Event; 
	s_ingame.team.string				= "START";
	s_ingame.team.color					= text_big_color;
	s_ingame.team.style					= UI_CENTER|UI_SMALLFONT;
#endif

#ifndef TA_MISC // INGAME_SERVER_MENU
	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.addbots.generic.type		= MTYPE_PTEXT;
	s_ingame.addbots.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.addbots.generic.x			= 320;
	s_ingame.addbots.generic.y			= y;
	s_ingame.addbots.generic.id			= ID_ADDBOTS;
	s_ingame.addbots.generic.callback	= InGame_Event; 
	s_ingame.addbots.string				= "ADD BOTS";
	s_ingame.addbots.color				= text_big_color;
	s_ingame.addbots.style				= UI_CENTER|UI_SMALLFONT;
	if( !trap_Cvar_VariableValue( "sv_running" ) || !trap_Cvar_VariableValue( "bot_enable" )
#ifdef TA_SP
	|| (trap_Cvar_VariableValue( "ui_singlePlayerActive" ) == 1)
#else
	|| (trap_Cvar_VariableValue( "g_gametype" ) == GT_SINGLE_PLAYER)
#endif
	) {
		s_ingame.addbots.generic.flags |= QMF_GRAYED;
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.removebots.generic.type		= MTYPE_PTEXT;
	s_ingame.removebots.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.removebots.generic.x			= 320;
	s_ingame.removebots.generic.y			= y;
	s_ingame.removebots.generic.id			= ID_REMOVEBOTS;
	s_ingame.removebots.generic.callback	= InGame_Event; 
	s_ingame.removebots.string				= "REMOVE BOTS";
	s_ingame.removebots.color				= text_big_color;
	s_ingame.removebots.style				= UI_CENTER|UI_SMALLFONT;
	if( !trap_Cvar_VariableValue( "sv_running" ) || !trap_Cvar_VariableValue( "bot_enable" )
#ifdef TA_SP
	|| (trap_Cvar_VariableValue( "ui_singlePlayerActive" ) == 1)
#else
	|| (trap_Cvar_VariableValue( "g_gametype" ) == GT_SINGLE_PLAYER)
#endif
	) {
		s_ingame.removebots.generic.flags |= QMF_GRAYED;
	}
#endif

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.teamorders.generic.type		= MTYPE_PTEXT;
	s_ingame.teamorders.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.teamorders.generic.x			= 320;
	s_ingame.teamorders.generic.y			= y;
	s_ingame.teamorders.generic.id			= ID_TEAMORDERS;
	s_ingame.teamorders.generic.callback	= InGame_Event; 
	s_ingame.teamorders.string				= "TEAM ORDERS";
	s_ingame.teamorders.color				= text_big_color;
	s_ingame.teamorders.style				= UI_CENTER|UI_SMALLFONT;
	if( !(trap_Cvar_VariableValue( "g_gametype" ) >= GT_TEAM) ) {
		s_ingame.teamorders.generic.flags |= QMF_GRAYED;
	}
	else {
		trap_GetClientState( &cs );
		trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
		team = atoi( Info_ValueForKey( info, "t" ) );
		if( team == TEAM_SPECTATOR ) {
			s_ingame.teamorders.generic.flags |= QMF_GRAYED;
		}
	}

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.setup.generic.type			= MTYPE_PTEXT;
	s_ingame.setup.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.setup.generic.x			= 320;
	s_ingame.setup.generic.y			= y;
	s_ingame.setup.generic.id			= ID_SETUP;
	s_ingame.setup.generic.callback		= InGame_Event; 
#ifdef TA_SP // New menus
	s_ingame.setup.string				= "OPTIONS";
#else
	s_ingame.setup.string				= "SETUP";
#endif
	s_ingame.setup.color				= text_big_color;
	s_ingame.setup.style				= UI_CENTER|UI_SMALLFONT;

#ifdef TA_MISC
	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.setupplayer.generic.type		= MTYPE_PTEXT;
	s_ingame.setupplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.setupplayer.generic.x			= 320;
	s_ingame.setupplayer.generic.y			= y;
	s_ingame.setupplayer.generic.id			= ID_CUSTOMIZEPLAYER;
	s_ingame.setupplayer.generic.callback	= InGame_Event; 
	s_ingame.setupplayer.string				= "PLAYER";
	s_ingame.setupplayer.color				= text_big_color;
	s_ingame.setupplayer.style				= UI_CENTER|UI_SMALLFONT;
#endif

#ifdef TA_MISC // INGAME_SERVER_MENU
	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.inserver.generic.type		= MTYPE_PTEXT;
	s_ingame.inserver.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.inserver.generic.x			= 320;
	s_ingame.inserver.generic.y			= y;
	s_ingame.inserver.generic.id		= ID_SERVER;
	s_ingame.inserver.generic.callback	= InGame_Event; 
	s_ingame.inserver.string			= "SERVER";
	s_ingame.inserver.color				= text_big_color;
	s_ingame.inserver.style				= UI_CENTER|UI_SMALLFONT;
	if( !trap_Cvar_VariableValue( "sv_running" ) ) {
		s_ingame.inserver.generic.flags |= QMF_GRAYED;
	}
#endif

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.server.generic.type		= MTYPE_PTEXT;
	s_ingame.server.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.server.generic.x			= 320;
	s_ingame.server.generic.y			= y;
	s_ingame.server.generic.id			= ID_SERVERINFO;
	s_ingame.server.generic.callback	= InGame_Event; 
	s_ingame.server.string				= "SERVER INFO";
	s_ingame.server.color				= text_big_color;
	s_ingame.server.style				= UI_CENTER|UI_SMALLFONT;

#ifndef TA_MISC // INGAME_SERVER_MENU
	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.restart.generic.type		= MTYPE_PTEXT;
	s_ingame.restart.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.restart.generic.x			= 320;
	s_ingame.restart.generic.y			= y;
	s_ingame.restart.generic.id			= ID_RESTART;
	s_ingame.restart.generic.callback	= InGame_Event; 
	s_ingame.restart.string				= "RESTART ARENA";
	s_ingame.restart.color				= text_big_color;
	s_ingame.restart.style				= UI_CENTER|UI_SMALLFONT;
	if( !trap_Cvar_VariableValue( "sv_running" ) ) {
		s_ingame.restart.generic.flags |= QMF_GRAYED;
	}
#endif

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.resume.generic.type			= MTYPE_PTEXT;
	s_ingame.resume.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.resume.generic.x				= 320;
	s_ingame.resume.generic.y				= y;
	s_ingame.resume.generic.id				= ID_RESUME;
	s_ingame.resume.generic.callback		= InGame_Event; 
	s_ingame.resume.string					= "RESUME GAME";
	s_ingame.resume.color					= text_big_color;
	s_ingame.resume.style					= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.leave.generic.type			= MTYPE_PTEXT;
	s_ingame.leave.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.leave.generic.x			= 320;
	s_ingame.leave.generic.y			= y;
	s_ingame.leave.generic.id			= ID_LEAVEARENA;
	s_ingame.leave.generic.callback		= InGame_Event; 
	s_ingame.leave.string				= "LEAVE ARENA";
	s_ingame.leave.color				= text_big_color;
	s_ingame.leave.style				= UI_CENTER|UI_SMALLFONT;

	y += INGAME_MENU_VERTICAL_SPACING;
	s_ingame.quit.generic.type			= MTYPE_PTEXT;
	s_ingame.quit.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.quit.generic.x				= 320;
	s_ingame.quit.generic.y				= y;
	s_ingame.quit.generic.id			= ID_QUIT;
	s_ingame.quit.generic.callback		= InGame_Event; 
	s_ingame.quit.string				= "EXIT GAME";
	s_ingame.quit.color					= text_big_color;
	s_ingame.quit.style					= UI_CENTER|UI_SMALLFONT;

	Menu_AddItem( &s_ingame.menu, &s_ingame.frame );
	Menu_AddItem( &s_ingame.menu, &s_ingame.team );
#ifndef TA_MISC // INGAME_SERVER_MENU
	Menu_AddItem( &s_ingame.menu, &s_ingame.addbots );
	Menu_AddItem( &s_ingame.menu, &s_ingame.removebots );
#endif
	Menu_AddItem( &s_ingame.menu, &s_ingame.teamorders );
	Menu_AddItem( &s_ingame.menu, &s_ingame.setup );
#ifdef TA_MISC
	Menu_AddItem( &s_ingame.menu, &s_ingame.setupplayer );
#endif
#ifdef TA_MISC // INGAME_SERVER_MENU
	Menu_AddItem( &s_ingame.menu, &s_ingame.inserver );
#endif
	Menu_AddItem( &s_ingame.menu, &s_ingame.server );
#ifndef TA_MISC // INGAME_SERVER_MENU
	Menu_AddItem( &s_ingame.menu, &s_ingame.restart );
#endif
	Menu_AddItem( &s_ingame.menu, &s_ingame.resume );
	Menu_AddItem( &s_ingame.menu, &s_ingame.leave );
	Menu_AddItem( &s_ingame.menu, &s_ingame.quit );
}


/*
=================
InGame_Cache
=================
*/
void InGame_Cache( void ) {
	trap_R_RegisterShaderNoMip( INGAME_FRAME );
}


/*
=================
UI_InGameMenu
=================
*/
void UI_InGameMenu( void ) {
	// force as top level menu
	uis.menusp = 0;  

	// set menu cursor to a nice location
	uis.cursorx = 319;
	uis.cursory = 80;

	InGame_MenuInit();
	UI_PushMenu( &s_ingame.menu );
}
