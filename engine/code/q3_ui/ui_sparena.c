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
#include "ui_local.h"

void UI_SPArena_Start( const char *arenaInfo ) {
	char	*map;
	int		level;
	int		n;
	char	*txt;

	n = (int)trap_Cvar_VariableValue( "sv_maxclients" );
	if ( n < 8 ) {
		trap_Cvar_SetValue( "sv_maxclients", 8 );
	}

	level = atoi( Info_ValueForKey( arenaInfo, "num" ) );
	txt = Info_ValueForKey( arenaInfo, "special" );
	if( txt[0] ) {
		if( Q_stricmp( txt, "training" ) == 0 ) {
			level = -4;
		}
		else if( Q_stricmp( txt, "final" ) == 0 ) {
			level = UI_GetNumSPTiers() * ARENAS_PER_TIER;
		}
	}
	trap_Cvar_SetValue( "ui_spSelection", level );

	map = Info_ValueForKey( arenaInfo, "map" );
#ifdef TMNTSP
	trap_Cvar_Set("ui_singlePlayerActive", "1");
#endif
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "spmap %s\n", map ) );
}

#ifdef TMNTSP

// Load Game Menu, based on demos menu
//
#define ART_BACK0			"menu/art/back_0"
#define ART_BACK1			"menu/art/back_1"
// Turtle Man: Demos use play_0/play_1 i use load_0/load_1
#define ART_GO0				"menu/art/load_0"
#define ART_GO1				"menu/art/load_1"
#define ART_FRAMEL			"menu/art/frame2_l"
#define ART_FRAMER			"menu/art/frame1_r"
#define ART_ARROWS			"menu/art/arrows_horz_0"
#define ART_ARROWLEFT		"menu/art/arrows_horz_left"
#define ART_ARROWRIGHT		"menu/art/arrows_horz_right"

#define MAX_DEMOS			128
#define NAMEBUFSIZE			( MAX_DEMOS * 16 )

#define ID_BACK				10
#define ID_GO				11
#define ID_LIST				12
#define ID_RIGHT			13
#define ID_LEFT				14

#define ARROWS_WIDTH		128
#ifdef TMNTDATASYS
#define ARROWS_HEIGHT		64
#else
#define ARROWS_HEIGHT		48
#endif


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;

	menulist_s		list;

	menubitmap_s	arrows;
	menubitmap_s	left;
	menubitmap_s	right;
	menubitmap_s	back;
	menubitmap_s	go;

	int				numDemos;
	char			names[NAMEBUFSIZE];
	char			*demolist[MAX_DEMOS];
} savegames_t;

static savegames_t	s_savegames;


/*
===============
Demos_MenuEvent
===============
*/
static void LoadGame_MenuEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GO:
		UI_ForceMenuOff ();
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "loadgame %s\n", // TMNTSP
								s_savegames.list.itemnames[s_savegames.list.curvalue]) );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_LEFT:
		ScrollList_Key( &s_savegames.list, K_LEFTARROW );
		break;

	case ID_RIGHT:
		ScrollList_Key( &s_savegames.list, K_RIGHTARROW );
		break;
	}
}


/*
=================
UI_DemosMenu_Key
=================
*/
static sfxHandle_t UI_LoadGameMenu_Key( int key ) {
	menucommon_s	*item;

	item = Menu_ItemAtCursor( &s_savegames.menu );

	return Menu_DefaultKey( &s_savegames.menu, key );
}


