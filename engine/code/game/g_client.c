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
#include "g_local.h"

// g_client.c -- client functions that don't happen every frame

#ifndef TMNTPLAYERSYS
static vec3_t	playerMins = {-15, -15, -24};
static vec3_t	playerMaxs = {15, 15, 32};
#endif

/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for deathmatch games.
The first time a player enters the game, they will be at an 'initial' spot.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_deathmatch( gentity_t *ent ) {
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}
}

/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
equivelant to info_player_deathmatch
*/
void SP_info_player_start(gentity_t *ent) {
	ent->classname = "info_player_deathmatch";
	SP_info_player_deathmatch( ent );
}

/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
The intermission will be viewed from this point.  Target an info_notnull for the view direction.
*/
void SP_info_player_intermission( gentity_t *ent ) {

}



/*
=======================================================================

  SelectSpawnPoint

=======================================================================
*/

/*
================
SpotWouldTelefrag

================
*/
qboolean SpotWouldTelefrag( gentity_t *spot
#ifdef TMNTPLAYERSYS
	, gentity_t *ent
#endif
	)
{
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;

#ifdef TMNTPLAYERSYS
	if (ent && ent->client)
	{
		// Turtle Man: Use per-player bounding box for telefrag checking!
		VectorAdd( spot->s.origin, ent->client->pers.playercfg.bbmins, mins );
		VectorAdd( spot->s.origin, ent->client->pers.playercfg.bbmaxs, maxs );
	}
	else
	{
		// Use default Q3 mins and maxs. (Set in BG_LoadPlayerCFGFile)
		vec3_t	playerMins = {-15, -15, -24};
		vec3_t	playerMaxs = { 15,  15,  32};

		VectorAdd( spot->s.origin, playerMins, mins );
		VectorAdd( spot->s.origin, playerMaxs, maxs );
	}
#else
	VectorAdd( spot->s.origin, playerMins, mins );
	VectorAdd( spot->s.origin, playerMaxs, maxs );
#endif
	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for (i=0 ; i<num ; i++) {
		hit = &g_entities[touch[i]];
		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
		if ( hit->client) {
			return qtrue;
		}

	}

	return qfalse;
}

#ifndef IOQ3ZTM // Turtle Man: unused
/*
================
SelectNearestDeathmatchSpawnPoint

Find the spot that we DON'T want to use
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist, nearestDist;
	gentity_t	*nearestSpot;

	nearestDist = 999999;
	nearestSpot = NULL;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {

		VectorSubtract( spot->s.origin, from, delta );
		dist = VectorLength( delta );
		if ( dist < nearestDist ) {
			nearestDist = dist;
			nearestSpot = spot;
		}
	}

	return nearestSpot;
}


/*
================
SelectRandomDeathmatchSpawnPoint

go to a random point that doesn't telefrag
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
	gentity_t	*spot;
	int			count;
	int			selection;
	gentity_t	*spots[MAX_SPAWN_POINTS];

	count = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		spots[ count ] = spot;
		count++;
	}

	if ( !count ) {	// no spots that won't telefrag
		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
	}

	selection = rand() % count;
	return spots[ selection ];
}
#endif

/*
===========
SelectRandomFurthestSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectRandomFurthestSpawnPoint (
#ifdef TMNTPLAYERSYS
	gentity_t *ent,
#else
	vec3_t avoidPoint,
#endif
	vec3_t origin, vec3_t angles ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist;
	float		list_dist[64];
	gentity_t	*list_spot[64];
	int			numSpots, rnd, i, j;
#ifdef TMNTPLAYERSYS
	vec3_t avoidPoint;

	if (ent && ent->client)
	{
		VectorCopy(ent->client->ps.origin, avoidPoint);
	}
	else
	{
		VectorCopy(vec3_origin, avoidPoint);
	}
#endif

	numSpots = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot
#ifdef TMNTPLAYERSYS
			, ent
#endif
		) ) {
			continue;
		}
		VectorSubtract( spot->s.origin, avoidPoint, delta );
		dist = VectorLength( delta );
		for (i = 0; i < numSpots; i++) {
			if ( dist > list_dist[i] ) {
				if ( numSpots >= 64 )
					numSpots = 64-1;
				for (j = numSpots; j > i; j--) {
					list_dist[j] = list_dist[j-1];
					list_spot[j] = list_spot[j-1];
				}
				list_dist[i] = dist;
				list_spot[i] = spot;
				numSpots++;
				if (numSpots > 64)
					numSpots = 64;
				break;
			}
		}
		if (i >= numSpots && numSpots < 64) {
			list_dist[numSpots] = dist;
			list_spot[numSpots] = spot;
			numSpots++;
		}
	}
	if (!numSpots) {
		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
		if (!spot)
			G_Error( "Couldn't find a spawn point" );
		VectorCopy (spot->s.origin, origin);
		origin[2] += 9;
		VectorCopy (spot->s.angles, angles);
		return spot;
	}

	// select a random spot from the spawn points furthest away
	rnd = random() * (numSpots / 2);

	VectorCopy (list_spot[rnd]->s.origin, origin);
	origin[2] += 9;
	VectorCopy (list_spot[rnd]->s.angles, angles);

	return list_spot[rnd];
}

/*
===========
SelectSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectSpawnPoint (
#ifdef TMNTPLAYERSYS
	gentity_t *ent,
#else
	vec3_t avoidPoint,
#endif
	vec3_t origin, vec3_t angles ) {
	return SelectRandomFurthestSpawnPoint(
#ifdef TMNTPLAYERSYS
		ent,
#else
		avoidPoint,
#endif
		origin, angles );

	/*
	gentity_t	*spot;
	gentity_t	*nearestSpot;

	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );

	spot = SelectRandomDeathmatchSpawnPoint ( );
	if ( spot == nearestSpot ) {
		// roll again if it would be real close to point of death
		spot = SelectRandomDeathmatchSpawnPoint ( );
		if ( spot == nearestSpot ) {
			// last try
			spot = SelectRandomDeathmatchSpawnPoint ( );
		}		
	}

	// find a single player start spot
	if (!spot) {
		G_Error( "Couldn't find a spawn point" );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
	*/
}

