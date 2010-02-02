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

USER INTERFACE MAIN

=======================================================================
*/


#include "ui_local.h"


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .qvm file
================
*/
Q_EXPORT intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
	switch ( command ) {
	case UI_GETAPIVERSION:
		return UI_API_VERSION;

	case UI_INIT:
		UI_Init();
		return 0;

	case UI_SHUTDOWN:
		UI_Shutdown();
		return 0;

	case UI_KEY_EVENT:
		UI_KeyEvent( arg0, arg1 );
		return 0;

	case UI_MOUSE_EVENT:
		UI_MouseEvent( arg0, arg1 );
		return 0;

	case UI_REFRESH:
		UI_Refresh( arg0 );
		return 0;

	case UI_IS_FULLSCREEN:
		return UI_IsFullscreen();

	case UI_SET_ACTIVE_MENU:
		UI_SetActiveMenu( arg0 );
		return 0;

	case UI_CONSOLE_COMMAND:
		return UI_ConsoleCommand(arg0);

	case UI_DRAW_CONNECT_SCREEN:
		UI_DrawConnectScreen( arg0 );
		return 0;
#ifdef IOQUAKE3 // Turtle Man: CDKEY
	case UI_HASUNIQUECDKEY:				// mod authors need to observe this
		return qtrue;  // change this to qfalse for mods!
#endif
	}

	return -1;
}


/*
================
cvars
================
*/

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

vmCvar_t	ui_ffa_fraglimit;
vmCvar_t	ui_ffa_timelimit;

vmCvar_t	ui_tourney_fraglimit;
vmCvar_t	ui_tourney_timelimit;

vmCvar_t	ui_team_fraglimit;
vmCvar_t	ui_team_timelimit;
vmCvar_t	ui_team_friendly;

vmCvar_t	ui_ctf_capturelimit;
vmCvar_t	ui_ctf_timelimit;
vmCvar_t	ui_ctf_friendly;

#ifdef MISSIONPACK // Turtle Man: MP_GAMETYPES
vmCvar_t	ui_1flag_capturelimit;
vmCvar_t	ui_1flag_timelimit;
vmCvar_t	ui_1flag_friendly;

vmCvar_t	ui_obelisk_capturelimit;
vmCvar_t	ui_obelisk_timelimit;
vmCvar_t	ui_obelisk_friendly;

#ifdef MISSIONPACK_HARVESTER
vmCvar_t	ui_harvester_capturelimit;
vmCvar_t	ui_harvester_timelimit;
vmCvar_t	ui_harvester_friendly;
#endif
#endif

vmCvar_t	ui_arenasFile;
vmCvar_t	ui_botsFile;
vmCvar_t	ui_spScores1;
vmCvar_t	ui_spScores2;
vmCvar_t	ui_spScores3;
vmCvar_t	ui_spScores4;
vmCvar_t	ui_spScores5;
vmCvar_t	ui_spAwards;
vmCvar_t	ui_spVideos;
vmCvar_t	ui_spSkill;

#ifdef TMNTSP
vmCvar_t	ui_singlePlayerActive;
vmCvar_t	ui_saveVersions;
vmCvar_t	ui_saveTypes;
#endif
vmCvar_t	ui_spSelection;

vmCvar_t	ui_browserMaster;
vmCvar_t	ui_browserGameType;
vmCvar_t	ui_browserSortKey;
vmCvar_t	ui_browserShowFull;
vmCvar_t	ui_browserShowEmpty;

vmCvar_t	ui_brassTime;
vmCvar_t	ui_drawCrosshair;
vmCvar_t	ui_drawCrosshairNames;
vmCvar_t	ui_marks;

vmCvar_t	ui_server1;
vmCvar_t	ui_server2;
vmCvar_t	ui_server3;
vmCvar_t	ui_server4;
vmCvar_t	ui_server5;
vmCvar_t	ui_server6;
vmCvar_t	ui_server7;
vmCvar_t	ui_server8;
vmCvar_t	ui_server9;
vmCvar_t	ui_server10;
vmCvar_t	ui_server11;
vmCvar_t	ui_server12;
vmCvar_t	ui_server13;
vmCvar_t	ui_server14;
vmCvar_t	ui_server15;
vmCvar_t	ui_server16;

vmCvar_t	ui_cdkeychecked;
vmCvar_t	ui_ioq3;

