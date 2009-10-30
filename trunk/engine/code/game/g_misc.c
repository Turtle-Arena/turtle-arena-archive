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
// g_misc.c

#include "g_local.h"


/*QUAKED func_group (0 0 0) ?
Used to group brushes together just for editor convenience.  They are turned into normal brushes by the utilities.
*/


/*QUAKED info_camp (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_camp( gentity_t *self ) {
	G_SetOrigin( self, self->s.origin );
}


/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_null( gentity_t *self ) {
	G_FreeEntity( self );
}


/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
target_position does the same thing
*/
void SP_info_notnull( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}


/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
Non-displayed light.
"light" overrides the default 300 intensity.
Linear checbox gives linear falloff instead of inverse square
Lights pointed at a target will be spotlights.
"radius" overrides the default 64 unit radius of a spotlight at the target point.
*/
void SP_light( gentity_t *self ) {
	G_FreeEntity( self );
}



/*
=================================================================================

TELEPORTERS

=================================================================================
*/

void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles ) {
	gentity_t	*tent;

	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		tent = G_TempEntity( player->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = player->s.clientNum;

		tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
		tent->s.clientNum = player->s.clientNum;
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	trap_UnlinkEntity (player);

	VectorCopy ( origin, player->client->ps.origin );
	player->client->ps.origin[2] += 1;

	// spit the player out
	AngleVectors( angles, player->client->ps.velocity, NULL, NULL );
	VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
	player->client->ps.pm_time = 160;		// hold time
	player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// set angles
	SetClientViewAngle( player, angles );

	// kill anything at the destination
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		G_KillBox (player);
	}

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );

	// use the precise origin for linking
	VectorCopy( player->client->ps.origin, player->r.currentOrigin );

	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		trap_LinkEntity (player);
	}
}


/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
Point teleporters at these.
Now that we don't have teleport destination pads, this is just
an info_notnull
*/
void SP_misc_teleporter_dest( gentity_t *ent ) {
}


//===========================================================

/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
"model"		arbitrary .md3 file to display
*/
void SP_misc_model( gentity_t *ent ) {

#if 0
	ent->s.modelindex = G_ModelIndex( ent->model );
	VectorSet (ent->r.mins, -16, -16, -16);
	VectorSet (ent->r.maxs, 16, 16, 16);
	trap_LinkEntity (ent);

	G_SetOrigin( ent, ent->s.origin );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );
#else
	G_FreeEntity( ent );
#endif
}

//===========================================================

void locateCamera( gentity_t *ent ) {
	vec3_t		dir;
	gentity_t	*target;
	gentity_t	*owner;

	owner = G_PickTarget( ent->target );
	if ( !owner ) {
		G_Printf( "Couldn't find target for misc_partal_surface\n" );
		G_FreeEntity( ent );
		return;
	}
	ent->r.ownerNum = owner->s.number;

	// frame holds the rotate speed
	if ( owner->spawnflags & 1 ) {
		ent->s.frame = 25;
	} else if ( owner->spawnflags & 2 ) {
		ent->s.frame = 75;
	}

	// swing camera ?
	if ( owner->spawnflags & 4 ) {
		// set to 0 for no rotation at all
		ent->s.powerups = 0;
	}
	else {
		ent->s.powerups = 1;
	}

	// clientNum holds the rotate offset
	ent->s.clientNum = owner->s.clientNum;

	VectorCopy( owner->s.origin, ent->s.origin2 );

	// see if the portal_camera has a target
	target = G_PickTarget( owner->target );
	if ( target ) {
		VectorSubtract( target->s.origin, owner->s.origin, dir );
		VectorNormalize( dir );
	} else {
		G_SetMovedir( owner->s.angles, dir );
	}

	ent->s.eventParm = DirToByte( dir );
}

/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/
void SP_misc_portal_surface(gentity_t *ent) {
	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	ent->r.svFlags = SVF_PORTAL;
	ent->s.eType = ET_PORTAL;

	if ( !ent->target ) {
		VectorCopy( ent->s.origin, ent->s.origin2 );
	} else {
		ent->think = locateCamera;
		ent->nextthink = level.time + 100;
	}
}

