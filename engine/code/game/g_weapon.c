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
// g_weapon.c 
// perform the server side effects of a weapon firing

#include "g_local.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

#ifndef TMNTWEAPSYS_2
#define NUM_NAILSHOTS 15
#endif

/*
================
G_BounceProjectile
================
*/
void G_BounceProjectile( vec3_t start, vec3_t impact, vec3_t dir, vec3_t endout ) {
	vec3_t v, newv;
	float dot;

	VectorSubtract( impact, start, v );
	dot = DotProduct( v, dir );
	VectorMA( v, -2*dot, dir, newv );

	VectorNormalize(newv);
	VectorMA(impact, 8192, newv, endout);
}


#ifdef TMNT // if THIRD_PERSON?
/*
================
G_AutoAim

...Because in third person it is hard to aim.
================
*/
void G_AutoAim(gentity_t *ent, int projnum, vec3_t start, vec3_t forward, vec3_t right, vec3_t up)
{
	gentity_t *target;
	vec3_t targetOrigin;
	vec3_t dir;
	int i;
	float maxRand;
	float range;
	float angle;

#ifdef TMNTWEAPSYS_2
	if (bg_projectileinfo[projnum].grappling) {
		// Don't auto aim grapple.
		//   Maybe at target_grapple entities to autoaim grapple at?...
		return;
	}
#endif

	// Currently no way to know if in first person.
	// if (ent->client && ent->client is in firstPerson && !ent->bgNPC.info)
	//   return;

	angle = 30.0f;
	range = 2000.0f;
#ifdef TMNTWEAPSYS_2
	if (bg_projectileinfo[projnum].instantDamage) {
		range = bg_projectileinfo[projnum].speed;
	}
#endif

	// Turtle Man: TODO: If locked on to a entity (Like in LoZ:TP), or is a NPC
	target = ent->enemy;
	if (target && (target == ent || !target->takedamage))
	{
		target = NULL;
	}

	if (!target || !G_ValidTarget(ent, target, start, forward, range, angle, 2)) // && !NPC?
	{
		// Search for a target
		target = G_FindTarget(ent, start, forward, range, angle);
		// ent->enemy = target; // Turtle Man: Update target?
	}

	if (!target) {
		return;
	}

	//G_Printf("DEBUG: Targeting %d: %s...\n", target-g_entities, target->classname);
	VectorCopy(target->r.currentOrigin, targetOrigin);

	// Aim higher then origin
	for (i = 0; i < 3; i++)
	{
		targetOrigin[i] += (target->r.mins[i] + target->r.maxs[i]) * 0.5;
		// Move around a bit so that shurikens don't all stick in the same spot.
		maxRand = target->r.maxs[i] * 0.5;
		targetOrigin[i] += maxRand - (random() * (maxRand * 2));
	}

	// see if we have a target
	VectorSubtract( targetOrigin, start, dir );
	VectorNormalize( dir );

	// Get up and right from dir (which is "forward")
	VectorCopy( dir, forward );
	PerpendicularVector( up, dir );
	CrossProduct( up, dir, right );
}
#endif

#ifdef TMNTHOLDABLE
void G_ThrowShuriken(gentity_t *ent, holdable_t holdable)
{
#ifndef TMNTWEAPSYS_2
	gentity_t	*m;
#endif

	// Turtle Man: TODO: Player animation (Throw shuriken)?

	switch (holdable)
	{
		case HI_SHURIKEN:
		case HI_ELECTRICSHURIKEN:
		case HI_FIRESHURIKEN:
		case HI_LASERSHURIKEN:
			// Turtle Man: TODO: Throw from origin of tag_hand_* (primary or secondary?)

			// set aiming directions
			AngleVectors (ent->client->ps.viewangles, forward, right, up);

			CalcMuzzlePoint ( ent, forward, right, up, muzzle );
			//CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );

#ifdef TMNTWEAPSYS_2
			fire_shuriken (ent, muzzle, forward, right, up, holdable);
#else
			m = fire_shuriken (ent, muzzle, forward, right, up, holdable);
			m->damage *= s_quadFactor;
			m->splashDamage *= s_quadFactor;

//			VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
#endif
			break;
		default:
			G_Error("Unknown shuriken type (holdable=%d)\n", holdable);
			return;
	}
}
#endif

#ifdef TMNTWEAPSYS // MELEEATTACK
#define MELEE_CHAINTIME 1500
void G_StartMeleeAttack(gentity_t *ent)
{
	gclient_t *client = ent->client;
	weapontype_t wt = BG_WeaponTypeForPlayerState(&ent->client->ps);

	// Make sure it is a melee weapon.
	if ( !BG_WeapTypeIsMelee( wt ) ) {
		return;
	}

	// Must press the button each time to attack...
	if (client->fireHeld) {
		return;
	}
	client->fireHeld = qtrue;

	// Turtle Man: Use the animation time for the attack time!
	client->ps.meleeTime = BG_AnimationTime(&ent->client->pers.playercfg.animations[BG_TorsoAttackForPlayerState(&ent->client->ps)]);
	client->ps.meleeLinkTime = 3.75f * client->ps.meleeTime; // MELEE_CHAINTIME

	client->ps.meleeAttack++;

	//G_Printf("DEBUG: client %i started new melee attack (%i)\n", ent - g_entities, client->ps.meleeAttack);

	// Play sound and stuff.
	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
}

#ifdef TMNT_GAME_MODELS
/*
======================
G_PositionEntityOnTag

Modifies the child's position and axis by the given
tag location
======================
*/
qboolean G_PositionEntityOnTag( orientation_t *child, lerpFrame_t *parentLF,
	orientation_t parent, qhandle_t parentTags, char *tagName )
{
	int				i;
	orientation_t	lerped;
	qboolean rtn = qtrue;

	// lerp the tag
	if (!trap_LerpTag( &lerped, parentTags, parentLF->oldFrame, parentLF->frame,
		1.0 - parentLF->backlerp, tagName ))
	{
		rtn = qfalse;
	}

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent.origin, child->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( child->origin, lerped.origin[i], parent.axis[i], child->origin );
	}

	MatrixMultiply( lerped.axis, parent.axis, child->axis );

	return rtn;
}


