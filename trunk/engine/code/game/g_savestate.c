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
// Saving and Loading code
// Based on code by "Dairyman" (at rfactory.org) which was in turn based on
//   "Return to Castle Wolfenstein" and "Star Trek: Elite Force" savegame code.
// None of the code on which this is based is GPL.

#include "g_local.h"
#include "../qcommon/q_shared.h"

#ifdef TMNTSP
#ifndef SINGLEPLAYER // GPL code
#define	SAVE_VERSION 2 // version of save/load routines
#else
#define	SAVE_VERSION 1 // version of save/load routines
#endif

typedef enum
{
	// Remove SAVE_MINIMUM?, it was made for G_SavePersistant, which uses cvars now.
    SAVE_MINIMUM,	// Save data from one level to the next.
    SAVE_FULL,		// Full save of the level, includes all entities.
    SAVE_UNKNOWN

} save_type_e;

// Note: Save files should always be the same endianness on all platforms.

// !! If save_header_t is changed update HEADER_SIZE !!
#define HEADER_SIZE (1+1+MAX_QPATH+4+1+4+4)
typedef struct
{
    byte version; // Save file version.
    byte save_type; // Type of save file.
    char mapname[MAX_QPATH];
    byte level_time[4]; // int --unused in minimum save type.
    byte skill;

    // Safety checks.
    byte entity_size[4]; // int
    byte client_size[4]; // int

    // The header is followed by entity and client data.

} save_header_t;

// Remove intToBytes bytesToInt?
void intToBytes(int s, byte *buf)
{
	buf[3] = (s>>24);
	buf[2] = ((s>>16) & 0xFF);
	buf[1] = ((s>>8) & 0xFF);
	buf[0] = (s & 0xFF);
}

int bytesToInt(const byte *buf)
{
	return (buf[0] | buf[1]<<8 | buf[2]<<16 | buf[3]<<24);
}

void writeInt(int *i, fileHandle_t f)
{
#if 0 // DEBUG
    byte buf[4];
    intToBytes(*i, buf);
    trap_FS_Write (buf, 4, f);
#else
    trap_FS_Write (i, sizeof(int), f);
#endif
}

void readInt(int *i, fileHandle_t f)
{
#if 0 // DEBUG
    byte buf[4];
    trap_FS_Read (buf, 4, f);
    *i = bytesToInt(buf);

    // DEBUG
    //G_Printf("readInt: i=%i, from=%x%x%x%x\n", *i, buf[0], buf[1], buf[2], buf[3]);
#else
    trap_FS_Read (i, sizeof(int), f);
#endif
}

#ifndef SINGLEPLAYER // GPL code
void WriteEntity(fileHandle_t f, int entity);
void ReadEntity(fileHandle_t f, int entity);
void WriteClient(fileHandle_t f, int clnum);
void ReadClient(fileHandle_t f, int clnum);
void WriteMinimumClient(fileHandle_t f, int clnum);
void ReadMinimumClient(fileHandle_t f, int clnum);

/*
G_SaveGame

Saves the current level data so that the game can be restored later.
*/
qboolean G_SaveGame(fileHandle_t f)
{
	save_header_t header;
	qboolean fullsave = qtrue;
	char argv[MAX_QPATH];
	int i;

	if (g_gametype.integer != GT_SINGLE_PLAYER || !g_singlePlayer.integer) {
		G_Printf("Can't savegame, saving is for single player only!\n");
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	// Check for living client
	for (i=0; i < level.maxclients; i++)
	{
		if (g_entities[i].health > 0) {
			break;
		}
	}
	if (i >= level.maxclients) {
		G_Printf("Can't savegame, there must be a living player!\n");
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	// Check for minimum save
	if (trap_Argc() > 2)
	{
		trap_Argv(2, argv, MAX_QPATH);
		fullsave = Q_stricmp(argv, "-minimum");
		if (fullsave) {
			fullsave = Q_stricmp(argv, "-min");
		}
	}

	G_Printf("Saving game...%s\n", fullsave ? "" :  " (minimum save)");

	// Setup header
    memset(&header, 0, HEADER_SIZE);

    header.version = SAVE_VERSION;

	if (fullsave == qtrue)
		header.save_type = SAVE_FULL;
	else
		header.save_type = SAVE_MINIMUM;

	memset(header.mapname, 0, MAX_QPATH);

	// Check for map name override
	if (trap_Argc() > 3)
	{
		trap_Argv(3, header.mapname, MAX_QPATH);
	}
	else
	{
		trap_Cvar_VariableStringBuffer( "mapname", header.mapname, MAX_QPATH );
	}
	header.mapname[MAX_QPATH-1] = 0;

    intToBytes(level.time, header.level_time);

    header.skill = trap_Cvar_VariableValue( "g_spSkill" );

    intToBytes(sizeof (gentity_t), header.entity_size);
    intToBytes(sizeof (gclient_t), header.client_size);

    // Write header
	trap_FS_Write(&header, HEADER_SIZE, f);

	if (fullsave == qtrue)
	{
		//gentity_t	*ent;
		gclient_t	*cl;

		for (i = 0; i < level.num_entities; i++)
		{
			// Check if it should be written?
            writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteEntity(f, i);
		}
		i = -1;
		writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
		for (i = 0; i < level.maxclients; i++)
		{
			cl = &level.clients[i];
			if (cl->pers.connected != CON_CONNECTED)
				continue;
            writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteClient(f, i);
		}
	}
	else
	{
		gclient_t	*cl;
		for (i = 0; i < level.maxclients; i++)
		{
			cl = &level.clients[i];
			if (cl->pers.connected != CON_CONNECTED)
				continue;
            writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteMinimumClient(f, i);
		}
	}

	// End save file.
	i = -1;
	writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);

	// Close file
	trap_FS_FCloseFile( f );
	return qtrue;
}

void G_LoadGame(fileHandle_t f)
{
	save_header_t header;
	qboolean fullsave;

	// Read header
    memset(&header, 0, HEADER_SIZE);
	trap_FS_Read(&header, HEADER_SIZE, f);

	if (header.version != SAVE_VERSION)
	{
	    G_Printf( "Error: Savegame version is %i, expected %i!\n", header.version, SAVE_VERSION);
        return;
	}

	fullsave = (header.save_type == SAVE_FULL);

	G_Printf("Savegame save-type is %s\n", fullsave ? "Full save" : "Minimum save");

	// FINISHME
	G_Printf("Sorry the GPL savegame loading code isn't done yet\n");


	// Close file
	trap_FS_FCloseFile( f );
	return;
}

void WriteEntity(fileHandle_t f, int entity)
{
	// ...
}

void ReadEntity(fileHandle_t f, int entity)
{
	// ...
}

void WriteClient(fileHandle_t f, int clnum)
{
	// ...
}

void ReadClient(fileHandle_t f, int clnum)
{
	// ...
}

void WriteMinimumClient(fileHandle_t f, int clnum)
{
	// ...
}

void ReadMinimumClient(fileHandle_t f, int clnum)
{
	// ...
}
#else
// Turtle Man: FIXME: NON-GPL Code, my changes are GPL but the rest isn't...
//
// Function Conversion
//
// this stores all functions in the game code

typedef struct {
	char *funcStr;
	byte *funcPtr;
} funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t * ent );
} ent1_funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t *ent, gentity_t *other );
} ent2_funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t *ent, gentity_t *other, trace_t * trace );
} ent2trace1_funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t *ent, gentity_t *other, gentity_t *activator );
} ent3_funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t * self , gentity_t * attacker , int damage );
} ent2int1_funcList_t;

