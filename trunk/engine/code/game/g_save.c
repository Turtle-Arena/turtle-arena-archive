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

#define	SAVE_VERSION 2 // current version of save/load routines

typedef enum
{
	// NOTE: In Engine if save type is < 128 doesn't reload the map if it is loaded,
	//          if >= 128 always reloads the map (Even if it is loaded).
	// Turtle Man: Shouldn't the map always be reloaded?
	//               (I don't remember why I made it this way)

    SAVE_MINIMUM = 0,	// Save data so that player can start level later.
    SAVE_FULL = 128,	// Turtle Man: TODO?: Full save of the level, includes all entities.
    SAVE_UNKNOWN

} save_type_e;

#define HEADER_SIZE (1+1+MAX_QPATH+1)
typedef struct
{
	// server exspects these in this order...
	//
    byte version; // Save file version.
    byte save_type; // Type of save file.
    char mapname[MAX_QPATH];

	// game only, server doesn't read these.
	//
    byte skill;

} save_header_t;

/*
// (SAVE_FULL only)
typedef struct
{
    int level_time;

    // Safety checks
    int entity_size[4];
    int client_size[4];

    // The header is followed by entity and client data.
} save_full_data_t;
*/

/*
============
G_SaveGame
============
*/
qboolean G_SaveGame(fileHandle_t f)
{
	save_header_t header;
	int client;
	int j;
	int integer;

	if (!g_singlePlayer.integer/* || !g_gametype.integer == GT_SINGLE_PLAYER*/) {
		G_Printf("Can't savegame, saving is for single player only!\n");
		return qfalse;
	}

	// Setup header
    memset(&header, 0, HEADER_SIZE);
	header.version = SAVE_VERSION;
	header.save_type = SAVE_MINIMUM;
	// Turtle Man: FIXME: If minimum save use next_map? So we can save before loading map?
	trap_Cvar_VariableStringBuffer( "mapname", header.mapname, MAX_QPATH );
	header.skill = trap_Cvar_VariableIntegerValue("g_spSkill");

	// write header.
	trap_FS_Write(&header, HEADER_SIZE, f);

	// Write clients.
	if (header.save_type == SAVE_MINIMUM)
	{
		for (client = 0; client < level.maxclients; client++)
		{
			if (level.clients[client].pers.connected != CON_CONNECTED)
				continue;
			integer = client;
			trap_FS_Write(&integer, sizeof(integer), f);

			// Write clients data.
			// TODO: client name?

			// model/skin
			trap_FS_Write(&level.clients[client].pers.playercfg.model, MAX_QPATH, f);
			// headmodel/skin
			trap_FS_Write(&level.clients[client].pers.playercfg.headModel, MAX_QPATH, f);

			// holdable items
			for (j = 0; j < MAX_HOLDABLE; j++)
			{
				integer = level.clients[client].ps.holdable[j];
				trap_FS_Write(&integer, sizeof(integer), f);
			}
		}
	}
	else if (header.save_type == SAVE_FULL)
	{
		// ...
	}

	// End of file
	integer = -1;
	trap_FS_Write(&integer, sizeof(integer), f);

	return qtrue;
}

// Called after level is loaded.
void G_LoadGame(fileHandle_t f)
{
	save_header_t header;

	// Read header
    memset(&header, 0, HEADER_SIZE);
	trap_FS_Read(&header, HEADER_SIZE, f);

	// The server should check but just in case...
	if (header.version != SAVE_VERSION)
	{
	    G_Printf( "Error: Unsupported savegame version is %i\n", header.version);
        return;
	}

    // set the skill level (after every thing is okay).
	trap_Cvar_Set( "g_spSkill", va("%i",header.skill) );

	if (header.save_type == SAVE_MINIMUM)
	{
		int i, j;
		int client;
		int integer;
		char s[2][MAX_QPATH];

		for (i = 0; i < MAX_CLIENTS+1; i++)
		{
			trap_FS_Read(&client, sizeof(client), f);
			if (client == -1)
			{
				break;
			}

			if (level.clients[client].pers.connected != CON_CONNECTED)
				continue;

			// Read model and headModel
			for (j = 0; j < 2; j++)
			{
				trap_FS_Read(s[j], MAX_QPATH, f);
				if (s[j][0] == '\0') {
					strncpy(s[j], DEFAULT_MODEL, MAX_QPATH);
				}
			}
			trap_SendServerCommand( client, va("spmodel %s; spheadmodel %s\n", s[0], s[1]) );

            for (j = 0; j < MAX_HOLDABLE; j++)
            {
            	trap_FS_Read(&integer, sizeof(integer), f);
            	// Turtle Man: FIXME: Check 'bg_item' if item has multiple uses.
            	if (j >= HI_SHURIKEN && j <= HI_LASERSHURIKEN)
            	{
            		level.clients[client].ps.holdable[j] += integer;
            	}
            	else
            	{
					level.clients[client].ps.holdable[j] = integer;
            	}
            }
		}
	}

	//else if (header.save_type == SAVE_FULL)
	//{
	//}

	//else
	//{
	//    G_Printf( "Error: Unknown savegame type %i\n", header.save_type);
    //    return;
	//}
}

#endif // TMNTSP
