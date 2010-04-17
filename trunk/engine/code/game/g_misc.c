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
#ifdef TURTLEARENA // POWERS
	if (g_teleportFluxTime.integer)
	{
		player->client->ps.powerups[PW_FLASHING] = level.time + g_teleportFluxTime.integer * 1000;
		// Become non-solid
		player->r.contents &= ~CONTENTS_BODY;
	}
#endif

	// set angles
	SetClientViewAngle( player, angles );

	// kill anything at the destination
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
#ifdef TURTLEARENA // POWERS
		if (!player->client->ps.powerups[PW_FLASHING])
#endif
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

#ifdef TURTLEARENA // LOCKON
	if (ent->random == -1)
	{
#ifdef TA_WEAPSYS
		G_AutoAim(ent, bg_weapongroupinfo[ent->s.weapon].weapon[0]->projnum,
				ent->s.origin, dir, right, up);
#else
		G_AutoAim(ent, 0, ent->s.origin, dir, right, up);
#endif
	}
	else
	{
#endif
		// randomize a bit
		PerpendicularVector( up, dir );
		CrossProduct( up, dir, right );

		deg = crandom() * ent->random;
		VectorMA( dir, deg, up, dir );

		deg = crandom() * ent->random;
		VectorMA( dir, deg, right, dir );

		VectorNormalize( dir );
#ifdef TURTLEARENA
	}
#endif

#ifdef TA_WEAPSYS
	if (!fire_weapon(ent, ent->s.origin, dir, right, up,
				bg_weapongroupinfo[ent->s.weapon].weaponnum[0], 1, HAND_NONE))
	{
		return;
	}
#else
	switch ( ent->s.weapon ) {
	case WP_GRENADE_LAUNCHER:
		fire_grenade( ent, ent->s.origin, dir );
		break;
	case WP_ROCKET_LAUNCHER:
		fire_rocket( ent, ent->s.origin, dir );
		break;
	case WP_PLASMAGUN:
		fire_plasma( ent, ent->s.origin, dir );
		break;
	}
#endif

	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
}


static void InitShooter_Finish( gentity_t *ent ) {
	ent->enemy = G_PickTarget( ent->target );
	ent->think = 0;
	ent->nextthink = 0;
}

void InitShooter( gentity_t *ent, int weapon ) {
	ent->use = Use_Shooter;
	ent->s.weapon = weapon;

	RegisterItem( BG_FindItemForWeapon( weapon ) );

	G_SetMovedir( ent->s.angles, ent->movedir );

#ifdef TURTLEARENA // LOCKON
	if (ent->random != -1)
	{
#endif
		if ( !ent->random ) {
			ent->random = 1.0;
		}
		ent->random = sin( M_PI * ent->random / 180 );
#ifdef TURTLEARENA // LOCKON
	}
#endif
	// target might be a moving object, so we can't set movedir for it
	if ( ent->target ) {
		ent->think = InitShooter_Finish;
		ent->nextthink = level.time + 500;
	}
	trap_LinkEntity( ent );
}

#ifdef TA_WEAPSYS
/*QUAKED misc_shooter (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
			if random is -1 uses autoaim to shoot a player.
"message" name of weapon group, used to find projectile (See scripts/weaponinfo.txt)
*/
void SP_misc_shooter( gentity_t *ent ) {
	InitShooter( ent, BG_WeaponGroupIndexForName(ent->message) );
}
#endif

