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

#define NUM_NAILSHOTS 15

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

#ifdef TMNTHOLDABLE
void G_ThrowShuriken(gentity_t *ent, holdable_t holdable)
{
	gentity_t	*m;

	// Turtle Man: TODO: Player animation (Throw shuriken)

	switch (holdable)
	{
		case HI_SHURIKEN:
		case HI_ELECTRICSHURIKEN:
		case HI_FIRESHURIKEN:
		case HI_LASERSHURIKEN:
			//G_Printf("  Used a shuriken item! (holdable=%d)\n", holdable);

			// Turtle Man: TODO: Throw from origin of tag_hand_* (primary or secondary?)

			// set aiming directions
			AngleVectors (ent->client->ps.viewangles, forward, right, up);

			CalcMuzzlePoint ( ent, forward, right, up, muzzle );
			//CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );

			m = fire_shuriken (ent, muzzle, forward, holdable);
			m->damage *= s_quadFactor;
			m->splashDamage *= s_quadFactor;

//			VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
			break;
		default:
			G_Error("Unknown shuriken type (holdable=%d)\n", holdable);
			return;
	}
}
#endif

#ifdef TMNTWEAPSYS // MELEEATTACK
#define MELEE_COMBOTIME 1500
// Turtle Man: TODO: make MELEE_ATTACK_TIME per weapontype setting,
//       attack animations should be the same time
// 20090502: ...or use the animation time for the attack time! Its load in the playercfg!
#define MELEE_ATTACK_TIME 400

void G_StartMeleeAttack(gentity_t *ent)
{
	gclient_t *client = ent->client;
	weapontype_t wt = BG_WeaponTypeForPlayerState(&ent->client->ps);

	// Make sure it is a melee weapon.
	if ( !BG_WeapTypeIsMelee( wt ) ) {
		return;
	}

	client->ps.meleeAttack++;
	client->ps.meleeTime = MELEE_ATTACK_TIME;
	client->ps.comboTime = MELEE_COMBOTIME;
#ifndef TMNTRELEASE
	G_Printf("DEBUG: client %i started new melee attack (%i)\n", ent - g_entities, client->ps.meleeAttack);
#endif
}

#ifdef TMNTWEAPSYS_1 // GAME_TAGS
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

	// lerp the tag
	if (!trap_LerpTag( &lerped, parentTags, parentLF->oldFrame, parentLF->frame,
		1.0 - parentLF->backlerp, tagName ))
	{
		return qfalse;
	}

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent.origin, child->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( child->origin, lerped.origin[i], parent.axis[i], child->origin );
	}

	MatrixMultiply( lerped.axis, parent.axis, child->axis );

	return qtrue;
}


/*
======================
G_PositionRotatedEntityOnTag

Modifies the child's position and axis by the given
tag location
======================
*/
void  G_PositionRotatedEntityOnTag( orientation_t *child, lerpFrame_t *parentLF,
	orientation_t parent, qhandle_t parentTags, char *tagName )
{
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];