/*
===============
LoadGame_MenuInit
===============
*/
static void LoadGame_MenuInit( void ) {
	int		i;
	int		len;
	char	*demoname/*, extension[32]*/;

	memset( &s_savegames, 0 ,sizeof(savegames_t) );
	s_savegames.menu.key = UI_LoadGameMenu_Key;

	LoadGame_Cache();

	s_savegames.menu.fullscreen = qtrue;
	s_savegames.menu.wrapAround = qtrue;

	s_savegames.banner.generic.type		= MTYPE_BTEXT;
	s_savegames.banner.generic.x		= 320;
	s_savegames.banner.generic.y		= 16;
	s_savegames.banner.string			= "LOAD SAVE GAME"; // TMNTSP
	s_savegames.banner.color			= color_white;
	s_savegames.banner.style			= UI_CENTER;

	s_savegames.framel.generic.type		= MTYPE_BITMAP;
	s_savegames.framel.generic.name		= ART_FRAMEL;
	s_savegames.framel.generic.flags	= QMF_INACTIVE;
	s_savegames.framel.generic.x		= 0;
	s_savegames.framel.generic.y		= 78;
	s_savegames.framel.width			= 256;
	s_savegames.framel.height			= 329;

	s_savegames.framer.generic.type		= MTYPE_BITMAP;
	s_savegames.framer.generic.name		= ART_FRAMER;
	s_savegames.framer.generic.flags	= QMF_INACTIVE;
	s_savegames.framer.generic.x		= 376;
	s_savegames.framer.generic.y		= 76;
	s_savegames.framer.width			= 256;
	s_savegames.framer.height			= 334;

	s_savegames.arrows.generic.type		= MTYPE_BITMAP;
	s_savegames.arrows.generic.name		= ART_ARROWS;
	s_savegames.arrows.generic.flags	= QMF_INACTIVE;
	s_savegames.arrows.generic.x		= 320-ARROWS_WIDTH/2;
	s_savegames.arrows.generic.y		= 400;
	s_savegames.arrows.width			= ARROWS_WIDTH;
	s_savegames.arrows.height			= ARROWS_HEIGHT;

	s_savegames.left.generic.type		= MTYPE_BITMAP;
	s_savegames.left.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_savegames.left.generic.x			= 320-ARROWS_WIDTH/2;
	s_savegames.left.generic.y			= 400;
	s_savegames.left.generic.id			= ID_LEFT;
	s_savegames.left.generic.callback	= LoadGame_MenuEvent;
	s_savegames.left.width				= ARROWS_WIDTH/2;
	s_savegames.left.height				= ARROWS_HEIGHT;
	s_savegames.left.focuspic			= ART_ARROWLEFT;

	s_savegames.right.generic.type		= MTYPE_BITMAP;
	s_savegames.right.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_savegames.right.generic.x			= 320;
	s_savegames.right.generic.y			= 400;
	s_savegames.right.generic.id		= ID_RIGHT;
	s_savegames.right.generic.callback	= LoadGame_MenuEvent;
	s_savegames.right.width				= ARROWS_WIDTH/2;
	s_savegames.right.height			= ARROWS_HEIGHT;
	s_savegames.right.focuspic			= ART_ARROWRIGHT;

	s_savegames.back.generic.type		= MTYPE_BITMAP;
	s_savegames.back.generic.name		= ART_BACK0;
	s_savegames.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_savegames.back.generic.id			= ID_BACK;
	s_savegames.back.generic.callback	= LoadGame_MenuEvent;
	s_savegames.back.generic.x			= 0;
	s_savegames.back.generic.y			= 480-64;
	s_savegames.back.width				= 128;
	s_savegames.back.height				= 64;
	s_savegames.back.focuspic			= ART_BACK1;

	s_savegames.go.generic.type			= MTYPE_BITMAP;
	s_savegames.go.generic.name			= ART_GO0;
	s_savegames.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_savegames.go.generic.id			= ID_GO;
	s_savegames.go.generic.callback		= LoadGame_MenuEvent;
	s_savegames.go.generic.x			= 640;
	s_savegames.go.generic.y			= 480-64;
	s_savegames.go.width				= 128;
	s_savegames.go.height				= 64;
	s_savegames.go.focuspic				= ART_GO1;

	s_savegames.list.generic.type		= MTYPE_SCROLLLIST;
	s_savegames.list.generic.flags		= QMF_PULSEIFFOCUS;
	s_savegames.list.generic.callback	= LoadGame_MenuEvent;
	s_savegames.list.generic.id			= ID_LIST;
	s_savegames.list.generic.x			= 118;
	s_savegames.list.generic.y			= 130;
	s_savegames.list.width				= 16;
	s_savegames.list.height				= 14;
	//Com_sprintf(extension, sizeof(extension), "dm_%d", (int)trap_Cvar_VariableValue( "protocol" ) );
	s_savegames.list.numitems			= trap_FS_GetFileList( SAVEGAMEDIR, ".sav"/*extension*/, s_savegames.names, NAMEBUFSIZE );
	s_savegames.list.itemnames			= (const char **)s_savegames.demolist;
	s_savegames.list.columns			= 3;

	if (!s_savegames.list.numitems) {
		strcpy( s_savegames.names, "No Save Games.sav" ); // TMNTSP
		s_savegames.list.numitems = 1;

		//degenerate case, not selectable
		s_savegames.go.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
	}
	else if (s_savegames.list.numitems > MAX_DEMOS)
		s_savegames.list.numitems = MAX_DEMOS;

	demoname = s_savegames.names;
	for ( i = 0; i < s_savegames.list.numitems; i++ ) {
		s_savegames.list.itemnames[i] = demoname;

		// strip extension
		len = strlen( demoname );
		if (!Q_stricmp(demoname +  len - 4,".sav")) // TMNTSP
			demoname[len-4] = '\0';

//		Q_strupr(demoname);

		demoname += len + 1;
	}

	Menu_AddItem( &s_savegames.menu, &s_savegames.banner );
	Menu_AddItem( &s_savegames.menu, &s_savegames.framel );
	Menu_AddItem( &s_savegames.menu, &s_savegames.framer );
	Menu_AddItem( &s_savegames.menu, &s_savegames.list );
	Menu_AddItem( &s_savegames.menu, &s_savegames.arrows );
	Menu_AddItem( &s_savegames.menu, &s_savegames.left );
	Menu_AddItem( &s_savegames.menu, &s_savegames.right );
	Menu_AddItem( &s_savegames.menu, &s_savegames.back );
	Menu_AddItem( &s_savegames.menu, &s_savegames.go );
}

