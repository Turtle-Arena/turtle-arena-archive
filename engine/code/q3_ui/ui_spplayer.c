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

#ifdef TA_SP

#define ART_BACK					"menu/art/back_0"
#define ART_BACK_FOCUS				"menu/art/back_1"
#ifdef TA_MISC // NO_MENU_FIGHT
#define ART_PLAY					"menu/art/play_0"
#define ART_PLAY_FOCUS				"menu/art/play_1"
#else
#define ART_PLAY					"menu/art/fight_0"
#define ART_PLAY_FOCUS				"menu/art/fight_1"
#endif
#define ART_LEO						"menu/art/char_leo"
#define ART_DON						"menu/art/char_don"
#define ART_RAPH					"menu/art/char_raph"
#define ART_MIKE					"menu/art/char_mike"
#define ART_FOCUS					"menu/art/char_focus"
#define ART_DEFER					"gfx/2d/defer"

#define ID_BACK						10
#define ID_PLAY						11
#define ID_CLIENT0					12
#define ID_CLIENT0_ENABLED			(ID_CLIENT0+MAX_SPLITVIEW)

const char *spCharacterNames[NUM_SP_CHARACTERS] = { "leo", "don", "raph", "mike" };
char *spCharacterShaders[NUM_SP_CHARACTERS] = { ART_LEO, ART_DON, ART_RAPH, ART_MIKE };

// Player#
char *spPnum[MAX_SPLITVIEW] = { "P1", "P2", "P3", "P4" };

typedef struct {
	menuframework_s	menu;

	menutext_s		art_banner;

	menutext_s			item_p1;
	menuradiobutton_s	clientEnabled[MAX_SPLITVIEW-1];
	menubitmap_s		clientCharacter[MAX_SPLITVIEW];
	menubitmap_s		clientConflict[MAX_SPLITVIEW];

	menubitmap_s	back;
	menubitmap_s	play;

	const char		*arenaInfo;
	sfxHandle_t		silenceSound;
	sfxHandle_t		characterSound[NUM_SP_CHARACTERS];

	int				selectedCharacter[MAX_SPLITVIEW];
	int				characterConflict[MAX_SPLITVIEW];
} playerMenuInfo_t;

static playerMenuInfo_t	playerMenuInfo;

/*
=================
UI_CheckCharacterConflicts

Only one client is allowed to use each character, but we have to allow
selecting the same character so clients can trade characters...
=================
*/
static void UI_CheckCharacterConflicts(int localClientNum, int oldCharacter, int newCharacter) {
	int i;
	int oldConflict;

	oldConflict = playerMenuInfo.characterConflict[localClientNum];
	playerMenuInfo.characterConflict[localClientNum] = 0;

	for (i = 0; i < MAX_SPLITVIEW; ++i) {
		if (i == localClientNum) {
			continue;
		}

		if (playerMenuInfo.clientCharacter[i].generic.flags & QMF_GRAYED) {
			continue;
		}

		if (playerMenuInfo.selectedCharacter[i] == newCharacter) {
			playerMenuInfo.characterConflict[localClientNum]++;
		}

		if (playerMenuInfo.selectedCharacter[i] == oldCharacter) {
			// Check if someone else should move up in the que
			if (playerMenuInfo.characterConflict[i] >= oldConflict) {
				playerMenuInfo.characterConflict[i]--;
				if (!playerMenuInfo.characterConflict[i]) {
					// Remove conflict image
					playerMenuInfo.clientConflict[i].generic.flags |= QMF_HIDDEN;
				}
			}
		}
	}

	// Show conflit image when needed.
	if (playerMenuInfo.characterConflict[localClientNum]) {
		playerMenuInfo.clientConflict[localClientNum].generic.flags &= ~QMF_HIDDEN;
	} else {
		playerMenuInfo.clientConflict[localClientNum].generic.flags |= QMF_HIDDEN;
	}

	// Don't allow the game to be started while there are character conflicts.
	for (i = 0; i < MAX_SPLITVIEW; ++i) {
		if (playerMenuInfo.characterConflict[i]) {
			playerMenuInfo.play.generic.flags |= QMF_GRAYED;
			return;
		}
	}

	// No character conflicts, allow the user to start the game.
	playerMenuInfo.play.generic.flags &= ~QMF_GRAYED;
}