/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing
The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
"roll" an angle modifier to orient the camera around the target vector;
*/
void SP_misc_portal_camera(gentity_t *ent) {
	float	roll;

	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	G_SpawnFloat( "roll", "0", &roll );

	ent->s.clientNum = roll/360.0 * 256;
}

/*
======================================================================

  SHOOTERS

======================================================================
*/

void Use_Shooter( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	vec3_t		dir;
	float		deg;
	vec3_t		up, right;

	// see if we have a target
	if ( ent->enemy ) {
		VectorSubtract( ent->enemy->r.currentOrigin, ent->s.origin, dir );
		VectorNormalize( dir );
	} else {
		VectorCopy( ent->movedir, dir );
	}

	// randomize a bit
	PerpendicularVector( up, dir );
	CrossProduct( up, dir, right );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, up, dir );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, right, dir );

	VectorNormalize( dir );

	switch ( ent->s.weapon ) {
#ifdef TMNTWEAPONS
	case WP_GUN:
		fire_gun( ent, ent->s.origin, dir );
		break;
	case WP_ELECTRIC_LAUNCHER:
		fire_plasma( ent, ent->s.origin, dir );
		break;
	case WP_ROCKET_LAUNCHER:
		fire_rocket( ent, ent->s.origin, dir );
		break;
	case WP_HOMING_LAUNCHER:
		fire_homingrocket( ent, ent->s.origin, dir );
		break;
#else
	case WP_GRENADE_LAUNCHER:
		fire_grenade( ent, ent->s.origin, dir );
		break;
	case WP_ROCKET_LAUNCHER:
		fire_rocket( ent, ent->s.origin, dir );
		break;
	case WP_PLASMAGUN:
		fire_plasma( ent, ent->s.origin, dir );
		break;
#endif
	}

	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
}