#ifdef TMNTSP
/*
===========
SelectSinglePlayerSpawnPoint

Trys to find info_player_start with client num,
if none uses info_player_start for client 0,
if none uses last info_player_start found.
if none uses 'initial' deathmatch spawn point,
if none uses deathmatch spawn point.
============
*/
gentity_t *SelectSinglePlayerSpawnPoint(
#ifdef TMNTPLAYERSYS
	gentity_t *ent,
#endif
	int clientnum, vec3_t origin, vec3_t angles )
{
	gentity_t	*spot = NULL;
	gentity_t	*spot0 = NULL;
	gentity_t	*spotlast = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_start")) != NULL) {
		spotlast = spot;
		// Save spot 0 as it will be the default there isn't one for this client.
		if (spot->count == 0) {
			spot0 = spot;
		}
		if ( clientnum < 0 || spot->count == clientnum ) {
			break;
		}
	}
	if (!spot && spot0) {
		spot = spot0;
	}
	if (!spot && spotlast) {
		spot = spotlast;
	}

	// There are no info_player_start, use a deathmatch start.
	if (!spot)
	{
		G_Printf("Warning: Map missing info_player_start atemping to use deathmacth spawn.\n");

		while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
			if ( spot->spawnflags & 1 ) {
				break;
			}
		}
	}

	if ( !spot || SpotWouldTelefrag( spot
#ifdef TMNTPLAYERSYS
		, ent
#endif
	) ) {
		return SelectSpawnPoint(
#ifdef TMNTPLAYERSYS
		ent,
#else
		vec3_origin,
#endif
		origin, angles );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}
#endif

/*
===========
SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
gentity_t *SelectInitialSpawnPoint(
#ifdef TMNTPLAYERSYS
	gentity_t *ent,
#endif
	vec3_t origin, vec3_t angles )
{
	gentity_t	*spot;

	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( spot->spawnflags & 1 ) {
			break;
		}
	}

	if ( !spot || SpotWouldTelefrag( spot
#ifdef TMNTPLAYERSYS
		, ent
#endif
	) ) {
		return SelectSpawnPoint(
#ifdef TMNTPLAYERSYS
		ent,
#else
		vec3_origin,
#endif
		origin, angles );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}

/*
===========
SelectSpectatorSpawnPoint

============
*/
gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
	FindIntermissionPoint();

	VectorCopy( level.intermission_origin, origin );
	VectorCopy( level.intermission_angle, angles );

	return NULL;
}

/*
=======================================================================

BODYQUE

=======================================================================
*/

/*
===============
InitBodyQue
===============
*/
void InitBodyQue (void) {
	int		i;
	gentity_t	*ent;

	level.bodyQueIndex = 0;
	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
		ent = G_Spawn();
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		level.bodyQue[i] = ent;
	}
}

/*
=============
BodySink

After sitting around for five seconds, fall into the ground and dissapear
=============
*/
void BodySink( gentity_t *ent ) {
	if ( level.time - ent->timestamp > 6500 ) {
		// the body ques are never actually freed, they are just unlinked
		trap_UnlinkEntity( ent );
		ent->physicsObject = qfalse;
		return;	
	}
	ent->nextthink = level.time + 100;
	ent->s.pos.trBase[2] -= 1;
}

/*
=============
CopyToBodyQue

A player is respawning, so make an entity that looks
just like the existing corpse to leave behind.
=============
*/
void CopyToBodyQue( gentity_t *ent ) {
#if defined MISSIONPACK && !defined TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	gentity_t	*e;
	int i;
#endif
	gentity_t		*body;
	int			contents;

	trap_UnlinkEntity (ent);

	// if client is in a nodrop area, don't leave the body
	contents = trap_PointContents( ent->s.origin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		return;
	}

	// grab a body que and cycle to the next one
	body = level.bodyQue[ level.bodyQueIndex ];
	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;

	trap_UnlinkEntity (body);

	body->s = ent->s;
	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc
#if defined MISSIONPACK && !defined TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	if ( ent->s.eFlags & EF_KAMIKAZE ) {
		body->s.eFlags |= EF_KAMIKAZE;

		// check if there is a kamikaze timer around for this owner
		for (i = 0; i < MAX_GENTITIES; i++) {
			e = &g_entities[i];
			if (!e->inuse)
				continue;
			if (e->activator != ent)
				continue;
			if (strcmp(e->classname, "kamikaze timer"))
				continue;
			e->activator = body;
			break;
		}
	}
#endif
	body->s.powerups = 0;	// clear powerups
	body->s.loopSound = 0;	// clear lava burning
	body->s.number = body - g_entities;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->physicsBounce = 0;		// don't bounce
	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
		body->s.pos.trType = TR_GRAVITY;
		body->s.pos.trTime = level.time;
		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
	} else {
		body->s.pos.trType = TR_STATIONARY;
	}
	body->s.event = 0;

	// change the animation to the last-frame only, so the sequence
	// doesn't repeat anew for the body
	switch ( body->s.legsAnim & ~ANIM_TOGGLEBIT ) {
	case BOTH_DEATH1:
	case BOTH_DEAD1:
		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD1;
		break;
	case BOTH_DEATH2:
	case BOTH_DEAD2:
		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD2;
		break;
	case BOTH_DEATH3:
	case BOTH_DEAD3:
	default:
		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD3;
		break;
	}

	body->r.svFlags = ent->r.svFlags;
	VectorCopy (ent->r.mins, body->r.mins);
	VectorCopy (ent->r.maxs, body->r.maxs);
	VectorCopy (ent->r.absmin, body->r.absmin);
	VectorCopy (ent->r.absmax, body->r.absmax);

	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_CORPSE;
	body->r.ownerNum = ent->s.number;

	body->nextthink = level.time + 5000;
	body->think = BodySink;

	body->die = body_die;

#ifdef NOTRATEDM // No gibs.
	body->takedamage = qtrue;
#else
	// don't take more damage if already gibbed
	if ( ent->health <= GIB_HEALTH ) {
		body->takedamage = qfalse;
	} else {
		body->takedamage = qtrue;
	}
#endif


	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
	trap_LinkEntity (body);
}

//======================================================================


/*
==================
SetClientViewAngle

==================
*/
void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
	int			i;

	// set the delta angle
	for (i=0 ; i<3 ; i++) {
		int		cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy( angle, ent->s.angles );
	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
}

/*
================
respawn
================
*/
void respawn( gentity_t *ent ) {
	gentity_t	*tent;

	CopyToBodyQue (ent);
	ClientSpawn(ent);

#if 0 //#ifdef TMNTSP // No teleport effect in Single Player
    if (g_gametype.integer == GT_SINGLE_PLAYER)
    {
        return;
    }
#endif
	// add a teleportation effect
	tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
	tent->s.clientNum = ent->s.clientNum;
}

/*
================
TeamCount

Returns number of players on a team
================
*/
team_t TeamCount( int ignoreClientNum, int team ) {
	int		i;
	int		count = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( i == ignoreClientNum ) {
			continue;
		}
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( level.clients[i].sess.sessionTeam == team ) {
			count++;
		}
	}

	return count;
}

/*
================
TeamLeader

Returns the client number of the team leader
================
*/
int TeamLeader( int team ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( level.clients[i].sess.sessionTeam == team ) {
			if ( level.clients[i].sess.teamLeader )
				return i;
		}
	}

	return -1;
}