/*
=================
UI_SetSPCharacter
=================
*/
static void UI_SetSPCharacter(int localClientNum, int character) {
	if (playerMenuInfo.selectedCharacter[localClientNum] == character) {
		return;
	}

	playerMenuInfo.selectedCharacter[localClientNum] = character;

	// Change shader and force loading it
	playerMenuInfo.clientCharacter[localClientNum].generic.name = spCharacterShaders[character];
	playerMenuInfo.clientCharacter[localClientNum].shader = 0;
}

/*
=================
Character_Key
=================
*/
static sfxHandle_t Character_Key( menubitmap_s *s, int key )
{
	sfxHandle_t	sound;
	int			localClientNum;
	int			curvalue;
	int			numitems;

	localClientNum = s->generic.id - ID_CLIENT0;
	curvalue = playerMenuInfo.selectedCharacter[localClientNum];
	numitems = NUM_SP_CHARACTERS;

#ifdef TA_MISC // MENU: Right Mouse button = left arrow
	if (key == K_MOUSE2 && !(s->generic.flags & QMF_HASMOUSEFOCUS)) {
		return 0;
	}
#endif

	sound = 0;
	switch (key)
	{
		case K_KP_RIGHTARROW:
#ifdef IOQ3ZTM // CHECK_NUMLOCK
			if (trap_Key_IsDown(K_KP_NUMLOCK)) {
				break;
			}
#endif
		case K_RIGHTARROW:
			curvalue++;
			if (curvalue >= numitems)
				curvalue = 0;
			sound = menu_move_sound;
			break;
		
		case K_KP_LEFTARROW:
#ifdef IOQ3ZTM // CHECK_NUMLOCK
			if (trap_Key_IsDown(K_KP_NUMLOCK)) {
				break;
			}
#endif
		case K_LEFTARROW:
#ifdef TA_MISC // MENU: Right Mouse button = left arrow
		case K_MOUSE2:
#endif
			curvalue--;
			if (curvalue < 0)
				curvalue = numitems-1;
			sound = menu_move_sound;
			break;
	}

	if (sound) {
		UI_CheckCharacterConflicts(localClientNum, playerMenuInfo.selectedCharacter[localClientNum], curvalue);

		UI_SetSPCharacter(localClientNum, curvalue);

		trap_S_StartLocalSound( playerMenuInfo.characterSound[curvalue], CHAN_ANNOUNCER );
	}

	return (sound);
}

/*
=================
UI_SPPlayerMenu_CharacterEvent
=================
*/
static void UI_SPPlayerMenu_CharacterEvent( void *ptr, int notification ) {
	if (notification != QM_ACTIVATED)
		return;

	Character_Key((menubitmap_s*)ptr, K_RIGHTARROW);
}

/*
=================
UI_SPPlayerMenu_PlayerToggleEvent
=================
*/
static void UI_SPPlayerMenu_PlayerToggleEvent( void *ptr, int notification ) {
	int		id;
	int		localClientNum;
	int		character;

	if (notification != QM_ACTIVATED)
		return;

	id = ((menucommon_s*)ptr)->id;
	localClientNum = id - ID_CLIENT0_ENABLED;

	character = playerMenuInfo.selectedCharacter[localClientNum];

	if (((menuradiobutton_s*)ptr)->curvalue) {
		playerMenuInfo.clientCharacter[localClientNum].generic.flags &= ~QMF_GRAYED;

		trap_S_StartLocalSound( playerMenuInfo.characterSound[character], CHAN_ANNOUNCER );
		
		UI_CheckCharacterConflicts(localClientNum, -1, character);
	} else {
		playerMenuInfo.clientCharacter[localClientNum].generic.flags |= QMF_GRAYED;
		
		UI_CheckCharacterConflicts(localClientNum, character, -1);
	}
}

