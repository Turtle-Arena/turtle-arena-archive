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
// ui_players.c

#include "ui_local.h"


#ifndef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
#define UI_TIMER_GESTURE		2300
#endif
#define UI_TIMER_JUMP			1000
#ifndef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
#define UI_TIMER_LAND			130
#endif
#define UI_TIMER_WEAPON_SWITCH	300
#ifndef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
#define UI_TIMER_ATTACK			500
#endif
#define	UI_TIMER_MUZZLE_FLASH	20
#define	UI_TIMER_WEAPON_DELAY	250

#define JUMP_HEIGHT				56

#ifndef IOQ3ZTM // BG_SWING_ANGLES
#define SWINGSPEED				0.3f
#endif

#define SPIN_SPEED				0.9f
#define COAST_TIME				1000


static int			dp_realtime;
static float		jumpHeight;
#ifndef TMNTWEAPSYS_EX
sfxHandle_t weaponChangeSound;
#endif


/*
===============
UI_PlayerInfo_SetWeapon
===============
*/
static void UI_PlayerInfo_SetWeapon( playerInfo_t *pi, weapon_t weaponNum ) {
#ifndef TMNTWEAPSYS
	gitem_t *	item;
#endif
	char		path[MAX_QPATH];

	pi->currentWeapon = weaponNum;
#ifndef TMNTWEAPSYS
tryagain:
#endif
	pi->realWeapon = weaponNum;
	pi->weaponModel = 0;
	pi->barrelModel = 0;
	pi->flashModel = 0;

	if ( weaponNum == WP_NONE ) {
		return;
	}

#ifdef TMNTWEAPSYS
	pi->weaponModel = trap_R_RegisterModel(bg_weapongroupinfo[weaponNum].weapon[0]->model);
	pi->weaponModel2 = trap_R_RegisterModel(bg_weapongroupinfo[weaponNum].weapon[1]->model);
#else
	for ( item = bg_itemlist + 1; item->classname ; item++ ) {
		if ( item->giType != IT_WEAPON ) {
			continue;
		}
		if ( item->giTag == weaponNum ) {
			break;
		}
	}

	if ( item->classname ) {
		pi->weaponModel = trap_R_RegisterModel( item->world_model[0] );
	}
#endif

#ifndef TMNTWEAPSYS
	if( pi->weaponModel == 0 ) {
		if( weaponNum == WP_MACHINEGUN ) {
			weaponNum = WP_NONE;
			goto tryagain;
		}
		weaponNum = WP_MACHINEGUN;
		goto tryagain;
	}

	if ( weaponNum == WP_MACHINEGUN || weaponNum == WP_GAUNTLET || weaponNum == WP_BFG )
#endif
	{
#ifdef TMNTWEAPSYS
		strcpy( path, bg_weapongroupinfo[weaponNum].weapon[0]->model );
#else
		strcpy( path, item->world_model[0] );
#endif
		COM_StripExtension(path, path, sizeof(path));
		strcat( path, "_barrel.md3" );
		pi->barrelModel = trap_R_RegisterModel( path );
	}

#ifdef TMNTWEAPSYS
	strcpy( path, bg_weapongroupinfo[weaponNum].weapon[0]->model );
#else
	strcpy( path, item->world_model[0] );
#endif
	COM_StripExtension(path, path, sizeof(path));
	strcat( path, "_flash.md3" );
	pi->flashModel = trap_R_RegisterModel( path );

#ifdef TMNTWEAPSYS
	MAKERGB( pi->flashDlightColor,
			bg_weapongroupinfo[weaponNum].weapon[0]->flashColor[0],
			bg_weapongroupinfo[weaponNum].weapon[0]->flashColor[1],
			bg_weapongroupinfo[weaponNum].weapon[0]->flashColor[2] );
#else
	switch( weaponNum ) {
	case WP_GAUNTLET:
		MAKERGB( pi->flashDlightColor, 0.6f, 0.6f, 1 );
		break;

	case WP_MACHINEGUN:
		MAKERGB( pi->flashDlightColor, 1, 1, 0 );
		break;

	case WP_SHOTGUN:
		MAKERGB( pi->flashDlightColor, 1, 1, 0 );
		break;

	case WP_GRENADE_LAUNCHER:
		MAKERGB( pi->flashDlightColor, 1, 0.7f, 0.5f );
		break;

	case WP_ROCKET_LAUNCHER:
		MAKERGB( pi->flashDlightColor, 1, 0.75f, 0 );
		break;

	case WP_LIGHTNING:
		MAKERGB( pi->flashDlightColor, 0.6f, 0.6f, 1 );
		break;

	case WP_RAILGUN:
		MAKERGB( pi->flashDlightColor, 1, 0.5f, 0 );
		break;

	case WP_PLASMAGUN:
		MAKERGB( pi->flashDlightColor, 0.6f, 0.6f, 1 );
		break;

	case WP_BFG:
		MAKERGB( pi->flashDlightColor, 1, 0.7f, 1 );
		break;

	case WP_GRAPPLING_HOOK:
		MAKERGB( pi->flashDlightColor, 0.6f, 0.6f, 1 );
		break;

	default:
		MAKERGB( pi->flashDlightColor, 1, 1, 1 );
		break;
	}
#endif
}


