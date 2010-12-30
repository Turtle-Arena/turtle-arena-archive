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
				bg_weapongroupinfo[ent->s.weapon].weaponnum[0], 1, HS_CENTER))
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
/*QUAKED misc_object (1 0 0) (-16 -16 -16) (16 16 16) suspended knockback unsoliddeath invisdeath no_bbox
"model"		arbitrary .md3 file to display
"config"	config filename (defaults to model with .cfg extension)
"health"	if health > 0 object can be damaged and killed. (default 0)
"targetname" Kills object when triggered.
"target"    if inital health > 0, trigger targets on death, else trigger targets on touch.
"paintarget" Called on pain (if health > 0)
"wait"      Time in seconds before respawning (default 0, no respawn)
"message"	Item to drop on death.
"knockback" Whether the object takes knockback (defaults to value from config)
"pushable"	Whether the object is pushable (defaults to value from config)
*/

// object spawn flags
#define MOBJF_SUSPENDED 1
#define MOBJF_NO_BBOX 2


void ObjectPain(gentity_t *self, gentity_t *attacker, int damage)
{
	int health;

	// Default health
	health = self->splashRadius;

	// Change to damge animation at X health
	if (self->health < (health/5) * 2) {
		G_SetMiscAnim(self, OBJECT_DEATH3);
	} else if (self->health < (health/5) * 3) {
		G_SetMiscAnim(self, OBJECT_DEATH2);
	} else if (self->health < (health/5) * 4) {
		G_SetMiscAnim(self, OBJECT_DEATH1);
	}
}

gentity_t *ObjectSpawn(gentity_t *ent, int health, vec3_t origin, vec3_t angles, int flags);

// Based on G_BreakableRespawn
void ObjectRespawn(gentity_t *self)
{
	// Don't let the humans see it respawn
	if (G_SeenByHumans(self))
	{
		// Defer for a max of the total respawn time
		if (self->random < self->wait)
		{
			self->random++;

			// Try again later
			self->nextthink = level.time + 1000;
			self->think = ObjectRespawn;
			return;
		}
	}
	self->random = 0; // clear defer count

	// Kill players so they don't get stuck
	G_KillBox(self);

	// Remove dropped item
	if (self->chain) {
		G_FreeEntity(self->chain);
	}

	// Restore settings
	ObjectSpawn(self, self->splashRadius, self->pos1, self->s.angles2, self->flags);
}

void ObjectRemove(gentity_t *self)
{
	// Don't let the humans see it be removed
	if (G_SeenByHumans(self))
	{
		// Defer for a max of the total delay time
		if (self->random < self->wait)
		{
			self->random++;

			// Try again later
			self->nextthink = level.time + 1000;
			self->think = ObjectRemove;
			return;
		}
	}
	self->random = 0; // clear defer count

	// Good bye!
	G_FreeEntity(self);
}

void ObjectDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	int anim;

	if (!self->takedamage) {
		return;
	}
	self->takedamage = qfalse;

	// Pick one of the 3 dead animations.
	anim = OBJECT_DEAD1+rand()%3;

	// Change to dead animation.
	G_SetMiscAnim(self, anim);

	if (self->objectcfg->unsolidOnDeath && !self->objectcfg->invisibleUnsolidDeath) {
		self->r.contents = 0;
		trap_LinkEntity( self );
	}

	G_UseTargets(self, attacker);

	// Spawn item
	self->chain = NULL;
	if (self->message) {
		vec3_t origin, pos1, pos2;

		// Tequila comment: set breakable center as origin for G_BreakableRespawn needs
		VectorSubtract(self->r.absmax, self->r.absmin, pos1);
		VectorScale(pos1, 0.5f, pos2);
		VectorAdd(pos2, self->r.absmin, origin);

#ifdef TA_WEAPSYS
		if (Q_stricmp(self->message, "weapon_random") == 0) {
			// weapon_random: Change item!
			if (!(self->s.eFlags & EF_VOTED) && !self->item) {
				self->item = G_RandomWeaponItem(self, self->spawnflags>>7);
			}
		} else
#endif
		{
			self->item = BG_FindItemForClassname(self->message);
		}

		if (self->item) {
			self->chain = LaunchItem(self->item, origin, vec3_origin);
		}
	}

	// if respawn
	if (self->wait > 0) {
		// Respawn after X seconds
		self->nextthink = level.time + (self->wait * 1000);
		self->think = ObjectRespawn;
		if (self->objectcfg->invisibleUnsolidDeath) {
			// Invisible non-solid
			self->r.contents = 0;
			trap_UnlinkEntity(self);
		}
	} else if (self->wait < 0) {
		// Remove after X seconds
		self->wait = abs(self->wait);
		self->nextthink = level.time + (self->wait * 1000);
		self->think = ObjectRemove;
	}
}