/*
=================
Demos_Cache
=================
*/
void LoadGame_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_GO0 );
	trap_R_RegisterShaderNoMip( ART_GO1 );
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
	trap_R_RegisterShaderNoMip( ART_ARROWS );
	trap_R_RegisterShaderNoMip( ART_ARROWLEFT );
	trap_R_RegisterShaderNoMip( ART_ARROWRIGHT );
}

/*
===============
UI_DemosMenu
===============
*/
void UI_LoadGameMenu( void ) {
	LoadGame_MenuInit();
	UI_PushMenu( &s_savegames.menu );
}

// Save Game Menu, UI_SpecifyServerMenu
// Turtle Man: TODO: Also browse for save game to over write, using load game menu?
//
/*********************************************************************************
	SPECIFY SAVE
*********************************************************************************/

#define SPECIFYSERVER_FRAMEL	"menu/art/frame2_l"
#define SPECIFYSERVER_FRAMER	"menu/art/frame1_r"
#define SPECIFYSERVER_BACK0		"menu/art/back_0"
#define SPECIFYSERVER_BACK1		"menu/art/back_1"
// Turtle Man: SPECIFY SEVER uses flight_0/1 i use save_0/1
#define SPECIFYSERVER_FIGHT0	"menu/art/save_0"
#define SPECIFYSERVER_FIGHT1	"menu/art/save_1"

#define ID_SPECIFYSERVERBACK	102
#define ID_SPECIFYSERVERGO		103

static char* specifysave_artlist[] =
{
	SPECIFYSERVER_FRAMEL,
	SPECIFYSERVER_FRAMER,
	SPECIFYSERVER_BACK0,
	SPECIFYSERVER_BACK1,
	SPECIFYSERVER_FIGHT0,
	SPECIFYSERVER_FIGHT1,
	NULL
};

typedef struct
{
	menuframework_s	menu;
	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;
	menufield_s		domain;
	//menufield_s		port;
	menubitmap_s	go;
	menubitmap_s	back;
} specifysave_t;

static specifysave_t	s_specifysave;

/*
=================
SpecifySave_Event
=================
*/
static void SpecifySave_Event( void* ptr, int event )
{
	char	buff[256];

	switch (((menucommon_s*)ptr)->id)
	{
		case ID_SPECIFYSERVERGO:
			if (event != QM_ACTIVATED)
				break;

			if (s_specifysave.domain.field.buffer[0])
			{
				strcpy(buff,s_specifysave.domain.field.buffer);
				//if (s_specifysave.port.field.buffer[0])
					//Com_sprintf( buff+strlen(buff), 128, ":%s", s_specifysave.port.field.buffer );

				// Turtle Man: TODO: If file "SAVEGAMEDIR/buff" exist ask for over write.

				trap_Cmd_ExecuteText( EXEC_APPEND, va( "savegame %s\n", buff ) );
			}
			break;

		case ID_SPECIFYSERVERBACK:
			if (event != QM_ACTIVATED)
				break;

			UI_PopMenu();
			break;
	}
}