/*
===============
UI_ForceLegsAnim
===============
*/
static void UI_ForceLegsAnim( playerInfo_t *pi, int anim ) {
	pi->legsAnim = ( ( pi->legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;

	if ( anim == LEGS_JUMP ) {
		pi->legsAnimationTimer = UI_TIMER_JUMP;
	}
}


/*
===============
UI_SetLegsAnim
===============
*/
static void UI_SetLegsAnim( playerInfo_t *pi, int anim ) {
	if ( pi->pendingLegsAnim ) {
		anim = pi->pendingLegsAnim;
		pi->pendingLegsAnim = 0;
	}
	UI_ForceLegsAnim( pi, anim );
}


/*
===============
UI_ForceTorsoAnim
===============
*/
static void UI_ForceTorsoAnim( playerInfo_t *pi, int anim ) {
	pi->torsoAnim = ( ( pi->torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;

	if ( anim == TORSO_GESTURE ) {
#ifdef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
		pi->torsoAnimationTimer = BG_AnimationTime(&pi->playercfg.animations[anim]);
#else
		pi->torsoAnimationTimer = UI_TIMER_GESTURE;
#endif
	}

#ifdef TMNTWEAPSYS
	if (BG_PlayerAttackAnim(anim))
#else
	if ( anim == TORSO_ATTACK || anim == TORSO_ATTACK2 )
#endif
	{
#ifdef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
		pi->torsoAnimationTimer = BG_AnimationTime(&pi->playercfg.animations[anim]);
#else
		pi->torsoAnimationTimer = UI_TIMER_ATTACK;
#endif
	}
}


/*
===============
UI_SetTorsoAnim
===============
*/
static void UI_SetTorsoAnim( playerInfo_t *pi, int anim ) {
	if ( pi->pendingTorsoAnim ) {
		anim = pi->pendingTorsoAnim;
		pi->pendingTorsoAnim = 0;
	}

	UI_ForceTorsoAnim( pi, anim );
}


/*
===============
UI_TorsoSequencing
===============
*/
static void UI_TorsoSequencing( playerInfo_t *pi ) {
	int		currentAnim;

	currentAnim = pi->torsoAnim & ~ANIM_TOGGLEBIT;

	if ( pi->weapon != pi->currentWeapon ) {
		if ( currentAnim != TORSO_DROP ) {
			pi->torsoAnimationTimer = UI_TIMER_WEAPON_SWITCH;
			UI_ForceTorsoAnim( pi, TORSO_DROP );
		}
	}

	if ( pi->torsoAnimationTimer > 0 ) {
		return;
	}

	if( currentAnim == TORSO_GESTURE ) {
#ifdef TMNTWEAPSYS
		UI_SetTorsoAnim( pi, BG_TorsoStandForWeapon(pi->realWeapon) );
#else
		UI_SetTorsoAnim( pi, TORSO_STAND );
#endif
		return;
	}

#ifdef TMNTWEAPSYS
	if (BG_PlayerAttackAnim(currentAnim)) {
		UI_SetTorsoAnim( pi, BG_TorsoStandForWeapon(pi->realWeapon) );
		return;
	}
#else
	if( currentAnim == TORSO_ATTACK || currentAnim == TORSO_ATTACK2 ) {
		UI_SetTorsoAnim( pi, TORSO_STAND );
		return;
	}
#endif

	if ( currentAnim == TORSO_DROP ) {
		UI_PlayerInfo_SetWeapon( pi, pi->weapon );
		pi->torsoAnimationTimer = UI_TIMER_WEAPON_SWITCH;
		UI_ForceTorsoAnim( pi, TORSO_RAISE );
		return;
	}

	if ( currentAnim == TORSO_RAISE ) {
#ifdef TMNTWEAPSYS
		UI_SetTorsoAnim( pi, BG_TorsoStandForWeapon(pi->realWeapon) );
#else
		UI_SetTorsoAnim( pi, TORSO_STAND );
#endif
		return;
	}
}


/*
===============
UI_LegsSequencing
===============
*/
static void UI_LegsSequencing( playerInfo_t *pi ) {
	int		currentAnim;

	currentAnim = pi->legsAnim & ~ANIM_TOGGLEBIT;

	if ( pi->legsAnimationTimer > 0 ) {
		if ( currentAnim == LEGS_JUMP ) {
			jumpHeight = JUMP_HEIGHT * sin( M_PI * ( UI_TIMER_JUMP - pi->legsAnimationTimer ) / UI_TIMER_JUMP );
		}
		return;
	}

	if ( currentAnim == LEGS_JUMP ) {
		UI_ForceLegsAnim( pi, LEGS_LAND );
#ifdef TMNTPLAYERSYS // PLAYERCFG_ANIMATION_TIMES
		pi->legsAnimationTimer = BG_AnimationTime(&pi->playercfg.animations[LEGS_LAND]);
#else
		pi->legsAnimationTimer = UI_TIMER_LAND;
#endif
		jumpHeight = 0;
		return;
	}

	if ( currentAnim == LEGS_LAND ) {
		UI_SetLegsAnim( pi, LEGS_IDLE );
		return;
	}
}


/*
======================
UI_PositionEntityOnTag
======================
*/
#ifdef IOQ3ZTM
static qboolean UI_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent,
							clipHandle_t parentModel, char *tagName )
#else
static void UI_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							clipHandle_t parentModel, char *tagName )
#endif
{
	int				i;
	orientation_t	lerped;
	
	// lerp the tag
#ifdef IOQ3ZTM
	qboolean rtn = qtrue;
	if (!trap_CM_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName ))
	{
		rtn = qfalse;
	}
#else
	trap_CM_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );
#endif

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// cast away const because of compiler problems
	MatrixMultiply( lerped.axis, ((refEntity_t*)parent)->axis, entity->axis );
	entity->backlerp = parent->backlerp;
#ifdef IOQ3ZTM
	return rtn;
#endif
}


/*
======================
UI_PositionRotatedEntityOnTag
======================
*/
#ifdef IOQ3ZTM
static qboolean UI_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent,
							clipHandle_t parentModel, char *tagName )
#else
static void UI_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							clipHandle_t parentModel, char *tagName )
#endif
{
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];

	// lerp the tag
#ifdef IOQ3ZTM
	qboolean rtn = qtrue;
	if (!trap_CM_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName ))
	{
		rtn = qfalse;
	}
#else
	trap_CM_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );
#endif

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// cast away const because of compiler problems
	MatrixMultiply( entity->axis, ((refEntity_t *)parent)->axis, tempAxis );
	MatrixMultiply( lerped.axis, tempAxis, entity->axis );
#ifdef IOQ3ZTM
	return rtn;
#endif
}


#ifndef IOQ3ZTM // LERP_FRAME_CLIENT_LESS // Moved to bg_misc.c
/*
===============
UI_SetLerpFrameAnimation
===============
*/
static void UI_SetLerpFrameAnimation( playerInfo_t *ci, lerpFrame_t *lf, int newAnimation ) {
	animation_t	*anim;

	lf->animationNumber = newAnimation;
	newAnimation &= ~ANIM_TOGGLEBIT;

	if ( newAnimation < 0 || newAnimation >= MAX_ANIMATIONS ) {
		trap_Error( va("Bad animation number: %i", newAnimation) );
	}

#ifdef TMNTPLAYERSYS
	anim = &ci->playercfg.animations[ newAnimation ];
#else
	anim = &ci->animations[ newAnimation ];
#endif

	lf->animation = anim;
	lf->animationTime = lf->frameTime + anim->initialLerp;
}