/*
======================
G_PositionRotatedEntityOnTag

Modifies the child's position and axis by the given
tag location
======================
*/
qboolean G_PositionRotatedEntityOnTag( orientation_t *child, lerpFrame_t *parentLF,
	orientation_t parent, qhandle_t parentTags, char *tagName )
{
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];
	qboolean rtn = qtrue;

//AxisClear( entity->axis );
	// lerp the tag
	if (!trap_LerpTag( &lerped, parentTags, parentLF->oldFrame, parentLF->frame,
		1.0 - parentLF->backlerp, tagName ))
	{
		rtn = qfalse;
	}

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent.origin, child->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( child->origin, lerped.origin[i], parent.axis[i], child->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( child->axis, lerped.axis, tempAxis );
	MatrixMultiply( tempAxis, parent.axis, child->axis );

	return rtn;
}

/*
======================
G_SetupPlayerTagOrientation

Sets up the orientation to use G_PositionEntityOnTag and G_PositionRotatedEntityOnTag.
======================
*/
qboolean G_SetupPlayerTagOrientation(gentity_t *ent, orientation_t *legsOrientation, orientation_t *torsoOrientation, orientation_t *tagOrientation)
{
#if 0
	// Debug messages (Should only print once. Server is spammed...)
	if (!ent->client->pers.legsModel && !ent->client->pers.torsoModel)
	{
		G_Printf("DEBUG: G_GetPlayerTagOrientation: Invalid tags for legs and torso model.\n");
		return qfalse;
	}
	if (!ent->client->pers.legsModel)
	{
		G_Printf("DEBUG: G_GetPlayerTagOrientation: Invalid tags for legs model.\n");
		return qfalse;
	}
	if (!ent->client->pers.torsoModel)
	{
		G_Printf("DEBUG: G_GetPlayerTagOrientation: Invalid tags for torso model.\n");
		return qfalse;
	}
#endif

	if (tagOrientation != NULL)
	{
		// Clear memory
		memset(tagOrientation, 0, sizeof (*tagOrientation));
	}

	if (legsOrientation != NULL)
	{
	// Clear memory
		memset(legsOrientation, 0, sizeof (*legsOrientation));

	// Set to player's origin
		VectorCopy(ent->r.currentOrigin, legsOrientation->origin);

	// Use pre-calculated legsAxis
		AxisCopy(ent->client->pers.legsAxis, legsOrientation->axis);
	}

	if (torsoOrientation != NULL)
	{
		// Clear memory
		memset(torsoOrientation, 0, sizeof (*torsoOrientation));

	// Use pre-calculated torsoAxis
		AxisCopy(ent->client->pers.torsoAxis, torsoOrientation->axis);

		if (legsOrientation != NULL)
		{
	// Find torso origin
			if (!G_PositionRotatedEntityOnTag(torsoOrientation, &ent->client->pers.legs,
				*legsOrientation, ent->client->pers.legsModel, "tag_torso"))
			{
				return qfalse;
			}
		}
	}

	return qtrue;
}

/*
======================
G_GetPlayerTagOrientation

This function makes it easy to find a tag on the torso or legs
If onLegs find tag on legs, else torso (maybe later find tags on head?)
======================
*/
qboolean G_GetPlayerTagOrientation(gentity_t *ent, char *tagName, qboolean onLegs, orientation_t *tagOrientation)
{
	orientation_t	legsOrientation;
	orientation_t	torsoOrientation;

	if (!onLegs) // torso
	{
		if (!G_SetupPlayerTagOrientation(ent, &legsOrientation, &torsoOrientation, tagOrientation))
		{
			return qfalse;
		}
	}
	else
	{
		if (!G_SetupPlayerTagOrientation(ent, &legsOrientation, NULL, tagOrientation))
		{
			return qfalse;
		}
	}

	if (tagOrientation == NULL)
	{
		return qfalse;
	}

	if (!onLegs) // torso
	{
		if (!G_PositionEntityOnTag(tagOrientation, &ent->client->pers.torso,
					torsoOrientation, ent->client->pers.torsoModel, tagName))
		{
			return qfalse;
		}
	}
	else
	{
		// Find tag on legs
		if (!G_PositionEntityOnTag(tagOrientation, &ent->client->pers.legs,
			legsOrientation, ent->client->pers.legsModel, tagName))
		{
			return qfalse;
		}
	}

	return qtrue;
}
#endif

// Based on CheckGauntletHit
qboolean G_MeleeDamageSingle(gentity_t *ent, qboolean dodamage, int hand, weapontype_t wt, qboolean checkTeamHit)
{
	trace_t		tr;
	vec3_t		start;
	vec3_t		end;
	vec3_t		pushDir; // dir for knockback
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;
	weapon_t weaponnum;
	int mod; // Means of death
#ifdef TMNTWEAPSYS_2
	bg_weaponinfo_t *weapon;
#else
	int start_range;
	int end_range;
#endif
	int i;
	qboolean traceHit;
	orientation_t	weaponOrientation;
#ifdef TMNT_GAME_MODELS // Use the weapon tag angles and pos!
	orientation_t	legsOrientation;
	orientation_t	torsoOrientation;

	// Setup the orientations
	if (!G_SetupPlayerTagOrientation(ent, &legsOrientation, &torsoOrientation, &weaponOrientation))
	{
		return qfalse;
	}
#endif

	weaponnum = ent->client->ps.weapon;

	// Use hand to select the weapon tag.
	if (hand == HAND_PRIMARY)
	{
		if (ent->client->melee_debounce &&
			ent->client->melee_debounce > level.time)
		{
			return qfalse;
		}
#ifdef TMNT_GAME_MODELS // TMNTWEAPSYS
		// put weapon on torso
#ifdef TMNTPLAYERS
		if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
			torsoOrientation, ent->client->pers.torsoModel, "tag_hand_primary"))
#endif
		{
#if !defined TMNTPLAYERS || defined TMNT_SUPPORTQ3
			if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
				torsoOrientation, ent->client->pers.torsoModel, "tag_weapon"))
#endif
			{
				// Failed to put weapon on torso!
				// Turtle Man: TODO: This should be moved to player tag loading or just removed.
				//G_Printf("DEBUG: G_DoMeleeDamage: Player missing primary weapon tag!\n");
				return qfalse;
			}
		}
#else
		// set aiming directions
		AngleVectors (ent->client->ps.viewangles, forward, right, up);

		CalcMuzzlePoint ( ent, forward, right, up, muzzle );