//AxisClear( entity->axis );
	// lerp the tag
	trap_LerpTag( &lerped, parentTags, parentLF->oldFrame, parentLF->frame,
		1.0 - parentLF->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent.origin, child->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( child->origin, lerped.origin[i], parent.axis[i], child->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( child->axis, lerped.axis, tempAxis );
	MatrixMultiply( tempAxis, parent.axis, child->axis );
}
#endif

// Based on CheckGauntletHit
qboolean G_DoMeleeDamage(gentity_t *ent, qboolean nodamage, int hand, weapontype_t wt)
{
	trace_t		tr;
	vec3_t		start;
	vec3_t		end;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;
	weapon_t weaponnum;
	int mod; // Means of death
#ifdef TMNTWEAPSYS_1 // Use the weapon tag angles and pos!
	orientation_t	legsOrientation; // Must init!
	orientation_t	torsoOrientation;
	orientation_t	weaponOrientation;

	if (!ent->client->pers.legsTags && !ent->client->pers.torsoTags)
	{
		G_Printf("DEBUG: G_DoMeleeDamage: Invalid tags for legs and torso model.\n");
		return qfalse;
	}
	if (!ent->client->pers.legsTags)
	{
		G_Printf("DEBUG: G_DoMeleeDamage: Invalid tags for legs model.\n");
		return qfalse;
	}
	if (!ent->client->pers.torsoTags)
	{
		G_Printf("DEBUG: G_DoMeleeDamage: Invalid tags for torso model.\n");
		return qfalse;
	}

	weaponnum = ent->client->ps.weapon;

	// Clear memory
	memset(&legsOrientation, 0, sizeof (legsOrientation));
	memset(&torsoOrientation, 0, sizeof (torsoOrientation));
	memset(&weaponOrientation, 0, sizeof (weaponOrientation));

	// Set to player's origin
	VectorCopy(ent->r.currentOrigin, legsOrientation.origin);

	// Use pre-calculated legsAxis
	AxisCopy(ent->client->pers.legsAxis, legsOrientation.axis);

	// Use pre-calculated torsoAxis
	AxisCopy(ent->client->pers.torsoAxis, torsoOrientation.axis);

	// Find torso origin
	G_PositionRotatedEntityOnTag(&torsoOrientation, &ent->client->pers.legs,
		legsOrientation, ent->client->pers.legsTags, "tag_torso");

	// Use hand to select the weapon tag.
	if (hand == HAND_PRIMARY)
	{
		// put weapon on torso
#ifdef TMNTPLAYERS
		if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
			torsoOrientation, ent->client->pers.torsoTags, "tag_hand_primary"))
#endif
		{
			if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
				torsoOrientation, ent->client->pers.torsoTags, "tag_weapon"))
			{
				// Failed to put weapon on torso!
				G_Printf("DEBUG: G_DoMeleeDamage: Player missing primary weapon tag!\n");
				return qfalse;
			}
		}
	}
	else
	{
		// put weapon on torso
#ifdef TMNTPLAYERS
		if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
			torsoOrientation, ent->client->pers.torsoTags, "tag_hand_secondary"))
#endif
		{
			if (!G_PositionEntityOnTag(&weaponOrientation, &ent->client->pers.torso,
				torsoOrientation, ent->client->pers.torsoTags, "tag_flag"))
			{
				// Failed to put weapon on torso!
				// Turtle Man: NOTE: Disable message so quake3 players
				//                   don't give lots of errors.
				//G_Printf("DEBUG: G_DoMeleeDamage: Player missing secondary weapon tag!\n");
				return qfalse;
			}
		}
	}

	// Setup "start" and "end" using weaponOrientation
	VectorCopy(weaponOrientation.axis[0], forward);
	VectorCopy(weaponOrientation.axis[1], right);
	VectorCopy(weaponOrientation.axis[2], up);

	// Guns use forward, Melee use up
	VectorMA (weaponOrientation.origin, bg_weaponinfo[weaponnum].start_range, up, start);
	VectorMA (weaponOrientation.origin, bg_weaponinfo[weaponnum].end_range, up, end);

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
		if (bg_weaponinfo[weaponnum].start_range != 0
			&& bg_weaponinfo[weaponnum].end_range != 0)
		{
			t = G_TempEntity( weaponOrientation.origin, EV_DEBUG_ORIGIN );
			t->s.eventParm = 3; // grey
			//t->s.otherEntityNum = ent->s.number; // client num
			t->r.svFlags = SVF_BROADCAST;	// send to everyone
		}
	}
#else
	weaponnum = ent->client->ps.weapon;

	// Turtle Man: DONE: Change angles and pos based on weapon tag!
	//       If will make melee attacking much more gooder,
	//         should be easier to code too.
	//       Use 'hand' to set angle and pos
	if (hand == HAND_PRIMARY)
	{
		// set aiming directions
		AngleVectors (ent->client->ps.viewangles, forward, right, up);

		CalcMuzzlePoint ( ent, forward, right, up, muzzle );
	}
	else if (hand == HAND_SECONDARY)
	{
		// Turtle Man: FIXME: Use the secondary weapon
		return qfalse;
	}

	VectorMA (muzzle, bg_weaponinfo[weaponnum].start_range, forward, start);
	VectorMA (muzzle, bg_weaponinfo[weaponnum].end_range, forward, end);