#ifdef TMNTSP // save/load
void InitShooter_Finish( gentity_t *ent ) {
#else
static void InitShooter_Finish( gentity_t *ent ) {
#endif
	ent->enemy = G_PickTarget( ent->target );
	ent->think = 0;
	ent->nextthink = 0;
}

void InitShooter( gentity_t *ent, int weapon ) {
	ent->use = Use_Shooter;
	ent->s.weapon = weapon;

	RegisterItem( BG_FindItemForWeapon( weapon ) );

	G_SetMovedir( ent->s.angles, ent->movedir );

	if ( !ent->random ) {
		ent->random = 1.0;
	}
	ent->random = sin( M_PI * ent->random / 180 );
	// target might be a moving object, so we can't set movedir for it
	if ( ent->target ) {
		ent->think = InitShooter_Finish;
		ent->nextthink = level.time + 500;
	}
	trap_LinkEntity( ent );
}

/*QUAKED shooter_rocket (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_rocket( gentity_t *ent ) {
	InitShooter( ent, WP_ROCKET_LAUNCHER );
}

/*QUAKED shooter_plasma (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_plasma( gentity_t *ent ) {
#ifdef TMNTWEAPONS
	InitShooter( ent, WP_ELECTRIC_LAUNCHER);
#else
	InitShooter( ent, WP_PLASMAGUN);
#endif
}

/*QUAKED shooter_grenade (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_grenade( gentity_t *ent ) {
#ifdef TMNTWEAPONS
	InitShooter( ent, WP_HOMING_LAUNCHER);
#else
	InitShooter( ent, WP_GRENADE_LAUNCHER);
#endif
}


#ifdef MISSIONPACK
static void PortalDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	G_FreeEntity( self );
	//FIXME do something more interesting
}


void DropPortalDestination( gentity_t *player ) {
	gentity_t	*ent;
	vec3_t		snapped;

	// create the portal destination
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex( "models/powerups/teleporter/tele_exit.md3" );

	VectorCopy( player->s.pos.trBase, snapped );
	SnapVector( snapped );
	G_SetOrigin( ent, snapped );
	VectorCopy( player->r.mins, ent->r.mins );
	VectorCopy( player->r.maxs, ent->r.maxs );

	ent->classname = "hi_portal destination";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE;
	ent->takedamage = qtrue;
	ent->health = 200;
	ent->die = PortalDie;

	VectorCopy( player->s.apos.trBase, ent->s.angles );

	ent->think = G_FreeEntity;
	ent->nextthink = level.time + 2 * 60 * 1000;

	trap_LinkEntity( ent );

	player->client->portalID = ++level.portalSequence;
	ent->count = player->client->portalID;

	// give the item back so they can drop the source now
#ifdef TMNTHOLDSYS
	player->client->ps.holdableIndex = HI_PORTAL;
#else
	player->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItem( "Portal" ) - bg_itemlist;
#endif
}


static void PortalTouch( gentity_t *self, gentity_t *other, trace_t *trace) {
	gentity_t	*destination;

	// see if we will even let other try to use it
	if( other->health <= 0 ) {
		return;
	}
	if( !other->client ) {
		return;
	}
//	if( other->client->ps.persistant[PERS_TEAM] != self->spawnflags ) {
//		return;
//	}

	if ( other->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_NEUTRALFLAG ), 0 );
		other->client->ps.powerups[PW_NEUTRALFLAG] = 0;
	}
	else if ( other->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_REDFLAG ), 0 );
		other->client->ps.powerups[PW_REDFLAG] = 0;
	}
	else if ( other->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_BLUEFLAG ), 0 );
		other->client->ps.powerups[PW_BLUEFLAG] = 0;
	}

	// find the destination
	destination = NULL;
	while( (destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL ) {
		if( destination->count == self->count ) {
			break;
		}
	}

	// if there is not one, die!
	if( !destination ) {
		if( self->pos1[0] || self->pos1[1] || self->pos1[2] ) {
			TeleportPlayer( other, self->pos1, self->s.angles );
		}
		G_Damage( other, other, other, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG );
		return;
	}

	TeleportPlayer( other, destination->s.pos.trBase, destination->s.angles );
}


static void PortalEnable( gentity_t *self ) {
	self->touch = PortalTouch;
	self->think = G_FreeEntity;
	self->nextthink = level.time + 2 * 60 * 1000;
}


void DropPortalSource( gentity_t *player ) {
	gentity_t	*ent;
	gentity_t	*destination;
	vec3_t		snapped;

	// create the portal source
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex( "models/powerups/teleporter/tele_enter.md3" );

	VectorCopy( player->s.pos.trBase, snapped );
	SnapVector( snapped );
	G_SetOrigin( ent, snapped );
	VectorCopy( player->r.mins, ent->r.mins );
	VectorCopy( player->r.maxs, ent->r.maxs );

	ent->classname = "hi_portal source";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE | CONTENTS_TRIGGER;
	ent->takedamage = qtrue;
	ent->health = 200;
	ent->die = PortalDie;

	trap_LinkEntity( ent );

	ent->count = player->client->portalID;
	player->client->portalID = 0;

//	ent->spawnflags = player->client->ps.persistant[PERS_TEAM];

	ent->nextthink = level.time + 1000;
	ent->think = PortalEnable;

	// find the destination
	destination = NULL;
	while( (destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL ) {
		if( destination->count == ent->count ) {
			VectorCopy( destination->s.pos.trBase, ent->pos1 );
			break;
		}
	}

}
#endif
#ifdef SINGLEPLAYER // entity
void FinishSpawningMD3Anim( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		dest;
	ent->s.modelindex = G_ModelIndex( ent->model );
	ent->s.eType = ET_MODELANIM;

	if ( ! (ent->spawnflags & 4 )) {
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		// drop to floor
		ent->r.contents = CONTENTS_BODY;
		VectorSet( ent->r.mins, -24, -24, -24 );
		VectorSet( ent->r.maxs, 24, 24, 24 );
		{
			char modelName[MAX_QPATH];
			char		fname[MAX_QPATH];
			int			len;
			char		text[10000],*text_p;
			fileHandle_t	f;

			trap_GetConfigstring( CS_MODELS+ent->s.modelindex ,modelName,sizeof(modelName));
			strcpy(fname,modelName);

			text_p=fname+strlen(fname);
			while (*text_p!='.' && *text_p!='\\' && text_p!=fname)
				text_p--;
			if (*text_p=='.')
				*text_p=0;
			Q_strcat(fname,sizeof(fname),".cfg");
			len = trap_FS_FOpenFile( fname, &f, FS_READ );
			if ( len <= 0 )
			{
				G_Printf("No config file found for %s , defaults will be used.\n",modelName);
				goto read_times;
			}
			if ( len >= sizeof( text ) - 1 )
			{
				G_Printf( "File %s too long\n", fname );
				goto read_times;
			}
			trap_FS_Read( text, len, f );
			text[len] = 0;
			trap_FS_FCloseFile( f );
			text_p=text;
			Parse1DMatrix (&text_p, 3, ent->r.mins);
			Parse1DMatrix (&text_p, 3, ent->r.maxs);
		read_times:
			;
		}
		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
		if ( tr.startsolid ) {
			G_Printf ("FinishSpawningMD3Anim: %s startsolid at %s\n",
                     ent->classname, vtos(ent->s.origin));
			G_FreeEntity( ent );
			return;
		}
		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;
		G_SetOrigin( ent, tr.endpos );
	}
	trap_LinkEntity (ent);
}

void ActivateAnimModel(gentity_t *ent,gentity_t *other,gentity_t *activator) {
	ent->s.modelindex2++;
}

/*QUAKED misc_model_anim (1 0 0) (-16 -16 -16) (16 16 16) HIDDEN_START HIDDEN_END NOTSUSPENDED
Animated MD3, select HIDDEN_START for the model to be hidden until de animation
 is triggered, and HIDDEN_END for disappearing after it's finished.
"model"		arbitrary .md3 file to display/animate
"speed"		speed scale (not less than 1.0) - used to scale ".anim" fps data (default = 1)
"wait"		1=wait for trigger activation  0=animate w/o waiting (default = 0)
*/

void SP_misc_model_anim( gentity_t *ent ) {
	G_SpawnFloat( "speed", "1.0", &ent->speed);
	if (ent->speed<1) ent->speed=1;
	VectorCopy( ent->s.angles, ent->s.apos.trBase );
	ent->nextthink = level.time + FRAMETIME * 2;
	ent->think = FinishSpawningMD3Anim;
	G_SpawnFloat( "wait", "0", &ent->wait);
	ent->s.modelindex2=0;
	ent->s.generic1=ent->spawnflags + ((int)(ent->speed * 16) & 0xF0);	// solo 3 flags
	if (ent->wait==0)
		ActivateAnimModel(ent,ent,ent);
	else
		ent->use = ActivateAnimModel;
}

void G_RunMD3Anim( gentity_t *ent ) {
	vec3_t		origin;
	trace_t		tr;
	int			contents;
	int			mask;
	// if groundentity has been set to -1, it may have been pushed off an edge
	if ( ent->s.groundEntityNum == -1 ) {
		if ( ent->s.pos.trType != TR_GRAVITY ) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
		}
	}
	if ( ent->s.pos.trType == TR_STATIONARY ) {
		// check think function
		G_RunThink( ent );
		return;
	}
	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
	// trace a line from the previous position to the current position
	if ( ent->clipmask ) {
		mask = ent->clipmask;
	} else {
		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY;//MASK_SOLID;
	}
	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin,
		ent->r.ownerNum, mask );
	VectorCopy( tr.endpos, ent->r.currentOrigin );
	if ( tr.startsolid ) {
		tr.fraction = 0;
	}
	trap_LinkEntity( ent );	// FIXME: avoid this for stationary?
	// check think function
	G_RunThink( ent );
	if ( tr.fraction == 1 ) {
		return;
	}
	// if it is in a nodrop volume, remove it
	contents = trap_PointContents( ent->r.currentOrigin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		G_FreeEntity( ent );
		return;
	}
}
#endif
#ifdef TMNTENTSYS // MISC_OBJECT
// Turtle Man: TODO: There should also be func_* for brushes that acts mostly the same.