typedef struct {
	char *funcStr;
    void (*funcPtr)( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int meansOfDeath );
} ent3int2_funcList_t;

// declare all game functions used as pointers
// Think functions
extern void BodySink ( gentity_t * ent ) ;
extern void G_FreeEntity ( gentity_t * ed ) ;
extern void RespawnItem ( gentity_t * ent ) ;
extern void FinishSpawningItem( gentity_t *ent );
extern void locateCamera ( gentity_t * ent ) ;
extern void InitShooter_Finish( gentity_t *ent );
extern void Weapon_HookThink ( gentity_t * ent ) ;
extern void G_ExplodeMissile ( gentity_t * ent ) ;
extern void ReturnToPos1 ( gentity_t * ent ) ;
extern void Think_MatchTeam ( gentity_t * ent ) ;
extern void Think_SpawnNewDoorTrigger ( gentity_t * ent ) ;
extern void Think_BeginMoving ( gentity_t * ent ) ;
extern void Think_SetupTrainTargets ( gentity_t * ent ) ;
extern void Think_Target_Delay ( gentity_t * ent ) ;
extern void target_laser_think ( gentity_t * self ) ;
extern void target_laser_start ( gentity_t * self ) ;
extern void target_location_linkup(gentity_t *ent) ;
extern void multi_wait ( gentity_t * ent ) ;
extern void trigger_always_think ( gentity_t * ent ) ;
extern void AimAtTarget ( gentity_t * self ) ;
extern void func_timer_think ( gentity_t * self ) ;
#ifdef SP_NPC
extern void ank_shot_think ( gentity_t * ent ) ;
extern void FinishSpawningNPC ( gentity_t * ent ) ;
#endif
#ifdef SINGLEPLAYER // entity
extern void FinishSpawningMD3Anim ( gentity_t * ent ) ;
#endif

// reached functions
extern void Reached_BinaryMover ( gentity_t * ent ) ;
extern void Reached_Train ( gentity_t * ent ) ;

// blocked functions
extern void Blocked_Door ( gentity_t * ent , gentity_t * other ) ;

// touch functions
extern void Touch_Item ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_DoorTrigger ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_PlatCenterTrigger ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Plat ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Button ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Multi ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void trigger_push_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void trigger_teleporter_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void hurt_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;

// use functions
extern void Use_Item( gentity_t *ent, gentity_t *other, gentity_t *activator );
extern void Use_Shooter ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_BinaryMover ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Give ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_target_remove_powerups ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Delay ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Speaker ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Print ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Score ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void target_laser_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_teleporter_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_use_end_level ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_kill_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_relay_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void Use_Multi ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_target_push ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void hurt_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void func_timer_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_level_end_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
#ifdef SP_NPC
extern void trig_FinishSpawningNPC ( gentity_t * ent , gentity_t * a , gentity_t * b ) ;
#endif
// Turtle Man: FIXME: target_player_stop : http://rfactory.org/singleEntities.html
#ifdef SINGLEPLAYER // entity
//extern void target_player_stop ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void ActivateAnimModel ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
#endif