#endif
#ifndef TMNTWEAPSYS_2
		start_range = bg_weaponinfo[weaponnum].start_range;
		end_range = bg_weaponinfo[weaponnum].end_range;
#endif
	}
	else
	{
		if (ent->client->melee_debounce2 &&
			ent->client->melee_debounce2 > level.time)
		{
			return qfalse;
		}
#ifdef TMNT_GAME_MODELS // TMNTWEAPSYS
		// put weapon on torso
#ifdef TMNTPLAYERS
		if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
			torsoOrientation, ent->client->pers.torsoModel, "tag_hand_secondary"))
#endif
		{
#if !defined TMNTPLAYERS || defined TMNT_SUPPORTQ3
			if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
				torsoOrientation, ent->client->pers.torsoModel, "tag_flag"))
#endif
			{
				// Failed to put weapon on torso!
				// Turtle Man: NOTE: Disable message so quake3 players
				//                   don't give lots of errors.
				//                   tag_flag is only used by Team Arena players.
				//G_Printf("DEBUG: G_DoMeleeDamage: Player missing secondary weapon tag!\n");
				return qfalse;
			}
		}
#else
		// Turtle Man: Secondary weapon currently does the damage.
		return qfalse;
#endif
#ifndef TMNTWEAPSYS_2
		start_range = bg_weaponinfo[weaponnum].start_range2;
		end_range = bg_weaponinfo[weaponnum].end_range2;
#endif
	}

#ifdef TMNT_GAME_MODELS // TMNTWEAPSYS
	// Setup "start" and "end" using weaponOrientation
	VectorCopy(weaponOrientation.axis[0], forward);
	VectorCopy(weaponOrientation.axis[1], right);
	VectorCopy(weaponOrientation.axis[2], up);
	VectorCopy(weaponOrientation.origin, muzzle);
#else
	// Setup weaponOrientation, so that the old code works with new code.
	VectorCopy(forward, weaponOrientation.axis[0]);
	VectorCopy(right, weaponOrientation.axis[1]);
	VectorCopy(up, weaponOrientation.axis[2]);
	VectorCopy(muzzle, weaponOrientation.origin);
#endif

	// Using "up" makes players fly into the air when hit.
	//VectorCopy(up, pushDir);
	VectorCopy(forward, pushDir);

#ifndef TMNTWEAPSYS_2
	// Gauntlet/Guns use forward, Melee use up
	// Guns shouldn't be here currently, but having a button to
	//   use guns as melee to hit people would be nice...
	if (wt == WT_GUN || wt == WT_GAUNTLET)
	{
		VectorMA (weaponOrientation.origin, start_range, forward, start);
		VectorMA (weaponOrientation.origin, end_range, forward, end);
	}
	else
	{
		VectorMA (weaponOrientation.origin, start_range, up, start);
		VectorMA (weaponOrientation.origin, end_range, up, end);
	}

	// DEBUG: Draw model at tag locations
	//  so I can see where they are.
	if ((level.time & 10) && qtrue) // Turtle Man: TODO: Cvar for this?
	{
		gentity_t *t;
#if 0 // Don't need to see legs and torso origin any more, and caused too many temp entities
		vec3_t origin;

		// Copy origin
		VectorCopy(legsOrientation.origin, origin);

		// Need to offset the bounding box to get the base of the legs model
		origin[2] += ent->client->pers.playercfg.bbmins[2];

		t = G_TempEntity( origin, EV_DEBUG_ORIGIN );
		t->s.eventParm = 0; // red
		//t->s.otherEntityNum = ent->s.number; // client num
		t->r.svFlags = SVF_BROADCAST;	// send to everyone

		t = G_TempEntity( torsoOrientation.origin, EV_DEBUG_ORIGIN );
		t->s.eventParm = 1; // blue
		//t->s.otherEntityNum = ent->s.number; // client num
		t->r.svFlags = SVF_BROADCAST;	// send to everyone
#endif

		// Always draw a model at the weapon tag.
		if (start_range != 0
			&& end_range != 0)
		{
			t = G_TempEntity( weaponOrientation.origin, EV_DEBUG_ORIGIN );
			t->s.eventParm = 3; // grey
			//t->s.otherEntityNum = ent->s.number; // client num
			t->r.svFlags = SVF_BROADCAST;	// send to everyone
		}
	}

	// DEBUG: Draw model at tag locations
	//  so I can see where they are.
	if ((level.time & 10) && qtrue) // Turtle Man: TODO: Cvar for this?
	{
		gentity_t *t;

		t = G_TempEntity( start, EV_DEBUG_ORIGIN );
		t->s.eventParm = 2; // yellow
		//t->s.otherEntityNum = ent->s.number; // client num
		t->r.svFlags = SVF_BROADCAST;	// send to everyone

		t = G_TempEntity( end, EV_DEBUG_ORIGIN );
		t->s.eventParm = 0; // red
		//t->s.otherEntityNum = ent->s.number; // client num
		t->r.svFlags = SVF_BROADCAST;	// send to everyone
	}
#endif // TMNTWEAPSYS_2

	if (!dodamage && !checkTeamHit)
	{
		return qtrue;
	}

	traceHit = qfalse;
#ifdef TMNTWEAPSYS_2
	if (hand == HAND_PRIMARY)
	{
		weapon = bg_weapongroupinfo[weaponnum].weapon[0];
		mod = weapon->mod;
		// Use default kill message
		if (mod == MOD_UNKNOWN) {
			mod = MOD_WEAPON_PRIMARY;
		}
	}
	else
	{
		weapon = bg_weapongroupinfo[weaponnum].weapon[1];
		mod = weapon->mod;
		// Use default kill message
		if (mod == MOD_UNKNOWN) {
			mod = MOD_WEAPON_SECONDARY;
		}
	}
#else
	if (hand == HAND_SECONDARY) {
		mod = bg_weaponinfo[weaponnum].mod2;
	} else {
		mod = bg_weaponinfo[weaponnum].mod;
	}
#endif

#ifdef TMNTWEAPSYS_2
	for (i = 0; i < MAX_WEAPON_BLADES; i++)
#else
	// Bo Staff does damage at both ends.
	for (i = 0; i < 2; i++)