/*
================
PickTeam

================
*/
team_t PickTeam( int ignoreClientNum ) {
	int		counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );

	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
		return TEAM_RED;
	}
	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
		return TEAM_BLUE;
	}
	// equal team count, so join the team with the lowest score
	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
		return TEAM_RED;
	}
	return TEAM_BLUE;
}

/*
===========
ForceClientSkin

Forces a client's skin (for teamplay)
===========
*/
/*
static void ForceClientSkin( gclient_t *client, char *model, const char *skin ) {
	char *p;

	if ((p = Q_strrchr(model, '/')) != 0) {
		*p = 0;
	}

	Q_strcat(model, MAX_QPATH, "/");
	Q_strcat(model, MAX_QPATH, skin);
}
*/

/*
===========
ClientCheckName
============
*/
static void ClientCleanName(const char *in, char *out, int outSize)
{
	int outpos = 0, colorlessLen = 0, spaces = 0;

	// discard leading spaces
	for(; *in == ' '; in++);
	
	for(; *in && outpos < outSize - 1; in++)
	{
		out[outpos] = *in;

		if(*in == ' ')
		{
			// don't allow too many consecutive spaces
			if(spaces > 2)
				continue;
			
			spaces++;
		}
		else if(outpos > 0 && out[outpos - 1] == Q_COLOR_ESCAPE)
		{
			if(Q_IsColorString(&out[outpos - 1]))
			{
				colorlessLen--;
				
				if(ColorIndex(*in) == 0)
				{
					// Disallow color black in names to prevent players
					// from getting advantage playing in front of black backgrounds
					outpos--;
					continue;
				}
			}
			else
			{
				spaces = 0;
				colorlessLen++;
			}
		}
		else
		{
			spaces = 0;
			colorlessLen++;
		}
		
		outpos++;
	}

	out[outpos] = '\0';

	// don't allow empty names
	if( *out == '\0' || colorlessLen == 0)
		Q_strncpyz(out, "UnnamedPlayer", outSize );
}

#ifdef TMNTPLAYERSYS
/*
===========
G_LoadPlayer

Load animation.cfg
============
*/
void G_LoadPlayer(int clientNum, const char *inModelName, const char *inHeadModel)
{
    char *p;
    char model[MAX_QPATH]; // model name without skin
    char headModel[MAX_QPATH]; // head model name without skin
    gentity_t *ent;
    gclient_t *client;
    bg_playercfg_t *playercfg;
#ifdef TMNTWEAPSYS
    weapon_t oldDefault;
#endif
#ifdef TMNT_GAME_MODELS
    char filename[MAX_QPATH];
#ifdef IOQ3ZTM // PLAYER_DIR
	int i;
#endif
#endif

	ent = &g_entities[clientNum];
	client = ent->client;
	playercfg = &client->pers.playercfg;

    Q_strncpyz(model, inModelName, MAX_QPATH);
	Q_strncpyz(headModel, inHeadModel, MAX_QPATH);

    // Remove skin
	if ((p = Q_strrchr(model, '/')) != 0) {
		*p = 0;
	}
	if ((p = Q_strrchr(headModel, '/')) != 0) {
		*p = 0;
	}

#ifdef TMNT_GAME_MODELS
	// Load model tags

	// Currently uses models, so they may be invalid after vid_restart / level change...?
	client->pers.torsoModel = 0;
	client->pers.legsModel = 0;

#ifdef IOQ3ZTM // PLAYER_DIR
	for (i = 0; bg_playerDirs[i] != NULL; i++)
	{
		if (i == 0 || !client->pers.torsoModel)
		{
			Com_sprintf( filename, sizeof( filename ), "%s/%s/upper.md3", bg_playerDirs[i], model );
			client->pers.torsoModel = trap_RegisterModel(filename);
		}
		if (i == 0 || !client->pers.legsModel)
		{
			Com_sprintf( filename, sizeof( filename ), "%s/%s/lower.md3", bg_playerDirs[i], model );
			client->pers.legsModel = trap_RegisterModel(filename);
		}
	}
#else
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", model );
	client->pers.torsoModel = trap_RegisterModel(filename);

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", model );
	client->pers.legsModel = trap_RegisterModel(filename);
#endif
	// Server doesn't have the player,,, fall back to DEFAULT_MODEL
	if (!client->pers.torsoModel)
	{
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", DEFAULT_MODEL );
		client->pers.torsoModel = trap_RegisterModel(filename);
	}
	if (!client->pers.legsModel)
	{
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", DEFAULT_MODEL );
		client->pers.legsModel = trap_RegisterModel(filename);
	}
#endif

	// Check if player has really changed!
	if ( Q_stricmpn(inModelName, playercfg->model, MAX_QPATH) == 0
		&& Q_stricmpn(inHeadModel, playercfg->headModel, MAX_QPATH) == 0 ) {
			// no change
			return;
		}

	// Turtle Man: NOTE: This message was used to tell when a client get playercfg loaded.
	//G_Printf("DEBUG: Changed player old=%s, new=%s\n", playercfg->model, model);

	// Load animation.cfg
	if (!model[0] || !BG_LoadPlayerCFGFile(playercfg, model, headModel))
	{
		G_Printf("G_LoadPlayer: Loading player failed (%s)\n", inModelName);
		// Fall back to DEFAULT_MODEL
		Q_strncpyz(model, DEFAULT_MODEL, MAX_QPATH);
		if (!BG_LoadPlayerCFGFile(playercfg, model, headModel))
		{
			G_Printf("G_LoadPlayer: Loading default player failed (%s)\n", inModelName);
			// The defaults were loaded in BG_LoadPlayerCFGFile,
			//   so we should be able to continue...
		}
	}

	// Set model to "model[/skin]"
	Q_strncpyz(playercfg->model, inModelName, MAX_QPATH);
	Q_strncpyz(playercfg->headModel, inHeadModel, MAX_QPATH);

	VectorCopy (client->pers.playercfg.bbmins, ent->r.mins);
	VectorCopy (client->pers.playercfg.bbmaxs, ent->r.maxs);

#ifdef TMNTWEAPSYS
	// DEFAULT_DEFAULT_WEAPON
	oldDefault = g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON];

	g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON] = playercfg->default_weapon;

	{
		// Make sure melee weapons (and grapple) have unlimited "ammo"
		int defaultAmmo = -1;

		if (BG_WeapUseAmmo(g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON]))
	{
		// GUNS_AS_DEFAULT
#if 0 // Unlimited ammo so nobody can "cheat" the ammo system... Plus I don't have ammo pickups
		// Guns are cheatable, for now anyway.
		gitem_t *item = BG_FindItemForWeapon(g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON]);
			defaultAmmo = item->quantity;
#endif
	}

		// Set the ammo value.