void ObjectUse(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	ObjectDie(self, NULL, activator, 10000, MOD_UNKNOWN);
}

void ObjectTouchTrigger(gentity_t *self, gentity_t *activator, trace_t *trace)
{
	G_UseTargets(self, activator);
}

gentity_t *ObjectSpawn(gentity_t *ent, int health, vec3_t origin, vec3_t angles, int flags)
{
	trace_t		tr;
	vec3_t		dest;

	VectorCopy( origin, ent->s.origin );
	VectorCopy( origin, ent->s.pos.trBase );
	VectorCopy( origin, ent->r.currentOrigin );

	VectorCopy( angles, ent->s.angles );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	// No not constant random weapon...
	if (!(ent->spawnflags & 8<<7)) {
		// Change weapons on respawn
		ent->s.eFlags |= EF_VOTED;
	}

	ent->s.eType = ET_MISCOBJECT;
	ent->flags = flags;

	// undamaged animation
	G_SetMiscAnim(ent, OBJECT_IDLE);

	if (health > 0) {
		//G_Printf("ObjectSpawn: animated damagable\n");
		ent->r.contents = CONTENTS_BODY;
		ent->health = health;
		ent->takedamage = qtrue;
		ent->die = ObjectDie;
		ent->pain = ObjectPain;
		ent->use = ObjectUse;
	} else if ( ent->target ) {
		//G_Printf("ObjectSpawn: animated touchable\n");
		ent->r.contents = CONTENTS_TRIGGER;
		ent->touch = ObjectTouchTrigger;
	} else {
		//G_Printf("ObjectSpawn: animated scenery\n");
	}

	if ( ent->spawnflags & MOBJF_SUSPENDED ) {
		//G_Printf("ObjectSpawn: no gravity\n");
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		//G_Printf("ObjectSpawn: dropped to ground\n");
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
		} else {
			// allow to ride movers
			ent->s.groundEntityNum = tr.entityNum;
			if (ent->spawnflags & MOBJF_NO_BBOX) {
				// no bbox, so assume mins is ground
				ent->r.mins[2] = ent->s.origin[2] - tr.endpos[2];
				ent->r.mins[0] = ent->r.mins[1] = -1;
				ent->r.maxs[0] = ent->r.maxs[1] = 1;
				G_SetOrigin( ent, ent->s.origin );
			} else {
				G_SetOrigin( ent, tr.endpos );
			}
		}

		ent->s.pos.trType = TR_GRAVITY;
		ent->s.pos.trTime = level.time;
	}

	ent->enemy = NULL;

	trap_LinkEntity( ent );

	return ent;
}

#if 0
// Based on SpawnObelisk
// Use info in owner to spawn object at origin with angles.
//  should allow "clone" spawning of misc_objects and easy respawning
gentity_t *misc_object_clone(gentity_t *owner, int health, vec3_t origin, vec3_t angles)
{
	gentity_t *ent;

	ent = G_Spaw();

	ent->activator = owner;
	VectorCopy(owner->r.mins, ent->r.mins);
	VectorCopy(owner->r.maxs, ent->r.maxs);
	ent->target = owner->target;
	ent->wait = owner->wait;
	ent->message = owner->message;
	ent->spawnflags = owner->spawnflags;
	ent->model = owner->model;
	ent->objectcfg = owner->objectcfg;
	ent->s.modelindex = owner->s.modelindex;

	// Save settings for respawning
	ent->splashRadius = ent->health;
	VectorCopy(ent->s.origin, ent->pos1);
	VectorCopy(ent->s.angles, ent->s.angles2);

	ObjectSpawn(ent, owner->health, owner->origin, owner->angles, owner->flags);
}
#endif