// pain functions
#ifdef SP_NPC
extern void PainNPC ( gentity_t * self , gentity_t * attacker , int damage ) ;
#endif

// die functions
extern void body_die ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int meansOfDeath ) ;
extern void player_die ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int meansOfDeath ) ;
#ifdef SP_NPC
extern void DieNPC ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int mod ) ;
extern void NPCBodySink( gentity_t *ent );
#endif


// conversion table for functions
ent1_funcList_t ent1_funcList[] = {
// Think functions
	{"BodySink", BodySink},
	{"G_FreeEntity", G_FreeEntity},
	{"RespawnItem", RespawnItem},
	{"FinishSpawningItem", FinishSpawningItem},
	{"locateCamera", locateCamera},
	{"InitShooter_Finish", InitShooter_Finish},
	{"Weapon_HookThink", Weapon_HookThink},
	{"G_ExplodeMissile", G_ExplodeMissile},
	{"ReturnToPos1", ReturnToPos1},
	{"Think_MatchTeam", Think_MatchTeam},
	{"Think_SpawnNewDoorTrigger", Think_SpawnNewDoorTrigger},
	{"Think_BeginMoving", Think_BeginMoving},
	{"Think_SetupTrainTargets", Think_SetupTrainTargets},
	{"Think_Target_Delay", Think_Target_Delay},
	{"target_laser_think", target_laser_think},
	{"target_laser_start", target_laser_start},
	{"target_location_linkup", target_location_linkup},
	{"multi_wait", multi_wait},
	{"trigger_always_think", trigger_always_think},
	{"AimAtTarget", AimAtTarget},
	{"func_timer_think", func_timer_think},
#ifdef SP_NPC
	{"ank_shot_think", ank_shot_think},
	{"FinishSpawningNPC", FinishSpawningNPC},
	{"NPCBodySink", NPCBodySink},
#endif
#ifdef SINGLEPLAYER // entity
	{"FinishSpawningMD3Anim", FinishSpawningMD3Anim},
#endif

// reached functions
	{"Reached_BinaryMover", Reached_BinaryMover},
	{"Reached_Train", Reached_Train},

	{0, 0}
};

ent2_funcList_t ent2_funcList[] = {
// blocked functions
	{"Blocked_Door", Blocked_Door},
	{0, 0}
};

ent2trace1_funcList_t ent2trace1_funcList[] = {
// touch functions
	{"Touch_Item", Touch_Item},
	{"Touch_DoorTrigger", Touch_DoorTrigger},
	{"Touch_PlatCenterTrigger", Touch_PlatCenterTrigger},
	{"Touch_Plat", Touch_Plat},
	{"Touch_Button", Touch_Button},
	{"Touch_Multi", Touch_Multi},
	{"trigger_push_touch", trigger_push_touch},
	{"trigger_teleporter_touch", trigger_teleporter_touch},
	{"hurt_touch", hurt_touch},
	{0, 0}
};

ent3_funcList_t ent3_funcList[] = {
// use functions
	{"Use_Item", Use_Item},
	{"Use_Shooter", Use_Shooter},
	{"Use_BinaryMover", Use_BinaryMover},
	{"Use_Target_Give", Use_Target_Give},
	{"Use_target_remove_powerups", Use_target_remove_powerups},
	{"Use_Target_Delay", Use_Target_Delay},
	{"Use_Target_Speaker", Use_Target_Speaker},
	{"Use_Target_Print", Use_Target_Print},
	{"Use_Target_Score", Use_Target_Score},
	{"target_laser_use", target_laser_use},
	{"target_teleporter_use", target_teleporter_use},
	// target_level_end_use
	{"target_level_end_use", target_level_end_use},
	{"target_kill_use", target_kill_use},
	{"target_relay_use", target_relay_use},
	{"Use_Multi", Use_Multi},
	{"Use_target_push", Use_target_push},
	{"hurt_use", hurt_use},
	{"func_timer_use", func_timer_use},
#ifdef SP_NPC
	{"trig_FinishSpawningNPC", trig_FinishSpawningNPC},
#endif
// Turtle Man: FIXME: target_player_stop : http://rfactory.org/singleEntities.html
#ifdef SINGLEPLAYER // entity
	//{"target_player_stop", target_player_stop},
	{"ActivateAnimModel", ActivateAnimModel},
#endif
	{0, 0}
};

ent2int1_funcList_t ent2int1_funcList[] = {
// pain functions
#ifdef SP_NPC
	{"PainNPC", PainNPC},
#endif
	{0, 0}
};

ent3int2_funcList_t ent3int2_funcList[] = {
// die functions
	{"body_die", body_die},
	{"player_die", player_die},
#ifdef SP_NPC
	{"DieNPC", DieNPC},
#endif
	{0, 0}
};

//
// Minimum save start
//
typedef struct {
	int		ofs;
	int		len;
} minimumField_t;

// gentity_t
// Turtle Man: I don't want to save health between levels,
//             but it will prove if the loading is working.
static minimumField_t gentityPersFields[] = {
	{FOFS(health), sizeof(int)},
	{0, 0}
};

// Turtle Man: Changed int to size_t
#define CFOFS(x) ((size_t)&(((gclient_t *)0)->x))

