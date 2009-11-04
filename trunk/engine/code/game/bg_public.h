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
// bg_public.h -- Included by server, client, and botlib (not by VMs)

// Turtle Man: The server/client/botlib doesn't need most of bg_misc.h
// There is only like 10-12 bytes of data here. Could we get it from "game"?
//                                      Add a VM_Call? or a cvar? (By adding a cvar we could support unmoded Q3 and games with the Cvar!)
//                                      { "g_bg_data", "5 5 6 0 1 2 3 4 0 2 3 [etc]", CVAR_ROM } ?
//                                        or as info-string "CS_WARMUP\5\PM_INTERMISSION\5\..." ?

// 	sv_bg_data = Cvar_Get ("g_bg_data", "5 5 6 0 1 2 3 4 0 2 3", CVAR_SYSTEMINFO | CVAR_ROM );

// server only
#define	CS_WARMUP				5		// server time when the match will be restarted

// client only
#define PM_INTERMISSION 5 // bg_misc.h enum pmtype_t
#define PM_SPINTERMISSION 6 // bg_misc.h enum pmtype_t

// botlib only
//NOTE: enum entityType_t in bg_public.h
//FIXME: these might change
enum {
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER
};

// server and botlib

// Turtle Man: Server and botlib need GT_SINGLE_PLAYER and [just] botlib needs GT_TEAM
//             If TMNTSP is not defined Client also needs GT_SINGLE_PLAYER and
//               Server also needs GT_FFA

//FIXME: these are game specific
typedef enum {
#ifdef TMNTSP
	GT_FFA__,				// free for all
#else
	GT_FFA,				// free for all
#endif
	GT_TOURNAMENT__,		// one on one tournament
	GT_SINGLE_PLAYER,	// single player tournament

#if 0 // #ifdef TMNT
	GT_LMS__,				// Last Man Standing
	GT_KOTH__,			// King Of The Hill
	GT_KEEPAWAY__,		// Keep Away
#endif

	//-- team games go after this --

	GT_TEAM,			// team deathmatch
#if 0 // #ifdef TMNT
	GT_LTEAMS__,			// last team standing
#endif
	GT_CTF__,				// capture the flag
#ifdef MISSIONPACK
	GT_1FCTF__,
	GT_OBELISK__,
#ifdef MISSIONPACK_HARVESTER
	GT_HARVESTER__,
#endif
#if 0 // Turtle Man: In the gametype name arrays there is a Team Tournament.
	GT_TEAMTOURNAMENT__,
#endif
#endif
	GT_MAX_GAME_TYPE
} gametype_t;