#endif
	{
#ifdef TMNTWEAPSYS_2
		if (weapon->blades[i].damage == 0)
			continue;

		// Move forward, right, up
		VectorMA (weaponOrientation.origin, weapon->blades[i].origin[2], up, start);
		VectorMA (weaponOrientation.origin, weapon->blades[i].tip[2], up, end);

		VectorMA (start, weapon->blades[i].origin[1], right, start);
		VectorMA (end, weapon->blades[i].tip[1], right, end);

		VectorMA (start, weapon->blades[i].origin[0], forward, start);
		VectorMA (end, weapon->blades[i].tip[0], forward, end);

		trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
#else
		if (i == 0)
		{
			// Bo Staff (trace from tag to start, it is going "down")
			if (start_range < 0 && end_range > 0)
			{
				trap_Trace (&tr, weaponOrientation.origin, NULL, NULL, start, ent->s.number, MASK_SHOT);
			}
			else
			{
#if 1
				trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
#else // When using mins and maxs weapon doesn't always hit the wall?...
	trap_Trace (&tr, start, bg_weaponinfo[weaponnum].mins,
			bg_weaponinfo[weaponnum].maxs, end, ent->s.number, MASK_SHOT);
#endif
			}
		}
		else
		{
			// Bo Staff (trace from tag to end, it is going "up")
			if (start_range < 0 && end_range > 0)
			{
				trap_Trace (&tr, weaponOrientation.origin, NULL, NULL, end, ent->s.number, MASK_SHOT);
			}
			else
			{
				break;
			}
		}
#endif

	if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			continue;
	}


		if (ent->client->ps.powerups[PW_QUAD] ) {
			s_quadFactor = g_quadfactor.value;
		} else {
			s_quadFactor = 1;
		}
#ifdef MISSIONPACK // MP_TMNT_OK
		if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
			s_quadFactor *= 2;
		}
#endif

#ifdef TMNTWEAPSYS_2
		damage = weapon->blades[i].damage;
#else
		if (hand == HAND_SECONDARY) {
			damage = bg_weaponinfo[weaponnum].damage2;
		} else {
			damage = bg_weaponinfo[weaponnum].damage;
		}
#endif

		damage *= s_quadFactor;

		if ( tr.startsolid || (tr.contents & CONTENTS_SOLID) ) {
			// Turtle Man: TODO: Push player away from trace dir!
			// Copied from G_Damage
			vec3_t	kvel;
			float	mass;
			int knockback;

			// ...not from G_Damage
			ent->client->ps.meleeTime /= 2;
			if (ent->client->ps.meleeTime < 1)
				ent->client->ps.meleeTime = 1;
			knockback = 5+damage*1.5f;

			mass = 200;

			VectorScale (tr.plane.normal, g_knockback.value * (float)knockback / mass, kvel);
			VectorAdd (ent->client->ps.velocity, kvel, ent->client->ps.velocity);

			// set the timer so that the other client can't cancel
			// out the movement immediately
			if ( !ent->client->ps.pm_time ) {
				int		t;

				t = knockback * 2;
				if ( t < 50 ) {
					t = 50;
				}
				if ( t > 200 ) {
					t = 200;
				}

				// ...not from G_Damage
				ent->client->ps.meleeDelay = t;

				ent->client->ps.pm_time = t;
				ent->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
			}
		}

	traceEnt = &g_entities[ tr.entityNum ];

		if (checkTeamHit)
		{
			if (traceEnt->takedamage && OnSameTeam(ent, traceEnt))
			{
				return qtrue;
			}
			continue;
		}

		// If client hit another client
		if (!traceEnt->client || !OnSameTeam(ent, traceEnt) || g_friendlyFire.integer != 0)
	{
		// pain_debounce?

			// Turtle Man: NOTE: Disabled, I don't need to see this as much as I once did.
			//                     (plus it fills the console.)
			//G_Printf("DEBUG: client %i hit client %i\n", ent - g_entities, tr.entityNum);

		// Turtle Man: Do a effect when hit anything!
		//   based on G_MissileImpact code
		if ( traceEnt->takedamage && traceEnt->client ) {
				tent = G_TempEntity( tr.endpos, EV_WEAPON_HIT );
		tent->s.otherEntityNum = traceEnt->s.number;
		} else if( tr.surfaceFlags & SURF_METALSTEPS ) {
				tent = G_TempEntity( tr.endpos, EV_WEAPON_MISS_METAL );
		}
			// Turtle Man: Don't show melee effect when hitting the air...
		else if (tr.fraction != 1.0) {
				tent = G_TempEntity( tr.endpos, EV_WEAPON_MISS );
		}
		else {
			// hit nothing.
			tent = NULL;
		}

		if (tent)
		{
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
				tent->s.clientNum = ent->s.number;
			}
	}

		if (!traceEnt->takedamage) {
			continue;
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
			// First time only
			if (!traceHit) {
		G_AddEvent( ent, EV_POWERUP_QUAD, 0 );
			}
	}

		G_Damage( traceEnt, ent, ent, pushDir, tr.endpos,
		damage, 0, mod );

		traceHit = qtrue;
	}
	if (checkTeamHit)
	{
		return qfalse;
	}
	if (traceHit)
	{
		// Extend combo,
		ent->client->ps.chain++;
		ent->client->ps.chainTime = MELEE_CHAINTIME;

		if (hand == HAND_PRIMARY)
			ent->client->melee_debounce = level.time + 200;
		else
			ent->client->melee_debounce2 = level.time + 200;
	}
	return traceHit;
}

qboolean G_MeleeDamage(gentity_t *ent, qboolean forceDamage)
{
	qboolean rtn, rtn2;
	weapontype_t wt;
	qboolean damage;

	wt = BG_WeaponTypeForPlayerState(&ent->client->ps);

	// Make sure it is a melee weapon.
	if ( !BG_WeapTypeIsMelee( wt ) ) {
		return qfalse;
	}

	rtn = rtn2 = qfalse;

	if (ent->client->ps.weaponHands & HAND_PRIMARY)
	{
		damage = forceDamage;
		if (!damage) {
			damage = (bg_weapongroupinfo[ent->client->ps.weapon].weapon[0]->flags & WIF_ALWAYS_DAMAGE);
		}
		rtn = G_MeleeDamageSingle(ent, damage, HAND_PRIMARY, wt, qfalse);
	}
	if (ent->client->ps.weaponHands & HAND_SECONDARY)
	{
		damage = forceDamage;
		if (!damage) {
			damage = (bg_weapongroupinfo[ent->client->ps.weapon].weapon[1]->flags & WIF_ALWAYS_DAMAGE);
		}
		rtn2 = G_MeleeDamageSingle(ent, damage, HAND_SECONDARY, wt, qfalse);
	}

	return (rtn || rtn2);
}
#endif // TMNTWEAPSYS
#ifndef TMNTWEAPSYS_2
/*
======================================================================

GAUNTLET

======================================================================
*/

