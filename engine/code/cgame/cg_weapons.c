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
// cg_weapons.c -- events and effects dealing with weapons
#include "cg_local.h"

#ifdef TA_HOLDSYS/*2*/
/*
===============
CG_NextHoldable_f
Based on CG_NextWeapon_f
===============
*/
void CG_NextHoldable_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	//cg.weaponSelectTime = cg.time;
	original = cg.holdableSelect;

	if (cg.holdableSelect == HI_NO_SELECT)
	{
		cg.holdableSelect = cg.snap->ps.holdableIndex;
	}

	for ( i = 1 ; i < HI_NUM_HOLDABLE ; i++ ) {
		cg.holdableSelect++;
		if ( cg.holdableSelect == HI_NUM_HOLDABLE ) {
			cg.holdableSelect = 1;
		}

#ifndef MISSIONPACK // if not MP skip its holdables.
		if (
#ifndef TA_HOLDABLE // NO_KAMIKAZE_ITEM
		cg.holdableSelect == HI_KAMIKAZE ||
#endif
		cg.holdableSelect == HI_PORTAL
#ifndef TURTLEARENA // POWERS
			|| cg.holdableSelect == HI_INVULNERABILITY
#endif
			) {
			continue;
		}
#endif

		if ( cg.snap->ps.holdable[cg.holdableSelect] != 0 ) {
			break;
		}
	}
	if ( i == HI_NUM_HOLDABLE ) {
		cg.holdableSelect = original;
	}
}

/*
===============
CG_PrevHoldable_f
Based on CG_PrevWeapon_f
===============
*/
void CG_PrevHoldable_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	//cg.weaponSelectTime = cg.time;
	original = cg.holdableSelect;

	if (cg.holdableSelect == HI_NO_SELECT)
	{
		cg.holdableSelect = cg.snap->ps.holdableIndex;
	}

	for ( i = 1 ; i < HI_NUM_HOLDABLE ; i++ ) {
		cg.holdableSelect--;
		if ( cg.holdableSelect == 0 ) {
			cg.holdableSelect = HI_NUM_HOLDABLE - 1;
		}

#ifndef MISSIONPACK // if not MP skip its holdables.
		if (
#ifndef TA_HOLDABLE // NO_KAMIKAZE_ITEM
		cg.holdableSelect == HI_KAMIKAZE ||
#endif
		cg.holdableSelect == HI_PORTAL
#ifndef TURTLEARENA // POWERS
			|| cg.holdableSelect == HI_INVULNERABILITY
#endif
			) {
			continue;
		}
#endif

		if ( cg.snap->ps.holdable[cg.holdableSelect] != 0 ) {
			break;
		}
	}
	if ( i == HI_NUM_HOLDABLE ) {
		cg.holdableSelect = original;
	}
}

/*
===============
CG_Holdable_f
Based on CG_Weapon_f
===============
*/
void CG_Holdable_f( void ) {
	int		num;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	num = atoi( CG_Argv( 1 ) );

	if ( num < 0 || num >= HI_NUM_HOLDABLE ) {
		return;
	}

	//if (num != 0 && cg.snap->ps.holdable[num] == 0) {
	//	return;		// don't have the holdable item
	//}

	cg.holdableSelect = num;
}
#endif

/*
==========================
CG_MachineGunEjectBrass
==========================
*/
static void CG_MachineGunEjectBrass( centity_t *cent
#ifdef TA_WEAPSYS
	, int handSide
#endif
) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			velocity, xvelocity;
	vec3_t			offset, xoffset;
	float			waterScale = 1.0f;
	vec3_t			v[3];

	if ( cg_brassTime.integer <= 0 ) {
		return;
	}

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	velocity[0] = 0;
	velocity[1] = -50 + 40 * crandom();
	velocity[2] = 100 + 50 * crandom();

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + cg_brassTime.integer + ( cg_brassTime.integer / 4 ) * random();

	le->pos.trType = TR_GRAVITY;
	le->pos.trTime = cg.time - (rand()&15);

	AnglesToAxis( cent->lerpAngles, v );

	offset[0] = 8;
#ifdef TA_WEAPSYS
	if (handSide == HAND_RIGHT)
		offset[1] = -4;
	else if (handSide == HAND_LEFT)
		offset[1] = 4;
	else
		offset[1] = 0;
#else
	offset[1] = -4;
#endif
	offset[2] = 24;

	xoffset[0] = offset[0] * v[0][0] + offset[1] * v[1][0] + offset[2] * v[2][0];
	xoffset[1] = offset[0] * v[0][1] + offset[1] * v[1][1] + offset[2] * v[2][1];
	xoffset[2] = offset[0] * v[0][2] + offset[1] * v[1][2] + offset[2] * v[2][2];
	VectorAdd( cent->lerpOrigin, xoffset, re->origin );

	VectorCopy( re->origin, le->pos.trBase );

	if ( CG_PointContents( re->origin, -1 ) & CONTENTS_WATER ) {
		waterScale = 0.10f;
	}

	xvelocity[0] = velocity[0] * v[0][0] + velocity[1] * v[1][0] + velocity[2] * v[2][0];
	xvelocity[1] = velocity[0] * v[0][1] + velocity[1] * v[1][1] + velocity[2] * v[2][1];
	xvelocity[2] = velocity[0] * v[0][2] + velocity[1] * v[1][2] + velocity[2] * v[2][2];
	VectorScale( xvelocity, waterScale, le->pos.trDelta );

	AxisCopy( axisDefault, re->axis );
	re->hModel = cgs.media.machinegunBrassModel;

	le->bounceFactor = 0.4 * waterScale;

	le->angles.trType = TR_LINEAR;
	le->angles.trTime = cg.time;
	le->angles.trBase[0] = rand()&31;
	le->angles.trBase[1] = rand()&31;
	le->angles.trBase[2] = rand()&31;
	le->angles.trDelta[0] = 2;
	le->angles.trDelta[1] = 1;
	le->angles.trDelta[2] = 0;

	le->leFlags = LEF_TUMBLE;
	le->leBounceSoundType = LEBS_BRASS;
	le->leMarkType = LEMT_NONE;
}

/*
==========================
CG_ShotgunEjectBrass
==========================
*/
static void CG_ShotgunEjectBrass( centity_t *cent
#ifdef TA_WEAPSYS
	, int handSide
#endif
	)
{
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			velocity, xvelocity;
	vec3_t			offset, xoffset;
	vec3_t			v[3];
	int				i;

	if ( cg_brassTime.integer <= 0 ) {
		return;
	}

	for ( i = 0; i < 2; i++ ) {
		float	waterScale = 1.0f;

		le = CG_AllocLocalEntity();
		re = &le->refEntity;

		velocity[0] = 60 + 60 * crandom();
		if ( i == 0 ) {
			velocity[1] = 40 + 10 * crandom();
		} else {
			velocity[1] = -40 + 10 * crandom();
		}
		velocity[2] = 100 + 50 * crandom();

		le->leType = LE_FRAGMENT;
		le->startTime = cg.time;
		le->endTime = le->startTime + cg_brassTime.integer*3 + cg_brassTime.integer * random();

		le->pos.trType = TR_GRAVITY;
		le->pos.trTime = cg.time;

		AnglesToAxis( cent->lerpAngles, v );

		offset[0] = 8;
#ifdef TA_WEAPSYS
		if (handSide == HAND_RIGHT)
			offset[1] = -4;
		else if (handSide == HAND_LEFT)
			offset[1] = 4;
		else
			offset[1] = 0;
#else
		offset[1] = 0;
#endif
		offset[2] = 24;

		xoffset[0] = offset[0] * v[0][0] + offset[1] * v[1][0] + offset[2] * v[2][0];
		xoffset[1] = offset[0] * v[0][1] + offset[1] * v[1][1] + offset[2] * v[2][1];
		xoffset[2] = offset[0] * v[0][2] + offset[1] * v[1][2] + offset[2] * v[2][2];
		VectorAdd( cent->lerpOrigin, xoffset, re->origin );
		VectorCopy( re->origin, le->pos.trBase );
		if ( CG_PointContents( re->origin, -1 ) & CONTENTS_WATER ) {
			waterScale = 0.10f;
		}

		xvelocity[0] = velocity[0] * v[0][0] + velocity[1] * v[1][0] + velocity[2] * v[2][0];
		xvelocity[1] = velocity[0] * v[0][1] + velocity[1] * v[1][1] + velocity[2] * v[2][1];
		xvelocity[2] = velocity[0] * v[0][2] + velocity[1] * v[1][2] + velocity[2] * v[2][2];
		VectorScale( xvelocity, waterScale, le->pos.trDelta );

		AxisCopy( axisDefault, re->axis );
#ifdef TA_DATA
		re->hModel = cgs.media.machinegunBrassModel;
#else
		re->hModel = cgs.media.shotgunBrassModel;
#endif
		le->bounceFactor = 0.3f;

		le->angles.trType = TR_LINEAR;
		le->angles.trTime = cg.time;
		le->angles.trBase[0] = rand()&31;
		le->angles.trBase[1] = rand()&31;
		le->angles.trBase[2] = rand()&31;
		le->angles.trDelta[0] = 1;
		le->angles.trDelta[1] = 0.5;
		le->angles.trDelta[2] = 0;

		le->leFlags = LEF_TUMBLE;
		le->leBounceSoundType = LEBS_BRASS;
		le->leMarkType = LEMT_NONE;
	}
}


#if defined MISSIONPACK || defined TA_WEAPSYS
/*
==========================
CG_NailgunEjectBrass
==========================
*/
static void CG_NailgunEjectBrass( centity_t *cent
#ifdef TA_WEAPSYS
	, int handSide
#endif
) {
	localEntity_t	*smoke;
	vec3_t			origin;
	vec3_t			v[3];
	vec3_t			offset;
	vec3_t			xoffset;
	vec3_t			up;

	AnglesToAxis( cent->lerpAngles, v );

	offset[0] = 0;
#ifdef TA_WEAPSYS
	if (handSide == HAND_RIGHT)
		offset[1] = -12;
	else if (handSide == HAND_LEFT)
		offset[1] = 12;
	else
		offset[1] = 0;
#else
	offset[1] = -12;
#endif
	offset[2] = 24;

	xoffset[0] = offset[0] * v[0][0] + offset[1] * v[1][0] + offset[2] * v[2][0];
	xoffset[1] = offset[0] * v[0][1] + offset[1] * v[1][1] + offset[2] * v[2][1];
	xoffset[2] = offset[0] * v[0][2] + offset[1] * v[1][2] + offset[2] * v[2][2];
	VectorAdd( cent->lerpOrigin, xoffset, origin );

	VectorSet( up, 0, 0, 64 );

	smoke = CG_SmokePuff( origin, up, 32, 1, 1, 1, 0.33f, 700, cg.time, 0, 0, cgs.media.smokePuffShader );
	// use the optimized local entity add
	smoke->leType = LE_SCALE_FADE;
}
#endif

/*
==========================
CG_RailTrail
==========================
*/
void CG_RailTrail (clientInfo_t *ci, vec3_t start, vec3_t end) {
	vec3_t axis[36], move, move2, next_move, vec, temp;
	float  len;
	int    i, j, skip;
 
	localEntity_t *le;
	refEntity_t   *re;
 
#define RADIUS   4
#define ROTATION 1
#define SPACING  5
 
	start[2] -= 4;
 
	le = CG_AllocLocalEntity();
	re = &le->refEntity;
 
	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + cg_railTrailTime.value;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);
 
	re->shaderTime = cg.time / 1000.0f;
	re->reType = RT_RAIL_CORE;
	re->customShader = cgs.media.railCoreShader;
 
	VectorCopy(start, re->origin);
	VectorCopy(end, re->oldorigin);
 
	re->shaderRGBA[0] = ci->color1[0] * 255;
	re->shaderRGBA[1] = ci->color1[1] * 255;
	re->shaderRGBA[2] = ci->color1[2] * 255;
	re->shaderRGBA[3] = 255;

	le->color[0] = ci->color1[0] * 0.75;
	le->color[1] = ci->color1[1] * 0.75;
	le->color[2] = ci->color1[2] * 0.75;
	le->color[3] = 1.0f;

	AxisClear( re->axis );
 
	if (cg_oldRail.integer
#ifdef IOQ3ZTM // LASERTAG
		|| cg_laserTag.integer
#endif
		)
	{
		// nudge down a bit so it isn't exactly in center
		re->origin[2] -= 8;
		re->oldorigin[2] -= 8;
		return;
	}

	VectorCopy (start, move);
	VectorSubtract (end, start, vec);
	len = VectorNormalize (vec);
	PerpendicularVector(temp, vec);
	for (i = 0 ; i < 36; i++)
	{
		RotatePointAroundVector(axis[i], vec, temp, i * 10);//banshee 2.4 was 10
	}

	VectorMA(move, 20, vec, move);
	VectorCopy(move, next_move);
	VectorScale (vec, SPACING, vec);

	skip = -1;
 
	j = 18;
	for (i = 0; i < len; i += SPACING)
	{
		if (i != skip)
		{
			skip = i + SPACING;
			le = CG_AllocLocalEntity();
			re = &le->refEntity;
			le->leFlags = LEF_PUFF_DONT_SCALE;
			le->leType = LE_MOVE_SCALE_FADE;
			le->startTime = cg.time;
			le->endTime = cg.time + (i>>1) + 600;
			le->lifeRate = 1.0 / (le->endTime - le->startTime);

			re->shaderTime = cg.time / 1000.0f;
			re->reType = RT_SPRITE;
			re->radius = 1.1f;
			re->customShader = cgs.media.railRingsShader;

			re->shaderRGBA[0] = ci->color2[0] * 255;
			re->shaderRGBA[1] = ci->color2[1] * 255;
			re->shaderRGBA[2] = ci->color2[2] * 255;
			re->shaderRGBA[3] = 255;

			le->color[0] = ci->color2[0] * 0.75;
			le->color[1] = ci->color2[1] * 0.75;
			le->color[2] = ci->color2[2] * 0.75;
			le->color[3] = 1.0f;

			le->pos.trType = TR_LINEAR;
			le->pos.trTime = cg.time;

			VectorCopy( move, move2);
			VectorMA(move2, RADIUS , axis[j], move2);
			VectorCopy(move2, le->pos.trBase);

			le->pos.trDelta[0] = axis[j][0]*6;
			le->pos.trDelta[1] = axis[j][1]*6;
			le->pos.trDelta[2] = axis[j][2]*6;
		}

		VectorAdd (move, vec, move);

		j = (j + ROTATION) % 36;
	}
}

/*
==========================
CG_RocketTrail
==========================
*/
#ifdef TA_WEAPSYS
static void CG_RocketTrail( centity_t *ent, const projectileInfo_t *wi )
#else
static void CG_RocketTrail( centity_t *ent, const weaponInfo_t *wi )
#endif
{
	int		step;
	vec3_t	origin, lastPos;
	int		t;
	int		startTime, contents;
	int		lastContents;
	entityState_t	*es;
	vec3_t	up;
	localEntity_t	*smoke;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	up[0] = 0;
	up[1] = 0;
	up[2] = 0;

	step = 50;

	es = &ent->currentState;
	startTime = ent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	contents = CG_PointContents( origin, -1 );

	// if object (e.g. grenade) is stationary, don't toss up smoke
	if ( es->pos.trType == TR_STATIONARY ) {
		ent->trailTime = cg.time;
		return;
	}

	BG_EvaluateTrajectory( &es->pos, ent->trailTime, lastPos );
	lastContents = CG_PointContents( lastPos, -1 );

	ent->trailTime = cg.time;

	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		if ( contents & lastContents & CONTENTS_WATER ) {
			CG_BubbleTrail( lastPos, origin, 8 );
		}
		return;
	}

	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up, 
					  wi->trailRadius, 
					  1, 1, 1, 0.33f,
					  wi->wiTrailTime, 
					  t,
					  0,
					  0, 
					  cgs.media.smokePuffShader );
		// use the optimized local entity add
		smoke->leType = LE_SCALE_FADE;
	}

}

#ifdef MISSIONPACK
/*
==========================
CG_NailTrail
==========================
*/
#ifdef TA_WEAPSYS
static void CG_NailTrail( centity_t *ent, const projectileInfo_t *wi )
#else
static void CG_NailTrail( centity_t *ent, const weaponInfo_t *wi )
#endif
{
	int		step;
	vec3_t	origin, lastPos;
	int		t;
	int		startTime, contents;
	int		lastContents;
	entityState_t	*es;
	vec3_t	up;
	localEntity_t	*smoke;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	up[0] = 0;
	up[1] = 0;
	up[2] = 0;

	step = 50;

	es = &ent->currentState;
	startTime = ent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	contents = CG_PointContents( origin, -1 );

	// if object (e.g. grenade) is stationary, don't toss up smoke
	if ( es->pos.trType == TR_STATIONARY ) {
		ent->trailTime = cg.time;
		return;
	}

	BG_EvaluateTrajectory( &es->pos, ent->trailTime, lastPos );
	lastContents = CG_PointContents( lastPos, -1 );

	ent->trailTime = cg.time;

	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		if ( contents & lastContents & CONTENTS_WATER ) {
			CG_BubbleTrail( lastPos, origin, 8 );
		}
		return;
	}

	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up, 
					  wi->trailRadius, 
					  1, 1, 1, 0.33f,
					  wi->wiTrailTime, 
					  t,
					  0,
					  0, 
					  cgs.media.nailPuffShader );
		// use the optimized local entity add
		smoke->leType = LE_SCALE_FADE;
	}

}
#endif