/*
===============
UI_RunLerpFrame
===============
*/
static void UI_RunLerpFrame( playerInfo_t *ci, lerpFrame_t *lf, int newAnimation ) {
	int			f;
	animation_t	*anim;

	// see if the animation sequence is switching
	if ( newAnimation != lf->animationNumber || !lf->animation ) {
		UI_SetLerpFrameAnimation( ci, lf, newAnimation );
	}

	// if we have passed the current frame, move it to
	// oldFrame and calculate a new frame
	if ( dp_realtime >= lf->frameTime ) {
		lf->oldFrame = lf->frame;
		lf->oldFrameTime = lf->frameTime;

		// get the next frame based on the animation
		anim = lf->animation;
		if ( dp_realtime < lf->animationTime ) {
			lf->frameTime = lf->animationTime;		// initial lerp
		} else {
			lf->frameTime = lf->oldFrameTime + anim->frameLerp;
		}
		f = ( lf->frameTime - lf->animationTime ) / anim->frameLerp;
		if ( f >= anim->numFrames ) {
			f -= anim->numFrames;
			if ( anim->loopFrames ) {
				f %= anim->loopFrames;
				f += anim->numFrames - anim->loopFrames;
			} else {
				f = anim->numFrames - 1;
				// the animation is stuck at the end, so it
				// can immediately transition to another sequence
				lf->frameTime = dp_realtime;
			}
		}
		lf->frame = anim->firstFrame + f;
		if ( dp_realtime > lf->frameTime ) {
			lf->frameTime = dp_realtime;
		}
	}

	if ( lf->frameTime > dp_realtime + 200 ) {
		lf->frameTime = dp_realtime;
	}

	if ( lf->oldFrameTime > dp_realtime ) {
		lf->oldFrameTime = dp_realtime;
	}
	// calculate current lerp value
	if ( lf->frameTime == lf->oldFrameTime ) {
		lf->backlerp = 0;
	} else {
		lf->backlerp = 1.0 - (float)( dp_realtime - lf->oldFrameTime ) / ( lf->frameTime - lf->oldFrameTime );
	}
}
#endif


/*
===============
UI_PlayerAnimation
===============
*/
static void UI_PlayerAnimation( playerInfo_t *pi, int *legsOld, int *legs, float *legsBackLerp,
						int *torsoOld, int *torso, float *torsoBackLerp ) {

	// legs animation
	pi->legsAnimationTimer -= uiInfo.uiDC.frameTime;
	if ( pi->legsAnimationTimer < 0 ) {
		pi->legsAnimationTimer = 0;
	}

	UI_LegsSequencing( pi );

	if ( pi->legs.yawing && ( pi->legsAnim & ~ANIM_TOGGLEBIT ) == LEGS_IDLE ) {
#ifdef IOQ3ZTM // LERP_FRAME_CLIENT_LESS
		BG_RunLerpFrame( &pi->legs,
#ifdef TMNTPLAYERSYS
			pi->playercfg.animations,
#else
			pi->animations,
#endif
			LEGS_TURN, dp_realtime, 1.0f );
#else
		UI_RunLerpFrame( pi, &pi->legs, LEGS_TURN );
#endif
	} else {
#ifdef IOQ3ZTM // LERP_FRAME_CLIENT_LESS
		BG_RunLerpFrame( &pi->legs,
#ifdef TMNTPLAYERSYS
			pi->playercfg.animations,
#else
			pi->animations,
#endif
			pi->legsAnim, dp_realtime, 1.0f );
#else
		UI_RunLerpFrame( pi, &pi->legs, pi->legsAnim );
#endif
	}
	*legsOld = pi->legs.oldFrame;
	*legs = pi->legs.frame;
	*legsBackLerp = pi->legs.backlerp;

	// torso animation
	pi->torsoAnimationTimer -= uiInfo.uiDC.frameTime;
	if ( pi->torsoAnimationTimer < 0 ) {
		pi->torsoAnimationTimer = 0;
	}

	UI_TorsoSequencing( pi );

#ifdef IOQ3ZTM // LERP_FRAME_CLIENT_LESS
	BG_RunLerpFrame( &pi->torso,
#ifdef TMNTPLAYERSYS
		pi->playercfg.animations,
#else
		pi->animations,
#endif
		pi->torsoAnim, dp_realtime, 1.0f );
#else
	UI_RunLerpFrame( pi, &pi->torso, pi->torsoAnim );
#endif
	*torsoOld = pi->torso.oldFrame;
	*torso = pi->torso.frame;
	*torsoBackLerp = pi->torso.backlerp;
}