/*QUAKED misc_object (1 0 0) (-16 -16 -16) (16 16 16) suspended knockback unsoliddeath
"model"		arbitrary .md3 file to display

: TODO: Animated objects!
: "*_anim" "first   count   looping   fps"
"normal_anim"	Start on spawn
"d1_anim"		Play/change to on damage (level 1)
"d2_anim"		Play/change to on damage (level 2)
"d3_anim"		Play/change to on damage (level 3)
"dead_anim"		Play/change to on death  (level 4)

"health"	if health > 0 object can be damaged and killed. (default 0)
"mins"		object's min bounds (default "-16 -16 0")
"maxs"		object's max bounds (default "16 16 32")
"targetname" TODO: Kill (or damage?) when triggered.
"target"    if inital health > 0, trigger targets on death, else trigger targets on touch.
"paintarget" Called on pain (if health > 0)
"wait"      Time in seconds before respawning (default 0)
*/

// misc_object flags
#define MOBJF_SUSPENDED 1
#define MOBJF_KNOCKBACK 2
#define MOBJF_UNSOLIDDEATH 4

// animations
#define OBJECT_NORMAL		0
#define OBJECT_DAMAGED1		1
#define OBJECT_DAMAGED2		2
#define OBJECT_DAMAGED3		3
#define OBJECT_KILLED		4