#ifdef TMNTWEAPSYS2
		g_entities[clientNum].client->ps.stats[STAT_SAVEDAMMO] = defaultAmmo;
#else
		g_entities[clientNum].client->ps.ammo[playercfg->default_weapon] = defaultAmmo;
#endif // TMNTWEAPSYS2
	}

#ifdef TMNTWEAPSYS2
	// If not holding new default, change to it.
	if (g_entities[clientNum].client->ps.weapon != g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON])
	{
		// Check if it is the old default weapon
		if (g_entities[clientNum].client->ps.weapon == oldDefault)
		{
			// Don't allow the weapon to be picked up, as it is the player's default.
			g_entities[clientNum].client->ps.stats[STAT_AMMO] = 0;

			// Change to new default (The old one will just be goes as it has "0" ammo).
			g_entities[clientNum].client->ps.stats[STAT_NEWWEAPON] = g_entities[clientNum].client->ps.stats[STAT_DEFAULTWEAPON];
		}
	}
	else
	{
		// Only update "ammo"
		g_entities[clientNum].client->ps.stats[STAT_AMMO] = g_entities[clientNum].client->ps.stats[STAT_SAVEDAMMO];
	}
#endif // TMNTWEAPSYS2
#endif // TMNTWEAPSYS
}
#endif

/*
===========
ClientUserInfoChanged

Called from ClientConnect when the player first connects and
directly by the server system when the player updates a userinfo variable.

The game can override any of the settings and call trap_SetUserinfo
if desired.
============
*/
void ClientUserinfoChanged( int clientNum ) {
	gentity_t *ent;
	int		teamTask, teamLeader, team, health;
	char	*s;
	char	model[MAX_QPATH];
	char	headModel[MAX_QPATH];
	char	oldname[MAX_STRING_CHARS];
	gclient_t	*client;
	char	c1[MAX_INFO_STRING];
	char	c2[MAX_INFO_STRING];
	char	redTeam[MAX_INFO_STRING];
	char	blueTeam[MAX_INFO_STRING];
	char	userinfo[MAX_INFO_STRING];
	char    guid[MAX_INFO_STRING];

	ent = g_entities + clientNum;
	client = ent->client;

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	// check for malformed or illegal info strings
	if ( !Info_Validate(userinfo) ) {
		strcpy (userinfo, "\\name\\badinfo");
	}

	// check for local client
	s = Info_ValueForKey( userinfo, "ip" );
	if ( !strcmp( s, "localhost" ) ) {
		client->pers.localClient = qtrue;
	}

	// check the item prediction
	s = Info_ValueForKey( userinfo, "cg_predictItems" );
	if ( !atoi( s ) ) {
		client->pers.predictItemPickup = qfalse;
	} else {
		client->pers.predictItemPickup = qtrue;
	}

	// set name
	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
	s = Info_ValueForKey (userinfo, "name");
	ClientCleanName( s, client->pers.netname, sizeof(client->pers.netname) );

	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
		}
	}

	if ( client->pers.connected == CON_CONNECTED ) {
		if ( strcmp( oldname, client->pers.netname ) ) {
			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " renamed to %s\n\"", oldname, 
				client->pers.netname) );
		}
	}

	// set max health
#ifdef MISSIONPACK
	if (client->ps.powerups[PW_GUARD]) {
		client->pers.maxHealth = 200;
	} else {
		health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
		client->pers.maxHealth = health;
		if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
			client->pers.maxHealth = 100;
		}
	}
#else
	health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
	client->pers.maxHealth = health;
	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
		client->pers.maxHealth = 100;
	}
#endif
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;

	// set model
#ifdef TMNTSP // SPMODEL
	if ( g_singlePlayer.integer /* && g_gametype.integer == GT_SINGLE_PLAYER*/ ) {
		Q_strncpyz( model, Info_ValueForKey (userinfo, "spmodel"), sizeof( model ) );
		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "spheadmodel"), sizeof( headModel ) );
	} else
#endif
#ifndef IOQ3ZTM_NO_TEAM_MODEL
	if( g_gametype.integer >= GT_TEAM ) {
		Q_strncpyz( model, Info_ValueForKey (userinfo, "team_model"), sizeof( model ) );
		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "team_headmodel"), sizeof( headModel ) );
	} else
#endif
	{
		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "headmodel"), sizeof( headModel ) );
	}

#ifdef TMNTPLAYERSYS
    G_LoadPlayer(clientNum, model, headModel);
#endif

	// bots set their team a few frames later
	if (g_gametype.integer >= GT_TEAM && g_entities[clientNum].r.svFlags & SVF_BOT) {
		s = Info_ValueForKey( userinfo, "team" );
		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
			team = TEAM_RED;
		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
			team = TEAM_BLUE;
		} else {
			// pick the team with the least number of players
			team = PickTeam( clientNum );
		}
	}
	else {
		team = client->sess.sessionTeam;
	}

/*	NOTE: all client side now

	// team
	switch( team ) {
	case TEAM_RED:
		ForceClientSkin(client, model, "red");
//		ForceClientSkin(client, headModel, "red");
		break;
	case TEAM_BLUE:
		ForceClientSkin(client, model, "blue");
//		ForceClientSkin(client, headModel, "blue");
		break;
	}
	// don't ever use a default skin in teamplay, it would just waste memory
	// however bots will always join a team but they spawn in as spectator
	if ( g_gametype.integer >= GT_TEAM && team == TEAM_SPECTATOR) {
		ForceClientSkin(client, model, "red");
//		ForceClientSkin(client, headModel, "red");
	}
*/

#ifdef MISSIONPACK
	if (g_gametype.integer >= GT_TEAM) {
		client->pers.teamInfo = qtrue;
	} else {
		s = Info_ValueForKey( userinfo, "teamoverlay" );
		if ( ! *s || atoi( s ) != 0 ) {
			client->pers.teamInfo = qtrue;
		} else {
			client->pers.teamInfo = qfalse;
		}
	}
#else
	// teamInfo
	s = Info_ValueForKey( userinfo, "teamoverlay" );
	if ( ! *s || atoi( s ) != 0 ) {
		client->pers.teamInfo = qtrue;
	} else {
		client->pers.teamInfo = qfalse;
	}