#ifndef IOQ3ZTM // BG_SWING_ANGLES
/*
==================
UI_SwingAngles
==================
*/
static void UI_SwingAngles( float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging ) {
	float	swing;
	float	move;
	float	scale;

	if ( !*swinging ) {
		// see if a swing should be started
		swing = AngleSubtract( *angle, destination );
		if ( swing > swingTolerance || swing < -swingTolerance ) {
			*swinging = qtrue;
		}
	}

	if ( !*swinging ) {
		return;
	}
	
	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract( destination, *angle );
	scale = fabs( swing );
	if ( scale < swingTolerance * 0.5 ) {
		scale = 0.5;
	} else if ( scale < swingTolerance ) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	// swing towards the destination angle
	if ( swing >= 0 ) {
		move = uiInfo.uiDC.frameTime * scale * speed;
		if ( move >= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	} else if ( swing < 0 ) {
		move = uiInfo.uiDC.frameTime * scale * -speed;
		if ( move <= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	}

	// clamp to no more than tolerance
	swing = AngleSubtract( destination, *angle );
	if ( swing > clampTolerance ) {
		*angle = AngleMod( destination - (clampTolerance - 1) );
	} else if ( swing < -clampTolerance ) {
		*angle = AngleMod( destination + (clampTolerance - 1) );
	}
}
#endif


/*
======================
UI_MovedirAdjustment
======================
*/
static float UI_MovedirAdjustment( playerInfo_t *pi ) {
	vec3_t		relativeAngles;
	vec3_t		moveVector;

	VectorSubtract( pi->viewAngles, pi->moveAngles, relativeAngles );
	AngleVectors( relativeAngles, moveVector, NULL, NULL );
	if ( Q_fabs( moveVector[0] ) < 0.01 ) {
		moveVector[0] = 0.0;
	}
	if ( Q_fabs( moveVector[1] ) < 0.01 ) {
		moveVector[1] = 0.0;
	}

	if ( moveVector[1] == 0 && moveVector[0] > 0 ) {
		return 0;
	}
	if ( moveVector[1] < 0 && moveVector[0] > 0 ) {
		return 22;
	}
	if ( moveVector[1] < 0 && moveVector[0] == 0 ) {
		return 45;
	}
	if ( moveVector[1] < 0 && moveVector[0] < 0 ) {
		return -22;
	}
	if ( moveVector[1] == 0 && moveVector[0] < 0 ) {
		return 0;
	}
	if ( moveVector[1] > 0 && moveVector[0] < 0 ) {
		return 22;
	}
	if ( moveVector[1] > 0 && moveVector[0] == 0 ) {
		return  -45;
	}

	return -22;
}


/*
===============
UI_PlayerAngles
===============
*/
static void UI_PlayerAngles( playerInfo_t *pi, vec3_t legs[3], vec3_t torso[3], vec3_t head[3] ) {
	vec3_t		legsAngles, torsoAngles, headAngles;
	float		dest;
	float		adjust;

	VectorCopy( pi->viewAngles, headAngles );
	headAngles[YAW] = AngleMod( headAngles[YAW] );
	VectorClear( legsAngles );
	VectorClear( torsoAngles );

	// --------- yaw -------------

	// allow yaw to drift a bit
	if ( ( pi->legsAnim & ~ANIM_TOGGLEBIT ) != LEGS_IDLE 
		|| ( pi->torsoAnim & ~ANIM_TOGGLEBIT ) !=
#ifdef TMNTWEAPSYS
		BG_TorsoStandForWeapon(pi->realWeapon)
#else
		TORSO_STAND
#endif
		  ) {
		// if not standing still, always point all in the same direction
		pi->torso.yawing = qtrue;	// always center
		pi->torso.pitching = qtrue;	// always center
		pi->legs.yawing = qtrue;	// always center
	}

	// adjust legs for movement dir
	adjust = UI_MovedirAdjustment( pi );
	legsAngles[YAW] = headAngles[YAW] + adjust;
	torsoAngles[YAW] = headAngles[YAW] + 0.25 * adjust;


	// torso
#ifdef IOQ3ZTM // BG_SWING_ANGLES
	BG_SwingAngles( torsoAngles[YAW], 25, 90, BG_SWINGSPEED, &pi->torso.yawAngle, &pi->torso.yawing, uiInfo.uiDC.frameTime );
	BG_SwingAngles( legsAngles[YAW], 40, 90, BG_SWINGSPEED, &pi->legs.yawAngle, &pi->legs.yawing, uiInfo.uiDC.frameTime );
#else
	UI_SwingAngles( torsoAngles[YAW], 25, 90, SWINGSPEED, &pi->torso.yawAngle, &pi->torso.yawing );
	UI_SwingAngles( legsAngles[YAW], 40, 90, SWINGSPEED, &pi->legs.yawAngle, &pi->legs.yawing );
#endif

	torsoAngles[YAW] = pi->torso.yawAngle;
	legsAngles[YAW] = pi->legs.yawAngle;

	// --------- pitch -------------

	// only show a fraction of the pitch angle in the torso
	if ( headAngles[PITCH] > 180 ) {
		dest = (-360 + headAngles[PITCH]) * 0.75;
	} else {
		dest = headAngles[PITCH] * 0.75;
	}
#ifdef IOQ3ZTM // BG_SWING_ANGLES
	BG_SwingAngles( dest, 15, 30, 0.1f, &pi->torso.pitchAngle, &pi->torso.pitching, uiInfo.uiDC.frameTime );
#else
	UI_SwingAngles( dest, 15, 30, 0.1f, &pi->torso.pitchAngle, &pi->torso.pitching );
#endif
	torsoAngles[PITCH] = pi->torso.pitchAngle;

	// pull the angles back out of the hierarchial chain
	AnglesSubtract( headAngles, torsoAngles, headAngles );
	AnglesSubtract( torsoAngles, legsAngles, torsoAngles );
	AnglesToAxis( legsAngles, legs );
	AnglesToAxis( torsoAngles, torso );
	AnglesToAxis( headAngles, head );
}


/*
===============
UI_PlayerFloatSprite
===============
*/
static void UI_PlayerFloatSprite( playerInfo_t *pi, vec3_t origin, qhandle_t shader ) {
	refEntity_t		ent;

	memset( &ent, 0, sizeof( ent ) );
	VectorCopy( origin, ent.origin );
	ent.origin[2] += 48;
	ent.reType = RT_SPRITE;
	ent.customShader = shader;
	ent.radius = 10;
	ent.renderfx = 0;
	trap_R_AddRefEntityToScene( &ent );
}


/*
======================
UI_MachinegunSpinAngle
======================
*/
float	UI_MachinegunSpinAngle( playerInfo_t *pi ) {
	int		delta;
	float	angle;
	float	speed;
	int		torsoAnim;

	delta = dp_realtime - pi->barrelTime;
	if ( pi->barrelSpinning ) {
		angle = pi->barrelAngle + delta * SPIN_SPEED;
	} else {
		if ( delta > COAST_TIME ) {
			delta = COAST_TIME;
		}

		speed = 0.5 * ( SPIN_SPEED + (float)( COAST_TIME - delta ) / COAST_TIME );
		angle = pi->barrelAngle + delta * speed;
	}

	torsoAnim = pi->torsoAnim  & ~ANIM_TOGGLEBIT;
#ifdef TMNTWEAPSYS
	if (BG_PlayerAttackAnim(torsoAnim))
#else
	if( torsoAnim == TORSO_ATTACK2 )
#endif
	{
		torsoAnim = TORSO_ATTACK;
	}
	if ( pi->barrelSpinning == !(torsoAnim == TORSO_ATTACK) ) {
		pi->barrelTime = dp_realtime;
		pi->barrelAngle = AngleMod( angle );
		pi->barrelSpinning = !!(torsoAnim == TORSO_ATTACK);
	}

	return angle;
}


/*
===============
UI_DrawPlayer
===============
*/
void UI_DrawPlayer( float x, float y, float w, float h, playerInfo_t *pi, int time ) {
	refdef_t		refdef;
	refEntity_t		legs;
	refEntity_t		torso;
	refEntity_t		head;
	refEntity_t		gun;
#ifdef TMNTWEAPSYS
	refEntity_t		gun_left;
#endif
	refEntity_t		barrel;
	refEntity_t		flash;
	vec3_t			origin;
	int				renderfx;
	vec3_t			mins = {-16, -16, -24};
	vec3_t			maxs = {16, 16, 32};
	float			len;
	float			xx;

	if ( !pi->legsModel || !pi->torsoModel || !pi->headModel
#ifdef TMNTPLAYERSYS
	|| !pi->playercfg.animations[0].numFrames ) {
#else
	|| !pi->animations[0].numFrames ) {
#endif
		return;
	}

	// this allows the ui to cache the player model on the main menu
	if (w == 0 || h == 0) {
		return;
	}

	dp_realtime = time;

	if ( pi->pendingWeapon != -1 && dp_realtime > pi->weaponTimer ) {
		pi->weapon = pi->pendingWeapon;
		pi->lastWeapon = pi->pendingWeapon;
		pi->pendingWeapon = -1;
		pi->weaponTimer = 0;
#ifndef TMNTWEAPSYS_EX
		if( pi->currentWeapon != pi->weapon ) {
			trap_S_StartLocalSound( weaponChangeSound, CHAN_LOCAL );
		}
#endif
	}

	UI_AdjustFrom640( &x, &y, &w, &h );

	y -= jumpHeight;

	memset( &refdef, 0, sizeof( refdef ) );
	memset( &legs, 0, sizeof(legs) );
	memset( &torso, 0, sizeof(torso) );
	memset( &head, 0, sizeof(head) );

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.fov_x = (int)((float)refdef.width / 640.0f * 90.0f);
	xx = refdef.width / tan( refdef.fov_x / 360 * M_PI );
	refdef.fov_y = atan2( refdef.height, xx );
	refdef.fov_y *= ( 360 / (float)M_PI );

	// calculate distance so the player nearly fills the box
	len = 0.7 * ( maxs[2] - mins[2] );		
	origin[0] = len / tan( DEG2RAD(refdef.fov_x) * 0.5 );
	origin[1] = 0.5 * ( mins[1] + maxs[1] );
	origin[2] = -0.5 * ( mins[2] + maxs[2] );

	refdef.time = dp_realtime;

	trap_R_ClearScene();

	// get the rotation information
	UI_PlayerAngles( pi, legs.axis, torso.axis, head.axis );
	
	// get the animation state (after rotation, to allow feet shuffle)
	UI_PlayerAnimation( pi, &legs.oldframe, &legs.frame, &legs.backlerp,
		 &torso.oldframe, &torso.frame, &torso.backlerp );

	renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;

	//
	// add the legs
	//
	legs.hModel = pi->legsModel;
	legs.customSkin = pi->legsSkin;

	VectorCopy( origin, legs.origin );

	VectorCopy( origin, legs.lightingOrigin );
	legs.renderfx = renderfx;
	VectorCopy (legs.origin, legs.oldorigin);

	trap_R_AddRefEntityToScene( &legs );

	if (!legs.hModel) {
		return;
	}

	//
	// add the torso
	//
	torso.hModel = pi->torsoModel;
	if (!torso.hModel) {
		return;
	}

	torso.customSkin = pi->torsoSkin;

	VectorCopy( origin, torso.lightingOrigin );

	UI_PositionRotatedEntityOnTag( &torso, &legs, pi->legsModel, "tag_torso");

	torso.renderfx = renderfx;

	trap_R_AddRefEntityToScene( &torso );

	//
	// add the head
	//
	head.hModel = pi->headModel;
	if (!head.hModel) {
		return;
	}
	head.customSkin = pi->headSkin;

	VectorCopy( origin, head.lightingOrigin );

	UI_PositionRotatedEntityOnTag( &head, &torso, pi->torsoModel, "tag_head");

	head.renderfx = renderfx;

	trap_R_AddRefEntityToScene( &head );

	//
	// add the gun
	//
	if ( pi->currentWeapon != WP_NONE ) {
		memset( &gun, 0, sizeof(gun) );
		gun.hModel = pi->weaponModel;
		VectorCopy( origin, gun.lightingOrigin );
#ifdef TMNT_SUPPORTQ3
		if (!UI_PositionEntityOnTag( &gun, &torso, pi->torsoModel, "tag_hand_primary"))
		{
		UI_PositionEntityOnTag( &gun, &torso, pi->torsoModel, "tag_weapon");
		}
#elif defined TMNTPLAYERS
		UI_PositionEntityOnTag( &gun, &torso, pi->torsoModel, "tag_hand_primary");
#else
		UI_PositionEntityOnTag( &gun, &torso, pi->torsoModel, "tag_weapon");
#endif
		gun.renderfx = renderfx;
		trap_R_AddRefEntityToScene( &gun );
#ifdef TMNTWEAPSYS
		// Secondary weapon
		memset( &gun_left, 0, sizeof(gun_left) );
		gun_left.hModel = pi->weaponModel2;
		VectorCopy( origin, gun_left.lightingOrigin );

		if (gun_left.hModel)
		{
#ifdef TMNT_SUPPORTQ3
			if (!UI_PositionEntityOnTag( &gun_left, &torso, pi->torsoModel, "tag_hand_secondary"))
			{
				UI_PositionEntityOnTag( &gun_left, &torso, pi->torsoModel, "tag_flag");
			}
#elif defined TMNTPLAYERS
			UI_PositionEntityOnTag( &gun_left, &torso, pi->torsoModel, "tag_hand_secondary");
#else
			UI_PositionEntityOnTag( &gun_left, &torso, pi->torsoModel, "tag_flag");
#endif
			gun_left.renderfx = renderfx;
			trap_R_AddRefEntityToScene( &gun_left );
		}
#endif
	}

	//
	// add the spinning barrel
	//
#ifdef TMNTWEAPSYS
	if ( pi->barrelModel )
#else
	if ( pi->realWeapon == WP_MACHINEGUN || pi->realWeapon == WP_GAUNTLET || pi->realWeapon == WP_BFG )
#endif
	{
		vec3_t	angles;

		memset( &barrel, 0, sizeof(barrel) );
		VectorCopy( origin, barrel.lightingOrigin );
		barrel.renderfx = renderfx;

		barrel.hModel = pi->barrelModel;
#ifdef TMNTWEAPSYS
		VectorClear(angles);
		if (bg_weapongroupinfo[pi->realWeapon].weapon[0]->barrelSpin != BS_NONE)
		{
			angles[bg_weapongroupinfo[pi->realWeapon].weapon[0]->barrelSpin]
						= UI_MachinegunSpinAngle( pi );
		}
#else
		angles[YAW] = 0;
		angles[PITCH] = 0;
		angles[ROLL] = UI_MachinegunSpinAngle( pi );
		if( pi->realWeapon == WP_GAUNTLET || pi->realWeapon == WP_BFG ) {
			angles[PITCH] = angles[ROLL];
			angles[ROLL] = 0;
		}
#endif
		AnglesToAxis( angles, barrel.axis );

		UI_PositionRotatedEntityOnTag( &barrel, &gun, pi->weaponModel, "tag_barrel");

		trap_R_AddRefEntityToScene( &barrel );
	}

	//
	// add muzzle flash
	//
	if ( dp_realtime <= pi->muzzleFlashTime ) {
		if ( pi->flashModel ) {
			memset( &flash, 0, sizeof(flash) );
			flash.hModel = pi->flashModel;
			VectorCopy( origin, flash.lightingOrigin );
			UI_PositionEntityOnTag( &flash, &gun, pi->weaponModel, "tag_flash");
			flash.renderfx = renderfx;
			trap_R_AddRefEntityToScene( &flash );
		}

		// make a dlight for the flash
		if ( pi->flashDlightColor[0] || pi->flashDlightColor[1] || pi->flashDlightColor[2] ) {
			trap_R_AddLightToScene( flash.origin, 200 + (rand()&31), pi->flashDlightColor[0],
				pi->flashDlightColor[1], pi->flashDlightColor[2] );
		}
	}

	//
	// add the chat icon
	//
	if ( pi->chat ) {
#ifdef TMNTDATA // shaders
		UI_PlayerFloatSprite( pi, origin, trap_R_RegisterShaderNoMip( "sprites/talkBalloon" ) );
#else
		UI_PlayerFloatSprite( pi, origin, trap_R_RegisterShaderNoMip( "sprites/balloon3" ) );
#endif
	}

	//
	// add an accent light
	//
	origin[0] -= 100;	// + = behind, - = in front
	origin[1] += 100;	// + = left, - = right
	origin[2] += 100;	// + = above, - = below
	trap_R_AddLightToScene( origin, 500, 1.0, 1.0, 1.0 );

	origin[0] -= 100;
	origin[1] -= 100;
	origin[2] -= 100;
	trap_R_AddLightToScene( origin, 500, 1.0, 0.0, 0.0 );

	trap_R_RenderScene( &refdef );
}

/*
==========================
UI_FileExists
==========================
*/
static qboolean	UI_FileExists(const char *filename) {
	int len;

	len = trap_FS_FOpenFile( filename, NULL, FS_READ );
	if (len>0) {
		return qtrue;
	}
	return qfalse;
}

/*
==========================
UI_FindClientHeadFile
==========================
*/
static qboolean	UI_FindClientHeadFile( char *filename, int length, const char *teamName, const char *headModelName, const char *headSkinName, const char *base, const char *ext ) {
	char *team, *headsFolder;
	int i;

	team = "default";

	if ( headModelName[0] == '*' ) {
		headsFolder = "heads/";
		headModelName++;
	}
	else {
		headsFolder = "";
	}
	while(1) {
		for ( i = 0; i < 2; i++ ) {
			if ( i == 0 && teamName && *teamName ) {
				Com_sprintf( filename, length, "models/players/%s%s/%s/%s%s_%s.%s", headsFolder, headModelName, headSkinName, teamName, base, team, ext );
			}
			else {
				Com_sprintf( filename, length, "models/players/%s%s/%s/%s_%s.%s", headsFolder, headModelName, headSkinName, base, team, ext );
			}
			if ( UI_FileExists( filename ) ) {
				return qtrue;
			}
			if ( i == 0 && teamName && *teamName ) {
				Com_sprintf( filename, length, "models/players/%s%s/%s%s_%s.%s", headsFolder, headModelName, teamName, base, headSkinName, ext );
			}
			else {
				Com_sprintf( filename, length, "models/players/%s%s/%s_%s.%s", headsFolder, headModelName, base, headSkinName, ext );
			}
			if ( UI_FileExists( filename ) ) {
				return qtrue;
			}
			if ( !teamName || !*teamName ) {
				break;
			}
		}
		// if tried the heads folder first
		if ( headsFolder[0] ) {
			break;
		}
		headsFolder = "heads/";
	}

	return qfalse;
}

/*
==========================
UI_RegisterClientSkin
==========================
*/
static qboolean	UI_RegisterClientSkin( playerInfo_t *pi, const char *modelName, const char *skinName, const char *headModelName, const char *headSkinName , const char *teamName) {
	char		filename[MAX_QPATH*2];
#ifdef IOQ3ZTM // PLAYER_DIR
	int i;

	for (i=0; bg_playerDirs[i] != NULL; i++)
	{
		if (i == 0 || !pi->legsSkin) {
			if (teamName && *teamName) {
				Com_sprintf( filename, sizeof( filename ), "%s/%s/%s/lower_%s.skin", bg_playerDirs[i], modelName, teamName, skinName );
			} else {
				Com_sprintf( filename, sizeof( filename ), "%s/%s/lower_%s.skin", bg_playerDirs[i], modelName, skinName );
			}
			pi->legsSkin = trap_R_RegisterSkin( filename );
		}

		if (i == 0 || !pi->torsoSkin) {
			if (teamName && *teamName) {
				Com_sprintf( filename, sizeof( filename ), "%s/%s/%s/upper_%s.skin", bg_playerDirs[i], modelName, teamName, skinName );
			} else {
				Com_sprintf( filename, sizeof( filename ), "%s/%s/upper_%s.skin", bg_playerDirs[i], modelName, skinName );
			}
			pi->torsoSkin = trap_R_RegisterSkin( filename );
		}
	}
#else
	if (teamName && *teamName) {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/%s/lower_%s.skin", modelName, teamName, skinName );
	} else {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower_%s.skin", modelName, skinName );
	}
	pi->legsSkin = trap_R_RegisterSkin( filename );
	if (!pi->legsSkin) {
		if (teamName && *teamName) {
			Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/%s/lower_%s.skin", modelName, teamName, skinName );
		} else {
			Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/lower_%s.skin", modelName, skinName );
		}
		pi->legsSkin = trap_R_RegisterSkin( filename );
	}

	if (teamName && *teamName) {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/%s/upper_%s.skin", modelName, teamName, skinName );
	} else {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper_%s.skin", modelName, skinName );
	}
	pi->torsoSkin = trap_R_RegisterSkin( filename );
	if (!pi->torsoSkin) {
		if (teamName && *teamName) {
			Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/%s/upper_%s.skin", modelName, teamName, skinName );
		} else {
			Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/upper_%s.skin", modelName, skinName );
		}
		pi->torsoSkin = trap_R_RegisterSkin( filename );
	}
#endif

	if ( UI_FindClientHeadFile( filename, sizeof(filename), teamName, headModelName, headSkinName, "head", "skin" ) ) {
		pi->headSkin = trap_R_RegisterSkin( filename );
	}

	if ( !pi->legsSkin || !pi->torsoSkin || !pi->headSkin ) {
		return qfalse;
	}

	return qtrue;
}


#ifndef TMNTPLAYERSYS // Moved to bg_misc.c BG_LoadPlayerCFGFile
/*
======================
UI_ParseAnimationFile
======================
*/
static qboolean UI_ParseAnimationFile( const char *filename, animation_t *animations ) {
	char		*text_p, *prev;
	int			len;
	int			i;
	char		*token;
	float		fps;
	int			skip;
	char		text[20000];
	fileHandle_t	f;

	memset( animations, 0, sizeof( animation_t ) * MAX_ANIMATIONS );

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= ( sizeof( text ) - 1 ) ) {
		Com_Printf( "File %s too long\n", filename );
		trap_FS_FCloseFile( f );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	COM_Compress(text);

	// parse the text
	text_p = text;
	skip = 0;	// quite the compiler warning

	// read optional parameters
	while ( 1 ) {
		prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		if ( !Q_stricmp( token, "footsteps" ) ) {
			token = COM_Parse( &text_p );
			if ( !token ) {
				break;
			}
			continue;
		} else if ( !Q_stricmp( token, "headoffset" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !token ) {
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "sex" ) ) {
			token = COM_Parse( &text_p );
			if ( !token ) {
				break;
			}
			continue;
		}

		// if it is a number, start parsing animations
		if ( token[0] >= '0' && token[0] <= '9' ) {
			text_p = prev;	// unget the token
			break;
		}

		Com_Printf( "unknown token '%s' is %s\n", token, filename );
	}

	// read information for each frame
	for ( i = 0 ; i < MAX_ANIMATIONS ; i++ ) {

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].firstFrame = atoi( token );
		// leg only frames are adjusted to not count the upper body only frames
		if ( i == LEGS_WALKCR ) {
			skip = animations[LEGS_WALKCR].firstFrame - animations[TORSO_GESTURE].firstFrame;
		}
		if ( i >= LEGS_WALKCR ) {
			animations[i].firstFrame -= skip;
		}

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].numFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].loopFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		fps = atof( token );
		if ( fps == 0 ) {
			fps = 1;
		}
		animations[i].frameLerp = 1000 / fps;
		animations[i].initialLerp = 1000 / fps;
	}

	if ( i != MAX_ANIMATIONS ) {
		Com_Printf( "Error parsing animation file: %s", filename );
		return qfalse;
	}

	return qtrue;
}
#endif // TMNTPLAYERSYS