/*
==========================
CG_PlasmaTrail
==========================
*/
#ifdef TA_WEAPSYS
static void CG_PlasmaTrail( centity_t *cent, const projectileInfo_t *wi )
#else
static void CG_PlasmaTrail( centity_t *cent, const weaponInfo_t *wi )
#endif
{
	localEntity_t	*le;
	refEntity_t		*re;
	entityState_t	*es;
	vec3_t			velocity, xvelocity, origin;
	vec3_t			offset, xoffset;
	vec3_t			v[3];
	int				t, startTime, step;

	float	waterScale = 1.0f;

	if ( cg_noProjectileTrail.integer || cg_oldPlasma.integer ) {
		return;
	}

	step = 50;

	es = &cent->currentState;
	startTime = cent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	velocity[0] = 60 - 120 * crandom();
	velocity[1] = 40 - 80 * crandom();
	velocity[2] = 100 - 200 * crandom();

	le->leType = LE_MOVE_SCALE_FADE;
	le->leFlags = LEF_TUMBLE;
	le->leBounceSoundType = LEBS_NONE;
	le->leMarkType = LEMT_NONE;

	le->startTime = cg.time;
	le->endTime = le->startTime + 600;

	le->pos.trType = TR_GRAVITY;
	le->pos.trTime = cg.time;

	AnglesToAxis( cent->lerpAngles, v );

	offset[0] = 2;
	offset[1] = 2;
	offset[2] = 2;

	xoffset[0] = offset[0] * v[0][0] + offset[1] * v[1][0] + offset[2] * v[2][0];
	xoffset[1] = offset[0] * v[0][1] + offset[1] * v[1][1] + offset[2] * v[2][1];
	xoffset[2] = offset[0] * v[0][2] + offset[1] * v[1][2] + offset[2] * v[2][2];

	VectorAdd( origin, xoffset, re->origin );
	VectorCopy( re->origin, le->pos.trBase );

	if ( CG_PointContents( re->origin, -1 ) & CONTENTS_WATER ) {
		waterScale = 0.10f;
	}

	xvelocity[0] = velocity[0] * v[0][0] + velocity[1] * v[1][0] + velocity[2] * v[2][0];
	xvelocity[1] = velocity[0] * v[0][1] + velocity[1] * v[1][1] + velocity[2] * v[2][1];
	xvelocity[2] = velocity[0] * v[0][2] + velocity[1] * v[1][2] + velocity[2] * v[2][2];
	VectorScale( xvelocity, waterScale, le->pos.trDelta );

	AxisCopy( axisDefault, re->axis );
    re->shaderTime = cg.time / 1000.0f;
    re->reType = RT_SPRITE;
    re->radius = 0.25f;
	re->customShader = cgs.media.railRingsShader;
	le->bounceFactor = 0.3f;

#ifdef TA_WEAPSYS
	{
		// ZTM: NOTE: Can't get really flash color
		//                     as it is in weaponInfo_t
		vec3_t flashDlightColor = {0.6, 0.6, 1.0};
		re->shaderRGBA[0] = flashDlightColor[0] * 63;
		re->shaderRGBA[1] = flashDlightColor[1] * 63;
		re->shaderRGBA[2] = flashDlightColor[2] * 63;
		re->shaderRGBA[3] = 63;

		le->color[0] = flashDlightColor[0] * 0.2;
		le->color[1] = flashDlightColor[1] * 0.2;
		le->color[2] = flashDlightColor[2] * 0.2;
		le->color[3] = 0.25f;
	}
#else
    re->shaderRGBA[0] = wi->flashDlightColor[0] * 63;
    re->shaderRGBA[1] = wi->flashDlightColor[1] * 63;
    re->shaderRGBA[2] = wi->flashDlightColor[2] * 63;
    re->shaderRGBA[3] = 63;

    le->color[0] = wi->flashDlightColor[0] * 0.2;
    le->color[1] = wi->flashDlightColor[1] * 0.2;
    le->color[2] = wi->flashDlightColor[2] * 0.2;
    le->color[3] = 0.25f;
#endif

	le->angles.trType = TR_LINEAR;
	le->angles.trTime = cg.time;
	le->angles.trBase[0] = rand()&31;
	le->angles.trBase[1] = rand()&31;
	le->angles.trBase[2] = rand()&31;
	le->angles.trDelta[0] = 1;
	le->angles.trDelta[1] = 0.5;
	le->angles.trDelta[2] = 0;

}
/*
==========================
CG_GrappleTrail
==========================
*/
#ifdef TA_WEAPSYS
void CG_GrappleTrail( centity_t *ent, const projectileInfo_t *wi )
#else
void CG_GrappleTrail( centity_t *ent, const weaponInfo_t *wi )
#endif
{
	vec3_t	origin;
	entityState_t	*es;
#ifndef IOQ3ZTM
	vec3_t			forward, up;
#endif
	refEntity_t		beam;

	es = &ent->currentState;

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	ent->trailTime = cg.time;

	memset( &beam, 0, sizeof( beam ) );

#ifdef IOQ3ZTM
	VectorCopy(cg_entities[ ent->currentState.otherEntityNum ].pe.flashOrigin, beam.origin);
#else
	//FIXME adjust for muzzle position
	VectorCopy ( cg_entities[ ent->currentState.otherEntityNum ].lerpOrigin, beam.origin );
	beam.origin[2] += 26;
	AngleVectors( cg_entities[ ent->currentState.otherEntityNum ].lerpAngles, forward, NULL, up );
	VectorMA( beam.origin, -6, up, beam.origin );
#endif
	VectorCopy( origin, beam.oldorigin );

#ifndef IOQ3ZTM
	if (Distance( beam.origin, beam.oldorigin ) < 64 )
		return; // Don't draw if close
#endif

	beam.reType = RT_LIGHTNING;
#ifdef TA_WEAPSYS
	if (bg_projectileinfo[wi-cg_projectiles].trailType == PT_LIGHTNING)
	{
		beam.customShader = cgs.media.lightningShader;
	}
	else
	{
		beam.customShader = cgs.media.grappleCableShader;
	}
#else
	beam.customShader = cgs.media.lightningShader;
#endif

	AxisClear( beam.axis );
	beam.shaderRGBA[0] = 0xff;
	beam.shaderRGBA[1] = 0xff;
	beam.shaderRGBA[2] = 0xff;
	beam.shaderRGBA[3] = 0xff;
	trap_R_AddRefEntityToScene( &beam );
}

/*
==========================
CG_GrenadeTrail
==========================
*/
#ifdef TA_WEAPSYS
static void CG_GrenadeTrail( centity_t *ent, const projectileInfo_t *wi )
#else
static void CG_GrenadeTrail( centity_t *ent, const weaponInfo_t *wi )
#endif
{
	CG_RocketTrail( ent, wi );
}

#ifdef TA_WEAPSYS
/*
==========================
CG_SparkTrail
==========================
*/
static void CG_SparkTrail( centity_t *ent, const projectileInfo_t *wi )
{
	int		step;
	vec3_t	origin, lastPos;
	int		t;
	int		startTime, contents;
	int		lastContents;
	entityState_t	*es;
	vec3_t	up;
	localEntity_t	*smoke;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	up[0] = 0;
	up[1] = 0;
	up[2] = 0;

	step = 50;

	es = &ent->currentState;
	startTime = ent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	contents = CG_PointContents( origin, -1 );

	// if object (e.g. grenade) is stationary, don't toss up smoke
	if ( es->pos.trType == TR_STATIONARY ) {
		ent->trailTime = cg.time;
		return;
	}

	BG_EvaluateTrajectory( &es->pos, ent->trailTime, lastPos );
	lastContents = CG_PointContents( lastPos, -1 );

	ent->trailTime = cg.time;

	/*if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		if ( contents & lastContents & CONTENTS_WATER ) {
			CG_BubbleTrail( lastPos, origin, 8 );
		}
		return;
	}*/

	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up,
					  wi->trailRadius,
					  1, 1, 1, 0.33f,
					  wi->wiTrailTime,
					  t,
					  0,
					  0,
					  cgs.media.sparkTrailShader );
		// use the optimized local entity add
		smoke->leType = LE_SCALE_FADE;

		// ZTM: add dynamic light
		if ( wi->missileDlight*0.66f > 0 ) {
			smoke->light = wi->missileDlight*0.66f; // 2/3 size
			VectorCopy(wi->missileDlightColor, smoke->lightColor);
		}
	}

}
#endif

#ifdef TA_HOLDABLE
// Currently this should only be called by CG_RegisterItemVisuals ...
void CG_RegisterHoldable( int holdableNum )
{
	//CG_RegisterItemVisuals(BG_ItemNumForHoldableNum(holdableNum));

	switch (holdableNum)
	{
		case HI_SHURIKEN:
#ifdef TA_WEAPSYS
			CG_RegisterProjectile(BG_ProjectileIndexForName("p_shuriken"));
#else
			cgs.media.shurikenModel = trap_R_RegisterModel( "models/shurikens/shuriken.md3" );
#endif
			break;
		case HI_ELECTRICSHURIKEN:
#ifdef TA_WEAPSYS
			CG_RegisterProjectile(BG_ProjectileIndexForName("p_electricshuriken"));
#else
			cgs.media.shurikenElectricModel = trap_R_RegisterModel( "models/shurikens/shurikenelectric.md3" );
#endif
			break;
		case HI_FIRESHURIKEN:
#ifdef TA_WEAPSYS
			CG_RegisterProjectile(BG_ProjectileIndexForName("p_fireshuriken"));
#else
			cgs.media.shurikenFireModel = trap_R_RegisterModel( "models/shurikens/shurikenfire.md3" );
#endif
			break;
		case HI_LASERSHURIKEN:
#ifdef TA_WEAPSYS
			CG_RegisterProjectile(BG_ProjectileIndexForName("p_lasershuriken"));
#else
				cgs.media.shurikenLaserModel = trap_R_RegisterModel( "models/shurikens/shurikenlaser.md3" );
#endif
			break;
		default:
			break;
	}
}
#endif

#ifdef TA_WEAPSYS
void CG_RegisterProjectile( int projectileNum )
{
	projectileInfo_t *projectileInfo;
	bg_projectileinfo_t *bgProj;
	int i;

	projectileInfo = &cg_projectiles[projectileNum];
	bgProj = &bg_projectileinfo[projectileNum];
	if ( projectileInfo->registered ) {
		return;
	}

	memset( projectileInfo, 0, sizeof( *projectileInfo ) );
	projectileInfo->registered = qtrue;

	VectorCopy(bgProj->missileDlightColor, projectileInfo->missileDlightColor);
	projectileInfo->missileDlight = bgProj->missileDlight;

	if (bgProj->missileSoundName[0] != '\0')
		projectileInfo->missileSound = trap_S_RegisterSound( bgProj->missileSoundName, qfalse );
	for (i = 0; i< 3; i++)
	{
		if (bgProj->hitSoundName[i][0] != '\0')
			projectileInfo->hitSound[i] = trap_S_RegisterSound( bgProj->hitSoundName[i], qfalse );
	}
	if (bgProj->hitPlayerSoundName[0] != '\0')
		projectileInfo->hitPlayerSound = trap_S_RegisterSound( bgProj->hitPlayerSoundName, qfalse );
	if (bgProj->hitMetalSoundName[0] != '\0')
		projectileInfo->hitMetalSound = trap_S_RegisterSound( bgProj->hitMetalSoundName, qfalse );
	for (i = 0; i< 2; i++)
	{
		if (bgProj->bounceSoundName[i][0] != '\0')
			projectileInfo->bounceSound[i] = trap_S_RegisterSound( bgProj->bounceSoundName[i], qfalse );
	}

	if (bgProj->model[0] != '\0')
	{
		projectileInfo->missileModel = trap_R_RegisterModel(bgProj->model);
		// Use normal model if blue/red versions are not set.
		projectileInfo->missileModelBlue = projectileInfo->missileModel;
		projectileInfo->missileModelRed = projectileInfo->missileModel;
	}
	if (bgProj->modelBlue[0] != '\0')
		projectileInfo->missileModelBlue = trap_R_RegisterModel(bgProj->modelBlue);
	if (bgProj->modelRed[0] != '\0')
		projectileInfo->missileModelRed = trap_R_RegisterModel(bgProj->modelRed);
	if (bgProj->sprite[0] != '\0')
		projectileInfo->spriteShader = trap_R_RegisterShader(bgProj->sprite);
	projectileInfo->spriteRadius = bgProj->spriteRadius;
	if (bgProj->wallmarkName[0] != '\0')
		projectileInfo->wallmarkShader = trap_R_RegisterShader(bgProj->wallmarkName);
	projectileInfo->wallmarkRadius = bgProj->wallmarkRadius;
	switch (bgProj->trailType)
	{
		default:
		case PT_NONE:
			break;

		case PT_PLASMA:
			cgs.media.railRingsShader = trap_R_RegisterShader( "railDisc" );
			projectileInfo->missileTrailFunc = CG_PlasmaTrail;
			break;
		case PT_ROCKET:
			// Uses cgs.media.smokePuffShader but it is used by other
			//    stuff so it is always loaded.
			projectileInfo->missileTrailFunc = CG_RocketTrail;
			projectileInfo->wiTrailTime = 2000;
			projectileInfo->trailRadius = 64;
			break;
		case PT_GRENADE:
			// Uses CG_RocketTrail ... so don't need to load
			//    cgs.media.smokePuffShader
			projectileInfo->missileTrailFunc = CG_GrenadeTrail;
			projectileInfo->wiTrailTime = 700;
			projectileInfo->trailRadius = 32;
			break;
		case PT_GRAPPLE:
			projectileInfo->missileTrailFunc = CG_GrappleTrail;
			cgs.media.grappleCableShader = trap_R_RegisterShader( "GrappleCable");
#if !defined TURTLEARENA || defined TA_SUPPORTQ3
			if (!cgs.media.grappleCableShader)
				cgs.media.grappleCableShader = trap_R_RegisterShader( "lightningBoltNew");
#endif
			break;
		case PT_NAIL:
#ifdef MISSIONPACK
			cgs.media.nailPuffShader = trap_R_RegisterShader( "nailtrail" );
			projectileInfo->missileTrailFunc = CG_NailTrail;
			projectileInfo->trailRadius = 16;
			projectileInfo->wiTrailTime = 250;
#else // Quake3 can use rocket trail instead...
			projectileInfo->missileTrailFunc = CG_RocketTrail;
			projectileInfo->trailRadius = 16;
			projectileInfo->wiTrailTime = 250;
#endif
			break;
		case PT_LIGHTNING:
			cgs.media.lightningShader = trap_R_RegisterShader( "lightningBoltNew");
			projectileInfo->missileTrailFunc = CG_GrappleTrail;
			break;
		case PT_RAIL:
			cgs.media.railRingsShader = trap_R_RegisterShader( "railDisc" );
			cgs.media.railCoreShader = trap_R_RegisterShader( "railCore" );
			break;
		case PT_BULLET:
			break;
		case PT_SPARKS:
			cgs.media.sparkTrailShader = trap_R_RegisterShader( "sparkTrail" );
			projectileInfo->missileTrailFunc = CG_SparkTrail;
			projectileInfo->wiTrailTime = 300;
			projectileInfo->trailRadius = 16;
			break;
	}

	switch (bgProj->deathType)
	{
		default:
		case PD_NONE:
			break;
		case PD_PLASMA:
			cgs.media.ringFlashModel = trap_R_RegisterModel("models/weaphits/ring02.md3");
			cgs.media.plasmaExplosionShader = trap_R_RegisterShader( "plasmaExplosion" );
			cgs.media.sfx_plasmaexp = trap_S_RegisterSound ("sound/weapons/plasma/plasmx1a.wav", qfalse);
			break;
		case PD_ROCKET:
		case PD_ROCKET_SMALL:
			cgs.media.dishFlashModel = trap_R_RegisterModel("models/weaphits/boom01.md3");
			cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
			cgs.media.sfx_rockexp = trap_S_RegisterSound ("sound/weapons/rocket/rocklx1a.wav", qfalse);
			break;
		case PD_BULLET:
			cgs.media.bulletFlashModel = trap_R_RegisterModel("models/weaphits/bullet.md3");
			cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );
			cgs.media.sfx_ric1 = trap_S_RegisterSound ("sound/weapons/machinegun/ric1.wav", qfalse);
			cgs.media.sfx_ric2 = trap_S_RegisterSound ("sound/weapons/machinegun/ric2.wav", qfalse);
			cgs.media.sfx_ric3 = trap_S_RegisterSound ("sound/weapons/machinegun/ric3.wav", qfalse);
			break;
		case PD_GRENADE:
			cgs.media.dishFlashModel = trap_R_RegisterModel("models/weaphits/boom01.md3");
#ifdef TA_DATA
			cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
#else
			cgs.media.grenadeExplosionShader = trap_R_RegisterShader( "grenadeExplosion" );
#endif
			break;
		case PD_RAIL:
			cgs.media.ringFlashModel = trap_R_RegisterModel("models/weaphits/ring02.md3");
#ifdef TA_DATA
			cgs.media.plasmaExplosionShader = trap_R_RegisterShader( "plasmaExplosion" );
#else
			cgs.media.railExplosionShader = trap_R_RegisterShader( "railExplosion" );
#endif
			break;
		case PD_BFG:
			cgs.media.dishFlashModel = trap_R_RegisterModel("models/weaphits/boom01.md3");
#ifdef TA_DATA
			cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
#else
			cgs.media.bfgExplosionShader = trap_R_RegisterShader( "bfgExplosion" );
#endif
			break;
		case PD_LIGHTNING:
			cgs.media.lightningExplosionModel = trap_R_RegisterModel( "models/weaphits/crackle.md3" );
			cgs.media.sfx_lghit1 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit.wav", qfalse );
			cgs.media.sfx_lghit2 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit2.wav", qfalse );
			cgs.media.sfx_lghit3 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit3.wav", qfalse );
			break;
	}
}