void misc_object_pain(gentity_t *self, gentity_t *attacker, int damage)
{
	// Change to damge animation at X health
	if (self->health < (self->activator->health/5) * 2)
	{
		self->s.modelindex2 = OBJECT_DAMAGED3;
	}
	else if (self->health < (self->activator->health/5) * 3)
	{
		self->s.modelindex2 = OBJECT_DAMAGED2;
	}
	else if (self->health < (self->activator->health/5) * 4)
	{
		self->s.modelindex2 = OBJECT_DAMAGED1;
	}

	// TODO: Limit how soon to call paintarget again?
	if ( self->paintarget )
	{
		G_UseTargets2(self, attacker, self->paintarget);
	}
}

gentity_t *misc_object_spawn(gentity_t *owner, vec3_t origin, vec3_t angles);

void misc_object_respawn(gentity_t *self)
{
	if (self->activator->spawnflags & MOBJF_KNOCKBACK)
	{
		// Turtle Man: FIXME: Not right if "self" moved from origin.

		// MOBJ_KNOCKBACK doesn't work with respawning if...
		//  clone spawning or on train? (or other moving brush?)
		misc_object_spawn(self->activator, self->activator->s.origin, self->activator->s.angles);
	}
	else
	{
		misc_object_spawn(self->activator, self->s.origin, self->s.angles);
	}

	G_FreeEntity(self);
}

void misc_object_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	// Change to dead animation.
	self->s.modelindex2 = OBJECT_KILLED;

	// DEBUG: Stop drawing model, so I know its dead...
	//self->s.modelindex = 0;

	if (self->activator->spawnflags & MOBJF_UNSOLIDDEATH)
	{
		// Remove body flag
		self->r.contents = 0;
	}

	// if respawn
	if (self->wait > 0)
	{
		// Respawn after X seconds? -if a flag is set?
		//  -time should be editable
		self->nextthink = level.time + (self->wait * 1000);
		self->think = misc_object_respawn;
	}

	if( self->target )
	{
		G_UseTargets(self, attacker);
	}
}

void misc_object_think(gentity_t *self)
{
	self->nextthink = level.time + 1000;

	// Do once per-second thinking code.
	// Such as animation (that should be in cgame...)
}

void misc_object_touch(gentity_t *self, gentity_t *activator, trace_t *trace)
{
	if( self->target )
	{
		G_UseTargets(self, activator);
	}
}