/*
==========================
UI_RegisterClientModelname
==========================
*/
qboolean UI_RegisterClientModelname( playerInfo_t *pi, const char *modelSkinName, const char *headModelSkinName, const char *teamName ) {
	char		modelName[MAX_QPATH];
	char		skinName[MAX_QPATH];
	char		headModelName[MAX_QPATH];
	char		headSkinName[MAX_QPATH];
	char		filename[MAX_QPATH];
	char		*slash;
#ifdef IOQ3ZTM // PLAYER_DIR
	int			i;
#endif

	pi->torsoModel = 0;
	pi->headModel = 0;

	if ( !modelSkinName[0] ) {
		return qfalse;
	}

	Q_strncpyz( modelName, modelSkinName, sizeof( modelName ) );

	slash = strchr( modelName, '/' );
	if ( !slash ) {
		// modelName did not include a skin name
		Q_strncpyz( skinName, "default", sizeof( skinName ) );
	} else {
		Q_strncpyz( skinName, slash + 1, sizeof( skinName ) );
		*slash = '\0';
	}

	Q_strncpyz( headModelName, headModelSkinName, sizeof( headModelName ) );
	slash = strchr( headModelName, '/' );
	if ( !slash ) {
		// modelName did not include a skin name
		Q_strncpyz( headSkinName, "default", sizeof( skinName ) );
	} else {
		Q_strncpyz( headSkinName, slash + 1, sizeof( skinName ) );
		*slash = '\0';
	}

	// load cmodels before models so filecache works

#ifdef IOQ3ZTM // PLAYER_DIR // Same code in CG_RegisterClientModelname and q3_ui
	for (i=0; bg_playerDirs[i] != NULL; i++)
	{
		if (i == 0 || !pi->legsModel)
		{
			Com_sprintf( filename, sizeof( filename ), "%s/%s/lower.md3", bg_playerDirs[i], modelName );
			pi->legsModel = trap_R_RegisterModel( filename );
		}
		if (i == 0 || !pi->torsoModel )
		{
			Com_sprintf( filename, sizeof( filename ), "%s/%s/upper.md3", bg_playerDirs[i], modelName );
			pi->torsoModel = trap_R_RegisterModel( filename );
		}
	}

	// failed?
	if ( !pi->legsModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}
	if ( !pi->torsoModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}

	// load head model
	pi->headModel = 0;

	if (headModelName[0] == '*' ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/heads/%s/%s.md3", &headModelName[1], &headModelName[1] );
		pi->headModel = trap_R_RegisterModel( filename );
	}

	if (!pi->headModel)
	{
		for (i=0; bg_playerDirs[i] != NULL; i++)
		{
			if (headModelName[0] == '*')
			{
				Com_sprintf( filename, sizeof( filename ), "%s/%s/head.md3", bg_playerDirs[i], &headModelName[1] );
			}
			else
			{
				Com_sprintf( filename, sizeof( filename ), "%s/%s/head.md3", bg_playerDirs[i], headModelName );
			}
			pi->headModel = trap_R_RegisterModel( filename );
			if (pi->headModel)
			{
				break;
			}
		}
	}

	if ( !pi->headModel && headModelName[0] != '*') {
		Com_sprintf( filename, sizeof( filename ), "models/players/heads/%s/%s.md3", headModelName, headModelName );
		pi->headModel = trap_R_RegisterModel( filename );
	}

	if (!pi->headModel) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}