#endif
	/*
	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
	if ( !*s || atoi( s ) == 0 ) {
		client->pers.pmoveFixed = qfalse;
	}
	else {
		client->pers.pmoveFixed = qtrue;
	}
	*/

	// team task (0 = none, 1 = offence, 2 = defence)
	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
	// team Leader (1 = leader, 0 is normal player)
	teamLeader = client->sess.teamLeader;

	// colors
	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));

	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));
	strcpy(guid, Info_ValueForKey(userinfo, "cl_guid"));

	// send over a subset of the userinfo keys so other clients can
	// print scoreboards, display models, and play custom sounds
	if (ent->r.svFlags & SVF_BOT)
	{
		s = va("n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d",
			client->pers.netname, team, model, headModel, c1, c2, 
			client->pers.maxHealth, client->sess.wins, client->sess.losses,
			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader );
	}
	else
	{
		s = va("n\\%s\\guid\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d",
			client->pers.netname, guid, client->sess.sessionTeam, model, headModel, redTeam, blueTeam, c1, c2, 
			client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader);
	}

	trap_SetConfigstring( CS_PLAYERS+clientNum, s );

	// this is not the userinfo, more like the configstring actually
	G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
}


/*
===========
ClientConnect

Called when a player begins connecting to the server.
Called again for every map change or tournement restart.

The session information will be valid after exit.

Return NULL if the client should be allowed, otherwise return
a string with the reason for denial.

Otherwise, the client will be sent the current gamestate
and will eventually get to ClientBegin.

firstTime will be qtrue the very first time a client connects
to the server machine, but qfalse on map changes and tournement
restarts.
============
*/
char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
	char		*value;
//	char		*areabits;
	gclient_t	*client;
	char		userinfo[MAX_INFO_STRING];
	gentity_t	*ent;

	ent = &g_entities[ clientNum ];

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

 	// IP filtering
 	// https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=500
 	// recommanding PB based IP / GUID banning, the builtin system is pretty limited
 	// check to see if they are on the banned IP list
	value = Info_ValueForKey (userinfo, "ip");
	if ( G_FilterPacket( value ) ) {
		return "You are banned from this server.";
	}

  // we don't check password for bots and local client
  // NOTE: local client <-> "ip" "localhost"
  //   this means this client is not running in our current process
	if ( !isBot && (strcmp(value, "localhost") != 0)) {
		// check for a password
		value = Info_ValueForKey (userinfo, "password");
		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) &&
			strcmp( g_password.string, value) != 0) {
			return "Invalid password";
		}
	}

	// they can connect
	ent->client = level.clients + clientNum;
	client = ent->client;

//	areabits = client->areabits;

	memset( client, 0, sizeof(*client) );

	client->pers.connected = CON_CONNECTING;

	// read or initialize the session data
	if ( firstTime || level.newSession ) {
		G_InitSessionData( client, userinfo );
	}
	G_ReadSessionData( client );

	if( isBot ) {
		ent->r.svFlags |= SVF_BOT;
		ent->inuse = qtrue;
		if( !G_BotConnect( clientNum, !firstTime ) ) {
			return "BotConnectfailed";
		}
	}

	// get and distribute relevent paramters
	G_LogPrintf( "ClientConnect: %i\n", clientNum );
	ClientUserinfoChanged( clientNum );

	// don't do the "xxx connected" messages if they were caried over from previous level
#ifdef TMNTSP
    // If its single player don't add message.
	if ( firstTime && !g_singlePlayer.integer)
#else
	if ( firstTime )
#endif
	{
		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname) );
	}

	if ( g_gametype.integer >= GT_TEAM &&
		client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BroadcastTeamChange( client, -1 );
	}

	// count current clients and rank for scoreboard
	CalculateRanks();

	// for statistics
//	client->areabits = areabits;
//	if ( !client->areabits )
//		client->areabits = G_Alloc( (trap_AAS_PointReachabilityAreaIndex( NULL ) + 7) / 8 );

	return NULL;
}

/*
===========
ClientBegin

called when a client has finished connecting, and is ready
to be placed into the level.  This will happen every level load,
and on transition between teams, but doesn't happen on respawns
============
*/
void ClientBegin( int clientNum ) {
	gentity_t	*ent;
	gclient_t	*client;
	gentity_t	*tent;
	int			flags;

	ent = g_entities + clientNum;

	client = level.clients + clientNum;

	if ( ent->r.linked ) {
		trap_UnlinkEntity( ent );
	}
	G_InitGentity( ent );
	ent->touch = 0;
	ent->pain = 0;
	ent->client = client;

	client->pers.connected = CON_CONNECTED;
	client->pers.enterTime = level.time;
	client->pers.teamState.state = TEAM_BEGIN;

	// save eflags around this, because changing teams will
	// cause this to happen with a valid entity, and we
	// want to make sure the teleport bit is set right
	// so the viewpoint doesn't interpolate through the
	// world to the new position
	flags = client->ps.eFlags;
	memset( &client->ps, 0, sizeof( client->ps ) );
	client->ps.eFlags = flags;
#ifdef TMNTSP
	if (g_gametype.integer == GT_SINGLE_PLAYER)
	{
		// Start with 3 lives and if not multiplayer 1 continue
		client->ps.persistant[PERS_LIVES] = 3;
		if (g_singlePlayer.integer) {
			client->ps.persistant[PERS_CONTINUES] = 1;
		}
	}
#endif

#ifdef TMNTPLAYERSYS
	G_LoadPlayer(clientNum, client->pers.playercfg.model, client->pers.playercfg.headModel);
#endif

	// locate ent at a spawn point
	ClientSpawn( ent );

	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
#ifdef TMNTSP
		if (!(g_singlePlayer.integer && g_gametype.integer == GT_SINGLE_PLAYER))
		{
#endif
		// send event
		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
		tent->s.clientNum = ent->s.clientNum;

		if ( g_gametype.integer != GT_TOURNAMENT  ) {
			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " entered the game\n\"", client->pers.netname) );
		}
#ifdef TMNTSP
		}
#endif
	}
	G_LogPrintf( "ClientBegin: %i\n", clientNum );

	// count current clients and rank for scoreboard
	CalculateRanks();
}

/*
===========
ClientSpawn

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState
============
*/
void ClientSpawn(gentity_t *ent) {
	int		index;
	vec3_t	spawn_origin, spawn_angles;
	gclient_t	*client;
	int		i;
	clientPersistant_t	saved;
	clientSession_t		savedSess;
	int		persistant[MAX_PERSISTANT];
	gentity_t	*spawnPoint;
	int		flags;
	int		savedPing;
//	char	*savedAreaBits;
	int		accuracy_hits, accuracy_shots;
	int		eventSequence;
	char	userinfo[MAX_INFO_STRING];

	index = ent - g_entities;
	client = ent->client;

	// find a spawn point
	// do it before setting health back up, so farthest
	// ranging doesn't count this client
	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		spawnPoint = SelectSpectatorSpawnPoint ( 
						spawn_origin, spawn_angles);
	} else if (g_gametype.integer >= GT_CTF ) {
		// all base oriented team games use the CTF spawn points
		spawnPoint = SelectCTFSpawnPoint ( 
#ifdef TMNTPLAYERSYS
						ent,
#endif
						client->sess.sessionTeam, 
						client->pers.teamState.state, 
						spawn_origin, spawn_angles);
#ifdef TMNTSP
	} else if (g_gametype.integer == GT_SINGLE_PLAYER) {
		spawnPoint = SelectSinglePlayerSpawnPoint(
#ifdef TMNTPLAYERSYS
						ent,
#endif
						index, spawn_origin, spawn_angles);
#endif
	} else {
		do {
			// the first spawn should be at a good looking spot
			if ( !client->pers.initialSpawn && client->pers.localClient ) {
				client->pers.initialSpawn = qtrue;
				spawnPoint = SelectInitialSpawnPoint(
#ifdef TMNTPLAYERSYS
						ent,
#endif
						spawn_origin, spawn_angles );
			} else {
				// don't spawn near existing origin if possible
				spawnPoint = SelectSpawnPoint ( 
#ifdef TMNTPLAYERSYS
					ent,
#else
					client->ps.origin, 
#endif
					spawn_origin, spawn_angles);
			}

			// Tim needs to prevent bots from spawning at the initial point
			// on q3dm0...
			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}
			// just to be symetric, we have a nohumans option...
			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}

			break;

		} while ( 1 );
	}
	client->pers.teamState.state = TEAM_ACTIVE;