void Weapon_Gauntlet( gentity_t *ent ) {

}
#endif

#ifndef TMNTWEAPSYS
/*
===============
CheckGauntletAttack
===============
*/
qboolean CheckGauntletAttack( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );

	VectorMA (muzzle, 32, forward, end);

	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	if ( tr.surfaceFlags & SURF_NOIMPACT ) {
		return qfalse;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	// send blood impact
	if ( traceEnt->takedamage && traceEnt->client ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	}

	if ( !traceEnt->takedamage) {
		return qfalse;
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
		G_AddEvent( ent, EV_POWERUP_QUAD, 0 );
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
#ifdef MISSIONPACK
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2;
	}
#endif

	damage = 50 * s_quadFactor;
	G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		damage, 0, MOD_GAUNTLET );

	return qtrue;
}
#endif // TMNTWEAPSYS


/*
======================================================================

MACHINEGUN

======================================================================
*/

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}

#ifndef TMNTWEAPSYS_2 // Turtle Man: I replaced all of these, see fire_weapon
#ifdef MISSIONPACK
#define CHAINGUN_SPREAD		600
#endif
#define MACHINEGUN_SPREAD	200
#define	MACHINEGUN_DAMAGE	7
#define	MACHINEGUN_TEAM_DAMAGE	5		// wimpier MG in teamplay

void Bullet_Fire (gentity_t *ent, float spread, int damage ) {
	trace_t		tr;
	vec3_t		end;
#if defined MISSIONPACK && !defined TMNT // POWERS
	vec3_t		impactpoint, bouncedir;
#endif
	float		r;
	float		u;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			i, passent;

	damage *= s_quadFactor;

	r = random() * M_PI * 2.0f;
	u = sin(r) * crandom() * spread * 16;
	r = cos(r) * crandom() * spread * 16;
	VectorMA (muzzle, 8192*16, forward, end);
	VectorMA (end, r, right, end);
	VectorMA (end, u, up, end);

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {

		trap_Trace (&tr, muzzle, NULL, NULL, end, passent, MASK_SHOT);
		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		// snap the endpos to integers, but nudged towards the line
		SnapVectorTowards( tr.endpos, muzzle );

		// send bullet impact
#ifdef TMNTNPCSYS
		if ( traceEnt->takedamage && (traceEnt->client || traceEnt->s.eType == ET_NPC))
#else
		if ( traceEnt->takedamage && traceEnt->client )
#endif
		{
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_FLESH );
			tent->s.eventParm = traceEnt->s.number;
			if( LogAccuracyHit( traceEnt, ent ) ) {
				ent->client->accuracy_hits++;
			}
		} else {
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_WALL );
			tent->s.eventParm = DirToByte( tr.plane.normal );
		}
		tent->s.otherEntityNum = ent->s.number;

		if ( traceEnt->takedamage) {
#if defined MISSIONPACK && !defined TMNT // POWERS
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					VectorCopy( impactpoint, muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, muzzle );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
#endif
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0,
#ifdef TMNTWEAPONS // MOD
					MOD_GUN);
#else
					MOD_MACHINEGUN);
#endif
#if defined MISSIONPACK && !defined TMNT // POWERS
			}
#endif
		}
		break;
	}
}

/*
======================================================================

BFG

======================================================================
*/

void BFG_Fire ( gentity_t *ent ) {
	gentity_t	*m;

	m = fire_bfg (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}


/*
======================================================================

SHOTGUN

======================================================================
*/

// DEFAULT_SHOTGUN_SPREAD and DEFAULT_SHOTGUN_COUNT	are in bg_misc.h, because
// client predicts same spreads
#define	DEFAULT_SHOTGUN_DAMAGE	10

qboolean ShotgunPellet( vec3_t start, vec3_t end, gentity_t *ent ) {
	trace_t		tr;
	int			damage, i, passent;
	gentity_t	*traceEnt;
#if defined MISSIONPACK && !defined TMNT // POWERS
	vec3_t		impactpoint, bouncedir;
#endif
	vec3_t		tr_start, tr_end;

	passent = ent->s.number;
	VectorCopy( start, tr_start );
	VectorCopy( end, tr_end );
	for (i = 0; i < 10; i++) {
		trap_Trace (&tr, tr_start, NULL, NULL, tr_end, passent, MASK_SHOT);
		traceEnt = &g_entities[ tr.entityNum ];

		// send bullet impact
		if (  tr.surfaceFlags & SURF_NOIMPACT ) {
			return qfalse;
		}

		if ( traceEnt->takedamage) {
			damage = DEFAULT_SHOTGUN_DAMAGE * s_quadFactor;
#if defined MISSIONPACK && !defined TMNT // POWERS
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( tr_start, impactpoint, bouncedir, tr_end );
					VectorCopy( impactpoint, tr_start );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, tr_start );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0, MOD_SHOTGUN);
				if( LogAccuracyHit( traceEnt, ent ) ) {
					return qtrue;
				}
			}
#else
			G_Damage( traceEnt, ent, ent, forward, tr.endpos,	damage, 0, MOD_SHOTGUN);
				if( LogAccuracyHit( traceEnt, ent ) ) {
					return qtrue;
				}
#endif
		}
		return qfalse;
	}
	return qfalse;
}