#else
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", modelName );
	pi->legsModel = trap_R_RegisterModel( filename );
	if ( !pi->legsModel ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/lower.md3", modelName );
		pi->legsModel = trap_R_RegisterModel( filename );
		if ( !pi->legsModel ) {
			Com_Printf( "Failed to load model file %s\n", filename );
			return qfalse;
		}
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", modelName );
	pi->torsoModel = trap_R_RegisterModel( filename );
	if ( !pi->torsoModel ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/upper.md3", modelName );
		pi->torsoModel = trap_R_RegisterModel( filename );
		if ( !pi->torsoModel ) {
			Com_Printf( "Failed to load model file %s\n", filename );
			return qfalse;
		}
	}

	if (headModelName[0] == '*' ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/heads/%s/%s.md3", &headModelName[1], &headModelName[1] );
	}
	else {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/head.md3", headModelName );
	}
	pi->headModel = trap_R_RegisterModel( filename );
	if ( !pi->headModel && headModelName[0] != '*') {
		Com_sprintf( filename, sizeof( filename ), "models/players/heads/%s/%s.md3", headModelName, headModelName );
		pi->headModel = trap_R_RegisterModel( filename );
	}

	if (!pi->headModel) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}
#endif

	// if any skins failed to load, fall back to default
#ifdef TMNT // Turtle Man: FIXME: Use same skin loading in ui as in cgame.
#endif
	if ( !UI_RegisterClientSkin( pi, modelName, skinName, headModelName, headSkinName, teamName) ) {
		if ( !UI_RegisterClientSkin( pi, modelName, "default", headModelName, "default", teamName ) ) {
			Com_Printf( "Failed to load skin file: %s : %s\n", modelName, skinName );
			return qfalse;
		}
	}

	// load the animations