#ifndef TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	// always clear the kamikaze flag
	ent->s.eFlags &= ~EF_KAMIKAZE;
#endif

	// toggle the teleport bit so the client knows to not lerp
	// and never clear the voted flag
	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT | EF_VOTED | EF_TEAMVOTED);
	flags ^= EF_TELEPORT_BIT;
#ifdef TMNT
	flags |= EF_TELE_EFFECT;
#endif

	// clear everything but the persistant data

	saved = client->pers;
	savedSess = client->sess;
	savedPing = client->ps.ping;
//	savedAreaBits = client->areabits;
	accuracy_hits = client->accuracy_hits;
	accuracy_shots = client->accuracy_shots;
	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		persistant[i] = client->ps.persistant[i];
	}
	eventSequence = client->ps.eventSequence;

	Com_Memset (client, 0, sizeof(*client));

	client->pers = saved;
	client->sess = savedSess;
	client->ps.ping = savedPing;
//	client->areabits = savedAreaBits;
	client->accuracy_hits = accuracy_hits;
	client->accuracy_shots = accuracy_shots;
	client->lastkilled_client = -1;

	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		client->ps.persistant[i] = persistant[i];
	}
	client->ps.eventSequence = eventSequence;
	// increment the spawncount so the client will detect the respawn
	client->ps.persistant[PERS_SPAWN_COUNT]++;
	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;

#ifdef TMNT // EF_TELE_EFFECT
	client->teleEffectTime = level.time;
#endif

	client->airOutTime = level.time + 12000;

	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );
	// set max health
	client->pers.maxHealth = atoi( Info_ValueForKey( userinfo, "handicap" ) );
	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
		client->pers.maxHealth = 100;
	}
	// clear entity values
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
	client->ps.eFlags = flags;

	ent->s.groundEntityNum = ENTITYNUM_NONE;
	ent->client = &level.clients[index];
	ent->takedamage = qtrue;
	ent->inuse = qtrue;
	ent->classname = "player";
	ent->r.contents = CONTENTS_BODY;
	ent->clipmask = MASK_PLAYERSOLID;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags = 0;
	
#ifdef TMNTPLAYERSYS
	VectorCopy (client->pers.playercfg.bbmins, ent->r.mins);
	VectorCopy (client->pers.playercfg.bbmaxs, ent->r.maxs);
#else
	VectorCopy (playerMins, ent->r.mins);
	VectorCopy (playerMaxs, ent->r.maxs);
#endif

	client->ps.clientNum = index;

#ifdef TMNTCAMERA
	G_ClientCameraReset(client);
#endif

#ifdef SP_NPC
	ent->slow_event = level.time + 1000;
#endif

#ifdef TMNTHOLDSYS
#ifdef TMNTHOLDABLE // Start with 10 shurikens!
	client->ps.holdable[HI_SHURIKEN] = 10;
	client->ps.holdableIndex = HI_SHURIKEN;
#else
	client->ps.holdableIndex = HI_NONE;
#endif
#endif
#ifdef TMNTWEAPSYS // Turtle Man: Respawn code. Start with default weapon. Set ammo values.
	// Set default weapon
#if defined TMNTPLAYERSYS// && defined TMNTWEAPSYS
	client->ps.stats[STAT_DEFAULTWEAPON] = client->pers.playercfg.default_weapon;
#else
	client->ps.stats[STAT_DEFAULTWEAPON] = DEFAULT_DEFAULT_WEAPON;
#endif

#ifdef TMNTWEAPSYS2
	client->ps.stats[STAT_OLDWEAPON] = WP_NONE;

	// Set default ammo values.
	client->ps.stats[STAT_SAVEDAMMO] = -1;
	client->ps.stats[STAT_OLDAMMO] = -1;
	client->ps.stats[STAT_NEWAMMO] = -1;
	client->ps.stats[STAT_AMMO] = -1;
#else
	// Set default ammo values.
	{
		int i;
		for (i = 0; i < MAX_WEAPONS/*WP_NUM_WEAPONS*/; ++i)
		{
			if (BG_WeapUseAmmo(i))
			{
				client->ps.ammo[i] = 0;
			}
			else
			{
				// If its not a gun, no ammo.
				client->ps.ammo[i] = -1;
			}
		}
	}
#endif

	{
		// Turtle Man: Start with default weapon.
		gitem_t *item = NULL;
		weapon_t weapon;

		weapon = client->ps.stats[STAT_DEFAULTWEAPON];
#ifdef TMNTWEAPSYS2
		client->ps.stats[STAT_NEWWEAPON] = weapon;
#else
		client->ps.stats[STAT_WEAPONS] = ( 1 << weapon);
#endif

		if (weapon > 0)
		{
		item = BG_FindItemForWeapon( weapon );
		}

		if (!item || (item && item->quantity == 0))
		{
			// Weapon that doesn't use ammo.
#ifdef TMNTWEAPSYS2
			client->ps.stats[STAT_AMMO] = -1;
#else
			client->ps.ammo[weapon] = -1;
#endif
		}
		else
		{
#ifdef TMNTWEAPSYS2
			client->ps.stats[STAT_AMMO] = item->quantity;

			if ( g_gametype.integer == GT_TEAM )
			{
				client->ps.stats[STAT_AMMO] /= 2;
			}
#else
			client->ps.ammo[weapon] = item->quantity;

			if ( g_gametype.integer == GT_TEAM )
			{
				client->ps.ammo[weapon] /= 2;
			}
#endif
		}
	}