// gclient_t
static minimumField_t gclientPersFields[] = {
	{CFOFS(ps.weapon), sizeof(int)},
#ifndef TMNTWEAPSYS2
	{CFOFS(ps.ammo[0]), sizeof(int) * MAX_WEAPONS},
#endif
	{CFOFS(ps.persistant[0]), sizeof(int) * MAX_PERSISTANT},
	{CFOFS(ps.stats[0]), sizeof(int) * MAX_STATS},
	{CFOFS(ps.powerups[0]), sizeof(int) * MAX_POWERUPS},
#ifdef TMNTHOLDSYS
	{CFOFS(ps.holdable[0]), sizeof(int) * MAX_HOLDABLE},
#endif
	{0, 0}
};

void WriteMinimumCLient(fileHandle_t f, int clnum)
{
	minimumField_t *field;
	gclient_t *cl = &level.clients[clnum];
	gentity_t *ent = &g_entities[clnum];

	// Turtle Man: TODO: Write client's player model/skin

	// save the client fields
	for (field = gclientPersFields; field->len; field++)
	{
		// write the block
		trap_FS_Write( (void *)((byte *)cl + field->ofs), field->len, f);
	}

	// save the entity fields
	for (field=gentityPersFields ; field->len ; field++)
	{
		// write the block
		trap_FS_Write ( (void *)((byte *)ent + field->ofs), field->len, f);
	}
}

void ReadMinimumCLient(fileHandle_t f, int clnum)
{
	minimumField_t *field;
	gclient_t *cl = &level.clients[clnum];
	gentity_t *ent = &g_entities[clnum];

	// Turtle Man: TODO: Read client's player model/skin

	// read the client fields
	for (field = gclientPersFields; field->len; field++)
	{
		// read the block
		trap_FS_Read( (void *)((byte *)cl + field->ofs), field->len, f);
	}

	// read the entity fields
	for (field=gentityPersFields; field->len; field++)
	{
		// read the block
		trap_FS_Read( (void *)((byte *)ent + field->ofs), field->len, f);
	}
}
// Minimum save end

//
// types of pointers we must convert
//
typedef enum {
	F_NONE,
	F_STRING,
	F_ENTITY,
	F_ITEM,
	F_CLIENT,
	F_FUNCTION
} saveFieldtype_t;

typedef struct {
	int		ofs;
	saveFieldtype_t	type;
} saveField_t;

//
// these are the fields that cannot be saved directly,
// so they need to be converted
//
// Entity pointers
//
static saveField_t gentityFields[] = {
	{FOFS(client),		F_CLIENT},
	{FOFS(classname),	F_STRING},
	{FOFS(model),		F_STRING},
	{FOFS(model2),		F_STRING},
	{FOFS(parent),		F_ENTITY},
	{FOFS(nextTrain),	F_ENTITY},
	{FOFS(prevTrain),	F_ENTITY},
	{FOFS(message),		F_STRING},
	{FOFS(target),		F_STRING},
#ifdef TMNTENTITIES
	{FOFS(paintarget),		F_STRING},
#endif
	{FOFS(targetname),	F_STRING},
	{FOFS(team),		F_STRING},
	{FOFS(target_ent),	F_ENTITY},
	{FOFS(think),		F_FUNCTION},
	{FOFS(reached),		F_FUNCTION},
	{FOFS(blocked),		F_FUNCTION},
	{FOFS(touch),		F_FUNCTION},
	{FOFS(use),			F_FUNCTION},
	{FOFS(pain),		F_FUNCTION},
	{FOFS(die),			F_FUNCTION},
	{FOFS(chain),		F_ENTITY},
	{FOFS(enemy),		F_ENTITY},
	{FOFS(activator),	F_ENTITY},
	{FOFS(teamchain),	F_ENTITY},
	{FOFS(teammaster),	F_ENTITY},
	{FOFS(item),		F_ITEM},
#ifdef SP_NPC
	{FOFS(ns.enemy),	F_ENTITY},
#endif

	{0, 0}
};

//
// Client pointers
//
static saveField_t gclientFields[] = {
	{CFOFS(hook),		F_ENTITY},

	{0, 0}
};

//
// pointers that are not overwritten when loading a saved game
//
typedef struct {
	int		ofs;
	int		len;
} ignoreField_t;

//
// Entity ignore pointers
//
static ignoreField_t gentityIgnoreFields[] = {
#ifdef SP_NPC
	{FOFS(npc),	sizeof(bgnpc_t *)},
#endif

	{0, 0}
};

//
// Client ignore pointers
//
static ignoreField_t gclientIgnoreFields[] = {

	{0, 0}
};

//
// find a funcList entry by function address
//
funcList_t *G_FindFuncAtAddress( byte *adr )
{
    funcList_t *funcList;
    funcList_t *function;
	int i;

    funcList = (funcList_t*)ent1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
    funcList = (funcList_t*)ent2_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
    funcList = (funcList_t*)ent2trace1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
    funcList = (funcList_t*)ent3_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
    funcList = (funcList_t*)ent2int1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
    funcList = (funcList_t*)ent3int2_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (function->funcPtr == adr)
		{
			return function;
		}
	}
	return NULL;
}