// this should match CG_ShotgunPattern
void ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, gentity_t *ent ) {
	int			i;
	float		r, u;
	vec3_t		end;
	vec3_t		forward, right, up;
#ifndef IOQ3ZTM // unused
	int			oldScore;
#endif
	qboolean	hitClient = qfalse;

	// derive the right and up vectors from the forward vector, because
	// the client won't have any other information
	VectorNormalize2( origin2, forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

#ifndef IOQ3ZTM // unused
	oldScore = ent->client->ps.persistant[PERS_SCORE];
#endif

	// generate the "random" spread pattern
	for ( i = 0 ; i < DEFAULT_SHOTGUN_COUNT ; i++ ) {
		r = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		u = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		VectorMA( origin, 8192 * 16, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);
		if( ShotgunPellet( origin, end, ent ) && !hitClient ) {
			hitClient = qtrue;
			ent->client->accuracy_hits++;
		}
	}
}


void weapon_supershotgun_fire (gentity_t *ent) {
	gentity_t		*tent;

	// send shotgun blast
	tent = G_TempEntity( muzzle, EV_SHOTGUN );
	VectorScale( forward, 4096, tent->s.origin2 );
	SnapVector( tent->s.origin2 );
	tent->s.eventParm = rand() & 255;		// seed for spread pattern
	tent->s.otherEntityNum = ent->s.number;

	ShotgunPattern( tent->s.pos.trBase, tent->s.origin2, tent->s.eventParm, ent );
}


/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void weapon_grenadelauncher_fire (gentity_t *ent) {
	gentity_t	*m;

#ifndef TMNTWEAPSYS_2
	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );
#endif

	m = fire_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

ROCKET

======================================================================
*/

void Weapon_RocketLauncher_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_rocket (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

PLASMA GUN

======================================================================
*/

void Weapon_Plasmagun_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_plasma (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

RAILGUN

======================================================================
*/


/*
=================
weapon_railgun_fire
=================
*/
#define	MAX_RAIL_HITS	4
void weapon_railgun_fire (gentity_t *ent) {
	vec3_t		end;
#if defined MISSIONPACK && !defined TMNT // POWERS
	vec3_t impactpoint, bouncedir;
#endif
	trace_t		trace;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;
	int			i;
	int			hits;
	int			unlinked;
	int			passent;
	gentity_t	*unlinkedEntities[MAX_RAIL_HITS];

	damage = 100 * s_quadFactor;

	VectorMA (muzzle, 8192, forward, end);

	// trace only against the solids, so the railgun will go through people
	unlinked = 0;
	hits = 0;
	passent = ent->s.number;
	do {
		trap_Trace (&trace, muzzle, NULL, NULL, end, passent, MASK_SHOT );
		if ( trace.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			break;
		}
		traceEnt = &g_entities[ trace.entityNum ];
		if ( traceEnt->takedamage ) {
#if defined MISSIONPACK && !defined TMNT // POWERS
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if ( G_InvulnerabilityEffect( traceEnt, forward, trace.endpos, impactpoint, bouncedir ) ) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					// snap the endpos to integers to save net bandwidth, but nudged towards the line
					SnapVectorTowards( trace.endpos, muzzle );
					// send railgun beam effect
					tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );
					// set player number for custom colors on the railtrail
					tent->s.clientNum = ent->s.clientNum;
					VectorCopy( muzzle, tent->s.origin2 );
					// move origin a bit to come closer to the drawn gun muzzle
					VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
					VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );
					tent->s.eventParm = 255;	// don't make the explosion at the end
					//
					VectorCopy( impactpoint, muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
			}
			else {
				if( LogAccuracyHit( traceEnt, ent ) ) {
					hits++;
				}
				G_Damage (traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_RAILGUN);
			}
#else
				if( LogAccuracyHit( traceEnt, ent ) ) {
					hits++;
				}
				G_Damage (traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_RAILGUN);
#endif
		}
		if ( trace.contents & CONTENTS_SOLID ) {
			break;		// we hit something solid enough to stop the beam
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_RAIL_HITS );

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ ) {
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );

	// set player number for custom colors on the railtrail
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( muzzle, tent->s.origin2 );
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
	VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ( trace.surfaceFlags & SURF_NOIMPACT ) {
		tent->s.eventParm = 255;	// don't make the explosion at the end
	} else {
		tent->s.eventParm = DirToByte( trace.plane.normal );
	}
	tent->s.clientNum = ent->s.clientNum;

	// give the shooter a reward sound if they have made two railgun hits in a row
	if ( hits == 0 ) {
		// complete miss
		ent->client->accurateCount = 0;
	} else {
		// check for "impressive" reward sound
		ent->client->accurateCount += hits;
		if ( ent->client->accurateCount >= 2 ) {
			ent->client->accurateCount -= 2;
			ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
			// add the sprite over the player's head
#ifdef IOQ3ZTM
			ent->client->ps.eFlags &= ~EF_AWARD_BITS;
#else
			ent->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
#endif
			ent->client->ps.eFlags |= EF_AWARD_IMPRESSIVE;
			ent->client->rewardTime = level.time + REWARD_SPRITE_TIME;
		}
		ent->client->accuracy_hits++;
	}

}
#endif // #ifndef TMNTWEAPSYS_2

/*
======================================================================

GRAPPLING HOOK

======================================================================
*/

#ifndef TMNTWEAPSYS_2
void Weapon_GrapplingHook_Fire (gentity_t *ent)
{
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple (ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}
#endif // #ifndef TMNTWEAPSYS_2

void Weapon_HookFree (gentity_t *ent)
{
#ifdef IOQ3ZTM // Better grapple.
	if (ent->parent->client->ps.pm_type != PM_DEAD)
	{
		// Turtle Man: TODO: Pull grapple back to player before removing entity, like LoZ: TP?
	}
#endif
	ent->parent->client->hook = NULL;
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_PULL;
#ifdef IOQ3ZTM // IOQ3BUGFIX: Fix Grapple-Attack player animation.
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_SHOT;
#endif
	G_FreeEntity( ent );
}

void Weapon_HookThink (gentity_t *ent)
{
	if (ent->enemy) {
		vec3_t v, oldorigin;

		VectorCopy(ent->r.currentOrigin, oldorigin);
		v[0] = ent->enemy->r.currentOrigin[0] + (ent->enemy->r.mins[0] + ent->enemy->r.maxs[0]) * 0.5;
		v[1] = ent->enemy->r.currentOrigin[1] + (ent->enemy->r.mins[1] + ent->enemy->r.maxs[1]) * 0.5;
		v[2] = ent->enemy->r.currentOrigin[2] + (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5;
		SnapVectorTowards( v, oldorigin );	// save net bandwidth

		G_SetOrigin( ent, v );
	}

	VectorCopy( ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);
}

#ifndef TMNTWEAPSYS_2
/*
======================================================================

LIGHTNING GUN

======================================================================
*/

void Weapon_LightningFire( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
#if defined MISSIONPACK && !defined TMNT // POWERS
	vec3_t impactpoint, bouncedir;
#endif
	gentity_t	*traceEnt, *tent;
	int			damage, i, passent;

	damage = 8 * s_quadFactor;

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {
		VectorMA( muzzle, LIGHTNING_RANGE, forward, end );

		trap_Trace( &tr, muzzle, NULL, NULL, end, passent, MASK_SHOT );

#if defined MISSIONPACK && !defined TMNT // POWERS
		// if not the first trace (the lightning bounced of an invulnerability sphere)
		if (i) {
			// add bounced off lightning bolt temp entity
			// the first lightning bolt is a cgame only visual
			//
			tent = G_TempEntity( muzzle, EV_LIGHTNINGBOLT );
			VectorCopy( tr.endpos, end );
			SnapVector( end );
			VectorCopy( end, tent->s.origin2 );
		}
#endif
		if ( tr.entityNum == ENTITYNUM_NONE ) {
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		if ( traceEnt->takedamage) {
#if defined MISSIONPACK && !defined TMNT // POWERS
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					VectorCopy( impactpoint, muzzle );
					VectorSubtract( end, impactpoint, forward );
					VectorNormalize(forward);
					// the player can hit him/herself with the bounced lightning
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, muzzle );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0, MOD_LIGHTNING);
			}
#else
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0, MOD_LIGHTNING);
#endif
		}

		if ( traceEnt->takedamage && traceEnt->client ) {
			tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
			tent->s.otherEntityNum = traceEnt->s.number;
			tent->s.eventParm = DirToByte( tr.plane.normal );
			tent->s.weapon = ent->s.weapon;
			if( LogAccuracyHit( traceEnt, ent ) ) {
				ent->client->accuracy_hits++;
			}
		} else if ( !( tr.surfaceFlags & SURF_NOIMPACT ) ) {
			tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
			tent->s.eventParm = DirToByte( tr.plane.normal );
		}

		break;
	}
}
#ifdef MISSIONPACK
/*
======================================================================

NAILGUN

======================================================================
*/