void CG_RegisterWeapon( int weaponNum )
{
	weaponInfo_t *weaponInfo;
	bg_weaponinfo_t *weap;
	char			path[MAX_QPATH];

	weaponInfo = &cg_weapons[weaponNum];
	weap = &bg_weaponinfo[weaponNum];

	if ( weaponInfo->registered ) {
		return;
	}

	memset( weaponInfo, 0, sizeof( *weaponInfo ) );
	weaponInfo->registered = qtrue;

	if (weap->model[0] != '\0') {
		weaponInfo->weaponModel = trap_R_RegisterModel( weap->model );

		strcpy( path, weap->model );
		COM_StripExtension(path, path, sizeof(path));
		strcat( path, "_flash.md3" );
		weaponInfo->flashModel = trap_R_RegisterModel( path );

		strcpy( path, weap->model );
		COM_StripExtension(path, path, sizeof(path));
		strcat( path, "_barrel.md3" );
		weaponInfo->barrelModel = trap_R_RegisterModel( path );
	}

	VectorCopy(weap->flashColor, weaponInfo->flashDlightColor);

	// wallmark
	if (weap->wallmarkName[0] != '\0')
		weaponInfo->wallmarkShader = trap_R_RegisterShader(weap->wallmarkName);
	weaponInfo->wallmarkRadius = weap->wallmarkRadius;

	// Sounds
	if (weap->readySoundName[0] != '\0')
		weaponInfo->readySound = trap_S_RegisterSound( weap->readySoundName, qfalse );
	if (weap->firingSoundName[0] != '\0')
		weaponInfo->firingSound = trap_S_RegisterSound( weap->firingSoundName, qfalse );
	if (weap->flashSoundName[0][0] != '\0')
		weaponInfo->flashSound[0] = trap_S_RegisterSound( weap->flashSoundName[0], qfalse );
	if (weap->flashSoundName[1][0] != '\0')
		weaponInfo->flashSound[1] = trap_S_RegisterSound( weap->flashSoundName[1], qfalse );
	if (weap->flashSoundName[2][0] != '\0')
		weaponInfo->flashSound[2] = trap_S_RegisterSound( weap->flashSoundName[2], qfalse );
	if (weap->flashSoundName[3][0] != '\0')
		weaponInfo->flashSound[3] = trap_S_RegisterSound( weap->flashSoundName[3], qfalse );
	if (weap->firingStoppedSoundName[0] != '\0')
		weaponInfo->firingStoppedSound = trap_S_RegisterSound( weap->firingStoppedSoundName, qfalse );


	// \unused
	//weaponInfo->ejectBrassFunc = NULL;

	CG_RegisterProjectile(weap->projnum);
}
#endif

/*
=================
CG_RegisterWeapon

The server says this item is used on this level
=================
*/
#ifdef TA_WEAPSYS
void CG_RegisterWeaponGroup( int weaponNum )
#else
void CG_RegisterWeapon( int weaponNum )
#endif
{
#ifdef TA_WEAPSYS
	weaponGroupInfo_t	*weaponInfo;
#else
	weaponInfo_t	*weaponInfo;
#endif
#ifdef TURTLEARENA // WEAPONS
	gitem_t			*item;
#else
	gitem_t			*item, *ammo;
#endif
	char			path[MAX_QPATH];
	vec3_t			mins, maxs;
	int				i;

#ifdef TA_WEAPSYS
	weaponInfo = &cg_weapongroups[weaponNum];
#else
	weaponInfo = &cg_weapons[weaponNum];
#endif

#ifdef TA_WEAPSYS // Safety check
	if ( weaponNum <= 0 || weaponNum >= BG_NumWeaponGroups() ) {
		return;
	}
#else
	if ( weaponNum == 0 ) {
		return;
	}
#endif

	if ( weaponInfo->registered ) {
		return;
	}

	memset( weaponInfo, 0, sizeof( *weaponInfo ) );
	weaponInfo->registered = qtrue;

#ifdef TA_WEAPSYS
	item = BG_ItemForItemNum(0);
	for (i = BG_NumItems()-1; i > 0; i--)
	{
		item = BG_ItemForItemNum(i);
		if (!item->classname)
			continue;
		if ( item->giType == IT_WEAPON && item->giTag == weaponNum ) {
			break;
		}
	}
#else
	for ( item = bg_itemlist + 1 ; item->classname ; item++ ) {
		if ( item->giType == IT_WEAPON && item->giTag == weaponNum ) {
			weaponInfo->item = item;
			break;
		}
	}

	if ( !item->classname ) {
		CG_Error( "Couldn't find weapon %i", weaponNum );
	}
#ifdef IOQ3ZTM
	CG_RegisterItemVisuals( ITEM_INDEX(item) );
#else
	CG_RegisterItemVisuals( item - bg_itemlist );
#endif
#endif

	// load cmodel before model so filecache works
#ifdef TA_WEAPSYS
	if ( item->classname )
	{
		weaponInfo->weaponModel = trap_R_RegisterModel( item->world_model[0] );

		// calc midpoint for rotation
		trap_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
		for ( i = 0 ; i < 3 ; i++ ) {
			weaponInfo->weaponMidpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
		}
	}
#else
	weaponInfo->weaponModel = trap_R_RegisterModel( item->world_model[0] );

	// calc midpoint for rotation
	trap_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
	for ( i = 0 ; i < 3 ; i++ ) {
		weaponInfo->weaponMidpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
	}
#endif

#ifdef TA_WEAPSYS
	weaponInfo->weaponIcon = trap_R_RegisterShader( bg_weapongroupinfo[weaponNum].iconName );
#else
	weaponInfo->weaponIcon = trap_R_RegisterShader( item->icon );
#endif
#ifndef TURTLEARENA // WEAPONS
	weaponInfo->ammoIcon = trap_R_RegisterShader( item->icon );

#ifdef TA_WEAPSYS
	ammo = NULL; // stop warning
	for (i = BG_NumItems()-1; i > 0; i--)
	{
		ammo = BG_ItemForItemNum(i);
		if (!ammo->classname)
			continue;
		if ( ammo->giType == IT_AMMO && ammo->giTag == weaponNum ) {
			break;
		}
	}
#else
	for ( ammo = bg_itemlist + 1 ; ammo->classname ; ammo++ ) {
		if ( ammo->giType == IT_AMMO && ammo->giTag == weaponNum ) {
			break;
		}
	}
#endif

	if (
#ifdef TA_WEAPSYS // stop warning
	ammo &&
#endif
	ammo->classname && ammo->world_model[0] ) {
		weaponInfo->ammoModel = trap_R_RegisterModel( ammo->world_model[0] );
	}
#endif

#ifndef TA_WEAPSYS
	strcpy( path, item->world_model[0] );
	COM_StripExtension(path, path, sizeof(path));
	strcat( path, "_flash.md3" );
	weaponInfo->flashModel = trap_R_RegisterModel( path );

	strcpy( path, item->world_model[0] );
	COM_StripExtension(path, path, sizeof(path));
	strcat( path, "_barrel.md3" );
	weaponInfo->barrelModel = trap_R_RegisterModel( path );
#endif

#ifdef TA_WEAPSYS
	if (item)
	{
#endif
	strcpy( path, item->world_model[0] );
	COM_StripExtension(path, path, sizeof(path));
	strcat( path, "_hand.md3" );
	weaponInfo->handsModel = trap_R_RegisterModel( path );
#ifdef TA_WEAPSYS
	}
#endif

	if ( !weaponInfo->handsModel ) {
#ifdef TA_DATA
		weaponInfo->handsModel = trap_R_RegisterModel( "models/weapons2/gun/gun_hand.md3" );
#else
		weaponInfo->handsModel = trap_R_RegisterModel( "models/weapons2/shotgun/shotgun_hand.md3" );
#endif
	}

#ifndef IOQ3ZTM // unused
	weaponInfo->loopFireSound = qfalse;
#endif

#ifdef TA_WEAPSYS
	CG_RegisterWeapon(bg_weapongroupinfo[weaponNum].weaponnum[0]);
	CG_RegisterWeapon(bg_weapongroupinfo[weaponNum].weaponnum[1]);
#else
	switch ( weaponNum ) {
	case WP_GAUNTLET:
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/melee/fstrun.wav", qfalse );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/melee/fstatck.wav", qfalse );
		break;

	case WP_LIGHTNING:
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/melee/fsthum.wav", qfalse );
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/lightning/lg_hum.wav", qfalse );

		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/lightning/lg_fire.wav", qfalse );
		cgs.media.lightningShader = trap_R_RegisterShader( "lightningBoltNew");
		cgs.media.lightningExplosionModel = trap_R_RegisterModel( "models/weaphits/crackle.md3" );
		cgs.media.sfx_lghit1 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit.wav", qfalse );
		cgs.media.sfx_lghit2 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit2.wav", qfalse );
		cgs.media.sfx_lghit3 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit3.wav", qfalse );
		break;

	case WP_GRAPPLING_HOOK:
#ifdef IOQ3ZTM // IOQ3BUGFIX: Load trail shader
		cgs.media.lightningShader = trap_R_RegisterShader( "lightningBoltNew");
#endif
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->missileModel = trap_R_RegisterModel( "models/ammo/rocket/rocket.md3" );
		weaponInfo->missileTrailFunc = CG_GrappleTrail;
		weaponInfo->missileDlight = 200;
#ifndef IOQ3ZTM // unused
		weaponInfo->wiTrailTime = 2000;
		weaponInfo->trailRadius = 64;
#endif
		MAKERGB( weaponInfo->missileDlightColor, 1, 0.75f, 0 );
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/melee/fsthum.wav", qfalse );
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/melee/fstrun.wav", qfalse );
		break;

#ifdef MISSIONPACK
	case WP_CHAINGUN:
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/vulcan/wvulfire.wav", qfalse );
#ifndef IOQ3ZTM // unused
		weaponInfo->loopFireSound = qtrue;
#endif
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf1b.wav", qfalse );
		weaponInfo->flashSound[1] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf2b.wav", qfalse );
		weaponInfo->flashSound[2] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf3b.wav", qfalse );
		weaponInfo->flashSound[3] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf4b.wav", qfalse );
		weaponInfo->ejectBrassFunc = CG_MachineGunEjectBrass;
		cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );
		break;
#endif

	case WP_MACHINEGUN:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf1b.wav", qfalse );
		weaponInfo->flashSound[1] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf2b.wav", qfalse );
		weaponInfo->flashSound[2] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf3b.wav", qfalse );
		weaponInfo->flashSound[3] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf4b.wav", qfalse );
		weaponInfo->ejectBrassFunc = CG_MachineGunEjectBrass;
		cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );
		break;

	case WP_SHOTGUN:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/shotgun/sshotf1b.wav", qfalse );
		weaponInfo->ejectBrassFunc = CG_ShotgunEjectBrass;
		break;

	case WP_ROCKET_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/ammo/rocket/rocket.md3" );
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/rocket/rockfly.wav", qfalse );
		weaponInfo->missileTrailFunc = CG_RocketTrail;
		weaponInfo->missileDlight = 200;
		weaponInfo->wiTrailTime = 2000;
		weaponInfo->trailRadius = 64;
		
		MAKERGB( weaponInfo->missileDlightColor, 1, 0.75f, 0 );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.75f, 0 );

		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/rocket/rocklf1a.wav", qfalse );
		cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
		break;

#ifdef MISSIONPACK
	case WP_PROX_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/proxmine.md3" );
		weaponInfo->missileTrailFunc = CG_GrenadeTrail;
		weaponInfo->wiTrailTime = 700;
		weaponInfo->trailRadius = 32;
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.70f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/proxmine/wstbfire.wav", qfalse );
		cgs.media.grenadeExplosionShader = trap_R_RegisterShader( "grenadeExplosion" );
		break;
#endif

	case WP_GRENADE_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/ammo/grenade1.md3" );
		weaponInfo->missileTrailFunc = CG_GrenadeTrail;
		weaponInfo->wiTrailTime = 700;
		weaponInfo->trailRadius = 32;
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.70f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/grenade/grenlf1a.wav", qfalse );
		cgs.media.grenadeExplosionShader = trap_R_RegisterShader( "grenadeExplosion" );
		break;

#ifdef MISSIONPACK
	case WP_NAILGUN:
		weaponInfo->ejectBrassFunc = CG_NailgunEjectBrass;
		weaponInfo->missileTrailFunc = CG_NailTrail;
//		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/nailgun/wnalflit.wav", qfalse );
		weaponInfo->trailRadius = 16;
		weaponInfo->wiTrailTime = 250;
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/nail.md3" );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.75f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/nailgun/wnalfire.wav", qfalse );
		break;
#endif

	case WP_PLASMAGUN:
//		weaponInfo->missileModel = cgs.media.invulnerabilityPowerupModel;
		weaponInfo->missileTrailFunc = CG_PlasmaTrail;
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/plasma/lasfly.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/plasma/hyprbf1a.wav", qfalse );
		cgs.media.plasmaExplosionShader = trap_R_RegisterShader( "plasmaExplosion" );
		cgs.media.railRingsShader = trap_R_RegisterShader( "railDisc" );
		break;

	case WP_RAILGUN:
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/railgun/rg_hum.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.5f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/railgun/railgf1a.wav", qfalse );
		cgs.media.railExplosionShader = trap_R_RegisterShader( "railExplosion" );
		cgs.media.railRingsShader = trap_R_RegisterShader( "railDisc" );
		cgs.media.railCoreShader = trap_R_RegisterShader( "railCore" );
		break;

	case WP_BFG:
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/bfg/bfg_hum.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.7f, 1 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/bfg/bfg_fire.wav", qfalse );
		cgs.media.bfgExplosionShader = trap_R_RegisterShader( "bfgExplosion" );
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/bfg.md3" );
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/rocket/rockfly.wav", qfalse );
		break;

	 default:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 1 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/rocket/rocklf1a.wav", qfalse );
		break;
	}
#endif
}

/*
=================
CG_RegisterItemVisuals

The server says this item is used on this level
=================
*/
void CG_RegisterItemVisuals( int itemNum ) {
	itemInfo_t		*itemInfo;
	gitem_t			*item;

#ifdef TA_WEAPSYS
	if ( itemNum < 0 || itemNum >= BG_NumItems() ) {
		CG_Error( "CG_RegisterItemVisuals: itemNum %d out of range [0-%d]", itemNum, BG_NumItems()-1 );
	}
#else
	if ( itemNum < 0 || itemNum >= bg_numItems ) {
		CG_Error( "CG_RegisterItemVisuals: itemNum %d out of range [0-%d]", itemNum, bg_numItems-1 );
	}
#endif

	itemInfo = &cg_items[ itemNum ];
	if ( itemInfo->registered ) {
		return;
	}

#ifdef TA_WEAPSYS
	item = BG_ItemForItemNum(itemNum);
#else
	item = &bg_itemlist[ itemNum ];
#endif

	memset( itemInfo, 0, sizeof( &itemInfo ) );
	itemInfo->registered = qtrue;

	itemInfo->models[0] = trap_R_RegisterModel( item->world_model[0] );

	itemInfo->icon = trap_R_RegisterShader( item->icon );

#ifdef IOQ3ZTM // FLAG_MODEL
	if (item->skin && item->skin[0]) {
		itemInfo->skin = trap_R_RegisterSkin( item->skin );
	}
#endif

	if ( item->giType == IT_WEAPON ) {
#ifdef TA_WEAPSYS
		CG_RegisterWeaponGroup( item->giTag );
#else
		CG_RegisterWeapon( item->giTag );
#endif
	}
#ifdef TA_HOLDABLE
	if ( item->giType == IT_HOLDABLE ) {
		CG_RegisterHoldable( item->giTag );
	}
	// ZTM: Cache shurikens
	if (item->giType == IT_PERSISTANT_POWERUP
		&& item->giTag == PW_AMMOREGEN)
	{
		CG_RegisterItemVisuals(BG_ItemNumForHoldableNum(HI_SHURIKEN));
		CG_RegisterItemVisuals(BG_ItemNumForHoldableNum(HI_ELECTRICSHURIKEN));
		CG_RegisterItemVisuals(BG_ItemNumForHoldableNum(HI_FIRESHURIKEN));
		CG_RegisterItemVisuals(BG_ItemNumForHoldableNum(HI_LASERSHURIKEN));
	}
#endif

#ifdef TA_DATA // FLAG_MODEL
	if ( item->giType == IT_TEAM )
	{
		if ( item->world_model[1] ) {
			itemInfo->models[1] = trap_R_RegisterModel( item->world_model[1] );
		}
	}
#endif
#ifndef TA_DATA
	//
	// powerups have an accompanying ring or sphere
	//
	if ( item->giType == IT_POWERUP || item->giType == IT_HEALTH || 
#ifndef TURTLEARENA // NOARMOR
		item->giType == IT_ARMOR ||
#endif
		item->giType == IT_HOLDABLE ) {
		if ( item->world_model[1] ) {
			itemInfo->models[1] = trap_R_RegisterModel( item->world_model[1] );
		}
	}
#endif
}


/*
========================================================================================

VIEW WEAPON

========================================================================================
*/

/*
=================
CG_MapTorsoToWeaponFrame

=================
*/
static int CG_MapTorsoToWeaponFrame( clientInfo_t *ci, int frame ) {

#ifdef TA_PLAYERSYS
	// change weapon
	if ( frame >= ci->playercfg.animations[TORSO_DROP].firstFrame
		&& frame < ci->playercfg.animations[TORSO_DROP].firstFrame + 9 ) {
		return frame - ci->playercfg.animations[TORSO_DROP].firstFrame + 6;
	}

	// stand attack
	if ( frame >= ci->playercfg.animations[TORSO_ATTACK].firstFrame
		&& frame < ci->playercfg.animations[TORSO_ATTACK].firstFrame + 6 ) {
		return 1 + frame - ci->playercfg.animations[TORSO_ATTACK].firstFrame;
	}

	// stand attack 2
	if ( frame >= ci->playercfg.animations[TORSO_ATTACK2].firstFrame
		&& frame < ci->playercfg.animations[TORSO_ATTACK2].firstFrame + 6 ) {
		return 1 + frame - ci->playercfg.animations[TORSO_ATTACK2].firstFrame;
	}
#else
	// change weapon
	if ( frame >= ci->animations[TORSO_DROP].firstFrame 
		&& frame < ci->animations[TORSO_DROP].firstFrame + 9 ) {
		return frame - ci->animations[TORSO_DROP].firstFrame + 6;
	}

	// stand attack
	if ( frame >= ci->animations[TORSO_ATTACK].firstFrame 
		&& frame < ci->animations[TORSO_ATTACK].firstFrame + 6 ) {
		return 1 + frame - ci->animations[TORSO_ATTACK].firstFrame;
	}

	// stand attack 2
	if ( frame >= ci->animations[TORSO_ATTACK2].firstFrame 
		&& frame < ci->animations[TORSO_ATTACK2].firstFrame + 6 ) {
		return 1 + frame - ci->animations[TORSO_ATTACK2].firstFrame;
	}
#endif
	
	return 0;
}