//
// find a funcList entry by function name
//
byte *G_FindFuncByName( char *name )
{
    funcList_t *funcList;
    funcList_t *function;
	int i;

    funcList = (funcList_t*)ent1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
    funcList = (funcList_t*)ent2_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
    funcList = (funcList_t*)ent2trace1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
    funcList = (funcList_t*)ent3_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
    funcList = (funcList_t*)ent2int1_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
    funcList = (funcList_t*)ent3int2_funcList;
	for (i=0; funcList[i].funcStr; i++) {
        function = (funcList_t*)&funcList[i];
		if (!strcmp(name, function->funcStr))
		{
			return function->funcPtr;
		}
	}
	return NULL;
}

//
// convert a pointer when saving
//
void WriteField1 (saveField_t *field, byte *base)
{
	void		*p;
	int			len = 0;
	int			index;
	funcList_t	*func;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		if ( *(char **)p )
			len = strlen(*(char **)p) + 1;
		else
			len = 0;
		*(int *)p = len;
		break;
	case F_ENTITY:
		if ( *(gentity_t **)p == NULL)
			index = -1;
		else
			index = *(gentity_t **)p - g_entities;
		if (index >= MAX_GENTITIES || index < -1)
			G_Error( "WriteField1: entity out of range (%i)", index );
		*(int *)p = index;
		break;
	case F_CLIENT:
		if ( *(gclient_t **)p == NULL)
			index = -1;
		else
			index = *(gclient_t **)p - level.clients;
		if (index >= MAX_CLIENTS || index < -1)
			G_Error( "WriteField1: client out of range (%i)", index );
		*(int *)p = index;
		break;
	case F_ITEM:
		if ( *(gitem_t **)p == NULL)
			index = -1;
		else
			index = *(gitem_t **)p - bg_itemlist;
		*(int *)p = index;
		break;
	//	match this with a function address in the function list
	//	We then save the string equivalent of the function name.
	case F_FUNCTION:
		if (*(byte **)p == NULL)
			len = 0;
		else {
			func = G_FindFuncAtAddress( *(byte **)p );
			if (!func) {
				G_Error ("WriteField1: unknown function, cannot save game");
			}
			else
			    len = strlen(func->funcStr) + 1;
		}
		*(int *)p = len;
		break;

	default:
		G_Error ("WriteField1: unknown field type");
	}
}

//
// write out the string data
//
void WriteField2 (fileHandle_t f, saveField_t *field, byte *base)
{
	int			len;
	void		*p;
	funcList_t	*func;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		if ( (*(char **)p) ) // Turtle Man: Should be > 0!
		{
			trap_FS_Write ("STRG", 4, f);
			len = strlen(*(char **)p) + 1;
			trap_FS_Write (*(char **)p, len, f);
		}
		break;
	case F_FUNCTION:
		if ( *(byte **)p )
		{
			func = G_FindFuncAtAddress( *(byte **)p );
			if (!func) {
				G_Error ("WriteField2: unknown function, cannot save game");
			}
			else
			{
				trap_FS_Write ("STRG", 4, f);
				len = strlen(func->funcStr) + 1;
				trap_FS_Write (func->funcStr, len, f);
			}
		}
		break;
  default:
    break;
	}
}

/*
===============
WriteClient
===============
*/
void WriteClient(fileHandle_t f, gclient_t *cl)
{
	saveField_t	*field;
	gclient_t	temp;
	int length;

	// copy the structure across, then process the fields
	temp = *cl;

	// first, kill all events (assume they have been processed)
	memset( temp.ps.events, 0, sizeof(temp.ps.events) );
	memset( temp.ps.eventParms, 0, sizeof(temp.ps.eventParms) );
	temp.ps.eventSequence = 0;
	temp.ps.entityEventSequence = 0;

	// change the pointers to lengths or indexes
	for (field=gclientFields ; field->type ; field++)
	{
		WriteField1 (field, (byte *)&temp);
	}

	trap_FS_Write ("CLEN", 4, f);
	// write the block
	length = sizeof( temp );
    writeInt(&length, f); // trap_FS_Write (&length, sizeof(length), f);
	trap_FS_Write (&temp, length, f);

	// now write any allocated data following the edict
	for (field=gclientFields ; field->type ; field++)
	{
		WriteField2 (f, field, (byte *)cl);
	}

}

/*
===============
WriteEntity
===============
*/
void WriteEntity(fileHandle_t f, gentity_t *ent)
{
	saveField_t	*field;
	gentity_t	temp;
	int length;

	// copy the structure across, then process the fields
	temp = *ent;

	// first, kill all events (assume they have been processed)
	temp.s.event = 0;
	temp.s.eventParm = 0;

	// change the pointers to lengths or indexes
	for (field=gentityFields ; field->type ; field++)
	{
		WriteField1 (field, (byte *)&temp);
	}

	trap_FS_Write ("GENT", 4, f);
	// write the block
	length = sizeof( temp);
	writeInt(&length, f); // trap_FS_Write (&length, sizeof(length), f);
	trap_FS_Write (&temp, length, f);

	// now write any allocated data following the edict
	for (field=gentityFields ; field->type ; field++)
	{
		WriteField2 (f, field, (byte *)ent);
	}
}

