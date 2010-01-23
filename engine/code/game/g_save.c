/*
===========================================================================
Copyright (C) 2009 Turtle Man

This file is part of TMNT Arena source code.

TMNT Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

TMNT Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TMNT Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

//#include "../qcommon/q_shared.h"
#include "g_local.h"

#ifdef TMNTSP // Save/load

// NOTE: Make sure BG_SAVE_VERSIONS and BG_SAVE_TYPES stay up to date
//         with current save code.

#define	SAVE_VERSION 3 // current version of save/load routines

#define MAX_SAVE_CLIENTS 8

typedef struct
{
	byte connected;
	char model[MAX_QPATH];
	char headModel[MAX_QPATH];
	byte holdable[MAX_HOLDABLE];
	byte lives;
	byte continues;
} save_client_t;

typedef struct
{
	// server exspects these in this order!
	//
    byte version; // Save file version.
    char mapname[MAX_QPATH];
    byte skill;
    byte maxclients;

	// game only, server doesn't read these.
	//
    save_client_t clients[MAX_SAVE_CLIENTS];

} save_t;

/*
============
G_SaveGame
============
*/
qboolean G_SaveGame(fileHandle_t f)
{
	save_t saveData;
	int client;
	int j;

	if (!g_singlePlayer.integer || !g_gametype.integer == GT_SINGLE_PLAYER) {
		G_Printf("Can't savegame, saving is for single player only!\n");
		return qfalse;
	}

	// Setup header
    memset(&saveData, 0, sizeof (save_t));
	saveData.version = SAVE_VERSION;
	// Turtle Man: FIXME: Use next_map? So we can save before loading map?
	trap_Cvar_VariableStringBuffer( "mapname", saveData.mapname, MAX_QPATH );
	saveData.skill = trap_Cvar_VariableIntegerValue("g_spSkill");
	saveData.maxclients = level.maxclients;
	if (saveData.maxclients > MAX_SAVE_CLIENTS)
		saveData.maxclients = MAX_SAVE_CLIENTS;

	for (client = 0; client < saveData.maxclients; client++)
	{
		if (g_entities[client].r.svFlags & SVF_BOT)
			continue;
		saveData.clients[client].connected = level.clients[client].pers.connected;
		if (level.clients[client].pers.connected != CON_CONNECTED)
			continue;

		// model/skin
		Q_strncpyz(saveData.clients[client].model, level.clients[client].pers.playercfg.model, MAX_QPATH);
		// headmodel/skin
		Q_strncpyz(saveData.clients[client].model, level.clients[client].pers.playercfg.headModel, MAX_QPATH);

		// holdable items
		for (j = 0; j < MAX_HOLDABLE; j++)
		{
			saveData.clients[client].holdable[j] = level.clients[client].ps.holdable[j];
		}
		saveData.clients[client].lives = level.clients[client].ps.persistant[PERS_LIVES];
		saveData.clients[client].continues = level.clients[client].ps.persistant[PERS_CONTINUES];
	}

	// Write saveData
	trap_FS_Write(&saveData, sizeof (save_t), f);

	return qtrue;
}

// Called after level is loaded.
void G_LoadGame(fileHandle_t f)
{
	save_t saveData;
	int j;
	int client;

	// Read saveData
	trap_FS_Read(&saveData, sizeof (save_t), f);

	// The server should check but just in case...
	if (saveData.version != SAVE_VERSION)
	{
	    G_Printf( "Error: Unsupported savegame version, %i\n", saveData.version);
        return;
	}

    // Server sets skill and maxclients before loading the level.

	for (client = 0; client < saveData.maxclients; client++)
	{
		if (saveData.clients[client].connected != CON_CONNECTED)
			continue;
		if (g_entities[client].r.svFlags & SVF_BOT)
			continue;
		//if (level.clients[client].pers.connected != CON_CONNECTED)
			//continue;

		//trap_SendServerCommand( client, va("spmodel %s; spheadmodel %s\n", saveData.clients[client].model, saveData.clients[client].headModel) );
		//trap_SendServerCommand( client, va("model %s; headmodel %s\n", saveData.clients[client].model, saveData.clients[client].headModel) );

		for (j = 0; j < MAX_HOLDABLE; j++)
		{
			level.clients[client].ps.holdable[j] = saveData.clients[client].holdable[j];
		}
		level.clients[client].ps.persistant[PERS_LIVES] = saveData.clients[client].lives;
		level.clients[client].ps.persistant[PERS_CONTINUES] = saveData.clients[client].continues;
	}
}
#endif // TMNTSP