/*
=================
SpecifySave_MenuInit
=================
*/
void SpecifySave_MenuInit( void )
{
	// zero set all our globals
	memset( &s_specifysave, 0 ,sizeof(specifysave_t) );

	SpecifySave_Cache();

	s_specifysave.menu.wrapAround = qtrue;
	s_specifysave.menu.fullscreen = qtrue;

	s_specifysave.banner.generic.type	 = MTYPE_BTEXT;
	s_specifysave.banner.generic.x     = 320;
	s_specifysave.banner.generic.y     = 16;
	s_specifysave.banner.string		 = "SPECIFY SAVE";
	s_specifysave.banner.color  		 = color_white;
	s_specifysave.banner.style  		 = UI_CENTER;

	s_specifysave.framel.generic.type  = MTYPE_BITMAP;
	s_specifysave.framel.generic.name  = SPECIFYSERVER_FRAMEL;
	s_specifysave.framel.generic.flags = QMF_INACTIVE;
	s_specifysave.framel.generic.x	 = 0;
	s_specifysave.framel.generic.y	 = 78;
	s_specifysave.framel.width  	     = 256;
	s_specifysave.framel.height  	     = 329;

	s_specifysave.framer.generic.type  = MTYPE_BITMAP;
	s_specifysave.framer.generic.name  = SPECIFYSERVER_FRAMER;
	s_specifysave.framer.generic.flags = QMF_INACTIVE;
	s_specifysave.framer.generic.x	 = 376;
	s_specifysave.framer.generic.y	 = 76;
	s_specifysave.framer.width  	     = 256;
	s_specifysave.framer.height  	     = 334;

	s_specifysave.domain.generic.type       = MTYPE_FIELD;
	s_specifysave.domain.generic.name       = "Save Name:";
	s_specifysave.domain.generic.flags      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_specifysave.domain.generic.x	      = 206;
	s_specifysave.domain.generic.y	      = 220;
	s_specifysave.domain.field.widthInChars = 38;
	s_specifysave.domain.field.maxchars     = 80;

/*
	s_specifysave.port.generic.type       = MTYPE_FIELD;
	s_specifysave.port.generic.name	    = "Port:";
	s_specifysave.port.generic.flags	    = QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NUMBERSONLY;
	s_specifysave.port.generic.x	        = 206;
	s_specifysave.port.generic.y	        = 250;
	s_specifysave.port.field.widthInChars = 6;
	s_specifysave.port.field.maxchars     = 5;
*/

	s_specifysave.go.generic.type	    = MTYPE_BITMAP;
	s_specifysave.go.generic.name     = SPECIFYSERVER_FIGHT0;
	s_specifysave.go.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_specifysave.go.generic.callback = SpecifySave_Event;
	s_specifysave.go.generic.id	    = ID_SPECIFYSERVERGO;
	s_specifysave.go.generic.x		= 640;
	s_specifysave.go.generic.y		= 480-64;
	s_specifysave.go.width  		    = 128;
	s_specifysave.go.height  		    = 64;
	s_specifysave.go.focuspic         = SPECIFYSERVER_FIGHT1;

	s_specifysave.back.generic.type	  = MTYPE_BITMAP;
	s_specifysave.back.generic.name     = SPECIFYSERVER_BACK0;
	s_specifysave.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_specifysave.back.generic.callback = SpecifySave_Event;
	s_specifysave.back.generic.id	      = ID_SPECIFYSERVERBACK;
	s_specifysave.back.generic.x		  = 0;
	s_specifysave.back.generic.y		  = 480-64;
	s_specifysave.back.width  		  = 128;
	s_specifysave.back.height  		  = 64;
	s_specifysave.back.focuspic         = SPECIFYSERVER_BACK1;

	Menu_AddItem( &s_specifysave.menu, &s_specifysave.banner );
	Menu_AddItem( &s_specifysave.menu, &s_specifysave.framel );
	Menu_AddItem( &s_specifysave.menu, &s_specifysave.framer );
	Menu_AddItem( &s_specifysave.menu, &s_specifysave.domain );
	//Menu_AddItem( &s_specifysave.menu, &s_specifysave.port );
	Menu_AddItem( &s_specifysave.menu, &s_specifysave.go );
	Menu_AddItem( &s_specifysave.menu, &s_specifysave.back );

	//Com_sprintf( s_specifysave.port.field.buffer, 6, "%i", 27960 );
}