/*
==============
CG_CalculateWeaponPosition
==============
*/
static void CG_CalculateWeaponPosition( vec3_t origin, vec3_t angles ) {
	float	scale;
	int		delta;
	float	fracsin;

	VectorCopy( cg.refdef.vieworg, origin );
	VectorCopy( cg.refdefViewAngles, angles );

	// on odd legs, invert some angles
	if ( cg.bobcycle & 1 ) {
		scale = -cg.xyspeed;
	} else {
		scale = cg.xyspeed;
	}

	// gun angles from bobbing
	angles[ROLL] += scale * cg.bobfracsin * 0.005;
	angles[YAW] += scale * cg.bobfracsin * 0.01;
	angles[PITCH] += cg.xyspeed * cg.bobfracsin * 0.005;

	// drop the weapon when landing
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		origin[2] += cg.landChange*0.25 * delta / LAND_DEFLECT_TIME;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		origin[2] += cg.landChange*0.25 * 
			(LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta) / LAND_RETURN_TIME;
	}

#if 0
	// drop the weapon when stair climbing
	delta = cg.time - cg.stepTime;
	if ( delta < STEP_TIME/2 ) {
		origin[2] -= cg.stepChange*0.25 * delta / (STEP_TIME/2);
	} else if ( delta < STEP_TIME ) {
		origin[2] -= cg.stepChange*0.25 * (STEP_TIME - delta) / (STEP_TIME/2);
	}
#endif

	// idle drift
	scale = cg.xyspeed + 40;
	fracsin = sin( cg.time * 0.001 );
	angles[ROLL] += scale * fracsin * 0.01;
	angles[YAW] += scale * fracsin * 0.01;
	angles[PITCH] += scale * fracsin * 0.01;
}


/*
===============
CG_LightningBolt

Origin will be the exact tag point, which is slightly
different than the muzzle point used for determining hits.
The cent should be the non-predicted cent if it is from the player,
so the endpoint will reflect the simulated strike (lagging the predicted
angle)
===============
*/
static void CG_LightningBolt( centity_t *cent, vec3_t origin
#ifdef TA_WEAPSYS
	, int range
#endif
	)
{
	trace_t  trace;
	refEntity_t  beam;
	vec3_t   forward;
	vec3_t   muzzlePoint, endPoint;
#ifdef IOQ3ZTM
	int anim;
#endif

#ifndef TA_WEAPSYS
	if (cent->currentState.weapon != WP_LIGHTNING) {
		return;
	}
#endif

	memset( &beam, 0, sizeof( beam ) );

	// CPMA  "true" lightning
	if ((cent->currentState.number == cg.predictedPlayerState.clientNum) && (cg_trueLightning.value != 0)) {
		vec3_t angle;
		int i;

		for (i = 0; i < 3; i++) {
			float a = cent->lerpAngles[i] - cg.refdefViewAngles[i];
			if (a > 180) {
				a -= 360;
			}
			if (a < -180) {
				a += 360;
			}

			angle[i] = cg.refdefViewAngles[i] + a * (1.0 - cg_trueLightning.value);
			if (angle[i] < 0) {
				angle[i] += 360;
			}
			if (angle[i] > 360) {
				angle[i] -= 360;
			}
		}

		AngleVectors(angle, forward, NULL, NULL );
		VectorCopy(cent->lerpOrigin, muzzlePoint );
//		VectorCopy(cg.refdef.vieworg, muzzlePoint );
	} else {
		// !CPMA
		AngleVectors( cent->lerpAngles, forward, NULL, NULL );
		VectorCopy(cent->lerpOrigin, muzzlePoint );
	}

#ifdef IOQ3ZTM // IOQ3BUGFIX: Copied from CG_CalcMuzzlePoint
	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;
	if ( anim == LEGS_WALKCR || anim == LEGS_IDLECR ) {
		muzzlePoint[2] += CROUCH_VIEWHEIGHT;
	} else {
		muzzlePoint[2] += DEFAULT_VIEWHEIGHT;
	}
#else
	// FIXME: crouch
	muzzlePoint[2] += DEFAULT_VIEWHEIGHT;
#endif

	VectorMA( muzzlePoint, 14, forward, muzzlePoint );

	// project forward by the lightning range
#ifdef TA_WEAPSYS
	VectorMA( muzzlePoint, range, forward, endPoint );
#else
	VectorMA( muzzlePoint, LIGHTNING_RANGE, forward, endPoint );
#endif

	// see if it hit a wall
	CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint, 
		cent->currentState.number, MASK_SHOT );

	// this is the endpoint
	VectorCopy( trace.endpos, beam.oldorigin );

	// use the provided origin, even though it may be slightly
	// different than the muzzle origin
	VectorCopy( origin, beam.origin );

	beam.reType = RT_LIGHTNING;
	beam.customShader = cgs.media.lightningShader;
	trap_R_AddRefEntityToScene( &beam );

	// add the impact flare if it hit something
	if ( trace.fraction < 1.0 ) {
#ifdef TA_WEAPSYS // ZTM: FIXME: Support Q3 lightning gun
		// PT_LIGHTNING
		// CG_MissileHitWall (Exposion only, no sounds or wallmarks.)
#else
		vec3_t	angles;
		vec3_t	dir;

		VectorSubtract( beam.oldorigin, beam.origin, dir );
		VectorNormalize( dir );

		memset( &beam, 0, sizeof( beam ) );
		beam.hModel = cgs.media.lightningExplosionModel;

		VectorMA( trace.endpos, -16, dir, beam.origin );

		// make a random orientation
		angles[0] = rand() % 360;
		angles[1] = rand() % 360;
		angles[2] = rand() % 360;
		AnglesToAxis( angles, beam.axis );
		trap_R_AddRefEntityToScene( &beam );
#endif
	}
}
/*

static void CG_LightningBolt( centity_t *cent, vec3_t origin ) {
	trace_t		trace;
	refEntity_t		beam;
	vec3_t			forward;
	vec3_t			muzzlePoint, endPoint;

	if ( cent->currentState.weapon != WP_LIGHTNING ) {
		return;
	}

	memset( &beam, 0, sizeof( beam ) );

	// find muzzle point for this frame
	VectorCopy( cent->lerpOrigin, muzzlePoint );
	AngleVectors( cent->lerpAngles, forward, NULL, NULL );

	// FIXME: crouch
	muzzlePoint[2] += DEFAULT_VIEWHEIGHT;

	VectorMA( muzzlePoint, 14, forward, muzzlePoint );

	// project forward by the lightning range
	VectorMA( muzzlePoint, LIGHTNING_RANGE, forward, endPoint );

	// see if it hit a wall
	CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint, 
		cent->currentState.number, MASK_SHOT );

	// this is the endpoint
	VectorCopy( trace.endpos, beam.oldorigin );

	// use the provided origin, even though it may be slightly
	// different than the muzzle origin
	VectorCopy( origin, beam.origin );

	beam.reType = RT_LIGHTNING;
	beam.customShader = cgs.media.lightningShader;
	trap_R_AddRefEntityToScene( &beam );

	// add the impact flare if it hit something
	if ( trace.fraction < 1.0 ) {
		vec3_t	angles;
		vec3_t	dir;

		VectorSubtract( beam.oldorigin, beam.origin, dir );
		VectorNormalize( dir );

		memset( &beam, 0, sizeof( beam ) );
		beam.hModel = cgs.media.lightningExplosionModel;

		VectorMA( trace.endpos, -16, dir, beam.origin );

		// make a random orientation
		angles[0] = rand() % 360;
		angles[1] = rand() % 360;
		angles[2] = rand() % 360;
		AnglesToAxis( angles, beam.axis );
		trap_R_AddRefEntityToScene( &beam );
	}
}
*/

/*
===============
CG_SpawnRailTrail

Origin will be the exact tag point, which is slightly
different than the muzzle point used for determining hits.
===============
*/
static void CG_SpawnRailTrail( centity_t *cent, vec3_t origin ) {
	clientInfo_t	*ci;

#ifndef TA_WEAPSYS
	if ( cent->currentState.weapon != WP_RAILGUN ) {
		return;
	}
#endif
	if ( !cent->pe.railgunFlash ) {
		return;
	}
	cent->pe.railgunFlash = qtrue;
	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
	CG_RailTrail( ci, origin, cent->pe.railgunImpact );
}


/*
======================
CG_MachinegunSpinAngle
======================
*/
#define		SPIN_SPEED	0.9
#define		COAST_TIME	1000
static float	CG_MachinegunSpinAngle( centity_t *cent ) {
	int		delta;
	float	angle;
	float	speed;

	delta = cg.time - cent->pe.barrelTime;
	if ( cent->pe.barrelSpinning ) {
		angle = cent->pe.barrelAngle + delta * SPIN_SPEED;
	} else {
		if ( delta > COAST_TIME ) {
			delta = COAST_TIME;
		}

		speed = 0.5 * ( SPIN_SPEED + (float)( COAST_TIME - delta ) / COAST_TIME );
		angle = cent->pe.barrelAngle + delta * speed;
	}

	if ( cent->pe.barrelSpinning == !(cent->currentState.eFlags & EF_FIRING) ) {
		cent->pe.barrelTime = cg.time;
		cent->pe.barrelAngle = AngleMod( angle );
		cent->pe.barrelSpinning = !!(cent->currentState.eFlags & EF_FIRING);
#ifdef TA_WEAPSYS
		// Stopped firing, play sound
		if ( !cent->pe.barrelSpinning ) {
			trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, cg_weapons[bg_weapongroupinfo[cent->currentState.weapon].weaponnum[0]].firingStoppedSound );
		}
#else
#ifdef MISSIONPACK
		if ( cent->currentState.weapon == WP_CHAINGUN && !cent->pe.barrelSpinning ) {
			trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, trap_S_RegisterSound( "sound/weapons/vulcan/wvulwind.wav", qfalse ) );
		}
#endif
#endif
	}

	return angle;
}


/*
========================
CG_AddWeaponWithPowerups
========================
*/
static void CG_AddWeaponWithPowerups( refEntity_t *gun, int powerups )
{
	// add powerup effects
#ifdef TURTLEARENA // POWERS
	if ( powerups & ( 1 << PW_FLASHING ) ) {
		gun->renderfx |= RF_FORCE_ENT_ALPHA;
		gun->shaderRGBA[3] = 32;
		trap_R_AddRefEntityToScene( gun );

		gun->shaderRGBA[3] = 64;
		gun->customShader = cgs.media.playerTeleportShader;
		trap_R_AddRefEntityToScene( gun );
	} else
#endif
	if ( powerups & ( 1 << PW_INVIS ) ) {
		gun->customShader = cgs.media.invisShader;
		trap_R_AddRefEntityToScene( gun );
	} else {
		trap_R_AddRefEntityToScene( gun );

#ifndef TURTLEARENA // POWERS
		if ( powerups & ( 1 << PW_BATTLESUIT ) ) {
			gun->customShader = cgs.media.battleWeaponShader;
			trap_R_AddRefEntityToScene( gun );
		}
		if ( powerups & ( 1 << PW_QUAD ) ) {
			gun->customShader = cgs.media.quadWeaponShader;
			trap_R_AddRefEntityToScene( gun );
		}
#endif
	}
}



#ifdef TA_MISC // GHOST
/*
=============
CG_GhostRefEntity
=============
*/
localEntity_t *CG_GhostRefEntity(refEntity_t *refEnt, int timetolive, int alpha)
{
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	le->leFlags = LEF_PUFF_DONT_SCALE;
	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = cg.time;
	le->endTime = cg.time + timetolive;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	le->refEntity = *refEnt;
	re = &le->refEntity;

	re->shaderTime = cg.time / 1000.0f;

	re->radius = 5;
	//re->customShader = 0;
	re->shaderRGBA[0] = 0xff;
	re->shaderRGBA[1] = 0xff;
	re->shaderRGBA[2] = 0xff;
	re->shaderRGBA[3] = alpha;
	re->renderfx |= RF_FORCE_ENT_ALPHA;

	le->color[3] = alpha / 0xff;

	le->pos.trType = TR_LINEAR;
	le->pos.trTime = cg.time;
	VectorCopy( refEnt->origin, le->pos.trBase );
	VectorClear(le->pos.trDelta);
	//le->pos.trDelta[0] = crandom()*5;
	//le->pos.trDelta[1] = crandom()*5;
	//le->pos.trDelta[2] = crandom()*5 + 6;

	return le;
}
#endif

#ifdef TA_WEAPSYS // MELEE_TRAIL
//
// CG_AddWeaponTrailOld
//
/* Idea
Draw a shader from bottom of weapon attack to top of weapon attack,
     scale length using speed weapon is moving?
Allow the shader to be per-weapon
*/
void CG_AddWeaponTrailOld(centity_t *cent, refEntity_t *gun, int weaponHand)
{
	weapon_t weaponNum;
	refEntity_t trail;
	vec3_t		angles, dir;
	float		angle, d;
	float *yawAngle;
	qboolean *yawing;
	int clientNum;
	clientInfo_t *ci;
	float scale;
	int anim;

	static vec3_t weaponMoveDir[MAX_CLIENTS][MAX_HANDS];
	static vec3_t trailOldOrigin[MAX_CLIENTS][MAX_HANDS];
	static int weaponMoveDirInit = 0;
	if (!weaponMoveDirInit)
	{
		weaponMoveDirInit = 1;
		memset(&weaponMoveDir, 0, sizeof (weaponMoveDir));
		memset(&trailOldOrigin, 0, sizeof (trailOldOrigin));
	}

	if (!cg_drawMeleeWeaponTrails.integer)
		return;

	// Only melee weapons have trails.
	if (!BG_WeaponHasMelee(cent->currentState.weapon))
		return;

	anim = ( cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT );

	if (!BG_PlayerAttackAnim(anim))
		return;

	yawAngle = &cent->pe.weaponTrails[weaponHand].yawAngle;
	yawing = &cent->pe.weaponTrails[weaponHand].yawing;

	weaponNum = cent->currentState.weapon;

	clientNum = cent->currentState.clientNum;
	if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
		ci = &cgs.clientinfo[ clientNum ];

		// Scale up and back down
		scale = (float)(cent->pe.torso.frame-ci->playercfg.animations[anim].firstFrame) / ci->playercfg.animations[anim].numFrames;

		if (scale > 0.5f)
			scale = 1.0f - scale;

		scale *= 2;

		// Set move dir
		if (!VectorLength(weaponMoveDir[clientNum][weaponHand]))
		{
			VectorCopy( cent->currentState.pos.trDelta, weaponMoveDir[clientNum][weaponHand]);
		}

		VectorCopy( weaponMoveDir[clientNum][weaponHand], dir );
	}
	else
	{
		ci = NULL;
		scale = 1.0f;
		VectorCopy( cent->currentState.pos.trDelta, dir );
		// add gravity
		//dir[2] += 100;
		// ZTM: Up not down.
		dir[2] -= 100;
	}

	//
	// Use code from CTF flag so it can turn and stuff
	//

	// show the trail model
	memset( &trail, 0, sizeof(trail) );
	trail.hModel = cgs.media.flagFlapModel;
	// ZTM: TODO?: Per-weapon trail shader?
	trail.customShader = cgs.media.weaponTrailShader;
	VectorCopy( gun->lightingOrigin, trail.lightingOrigin );
	trail.shadowPlane = gun->shadowPlane;
	trail.renderfx = gun->renderfx;

	VectorClear(angles);

	//if ( updateangles )
	{
		VectorNormalize( dir );
		d = DotProduct(gun->axis[2], dir);
		// if there is anough movement orthogonal to the flag pole
		if (fabs(d) < 0.9) {
			//
			d = DotProduct(gun->axis[0], dir);
			if (d > 1.0f) {
				d = 1.0f;
			}
			else if (d < -1.0f) {
				d = -1.0f;
			}
#ifdef IOQ3ZTM_NO_COMPAT // FIXED_ACOS
			angle = Q_acos(d);
#else
			angle = acos(d);
#endif

			d = DotProduct(gun->axis[1], dir);
			if (d < 0) {
				angles[YAW] = 360 - angle * 180 / M_PI;
			}
			else {
				angles[YAW] = angle * 180 / M_PI;
			}
			if (angles[YAW] < 0)
				angles[YAW] += 360;
			if (angles[YAW] > 360)
				angles[YAW] -= 360;

			//vectoangles( cent->currentState.pos.trDelta, tmpangles );
			//angles[YAW] = tmpangles[YAW] + 45 - cent->pe.torso.yawAngle;
			// change the yaw angle
#ifdef IOQ3ZTM // BG_SWING_ANGLES
			BG_SwingAngles( angles[YAW], 25, 90, 0.15f, yawAngle, yawing, cg.frametime );
#else
			CG_SwingAngles( angles[YAW], 25, 90, 0.15f, yawAngle, yawing );
#endif
		}

		/*
		d = DotProduct(pole.axis[2], dir);
		angle = Q_acos(d);

		d = DotProduct(pole.axis[1], dir);
		if (d < 0) {
			angle = 360 - angle * 180 / M_PI;
		}
		else {
			angle = angle * 180 / M_PI;
		}
		if (angle > 340 && angle < 20) {
			flagAnim = FLAG_RUNUP;
		}
		if (angle > 160 && angle < 200) {
			flagAnim = FLAG_RUNDOWN;
		}
		*/
	}

	// set the yaw angle
	angles[YAW] = *yawAngle;
	trail.oldframe = 0;
	trail.frame = 0;
	trail.backlerp = 0;

	AnglesToAxis( angles, trail.axis );
	VectorScale(trail.axis[0], scale, trail.axis[0]);
	CG_PositionRotatedEntityOnTag( &trail, gun, gun->hModel, "tag_weapon" );

	trap_R_AddRefEntityToScene( &trail );

	if ( clientNum >= 0 && clientNum < MAX_CLIENTS )
	{
		VectorSubtract(trail.origin, trailOldOrigin[clientNum][weaponHand], weaponMoveDir[clientNum][weaponHand]);
		VectorCopy(trail.origin, trailOldOrigin[clientNum][weaponHand]);
	}
}