/*
===============
G_SaveGame

Note: only call using the console,

"savegame filename <-minimum> <-[mapname-overide]>"

  returns qtrue if successful

===============
*/
qboolean G_SaveGame(fileHandle_t f)
{
	int	i;
	gentity_t	*ent;
	gclient_t	*cl;
	save_header_t header;
	qboolean fullsave = qtrue;

	if (g_entities[0].health <= 0) {
		G_Printf("Can't savegame when dead\n");
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	if (g_gametype.integer != GT_SINGLE_PLAYER || !g_singlePlayer.integer) {
		G_Printf("Can't savegame, saving is for single player only\n");
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	if (trap_Argc() > 2)
	{
		trap_Argv(2, header.mapname, MAX_QPATH);
		fullsave = Q_stricmp(header.mapname, "-minimum");
		if (fullsave)
			fullsave = Q_stricmp(header.mapname, "-min");
	}

	G_Printf("Saving game...%s\n", fullsave ? "" :  "(minimum save)");

    memset(&header, 0, HEADER_SIZE);

    header.version = SAVE_VERSION;

	if (fullsave == qtrue)
		header.save_type = SAVE_FULL;
	else
		header.save_type = SAVE_MINIMUM;

	memset(header.mapname, 0, MAX_QPATH);

	// map name override.
	if (trap_Argc() > 3)
	{
		trap_Argv(3, header.mapname, MAX_QPATH);
	}
	else
	{
		trap_Cvar_VariableStringBuffer( "mapname", header.mapname, MAX_QPATH );
	}
	header.mapname[MAX_QPATH-1] = 0;

    intToBytes(level.time, header.level_time);

    header.skill = trap_Cvar_VariableValue( "g_spSkill" );

    intToBytes(sizeof(gentity_t), header.entity_size);
    intToBytes(sizeof(gclient_t), header.client_size);

    // Write header
	trap_FS_Write (&header, HEADER_SIZE, f);

	if (fullsave == qtrue)
	{
		for (i = 0; i<level.num_entities; i++)
		{
			ent = &g_entities[i];
			if (!ent->inuse || ent->s.number == ENTITYNUM_WORLD)
				continue;
			writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteEntity (f, ent);
		}
		i = -1;
		writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);

		// write out the client structures
		for (i=0 ; i<MAX_CLIENTS ; i++)
		{
			cl = &level.clients[i];
			if (cl->pers.connected != CON_CONNECTED)
				continue;
			writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteClient (f, cl);
		}
		i = -1;
		writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
	}
	else // save only the minimum.
	{
		for (i=0 ; i<MAX_CLIENTS ; i++)
		{
			cl = &level.clients[i];
			if (cl->pers.connected != CON_CONNECTED)
				continue;
            writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
			WriteMinimumCLient(f, i);
		}
		i = -1;
		writeInt(&i, f); // trap_FS_Write (&i, sizeof(i), f);
	}

	trap_FS_FCloseFile( f );

	return qtrue;
}

//
// Read saved file
//

void ReadField (fileHandle_t f, saveField_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;
	char		funcStr[512];
	char		strhdr[5];

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "STRG") != 0)
				G_Error ("ReadField: unknown string '%s'\ncannot load game", strhdr);
			*(char **)p = G_Alloc (len);
			trap_FS_Read (*(char **)p, len, f);
		}
		break;
	case F_ENTITY:
		index = *(int *)p;
		if (index >= MAX_GENTITIES || index < -1)
			G_Error( "ReadField: entity out of range (%i)", index );
		if ( index == -1 )
			*(gentity_t **)p = NULL;
		else
			*(gentity_t **)p = &g_entities[index];
		break;
	case F_CLIENT:
		index = *(int *)p;
		if (index >= MAX_CLIENTS || index < -1)
			G_Error( "ReadField: client out of range (%i)", index );
		if ( index == -1 )
			*(gclient_t **)p = NULL;
		else
			*(gclient_t **)p = &level.clients[index];
		break;
	case F_ITEM:
		index = *(int *)p;
		if ( index == -1 )
			*(gitem_t **)p = NULL;
		else
			*(gitem_t **)p = &bg_itemlist[index];
		break;

	//relative to code segment
	case F_FUNCTION:
		len = *(int *)p;
		if (!len)
			*(byte **)p = NULL;
		else
		{
			if (len > sizeof(funcStr)) {
				G_Error ("ReadField: function name is greater than buffer (%i chars)", sizeof(funcStr));
			}
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "STRG") != 0)
				G_Error ("ReadField: unknown string '%s'\ncannot load game", strhdr);
			trap_FS_Read (funcStr, len, f);
			*(byte **)p = G_FindFuncByName( funcStr );
			if (!(*(byte **)p)) {
				G_Error ("ReadField: unknown function '%s'\ncannot load game", funcStr);
			}
		}
		break;

	default:
		G_Error ("ReadField: unknown field type");
	}
}

/*
===============
ReadClient
===============
*/
void ReadClient (fileHandle_t f, gclient_t *client, int size)
{
	saveField_t	*field;
	ignoreField_t *ifield;
	gclient_t	temp;
	gentity_t	*ent;
	int decodedSize;

	// read the encoded chunk
	readInt(&decodedSize, f); // trap_FS_Read (&decodedSize, sizeof(int), f);
	trap_FS_Read ( (byte *)&temp, decodedSize, f );

	// convert any feilds back to the correct data
	for (field=gclientFields ; field->type ; field++)
	{
		ReadField (f, field, (byte *)&temp);
	}

	// backup any fields that we don't want to read in
	for (ifield=gclientIgnoreFields ; ifield->len ; ifield++)
	{
		memcpy( ((byte *)&temp) + ifield->ofs, ((byte *)client) + ifield->ofs, ifield->len );
	}

	// now copy the temp structure into the existing structure
	memcpy( client, &temp, size );

	ent = &g_entities[client->ps.clientNum];

	trap_GetUsercmd( ent->client - level.clients, &ent->client->pers.cmd );
	SetClientViewAngle( ent, ent->client->ps.viewangles );

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	trap_GetUsercmd( ent-g_entities, &ent->client->pers.cmd );
	ent->client->ps.commandTime = ent->client->pers.cmd.serverTime - 100;
	ClientThink( ent-g_entities );

}