#else
	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_MACHINEGUN );
	if ( g_gametype.integer == GT_TEAM ) {
		client->ps.ammo[WP_MACHINEGUN] = 50;
	} else {
		client->ps.ammo[WP_MACHINEGUN] = 100;
	}

	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GAUNTLET );
	client->ps.ammo[WP_GAUNTLET] = -1;
	client->ps.ammo[WP_GRAPPLING_HOOK] = -1;
#endif

#ifdef TMNTMISC // no health countdown
	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH];
#else
	// health will count down towards max_health
	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] + 25;
#endif

	G_SetOrigin( ent, spawn_origin );
	VectorCopy( spawn_origin, client->ps.origin );

	// the respawned flag will be cleared after the attack and jump keys come up
	client->ps.pm_flags |= PMF_RESPAWNED;

	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
	SetClientViewAngle( ent, spawn_angles );

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {

	} else {
#ifdef TMNTSP
		// Turtle Man: FIXME: Disable telefraging to Single Player?
#endif
		G_KillBox( ent );
		trap_LinkEntity (ent);

		// force the base weapon up
#ifdef TMNTWEAPSYS // Turtle Man: Set ready weapon to default weapon.
		// Turtle Man: Start with default weapon.
		client->ps.weapon = client->ps.stats[STAT_DEFAULTWEAPON];
		// Set default hands.
		client->ps.weaponHands = BG_WeaponHandsForWeaponNum(client->ps.stats[STAT_DEFAULTWEAPON]);
#else
		client->ps.weapon = WP_MACHINEGUN;
#endif
		client->ps.weaponstate = WEAPON_READY;

	}

	// don't allow full run speed for a bit
	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	client->ps.pm_time = 100;

	client->respawnTime = level.time;
	client->inactivityTime = level.time + g_inactivity.integer * 1000;
	client->latched_buttons = 0;

	// set default animations
	client->ps.torsoAnim = TORSO_STAND;
	client->ps.legsAnim = LEGS_IDLE;

	if ( level.intermissiontime ) {
		MoveClientToIntermission( ent );
	} else {
		// fire the targets of the spawn point
		G_UseTargets( spawnPoint, ent );

#ifndef TMNTWEAPSYS2 // don't select any weapon after spawning
		// select the highest weapon number available, after any
		// spawn given items have fired
		client->ps.weapon = 1;
		for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
				client->ps.weapon = i;
				break;
			}
		}
#endif
	}

#ifdef TMNTSP
	//
	// Load persistant data
	//
	// Turtle Man: TODO: Load after all clients have spawned?
	//
	G_LoadPersistant(index); // get data
#endif

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	client->ps.commandTime = level.time - 100;
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink( ent-g_entities );

	// positively link the client, even if the command times are weird
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
		trap_LinkEntity( ent );
	}

	// run the presend to set anything else
	ClientEndFrame( ent );

	// clear entity state values
	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
}


/*
===========
ClientDisconnect

Called when a player drops from the server.
Will not be called between levels.

This should NOT be called directly by any game logic,
call trap_DropClient(), which will call this and do
server system housekeeping.
============
*/
void ClientDisconnect( int clientNum ) {
	gentity_t	*ent;
	gentity_t	*tent;
	int			i;

	// cleanup if we are kicking a bot that
	// hasn't spawned yet
	G_RemoveQueuedBotBegin( clientNum );

	ent = g_entities + clientNum;
	if ( !ent->client ) {
		return;
	}

	// stop any following clients
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
			&& level.clients[i].sess.spectatorClient == clientNum ) {
			StopFollowing( &g_entities[i] );
		}
	}

	// send effect if they were completely connected
	if ( ent->client->pers.connected == CON_CONNECTED 
		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = ent->s.clientNum;

		// They don't get to take powerups with them!
		// Especially important for stuff like CTF flags
		TossClientItems( ent );
#ifdef MISSIONPACK
		TossClientPersistantPowerups( ent );
#ifdef MISSIONPACK_HARVESTER
		if( g_gametype.integer == GT_HARVESTER ) {
			TossClientCubes( ent );
		}
#endif
#endif

	}

	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );

	// if we are playing in tourney mode and losing, give a win to the other player
	if ( (g_gametype.integer == GT_TOURNAMENT )
		&& !level.intermissiontime
		&& !level.warmupTime && level.sortedClients[1] == clientNum ) {
		level.clients[ level.sortedClients[0] ].sess.wins++;
		ClientUserinfoChanged( level.sortedClients[0] );
	}

	if( g_gametype.integer == GT_TOURNAMENT &&
		ent->client->sess.sessionTeam == TEAM_FREE &&
		level.intermissiontime ) {

		trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
		level.restarted = qtrue;
		level.changemap = NULL;
		level.intermissiontime = 0;
	}

	trap_UnlinkEntity (ent);
	ent->s.modelindex = 0;
	ent->inuse = qfalse;
	ent->classname = "disconnected";
	ent->client->pers.connected = CON_DISCONNECTED;
	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
	ent->client->sess.sessionTeam = TEAM_FREE;

	trap_SetConfigstring( CS_PLAYERS + clientNum, "");

	CalculateRanks();

	if ( ent->r.svFlags & SVF_BOT ) {
		BotAIShutdownClient( clientNum, qfalse );
	}
}

