/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2009 Turtle Man

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

#include "g_local.h"

/* Turtle Man: TODO: Finish new general path code,
		for trains, camera "scripts", NPCs, NiGHTS mode?, 2D mode?
		and what not.

	It is however unfinish and untested.
*/

#ifdef TMNTPATHSYS
typedef enum
{
	PATHF_BEGIN		= 0x01, // path can start on this entity
	PATHF_POINT		= 0x02, // unused
	PATHF_AXIS		= 0x04, // unused Rotate around point See func_rotating
	PATHF_FINISH	= 0x08, // Last entity in path
	PATHF_CANFINISH	= 0x10, // Doesn't give error if its last in the path

	PATHF_DUMMY
} pathinfoflags_e;

typedef struct
{
	char *classname;
	int flags; // pathinfoflags_e

} gpathinfo_t;

// The first must have a non-NULL classname
gpathinfo_t gpathinfo[] =
{
	// Quake 3 (and etc) compatibility
	{ "path_corner", PATHF_BEGIN | PATHF_POINT },

	// TDC_NPC compatiblity...
	{ "npcpath", PATHF_BEGIN | PATHF_POINT | PATHF_CANFINISH },

	// TMNT Path entities
	{ "path_start", PATHF_BEGIN | PATHF_POINT }, // Is there a reason for "start"?
	{ "path_point", PATHF_POINT },
	{ "path_axis", PATHF_AXIS }, // Like SRB2's MT_AXIS, for NiGHTS mode. (entity is point to be rotated around)
	{ "path_end", PATHF_FINISH | PATHF_POINT }, // A end is not needed for circit paths, just link "end" to path_start.

#ifdef NIGHTSMODE
	// NiGHTS
	{ "nights_target", PATHF_POINT },
#endif

	// end of list marker
	{ NULL, 0 }
};

void SP_path_start( gentity_t *ent )
{
	// Setup path to avoid freeze in game?
}

/*
===========
G_SetupPath

ent is the who is going to use the path
target is the targetname of the first path entity in the path

This function also supports pervTrain, so that paths can be two way.

Based on Q3's g_mover.c's Think_SetupTrainTargets
===========
*/
gpathtype_e G_SetupPath(gentity_t *ent, const char *target)
{
	gentity_t		*path, *next, *start;
	int 			i = 0, prev_flags = 0;
	qboolean pathNotBegin = qfalse; // qtrue it trying to start a path entity without PATHF_BEGIN

	if (!ent) {
		return PATH_ERROR;
	}

	ent->prevTrain = NULL;
	ent->nextTrain = NULL;
	// Only start path on a path entity not just the first one we find,
	//    also support PATHF_BEGIN
	do {
		ent->nextTrain = G_Find( ent->nextTrain, FOFS(targetname), target );
		if ( !ent->nextTrain )
		{
			if (pathNotBegin)
				G_Printf( "Entity using paths at %s with a target of wrong path type\n",
					vtos(ent->r.absmin) );
			else
				G_Printf( "Entity using paths at %s with an unfound target\n",
					vtos(ent->r.absmin) );
			return PATH_ERROR;
		}

		// Check for all path types, not just "path_corner"
		for (i=0; gpathinfo[i].classname; i++)
		{
			if (!strcmp( ent->nextTrain->classname, gpathinfo[i].classname ))
			{
				if (gpathinfo[i].flags & PATHF_BEGIN)
				{
					pathNotBegin = qfalse;
					break;
				}
				else
				{
					pathNotBegin = qtrue;
				}
			}
		}
	} while ( !gpathinfo[i].classname );

	// First two path entities are linked, and the second one's prev is the first one.
	// This means that the path was already setup,
	if (ent->nextTrain->nextTrain &&
		ent->nextTrain->nextTrain->prevTrain == ent->nextTrain
		&& (ent->nextTrain->pathflags & PATH_FIRST))
	{
		// The path's nextTrain should only be set here is this should be fine,
		//  unless somehow a mapper changed the path after the train was started...

		if (ent->nextTrain->pathflags & PATH_CIRCIT)
			return PATH_CIRCIT;
		else if (ent->nextTrain->pathflags & PATH_LINE)
			return PATH_LINE;
		else
			return PATH_ERROR;
	}

	start = NULL;
	next = NULL;
	for ( path = ent->nextTrain ; path != start ; path = next ) {
		if ( !start ) {
			start = path;
			start->prevTrain = NULL;
			start->pathflags = PATH_FIRST;
		}

		if ( !path->target )
		{
			// End path properly
			start->pathflags = PATH_LINE | PATH_FIRST;
			path->pathflags = PATH_LINE | PATH_LAST;
			path->nextTrain = NULL;

			if (prev_flags & PATHF_CANFINISH)
			{
				return PATH_LINE; // path ended normally.
			}

			G_Printf( "Path entity at %s without a target\n",
				vtos(path->s.origin) );
			return PATH_ERROR;
		}

		// find a path_corner among the targets
		// there may also be other targets that get fired when the corner
		// is reached
		next = NULL;
		do {
			next = G_Find( next, FOFS(targetname), path->target );
			if ( !next )
			{
				// End path properly
				start->pathflags = PATH_LINE | PATH_FIRST;
				path->pathflags = PATH_LINE | PATH_LAST;
				path->nextTrain = NULL;

				if (prev_flags & PATHF_CANFINISH)
				{
					return PATH_LINE; // path ended normally.
				}

				G_Printf( "Path entity at %s without a target path entity\n",
					vtos(path->s.origin) );
				return PATH_ERROR;
			}

			// Check for all path types, not just "path_corner"
			for (i=0; gpathinfo[i].classname; i++)
			{
				if (!strcmp( next->classname, gpathinfo[i].classname ) )
					break;
			}
		} while ( !gpathinfo[i].classname );

		path->nextTrain = next;
		// Link prevTrain so we can go backward on the path too.
		next->prevTrain = path;

		// If it found a valid path entity and its end the path,
		//  quit linking.
		if (gpathinfo[i].flags & PATHF_FINISH)
		{
			start->pathflags = PATH_LINE | PATH_FIRST;
			next->pathflags = PATH_LINE | PATH_LAST;
			next->nextTrain = NULL;
			return PATH_LINE;
		}
		if ((gpathinfo[i].flags & PATHF_CANFINISH) && !next->target)
		{
			start->pathflags = PATH_LINE | PATH_FIRST;
			next->pathflags = PATH_LINE | PATH_LAST;
			next->nextTrain = NULL;
			return PATH_LINE;
		}

		prev_flags = gpathinfo[i].flags;
	}

	// If we made it here the path is a circit, so link the first and last.
	start->prevTrain = next;
	next->nextTrain = start;

	start->pathflags = PATH_CIRCIT | PATH_FIRST;
	next->pathflags = PATH_CIRCIT | PATH_LAST;

	return PATH_CIRCIT;
}