#endif

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

	// Turtle Man: FIXME: Don't stop at first entity hit
	//trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
	trap_Trace (&tr, start, bg_weaponinfo[weaponnum].mins,
			bg_weaponinfo[weaponnum].maxs, end, ent->s.number, MASK_SHOT);

	if ( tr.surfaceFlags & SURF_NOIMPACT ) {
		return qfalse;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	if (tr.entityNum < MAX_CLIENTS)
	{
		// pain_debounce?
#ifndef TMNTRELEASE
		G_Printf("DEBUG: client %i hit client %i\n", ent - g_entities, tr.entityNum);
#endif
	}

	// send blood impact
	// Turtle Man: TODO: Do when hit anything! (melee hit effect, not blood...)
	if ( !nodamage && traceEnt->takedamage/* && traceEnt->client*/ ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	}

	if ( !traceEnt->takedamage) {
		return qfalse;
	}

	if (nodamage)
	{
		return qtrue;
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
		G_AddEvent( ent, EV_POWERUP_QUAD, 0 );
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
#ifdef MISSIONPACK // MP_TMNT_OK
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2;
	}
#endif

#ifdef TMNTWEAPONS
	if (wt == WT_HAMMER || wt == WT_HAMMER_PRIMARY)
	{
		damage = 20;
	}
	else
#endif
	{
		damage = 10;
	}

	damage *= s_quadFactor;

	if (hand == HAND_SECONDARY)
	{
		mod = bg_weaponinfo[weaponnum].mod2;
	}
	else
	{
		mod = bg_weaponinfo[weaponnum].mod;
	}

	G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		damage, 0, mod );

	ent->client->ps.comboTime = MELEE_COMBOTIME;
	return qtrue;
}

qboolean G_MeleeAttack(gentity_t *ent, qboolean nodamage)
{
	qboolean rtn, rtn2;
	weapontype_t wt;

	wt = BG_WeaponTypeForPlayerState(&ent->client->ps);

	// Make sure it is a melee weapon.
	if ( !BG_WeapTypeIsMelee( wt ) ) {
		return qfalse;
	}

	rtn = rtn2 = qfalse;

	if (ent->client->ps.weaponHands & HAND_PRIMARY)
	{
		rtn = G_DoMeleeDamage(ent, nodamage, HAND_PRIMARY, wt);
	}
	if (ent->client->ps.weaponHands & HAND_SECONDARY)
	{
		rtn2 = G_DoMeleeDamage(ent, nodamage, HAND_SECONDARY, wt);
	}

	return (rtn || rtn2);
}
#endif // TMNTWEAPSYS
#ifndef TMNTWEAPONS
/*
======================================================================

GAUNTLET

======================================================================
*/

void Weapon_Gauntlet( gentity_t *ent ) {

}

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
#endif // TMNTWEAPONS


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
#ifdef SP_NPC
		if ( traceEnt->takedamage && (traceEnt->client || traceEnt->s.eType==ET_NPC)) {
#else
		if ( traceEnt->takedamage && traceEnt->client ) {
#endif
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_FLESH );
			tent->s.eventParm = traceEnt->s.number;
#ifndef SP_NPC
			if( LogAccuracyHit( traceEnt, ent ) ) {
				ent->client->accuracy_hits++;
			}
#endif
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

#ifndef TMNTWEAPONS
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

// DEFAULT_SHOTGUN_SPREAD and DEFAULT_SHOTGUN_COUNT	are in bg_public.h, because
// client predicts same spreads
#define	DEFAULT_SHOTGUN_DAMAGE	10

qboolean ShotgunPellet( vec3_t start, vec3_t end, gentity_t *ent ) {
	trace_t		tr;
	int			damage, i, passent;
	gentity_t	*traceEnt;
#ifdef MISSIONPACK
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
	int			oldScore;
	qboolean	hitClient = qfalse;

	// derive the right and up vectors from the forward vector, because
	// the client won't have any other information
	VectorNormalize2( origin2, forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

	oldScore = ent->client->ps.persistant[PERS_SCORE];

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

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}
#endif // TMNTWEAPONS

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

#ifdef TMNTWEAPONS
/*
======================================================================

HOMING ROCKET

======================================================================
*/

void Weapon_HomingLauncher_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_homingrocket (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

GUN

======================================================================
*/

void Weapon_Gun_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_gun (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}
#endif

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

#ifndef TMNTWEAPONS
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
#ifdef MISSIONPACK
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
			ent->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
			ent->client->ps.eFlags |= EF_AWARD_IMPRESSIVE;
			ent->client->rewardTime = level.time + REWARD_SPRITE_TIME;
		}
		ent->client->accuracy_hits++;
	}

}
#endif // TMNTWEAPONS

