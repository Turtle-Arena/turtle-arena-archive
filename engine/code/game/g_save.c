/*
===========================================================================
Copyright (C) 2009-2010 Zack "ZTurtleMan" Middleton

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

//#include "../qcommon/q_shared.h"
#include "g_local.h"

#ifdef TA_SP // Save/load

// NOTE: Make sure BG_SAVE_VERSIONS stays up to date
//         with current save code.

#define	SAVE_VERSION 5 // current version of save/load routines

#define MAX_SAVE_CLIENTS 1 // ZTM: NOTE: If splitscreen is added, support more players in single player.

typedef struct
{
	byte connected;
	char model[MAX_QPATH];
	char headModel[MAX_QPATH];
	byte holdable[MAX_HOLDABLE];
	byte holdableSelected;
	int score;
	byte lives;
	byte continues;
} save_client_t;

typedef struct
{
	// server expects these in this order!
	//
    byte version; // Save file version.
    char mapname[MAX_QPATH];
    byte skill;
    byte maxclients;

	// game only, server doesn't read these.
	//
    save_client_t clients[MAX_SAVE_CLIENTS];

} save_t;

save_t loadData;
qboolean clientLoad[MAX_CLIENTS];

/*
============
G_SaveGame
============
*/
qboolean G_SaveGame(fileHandle_t f)
{
	save_t saveData;
	int client;
	int i;
	int j;

	if (!g_singlePlayer.integer || g_gametype.integer != GT_SINGLE_PLAYER) {
		G_Printf("Can't savegame, saving is for single player only!\n");
		return qfalse;
	}

	// Setup header
    memset(&saveData, 0, sizeof (save_t));
	saveData.version = SAVE_VERSION;

	// If savemap is set to the name of the next map we can save before level change
	trap_Cvar_VariableStringBuffer( "g_saveMapname", saveData.mapname, MAX_QPATH );
	trap_Cvar_Set("g_saveMapname", "");

	if (!saveData.mapname[0]) {
		if (!g_cheats.integer) {
			// Most likely a user trying to save, which can be used to cheat.
			G_Printf("The game autosaves, complete the level to save.\n");
			return qfalse;
		}

		trap_Cvar_VariableStringBuffer( "mapname", saveData.mapname, MAX_QPATH );
	}

	saveData.skill = trap_Cvar_VariableIntegerValue("g_spSkill");
	saveData.maxclients = level.maxclients;
	if (saveData.maxclients > MAX_SAVE_CLIENTS)
		saveData.maxclients = MAX_SAVE_CLIENTS;

	client = 0;
	for (i = 0; i < level.maxclients; i++)
	{
		if (g_entities[i].r.svFlags & SVF_BOT) {
			// Don't save bots
			continue;
		}
		if (level.clients[i].pers.connected != CON_CONNECTED)  {
			continue;
		}

		saveData.clients[client].connected = level.clients[i].pers.connected;

		// model/skin
		Q_strncpyz(saveData.clients[client].model, level.clients[i].pers.playercfg.model, MAX_QPATH);
		// headmodel/skin
		Q_strncpyz(saveData.clients[client].headModel, level.clients[i].pers.playercfg.headModel, MAX_QPATH);

		// holdable items
		for (j = 0; j < MAX_HOLDABLE; j++)
		{
			saveData.clients[client].holdable[j] = level.clients[i].ps.holdable[j];
		}
		saveData.clients[client].holdableSelected = level.clients[i].ps.holdableIndex;
		saveData.clients[client].score = level.clients[i].ps.persistant[PERS_SCORE];
		saveData.clients[client].lives = level.clients[i].ps.persistant[PERS_LIVES];
		saveData.clients[client].continues = level.clients[i].ps.persistant[PERS_CONTINUES];
		client++;
		if (client >= saveData.maxclients) {
			break;
		}
	}

	// Write saveData
	trap_FS_Write(&saveData, sizeof (save_t), f);

	return qtrue;
}

// Called after level is loaded.
void G_LoadGame(fileHandle_t f)
{
	// Read saveData
	trap_FS_Read(&loadData, sizeof (save_t), f);

	memset(clientLoad, qfalse, MAX_CLIENTS * sizeof (qboolean));

	// The server should check but just in case...
	if (loadData.version != SAVE_VERSION) {
	    G_Error( "Unsupported savegame version, %i\n", loadData.version);
        return;
	}

    // Server sets skill and maxclients before loading the level.
}

void G_LoadGameClientEx(int gameClient, int saveClient)
{
	int j;
	gclient_t *client;
	save_client_t *saved;

	if (clientLoad[saveClient]) {
		return;
	}
	clientLoad[saveClient] = qtrue;

	if (saveClient >= loadData.maxclients) {
		return;
	}

	saved = &loadData.clients[saveClient];
	if (saved->connected != CON_CONNECTED) {
		return;
	}

	client = &level.clients[gameClient];
	if (client->pers.connected != CON_CONNECTED) {
		return;
	}

	trap_SendServerCommand( gameClient, va("spPlayer %s %s", saved->model, saved->headModel) );
	// ZTM: FIXME: Change model now! (Otherwise can see model change!)

	for (j = 0; j < MAX_HOLDABLE; j++)
	{
		client->ps.holdable[j] = saved->holdable[j];
	}
	client->ps.holdableIndex = saved->holdableSelected;
	client->ps.persistant[PERS_SCORE] = saved->score;
	client->ps.persistant[PERS_LIVES] = saved->lives;
	client->ps.persistant[PERS_CONTINUES] = saved->continues;
}

/*
==========
G_LoadGameClient

Figure out which client data to load from save file for this gameClient
There can be a different number of bots on different maps, the order of the clients changes sometimes,
	so find the human(s) to get correct client.
==========
*/
void G_LoadGameClient(int gameClient)
{
	int firstHumanGameClient;
	int saveClient;
	int i;

	// Check if save game was loaded.
	if (loadData.version != SAVE_VERSION) {
        return;
	}

	// Bots don't save data in save files.
	if (g_entities[gameClient].r.svFlags & SVF_BOT) {
		return;
	}

	firstHumanGameClient = 0;

	for (i = 0; i < level.maxclients; i++)
	{
		if (g_entities[i].r.svFlags & SVF_BOT)
			continue;

		firstHumanGameClient = i;
		break;
	}

	saveClient = gameClient - firstHumanGameClient;

	if (saveClient < 0) {
		G_Error("Invalid saved client, %d\n", saveClient);
		return;
	}

	//G_Printf("DEBUG: Human client (%d) in saveData slot (%d)?...\n", gameClient, saveClient);

	G_LoadGameClientEx(gameClient, saveClient);
}
#endif // TA_SP