/*QUAKED shooter_rocket (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_rocket( gentity_t *ent ) {
#ifdef TA_WEAPSYS
	InitShooter( ent, BG_WeaponGroupIndexForName("wp_rocket_launcher") );
#else
	InitShooter( ent, WP_ROCKET_LAUNCHER );
#endif
}

/*QUAKED shooter_plasma (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_plasma( gentity_t *ent ) {
#ifdef TA_WEAPSYS
	int weapon;

	weapon = BG_WeaponGroupIndexForName("wp_plasmagun");
#ifdef TURTLEARENA // WEAPONS
	if (!weapon) {
		weapon = BG_WeaponGroupIndexForName("wp_electric_launcher");
	}
#endif

	InitShooter( ent, weapon);
#else
	InitShooter( ent, WP_PLASMAGUN);
#endif
}

/*QUAKED shooter_grenade (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_grenade( gentity_t *ent ) {
#ifdef TA_WEAPSYS
	int weapon;

	weapon = BG_WeaponGroupIndexForName("wp_grenade_launcher");
#ifdef TURTLEARENA // WEAPONS
	if (!weapon) {
		weapon = BG_WeaponGroupIndexForName("wp_homing_launcher");
	}
#endif

	InitShooter( ent, weapon);
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
#ifdef TA_HOLDSYS
	player->client->ps.holdableIndex = HI_PORTAL;
	player->client->ps.holdable[HI_PORTAL]++;
#else
#ifdef IOQ3ZTM
	player->client->ps.stats[STAT_HOLDABLE_ITEM] = ITEM_INDEX( BG_FindItem( "Portal" ) );
#else
	player->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItem( "Portal" ) - bg_itemlist;
#endif
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
#ifdef IOQ3ZTM // Check for teleport delay
	if (self->pain_debounce_time && self->pain_debounce_time > level.time) {
		return;
	}
#endif

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
#ifdef IOQ3ZTM // Delay portaling 1/10th sec for bad dest and source in the same origin
	self->pain_debounce_time = level.time+100;
#endif
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
#ifdef TA_ENTSYS // MISC_OBJECT
// ZTM: TODO: There should also be func_* for brushes that acts mostly the same?

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

// misc_object spawn flags
#define MOBJF_SUSPENDED 1
#define MOBJF_KNOCKBACK 2
#define MOBJF_UNSOLIDDEATH 4

void misc_object_pain(gentity_t *self, gentity_t *attacker, int damage)
{
	//G_Printf("misc_object_pain: damaging...\n");

	// Change to damge animation at X health
	if (self->health < (self->activator->health/5) * 2)
	{
		G_SetMiscAnim(self, OBJECT_DEATH3);
		//G_Printf("    anim = OBJECT_DEATH3\n");
	}
	else if (self->health < (self->activator->health/5) * 3)
	{
		G_SetMiscAnim(self, OBJECT_DEATH2);
		//G_Printf("    anim = OBJECT_DEATH2\n");
	}
	else if (self->health < (self->activator->health/5) * 4)
	{
		G_SetMiscAnim(self, OBJECT_DEATH1);
		//G_Printf("    anim = OBJECT_DEATH1\n");
	}

	// ZTM: TODO: Limit how soon to call paintarget again? Use pain_debounce?
	if ( self->paintarget )
	{
		G_UseTargets2(self, attacker, self->paintarget);
	}
}

gentity_t *misc_object_spawn(gentity_t *owner, vec3_t origin, vec3_t angles);

void misc_object_respawn(gentity_t *self)
{
#if 1
	// Kill players so they don't get stuck
	G_KillBox(self);
#else
	G_Printf("DEBUG: Atempting to respawn...\n");

	// Don't let the humans see it respawn
	if (G_SeenByHumans(self))
	{
		G_Printf("DEBUG: respawing deffered...\n");

		// Try again later
		self->nextthink = level.time + 1000;
		self->think = misc_object_respawn;
		return;
	}
#endif
	
	//G_Printf("misc_object_respawn: respawning...\n");
	if (self->activator->spawnflags & MOBJF_KNOCKBACK)
	{
		// ZTM: FIXME: Not right if "self" moved from origin.

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
	int anim;

	if (!self->takedamage)
		return;
	self->takedamage = qfalse;

	// Pick one of the 3 dead animations.
	anim = OBJECT_DEAD1+rand()%3;

	// Change to dead animation.
	if (!(self->s.modelindex2 & ANIM_TOGGLEBIT))
		self->s.modelindex2 = (anim|ANIM_TOGGLEBIT);
	else
		self->s.modelindex2 = anim;

	//G_Printf("misc_object_die: killed object.\n");

	if (self->activator->spawnflags & MOBJF_UNSOLIDDEATH)
	{
		self->r.contents = 0;
		trap_LinkEntity( self );
		//G_Printf("    unsolid misc_object\n");
	}

	// if respawn
	if (self->wait > 0)
	{
		// Respawn after X seconds
		self->nextthink = level.time + (self->wait * 1000);
		self->think = misc_object_respawn;
	}

	if( self->target )
	{
		G_UseTargets(self, attacker);
	}
}

#if 0
void misc_object_think(gentity_t *self)
{
	self->nextthink = level.time + 1000;

	// Do once per-second thinking code.
	// Such as animation (but that should be in cgame...)
}
#endif

void Use_MiscObject(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	misc_object_die(self, NULL, activator, 10000, MOD_UNKNOWN);
}

void misc_object_touch(gentity_t *self, gentity_t *activator, trace_t *trace)
{
	//G_Printf("misc_object_touch: touched\n");
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
	ent->wait = owner->wait;

	ent->s.eType = ET_MISCOBJECT;
	if (!(owner->spawnflags & MOBJF_KNOCKBACK)) {
		ent->flags = FL_NO_KNOCKBACK;
	}

	// Setup stuff.
	ent->s.modelindex = G_ModelIndex( owner->model );

	// undamaged animation
	if (!(ent->s.modelindex2 & ANIM_TOGGLEBIT))
		ent->s.modelindex2 = (OBJECT_IDLE|ANIM_TOGGLEBIT);
	else
		ent->s.modelindex2 = OBJECT_IDLE;

	if (owner->health > 0) {
		//G_Printf("misc_object_spawn: animated damagable\n");
		ent->r.contents = CONTENTS_BODY;
		ent->health = owner->health;
		ent->takedamage = qtrue;
		ent->die = misc_object_die;
		ent->pain = misc_object_pain;
		ent->use = Use_MiscObject;
		//ent->think = misc_object_think;
		//ent->nextthink = level.time + 1000;
	} else if ( ent->target ) {
		//G_Printf("misc_object_spawn: animated touchable\n");
		ent->r.contents = CONTENTS_TRIGGER;
		ent->touch = misc_object_touch;
	} else {
		//G_Printf("misc_object_spawn: animated scenery\n");
	}

	if ( owner->spawnflags & MOBJF_SUSPENDED ) {
		//G_Printf("misc_object_spawn: no gravity\n");
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		//G_Printf("misc_object_spawn: dropped to ground\n");
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

		// ZTM: TODO: Have gravity/pushable spawnflag?
		//ent->s.pos.trType = TR_GRAVITY;
		//ent->s.pos.trTime = level.time;
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
	qboolean entHealth;
	qboolean entWait;
	qboolean entSpeed;
	qboolean entMins, entMaxs;
#ifdef IOQ3ZTM // RENDERFLAGS
	int mirrorType;
#endif

	entHealth = G_SpawnInt( "health", "0", &ent->health);
	entWait = G_SpawnFloat( "wait", "0", &ent->wait);
	entSpeed = G_SpawnFloat( "speed", "0", &ent->speed);
#ifdef IOQ3ZTM // RENDERFLAGS
	G_SpawnInt("mirrorType", "0", &mirrorType );

	if (mirrorType == 1)
		ent->s.eFlags |= EF_ONLY_MIRROR;
	else if (mirrorType == 2)
		ent->s.eFlags |= EF_NOT_MIRROR;
#endif

#if 1
	entMins = entMaxs = qfalse;
#else
	entMins = G_SpawnVector( "mins", "-16 -16 0", ent->r.mins );
	entMaxs = G_SpawnVector( "maxs", "16 16 32", ent->r.maxs );
#endif

	G_SetOrigin( ent, ent->s.origin );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	// Use a animation config file!
	{
		char filename[MAX_QPATH];
		bg_objectcfg_t objectcfg; // ZTM: TODO: Move to gentity_s ?

		Com_Memset(&objectcfg, 0, sizeof (objectcfg));

		// !! It took forever to find why bounding box wasn't loaded, there was
		//    no config filename !!
		trap_GetConfigstring( CS_MODELS + G_ModelIndex( ent->model ), filename, sizeof(filename));

		if (filename[0] == '\0')
		{
			G_Printf("DEBUG: Missing filename for misc_object model!\n");
		}
		else
		{
			G_SetFileExt(filename, ".cfg");
		}

		BG_ParseObjectCFGFile(filename, &objectcfg);

		// The data in this entity over-rides the cfg file.
		if (!entHealth)
		{
			// Use health from cfg.
			ent->health = objectcfg.health;
		}
		if (!entWait)
		{
			// Use wait from cfg.
			ent->wait = objectcfg.wait;
		}
		if (!entSpeed)
		{
			// Use speed from cfg.
			ent->speed = objectcfg.speed;
		}
		if (!entMins)
		{
			VectorCopy(objectcfg.bbmins, ent->r.mins);
		}
		if (!entMaxs)
		{
			VectorCopy(objectcfg.bbmaxs, ent->r.maxs);
		}

		if (ent->speed < 1.0f)
		{
			ent->speed = 1.0f;
		}
	}

	//trap_LinkEntity (ent);

	misc_object_spawn(ent, ent->s.origin, ent->s.angles);
}
#endif
