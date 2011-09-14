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
			level = -ARENAS_PER_TIER;
		}
		else if( Q_stricmp( txt, "final" ) == 0 ) {
			level = UI_GetNumSPTiers() * ARENAS_PER_TIER;
		}
	}
	trap_Cvar_SetValue( "ui_spSelection", level );

	map = Info_ValueForKey( arenaInfo, "map" );
#ifdef TA_SP
	trap_Cvar_Set("ui_singlePlayerActive", "1");
#endif
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "spmap %s\n", map ) );
}

#ifdef TA_SP

// Load Game Menu, based on demos menu
//
#define ART_BACK0			"menu/art/back_0"
#define ART_BACK1			"menu/art/back_1"
// ZTM: Demos use play_0/play_1 i use load_0/load_1
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
#ifdef TA_DATA
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
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "loadgame %s\n", // TA_SP
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
===============
LoadGame_MenuInit
===============
*/
static void LoadGame_MenuInit( void ) {
	int		i;
	int		len;
	char	*demoname/*, extension[32]*/;

	memset( &s_savegames, 0 ,sizeof(savegames_t) );

	LoadGame_Cache();

	s_savegames.menu.fullscreen = qtrue;
	s_savegames.menu.wrapAround = qtrue;

	s_savegames.banner.generic.type		= MTYPE_BTEXT;
	s_savegames.banner.generic.x		= 320;
	s_savegames.banner.generic.y		= 16;
	s_savegames.banner.string			= "LOAD SAVE GAME"; // TA_SP
	s_savegames.banner.color			= text_banner_color;
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
		strcpy( s_savegames.names, "No Save Games.sav" ); // TA_SP
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
		if (!Q_stricmp(demoname +  len - 4,".sav")) // TA_SP
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

// Single Player Menu
//

#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"
#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define VERTICAL_SPACING	30

#define ID_BACK				10
#define ID_SP_NEWGAME		11
#define ID_SP_LOADGAME		12
#define ID_SP_ARCADE		13
#define ID_SP_DEMOS			14
#define ID_SP_CINEMATICS	15
#define ID_SP_MODS			16

typedef struct {
	menuframework_s	menu;
	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;
	menutext_s		sp_newgame;
	menutext_s		sp_loadgame;
	menutext_s		sp_arcade;
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
			UI_SPPlayerMenu(UI_GetArenaInfoByNumber(0));
			break;

		case ID_SP_LOADGAME:
			UI_LoadGameMenu();
			break;

		case ID_SP_ARCADE:
			UI_StartServerMenu( qfalse );
			break;

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
	spMenuInfo.menu.wrapAround = qtrue;

	spMenuInfo.banner.generic.type		= MTYPE_BTEXT;
	spMenuInfo.banner.generic.x			= 320;
	spMenuInfo.banner.generic.y			= 16;
	spMenuInfo.banner.string			= "PLAY";
	spMenuInfo.banner.color				= text_banner_color;
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

	y = 150;
	spMenuInfo.sp_newgame.generic.type		= MTYPE_PTEXT;
	spMenuInfo.sp_newgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_newgame.generic.x			= 320;
	spMenuInfo.sp_newgame.generic.y			= y;
	spMenuInfo.sp_newgame.generic.id		= ID_SP_NEWGAME;
	spMenuInfo.sp_newgame.generic.callback	= UI_SPMenu_Event;
	spMenuInfo.sp_newgame.string			= "New Game";
	spMenuInfo.sp_newgame.color				= text_big_color;
	spMenuInfo.sp_newgame.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	spMenuInfo.sp_loadgame.generic.type			= MTYPE_PTEXT;
	spMenuInfo.sp_loadgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_loadgame.generic.x			= 320;
	spMenuInfo.sp_loadgame.generic.y			= y;
	spMenuInfo.sp_loadgame.generic.id			= ID_SP_LOADGAME;
	spMenuInfo.sp_loadgame.generic.callback		= UI_SPMenu_Event;
	spMenuInfo.sp_loadgame.string				= "Load Game";
	spMenuInfo.sp_loadgame.color				= text_big_color;
	spMenuInfo.sp_loadgame.style				= UI_CENTER;

	// Extra space between, "single player" and the others
	y += VERTICAL_SPACING;

	// Moved here from q3 sp arena select
	y += VERTICAL_SPACING;
	spMenuInfo.sp_arcade.generic.type				= MTYPE_PTEXT;
	spMenuInfo.sp_arcade.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_arcade.generic.x					= 320;
	spMenuInfo.sp_arcade.generic.y					= y;
	spMenuInfo.sp_arcade.generic.id					= ID_SP_ARCADE;
	spMenuInfo.sp_arcade.generic.callback			= UI_SPMenu_Event;
	spMenuInfo.sp_arcade.string						= "Arcade";
	spMenuInfo.sp_arcade.color						= text_big_color;
	spMenuInfo.sp_arcade.style						= UI_CENTER;

	// ZTM: TOOD: Add menu for viewing arcade scores

	// Extra space between, arcade and the others
	y += VERTICAL_SPACING;

	// Moved here from q3 main menu.
	y += VERTICAL_SPACING;
	spMenuInfo.sp_demos.generic.type				= MTYPE_PTEXT;
	spMenuInfo.sp_demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_demos.generic.x					= 320;
	spMenuInfo.sp_demos.generic.y					= y;
	spMenuInfo.sp_demos.generic.id					= ID_SP_DEMOS;
	spMenuInfo.sp_demos.generic.callback			= UI_SPMenu_Event;
	spMenuInfo.sp_demos.string						= "Demos";
	spMenuInfo.sp_demos.color						= text_big_color;
	spMenuInfo.sp_demos.style						= UI_CENTER;

	y += VERTICAL_SPACING;
	spMenuInfo.sp_cinematics.generic.type			= MTYPE_PTEXT;
	spMenuInfo.sp_cinematics.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_cinematics.generic.x				= 320;
	spMenuInfo.sp_cinematics.generic.y				= y;
	spMenuInfo.sp_cinematics.generic.id				= ID_SP_CINEMATICS;
	spMenuInfo.sp_cinematics.generic.callback		= UI_SPMenu_Event;
	spMenuInfo.sp_cinematics.string					= "Cinematics";
	spMenuInfo.sp_cinematics.color					= text_big_color;
	spMenuInfo.sp_cinematics.style					= UI_CENTER;

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
		s_main.teamArena.color					= text_big_color;
		s_main.teamArena.style					= UI_CENTER;
	}
*/

	y += VERTICAL_SPACING;
	spMenuInfo.sp_mods.generic.type		= MTYPE_PTEXT;
	spMenuInfo.sp_mods.generic.flags	= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	spMenuInfo.sp_mods.generic.x		= 320;
	spMenuInfo.sp_mods.generic.y		= y;
	spMenuInfo.sp_mods.generic.id		= ID_SP_MODS;
	spMenuInfo.sp_mods.generic.callback	= UI_SPMenu_Event;
	spMenuInfo.sp_mods.string			= "Mods";
	spMenuInfo.sp_mods.color			= text_big_color;
	spMenuInfo.sp_mods.style			= UI_CENTER;

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
	Menu_AddItem( &spMenuInfo.menu, &spMenuInfo.sp_arcade );
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
#endif