// Basd on SpawnObelisk
// Use info in owner to spawn object at origin with angles.
//  should allow "clone" spawning of misc_objects and easy respawning
gentity_t *misc_object_spawn(gentity_t *owner, vec3_t origin, vec3_t angles)
{
	trace_t		tr;
	vec3_t		dest;
	gentity_t	*ent;

	ent = G_Spawn();

	VectorCopy( origin, ent->s.origin );
	VectorCopy( origin, ent->s.pos.trBase );
	VectorCopy( origin, ent->r.currentOrigin );

	VectorCopy( angles, ent->s.angles );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	VectorCopy(owner->r.mins, ent->r.mins);
	VectorCopy(owner->r.maxs, ent->r.maxs);
	ent->target = owner->target;

	ent->s.eType = ET_MISCOBJECT; // ET_GENERAL;
	if (!(owner->spawnflags & MOBJF_KNOCKBACK)) {
		ent->flags = FL_NO_KNOCKBACK;
	}

	// Setup stuff.
	ent->s.modelindex = G_ModelIndex( owner->model );

	// undamaged animation
	ent->s.modelindex2 = OBJECT_NORMAL;

	if (owner->health > 0) {
		ent->r.contents = CONTENTS_BODY;// CONTENTS_SOLID;
		ent->health = owner->health;
		//if (ent->health) {
			ent->takedamage = qtrue;
		//}
		ent->die = misc_object_die;
		ent->pain = misc_object_pain;
		ent->think = misc_object_think;
		ent->nextthink = level.time + 1000;
	}
	else if ( ent->target ) {
		ent->r.contents = CONTENTS_TRIGGER;
		ent->touch = misc_object_touch;
	}

	if ( owner->spawnflags & MOBJF_SUSPENDED ) {
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		// mappers like to put them exactly on the floor, but being coplanar
		// will sometimes show up as starting in solid, so lif it up one pixel
		ent->s.origin[2] += 1;

		// drop to floor
		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
		if ( tr.startsolid ) {
			ent->s.origin[2] -= 1;
			G_Printf( "Misc_object: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin) );

			ent->s.groundEntityNum = ENTITYNUM_NONE;
			G_SetOrigin( ent, ent->s.origin );
		}
		else {
			// allow to ride movers
			ent->s.groundEntityNum = tr.entityNum;
			G_SetOrigin( ent, tr.endpos );
		}
	}

	ent->activator = owner;

	trap_LinkEntity( ent );

	return ent;
}

// TODO: Make this / CG_SetFileExt be Com_SetFileExt ?
void G_SetFileExt(char *filename, char *ext)
{
	int i;

	for (i = 0; filename[i] && filename[i] != '.'; i++)
	{
		// just increase i.
	}
	filename[i] = 0;

	Q_strcat(filename, MAX_QPATH, ext);
}

// Based on SP_misc_model and SP_team_redobelisk
// The passed "ent" is the misc_object controler.
void SP_misc_object( gentity_t *ent ) {
	G_SpawnVector( "mins", "-16 -16 0", ent->r.mins );
	G_SpawnVector( "maxs", "16 16 32", ent->r.maxs );

	G_SetOrigin( ent, ent->s.origin );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	// Use a animation config file!
	{
		/*
		#define MAX_MISC_OBJECT_ANIMATIONS 5
		const char *misc_object_anim_names[MAX_MISC_OBJECT_ANIMATIONS] =
		{
			"OBJECT_NORMAL",
			"OBJECT_DAMAGED1",
			"OBJECT_DAMAGED2",
			"OBJECT_DAMAGED3",
			"OBJECT_KILLED"
		};

		animation_t animations[MAX_MISC_OBJECT_ANIMATIONS];
		*/
		char filename[MAX_QPATH];
		int health;
		int wait;
		float speed;

		// so we can check if they were changed.
		health = wait = speed = -2;

		trap_GetConfigstring( CS_MODELS + ent->s.modelindex, filename, sizeof(filename));

		G_SetFileExt(filename, ".cfg");

		BG_ParseObjectCFGFile(filename, NULL, NULL, 0, // names, animations, max_anim
			ent->r.mins, ent->r.maxs, &health, &wait, &speed);

		// The data in this entity over-rides the cfg file.
		//  if loaded data from cfg and ent had default, use cfg data
		if (health != -2 && ent->health == 0)
		{
			// Use health from cfg.
			ent->health = health;
		}
		if (wait != -2 && ent->wait == 0)
		{
			// Use wait from cfg.
			ent->wait = wait;
		}
		if (speed != -2 && ent->speed == 0)
		{
			// Use speed from cfg.
			ent->speed = speed;
		}

		if (ent->speed < 1.0f)
		{
			ent->speed = 1.0f;
		}
	}

	misc_object_spawn(ent, ent->s.origin, ent->s.angles);

	trap_LinkEntity (ent);
}
#endif