// Turtle Man: TODO: Reached_Train
//was G_NextPath
qboolean G_ReachedPath(gentity_t *ent, qboolean backward, qboolean check)
{
	gentity_t		*next, *prev;

	if (!ent) {
		return qfalse;
	}

	prev = ent->prevTrain;
	next = ent->nextTrain;

	if ( (!backward && (!next || !next->nextTrain))
		|| (backward && (!prev || !prev->prevTrain)) )
	{
		// end of train or path not setup.
		// Turtle Man: TODO: spawnflag for trains to allow PATH_LINE support or always support it?
		return qfalse;		// train just stops
	}

	// Turtle Man: Check if we have made it to the nextTrain/prevTrain
	//               Doesn't work with PATHF_AXIS!
	if (check)
	{
		vec3_t targetPos;
		vec_t dist;

		if (backward)
			VectorCopy(prev->s.origin/*s.pos*/, targetPos);
		else
			VectorCopy(next->s.origin/*s.pos*/, targetPos);

		dist = Distance(ent->s.origin/*s.pos*/, targetPos);

		// Turtle Man: Value is untested.
		if (dist > 20.0f)
		return qfalse;
		else {
			G_Printf("DEBUG: G_ReachedPath: Made it to path entity...\n");
		}
	}

	// fire all other targets
	if (backward)
		G_UseTargets( prev, ent/*NULL*/ );
	else
		G_UseTargets( next, ent/*NULL*/ );

	// Setup next move
	if (backward)
	{
		ent->nextTrain = next->prevTrain;
		ent->prevTrain = prev->prevTrain;
	}
	else
	{
		ent->nextTrain = next->nextTrain;
		ent->prevTrain = prev->nextTrain;
	}

	if (ent->s.eType == ET_MOVER) {
		// set the new trajectory
		if (backward)
		{
		VectorCopy( next->s.origin, ent->pos1 );
		VectorCopy( next->nextTrain->s.origin, ent->pos2 );
	}
		else
		{
			VectorCopy( next->s.origin, ent->pos1 );
			VectorCopy( next->nextTrain->s.origin, ent->pos2 );
		}
	}

	return qtrue;
}

void G_MoveOnPath(gentity_t *ent)
{
	if (!ent)
		return;
	if (ent->client)
	{
		// Player... NiGHTS mode or 2D mode.
	}
}
#endif