void Weapon_Nailgun_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < NUM_NAILSHOTS; count++ ) {
		m = fire_nail (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}


/*
======================================================================

PROXIMITY MINE LAUNCHER

======================================================================
*/

void weapon_proxlauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_prox (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

#endif
#endif // #ifndef TMNTWEAPSYS_2

//======================================================================


/*
===============
LogAccuracyHit
===============
*/
qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker ) {
	if( !target->takedamage ) {
		return qfalse;
	}

	if ( target == attacker ) {
		return qfalse;
	}

	if( !target->client
#ifdef TMNTNPCSYS // Turtle Man: When hit NPCs too
		&& target->s.eType != ET_NPC
#endif
	) {
		return qfalse;
	}

	if( !attacker->client ) {
		return qfalse;
	}

#ifdef TMNTNPCSYS // Turtle Man: When hit NPCs too
	if (!target->client)
	{
		if( target->health <= 0 ) {
			return qfalse;
		}
	}
	else
#endif
	if( target->client->ps.stats[STAT_HEALTH] <= 0 ) {
		return qfalse;
	}

	if ( OnSameTeam( target, attacker ) ) {
		return qfalse;
	}

	return qtrue;
}


/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
#ifdef TMNTNPCSYS
	if (ent->bgNPC.info)
	{
		muzzlePoint[2] += ent->bgNPC.npc_ps.viewheight;
	}
	else
#endif
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}

/*
===============
CalcMuzzlePointOrigin

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePointOrigin ( gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}



/*
===============
FireWeapon
===============
*/
void FireWeapon( gentity_t *ent ) {
	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
#ifdef MISSIONPACK // MP_TMNT_OK
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2;
	}
#endif

	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
#ifdef TMNTWEAPSYS
	if (BG_WeapUseAmmo(ent->s.weapon))
#else
	if( ent->s.weapon != WP_GRAPPLING_HOOK && ent->s.weapon != WP_GAUNTLET )
#endif
	{
#ifdef TMNTWEAPSYS_2
		ent->client->accuracy_shots += bg_weapongroupinfo[ent->s.weapon].weapon[0]->proj->numProjectiles;
#elif defined MISSIONPACK
		if( ent->s.weapon == WP_NAILGUN ) {
			ent->client->accuracy_shots += NUM_NAILSHOTS;
		} else {
			ent->client->accuracy_shots++;
		}
#else
		ent->client->accuracy_shots++;
#endif
	}

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );

#ifdef TMNTWEAPSYS
    if (BG_WeaponTypeForNum(ent->s.weapon) != WT_GUN
#ifndef TMNTWEAPSYS_2
		&& BG_WeaponTypeForNum(ent->s.weapon) != WT_GUN_PRIMARY
#endif
		)
    {
        return;
    }
#endif

#ifdef TMNTWEAPSYS_2
#ifdef TMNT
	G_AutoAim(ent, bg_weapongroupinfo[ent->s.weapon].weapon[0]->projnum,
			muzzle, forward, right, up);
#endif
	fire_weapon(ent, muzzle, forward, right, up,
			bg_weapongroupinfo[ent->s.weapon].weaponnum[0], s_quadFactor);
#else
#ifdef TMNT
	G_AutoAim(ent, 0, muzzle, forward, right, up);
#endif
	// fire the specific weapon
	switch( ent->s.weapon ) {
	case WP_GAUNTLET:
		Weapon_Gauntlet( ent );
		break;
	case WP_LIGHTNING:
		Weapon_LightningFire( ent );
		break;
	case WP_SHOTGUN:
		weapon_supershotgun_fire( ent );
		break;
	case WP_MACHINEGUN:
		if ( g_gametype.integer != GT_TEAM ) {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_DAMAGE );
		} else {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_TEAM_DAMAGE );
		}
		break;
	case WP_GRENADE_LAUNCHER:
		weapon_grenadelauncher_fire( ent );
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_RocketLauncher_Fire( ent );
		break;
	case WP_PLASMAGUN:
		Weapon_Plasmagun_Fire( ent );
		break;
	case WP_RAILGUN:
		weapon_railgun_fire( ent );
		break;
	case WP_BFG:
		BFG_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