/*
=============
CG_AddWeaponTrail

Add tranparent copies of the model when melee attacking
=============
*/
void CG_AddWeaponTrail(centity_t *cent, refEntity_t *gun, int weaponHand, qboolean barrel)
{
	int anim;

	static int trailOldTime[MAX_CLIENTS][MAX_HANDS];
	static int weaponMoveDirInit = 0;
	if (!weaponMoveDirInit)
	{
		weaponMoveDirInit = 1;
		memset(&trailOldTime, 0, sizeof (trailOldTime));
	}

	// Old type
	if (cg_drawMeleeWeaponTrails.integer == 2)
	{
		if (barrel)
			return;

		CG_AddWeaponTrailOld(cent, gun, weaponHand);
		return;
	}

	if (!cg_drawMeleeWeaponTrails.integer)
		return;

	// Only melee weapons have trails.
	if (!BG_WeaponHasMelee(cent->currentState.weapon))
		return;

	anim = ( cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT );

	if (!BG_PlayerAttackAnim(anim))
		return;

	if (cg.time > trailOldTime[cent->currentState.clientNum][weaponHand] // barrel will be same time
		&& cg.time - trailOldTime[cent->currentState.clientNum][weaponHand] < 10)
		return;

	trailOldTime[cent->currentState.clientNum][weaponHand] = cg.time;
	CG_GhostRefEntity(gun, 100, gun->shaderRGBA[3]);
}
#endif

#ifdef TA_WEAPSYS
/*
=============
CG_AddPlayerDefaultWeapon

Used to add default weapon(s) on the player when not in use.

Draw default weapon at "tag_wp_away_primary" and "tag_wp_away_secondary"
	when a different weapon is being used.
=============
*/
void CG_AddPlayerDefaultWeapon( refEntity_t *parent, centity_t *cent, int team) {
	refEntity_t	gun;
	refEntity_t	barrel;
	vec3_t		angles;
	weapon_t weaponNum;
	weaponGroupInfo_t	*weapon;
	int i;
	//centity_t	*nonPredictedCent;
	weapon_t heldWeaponNum;
	refEntity_t	gun_left; // Left (secondary) hand weapon.
	//refEntity_t	gun; // Right (primary) hand weapon.
	// When "both hand" share the weapon, primary hand tag is used.
	qboolean draw_primary = qfalse, draw_secondary = qfalse;
	clientInfo_t	*ci;

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];

	weaponNum = cgs.clientinfo[cent->currentState.clientNum].playercfg.default_weapon;

	//if (cent->currentState.weapon == weaponNum)
	//{
		//return;
	//}

	heldWeaponNum = cent->currentState.weapon;

	CG_RegisterWeaponGroup( weaponNum );
	weapon = &cg_weapongroups[weaponNum];

	if (bg_weapongroupinfo[weaponNum].weaponnum[0])
	{
		draw_primary = qtrue;
	}
	if (bg_weapongroupinfo[weaponNum].weaponnum[1])
	{
		draw_secondary = qtrue;
	}

	// get hands from cent
	if (heldWeaponNum == weaponNum)
	{
		// If player is holding the weapon, don't draw it "away"
		if (cent->currentState.weaponHands & HAND_PRIMARY)
		{
			draw_primary = qfalse;
		}
		if (cent->currentState.weaponHands & HAND_SECONDARY)
		{
			draw_secondary = qfalse;
		}
	}

	// Check if we have some where to put it.
	if (draw_primary && draw_secondary
		&& !(ci->tagInfo & TI_TAG_WP_AWAY_PRIMARY) && !(ci->tagInfo & TI_TAG_WP_AWAY_SECONDARY))
	{
		return;
	}
	if (draw_primary && !draw_secondary
		&& !(ci->tagInfo & TI_TAG_WP_AWAY_PRIMARY))
	{
		return;
	}
	if (!draw_primary && draw_secondary
		&& !(ci->tagInfo & TI_TAG_WP_AWAY_SECONDARY))
	{
		return;
	}

	// add the weapon
	memset( &gun, 0, sizeof( gun ) );
	VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
	gun.shadowPlane = parent->shadowPlane;
	gun.renderfx = parent->renderfx;

	// Copy the primary hand weapon to the secondary hand weapon.
	memcpy( &gun_left, &gun, sizeof( gun_left ) );

	gun.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].weaponModel;
	gun_left.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[1]].weaponModel;
	if (!gun.hModel && !gun_left.hModel) {
		return;
	}

	// Primary weapon.
	if ( draw_primary && gun.hModel ) {
		if (!CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_wp_away_primary"))
		{
			// no weapon tag
			draw_primary = qfalse;
		} else {
			CG_AddWeaponWithPowerups( &gun, cent->currentState.powerups );
		}
	}

	// Secondary weapon.
	if ( draw_secondary && gun_left.hModel ) {
		if (!CG_PositionEntityOnTag( &gun_left, parent, parent->hModel, "tag_wp_away_secondary"))
		{
			// no weapon tag
			draw_secondary = qfalse;
		} else {
			CG_AddWeaponWithPowerups( &gun_left, cent->currentState.powerups );
		}
	}

	// NOTE: Any weapon type can have a barrel model

	// add the barrel for the weapons
	for (i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			if (!draw_secondary)
				continue;
		}
		else
		{
			if (!draw_primary)
				continue;
		}

		if (!cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].barrelModel)
			continue;

		memset( &barrel, 0, sizeof( barrel ) );
		VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
		barrel.shadowPlane = parent->shadowPlane;
		barrel.renderfx = parent->renderfx;

		barrel.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].barrelModel;
		angles[YAW] = 0;
		angles[PITCH] = 0;
		angles[ROLL] = 0; // Don't spin Gauntlet when not using it...
		AnglesToAxis( angles, barrel.axis );

		if (i == 1)
		{
			CG_PositionRotatedEntityOnTag( &barrel, &gun_left,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel,
				"tag_barrel" );
		}
		else
		{
			CG_PositionRotatedEntityOnTag( &barrel, &gun,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel,
				"tag_barrel" );
		}

		CG_AddWeaponWithPowerups( &barrel, cent->currentState.powerups );
	}
}
#endif

/*
=============
CG_AddPlayerWeapon

Used for both the view weapon (ps is valid) and the world modelother character models (ps is NULL)
The main player will have this called for BOTH cases, so effects like light and
sound should only be done on the world model case.
=============
*/
void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent, int team) {
	refEntity_t	gun;
	refEntity_t	barrel;
	refEntity_t	flash;
	int flashDLight;
	vec3_t		angles;
	weapon_t	weaponNum;
#ifdef TA_WEAPSYS
	weaponGroupInfo_t	*weapon;
	int i;
#else
	weaponInfo_t	*weapon;
#endif
	centity_t	*nonPredictedCent;
#ifdef TA_WEAPSYS
	float		barrelSpinAngle;
	refEntity_t	gun_left; // Left (secondary) hand weapon.
	//refEntity_t	gun; // Right (primary) hand weapon.
	// When "both hand" share the weapon, primary hand tag is used.
	qboolean draw_primary = qfalse, draw_secondary = qfalse;
	clientInfo_t	*ci;

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
#else
	orientation_t	lerped;
#endif

	weaponNum = cent->currentState.weapon;

#ifdef TA_WEAPSYS
	CG_RegisterWeaponGroup( weaponNum );
	weapon = &cg_weapongroups[weaponNum];
#else
	CG_RegisterWeapon( weaponNum );
	weapon = &cg_weapons[weaponNum];
#endif

#ifdef TA_WEAPSYS
	// If world model, add default weapon
	if (ps == NULL)
	{
		CG_AddPlayerDefaultWeapon(parent, cent, team);
	}

	if (cent->currentState.weapon <= 0 || cent->currentState.weapon >= BG_NumWeaponGroups())
	{
		return;
	}
#endif

	// add the weapon
	memset( &gun, 0, sizeof( gun ) );
	VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
	gun.shadowPlane = parent->shadowPlane;
	gun.renderfx = parent->renderfx;

	// set custom shading for railgun refire rate
	if ( ps ) {
		if (
#ifndef TA_WEAPSYS // ZTM: Do it for all weapons.
			cg.predictedPlayerState.weapon == WP_RAILGUN &&
#endif
			cg.predictedPlayerState.weaponstate == WEAPON_FIRING )
		{
			float	f;

			f = (float)cg.predictedPlayerState.weaponTime / 1500;
			gun.shaderRGBA[1] = 0;
			gun.shaderRGBA[0] = 
			gun.shaderRGBA[2] = 255 * ( 1.0 - f );
		} else {
			gun.shaderRGBA[0] = 255;
			gun.shaderRGBA[1] = 255;
			gun.shaderRGBA[2] = 255;
			gun.shaderRGBA[3] = 255;
		}

	}
#ifdef IOQ3ZTM // IOQ3BUGFIX: Don't have the railgun glow be black.
	else
	{
		gun.shaderRGBA[0] = 255;
		gun.shaderRGBA[1] = 255;
		gun.shaderRGBA[2] = 255;
		gun.shaderRGBA[3] = 255;
	}
#endif

#ifdef TA_WEAPSYS
	// Copy the primary hand weapon to the secondary hand weapon.
	memcpy( &gun_left, &gun, sizeof( gun_left ) );
#endif

#ifdef TA_WEAPSYS
	gun.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].weaponModel;
#else
	gun.hModel = weapon->weaponModel;
#endif
#ifdef TA_WEAPSYS
	// ZTM: Allow different model to be used in each hand!
	gun_left.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[1]].weaponModel;
	if (!gun.hModel && !gun_left.hModel) {
		return;
	}
#else
	if (!gun.hModel) {
		return;
	}
#endif

	if ( !ps ) {
		// add weapon ready sound
		cent->pe.lightningFiring = qfalse;
		if ( ( cent->currentState.eFlags & EF_FIRING ) &&
#ifdef TA_WEAPSYS
			cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].firingSound
#else
			weapon->firingSound
#endif
			) {
			// lightning gun and guantlet make a different sound when fire is held down
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin,
#ifdef TA_WEAPSYS
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].firingSound
#else
				weapon->firingSound
#endif
				);
			cent->pe.lightningFiring = qtrue;
		} else if (
#ifdef TA_WEAPSYS
			cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].readySound
#else
			weapon->readySound
#endif
			) {
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin,
#ifdef TA_WEAPSYS
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[0]].readySound
#else
				weapon->readySound
#endif
				);
		}
	}

#ifdef TA_WEAPSYS
	// get hands from cent
	if (cent->currentState.weaponHands & HAND_PRIMARY)
	{
		draw_primary = qtrue;
	}
	if (cent->currentState.weaponHands & HAND_SECONDARY)
	{
		draw_secondary = qtrue;
	}

	// Primary weapon.
	if ( draw_primary && gun.hModel ) {
#ifdef TA_WEAPSYS
		if (ci->tagInfo & TI_TAG_HAND_PRIMARY)
		{
			CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_hand_primary");
		}
#ifdef TA_SUPPORTQ3
		else if (ci->tagInfo & TI_TAG_WEAPON)
		{
			CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon");
		}
#endif
#elif defined TA_PLAYERS
		CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_hand_primary");
#else
		trap_R_LerpTag(&lerped, parent->hModel, parent->oldframe, parent->frame,
			1.0 - parent->backlerp, "tag_weapon");
		VectorCopy(parent->origin, gun.origin);
	  
		VectorMA(gun.origin, lerped.origin[0], parent->axis[0], gun.origin);

		// Make weapon appear left-handed for 2 and centered for 3
		if(ps && cg_drawGun.integer == 2)
			VectorMA(gun.origin, -lerped.origin[1], parent->axis[1], gun.origin);
		else if(!ps || cg_drawGun.integer != 3)
		   	VectorMA(gun.origin, lerped.origin[1], parent->axis[1], gun.origin);

		VectorMA(gun.origin, lerped.origin[2], parent->axis[2], gun.origin);

		MatrixMultiply(lerped.axis, ((refEntity_t *)parent)->axis, gun.axis);
		gun.backlerp = parent->backlerp;
#endif

		CG_AddWeaponWithPowerups( &gun, cent->currentState.powerups );

		// MELEE_TRAIL
		CG_AddWeaponTrail(cent, &gun, HAND_PRIMARY, qfalse);
	}

	// Secondary weapon.
	if ( draw_secondary && gun_left.hModel ) {
#ifdef TA_WEAPSYS
		if (ci->tagInfo & TI_TAG_HAND_SECONDARY)
		{
			CG_PositionEntityOnTag( &gun_left, parent, parent->hModel, "tag_hand_secondary");
		}
#ifdef TA_SUPPORTQ3
		else if (ci->tagInfo & TI_TAG_FLAG)
		{
			CG_PositionEntityOnTag( &gun_left, parent, parent->hModel, "tag_flag");
		}
#endif
#elif defined TA_PLAYERS
		CG_PositionEntityOnTag( &gun_left, parent, parent->hModel, "tag_hand_secondary");
#else
		CG_PositionEntityOnTag( &gun_left, parent, parent->hModel, "tag_flag");
#endif
		CG_AddWeaponWithPowerups( &gun_left, cent->currentState.powerups );

		// MELEE_TRAIL
		CG_AddWeaponTrail(cent, &gun_left, HAND_SECONDARY, qfalse);
	}

	// NOTE: Any weapon type can have a barrel model and/or flash model
	//         (And firingStoppedSound)
#else
	CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon");

	CG_AddWeaponWithPowerups( &gun, cent->currentState.powerups );
#endif

#ifdef TA_WEAPSYS
	// Even if no barrel check for firingStoppedSound
	// ZTM: FIXME: Both weapons share the same barrelSpinAngle/firingStoppedSound
	//                      (Bad for nunchucks?)
	barrelSpinAngle = CG_MachinegunSpinAngle( cent );
#endif

	// add the spinning barrel
#ifdef TA_WEAPSYS
	for (i = 0; i < 2; i++)
#else
	if ( weapon->barrelModel )
#endif
	{
#ifdef TA_WEAPSYS
		if (i == 1)
		{
			if (!draw_secondary)
				continue;
		}
		else
		{
			if (!draw_primary)
				continue;
		}

		if (!cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].barrelModel)
			continue;
#endif
		memset( &barrel, 0, sizeof( barrel ) );
		VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
		barrel.shadowPlane = parent->shadowPlane;
		barrel.renderfx = parent->renderfx;

#ifdef TA_WEAPSYS
		barrel.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].barrelModel;

		// ZTM: TESTME: In the UI there are checks for gauntlet and BFG
		//                        to spin pitch, but not here?
		// CHECKED: It should always be BS_ROLL in Q3, the `tag_barrel's are
		//		setup that way in the models.
		// So the code in the UI isn't getting run, ui ALWAYS spins ROLL
		//		(even with the checks).
		VectorClear(angles);
		if (bg_weapongroupinfo[weaponNum].weapon[i]->barrelSpin != BS_NONE)
		{
			if (i == 1) // Spin other direction
				angles[bg_weapongroupinfo[weaponNum].weapon[i]->barrelSpin] = 360-barrelSpinAngle;
			else
				angles[bg_weapongroupinfo[weaponNum].weapon[i]->barrelSpin] = barrelSpinAngle;
		}
#else
		barrel.hModel = weapon->barrelModel;

		angles[YAW] = 0;
		angles[PITCH] = 0;
		angles[ROLL] = CG_MachinegunSpinAngle( cent );
#endif
		AnglesToAxis( angles, barrel.axis );

#ifdef TA_WEAPSYS
		if (i == 1)
		{
			CG_PositionRotatedEntityOnTag( &barrel, &gun_left,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel,
				"tag_barrel" );
		}
		else
		{
			CG_PositionRotatedEntityOnTag( &barrel, &gun,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel,
				"tag_barrel" );
		}
#else
		CG_PositionRotatedEntityOnTag( &barrel, &gun, weapon->weaponModel, "tag_barrel" );
#endif

		CG_AddWeaponWithPowerups( &barrel, cent->currentState.powerups );

#ifdef TA_WEAPSYS // MELEE_TRAIL
		CG_AddWeaponTrail(cent, &barrel, i, qtrue);
#endif
	}

	// make sure we aren't looking at cg.predictedPlayerEntity for LG
	nonPredictedCent = &cg_entities[cent->currentState.clientNum];

	// if the index of the nonPredictedCent is not the same as the clientNum
	// then this is a fake player (like on teh single player podiums), so
	// go ahead and use the cent
	if( ( nonPredictedCent - cg_entities ) != cent->currentState.clientNum ) {
		nonPredictedCent = cent;
	}

	// add the flash
#ifdef TA_WEAPSYS
	for (i = 0; i < 2; i++)
#endif
	{
		if (
#ifdef TA_WEAPSYS
			( bg_weapongroupinfo[weaponNum].weapon[i]->flags & WIF_CONTINUOUS_FLASH )
#else
			( weaponNum == WP_LIGHTNING || weaponNum == WP_GAUNTLET || weaponNum == WP_GRAPPLING_HOOK )
#endif
			&& ( nonPredictedCent->currentState.eFlags & EF_FIRING ) ) 
		{
			// continuous flash
		} else {
			// impulse flash
			if ( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME && !cent->pe.railgunFlash ) {
				return;
			}
		}

		memset( &flash, 0, sizeof( flash ) );
		VectorCopy( parent->lightingOrigin, flash.lightingOrigin );
		flash.shadowPlane = parent->shadowPlane;
		flash.renderfx = parent->renderfx;

#ifdef TA_WEAPSYS
		flash.hModel = cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashModel;
#else
		flash.hModel = weapon->flashModel;
#endif
		if (!flash.hModel) {
#ifdef IOQ3ZTM
#ifdef TA_WEAPSYS // ZTM: FIXME: Support secondary weapon!
			if (i == 0)
#endif
			{
				// Use default flashOrigin when no flash model.
				VectorCopy(nonPredictedCent->lerpOrigin, nonPredictedCent->pe.flashOrigin);
				nonPredictedCent->pe.flashOrigin[2] += DEFAULT_VIEWHEIGHT - 6;
			}
#endif
			return;
		}
		angles[YAW] = 0;
		angles[PITCH] = 0;
		angles[ROLL] = crandom() * 10;
		AnglesToAxis( angles, flash.axis );

#ifdef TA_WEAPSYS // ZTM: Do it for all weapons...
		// colorize the flash
#else
		// colorize the railgun blast
		if ( weaponNum == WP_RAILGUN )
#endif
		{
			clientInfo_t	*ci;

			ci = &cgs.clientinfo[ cent->currentState.clientNum ];
			flash.shaderRGBA[0] = 255 * ci->color1[0];
			flash.shaderRGBA[1] = 255 * ci->color1[1];
			flash.shaderRGBA[2] = 255 * ci->color1[2];
		}


#ifdef TA_WEAPSYS
		if (i == 1)
			CG_PositionRotatedEntityOnTag( &flash, &gun_left,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel, "tag_flash");
		else
			CG_PositionRotatedEntityOnTag( &flash, &gun,
				cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].weaponModel, "tag_flash");