#ifdef SP_NPC
//
// NPC time data fixup
//
void NPCTime (gentity_t *ent, int leveltime)
{
	if(ent->ns.checkTime!=0)
		ent->ns.checkTime = ent->ns.checkTime - leveltime + level.time ;
	if(ent->ns.dontMoveTime!=0)
		ent->ns.dontMoveTime = ent->ns.dontMoveTime - leveltime + level.time ;
	if(ent->ns.attackTime!=0)
		ent->ns.attackTime = ent->ns.attackTime - leveltime + level.time ;
	if(ent->ns.fireTime!=0)
		ent->ns.fireTime = ent->ns.fireTime - leveltime + level.time ;
	if(ent->ns.meleeTime!=0)
		ent->ns.meleeTime = ent->ns.meleeTime - leveltime + level.time ;
	if(ent->ns.quakeTime!=0)
		ent->ns.quakeTime = ent->ns.quakeTime - leveltime + level.time ;
	if(ent->ns.ps.commandTime!=0)
		ent->ns.ps.commandTime = ent->ns.ps.commandTime - leveltime + level.time ;
}
#endif

//
// Entity time fixup
//
void EntTime (gentity_t *ent, int leveltime)
{
	if(ent->timestamp!=0)
		ent->timestamp = ent->timestamp - leveltime + level.time ;
	if(ent->pain_debounce_time!=0)
		ent->pain_debounce_time = ent->pain_debounce_time - leveltime + level.time ;
	if(ent->nextthink!=0)
		ent->nextthink = ent->nextthink - leveltime + level.time ;
	if(ent->s.pos.trTime!=0)
		ent->s.pos.trTime = level.time ;
}

/*
===============
ReadEntity
===============
*/
void ReadEntity (fileHandle_t f, gentity_t *ent, int size, int leveltime)
{
	saveField_t	*field;
	ignoreField_t *ifield;
	gentity_t	temp, backup, backup2;
	int decodedSize;

	backup = *ent;

	// read the encoded chunk
	readInt(&decodedSize, f); // trap_FS_Read (&decodedSize, sizeof(int), f);
	trap_FS_Read ( (byte *)&temp, decodedSize, f );

	// convert any fields back to the correct data
	for (field=gentityFields ; field->type ; field++)
	{
		ReadField (f, field, (byte *)&temp);
	}

	// backup any fields that we don't want to read in
	for (ifield=gentityIgnoreFields ; ifield->len ; ifield++)
	{
		memcpy( ((byte *)&temp) + ifield->ofs, ((byte *)ent) + ifield->ofs, ifield->len );
	}

	// kill all events (assume they have been processed)
	if (!temp.freeAfterEvent) {
		temp.s.event = 0;
		temp.s.eventParm = 0;
	}

	// now copy the temp structure into the existing structure
	memcpy( ent, &temp, size );

#ifdef SP_NPC
//
// Fixup NPCs that are dead to show only last frame of animation
//
	if(ent->npc && ent->health<=0)
	{
		ent->ns.ps.eFlags |= EF_FORCE_END_FRAME;
	}
#endif

#ifdef SINGLEPLAYER // entity
//
// Fixup misc_model_anim with hidden end to not display again
//
	if(Q_stricmp(ent->classname, "misc_model_anim")==0)
	{
		if(ent->s.modelindex2>0 && (ent->spawnflags & 2 ))
		{
			ent->s.eFlags |= EF_FORCE_END_FRAME;
		}
	}
#endif

//
// Fixup entity times
//
	EntTime (ent, leveltime);
#ifdef SP_NPC
	NPCTime (ent, leveltime);
#endif

	// notify server of changes in position/orientation
	if (ent->r.linked) {
		trap_LinkEntity( ent );
	} else {
		trap_UnlinkEntity( ent );
	}

	// if this is a mover, check areaportals
	if (ent->s.eType == ET_MOVER && ent->moverState != backup.moverState) {
		if ( ent->teammaster == ent || !ent->teammaster ) {
			if (ent->moverState == MOVER_POS1 ) {
				// closed areaportal
				trap_AdjustAreaPortalState( ent, qfalse );
			} else {	// must be open
				// portals are always opened before the mover starts to open, so we must move
				// it back to the start position, link, set portals, then move it back
				backup2 = *ent;
				*ent = backup;
				// link it at original position
				trap_LinkEntity( ent );
				// set portals
				trap_AdjustAreaPortalState( ent, qtrue );
				// put it back
				*ent = backup2;
				trap_LinkEntity( ent );
			}
		}
	}

}