#ifdef MISSIONPACK
	case WP_NAILGUN:
		Weapon_Nailgun_Fire( ent );
		break;
	case WP_PROX_LAUNCHER:
		weapon_proxlauncher_fire( ent );
		break;
	case WP_CHAINGUN:
		Bullet_Fire( ent, CHAINGUN_SPREAD, MACHINEGUN_DAMAGE );
		break;
#endif
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}
#endif
}


#if defined MISSIONPACK && !defined TMNTHOLDABLE // NO_KAMIKAZE_ITEM

/*
===============
KamikazeRadiusDamage
===============
*/
static void KamikazeRadiusDamage( vec3_t origin, gentity_t *attacker, float damage, float radius ) {
	float		dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		if (!ent->takedamage) {
			continue;
		}

		// dont hit things we have already hit
		if( ent->kamikazeTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, MOD_KAMIKAZE );
			ent->kamikazeTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeShockWave
===============
*/
static void KamikazeShockWave( vec3_t origin, gentity_t *attacker, float damage, float push, float radius ) {
	float		dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 )
		radius = 1;

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		// dont hit things we have already hit
		if( ent->kamikazeShockTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, MOD_KAMIKAZE );
			//
			dir[2] = 0;
			VectorNormalize(dir);
			if ( ent->client ) {
				ent->client->ps.velocity[0] = dir[0] * push;
				ent->client->ps.velocity[1] = dir[1] * push;
				ent->client->ps.velocity[2] = 100;
			}
			ent->kamikazeShockTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeDamage
===============
*/
static void KamikazeDamage( gentity_t *self ) {
	int i;
	float t;
	gentity_t *ent;
	vec3_t newangles;

	self->count += 100;

	if (self->count >= KAMI_SHOCKWAVE_STARTTIME) {
		// shockwave push back
		t = self->count - KAMI_SHOCKWAVE_STARTTIME;
		KamikazeShockWave(self->s.pos.trBase, self->activator, 25, 400,	(int) (float) t * KAMI_SHOCKWAVE_MAXRADIUS / (KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVE_STARTTIME) );
	}
	//
	if (self->count >= KAMI_EXPLODE_STARTTIME) {
		// do our damage
		t = self->count - KAMI_EXPLODE_STARTTIME;
		KamikazeRadiusDamage( self->s.pos.trBase, self->activator, 400,	(int) (float) t * KAMI_BOOMSPHERE_MAXRADIUS / (KAMI_IMPLODE_STARTTIME - KAMI_EXPLODE_STARTTIME) );
	}

	// either cycle or kill self
	if( self->count >= KAMI_SHOCKWAVE_ENDTIME ) {
		G_FreeEntity( self );
		return;
	}
	self->nextthink = level.time + 100;

	// add earth quake effect
	newangles[0] = crandom() * 2;
	newangles[1] = crandom() * 2;
	newangles[2] = 0;
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		ent = &g_entities[i];
		if (!ent->inuse)
			continue;
		if (!ent->client)
			continue;

		if (ent->client->ps.groundEntityNum != ENTITYNUM_NONE) {
			ent->client->ps.velocity[0] += crandom() * 120;
			ent->client->ps.velocity[1] += crandom() * 120;
			ent->client->ps.velocity[2] = 30 + random() * 25;
		}

		ent->client->ps.delta_angles[0] += ANGLE2SHORT(newangles[0] - self->movedir[0]);
		ent->client->ps.delta_angles[1] += ANGLE2SHORT(newangles[1] - self->movedir[1]);
		ent->client->ps.delta_angles[2] += ANGLE2SHORT(newangles[2] - self->movedir[2]);
	}
	VectorCopy(newangles, self->movedir);
}

/*
===============
G_StartKamikaze
===============
*/
void G_StartKamikaze( gentity_t *ent ) {
	gentity_t	*explosion;
	gentity_t	*te;
	vec3_t		snapped;

	// start up the explosion logic
	explosion = G_Spawn();

	explosion->s.eType = ET_EVENTS + EV_KAMIKAZE;
	explosion->eventTime = level.time;

	if ( ent->client ) {
		VectorCopy( ent->s.pos.trBase, snapped );
	}
	else {
		VectorCopy( ent->activator->s.pos.trBase, snapped );
	}
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( explosion, snapped );

	explosion->classname = "kamikaze";
	explosion->s.pos.trType = TR_STATIONARY;

	explosion->kamikazeTime = level.time;

	explosion->think = KamikazeDamage;
	explosion->nextthink = level.time + 100;
	explosion->count = 0;
	VectorClear(explosion->movedir);

	trap_LinkEntity( explosion );

	if (ent->client) {
		//
		explosion->activator = ent;
		//
		ent->s.eFlags &= ~EF_KAMIKAZE;
		// nuke the guy that used it
		G_Damage( ent, ent, ent, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_KAMIKAZE );
	}
	else {
		if ( !strcmp(ent->activator->classname, "bodyque") ) {
			explosion->activator = &g_entities[ent->activator->r.ownerNum];
		}
		else {
			explosion->activator = ent->activator;
		}
	}

	// play global sound at all clients
	te = G_TempEntity(snapped, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = GTS_KAMIKAZE;
}
#endif

#ifdef TMNTNPCSYS
void NPC_FireWeapon(gentity_t *ent)
{
	// set aiming directions
	AngleVectors (ent->bgNPC.npc_ps.viewangles, forward, right, up);

	// Make sure ent->bgNPC.npc_ps.viewheight is set here.
	CalcMuzzlePoint ( ent, forward, right, up, muzzle );

	s_quadFactor=1;

#ifdef TMNT
	G_AutoAim(ent, bg_weapongroupinfo[ent->s.weapon].weapon[0]->projnum,
			muzzle, forward, right, up);
#else
	// NPC just shoots forward, not at target...
#endif
#ifdef TMNTWEAPSYS_2
	fire_weapon(ent, muzzle, forward, right, up,
			bg_weapongroupinfo[ent->s.weapon].weaponnum[0], s_quadFactor);
#else
#warning "TMNTWEAPSYS_2 must be defined for NPC_FireWeapon"
	Com_Printf("Warning: TMNTWEAPSYS_2 must be defined for NPC_FireWeapon.\n");
#endif
}
#endif