#else
		CG_PositionRotatedEntityOnTag( &flash, &gun, weapon->weaponModel, "tag_flash");
#endif
		trap_R_AddRefEntityToScene( &flash );

		if ( ps || cg.renderingThirdPerson ||
			cent->currentState.number != cg.predictedPlayerState.clientNum ) {
#ifdef TA_WEAPSYS // ZTM: FIXME: Support secondary weapon!
			if (i == 0)
#endif
			{
#ifdef IOQ3ZTM
				VectorCopy(flash.origin, nonPredictedCent->pe.flashOrigin);
#endif
			}

#ifdef TA_WEAPSYS
			if (bg_weapongroupinfo[cent->currentState.weapon].weapon[i]->proj->trailType == PT_LIGHTNING
				&& bg_weapongroupinfo[cent->currentState.weapon].weapon[i]->proj->instantDamage)
#endif
			{
				// add lightning bolt
				CG_LightningBolt( nonPredictedCent, flash.origin, bg_weapongroupinfo[cent->currentState.weapon].weapon[i]->proj->speed );
			}

#ifdef TA_WEAPSYS
			if ( bg_weapongroupinfo[cent->currentState.weapon].weapon[i]->proj->trailType == PT_RAIL)
#endif
			{
				// add rail trail
				CG_SpawnRailTrail( cent, flash.origin );
			}


#ifdef IOQ3ZTM // SMOOTH_FLASH
			flashDLight = 300 + (cg.time&31);
#else
			flashDLight = 300 + (rand()&31);
#endif
#ifdef TA_WEAPSYS
			if (cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[0]
				|| cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[1]
				|| cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[2])
			{
				trap_R_AddLightToScene( flash.origin, flashDLight,
					cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[0],
					cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[1],
					cg_weapons[bg_weapongroupinfo[weaponNum].weaponnum[i]].flashDlightColor[2] );
			}
#else
			if ( weapon->flashDlightColor[0] || weapon->flashDlightColor[1] || weapon->flashDlightColor[2] ) {
				trap_R_AddLightToScene( flash.origin, flashDLight, weapon->flashDlightColor[0],
					weapon->flashDlightColor[1], weapon->flashDlightColor[2] );
			}
#endif
		}
	}
}

/*
==============
CG_AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/
void CG_AddViewWeapon( playerState_t *ps ) {
	refEntity_t	hand;
	centity_t	*cent;
	clientInfo_t	*ci;
	float		fovOffset;
	vec3_t		angles;
#ifdef TA_WEAPSYS
	weaponGroupInfo_t	*weapon;
#else
	weaponInfo_t	*weapon;
#endif

	if ( ps->persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
		return;
	}

	if ( ps->pm_type == PM_INTERMISSION ) {
		return;
	}

	// no gun if in third person view or a camera is active
#ifdef CAMERASCRIPT
	if ( cg.renderingThirdPerson || cg.cameraMode) {
#else
	//if ( cg.renderingThirdPerson || cg.cameraMode) {
	if ( cg.renderingThirdPerson ) {
#endif
		return;
	}


	// allow the gun to be completely removed
	if ( !cg_drawGun.integer ) {
		vec3_t		origin;

		if ( cg.predictedPlayerState.eFlags & EF_FIRING ) {
			// special hack for lightning gun...
#ifdef TA_WEAPSYS
			int handSide;

			cent = &cg_entities[ps->clientNum];

			// ZTM: FIXME: Support secondary weapon!
			// Use default flashOrigin when no flash model.
			VectorCopy(cent->lerpOrigin, cent->pe.flashOrigin);
			cent->pe.flashOrigin[2] += DEFAULT_VIEWHEIGHT - 6;

			if (bg_weapongroupinfo[cent->currentState.weapon].weapon[0]->proj->trailType == PT_LIGHTNING
				&& bg_weapongroupinfo[cent->currentState.weapon].weapon[0]->proj->instantDamage)
			{
				VectorCopy( cg.refdef.vieworg, origin );

				handSide = cgs.clientinfo[cent->currentState.number].playercfg.primaryHandSide;
				if (handSide == HAND_RIGHT)
					VectorMA( origin, -8, cg.refdef.viewaxis[2], origin );
				else if (handSide == HAND_LEFT)
					VectorMA( origin, 8, cg.refdef.viewaxis[2], origin );

				CG_LightningBolt( cent, origin, LIGHTNING_RANGE);
			}

			if (bg_weapongroupinfo[cent->currentState.weapon].weapon[1]->proj->trailType == PT_LIGHTNING
				&& bg_weapongroupinfo[cent->currentState.weapon].weapon[1]->proj->instantDamage)
			{
				VectorCopy( cg.refdef.vieworg, origin );

				handSide = cgs.clientinfo[cent->currentState.number].playercfg.secondaryHandSide;
				if (handSide == HAND_RIGHT)
					VectorMA( origin, -8, cg.refdef.viewaxis[2], origin );
				else if (handSide == HAND_LEFT)
					VectorMA( origin, 8, cg.refdef.viewaxis[2], origin );
				CG_LightningBolt( cent, origin, LIGHTNING_RANGE);
			}
#else
			VectorCopy( cg.refdef.vieworg, origin );
			VectorMA( origin, -8, cg.refdef.viewaxis[2], origin );
			CG_LightningBolt( &cg_entities[ps->clientNum], origin );
#endif
		}
		return;
	}

	// don't draw if testing a gun model
	if ( cg.testGun ) {
		return;
	}

	// drop gun lower at higher fov
	if ( cg_fov.integer > 90 ) {
		fovOffset = -0.2 * ( cg_fov.integer - 90 );
#ifdef TA_MISC // FOV
	} else {
		fovOffset = 0.2 * ( cg_fov.integer - 90 );
	}
#else
	} else {
		fovOffset = 0;
	}
#endif

	cent = &cg.predictedPlayerEntity;	// &cg_entities[cg.snap->ps.clientNum];
#ifdef TA_WEAPSYS
	CG_RegisterWeaponGroup( ps->weapon );
	weapon = &cg_weapongroups[ ps->weapon ];
#else
	CG_RegisterWeapon( ps->weapon );
	weapon = &cg_weapons[ ps->weapon ];
#endif

	memset (&hand, 0, sizeof(hand));

	// set up gun position
	CG_CalculateWeaponPosition( hand.origin, angles );

	VectorMA( hand.origin, cg_gun_x.value, cg.refdef.viewaxis[0], hand.origin );
	VectorMA( hand.origin, cg_gun_y.value, cg.refdef.viewaxis[1], hand.origin );
	VectorMA( hand.origin, (cg_gun_z.value+fovOffset), cg.refdef.viewaxis[2], hand.origin );

	AnglesToAxis( angles, hand.axis );

	// map torso animations to weapon animations
	if ( cg_gun_frame.integer ) {
		// development tool
		hand.frame = hand.oldframe = cg_gun_frame.integer;
		hand.backlerp = 0;
	} else {
		// get clientinfo for animation map
		ci = &cgs.clientinfo[ cent->currentState.clientNum ];
		hand.frame = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.frame );
		hand.oldframe = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.oldFrame );
		hand.backlerp = cent->pe.torso.backlerp;
	}

	hand.hModel = weapon->handsModel;
	hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON | RF_MINLIGHT;

	// add everything onto the hand
	CG_AddPlayerWeapon( &hand, ps, &cg.predictedPlayerEntity, ps->persistant[PERS_TEAM] );
}

/*
==============================================================================

WEAPON SELECTION

==============================================================================
*/

#ifndef TA_WEAPSYS_EX
/*
===================
CG_DrawWeaponSelect
===================
*/
void CG_DrawWeaponSelect( void ) {
	int		i;
	int		bits;
	int		count;
	int		x, y, w;
	char	*name;
	float	*color;

	// don't display if dead
	if ( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 ) {
		return;
	}

	color = CG_FadeColor( cg.weaponSelectTime, WEAPON_SELECT_TIME );
	if ( !color ) {
		return;
	}
	trap_R_SetColor( color );

	// showing weapon select clears pickup item display, but not the blend blob
	cg.itemPickupTime = 0;

	// count the number of weapons owned
	bits = cg.snap->ps.stats[ STAT_WEAPONS ];
	count = 0;
	for ( i = 1 ; i < MAX_WEAPONS ; i++ ) {
		if ( bits & ( 1 << i ) ) {
			count++;
		}
	}

	x = 320 - count * 20;
	y = 380;

	for ( i = 1 ; i < MAX_WEAPONS ; i++ ) {
		if ( !( bits & ( 1 << i ) ) ) {
			continue;
		}

#ifdef TA_WEAPSYS
		CG_RegisterWeaponGroup( i );
#else
		CG_RegisterWeapon( i );
#endif

		// draw weapon icon
#ifdef TA_WEAPSYS
		CG_DrawPic( x, y, 32, 32, cg_weapongroups[i].weaponIcon );
#else
		CG_DrawPic( x, y, 32, 32, cg_weapons[i].weaponIcon );
#endif

		// draw selection marker
		if ( i == cg.weaponSelect ) {
			CG_DrawPic( x-4, y-4, 40, 40, cgs.media.selectShader );
		}

		// no ammo cross on top
		if ( !cg.snap->ps.ammo[ i ] ) {
			CG_DrawPic( x, y, 32, 32, cgs.media.noammoShader );
		}

		x += 40;
	}

	// draw the selected name
#ifdef TA_WEAPSYS
		name = bg_weapongroupinfo[ cg.weaponSelect ].pickupName;
#else
	if ( cg_weapons[ cg.weaponSelect ].item ) {
		name = cg_weapons[ cg.weaponSelect ].item->pickup_name;
#endif
		if ( name ) {
			w = CG_DrawStrlen( name ) * BIGCHAR_WIDTH;
			x = ( SCREEN_WIDTH - w ) / 2;
			CG_DrawBigStringColor(x, y - 22, name, color);
		}
#ifndef TA_WEAPSYS
	}
#endif

	trap_R_SetColor( NULL );

}

/*
===============
CG_WeaponSelectable
===============
*/
static qboolean CG_WeaponSelectable( int i ) {
	if ( !cg.snap->ps.ammo[i] ) {
		return qfalse;
	}
	if ( ! (cg.snap->ps.stats[ STAT_WEAPONS ] & ( 1 << i ) ) ) {
		return qfalse;
	}

	return qtrue;
}

/*
===============
CG_NextWeapon_f
===============
*/
void CG_NextWeapon_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	cg.weaponSelectTime = cg.time;
	original = cg.weaponSelect;

	for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
		cg.weaponSelect++;
		if ( cg.weaponSelect == MAX_WEAPONS ) {
			cg.weaponSelect = 0;
		}
#ifndef TURTLEARENA // WEAPONS
#ifdef TA_WEAPSYS
		if ( cg.weaponSelect == cg.snap->ps.stats[STAT_DEFAULTWEAPON] ) {
			continue;		// never cycle to gauntlet
		}
#else
		if ( cg.weaponSelect == WP_GAUNTLET ) {
			continue;		// never cycle to gauntlet
		}
#endif
#endif
		if ( CG_WeaponSelectable( cg.weaponSelect ) ) {
			break;
		}
	}
	if ( i == MAX_WEAPONS ) {
		cg.weaponSelect = original;
	}
}

/*
===============
CG_PrevWeapon_f
===============
*/
void CG_PrevWeapon_f( void ) {
	int		i;
	int		original;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	cg.weaponSelectTime = cg.time;
	original = cg.weaponSelect;

	for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
		cg.weaponSelect--;
		if ( cg.weaponSelect == -1 ) {
			cg.weaponSelect = MAX_WEAPONS - 1;
		}
#ifndef TURTLEARENA // WEAPONS
#ifdef TA_WEAPSYS
		if ( cg.weaponSelect == cg.snap->ps.stats[STAT_DEFAULTWEAPON] ) {
			continue;		// never cycle to gauntlet
		}
#else
		if ( cg.weaponSelect == WP_GAUNTLET ) {
			continue;		// never cycle to gauntlet
		}
#endif
#endif
		if ( CG_WeaponSelectable( cg.weaponSelect ) ) {
			break;
		}
	}
	if ( i == MAX_WEAPONS ) {
		cg.weaponSelect = original;
	}
}

/*
===============
CG_Weapon_f
===============
*/
void CG_Weapon_f( void ) {
	int		num;

	if ( !cg.snap ) {
		return;
	}
	if ( cg.snap->ps.pm_flags & PMF_FOLLOW ) {
		return;
	}

	num = atoi( CG_Argv( 1 ) );

#ifdef TA_WEAPSYS
	// Special case for WP_DEFAULT
	if (num == WP_DEFAULT)
	{
		num = cg.snap->ps.stats[STAT_DEFAULTWEAPON];
	}
#endif

	if ( num < 1 || num > MAX_WEAPONS-1 ) {
		return;
	}

	cg.weaponSelectTime = cg.time;

	if ( ! ( cg.snap->ps.stats[STAT_WEAPONS] & ( 1 << num ) ) ) {
		return;		// don't have the weapon
	}

	cg.weaponSelect = num;
}

/*
===================
CG_OutOfAmmoChange

The current weapon has just run out of ammo
===================
*/
void CG_OutOfAmmoChange( void ) {
#ifdef TA_WEAPSYS_EX
	// Change to default weapon
	cg.weaponSelectTime = cg.time;
	cg.weaponSelect = cg.predictedPlayerState.stats[STAT_DEFAULTWEAPON];
#else
	int		i;

	cg.weaponSelectTime = cg.time;

	for ( i = MAX_WEAPONS-1 ; i > 0 ; i-- ) {
		if ( CG_WeaponSelectable( i ) ) {
			cg.weaponSelect = i;
			break;
		}
	}
#endif
}
#endif // TA_WEAPSYS_EX



/*
===================================================================================================

WEAPON EVENTS

===================================================================================================
*/

#ifdef TA_WEAPSYS
/*
================
CG_WeaponUseEffect

Weapons can eject brass/smoke
================
*/
void CG_WeaponUseEffect( centity_t *cent, int handSide, int weaponNum )
{
	bg_weaponinfo_t *weap;
	int		contents;

	if (weaponNum <= 0 || weaponNum > BG_NumWeapons())
	{
		return;
	}

	weap = &bg_weaponinfo[weaponNum];

	if (weap->flags & WIF_EJECT_BRASS)
	{
		CG_MachineGunEjectBrass( cent, handSide );
	}
	if (weap->flags & WIF_EJECT_BRASS2)
	{
		CG_ShotgunEjectBrass( cent, handSide );
	}

	contents = trap_CM_PointContents( cent->currentState.pos.trBase, 0 );

	// ragepro can't alpha fade, so don't even bother with smoke
	if ( !( contents & CONTENTS_WATER ) && cgs.glconfig.hardwareType != GLHW_RAGEPRO)
	{
		if (weap->flags & WIF_EJECT_SMOKE)
		{
			CG_NailgunEjectBrass( cent, handSide );
		}

		// Shotgun smoke (From Q3's CG_ShotgunFire)
		if (weap->flags & WIF_EJECT_SMOKE2)
		{
			vec3_t			up;
			vec3_t			v;

			VectorSet( up, 0, 0, 8 );
			CG_SmokePuff( v, up, 32, 1, 1, 1, 0.33f, 900, cg.time, 0, LEF_PUFF_DONT_SCALE, cgs.media.shotgunSmokePuffShader );
		}
	}

	if (cg_weapons[weaponNum].ejectBrassFunc)
	{
		cg_weapons[weaponNum].ejectBrassFunc( cent );
	}
}
#endif