/*
======================================================================

GRAPPLING HOOK

======================================================================
*/

void Weapon_GrapplingHook_Fire (gentity_t *ent)
{
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple (ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}

void Weapon_HookFree (gentity_t *ent)
{
	ent->parent->client->hook = NULL;
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_PULL;
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

#ifndef TMNTWEAPONS
/*
======================================================================

LIGHTNING GUN

======================================================================
*/

void Weapon_LightningFire( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
#ifdef MISSIONPACK
	vec3_t impactpoint, bouncedir;
#endif
	gentity_t	*traceEnt, *tent;
	int			damage, i, passent;

	damage = 8 * s_quadFactor;

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {
		VectorMA( muzzle, LIGHTNING_RANGE, forward, end );

		trap_Trace( &tr, muzzle, NULL, NULL, end, passent, MASK_SHOT );

#ifdef MISSIONPACK
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
#endif // TMNTWEAPONS

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

	if( !target->client ) {
		return qfalse;
	}

	if( !attacker->client ) {
		return qfalse;
	}

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
	if( ent->s.weapon != WP_GRAPPLING_HOOK
#ifndef TMNTWEAPONS
	&& ent->s.weapon != WP_GAUNTLET
#endif
	) {
#if defined MISSIONPACK && !defined TMNTWEAPONS // missionpack
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
		&& BG_WeaponTypeForNum(ent->s.weapon) != WT_GUN_PRIMARY)
    {
        return;
    }
#endif

	// fire the specific weapon
	switch( ent->s.weapon ) {
#ifdef TMNTWEAPONS
	case WP_GUN:
		Weapon_Gun_Fire( ent );
		break;
	case WP_ELECTRIC_LAUNCHER:
		Weapon_Plasmagun_Fire( ent );
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_RocketLauncher_Fire( ent );
		break;
	case WP_HOMING_LAUNCHER:
		Weapon_HomingLauncher_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
#else
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
#endif
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}
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
#ifndef TMNTHOLDABLE // NO_KAMIKAZE_ITEM
		//
		ent->s.eFlags &= ~EF_KAMIKAZE;
#endif
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

#ifdef SP_NPC
void NPC_FireWeapon(gentity_t *ent,vec3_t target_pos)
{
	vec3_t dir,ang;
	vec3_t fire_org;
	VectorCopy(ent->r.currentOrigin,fire_org);
	if (ent->npc->npcType==NPC_SOLDIER1 || ent->npc->npcType==NPC_SOLDIER2
		|| ent->npc->npcType==NPC_PILOT || ent->npc->npcType==NPC_ANK)
	{
		fire_org[2]+=15;
	}
	else if (ent->npc->npcType==NPC_BAT)
	{
		fire_org[2]+=33;
	}
	else if (ent->npc->npcType==NPC_SEALORD)
	{
		fire_org[2]+=30;
		fire_org[1]-=120;
	}
	VectorSubtract (target_pos,fire_org, dir);
	vectoangles(dir,ang);
	//if (ent->npc->npcType!=NPC_SEALORD)
	//{
		ang[1]=ent->ns.ps.viewangles[1];
	//}
	//else if (ent->ns.fireTime > 0)
	//{
	//	ang[0]+=rand()%15-7;
	//	ang[1]+=rand()%15-7;
	//	ang[2]+=rand()%15-7;
	//}
	s_quadFactor=1;
	AngleVectors (ang, forward, right, up);
	VectorMA( fire_org, 1, forward, muzzle);
	SnapVector( muzzle );

#ifdef TMNTWEAPONS
	if (ent->npc->weapon == WP_GUN)
#else
	if (ent->npc->weapon == WP_MACHINEGUN)
#endif
	{
		Bullet_Fire(ent,50,20*(int)(1.0f+0.25f*(float)npc_skill));
	}
	else  if (ent->npc->npcType==NPC_MOUSER)
	{
		fire_mouser_shot(ent, muzzle, forward, ent->ns.enemy);
	}
	else  if (ent->npc->npcType==NPC_FLYBOT)
	{
		fire_flybot_shot(ent, muzzle, forward);
	}
}
#endif