/*
===============
G_LoadGame

Note: Only call using the console cmd, "loadgame"
===============
*/
void G_LoadGame(fileHandle_t f)
{
	int	i, leveltime, size, last;
	gentity_t	*ent;
	gclient_t	*cl;
	qboolean	serverEntityUpdate = qfalse;
	char strhdr[5];
	save_header_t header;
    qboolean fullsave;
    char map[MAX_QPATH];
    qboolean second_load = qfalse;

	if (g_gametype.integer != GT_SINGLE_PLAYER || !g_singlePlayer.integer) {
		G_Printf( "Can't loadgame, loading is for single player only.\n");
		return;
	}

	if (trap_Argc() > 2)
	{
		trap_Argv(2, map, MAX_QPATH);
		second_load = (Q_stricmp(map, "-two") == 0);
	}

    memset(&header, 0, HEADER_SIZE);
	trap_FS_Read(&header, HEADER_SIZE, f);

	if (header.version != SAVE_VERSION)
	{
	    G_Printf( "Error: Savegame version is %i, expected %i!\n", header.version, SAVE_VERSION);
        return;
	}

	fullsave = (header.save_type == SAVE_FULL);

	// LOAD MAP

	// Check if the map is loaded.
	trap_Cvar_VariableStringBuffer( "mapname", map, sizeof(map) );
	if (Q_stricmp(map, header.mapname) != 0)
	{
#if 1
		if (second_load == qfalse)
		{
			// Try to load the map
			trap_SendConsoleCommand( EXEC_APPEND, va( "spmap %s\n", header.mapname ) );

			trap_Argv(1, map, MAX_QPATH);
			trap_SendConsoleCommand( EXEC_APPEND, va( "loadgame %s -two\n", map) );
		}
		else
		{
			// Already tried to load the map, it failed.
			G_Printf("loadgame: Error failed to load map.\n");
		}
#else
		G_Printf("loadgame: Error savegame is for map '%s'\n", header.mapname);
#endif
		trap_FS_FCloseFile( f );
		return;
	}

	G_Printf( "Loading game... %s(map %s)\n", fullsave ? "" : "minimum save ", header.mapname);

	// set the level time
	leveltime = bytesToInt(header.level_time);

    // set the skill level
	trap_Cvar_Set( "g_spSkill", va("%i",header.skill) );

	size = bytesToInt(header.entity_size);
	if (size != sizeof(gentity_t))
	{
	    G_Printf( "Warning: Loading a savegame with a different entity size! (saved=%i,current=%i)\n", size, sizeof(gentity_t));
	}

	if (fullsave == qtrue)
	{
		last = 0;
		while (1)
		{
			readInt(&i, f); // trap_FS_Read (&i, sizeof(i), f);
			if (i < 0)
				break;
			if (i >= MAX_GENTITIES)
			{
				trap_FS_FCloseFile( f );
				G_Error( "G_LoadGame: entitynum out of range (%i, MAX = %i)\n", i, MAX_GENTITIES );
			}
			if (i >= level.num_entities)
			{	// notify server
				level.num_entities = i + 1;
				serverEntityUpdate = qtrue;
			}
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "GENT") != 0)
			{
				G_Error ("Load Entity : Unknown header '%s'\ncannot load game", strhdr);
			}
			ent = &g_entities[i];
			ReadEntity (f, ent, size, leveltime);

			// free all entities that we skipped
			for ( ; last < i; last++)
			{
				if (g_entities[last].inuse && i != ENTITYNUM_WORLD)
				{
					if (last < MAX_CLIENTS)
					{
						trap_DropClient( last, "" );
					} else
					{
						G_FreeEntity( &g_entities[last] );
					}
				}
			}
			last = i+1;
		}

		// clear all remaining entities
		for (ent = &g_entities[last]; last < MAX_GENTITIES; last++, ent++) {
			memset (ent, 0, sizeof(*ent));
			ent->classname = "freed";
			ent->freetime = level.time;
			ent->inuse = qfalse;
		}

		// read the client structures
		size = bytesToInt(header.client_size);
		if (size != sizeof(gclient_t))
		{
			G_Printf( "Warning: Loading a savegame with a different client size! (saved=%i,current=%i)\n", size, sizeof(gclient_t));
		}
		while (1)
		{
			readInt(&i, f); // trap_FS_Read (&i, sizeof(i), f);
			if (i < 0)
				break;
			if (i > MAX_CLIENTS) {
				trap_FS_FCloseFile( f );
				G_Error( "G_LoadGame: clientnum out of range\n" );
			}
			cl = &level.clients[i];
			if (cl->pers.connected == CON_DISCONNECTED) {
				trap_FS_FCloseFile( f );
				G_Error( "G_LoadGame: client mis-match in savegame" );
			}
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "CLEN") != 0)
				G_Error ("Load Client : Unknown header '%s'\ncannot load game", strhdr);
			ReadClient (f, cl, size);
		}
	}
	else // only the minimum was saved.
	{
	    int j;
		for (j=0; j<MAX_CLIENTS; j++)
		{
			readInt(&i, f); // trap_FS_Read (&i, sizeof(i), f);
			if (i < 0)
				break;
			if (i > MAX_CLIENTS) {
				trap_FS_FCloseFile( f );
				G_Error( "G_LoadGame: clientnum out of range\n" );
			}
			ReadMinimumCLient(f, i);
		}
	}

	// inform server of entity count if it has increased
	if (serverEntityUpdate) {
		// let the server system know that there are more entities
		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ),
			&level.clients[0].ps, sizeof( level.clients[0] ) );
	}

	trap_FS_FCloseFile( f );
}
#endif // SINGLEPLAYER
#endif // TMNTSP