// Based on SP_misc_model and SP_team_redobelisk
void SP_misc_object( gentity_t *ent ) {
	qboolean entHealth;
	qboolean entWait;
	qboolean entSpeed;
	int knockback;
	qboolean entKnockback;
	int pushable;
	qboolean entPushable;
#ifdef IOQ3ZTM // RENDERFLAGS
	int mirrorType;
#endif

	entHealth = G_SpawnInt( "health", "0", &ent->health);
	entWait = G_SpawnFloat( "wait", "0", &ent->wait);
	entSpeed = G_SpawnFloat( "speed", "0", &ent->speed);
	entKnockback = G_SpawnInt( "knockback", "0", &knockback);
	entPushable = G_SpawnInt( "pushable", "0", &pushable);
#ifdef IOQ3ZTM // RENDERFLAGS
	G_SpawnInt("mirrorType", "0", &mirrorType );

	if (mirrorType == 1)
		ent->s.eFlags |= EF_ONLY_MIRROR;
	else if (mirrorType == 2)
		ent->s.eFlags |= EF_NOT_MIRROR;
#endif

	G_SetOrigin( ent, ent->s.origin );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	// Use a animation config file!
	{
		char filename[MAX_QPATH];
		char *config;

		filename[0] = '\0';

		ent->s.modelindex = G_ModelIndex( ent->model );
		trap_GetConfigstring( CS_MODELS + ent->s.modelindex, filename, sizeof(filename));
		Com_SetExt(filename, ".cfg");

		if (G_SpawnString( "config", "", &config) && *config) {
			ent->s.modelindex2 = G_StringIndex( config );
			trap_GetConfigstring( CS_STRINGS + ent->s.modelindex2, filename, sizeof(filename));
			ent->s.modelindex2 = (ent->s.modelindex2*-1)-1; // Tell cgame modelindex2 is string
		}

		if (!(ent->objectcfg = BG_ParseObjectCFGFile(filename))) {
			ent->objectcfg = BG_DefaultObjectCFG();
		}

		VectorCopy(ent->objectcfg->bbmins, ent->r.mins);
		VectorCopy(ent->objectcfg->bbmaxs, ent->r.maxs);

		// The data in this entity over-rides the cfg file.
		if (!entHealth) {
			// Use health from cfg.
			ent->health = ent->objectcfg->health;
		}
		if (!entWait) {
			// Use wait from cfg.
			ent->wait = ent->objectcfg->wait;
		}
		if (!entSpeed) {
			// Use speed from cfg.
			ent->speed = ent->objectcfg->speed;
		}
		if (!entKnockback) {
			// Use knockback from cfg.
			knockback = ent->objectcfg->knockback;
		}
		if (!entPushable) {
			// Use pushable from cfg.
			pushable = ent->objectcfg->pushable;
		}

		if (ent->speed < 1.0f) {
			ent->speed = 1.0f;
		}
	}

	if (ent->spawnflags & MOBJF_NO_BBOX) {
		VectorCopy(vec3_origin, ent->r.mins);
		VectorCopy(vec3_origin, ent->r.maxs);
	}

	ent->flags = 0;
	if (!knockback) {
		ent->flags |= FL_NO_KNOCKBACK;
	}
	if (pushable) {
		ent->flags |= FL_PUSHABLE;
	}

	// Save settings for respawning
	ent->splashRadius = ent->health;
	VectorCopy(ent->s.origin, ent->pos1);
	VectorCopy(ent->s.angles, ent->s.angles2);

	ObjectSpawn(ent, ent->splashRadius, ent->pos1, ent->s.angles2, ent->flags);
}
#endif