/*
=================
SpecifySave_Cache
=================
*/
void SpecifySave_Cache( void )
{
	int	i;

	// touch all our pics
	for (i=0; ;i++)
	{
		if (!specifysave_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(specifysave_artlist[i]);
	}
}

/*
=================
UI_SpecifySaveMenu
=================
*/
void UI_SpecifySaveMenu( void )
{
	SpecifySave_MenuInit();
	UI_PushMenu( &s_specifysave.menu );
}

// Single Player Menu
//

#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"
#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define VERTICAL_SPACING	30

#define ID_BACK			10
#define ID_SP_NEWGAME	11
#define ID_SP_LOADGAME	12
//#define ID_SP_SAVEGAME	13
#define ID_SP_DEMOS		14
#define ID_SP_CINEMATICS	15
#define ID_SP_MODS		16

typedef struct {
	menuframework_s	menu;
	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;
	menutext_s		sp_newgame;
	menutext_s		sp_loadgame;
	//menutext_s		sp_savegame;
	menutext_s		sp_demos;
	menutext_s		sp_cinematics;
	menutext_s		sp_mods;
	menubitmap_s	back;
} spMenuInfo_t;

static spMenuInfo_t spMenuInfo;

/*
===============
UI_SPMenu_BackEvent
===============
*/
static void UI_SPMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
===============
UI_SPMenu_Event
===============
*/
static void UI_SPMenu_Event( void *ptr, int event ) {
	if (event != QM_ACTIVATED)
		return;

	switch (((menucommon_s*)ptr)->id)
	{
		default:
		case ID_SP_NEWGAME:
			// If in a game, ask user to confirm.
			//if ( !trap_Cvar_VariableValue( "sv_running" )
				//|| AskExitServer() == qtrue )
			UI_StageMenu();
			break;

		case ID_SP_LOADGAME:
			// If in a game, ask user to confirm.
			//if ( !trap_Cvar_VariableValue( "sv_running" )
				//|| AskExitServer() == qtrue )
			UI_LoadGameMenu();
			break;

		//case ID_SP_SAVEGAME:
			//UI_SpecifySaveMenu();
			//break;

		case ID_SP_DEMOS:
			UI_DemosMenu();
			break;

		case ID_SP_CINEMATICS:
			UI_CinematicsMenu();
			break;

		case ID_SP_MODS:
			UI_ModsMenu();
			break;
	}
}


/*
===============
UI_SPMenu_Init
===============
*/
static void UI_SPMenu_Init( void ) {
	int		y;

	UI_SPMenu_Cache();

	memset( &spMenuInfo, 0, sizeof(spMenuInfo) );
	spMenuInfo.menu.fullscreen = qtrue;

	spMenuInfo.banner.generic.type		= MTYPE_BTEXT;
	spMenuInfo.banner.generic.x			= 320;
	spMenuInfo.banner.generic.y			= 16;
	spMenuInfo.banner.string			= "PLAY";
	spMenuInfo.banner.color				= color_white;
	spMenuInfo.banner.style				= UI_CENTER;

	spMenuInfo.framel.generic.type		= MTYPE_BITMAP;
	spMenuInfo.framel.generic.name		= ART_FRAMEL;
	spMenuInfo.framel.generic.flags		= QMF_INACTIVE;
	spMenuInfo.framel.generic.x			= 0;
	spMenuInfo.framel.generic.y			= 78;
	spMenuInfo.framel.width  			= 256;
	spMenuInfo.framel.height  			= 329;

	spMenuInfo.framer.generic.type		= MTYPE_BITMAP;
	spMenuInfo.framer.generic.name		= ART_FRAMER;
	spMenuInfo.framer.generic.flags		= QMF_INACTIVE;
	spMenuInfo.framer.generic.x			= 376;
	spMenuInfo.framer.generic.y			= 76;
	spMenuInfo.framer.width  			= 256;
	spMenuInfo.framer.height  			= 334;

	y = 200;
	spMenuInfo.sp_newgame.generic.type		= MTYPE_PTEXT;
	spMenuInfo.sp_newgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_newgame.generic.x			= 320;
	spMenuInfo.sp_newgame.generic.y			= y;
	spMenuInfo.sp_newgame.generic.id		= ID_SP_NEWGAME;
	spMenuInfo.sp_newgame.generic.callback	= UI_SPMenu_Event;
	spMenuInfo.sp_newgame.string			= "New Game";
	spMenuInfo.sp_newgame.color				= color_red;
	spMenuInfo.sp_newgame.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	spMenuInfo.sp_loadgame.generic.type		= MTYPE_PTEXT;
	spMenuInfo.sp_loadgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_loadgame.generic.x			= 320;
	spMenuInfo.sp_loadgame.generic.y			= y;
	spMenuInfo.sp_loadgame.generic.id			= ID_SP_LOADGAME;
	spMenuInfo.sp_loadgame.generic.callback	= UI_SPMenu_Event;
	spMenuInfo.sp_loadgame.string				= "Load Game";
	spMenuInfo.sp_loadgame.color				= color_red;
	spMenuInfo.sp_loadgame.style				= UI_CENTER;

	y += VERTICAL_SPACING;
#if 0
	spMenuInfo.sp_savegame.generic.type		= MTYPE_PTEXT;
	spMenuInfo.sp_savegame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_savegame.generic.x			= 320;
	spMenuInfo.sp_savegame.generic.y			= y;
	spMenuInfo.sp_savegame.generic.id			= ID_SP_SAVEGAME;
	spMenuInfo.sp_savegame.generic.callback	= UI_SPMenu_Event;
	spMenuInfo.sp_savegame.string				= "Save Game";
	spMenuInfo.sp_savegame.color				= color_red;
	spMenuInfo.sp_savegame.style				= UI_CENTER;

	// If not in a single player game, set grayed.
	// NOTE: This is ALWAYS grayed out. because its in the main menu, which can only be gotten to when not in a game.
	if( !trap_Cvar_VariableValue( "sv_running" ) || !trap_Cvar_VariableValue( "ui_singlePlayerActive" ) ) {
		spMenuInfo.sp_savegame.generic.flags |= QMF_GRAYED;
	}
#endif

	// Moved here from q3 main menu.
	y += VERTICAL_SPACING;
	spMenuInfo.sp_demos.generic.type				= MTYPE_PTEXT;
	spMenuInfo.sp_demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_demos.generic.x					= 320;
	spMenuInfo.sp_demos.generic.y					= y;
	spMenuInfo.sp_demos.generic.id					= ID_SP_DEMOS;
	spMenuInfo.sp_demos.generic.callback			= UI_SPMenu_Event;
	spMenuInfo.sp_demos.string						= "Demos";
	spMenuInfo.sp_demos.color						= color_red;
	spMenuInfo.sp_demos.style						= UI_CENTER;

	y += VERTICAL_SPACING;
	spMenuInfo.sp_cinematics.generic.type			= MTYPE_PTEXT;
	spMenuInfo.sp_cinematics.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_cinematics.generic.x				= 320;
	spMenuInfo.sp_cinematics.generic.y				= y;
	spMenuInfo.sp_cinematics.generic.id				= ID_SP_CINEMATICS;
	spMenuInfo.sp_cinematics.generic.callback		= UI_SPMenu_Event;
	spMenuInfo.sp_cinematics.string					= "Cinematics";
	spMenuInfo.sp_cinematics.color					= color_red;
	spMenuInfo.sp_cinematics.style					= UI_CENTER;
	// Disabled for now.
	spMenuInfo.sp_cinematics.generic.flags |= QMF_GRAYED;

/*
	if (UI_TeamArenaExists()) {
		//teamArena = qtrue;
		y += VERTICAL_SPACING;
		s_main.teamArena.generic.type			= MTYPE_PTEXT;
		s_main.teamArena.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		s_main.teamArena.generic.x				= 320;
		s_main.teamArena.generic.y				= y;
		s_main.teamArena.generic.id				= ID_TEAMARENA;
		s_main.teamArena.generic.callback		= UI_SPMenu_Event;
		s_main.teamArena.string					= "TEAM ARENA";
		s_main.teamArena.color					= color_red;
		s_main.teamArena.style					= UI_CENTER;
	}
*/

	y += VERTICAL_SPACING;
	spMenuInfo.sp_mods.generic.type			= MTYPE_PTEXT;
	spMenuInfo.sp_mods.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_mods.generic.x				= 320;
	spMenuInfo.sp_mods.generic.y				= y;
	spMenuInfo.sp_mods.generic.id				= ID_SP_MODS;
	spMenuInfo.sp_mods.generic.callback		= UI_SPMenu_Event;
	spMenuInfo.sp_mods.string					= "Mods";
	spMenuInfo.sp_mods.color					= color_red;
	spMenuInfo.sp_mods.style					= UI_CENTER;

	spMenuInfo.back.generic.type		= MTYPE_BITMAP;
	spMenuInfo.back.generic.name		= ART_BACK0;
	spMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.back.generic.id			= ID_BACK;
	spMenuInfo.back.generic.callback	= UI_SPMenu_BackEvent;
	spMenuInfo.back.generic.x			= 0;
	spMenuInfo.back.generic.y			= 480-64;
	spMenuInfo.back.width				= 128;
	spMenuInfo.back.height				= 64;
	spMenuInfo.back.focuspic			= ART_BACK1;

	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.banner );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.framel );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.framer );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_newgame );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_loadgame );
	//Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_savegame );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_demos );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_cinematics );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_mods );
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.back );
}


