/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2010 by Zack "ZTurtleMan" Middleton

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
=============================================================================

SINGLE PLAYER, PLAYER SELECT MENU

=============================================================================
*/

#include "ui_local.h"


#define ART_BACK					"menu/art/back_0.tga"
#define ART_BACK_FOCUS				"menu/art/back_1.tga"
#ifdef TMNTMISC // NO_MENU_FIGHT
#define ART_FIGHT					"menu/art/play_0"
#define ART_FIGHT_FOCUS				"menu/art/play_1"
#else
#define ART_FIGHT					"menu/art/fight_0"
#define ART_FIGHT_FOCUS				"menu/art/fight_1"
#endif
#define ART_LEO						"menu/art/leo_0"
#define ART_LEO_FOCUS				"menu/art/leo_1"
#define ART_DON						"menu/art/don_0"
#define ART_DON_FOCUS				"menu/art/don_1"
#define ART_RAPH					"menu/art/raph_0"
#define ART_RAPH_FOCUS				"menu/art/raph_1"
#define ART_MIKE					"menu/art/mike_0"
#define ART_MIKE_FOCUS				"menu/art/mike_1"

#define ID_LEO						10
#define ID_DON						11
#define ID_RAPH						12
#define ID_MIKE						13
#define ID_BACK						14
#define ID_FIGHT					15

#define NUM_SPPLAYERS 4

const char *playerNames[NUM_SPPLAYERS] = { "leo", "don", "raph", "mike" };

typedef struct {
	menuframework_s	menu;

	menutext_s		art_banner;

	menubitmap_s	item_players[NUM_SPPLAYERS];

	menubitmap_s	item_back;
	menubitmap_s	item_fight;

	const char		*arenaInfo;
	sfxHandle_t		silenceSound;
	sfxHandle_t		playerSound[NUM_SPPLAYERS];
	
	int				player;
} playerMenuInfo_t;

static playerMenuInfo_t	playerMenuInfo;

/*
=================
UI_SPSkillMenu_SkillEvent
=================
*/
static void UI_SPPlayerMenu_PlayerEvent( void *ptr, int notification ) {
	int		id;
	int		player;

	if (notification != QM_ACTIVATED)
		return;

	id = ((menucommon_s*)ptr)->id;
	player = id - ID_LEO;
	trap_Cvar_Set( "spmodel", playerNames[player] );
	trap_Cvar_Set( "spheadmodel", playerNames[player] );

	trap_S_StartLocalSound( playerMenuInfo.playerSound[player], CHAN_ANNOUNCER );

	playerMenuInfo.item_players[player].generic.flags &= ~QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[player].generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);

	if (playerMenuInfo.player != player)
	{
		playerMenuInfo.item_players[playerMenuInfo.player].generic.flags |= QMF_PULSEIFFOCUS;
		playerMenuInfo.item_players[playerMenuInfo.player].generic.flags &= ~(QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	}

	playerMenuInfo.player = player;
}


/*
=================
UI_SPPlayerMenu_FightEvent
=================
*/
static void UI_SPPlayerMenu_FightEvent( void *ptr, int notification ) {
	if (notification != QM_ACTIVATED)
		return;

	UI_SPArena_Start( playerMenuInfo.arenaInfo );
}


/*
=================
UI_SPPlayerMenu_BackEvent
=================
*/
static void UI_SPPlayerMenu_BackEvent( void* ptr, int notification ) {
	if (notification != QM_ACTIVATED) {
		return;
	}

	trap_S_StartLocalSound( playerMenuInfo.silenceSound, CHAN_ANNOUNCER );
	UI_PopMenu();
}


/*
=================
UI_SPPlayerMenu_Key
=================
*/
static sfxHandle_t UI_SPPlayerMenu_Key( int key ) {
	if(
#ifdef TMNTMISC // MENU: Right Mouse button = left arrow
	key == K_MOUSE2 ||
#endif
	key == K_ESCAPE ) {
		trap_S_StartLocalSound( playerMenuInfo.silenceSound, CHAN_ANNOUNCER );
	}
	return Menu_DefaultKey( &playerMenuInfo.menu, key );
}


/*
=================
UI_SPPlayerMenu_Cache
=================
*/
void UI_SPPlayerMenu_Cache( void ) {
	int i;

	trap_R_RegisterShaderNoMip( ART_BACK );
	trap_R_RegisterShaderNoMip( ART_BACK_FOCUS );
	trap_R_RegisterShaderNoMip( ART_FIGHT );
	trap_R_RegisterShaderNoMip( ART_FIGHT_FOCUS );
	trap_R_RegisterShaderNoMip( ART_LEO );
	trap_R_RegisterShaderNoMip( ART_LEO_FOCUS );
	trap_R_RegisterShaderNoMip( ART_DON );
	trap_R_RegisterShaderNoMip( ART_DON_FOCUS );
	trap_R_RegisterShaderNoMip( ART_RAPH );
	trap_R_RegisterShaderNoMip( ART_RAPH_FOCUS );
	trap_R_RegisterShaderNoMip( ART_MIKE );
	trap_R_RegisterShaderNoMip( ART_MIKE_FOCUS );

	for (i = 0; i < NUM_SPPLAYERS; i++)
	{
		playerMenuInfo.playerSound[i] = trap_S_RegisterSound( va("sound/misc/ui_%s.wav", playerNames[i]), qfalse );
	}

	playerMenuInfo.silenceSound = trap_S_RegisterSound( "sound/misc/silence.wav", qfalse );
}