/*
================
CG_FireWeapon

Caused by an EV_FIRE_WEAPON event
================
*/
void CG_FireWeapon( centity_t *cent ) {
	entityState_t *ent;
	int				c;
#ifdef TA_WEAPSYS
	weaponGroupInfo_t	*weap;
#else
	weaponInfo_t	*weap;
#endif

	ent = &cent->currentState;
	if ( ent->weapon == WP_NONE ) {
		return;
	}
#ifdef TA_WEAPSYS
	if ( ent->weapon >= BG_NumWeaponGroups() )
#else
	if ( ent->weapon >= WP_NUM_WEAPONS )
#endif
	{
#ifdef TA_WEAPSYS
		CG_Error( "CG_FireWeapon: ent->weapon >= BG_NumWeaponGroups()" );
#else
		CG_Error( "CG_FireWeapon: ent->weapon >= WP_NUM_WEAPONS" );
#endif
		return;
	}
#ifdef TA_WEAPSYS
	weap = &cg_weapongroups[ ent->weapon ];
#else
	weap = &cg_weapons[ ent->weapon ];
#endif

	// mark the entity as muzzle flashing, so when it is added it will
	// append the flash to the weapon model
	cent->muzzleFlashTime = cg.time;

#ifdef TA_WEAPSYS // ZTM: FIXME: Add a flag for this?
						// The flag would make it so that when holding the attack
						//   button the flash sound and brass ecj would happen only on
						//   the first press.
						// Well for now check for lightning trail and instantDamge...
	// lightning gun only does this this on initial press
	if (bg_weapongroupinfo[ent->weapon].weapon[0]->proj->trailType == PT_LIGHTNING
		&& bg_weapongroupinfo[ent->weapon].weapon[0]->proj->instantDamage)
	{
		if ( cent->pe.lightningFiring ) {
			return;
		}
	}
#else
	// lightning gun only does this this on initial press
	if ( ent->weapon == WP_LIGHTNING ) {
		if ( cent->pe.lightningFiring ) {
			return;
		}
	}
#endif

#ifndef TURTLEARENA // POWERS
	// play quad sound if needed
	if ( cent->currentState.powerups & ( 1 << PW_QUAD ) ) {
		trap_S_StartSound (NULL, cent->currentState.number, CHAN_ITEM, cgs.media.quadSound );
	}
#endif

	// play a sound
	for ( c = 0 ; c < 4 ; c++ ) {
#ifdef TA_WEAPSYS
		if ( !cg_weapons[bg_weapongroupinfo[ent->weapon].weaponnum[0]].flashSound[c] )
#else
		if ( !weap->flashSound[c] )
#endif
		{
			break;
		}
	}
	if ( c > 0 ) {
		c = rand() % c;
#ifdef TA_WEAPSYS
		if ( cg_weapons[bg_weapongroupinfo[ent->weapon].weaponnum[0]].flashSound[c] )
#else
		if ( weap->flashSound[c] )
#endif
		{
			trap_S_StartSound( NULL, ent->number, CHAN_WEAPON,
#ifdef TA_WEAPSYS
					cg_weapons[bg_weapongroupinfo[ent->weapon].weaponnum[0]].flashSound[c]
#else
					weap->flashSound[c]
#endif
					);
		}
	}

	// do brass ejection
#ifdef TA_WEAPSYS
	CG_WeaponUseEffect(cent, cgs.clientinfo[cent->currentState.number].playercfg.primaryHandSide, bg_weapongroupinfo[ent->weapon].weaponnum[0]);
	CG_WeaponUseEffect(cent, cgs.clientinfo[cent->currentState.number].playercfg.secondaryHandSide, bg_weapongroupinfo[ent->weapon].weaponnum[1]);
#else
	if ( weap->ejectBrassFunc && cg_brassTime.integer > 0 ) {
		weap->ejectBrassFunc( cent );
	}
#endif
}

#ifdef TURTLEARENA // WEAPONS
/*
=================
CG_MeleeHit

Spawn "hit marker", based on CG_Bleed
=================
*/
void CG_MeleeHit( vec3_t origin/*, int entityNum */) {
	localEntity_t	*ex;
	int r = rand()%3;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;

	VectorCopy ( origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	if (r < 1)
		ex->refEntity.customShader = cgs.media.meleeHit1Shader;
	else if (r < 2)
		ex->refEntity.customShader = cgs.media.meleeHit2Shader;
	else
		ex->refEntity.customShader = cgs.media.meleeHit3Shader;
}
#endif

#ifdef TA_MISC // MATERIALS
/*
==================
CG_LaunchModel
==================
*/
localEntity_t *CG_LaunchModel( vec3_t origin, vec3_t velocity, qhandle_t hModel, float maxSize ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t angles;
	float frac;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 6000 + random() * 6000;

	VectorCopy( origin, re->origin );

	// random angles
	angles[0] = random() * 360;
	angles[1] = random() * 360;
	angles[2] = random() * 360;
	AnglesToAxis( angles, re->axis );

	// random size
	if (maxSize < 2.0f) {
		maxSize = 2.0f;
	}
	frac = (float)(random() * maxSize);
	if (frac < 1.0f) {
		frac = 1.0f;
	}
	VectorScale( re->axis[0], frac, re->axis[0] );
	VectorScale( re->axis[1], frac, re->axis[1] );
	VectorScale( re->axis[2], frac, re->axis[2] );
	re->nonNormalizedAxes = qtrue;

	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.3f; // 0.1f

	le->leBounceSoundType = LEBS_NONE;//LEBS_BRASS;
	le->leMarkType = LEMT_NONE;

	return le;
}

/*
=================
CG_ImpactParticles

Cool wall hit effects, note that the "particles" are models not sprites.
=================
*/
#define	EXP_VELOCITY	100
#define	EXP_JUMP		150
void CG_ImpactParticles( vec3_t origin, vec3_t dir, float radius, int surfaceFlags, int skipNum )
{
	localEntity_t	*le;
	int i, j;
	//int k;
	int numParticles;
	qhandle_t model;
	vec3_t velocity;
	vec3_t newOrigin;
	trace_t trace;

#ifdef IOQ3ZTM // LASERTAG
	if ( cg_laserTag.integer ) {
		return;
	}
#endif

	if (dir && VectorLength(dir))
	{
		// Move away from surface
		VectorMA( origin, 10, dir, origin );

		// Do a trace to get the flags.
		if (surfaceFlags == -1)
		{
			// Move into surface (- 10 [return to origin] - 20 [into surface])
			VectorMA( origin, -30, dir, newOrigin );

			CG_Trace( &trace, origin, NULL, NULL, newOrigin, skipNum, MASK_PLAYERSOLID );
			surfaceFlags = trace.surfaceFlags;
		}
	}
	else
	{
		CG_Trace( &trace, origin, NULL, NULL, origin, skipNum, MASK_PLAYERSOLID );

		VectorMA( origin, 10, trace.plane.normal, origin );

		if (surfaceFlags == -1) {
			surfaceFlags = trace.surfaceFlags;
		}
	}

	if (radius <= 10)
		numParticles = 3;
	else if (radius <= 18)
		numParticles = 6;
	else if (radius <= 26)
		numParticles = 12;
	else
		numParticles = 24;

	for (i = 1; i < NUM_MATERIAL_TYPES; i++)
	{
		if (cgs.media.matNumModels[i] > 0
			&& ((surfaceFlags & materialInfo[i].surfaceFlag)
			// default to stone debris if their is no set debris
			/*|| (surfaceFlags == 0 && materialInfo[i].surfaceFlag == SURF_STONE)*/))
		{
			// Impact sounds?
			// Smoke particles?

			for (j = 0; j < numParticles; j++)
			{
				VectorCopy(origin, newOrigin);

				newOrigin[0] += random()*(radius*1.2f)/2 - random()*(radius*1.2f)/2;
				newOrigin[1] += random()*(radius*1.2f)/2 - random()*(radius*1.2f)/2;
				newOrigin[2] += random()*(radius*1.2f)/2 - random()*(radius*1.2f)/2;

				velocity[0] = crandom()*EXP_VELOCITY;
				velocity[1] = crandom()*EXP_VELOCITY;
				velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;

				//for (k = 0; k < 3; k++)
				//{
				//	velocity[k] *= dir[k];
				//}

				model = cgs.media.matModels[i][rand()%cgs.media.matNumModels[i]];
				le = CG_LaunchModel(newOrigin, velocity, model, radius/4);
			}
		}
	}
}
#endif

/*
=================
CG_MissileHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/
void CG_MissileHitWall( int weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType )
{
	qhandle_t		mod;
	qhandle_t		mark;
	qhandle_t		shader;
	sfxHandle_t		sfx;
	float			radius;
	float			light;
	vec3_t			lightColor;
	localEntity_t	*le;
	int				r;
	qboolean		alphaFade;
	qboolean		isSprite;
	int				duration;
	vec3_t			sprOrg;
	vec3_t			sprVel;
#ifdef TA_WEAPSYS // SPR_EXP_SCALE
	int exp_base;
	int exp_add;
#endif

#ifdef IOQ3ZTM // LASERTAG
	if (cg_laserTag.integer)
	{
		return;
	}
#endif

	mark = 0;
	radius = 32;
	sfx = 0;
	mod = 0;
	shader = 0;
	light = 0;
	lightColor[0] = 1;
	lightColor[1] = 1;
	lightColor[2] = 0;

	// set defaults
	isSprite = qfalse;
	duration = 600;

#ifdef TA_WEAPSYS // SPR_EXP_SCALE
	// Sprite explosion scaling, starts at "exp_base" and add on "exp_add" using time scaling.
	//		so the bigest it will be is "exp_base"+"exp_add".
	exp_base = 30;
	exp_add = 42;
#endif

#ifdef TA_WEAPSYS
	if (bg_projectileinfo[weapon].trailType != PT_LIGHTNING)
	{
		switch (bg_projectileinfo[weapon].deathType)
		{
			default:
			case PD_NONE:
				break;
			case PD_PLASMA:
				mod = cgs.media.ringFlashModel;
				shader = cgs.media.plasmaExplosionShader;
				sfx = cgs.media.sfx_plasmaexp;
				break;
			case PD_ROCKET:
				mod = cgs.media.dishFlashModel;
				shader = cgs.media.rocketExplosionShader;
				sfx = cgs.media.sfx_rockexp;
				//mark = cgs.media.burnMarkShader;
				//radius = 64;
				light = 300;
				isSprite = qtrue;
				duration = 1000;
				lightColor[0] = 1;
				lightColor[1] = 0.75;
				lightColor[2] = 0.0;
				if (cg_oldRocket.integer == 0) {
					// explosion sprite animation
					VectorMA( origin, 24, dir, sprOrg );
					VectorScale( dir, 64, sprVel );

					CG_ParticleExplosion( "explode1", sprOrg, sprVel, 1400, 20, 30 );
				}
				break;
			case PD_ROCKET_SMALL: // Smaller explosion
				exp_base = 30 / 2;
				exp_add = 42 / 2;
				VectorScale( dir, 0.5f, dir );

				mod = cgs.media.dishFlashModel;
				shader = cgs.media.rocketExplosionShader;
				sfx = cgs.media.sfx_rockexp;
				//mark = cgs.media.burnMarkShader;
				//radius = 64 / 2;
				light = 300 / 2;
				isSprite = qtrue;
				duration = 1000;
				lightColor[0] = 1;
				lightColor[1] = 0.75;
				lightColor[2] = 0.0;
				/*
				if (cg_oldRocket.integer == 0) {
					// explosion sprite animation
					VectorMA( origin, 24, dir, sprOrg );
					VectorScale( dir, 64, sprVel );

					CG_ParticleExplosion( "explode1", sprOrg, sprVel, 1400, 20, 30 );
				}
				*/
				break;
			case PD_GRENADE:
				mod = cgs.media.dishFlashModel;
#ifdef TA_DATA
				shader = cgs.media.rocketExplosionShader;
#else
				shader = cgs.media.grenadeExplosionShader;
#endif
				sfx = cgs.media.sfx_rockexp;
				light = 300;
				isSprite = qtrue;
				break;
			case PD_BULLET:
				mod = cgs.media.bulletFlashModel;
				shader = cgs.media.bulletExplosionShader;
				r = rand() & 3;
				if ( r == 0 ) {
					sfx = cgs.media.sfx_ric1;
				} else if ( r == 1 ) {
					sfx = cgs.media.sfx_ric2;
				} else {
					sfx = cgs.media.sfx_ric3;
				}
				break;
			case PD_RAIL:
				mod = cgs.media.ringFlashModel;
#ifdef TA_DATA
				shader = cgs.media.plasmaExplosionShader;
#else
				shader = cgs.media.railExplosionShader;
#endif
				sfx = cgs.media.sfx_plasmaexp;
				break;
			case PD_BFG:
				mod = cgs.media.dishFlashModel;
#ifdef TA_DATA
				shader = cgs.media.rocketExplosionShader;
#else
				shader = cgs.media.bfgExplosionShader;
#endif
				sfx = cgs.media.sfx_rockexp;
				break;
			case PD_LIGHTNING:
				//
				break;
		}
	}

	// play sound
	if( soundType == IMPACTSOUND_FLESH && cg_projectiles[weapon].hitPlayerSound ) {
		sfx = cg_projectiles[weapon].hitPlayerSound;
	} else if( soundType == IMPACTSOUND_METAL && cg_projectiles[weapon].hitMetalSound ) {
		sfx = cg_projectiles[weapon].hitMetalSound;
	} else {
		for ( r = 0 ; r < 3 ; r++ ) {
			if ( !cg_projectiles[weapon].hitSound[r] )
			{
				break;
			}
		}
		if ( r > 0 ) {
			r = rand() & 3;
			if ( r < 2 ) {
				sfx = cg_projectiles[weapon].hitSound[1];
			} else if ( r == 2 ) {
				sfx = cg_projectiles[weapon].hitSound[0];
			} else {
				sfx = cg_projectiles[weapon].hitSound[2];
			}
		}
	}

/*
	switch ( weapon ) {
	default:
	case WP_LIGHTNING:
		// no explosion at LG impact, it is added with the beam
		break;
#ifndef TURTLEARENA // WEAPONS
	case WP_SHOTGUN:
		sfx = 0;
		break;
#endif
	}
*/
#else
	switch ( weapon ) {
	default:
#ifdef MISSIONPACK
	case WP_NAILGUN:
		if( soundType == IMPACTSOUND_FLESH ) {
			sfx = cgs.media.sfx_nghitflesh;
		} else if( soundType == IMPACTSOUND_METAL ) {
			sfx = cgs.media.sfx_nghitmetal;
		} else {
			sfx = cgs.media.sfx_nghit;
		}
		mark = cgs.media.holeMarkShader;
		radius = 12;
		break;
#endif
	case WP_LIGHTNING:
		// no explosion at LG impact, it is added with the beam
		r = rand() & 3;
		if ( r < 2 ) {
			sfx = cgs.media.sfx_lghit2;
		} else if ( r == 2 ) {
			sfx = cgs.media.sfx_lghit1;
		} else {
			sfx = cgs.media.sfx_lghit3;
		}
		mark = cgs.media.holeMarkShader;
		radius = 12;
		break;
#ifdef MISSIONPACK
	case WP_PROX_LAUNCHER:
		mod = cgs.media.dishFlashModel;
		shader = cgs.media.grenadeExplosionShader;
		sfx = cgs.media.sfx_proxexp;
		mark = cgs.media.burnMarkShader;
		radius = 64;
		light = 300;
		isSprite = qtrue;
		break;
#endif
	case WP_GRENADE_LAUNCHER:
		mod = cgs.media.dishFlashModel;
		shader = cgs.media.grenadeExplosionShader;
		sfx = cgs.media.sfx_rockexp;
		mark = cgs.media.burnMarkShader;
		radius = 64;
		light = 300;
		isSprite = qtrue;
		break;
	case WP_ROCKET_LAUNCHER:
		mod = cgs.media.dishFlashModel;
		shader = cgs.media.rocketExplosionShader;
		sfx = cgs.media.sfx_rockexp;
		mark = cgs.media.burnMarkShader;
		radius = 64;
		light = 300;
		isSprite = qtrue;
		duration = 1000;
		lightColor[0] = 1;
		lightColor[1] = 0.75;
		lightColor[2] = 0.0;
		if (cg_oldRocket.integer == 0) {
			// explosion sprite animation
			VectorMA( origin, 24, dir, sprOrg );
			VectorScale( dir, 64, sprVel );

			CG_ParticleExplosion( "explode1", sprOrg, sprVel, 1400, 20, 30 );
		}
		break;
	case WP_RAILGUN:
		mod = cgs.media.ringFlashModel;
		shader = cgs.media.railExplosionShader;
		sfx = cgs.media.sfx_plasmaexp;
		mark = cgs.media.energyMarkShader;
		radius = 24;
		break;
	case WP_PLASMAGUN:
		mod = cgs.media.ringFlashModel;
		shader = cgs.media.plasmaExplosionShader;
		sfx = cgs.media.sfx_plasmaexp;
		mark = cgs.media.energyMarkShader;
		radius = 16;
		break;
	case WP_BFG:
		mod = cgs.media.dishFlashModel;
		shader = cgs.media.bfgExplosionShader;
		sfx = cgs.media.sfx_rockexp;
		mark = cgs.media.burnMarkShader;
		radius = 32;
		isSprite = qtrue;
		break;
	case WP_SHOTGUN:
		mod = cgs.media.bulletFlashModel;
		shader = cgs.media.bulletExplosionShader;
		mark = cgs.media.bulletMarkShader;
		sfx = 0;
		radius = 4;
		break;

#ifdef MISSIONPACK
	case WP_CHAINGUN:
		mod = cgs.media.bulletFlashModel;
		if( soundType == IMPACTSOUND_FLESH ) {
			sfx = cgs.media.sfx_chghitflesh;
		} else if( soundType == IMPACTSOUND_METAL ) {
			sfx = cgs.media.sfx_chghitmetal;
		} else {
			sfx = cgs.media.sfx_chghit;
		}
		mark = cgs.media.bulletMarkShader;

		r = rand() & 3;
		if ( r < 2 ) {
			sfx = cgs.media.sfx_ric1;
		} else if ( r == 2 ) {
			sfx = cgs.media.sfx_ric2;
		} else {
			sfx = cgs.media.sfx_ric3;
		}

		radius = 8;
		break;
#endif

	case WP_MACHINEGUN:
		mod = cgs.media.bulletFlashModel;
		shader = cgs.media.bulletExplosionShader;
		mark = cgs.media.bulletMarkShader;

		r = rand() & 3;
		if ( r == 0 ) {
			sfx = cgs.media.sfx_ric1;
		} else if ( r == 1 ) {
			sfx = cgs.media.sfx_ric2;
		} else {
			sfx = cgs.media.sfx_ric3;
		}

		radius = 8;
		break;
	}
#endif

	if ( sfx ) {
		trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, sfx );
	}

	//
	// create the explosion
	//
	if ( mod ) {
		le = CG_MakeExplosion( origin, dir,
							   mod,	shader,
							   duration, isSprite );
		le->light = light;
		VectorCopy( lightColor, le->lightColor );
#ifdef TA_WEAPSYS
		if (bg_projectileinfo[weapon].flags & PF_EXPLOSION_COLORIZE)
#else
		if ( weapon == WP_RAILGUN )
#endif
		{
			// colorize with client color
			VectorCopy( cgs.clientinfo[clientNum].color1, le->color );
		}
#ifdef TA_WEAPSYS // SPR_EXP_SCALE
		le->radius = exp_base;
		le->refEntity.radius = exp_add;
#endif
	}

#ifdef TA_WEAPSYS
	mark = cg_projectiles[weapon].wallmarkShader;
	radius = cg_projectiles[weapon].wallmarkRadius;

	if (!mark || radius <= 0)
	{
		// No mark or radius.
		return;
	}
#endif

	//
	// impact mark
	//