static cvarTable_t		cvarTable[] = {
#ifdef TMNTMISC // frag to score
	{ &ui_ffa_fraglimit, "ui_ffa_scorelimit", "1000", CVAR_ARCHIVE },
#else
	{ &ui_ffa_fraglimit, "ui_ffa_fraglimit", "20", CVAR_ARCHIVE },
#endif
	{ &ui_ffa_timelimit, "ui_ffa_timelimit", "0", CVAR_ARCHIVE },

#ifdef TMNTMISC // frag to score and changed KO limit to 3
	{ &ui_tourney_fraglimit, "ui_tourney_scorelimit", "0", CVAR_ARCHIVE },
#else
	{ &ui_tourney_fraglimit, "ui_tourney_fraglimit", "0", CVAR_ARCHIVE },
#endif
	{ &ui_tourney_timelimit, "ui_tourney_timelimit", "15", CVAR_ARCHIVE },

#ifdef TMNTMISC // frag to score
	{ &ui_team_fraglimit, "ui_team_scorelimit", "0", CVAR_ARCHIVE },
#else
	{ &ui_team_fraglimit, "ui_team_fraglimit", "0", CVAR_ARCHIVE },
#endif
	{ &ui_team_timelimit, "ui_team_timelimit", "20", CVAR_ARCHIVE },
	{ &ui_team_friendly, "ui_team_friendly",  "1", CVAR_ARCHIVE },

#ifdef TMNTMISC
	{ &ui_ctf_capturelimit, "ui_ctf_capturelimit", "5", CVAR_ARCHIVE },
#else
	{ &ui_ctf_capturelimit, "ui_ctf_capturelimit", "8", CVAR_ARCHIVE },
#endif
	{ &ui_ctf_timelimit, "ui_ctf_timelimit", "30", CVAR_ARCHIVE },
	{ &ui_ctf_friendly, "ui_ctf_friendly",  "0", CVAR_ARCHIVE },

#ifdef MISSIONPACK // Turtle Man: MP_GAMETYPES
	{ &ui_1flag_capturelimit, "ui_1flag_capturelimit", "5", CVAR_ARCHIVE },
	{ &ui_1flag_timelimit, "ui_1flag_timelimit", "30", CVAR_ARCHIVE },
	{ &ui_1flag_friendly, "ui_1flag_friendly",  "0", CVAR_ARCHIVE },

	{ &ui_obelisk_capturelimit, "ui_obelisk_capturelimit", "5", CVAR_ARCHIVE },
	{ &ui_obelisk_timelimit, "ui_obelisk_timelimit", "20", CVAR_ARCHIVE },
	{ &ui_obelisk_friendly, "ui_obelisk_friendly",  "0", CVAR_ARCHIVE },

#ifdef MISSIONPACK_HARVESTER
	{ &ui_harvester_capturelimit, "ui_harvester_capturelimit", "5", CVAR_ARCHIVE },
	{ &ui_harvester_timelimit, "ui_harvester_timelimit", "30", CVAR_ARCHIVE },
	{ &ui_harvester_friendly, "ui_harvester_friendly",  "0", CVAR_ARCHIVE },
#endif
#endif

	{ &ui_arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM },
	{ &ui_botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM },
	{ &ui_spScores1, "g_spScores1", "", CVAR_ARCHIVE },
	{ &ui_spScores2, "g_spScores2", "", CVAR_ARCHIVE },
	{ &ui_spScores3, "g_spScores3", "", CVAR_ARCHIVE },
	{ &ui_spScores4, "g_spScores4", "", CVAR_ARCHIVE },
	{ &ui_spScores5, "g_spScores5", "", CVAR_ARCHIVE },
	{ &ui_spAwards, "g_spAwards", "", CVAR_ARCHIVE },
	{ &ui_spVideos, "g_spVideos", "", CVAR_ARCHIVE },
	{ &ui_spSkill, "g_spSkill", "2", CVAR_ARCHIVE | CVAR_LATCH },

#ifdef TMNTSP
	{ &ui_singlePlayerActive, "ui_singlePlayerActive", "0", CVAR_SERVERINFO|CVAR_ROM},
	{ &ui_saveVersions, "g_saveVersions", BG_SAVE_VERSIONS, CVAR_ROM },
	{ &ui_saveTypes, "g_saveTypes", BG_SAVE_TYPES, CVAR_ROM },
#endif
	{ &ui_spSelection, "ui_spSelection", "", CVAR_ROM },

	{ &ui_browserMaster, "ui_browserMaster", "0", CVAR_ARCHIVE },
	{ &ui_browserGameType, "ui_browserGameType", "0", CVAR_ARCHIVE },
	{ &ui_browserSortKey, "ui_browserSortKey", "4", CVAR_ARCHIVE },
	{ &ui_browserShowFull, "ui_browserShowFull", "1", CVAR_ARCHIVE },
	{ &ui_browserShowEmpty, "ui_browserShowEmpty", "1", CVAR_ARCHIVE },

	{ &ui_brassTime, "cg_brassTime", "2500", CVAR_ARCHIVE },
#ifdef TMNTMISC
	{ &ui_drawCrosshair, "cg_drawCrosshair", "1", CVAR_ARCHIVE },
#else
	{ &ui_drawCrosshair, "cg_drawCrosshair", "4", CVAR_ARCHIVE },
#endif
	{ &ui_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
	{ &ui_marks, "cg_marks", "1", CVAR_ARCHIVE },

	{ &ui_server1, "server1", "", CVAR_ARCHIVE },
	{ &ui_server2, "server2", "", CVAR_ARCHIVE },
	{ &ui_server3, "server3", "", CVAR_ARCHIVE },
	{ &ui_server4, "server4", "", CVAR_ARCHIVE },
	{ &ui_server5, "server5", "", CVAR_ARCHIVE },
	{ &ui_server6, "server6", "", CVAR_ARCHIVE },
	{ &ui_server7, "server7", "", CVAR_ARCHIVE },
	{ &ui_server8, "server8", "", CVAR_ARCHIVE },
	{ &ui_server9, "server9", "", CVAR_ARCHIVE },
	{ &ui_server10, "server10", "", CVAR_ARCHIVE },
	{ &ui_server11, "server11", "", CVAR_ARCHIVE },
	{ &ui_server12, "server12", "", CVAR_ARCHIVE },
	{ &ui_server13, "server13", "", CVAR_ARCHIVE },
	{ &ui_server14, "server14", "", CVAR_ARCHIVE },
	{ &ui_server15, "server15", "", CVAR_ARCHIVE },
	{ &ui_server16, "server16", "", CVAR_ARCHIVE },

#ifdef IOQUAKE3 // Turtle Man: CDKEY
	{ &ui_cdkeychecked, "ui_cdkeychecked", "0", CVAR_ROM },
#endif
	{ &ui_ioq3, "ui_ioq3", "1", CVAR_ROM }
};

static int cvarTableSize = sizeof(cvarTable) / sizeof(cvarTable[0]);


/*
=================
UI_RegisterCvars
=================
*/
void UI_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
	}
}

/*
=================
UI_UpdateCvars
=================
*/
void UI_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Update( cv->vmCvar );
	}
}