/*
=================
UI_SPPlayerMenu_PlayEvent
=================
*/
static void UI_SPPlayerMenu_PlayEvent( void *ptr, int notification ) {
	int i, localClients;

	if (notification != QM_ACTIVATED)
		return;

	// Set bits for enabled local clients.
	for (i = 1, localClients = 1; i < MAX_SPLITVIEW; ++i) {
		if (playerMenuInfo.clientEnabled[i-1].curvalue) {
			localClients |= (1<<i);
		}
	}

	trap_Cvar_SetValue( "cl_localClients", localClients );

	// Set characters in cvars.
	for (i = 0; i < MAX_SPLITVIEW; ++i) {
		trap_Cvar_Set( Com_LocalClientCvarName(i, "spmodel"), spCharacterNames[playerMenuInfo.selectedCharacter[i]] );
		trap_Cvar_Set( Com_LocalClientCvarName(i, "spheadmodel"), "" );
	}

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
	sfxHandle_t sound;
	void *ptr;
	int id;

	ptr = Menu_ItemAtCursor(&playerMenuInfo.menu);

	if (ptr) {
		id = ((menucommon_s*)ptr)->id;

		if (id >= ID_CLIENT0 && id < ID_CLIENT0+MAX_SPLITVIEW) {
			sound = Character_Key((menubitmap_s*)ptr, key);
	
			if (sound) {
				return sound;
			}
		}
	}

	if(
#ifndef TA_MISC // MENU: Right Mouse button = left arrow
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

	trap_R_RegisterShaderNoMip( ART_FOCUS );
	trap_R_RegisterShaderNoMip( ART_DEFER );
	trap_R_RegisterShaderNoMip( ART_BACK );
	trap_R_RegisterShaderNoMip( ART_BACK_FOCUS );
	trap_R_RegisterShaderNoMip( ART_PLAY );
	trap_R_RegisterShaderNoMip( ART_PLAY_FOCUS );

	for (i = 0; i < NUM_SP_CHARACTERS; i++) {
		trap_R_RegisterShaderNoMip( spCharacterShaders[i] );

		playerMenuInfo.characterSound[i] = trap_S_RegisterSound( va("sound/misc/ui_%s.wav", spCharacterNames[i]), qfalse );
	}

	playerMenuInfo.silenceSound = trap_S_RegisterSound( "sound/misc/silence.wav", qfalse );
}


/*
=================
UI_SPPlayerMenu_Init
=================
*/
static void UI_SPPlayerMenu_Init( int maxClients ) {
	int		i;
	int		spacing;
	int		leftOffset;

	if (maxClients > MAX_SPLITVIEW) {
		maxClients = MAX_SPLITVIEW;
	} else if (maxClients <= 0) {
		maxClients = 1;
	}

	// Setup equal spacing for the character select columns
	spacing = SCREEN_WIDTH / maxClients;
	leftOffset = (spacing - 128) / 2;

	memset( &playerMenuInfo, 0, sizeof(playerMenuInfo) );
	playerMenuInfo.menu.fullscreen = qtrue;
	playerMenuInfo.menu.wrapAround = qtrue;
	playerMenuInfo.menu.key = UI_SPPlayerMenu_Key;

	UI_SPPlayerMenu_Cache();

	playerMenuInfo.art_banner.generic.type		= MTYPE_BTEXT;
	playerMenuInfo.art_banner.generic.flags		= QMF_CENTER_JUSTIFY;
	playerMenuInfo.art_banner.generic.x			= 320;
	playerMenuInfo.art_banner.generic.y			= 16;
	playerMenuInfo.art_banner.string			= "PLAYER SELECT";
	playerMenuInfo.art_banner.color				= text_banner_color;
	playerMenuInfo.art_banner.style				= UI_CENTER;

	for (i = 0; i < MAX_SPLITVIEW; ++i) {
		if (i == 0) {
			// Main player is always enabled.
			playerMenuInfo.item_p1.generic.type		= MTYPE_TEXT;
			playerMenuInfo.item_p1.generic.flags	= QMF_LEFT_JUSTIFY;
			playerMenuInfo.item_p1.generic.x		= leftOffset+56+spacing*i;
			playerMenuInfo.item_p1.generic.y		= 112 - 32;
			playerMenuInfo.item_p1.string			= spPnum[i];
			playerMenuInfo.item_p1.color			= text_big_color;
			playerMenuInfo.item_p1.style			= UI_CENTER;
		} else {
			playerMenuInfo.clientEnabled[i-1].generic.type		= MTYPE_RADIOBUTTON;
			playerMenuInfo.clientEnabled[i-1].generic.name		= spPnum[i];
			playerMenuInfo.clientEnabled[i-1].generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
			playerMenuInfo.clientEnabled[i-1].generic.x			= leftOffset+56+spacing*i;
			playerMenuInfo.clientEnabled[i-1].generic.y			= 112 - 32;
			playerMenuInfo.clientEnabled[i-1].generic.callback	= UI_SPPlayerMenu_PlayerToggleEvent;
			playerMenuInfo.clientEnabled[i-1].generic.id		= ID_CLIENT0_ENABLED+i;
			playerMenuInfo.clientEnabled[i-1].curvalue			= 0;
		}

		playerMenuInfo.clientCharacter[i].generic.type		= MTYPE_BITMAP;
		//playerMenuInfo.clientCharacter[i].generic.name		= spCharacterShaders[i];
		playerMenuInfo.clientCharacter[i].generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
		playerMenuInfo.clientCharacter[i].generic.x			= leftOffset+spacing*i;
		playerMenuInfo.clientCharacter[i].generic.y			= 112;
		playerMenuInfo.clientCharacter[i].generic.callback	= UI_SPPlayerMenu_CharacterEvent;
		playerMenuInfo.clientCharacter[i].generic.id		= ID_CLIENT0+i;
		playerMenuInfo.clientCharacter[i].width				= 128;
		playerMenuInfo.clientCharacter[i].height			= 256;
		playerMenuInfo.clientCharacter[i].focuspic			= ART_FOCUS;

		if (i > 0) {
			// Extra players default to disabled.
			playerMenuInfo.clientCharacter[i].generic.flags |= QMF_GRAYED;
		}

		playerMenuInfo.clientConflict[i].generic.type		= MTYPE_BITMAP;
		playerMenuInfo.clientConflict[i].generic.name		= ART_DEFER;
		playerMenuInfo.clientConflict[i].generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE|QMF_HIDDEN;
		playerMenuInfo.clientConflict[i].generic.x			= leftOffset+spacing*i;
		playerMenuInfo.clientConflict[i].generic.y			= 112 + 64;
		playerMenuInfo.clientConflict[i].generic.id			= ID_CLIENT0+i;
		playerMenuInfo.clientConflict[i].width				= 128;
		playerMenuInfo.clientConflict[i].height				= 128;

		// Set default character
		playerMenuInfo.selectedCharacter[i] = -1;
		UI_SetSPCharacter(i, i % NUM_SP_CHARACTERS);
	}

	playerMenuInfo.back.generic.type		= MTYPE_BITMAP;
	playerMenuInfo.back.generic.name		= ART_BACK;
	playerMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.back.generic.x			= 0;
	playerMenuInfo.back.generic.y			= 480-64;
	playerMenuInfo.back.generic.callback	= UI_SPPlayerMenu_BackEvent;
	playerMenuInfo.back.generic.id			= ID_BACK;
	playerMenuInfo.back.width				= 128;
	playerMenuInfo.back.height				= 64;
	playerMenuInfo.back.focuspic			= ART_BACK_FOCUS;

	playerMenuInfo.play.generic.type		= MTYPE_BITMAP;
	playerMenuInfo.play.generic.name		= ART_PLAY;
	playerMenuInfo.play.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	playerMenuInfo.play.generic.callback	= UI_SPPlayerMenu_PlayEvent;
	playerMenuInfo.play.generic.id			= ID_PLAY;
	playerMenuInfo.play.generic.x			= 640;
	playerMenuInfo.play.generic.y			= 480-64;
	playerMenuInfo.play.width				= 128;
	playerMenuInfo.play.height				= 64;
	playerMenuInfo.play.focuspic			= ART_PLAY_FOCUS;

	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.art_banner );

	for (i = 0; i < maxClients; ++i) {
		if (i == 0) {
			Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.item_p1 );
		} else {
			Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.clientEnabled[i-1] );
		}
		Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.clientCharacter[i] );
		Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.clientConflict[i] );
	}

	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.back );
	Menu_AddItem( &playerMenuInfo.menu, ( void * )&playerMenuInfo.play );
}


void UI_SPPlayerMenu( int maxLocalClients, const char *arenaInfo ) {
	UI_SPPlayerMenu_Init(maxLocalClients);
	playerMenuInfo.arenaInfo = arenaInfo;
	UI_PushMenu( &playerMenuInfo.menu );
	Menu_SetCursorToItem( &playerMenuInfo.menu, &playerMenuInfo.clientCharacter[0] );
}

#endif // TA_SP