#ifdef TMNTSP // save/load
/* =============
G_SavePersistant

Saves the data in a cvar so we don't have to write a file
after each level in SinglePlayer.

Should allow 64 client single player games...

Save the lives, continues, score, and holdable stuff.
================*/
void G_SavePersistant(char *nextmap)
{
	char savedata[MAX_CVAR_VALUE_STRING*2];
	char *s;
	int i;
	gclient_t *client;
#ifdef TMNTHOLDSYS
	int j;
	char holdableData[(MAX_HOLDABLE*2)+1];
	int save_array;
#endif
	int skill;

	// If in real single player auto-save on level end? in case of crash or something?...
	if (g_singlePlayer.integer) {
		trap_SendConsoleCommand( EXEC_APPEND, va("savegame autosave -minimum %s", nextmap) );
	}
	skill = trap_Cvar_VariableValue( "g_spSkill" );

	memset(savedata, 0, sizeof(savedata));
	// Save game settings.
	Com_sprintf(savedata, sizeof(savedata), "map\\%s\\sk\\%i\\", nextmap, skill);
	//G_Printf("DEBUG: Savedata=%s\n", savedata);
	// Hack for CVAR_SEVERINFO
	for (i = 0; i < strlen(savedata); i++)
	{
		if (savedata[i] == '\\')
			savedata[i] = '/';
	}
	trap_Cvar_Set("g_spSaveData", savedata);
	savedata[0] = '\0';

	for (i = 0; i < level.maxclients; i++)
	{
		client = &level.clients[i];
		if (strlen(client->pers.netname) < 1) {
			continue;
		}

#ifdef TMNTHOLDSYS
		s = va("n\\%s\\livs\\%d\\ctns\\%d\\scr\\%i\\hi\\%i",
				client->pers.netname, client->ps.persistant[PERS_LIVES],
				client->ps.persistant[PERS_CONTINUES],
				client->ps.persistant[PERS_SCORE], client->ps.holdableIndex);
#else
		s = va("n\\%s\\livs\\%d\\ctns\\%d\\scr\\%i",
				client->pers.netname, client->ps.persistant[PERS_LIVES],
				client->ps.persistant[PERS_CONTINUES],
				client->ps.persistant[PERS_SCORE]);
#endif
		Q_strncpyz(savedata, s, sizeof(savedata));

#ifdef TMNTHOLDSYS
		// Save arrays.
		holdableData[0] = '\0';
		save_array = 0;
		for (j = 1; j < MAX_HOLDABLE; j++)
		{
			if (client->ps.holdable[j] > 0) {
				// Only need to save holdables up to save_array.
				save_array = j;
			}
			s = va("%s%i", client->ps.holdable[j] < 10 ? "0" : "", client->ps.holdable[j]);
			strcat(holdableData, s);
		}
		if (save_array)
		{
			strcat(savedata, "\\hd\\"); // holdable data
			holdableData[save_array*2 - 1] = 0;
			strcat(savedata, holdableData);
		}
#endif

		// Check var bounds
		if (strlen(savedata) > MAX_CVAR_VALUE_STRING)
		{
			// Turtle Man: This shouldn't happen.
			trap_Cvar_Set("g_spSaveData", "");

			if (g_singlePlayer.value)
				G_Error("Saving SP client data failed\ntoo much data! (%i/%i)\n%s\n", strlen(savedata), MAX_CVAR_VALUE_STRING, savedata);
			else // Don't error to network
				G_Printf("Saving co-op client data failed\ntoo much data! (%i/%i)\n%s\n", strlen(savedata), MAX_CVAR_VALUE_STRING, savedata);

			return;
		}

		// Hack for CVAR_SEVERINFO
		for (i = 0; i < strlen(savedata); i++)
		{
			if (savedata[i] == '\\')
				savedata[i] = '/';
		}

#ifndef TMNTRELEASE
		G_Printf("DEBUG: Savedata=%s\n", savedata);
#endif
		trap_Cvar_Set(va("g_spSaveData%i", i), savedata);
		savedata[0] = '\0';
	}
}

void G_LoadPersistant(int clientnum)
{
	char savedata[MAX_CVAR_VALUE_STRING+1];
	gclient_t *client;
	int h, i;
	char *config;
	char *s;
#ifdef TMNTHOLDSYS
	int j;
	int array_len;
	char array_data[3];
#endif
	char currentMap[MAX_QPATH];
	int skill;

	// Single Player and Co-op only
	if (g_gametype.integer != GT_SINGLE_PLAYER)
	{
		return;
	}

	//trap_SendConsoleCommand( EXEC_APPEND, "loadgame autosave" );

	memset(savedata, 0, sizeof(savedata));
	trap_Cvar_VariableStringBuffer("g_spSaveData", savedata, sizeof(savedata));
	config = savedata;

	if (!strlen(savedata)) {
		return; // nothing to load.
	}

	// Hack for CVAR_SEVERINFO
	for (i = 0; i < strlen(savedata); i++)
	{
		if (savedata[i] == '/')
			savedata[i] = '\\';
	}
#ifndef TMNTRELEASE
	G_Printf("DEBUG: Savedata=%s\n", savedata);
#endif

	s = Info_ValueForKey(config, "map");
	trap_Cvar_VariableStringBuffer( "mapname", currentMap, sizeof(currentMap) );
	if (strcmp(currentMap, s) != 0)
	{
		trap_Cvar_Set("g_spSaveData", "");
		return;
	}

	// Read skill
	s = Info_ValueForKey(config, "sk");
	skill = atoi(s);
	if (skill < 1) {
		skill = 1;
	}
	if (skill > 5) {
		skill = 5;
	}
	if (skill != trap_Cvar_VariableValue( "g_spSkill" ))
	{
		trap_Cvar_Set("g_spSkill", va("%i", skill));
		// Map needs to be reloaded!
		G_Error("SP skill mismatch, is %i should be %i.\n", trap_Cvar_VariableValue( "g_spSkill" ), skill);
	}

	for (h = 0; h < MAX_CLIENTS; h++)
	{
		memset(savedata, 0, sizeof(savedata));
		trap_Cvar_VariableStringBuffer(va("g_spSaveData%i", h), savedata, sizeof(savedata));
		config = savedata;

		if (strlen(savedata) < 1) {
			continue; // nothing to load.
		}

		// name is used to restore playerdata
		// Turtle Man: FIXME: what if player changes there name at intermission?
		//                    Do the clients stay in the same indexes?
		s = Info_ValueForKey(config, "n");
		if (strlen(s) < 1)
			continue;
#if 1 // CLIENTNUM
		// Check if clientnum
		client = &level.clients[clientnum];

		if (strlen(client->pers.netname) < 1)
			return;
		if (strcmp(client->pers.netname, s) != 0)
			continue;
#else
		for (i = 0; i < level.maxclients; i++)
		{
			client = &level.clients[i];
			if (strlen(client->pers.netname) < 1)
				continue;
			if (strcmp(client->pers.netname, s) == 0)
				break;
			}

		if (i < level.maxclients)
#endif
		{
#ifndef TMNTRELEASE
			G_Printf("DEBUG: Loading SP data for %s\n", s);
#endif
			// Found client.
			client->ps.persistant[PERS_LIVES] = atoi(Info_ValueForKey(config, "livs"));
			if (!g_singlePlayer.integer && !client->ps.persistant[PERS_LIVES]) {
				client->ps.persistant[PERS_LIVES] = 3;
			}
			client->ps.persistant[PERS_CONTINUES] = atoi(Info_ValueForKey(config, "ctns"));
			client->ps.persistant[PERS_SCORE] = atoi(Info_ValueForKey(config, "scr"));
#ifdef TMNTHOLDSYS
			client->ps.holdableIndex = atoi(Info_ValueForKey(config, "hi"));

			// Read array
			array_data[2] = 0;
			s = Info_ValueForKey(config, "hd");
			array_len = strlen(s) / 2;
			for (j = 0; j < array_len && j < MAX_HOLDABLE; j++)
			{
				array_data[0] = s[j*2];
				array_data[1] = s[(j*2)+1];
				client->ps.holdable[j+1] = atoi(array_data);
			}
#endif
		}

		// Don't ever load this data again.
		trap_Cvar_Set(va("g_spSaveData%i", h),"");
	}

#if 0 // CLIENTNUM // Need it so can load all clients.
	trap_Cvar_Set("g_spSaveData", "");
#endif
}
#endif