/*
=================
UI_SPSkillMenu_Init
=================
*/
static void UI_SPPlayerMenu_Init( void ) {
	char	playerModel[MAX_QPATH];
	int		i;

	memset( &playerMenuInfo, 0, sizeof(playerMenuInfo) );
	playerMenuInfo.menu.fullscreen = qtrue;
	playerMenuInfo.menu.key = UI_SPPlayerMenu_Key;

	UI_SPPlayerMenu_Cache();

	playerMenuInfo.art_banner.generic.type		= MTYPE_BTEXT;
	playerMenuInfo.art_banner.generic.flags		= QMF_CENTER_JUSTIFY;
	playerMenuInfo.art_banner.generic.x			= 320;
	playerMenuInfo.art_banner.generic.y			= 16;
	playerMenuInfo.art_banner.string			= "PLAYER SELECT";
	playerMenuInfo.art_banner.color				= color_white;
	playerMenuInfo.art_banner.style				= UI_CENTER;

	i = 0;
	playerMenuInfo.item_players[i].generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_players[i].generic.name		= ART_LEO;
	playerMenuInfo.item_players[i].generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[i].generic.x		= 122;
	playerMenuInfo.item_players[i].generic.y		= 480-194-220;
	playerMenuInfo.item_players[i].generic.callback	= UI_SPPlayerMenu_PlayerEvent;
	playerMenuInfo.item_players[i].generic.id		= ID_LEO;
	playerMenuInfo.item_players[i].width			= 169;
	playerMenuInfo.item_players[i].height			= 220;
	playerMenuInfo.item_players[i].focuspic			= ART_LEO_FOCUS;

	i++;
	playerMenuInfo.item_players[i].generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_players[i].generic.name		= ART_DON;
	playerMenuInfo.item_players[i].generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[i].generic.x		= 298;
	playerMenuInfo.item_players[i].generic.y		= 480-240-169;
	playerMenuInfo.item_players[i].generic.callback	= UI_SPPlayerMenu_PlayerEvent;
	playerMenuInfo.item_players[i].generic.id		= ID_DON;
	playerMenuInfo.item_players[i].width			= 220;
	playerMenuInfo.item_players[i].height			= 169;
	playerMenuInfo.item_players[i].focuspic			= ART_DON_FOCUS;

	i++;
	playerMenuInfo.item_players[i].generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_players[i].generic.name		= ART_RAPH;
	playerMenuInfo.item_players[i].generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[i].generic.x		= 122;
	playerMenuInfo.item_players[i].generic.y		= 480-16-169;
	playerMenuInfo.item_players[i].generic.callback	= UI_SPPlayerMenu_PlayerEvent;
	playerMenuInfo.item_players[i].generic.id		= ID_RAPH;
	playerMenuInfo.item_players[i].width			= 220;
	playerMenuInfo.item_players[i].height			= 169;
	playerMenuInfo.item_players[i].focuspic			= ART_RAPH_FOCUS;

	i++;
	playerMenuInfo.item_players[i].generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_players[i].generic.name		= ART_MIKE;
	playerMenuInfo.item_players[i].generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[i].generic.x		= 350;
	playerMenuInfo.item_players[i].generic.y		= 480-16-220;
	playerMenuInfo.item_players[i].generic.callback	= UI_SPPlayerMenu_PlayerEvent;
	playerMenuInfo.item_players[i].generic.id		= ID_MIKE;
	playerMenuInfo.item_players[i].width			= 169;
	playerMenuInfo.item_players[i].height			= 220;
	playerMenuInfo.item_players[i].focuspic			= ART_MIKE_FOCUS;

	playerMenuInfo.item_back.generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_back.generic.name		= ART_BACK;
	playerMenuInfo.item_back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_back.generic.x			= 0;
	playerMenuInfo.item_back.generic.y			= 480-64;
	playerMenuInfo.item_back.generic.callback	= UI_SPPlayerMenu_BackEvent;
	playerMenuInfo.item_back.generic.id			= ID_BACK;
	playerMenuInfo.item_back.width				= 128;
	playerMenuInfo.item_back.height				= 64;
	playerMenuInfo.item_back.focuspic			= ART_BACK_FOCUS;

	playerMenuInfo.item_fight.generic.type		= MTYPE_BITMAP;
	playerMenuInfo.item_fight.generic.name		= ART_FIGHT;
	playerMenuInfo.item_fight.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.item_fight.generic.callback	= UI_SPPlayerMenu_FightEvent;
	playerMenuInfo.item_fight.generic.id		= ID_FIGHT;
	playerMenuInfo.item_fight.generic.x			= 640;
	playerMenuInfo.item_fight.generic.y			= 480-64;
	playerMenuInfo.item_fight.width				= 128;
	playerMenuInfo.item_fight.height			= 64;
	playerMenuInfo.item_fight.focuspic			= ART_FIGHT_FOCUS;

	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.art_banner );

	for (i = 0; i < NUM_SPPLAYERS; i++)
	{
		Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.item_players[i] );
	}
	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.item_back );
	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.item_fight );

	// Select current player
	trap_Cvar_VariableStringBuffer( "spmodel", playerModel, sizeof(playerModel) );
	for (i = 0; i < NUM_SPPLAYERS; i++)
	{
		if (Q_stricmp(playerNames[i], playerModel) == 0)
		{
			playerMenuInfo.player = i;
			break;
		}
	}

	playerMenuInfo.item_players[playerMenuInfo.player].generic.flags &= ~QMF_PULSEIFFOCUS;
	playerMenuInfo.item_players[playerMenuInfo.player].generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
}


void UI_SPPlayerMenu( const char *arenaInfo ) {
	UI_SPPlayerMenu_Init();
	playerMenuInfo.arenaInfo = arenaInfo;
	UI_PushMenu( &playerMenuInfo.menu );
	Menu_SetCursorToItem( &playerMenuInfo.menu, &playerMenuInfo.item_fight );
}