/*
=================
UI_SPMenu_Cache
=================
*/
void UI_SPMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
}


/*
===============
UI_SPMenu
===============
*/
void UI_SPMenu( void ) {
	UI_SPMenu_Init();
	UI_PushMenu( &spMenuInfo.menu );
}


/*
===============
UI_SPMenu_f
===============
*/
void UI_SPMenu_f( void ) {
	int		n;

	n = atoi( UI_Argv( 1 ) );
	uis.menusp = 0;
	UI_SPMenu();
	Menu_SetCursorToItem( &spMenuInfo.menu, spMenuInfo.menu.items[n + 3] );
}


// Stage Select Menu
//

#if 0 // Reuse defines from above menu.
#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"
#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define VERTICAL_SPACING	30

#define ID_BACK			10
#endif
#define ID_STAGE1	11
#define ID_STAGE2	12
#define ID_STAGE3	13
#define ID_STAGE4	13
#define ID_STAGE5	15
#define ID_STAGE6	16
#define ID_STAGE7	17
#define ID_STAGE8	18
#define ID_STAGE9	19
#define ID_STAGE10	20
#define ID_STAGE11	21
#define ID_STAGE12	22
#define ID_STAGE13	23

//#define NUM_SP_STAGES 13 // Turtle Man: Am I thinking to big?
#define NUM_SP_STAGES 2