#ifdef TA_WEAPSYS
	// plasma fades alpha, all others fade color
	alphaFade = (bg_projectileinfo[weapon].flags & PF_WALLMARK_FADE_ALPHA);

	if (bg_projectileinfo[weapon].flags & PF_WALLMARK_COLORIZE)
#else
	alphaFade = (mark == cgs.media.energyMarkShader);	// plasma fades alpha, all others fade color
	if ( weapon == WP_RAILGUN )
#endif
	{
		float	*color;

		// colorize with client color
		color = cgs.clientinfo[clientNum].color2;
#ifdef TA_WEAPSYS
		if (!CG_ImpactMark( mark, origin, dir, random()*360, color[0],color[1], color[2],1, alphaFade, radius, qfalse ))
			return;
#else
		CG_ImpactMark( mark, origin, dir, random()*360, color[0],color[1], color[2],1, alphaFade, radius, qfalse );
#endif
	} else {
#ifdef TA_WEAPSYS
		if (!CG_ImpactMark( mark, origin, dir, random()*360, 1,1,1,1, alphaFade, radius, qfalse ))
			return;
#else
		CG_ImpactMark( mark, origin, dir, random()*360, 1,1,1,1, alphaFade, radius, qfalse );
#endif
	}

#ifdef TA_MISC // MATERIALS
	if (soundType == IMPACTSOUND_METAL)
		CG_ImpactParticles(origin, dir, radius, SURF_METALSTEPS, clientNum);
	else
		CG_ImpactParticles(origin, dir, radius, -1, clientNum);
#endif
}

/*
=================
CG_MissileHitPlayer
=================
*/
void CG_MissileHitPlayer( int weapon, vec3_t origin, vec3_t dir, int entityNum ) {
#ifndef NOBLOOD
	CG_Bleed( origin, entityNum );
#endif

	// some weapons will make an explosion with the blood, while
	// others will just make the blood
#ifdef TA_WEAPSYS
	// ZTM: TODO: Check explosionType != PE_NONE instead of deathType?
	if (bg_projectileinfo[weapon].deathType != PD_NONE)
	{
		CG_MissileHitWall( weapon, 0, origin, dir, IMPACTSOUND_FLESH );
	}
#else
	switch ( weapon ) {
	case WP_GRENADE_LAUNCHER:
	case WP_ROCKET_LAUNCHER:
#ifdef MISSIONPACK
	case WP_NAILGUN:
	case WP_CHAINGUN:
	case WP_PROX_LAUNCHER:
#endif
		CG_MissileHitWall( weapon, 0, origin, dir, IMPACTSOUND_FLESH );
		break;
	default:
		break;
	}
#endif
}

#ifdef TA_WEAPSYS
/*
=================
CG_WeaponHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/
void CG_WeaponHitWall( int weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType )
{
	qhandle_t		mod;
	qhandle_t		mark;
	qhandle_t		shader;
	sfxHandle_t		sfx;
	float			radius;
	float			light;
	vec3_t			lightColor;
	localEntity_t	*le;
	//int				r;
	qboolean		alphaFade;
	qboolean		isSprite;
	int				duration;
	//vec3_t			sprOrg;
	//vec3_t			sprVel;
	int exp_base;
	int exp_add;

#ifdef IOQ3ZTM // LASERTAG
	if (cg_laserTag.integer)
	{
		return;
	}
#endif

	mark = 0;
	radius = 32;
	sfx = 0;
	mod = 0;
	shader = 0;
	light = 0;
	lightColor[0] = 1;
	lightColor[1] = 1;
	lightColor[2] = 0;

	// set defaults
	isSprite = qfalse;
	duration = 600;

	// Sprite explosion scaling, starts at "exp_base" and add on "exp_add" using time scaling.
	//		so the bigest it will be is "exp_base"+"exp_add".
	exp_base = 30;
	exp_add = 42;

	// Melee Weapon hit wall.
	if (BG_WeaponHasMelee(weapon))
	{
		//CG_MeleeHit(origin);

#if 0 // ZTM: TODO: Only when hit entity that take_damage and not client.
		mod = cgs.media.dishFlashModel;
		isSprite = qtrue;
		// Smaller explosion
		exp_base = 30 / 3;
		exp_add = 42 / 4;

		r = rand() & 3;
		if (r < 1)
			shader = cgs.media.meleeHit1Shader;
		else if (r < 2)
			shader = cgs.media.meleeHit2Shader;
		else
			shader = cgs.media.meleeHit3Shader;
#endif

		/*if( soundType == IMPACTSOUND_FLESH ) {
			sfx = cgs.media.sfx_meleehit;//sfx_meleehitflesh;
		} else if( soundType == IMPACTSOUND_METAL ) {
			sfx = cgs.media.sfx_meleehitmetal;
		} else {
			sfx = cgs.media.sfx_meleehit;
		}*/
	}


	if ( sfx ) {
		trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, sfx );
	}

	//
	// create the explosion
	//
	if ( mod ) {
		le = CG_MakeExplosion( origin, dir,
							   mod,	shader,
							   duration, isSprite );
		le->light = light;
		VectorCopy( lightColor, le->lightColor );
		if (bg_weapongroupinfo[weapon].weapon[0]->flags & WIF_WALLMARK_COLORIZE)
		{
			// colorize with client color
			VectorCopy( cgs.clientinfo[clientNum].color1, le->color );
		}
		le->radius = exp_base;
		le->refEntity.radius = exp_add;
	}

	// ZTM: FIXME: Each weapon in the group has its own!
	mark = cg_weapons[bg_weapongroupinfo[weapon].weaponnum[0]].wallmarkShader;
	radius = cg_weapons[bg_weapongroupinfo[weapon].weaponnum[0]].wallmarkRadius;

	if (!mark || radius <= 0)
	{
		// No mark or radius.
		return;
	}

	//
	// impact mark
	//
	// plasma fades alpha, all others fade color
	alphaFade = (bg_weapongroupinfo[weapon].weapon[0]->flags & WIF_WALLMARK_FADE_ALPHA);

	if (bg_weapongroupinfo[weapon].weapon[0]->flags & WIF_WALLMARK_COLORIZE)
	{
		float	*color;

		// colorize with client color
		color = cgs.clientinfo[clientNum].color2;
		if (!CG_ImpactMark( mark, origin, dir, random()*360, color[0],color[1], color[2],1, alphaFade, radius, qfalse ))
			return; // no impact
	} else {
		if (!CG_ImpactMark( mark, origin, dir, random()*360, 1,1,1,1, alphaFade, radius, qfalse ))
			return; // no impact
	}

#ifdef TA_MISC // MATERIALS
	if (soundType == IMPACTSOUND_METAL)
		CG_ImpactParticles(origin, dir, radius, SURF_METALSTEPS, clientNum);
	else
		CG_ImpactParticles(origin, dir, radius, -1, clientNum);
#endif
}

/*
=================
CG_WeaponHitPlayer

weapon is weapon group index

This is called then there is a attack trace without a missile (melee weapons, lightning gun, etc)
=================
*/
void CG_WeaponHitPlayer( int weapon, vec3_t origin, vec3_t dir, int entityNum ) {

	if (BG_WeaponHasMelee(weapon))
	{
#ifndef NOBLOOD
		if ((bg_weaponinfo[weapon].flags & WIF_CUTS) || (rand()&20 > 15))
			CG_Bleed( origin, entityNum );
#endif
#ifdef TURTLEARENA // WEAPONS
		CG_MeleeHit(origin);
#endif
		return;
	}

#ifndef NOBLOOD
	CG_Bleed( origin, entityNum );
#endif

	// some weapons will make an explosion with the blood, while
	// others will just make the blood
	if (bg_weapongroupinfo[weapon].weapon[0]->proj->deathType != PD_NONE)
	{
		CG_MissileHitWall( bg_weapongroupinfo[weapon].weapon[0]->projnum, 0, origin, dir, IMPACTSOUND_FLESH );
	}
}
#endif


#ifndef TA_WEAPSYS
/*
============================================================================

SHOTGUN TRACING

============================================================================
*/

/*
================
CG_ShotgunPellet
================
*/
static void CG_ShotgunPellet( vec3_t start, vec3_t end, int skipNum ) {
	trace_t		tr;
	int sourceContentType, destContentType;

	CG_Trace( &tr, start, NULL, NULL, end, skipNum, MASK_SHOT );

	sourceContentType = trap_CM_PointContents( start, 0 );
	destContentType = trap_CM_PointContents( tr.endpos, 0 );

	// FIXME: should probably move this cruft into CG_BubbleTrail
	if ( sourceContentType == destContentType ) {
		if ( sourceContentType & CONTENTS_WATER ) {
			CG_BubbleTrail( start, tr.endpos, 32 );
		}
	} else if ( sourceContentType & CONTENTS_WATER ) {
		trace_t trace;

		trap_CM_BoxTrace( &trace, end, start, NULL, NULL, 0, CONTENTS_WATER );
		CG_BubbleTrail( start, trace.endpos, 32 );
	} else if ( destContentType & CONTENTS_WATER ) {
		trace_t trace;

		trap_CM_BoxTrace( &trace, start, end, NULL, NULL, 0, CONTENTS_WATER );
		CG_BubbleTrail( tr.endpos, trace.endpos, 32 );
	}

	if (  tr.surfaceFlags & SURF_NOIMPACT ) {
		return;
	}

	if ( cg_entities[tr.entityNum].currentState.eType == ET_PLAYER
#ifdef TA_NPCSYS
		|| cg_entities[tr.entityNum].currentState.eType == ET_NPC
#endif
	) {
		CG_MissileHitPlayer( WP_SHOTGUN, tr.endpos, tr.plane.normal, tr.entityNum );
	} else {
		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			// SURF_NOIMPACT will not make a flame puff or a mark
			return;
		}
		if ( tr.surfaceFlags & SURF_METALSTEPS ) {
			CG_MissileHitWall( WP_SHOTGUN, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_METAL );
		} else {
			CG_MissileHitWall( WP_SHOTGUN, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_DEFAULT );
		}
	}
}

/*
================
CG_ShotgunPattern

Perform the same traces the server did to locate the
hit splashes
================
*/
static void CG_ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, int otherEntNum ) {
	int			i;
	float		r, u;
	vec3_t		end;
	vec3_t		forward, right, up;

	// derive the right and up vectors from the forward vector, because
	// the client won't have any other information
	VectorNormalize2( origin2, forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

	// generate the "random" spread pattern
	for ( i = 0 ; i < DEFAULT_SHOTGUN_COUNT ; i++ ) {
		r = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		u = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		VectorMA( origin, 8192 * 16, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);

		CG_ShotgunPellet( origin, end, otherEntNum );
	}
}

/*
==============
CG_ShotgunFire
==============
*/
void CG_ShotgunFire( entityState_t *es ) {
	vec3_t	v;
	int		contents;

	VectorSubtract( es->origin2, es->pos.trBase, v );
	VectorNormalize( v );
	VectorScale( v, 32, v );
	VectorAdd( es->pos.trBase, v, v );
	if ( cgs.glconfig.hardwareType != GLHW_RAGEPRO ) {
		// ragepro can't alpha fade, so don't even bother with smoke
		vec3_t			up;

		contents = trap_CM_PointContents( es->pos.trBase, 0 );
		if ( !( contents & CONTENTS_WATER ) ) {
			VectorSet( up, 0, 0, 8 );
			CG_SmokePuff( v, up, 32, 1, 1, 1, 0.33f, 900, cg.time, 0, LEF_PUFF_DONT_SCALE, cgs.media.shotgunSmokePuffShader );
		}
	}
	CG_ShotgunPattern( es->pos.trBase, es->origin2, es->eventParm, es->otherEntityNum );
}
#endif

/*
============================================================================

BULLETS

============================================================================
*/


/*
===============
CG_Tracer
===============
*/
void CG_Tracer( vec3_t source, vec3_t dest ) {
	vec3_t		forward, right;
	polyVert_t	verts[4];
	vec3_t		line;
	float		len, begin, end;
	vec3_t		start, finish;
	vec3_t		midpoint;

	// tracer
	VectorSubtract( dest, source, forward );
	len = VectorNormalize( forward );

	// start at least a little ways from the muzzle
	if ( len < 100 ) {
		return;
	}
	begin = 50 + random() * (len - 60);
	end = begin + cg_tracerLength.value;
	if ( end > len ) {
		end = len;
	}
	VectorMA( source, begin, forward, start );
	VectorMA( source, end, forward, finish );

	line[0] = DotProduct( forward, cg.refdef.viewaxis[1] );
	line[1] = DotProduct( forward, cg.refdef.viewaxis[2] );

	VectorScale( cg.refdef.viewaxis[1], line[1], right );
	VectorMA( right, -line[0], cg.refdef.viewaxis[2], right );
	VectorNormalize( right );

	VectorMA( finish, cg_tracerWidth.value, right, verts[0].xyz );
	verts[0].st[0] = 0;
	verts[0].st[1] = 1;
	verts[0].modulate[0] = 255;
	verts[0].modulate[1] = 255;
	verts[0].modulate[2] = 255;
	verts[0].modulate[3] = 255;

	VectorMA( finish, -cg_tracerWidth.value, right, verts[1].xyz );
	verts[1].st[0] = 1;
	verts[1].st[1] = 0;
	verts[1].modulate[0] = 255;
	verts[1].modulate[1] = 255;
	verts[1].modulate[2] = 255;
	verts[1].modulate[3] = 255;

	VectorMA( start, -cg_tracerWidth.value, right, verts[2].xyz );
	verts[2].st[0] = 1;
	verts[2].st[1] = 1;
	verts[2].modulate[0] = 255;
	verts[2].modulate[1] = 255;
	verts[2].modulate[2] = 255;
	verts[2].modulate[3] = 255;

	VectorMA( start, cg_tracerWidth.value, right, verts[3].xyz );
	verts[3].st[0] = 0;
	verts[3].st[1] = 0;
	verts[3].modulate[0] = 255;
	verts[3].modulate[1] = 255;
	verts[3].modulate[2] = 255;
	verts[3].modulate[3] = 255;

	trap_R_AddPolyToScene( cgs.media.tracerShader, 4, verts );

	midpoint[0] = ( start[0] + finish[0] ) * 0.5;
	midpoint[1] = ( start[1] + finish[1] ) * 0.5;
	midpoint[2] = ( start[2] + finish[2] ) * 0.5;

	// add the tracer sound
	trap_S_StartSound( midpoint, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.tracerSound );

}


/*
======================
CG_CalcMuzzlePoint
======================
*/
static qboolean	CG_CalcMuzzlePoint( int entityNum, vec3_t muzzle ) {
	vec3_t		forward;
	centity_t	*cent;
	int			anim;

	if ( entityNum == cg.snap->ps.clientNum ) {
		VectorCopy( cg.snap->ps.origin, muzzle );
		muzzle[2] += cg.snap->ps.viewheight;
		AngleVectors( cg.snap->ps.viewangles, forward, NULL, NULL );
		VectorMA( muzzle, 14, forward, muzzle );
		return qtrue;
	}

	cent = &cg_entities[entityNum];
	if ( !cent->currentValid ) {
		return qfalse;
	}

	VectorCopy( cent->currentState.pos.trBase, muzzle );

	AngleVectors( cent->currentState.apos.trBase, forward, NULL, NULL );
	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;
	if ( anim == LEGS_WALKCR || anim == LEGS_IDLECR ) {
		muzzle[2] += CROUCH_VIEWHEIGHT;
	} else {
		muzzle[2] += DEFAULT_VIEWHEIGHT;
	}

	VectorMA( muzzle, 14, forward, muzzle );

	return qtrue;

}

/*
======================
CG_Bullet

Renders bullet effects.
======================
*/
void CG_Bullet( vec3_t end, int sourceEntityNum, vec3_t normal, qboolean flesh, int fleshEntityNum
#ifdef TA_WEAPSYS
		, int projnum
#endif
		)
{
#ifdef TA_WEAPSYS
	// if the shooter is currently valid, calc a source point and possibly
	// do trail effects
	if ( sourceEntityNum >= 0 )
	{
		vec3_t		start;

		if ( !CG_CalcMuzzlePoint( sourceEntityNum, start ) )
		{
			return;
		}

		// draw a tracer
		if ( cg_tracerChance.value > 0 && random() < cg_tracerChance.value ) {
			CG_Tracer( start, end );
		}

		if (!CG_BulletBubbleTrail(start, end, sourceEntityNum))
		{
			return;
		}
	}
#else
	trace_t trace;
	int sourceContentType, destContentType;
	vec3_t		start;

	// if the shooter is currently valid, calc a source point and possibly
	// do trail effects
	if ( sourceEntityNum >= 0 && cg_tracerChance.value > 0 ) {
		if ( CG_CalcMuzzlePoint( sourceEntityNum, start ) ) {
			sourceContentType = trap_CM_PointContents( start, 0 );
			destContentType = trap_CM_PointContents( end, 0 );

			// do a complete bubble trail if necessary
			if ( ( sourceContentType == destContentType ) && ( sourceContentType & CONTENTS_WATER ) ) {
				CG_BubbleTrail( start, end, 32 );
			}
			// bubble trail from water into air
			else if ( ( sourceContentType & CONTENTS_WATER ) ) {
				trap_CM_BoxTrace( &trace, end, start, NULL, NULL, 0, CONTENTS_WATER );
				CG_BubbleTrail( start, trace.endpos, 32 );
			}
			// bubble trail from air into water
			else if ( ( destContentType & CONTENTS_WATER ) ) {
				trap_CM_BoxTrace( &trace, start, end, NULL, NULL, 0, CONTENTS_WATER );
				CG_BubbleTrail( trace.endpos, end, 32 );
			}

			// draw a tracer
			if ( random() < cg_tracerChance.value ) {
				CG_Tracer( start, end );
			}
		}
	}
#endif

	// impact splash and mark
	if ( flesh ) {
#ifndef NOBLOOD
		CG_Bleed( end, fleshEntityNum );
#endif
	} else {
#ifdef TA_WEAPSYS
		CG_MissileHitWall( projnum, 0, end, normal, IMPACTSOUND_DEFAULT );
#else
		CG_MissileHitWall( WP_MACHINEGUN, 0, end, normal, IMPACTSOUND_DEFAULT );
#endif
	}

}