#ifdef TMNTPLAYERSYS
	return BG_LoadPlayerCFGFile(&pi->playercfg, modelName, headModelName);
#else
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/animation.cfg", modelName );
	if ( !UI_ParseAnimationFile( filename, pi->animations ) ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/animation.cfg", modelName );
		if ( !UI_ParseAnimationFile( filename, pi->animations ) ) {
			Com_Printf( "Failed to load animation file %s\n", filename );
			return qfalse;
		}
	}

	return qtrue;
#endif
}


/*
===============
UI_PlayerInfo_SetModel
===============
*/
void UI_PlayerInfo_SetModel( playerInfo_t *pi, const char *model, const char *headmodel, char *teamName ) {
	memset( pi, 0, sizeof(*pi) );
	UI_RegisterClientModelname( pi, model, headmodel, teamName );
#if defined TMNTPLAYERSYS && defined TMNTWEAPSYS
	pi->weapon = pi->playercfg.default_weapon;
#else
	pi->weapon = WP_MACHINEGUN;
#endif
	pi->currentWeapon = pi->weapon;
	pi->lastWeapon = pi->weapon;
	pi->pendingWeapon = -1;
	pi->weaponTimer = 0;
	pi->chat = qfalse;
	pi->newModel = qtrue;
	UI_PlayerInfo_SetWeapon( pi, pi->weapon );
}