typedef struct {
	menuframework_s	menu;
	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;
	menutext_s		sp_stages[NUM_SP_STAGES];
	menubitmap_s	back;
} stageMenuInfo_t;

static stageMenuInfo_t stageMenuInfo;

static char *stagename[/*NUM_SP_STAGES*/] =
{
	"Stage 1",
	"Stage 2",
	"Stage 3",
	"Stage 4",
	"Stage 5",
	"Stage 6",
	"Stage 7",
	"Stage 8",
	"Stage 9",
	"Stage 10",
	"Stage 11",
	"Stage 12",
	"Stage 13"
};

/*
===============
UI_StageMenu_BackEvent
===============
*/
static void UI_StageMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
===============
UI_StageMenu_Event
===============
*/
static void UI_StageMenu_Event( void *ptr, int event ) {
	int stageNum;
	if (event != QM_ACTIVATED)
		return;

	stageNum = ((menucommon_s*)ptr)->id - ID_STAGE1;

	if (stageNum >= 0 && stageNum < NUM_SP_STAGES)
	{
		trap_Cvar_SetValue( "ui_spStage", stageNum );
		trap_Cvar_SetValue( "ui_spSelection", stageNum * ARENAS_PER_TIER );
		UI_SPLevelMenu();
	}
}


/*
===============
UI_StageMenu_Init
===============
*/
static void UI_StageMenu_Init( void ) {
	int		y;
	int		x;
	int		i;

	UI_StageMenu_Cache();

	memset( &stageMenuInfo, 0, sizeof(stageMenuInfo) );
	stageMenuInfo.menu.fullscreen = qtrue;

	stageMenuInfo.banner.generic.type		= MTYPE_BTEXT;
	stageMenuInfo.banner.generic.x			= 320;
	stageMenuInfo.banner.generic.y			= 16;
	stageMenuInfo.banner.string				= "STAGE SELECT";
	stageMenuInfo.banner.color				= color_white;
	stageMenuInfo.banner.style				= UI_CENTER;

	stageMenuInfo.framel.generic.type		= MTYPE_BITMAP;
	stageMenuInfo.framel.generic.name		= ART_FRAMEL;
	stageMenuInfo.framel.generic.flags		= QMF_INACTIVE;
	stageMenuInfo.framel.generic.x			= 0;
	stageMenuInfo.framel.generic.y			= 78;
	stageMenuInfo.framel.width  			= 256;
	stageMenuInfo.framel.height  			= 329;

	stageMenuInfo.framer.generic.type		= MTYPE_BITMAP;
	stageMenuInfo.framer.generic.name		= ART_FRAMER;
	stageMenuInfo.framer.generic.flags		= QMF_INACTIVE;
	stageMenuInfo.framer.generic.x			= 376;
	stageMenuInfo.framer.generic.y			= 76;
	stageMenuInfo.framer.width  			= 256;
	stageMenuInfo.framer.height  			= 334;

	y = 135;
	x = 160+80;

	for (i = 0; i < NUM_SP_STAGES; i++)
	{
		// Move to next collum.
		if (i == 7)
		{
			y = 135+15;
			x = 320+80;
		}
		stageMenuInfo.sp_stages[i].generic.type			= MTYPE_PTEXT;
		stageMenuInfo.sp_stages[i].generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		stageMenuInfo.sp_stages[i].generic.x			= x;
		stageMenuInfo.sp_stages[i].generic.y			= y;
		stageMenuInfo.sp_stages[i].generic.id			= ID_STAGE1 + i;
		stageMenuInfo.sp_stages[i].generic.callback		= UI_StageMenu_Event;
		stageMenuInfo.sp_stages[i].string				= (char*)stagename[i];
		stageMenuInfo.sp_stages[i].color				= color_red;
		stageMenuInfo.sp_stages[i].style				= UI_CENTER;

		//if (qtrue) {
		//	stageMenuInfo.sp_stages[i].generic.flags |= QMF_GRAYED;
		//}

		y += VERTICAL_SPACING;
	}

	stageMenuInfo.back.generic.type			= MTYPE_BITMAP;
	stageMenuInfo.back.generic.name			= ART_BACK0;
	stageMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	stageMenuInfo.back.generic.id			= ID_BACK;
	stageMenuInfo.back.generic.callback		= UI_StageMenu_BackEvent;
	stageMenuInfo.back.generic.x			= 0;
	stageMenuInfo.back.generic.y			= 480-64;
	stageMenuInfo.back.width				= 128;
	stageMenuInfo.back.height				= 64;
	stageMenuInfo.back.focuspic				= ART_BACK1;

	Menu_AddItem( &stageMenuInfo.menu, &stageMenuInfo.banner );
	Menu_AddItem( &stageMenuInfo.menu, &stageMenuInfo.framel );
	Menu_AddItem( &stageMenuInfo.menu, &stageMenuInfo.framer );
	for (i = 0; i < NUM_SP_STAGES; i++)
	{
		Menu_AddItem( &stageMenuInfo.menu, &stageMenuInfo.sp_stages[i] );
	}
	Menu_AddItem( &stageMenuInfo.menu, &stageMenuInfo.back );
}


/*
=================
UI_StageMenu_Cache
=================
*/
void UI_StageMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
}


/*
===============
UI_StageMenu
===============
*/
void UI_StageMenu( void ) {
	UI_StageMenu_Init();
	UI_PushMenu( &stageMenuInfo.menu );
}


/*
===============
UI_StageMenu_f
===============
*/
void UI_StageMenu_f( void ) {
	int		n;

	n = atoi( UI_Argv( 1 ) );
	uis.menusp = 0;
	UI_StageMenu();
	Menu_SetCursorToItem( &stageMenuInfo.menu, stageMenuInfo.menu.items[n + 3] );
}
#endif