/*
===============
UI_PlayerInfo_SetInfo
===============
*/
void UI_PlayerInfo_SetInfo( playerInfo_t *pi, int legsAnim, int torsoAnim, vec3_t viewAngles, vec3_t moveAngles, weapon_t weaponNumber, qboolean chat ) {
	int			currentAnim;
	weapon_t	weaponNum;

	pi->chat = chat;

	// view angles
	VectorCopy( viewAngles, pi->viewAngles );

	// move angles
	VectorCopy( moveAngles, pi->moveAngles );

	if ( pi->newModel ) {
		pi->newModel = qfalse;

		jumpHeight = 0;
		pi->pendingLegsAnim = 0;
		UI_ForceLegsAnim( pi, legsAnim );
		pi->legs.yawAngle = viewAngles[YAW];
		pi->legs.yawing = qfalse;

		pi->pendingTorsoAnim = 0;
		UI_ForceTorsoAnim( pi, torsoAnim );
		pi->torso.yawAngle = viewAngles[YAW];
		pi->torso.yawing = qfalse;

		if ( weaponNumber != -1 ) {
			pi->weapon = weaponNumber;
			pi->currentWeapon = weaponNumber;
			pi->lastWeapon = weaponNumber;
			pi->pendingWeapon = -1;
			pi->weaponTimer = 0;
			UI_PlayerInfo_SetWeapon( pi, pi->weapon );
		}

		return;
	}

	// weapon
	if ( weaponNumber == -1 ) {
		pi->pendingWeapon = -1;
		pi->weaponTimer = 0;
	}
	else if ( weaponNumber != WP_NONE ) {
		pi->pendingWeapon = weaponNumber;
		pi->weaponTimer = dp_realtime + UI_TIMER_WEAPON_DELAY;
	}
	weaponNum = pi->lastWeapon;
	pi->weapon = weaponNum;

	if ( torsoAnim == BOTH_DEATH1 || legsAnim == BOTH_DEATH1 ) {
		torsoAnim = legsAnim = BOTH_DEATH1;
		pi->weapon = pi->currentWeapon = WP_NONE;
		UI_PlayerInfo_SetWeapon( pi, pi->weapon );

		jumpHeight = 0;
		pi->pendingLegsAnim = 0;
		UI_ForceLegsAnim( pi, legsAnim );

		pi->pendingTorsoAnim = 0;
		UI_ForceTorsoAnim( pi, torsoAnim );

		return;
	}

	// leg animation
	currentAnim = pi->legsAnim & ~ANIM_TOGGLEBIT;
	if ( legsAnim != LEGS_JUMP && ( currentAnim == LEGS_JUMP || currentAnim == LEGS_LAND ) ) {
		pi->pendingLegsAnim = legsAnim;
	}
	else if ( legsAnim != currentAnim ) {
		jumpHeight = 0;
		pi->pendingLegsAnim = 0;
		UI_ForceLegsAnim( pi, legsAnim );
	}

	// torso animation
#ifdef TMNTWEAPSYS
	if (BG_PlayerStandAnim(torsoAnim))
	{
		torsoAnim = BG_TorsoStandForWeapon(weaponNum);
	}
#else
	if ( torsoAnim == TORSO_STAND || torsoAnim == TORSO_STAND2 ) {
		if ( weaponNum == WP_NONE || weaponNum == WP_GAUNTLET ) {
			torsoAnim = TORSO_STAND2;
		}
		else {
			torsoAnim = TORSO_STAND;
		}
	}
#endif

#ifdef TMNTWEAPSYS
	if (BG_PlayerAttackAnim(torsoAnim))
	{
		torsoAnim = BG_TorsoAttackForWeapon(weaponNum);
		if (!BG_WeapTypeIsMelee(BG_WeaponTypeForNum(weaponNum)))
		{
			pi->muzzleFlashTime = dp_realtime + UI_TIMER_MUZZLE_FLASH;
		}
		//FIXME play firing sound here
	}
#else
	if ( torsoAnim == TORSO_ATTACK || torsoAnim == TORSO_ATTACK2 ) {
		if ( weaponNum == WP_NONE || weaponNum == WP_GAUNTLET ) {
			torsoAnim = TORSO_ATTACK2;
		}
		else {
			torsoAnim = TORSO_ATTACK;
		}
		pi->muzzleFlashTime = dp_realtime + UI_TIMER_MUZZLE_FLASH;
		//FIXME play firing sound here
	}
#endif

	currentAnim = pi->torsoAnim & ~ANIM_TOGGLEBIT;

	if ( weaponNum != pi->currentWeapon || currentAnim == TORSO_RAISE || currentAnim == TORSO_DROP ) {
		pi->pendingTorsoAnim = torsoAnim;
	}
	else if ( ( currentAnim == TORSO_GESTURE ||
#ifdef TMNTWEAPSYS
		BG_PlayerAttackAnim(currentAnim)
#else
		currentAnim == TORSO_ATTACK
#endif
	) && ( torsoAnim != currentAnim ) ) {
		pi->pendingTorsoAnim = torsoAnim;
	}
	else if ( torsoAnim != currentAnim ) {
		pi->pendingTorsoAnim = 0;
		UI_ForceTorsoAnim( pi, torsoAnim );
	}
}
