// g_npcmove.c
//
// JPL (jpl@ng.puc.cl) - The Dark Conjunction - ACE Team
//
// takes a npc state and a usercmd as input and returns a modifed npc state
// its really a "bg_pmove.c" (player movement) modified to fit NPC needs

#include "g_local.h"
#include "bg_local.h"

#ifdef SP_NPC

npmove_t		*npm;
npml_t		npml;

// movement parameters
float	npm_stopspeed = 100.0f;
float	npm_duckScale = 0.25f;
float	npm_swimScale = 0.50f;
float	npm_wadeScale = 0.70f;

float	npm_accelerate = 10.0f;
float	npm_airaccelerate = 1.0f;
float	npm_wateraccelerate = 4.0f;
float	npm_flyaccelerate = 8.0f;

float	npm_friction = 6.0f;
float	npm_waterfriction = 1.0f;
float	npm_flightfriction = 3.0f;
float	npm_spectatorfriction = 5.0f;

int		npc_pmove = 0;

void NPC_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap ) {
//	int		i;

	s->eType = ET_NPC;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction
	VectorCopy( ps->velocity, s->pos.trDelta );

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->modelindex2=ps->legsAnim;

/*	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
*/
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = (ps->entityEventSequence-1) & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		if ( ps->entityEventSequence < ps->eventSequence ) {
			ps->entityEventSequence++;
		}
	}

//	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

/*	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			s->powerups |= 1 << i;
		}
	}
*/
	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}



/*
===============
NPM_AddEvent

===============
*/
void NPM_AddEvent( int newEvent ) {
//	BG_AddPredictableEventToPlayerstate( newEvent, 0, npm->ps );
}

/*
===============
PM_AddTouchEnt
===============
*/
void NPM_AddTouchEnt( int entityNum ) {
	int		i;

	if ( entityNum == ENTITYNUM_WORLD ) {
		return;
	}
	if ( npm->numtouch == MAXTOUCH ) {
		return;
	}

	// see if it is already added
	for ( i = 0 ; i < npm->numtouch ; i++ ) {
		if ( npm->touchents[ i ] == entityNum ) {
			return;
		}
	}

	// add it
	npm->touchents[npm->numtouch] = entityNum;
	npm->numtouch++;
}

/*
===================
NPM_StartLegsAnim
===================
*/
void NPM_StartLegsAnim( int anim ) {
	if ( npm->ps->pm_type >= PM_DEAD ) {
		return;
	}
	if ( npm->ps->legsTimer > 0 ) {
		return;		// a high priority animation is running
	}
	npm->ps->legsAnim = ( ( npm->ps->legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT )
		| anim;
}

void NPM_ContinueLegsAnim( int anim ) {


	if ( ( npm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == anim ) {
		return;
	}
	if ( npm->ps->legsTimer > 0 ) {
		return;		// a high priority animation is running
	}
	NPM_StartLegsAnim( anim );
}

void NPM_ForceLegsAnim( int anim ) {
	npm->ps->legsTimer = 0;
	NPM_StartLegsAnim( anim );
}


/*
==================
NPM_ClipVelocity

Slide off of the impacting surface
==================
*/
void NPM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce ) {
	float	backoff;
	float	change;
	int		i;

	backoff = DotProduct (in, normal);

	if ( backoff < 0 ) {
		backoff *= overbounce;
	} else {
		backoff /= overbounce;
	}

	for ( i=0 ; i<3 ; i++ ) {
		change = normal[i]*backoff;
		out[i] = in[i] - change;
	}
}


/*
==================
NPM_Friction

Handles both ground friction and water friction
==================
*/
static void NPM_Friction( void ) {
	vec3_t	vec;
	float	*vel;
	float	speed, newspeed, control;
	float	drop;

	vel = npm->ps->velocity;

	VectorCopy( vel, vec );
	if ( npml.walking ) {
		vec[2] = 0;	// ignore slope movement
	}

	speed = VectorLength(vec);
	if (speed < 1) {
		vel[0] = 0;
		vel[1] = 0;		// allow sinking underwater
		// FIXME: still have z friction underwater?
		return;
	}

	drop = 0;

	// apply ground friction
	if ( npm->waterlevel <= 1 ) {
		if ( npml.walking && !(npml.groundTrace.surfaceFlags & SURF_SLICK) ) {
			// if getting knocked back, no friction
			if ( ! (npm->ps->pm_flags & PMF_TIME_KNOCKBACK) ) {
				control = speed < npm_stopspeed ? npm_stopspeed : speed;
				drop += control*npm_friction*npml.frametime;
			}
		}
	}

	if (npm->ns->inFlight)	// JPL - bat flying has "ground" friction
	{
		control = speed < npm_stopspeed ? npm_stopspeed : speed;
		drop += control*npm_friction*npml.frametime;
	}

	// apply water friction even if just wading
	if ( npm->waterlevel ) {
		drop += speed*npm_waterfriction*npm->waterlevel*npml.frametime;
	}

	// apply flying friction
	if ( npm->ps->powerups[PW_FLIGHT]) {
		drop += speed*npm_flightfriction*npml.frametime;
	}

	if ( npm->ps->pm_type == PM_SPECTATOR) {
		drop += speed*npm_spectatorfriction*npml.frametime;
	}

	// scale the velocity
	newspeed = speed - drop;
	if (newspeed < 0) {
		newspeed = 0;
	}
	newspeed /= speed;

	vel[0] = vel[0] * newspeed;
	vel[1] = vel[1] * newspeed;
	vel[2] = vel[2] * newspeed;
}


/*
==============
NPM_Accelerate

Handles user intended acceleration
==============
*/
static void NPM_Accelerate( vec3_t wishdir, float wishspeed, float accel ) {
	int			i;
	float		addspeed, accelspeed, currentspeed;

	currentspeed = DotProduct (npm->ps->velocity, wishdir);
	addspeed = wishspeed - currentspeed;
	if (addspeed <= 0) {
		return;
	}
	accelspeed = accel*npml.frametime*wishspeed;
	if (accelspeed > addspeed) {
		accelspeed = addspeed;
	}

	for (i=0 ; i<3 ; i++) {
		npm->ps->velocity[i] += accelspeed*wishdir[i];
	}
}



/*
============
NPM_CmdScale

Returns the scale factor to apply to cmd movements
This allows the clients to use axial -127 to 127 values for all directions
without getting a sqrt(2) distortion in speed.
============
*/
static float NPM_CmdScale( usercmd_t *cmd ) {
	int		max;
	float	total;
	float	scale;

	max = abs( cmd->forwardmove );
	if ( abs( cmd->rightmove ) > max ) {
		max = abs( cmd->rightmove );
	}
	if ( abs( cmd->upmove ) > max ) {
		max = abs( cmd->upmove );
	}
	if ( !max ) {
		return 0;
	}

	total = sqrt( cmd->forwardmove * cmd->forwardmove
		+ cmd->rightmove * cmd->rightmove + cmd->upmove * cmd->upmove );
	scale = (float)npm->ps->speed * max / ( 127.0 * total );
//
// Warning fixup
//
	if (npm->ns->toFire==2) scale*=2.7f;		// metlar jump attack!
//
// end fixup
//
	return scale;
}


/*
================
NPM_SetMovementDir

Determine the rotation of the legs reletive
to the facing dir
================
*/
static void NPM_SetMovementDir( void ) {
//	if ( npm->cmd.forwardmove || npm->cmd.rightmove ) {
//		if ( npm->cmd.rightmove == 0 && npm->cmd.forwardmove > 0 ) {
			npm->ps->movementDir = 0;
/*		} else if ( npm->cmd.rightmove < 0 && npm->cmd.forwardmove > 0 ) {
			npm->ps->movementDir = 1;
		} else if ( pm->cmd.rightmove < 0 && pm->cmd.forwardmove == 0 ) {
			pm->ps->movementDir = 2;
		} else if ( pm->cmd.rightmove < 0 && pm->cmd.forwardmove < 0 ) {
			pm->ps->movementDir = 3;
		} else if ( pm->cmd.rightmove == 0 && pm->cmd.forwardmove < 0 ) {
			pm->ps->movementDir = 4;
		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove < 0 ) {
			pm->ps->movementDir = 5;
		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove == 0 ) {
			pm->ps->movementDir = 6;
		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove > 0 ) {
			pm->ps->movementDir = 7;
		}
	} else {
		// if they aren't actively going directly sideways,
		// change the animation to the diagonal so they
		// don't stop too crooked
		if ( pm->ps->movementDir == 2 ) {
			pm->ps->movementDir = 1;
		} else if ( pm->ps->movementDir == 6 ) {
			pm->ps->movementDir = 7;
		}
	}
*/
}


/*
==================
NPM_SlideMove

Returns qtrue if the velocity was clipped in some way
==================
*/
#define	MAX_CLIP_PLANES	5
qboolean	NPM_SlideMove( qboolean gravity ) {
	int			bumpcount, numbumps;
	vec3_t		dir;
	float		d;
	int			numplanes;
	vec3_t		planes[MAX_CLIP_PLANES];
	vec3_t		primal_velocity;
	vec3_t		clipVelocity;
	int			i, j, k;
	trace_t	trace;
	vec3_t		end;
	float		time_left;
	float		into;
	vec3_t		endVelocity;
	vec3_t		endClipVelocity;

	numbumps = 4;

	VectorCopy (npm->ps->velocity, primal_velocity);

	if ( gravity ) {
		VectorCopy( npm->ps->velocity, endVelocity );
		endVelocity[2] -= npm->ps->gravity * npml.frametime;
		npm->ps->velocity[2] = ( npm->ps->velocity[2] + endVelocity[2] ) * 0.5;
		primal_velocity[2] = endVelocity[2];
		if ( npml.groundPlane ) {
			// slide along the ground plane
			NPM_ClipVelocity (npm->ps->velocity, npml.groundTrace.plane.normal,
				npm->ps->velocity, OVERCLIP );
		}
	}

	time_left = npml.frametime;

	// never turn against the ground plane
	if ( npml.groundPlane ) {
		numplanes = 1;
		VectorCopy( npml.groundTrace.plane.normal, planes[0] );
	} else {
		numplanes = 0;
	}

	// never turn against original velocity
	VectorNormalize2( npm->ps->velocity, planes[numplanes] );
	numplanes++;

	for ( bumpcount=0 ; bumpcount < numbumps ; bumpcount++ ) {

		// calculate position we are trying to move to
		VectorMA( npm->ps->origin, time_left, npm->ps->velocity, end );

		// see if we can make it there
		npm->trace ( &trace, npm->ps->origin, npm->mins, npm->maxs, end, npm->ps->clientNum, npm->tracemask);

		if (trace.allsolid) {
			// entity is completely trapped in another solid
			npm->ps->velocity[2] = 0;	// don't build up falling damage, but allow sideways acceleration
			return qtrue;
		}

		if (trace.fraction > 0) {
			// actually covered some distance
			VectorCopy (trace.endpos, npm->ps->origin);
		}

		if (trace.fraction == 1) {
			 break;		// moved the entire distance
		}

		// save entity for contact
		NPM_AddTouchEnt( trace.entityNum );

		time_left -= time_left * trace.fraction;

		if (numplanes >= MAX_CLIP_PLANES) {
			// this shouldn't really happen
			VectorClear( npm->ps->velocity );
			return qtrue;
		}

		//
		// if this is the same plane we hit before, nudge velocity
		// out along it, which fixes some epsilon issues with
		// non-axial planes
		//
		for ( i = 0 ; i < numplanes ; i++ ) {
			if ( DotProduct( trace.plane.normal, planes[i] ) > 0.99 ) {
				VectorAdd( trace.plane.normal, npm->ps->velocity, npm->ps->velocity );
				break;
			}
		}
		if ( i < numplanes ) {
			continue;
		}
		VectorCopy (trace.plane.normal, planes[numplanes]);
		numplanes++;

		//
		// modify velocity so it parallels all of the clip planes
		//

		// find a plane that it enters
		for ( i = 0 ; i < numplanes ; i++ ) {
			into = DotProduct( npm->ps->velocity, planes[i] );
			if ( into >= 0.1 ) {
				continue;		// move doesn't interact with the plane
			}

			// see how hard we are hitting things
			if ( -into > npml.impactSpeed ) {
				npml.impactSpeed = -into;
			}

			// slide along the plane
			NPM_ClipVelocity (npm->ps->velocity, planes[i], clipVelocity, OVERCLIP );

			// slide along the plane
			NPM_ClipVelocity (endVelocity, planes[i], endClipVelocity, OVERCLIP );

			// see if there is a second plane that the new move enters
			for ( j = 0 ; j < numplanes ; j++ ) {
				if ( j == i ) {
					continue;
				}
				if ( DotProduct( clipVelocity, planes[j] ) >= 0.1 ) {
					continue;		// move doesn't interact with the plane
				}

				// try clipping the move to the plane
				NPM_ClipVelocity( clipVelocity, planes[j], clipVelocity, OVERCLIP );
				NPM_ClipVelocity( endClipVelocity, planes[j], endClipVelocity, OVERCLIP );

				// see if it goes back into the first clip plane
				if ( DotProduct( clipVelocity, planes[i] ) >= 0 ) {
					continue;
				}

				// slide the original velocity along the crease
				CrossProduct (planes[i], planes[j], dir);
				VectorNormalize( dir );
				d = DotProduct( dir, npm->ps->velocity );
				VectorScale( dir, d, clipVelocity );

				CrossProduct (planes[i], planes[j], dir);
				VectorNormalize( dir );
				d = DotProduct( dir, endVelocity );
				VectorScale( dir, d, endClipVelocity );

				// see if there is a third plane the the new move enters
				for ( k = 0 ; k < numplanes ; k++ ) {
					if ( k == i || k == j ) {
						continue;
					}
					if ( DotProduct( clipVelocity, planes[k] ) >= 0.1 ) {
						continue;		// move doesn't interact with the plane
					}

					// stop dead at a tripple plane interaction
					VectorClear( npm->ps->velocity );
					return qtrue;
				}
			}

			// if we have fixed all interactions, try another move
			VectorCopy( clipVelocity, npm->ps->velocity );
			VectorCopy( endClipVelocity, endVelocity );
			break;
		}
	}

	if ( gravity ) {
		VectorCopy( endVelocity, npm->ps->velocity );
	}

	// don't change velocity if in a timer (FIXME: is this correct?)
	if ( npm->ps->pm_time ) {
		VectorCopy( primal_velocity, npm->ps->velocity );
	}

	return ( bumpcount != 0 );
}

/*
==================
NPM_StepSlideMove

==================
*/
void NPM_StepSlideMove( qboolean gravity ) {
	vec3_t		start_o, start_v;
	vec3_t		down_o, down_v;
	trace_t		trace;
//	float		down_dist, up_dist;
//	vec3_t		delta, delta2;
	vec3_t		up, down;

	VectorCopy (npm->ps->origin, start_o);
	VectorCopy (npm->ps->velocity, start_v);

	if ( NPM_SlideMove( gravity ) == 0 ) {
		return;		// we got exactly where we wanted to go first try
	}

	VectorCopy(start_o, down);
	down[2] -= STEPSIZE;
	npm->trace (&trace, start_o, npm->mins, npm->maxs, down, npm->ps->clientNum, npm->tracemask);
	VectorSet(up, 0, 0, 1);
	// never step up when you still have up velocity
	if ( npm->ps->velocity[2] > 0 && (trace.fraction == 1.0 ||
										DotProduct(trace.plane.normal, up) < 0.7)) {
		return;
	}

	VectorCopy (npm->ps->origin, down_o);
	VectorCopy (npm->ps->velocity, down_v);

	VectorCopy (start_o, up);
	up[2] += STEPSIZE;

	// test the player position if they were a stepheight higher
	npm->trace (&trace, up, npm->mins, npm->maxs, up, npm->ps->clientNum, npm->tracemask);
	if ( trace.allsolid ) {
		if ( npm->debugLevel ) {
			Com_Printf("%i:bend can't step\n", npc_pmove);
		}
		return;		// can't step up
	}

	// try slidemove from this position
	VectorCopy (up, npm->ps->origin);
	VectorCopy (start_v, npm->ps->velocity);

	NPM_SlideMove( gravity );

	// push down the final amount
	VectorCopy (npm->ps->origin, down);
	down[2] -= STEPSIZE;
	npm->trace (&trace, npm->ps->origin, npm->mins, npm->maxs, down, npm->ps->clientNum, npm->tracemask);
	if ( !trace.allsolid ) {
		VectorCopy (trace.endpos, npm->ps->origin);
	}
	if ( trace.fraction < 1.0 ) {
		NPM_ClipVelocity( npm->ps->velocity, trace.plane.normal, npm->ps->velocity, OVERCLIP );
	}

	{
		// use the step move
		float	delta;

		delta = npm->ps->origin[2] - start_o[2];
		if ( delta > 2 ) {
			if ( delta < 7 ) {
				NPM_AddEvent( EV_STEP_4 );
			} else if ( delta < 11 ) {
				NPM_AddEvent( EV_STEP_8 );
			} else if ( delta < 15 ) {
				NPM_AddEvent( EV_STEP_12 );
			} else {
				NPM_AddEvent( EV_STEP_16 );
			}
		}
		if ( npm->debugLevel ) {
			Com_Printf("%i:stepped\n", npc_pmove);
		}
	}
}

/*
=============
NPM_CheckJump
=============
*/
static qboolean NPM_CheckJump( void ) {
	if ( npm->ps->pm_flags & PMF_RESPAWNED ) {
		return qfalse;		// don't allow jump until all buttons are up
	}

	if ( npm->cmd.upmove < 10 ) {
		// not holding jump
		return qfalse;
	}

/*
	// must wait for jump to be released
	if ( npm->ps->pm_flags & PMF_JUMP_HELD ) {
		// clear upmove so cmdscale doesn't lower running speed
		npm->cmd.upmove = 0;
		return qfalse;
	}
*/
	npml.groundPlane = qfalse;		// jumping away
	npml.walking = qfalse;
	npm->ps->pm_flags |= PMF_JUMP_HELD;

	npm->ps->groundEntityNum = ENTITYNUM_NONE;
	npm->ps->velocity[2] = npm->cmd.upmove*8;	//		JUMP_VELOCITY;

//	NPM_AddEvent( EV_JUMP );

	if (npm->ns->toFire!=2)	// metlar long jump?
	{
		if ( npm->cmd.forwardmove >= 0 ) {
			NPM_ForceLegsAnim( ANPC_JUMP );
			npm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
		} else {
			NPM_ForceLegsAnim( ANPC_JUMP );
			npm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
		}
	}

	return qtrue;
}

/*
=============
NPM_CheckWaterJump
=============
*/
static qboolean	NPM_CheckWaterJump( void ) {
	vec3_t	spot;
	int		cont;
	vec3_t	flatforward;

	if (npm->ps->pm_time) {
		return qfalse;
	}

	// check for water jump
	if ( npm->waterlevel != 2 ) {
		return qfalse;
	}

	flatforward[0] = npml.forward[0];
	flatforward[1] = npml.forward[1];
	flatforward[2] = 0;
	VectorNormalize (flatforward);

	VectorMA (npm->ps->origin, 30, flatforward, spot);
	spot[2] += 4;
	cont = npm->pointcontents (spot, npm->ps->clientNum );
	if ( !(cont & CONTENTS_SOLID) ) {
		return qfalse;
	}

	spot[2] += 16;
	cont = npm->pointcontents (spot, npm->ps->clientNum );
	if ( cont ) {
		return qfalse;
	}

	// jump out of water
	VectorScale (npml.forward, 200, npm->ps->velocity);
	npm->ps->velocity[2] = 350;

	npm->ps->pm_flags |= PMF_TIME_WATERJUMP;
	npm->ps->pm_time = 2000;

	return qtrue;
}

//============================================================================


/*
===================
NPM_WaterJumpMove

Flying out of the water
===================
*/
static void NPM_WaterJumpMove( void ) {
	// waterjump has no control, but falls

	NPM_StepSlideMove( qtrue );

	npm->ps->velocity[2] -= npm->ps->gravity * npml.frametime;
	if (npm->ps->velocity[2] < 0) {
		// cancel as soon as we are falling down again
		npm->ps->pm_flags &= ~PMF_ALL_TIMES;
		npm->ps->pm_time = 0;
	}
}

/*
===================
NPM_WaterMove

===================
*/
static void NPM_WaterMove( void ) {
	int		i;
	vec3_t	wishvel;
	float	wishspeed;
	vec3_t	wishdir;
	float	scale;
	float	vel;

	if ( NPM_CheckWaterJump() ) {
		NPM_WaterJumpMove();
		return;
	}
	NPM_Friction ();

	scale = NPM_CmdScale( &npm->cmd );
	//
	// user intentions
	//
	if ( !scale ) {
		wishvel[0] = 0;
		wishvel[1] = 0;
		wishvel[2] = -60;		// sink towards bottom
	} else {
		for (i=0 ; i<3 ; i++)
			wishvel[i] = scale * npml.forward[i]*npm->cmd.forwardmove + scale * npml.right[i]*npm->cmd.rightmove;

		wishvel[2] += scale * npm->cmd.upmove;
	}

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	if ( wishspeed > npm->ps->speed * npm_swimScale ) {
		wishspeed = npm->ps->speed * npm_swimScale;
	}

	NPM_Accelerate (wishdir, wishspeed, npm_wateraccelerate);

	// make sure we can go up slopes easily under water
	if ( npml.groundPlane && DotProduct( npm->ps->velocity, npml.groundTrace.plane.normal ) < 0 ) {
		vel = VectorLength(npm->ps->velocity);
		// slide along the ground plane
		NPM_ClipVelocity (npm->ps->velocity, npml.groundTrace.plane.normal,
			npm->ps->velocity, OVERCLIP );

		VectorNormalize(npm->ps->velocity);
		VectorScale(npm->ps->velocity, vel, npm->ps->velocity);
	}

	NPM_SlideMove( qfalse );
}

/*
===================
NPM_FlyMove

Only with the flight powerup
===================
*/
static void NPM_FlyMove( void ) {
	int		i;
	vec3_t	wishvel;
	float	wishspeed;
	vec3_t	wishdir;
	float	scale;

	// normal slowdown
	NPM_Friction ();

	scale = NPM_CmdScale( &npm->cmd );
	//
	// user intentions
	//
	if ( !scale ) {
		wishvel[0] = 0;
		wishvel[1] = 0;
		wishvel[2] = 0;
	} else {
		for (i=0 ; i<3 ; i++) {
			wishvel[i] = scale * npml.forward[i]*npm->cmd.forwardmove + scale * npml.right[i]*npm->cmd.rightmove;
		}

		wishvel[2] += scale * npm->cmd.upmove;
	}

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	NPM_Accelerate (wishdir, wishspeed, npm_flyaccelerate);

	NPM_StepSlideMove( qfalse );

	NPM_ContinueLegsAnim( ANPC_WALK);	// flying bat animation
}



/*
===================
NPM_AirMove

===================
*/
static void NPM_AirMove( void ) {
	int			i;
	vec3_t		wishvel;
	float		fmove, smove;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;
	usercmd_t	cmd;

	NPM_Friction();

	fmove = npm->cmd.forwardmove;
	smove = npm->cmd.rightmove;

	cmd = npm->cmd;
	scale = NPM_CmdScale( &cmd );

	// set the movementDir so clients can rotate the legs for strafing
	NPM_SetMovementDir();

	// project moves down to flat plane
	npml.forward[2] = 0;
	npml.right[2] = 0;
	VectorNormalize (npml.forward);
	VectorNormalize (npml.right);

	for ( i = 0 ; i < 2 ; i++ ) {
		wishvel[i] = npml.forward[i]*fmove + npml.right[i]*smove;
	}
	wishvel[2] = 0;

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

	// not on ground, so little effect on velocity
	if (cmd.upmove>0)
		NPM_Accelerate (wishdir, wishspeed, npm_accelerate);	// just jumped, do normal acceleration
	else
		NPM_Accelerate (wishdir, wishspeed, npm_airaccelerate);

	// we may have a ground plane that is very steep, even
	// though we don't have a groundentity
	// slide along the steep plane
	if ( npml.groundPlane ) {
		NPM_ClipVelocity (npm->ps->velocity, npml.groundTrace.plane.normal,
			npm->ps->velocity, OVERCLIP );
	}

	if (npm->ns->toFire==2 && npm->ps->velocity[2]<0)
	{
		npm->ps->velocity[2]=0;
//		npm->ps->velocity[1]*=1.1;
//		npm->ps->velocity[0]*=1.1;
	}

	NPM_StepSlideMove ( qtrue );
}

/*
===================
NPM_GrappleMove

===================

static void NPM_GrappleMove( void ) {
	vec3_t vel, v;
	float vlen;

	VectorScale(npml.forward, -16, v);
	VectorAdd(npm->ps->grapplePoint, v, v);
	VectorSubtract(v, npm->ps->origin, vel);
	vlen = VectorLength(vel);
	VectorNormalize( vel );

	if (vlen <= 100)
		VectorScale(vel, 10 * vlen, vel);
	else
		VectorScale(vel, 800, vel);

	VectorCopy(vel, npm->ps->velocity);

	npml.groundPlane = qfalse;
}
*/
/*
===================
NPM_WalkMove

===================
*/
static void NPM_WalkMove( void ) {
	int			i;
	vec3_t		wishvel;
	float		fmove, smove;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;
	usercmd_t	cmd;
	float		accelerate;
	float		vel;

	if ( npm->waterlevel > 2 && DotProduct( npml.forward, npml.groundTrace.plane.normal ) > 0 ) {
		// begin swimming
		NPM_WaterMove();
		return;
	}


	if ( NPM_CheckJump () ) {
		// jumped away
		if ( npm->waterlevel > 1 ) {
			NPM_WaterMove();
		} else {
			NPM_AirMove();
		}
		return;
	}

	NPM_Friction ();

	fmove = npm->cmd.forwardmove;
	smove = npm->cmd.rightmove;

	cmd = npm->cmd;
	scale = NPM_CmdScale( &cmd );

	// set the movementDir so clients can rotate the legs for strafing
	NPM_SetMovementDir();

	// project moves down to flat plane
	npml.forward[2] = 0;
	npml.right[2] = 0;

	// project the forward and right directions onto the ground plane
	NPM_ClipVelocity (npml.forward, npml.groundTrace.plane.normal, npml.forward, OVERCLIP );
	NPM_ClipVelocity (npml.right, npml.groundTrace.plane.normal, npml.right, OVERCLIP );
	//
	VectorNormalize (npml.forward);
	VectorNormalize (npml.right);

	for ( i = 0 ; i < 3 ; i++ ) {
		wishvel[i] = npml.forward[i]*fmove + npml.right[i]*smove;
	}
	// when going up or down slopes the wish velocity should Not be zero
//	wishvel[2] = 0;

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

	// clamp the speed lower if ducking
	if ( npm->ps->pm_flags & PMF_DUCKED ) {
		if ( wishspeed > npm->ps->speed * npm_duckScale ) {
			wishspeed = npm->ps->speed * npm_duckScale;
		}
	}

	// clamp the speed lower if wading or walking on the bottom
	if ( npm->waterlevel ) {
		float	waterScale;

		waterScale = npm->waterlevel / 3.0;
		waterScale = 1.0 - ( 1.0 - npm_swimScale ) * waterScale;
		if ( wishspeed > npm->ps->speed * waterScale ) {
			wishspeed = npm->ps->speed * waterScale;
		}
	}

	// when a player gets hit, they temporarily lose
	// full control, which allows them to be moved a bit
	if ( ( npml.groundTrace.surfaceFlags & SURF_SLICK ) || npm->ps->pm_flags & PMF_TIME_KNOCKBACK ) {
		accelerate = npm_airaccelerate;
	} else {
		accelerate = npm_accelerate;
	}

	NPM_Accelerate (wishdir, wishspeed, accelerate);

	//Com_Printf("velocity = %1.1f %1.1f %1.1f\n", pm->ps->velocity[0], pm->ps->velocity[1], pm->ps->velocity[2]);
	//Com_Printf("velocity1 = %1.1f\n", VectorLength(pm->ps->velocity));

	if ( ( npml.groundTrace.surfaceFlags & SURF_SLICK ) || npm->ps->pm_flags & PMF_TIME_KNOCKBACK ) {
		npm->ps->velocity[2] -= npm->ps->gravity * npml.frametime;
	} else {
		// don't reset the z velocity for slopes
//		pm->ps->velocity[2] = 0;
	}

	vel = VectorLength(npm->ps->velocity);

	// slide along the ground plane
	NPM_ClipVelocity (npm->ps->velocity, npml.groundTrace.plane.normal,
		npm->ps->velocity, OVERCLIP );

	// don't decrease velocity when going up or down a slope
	VectorNormalize(npm->ps->velocity);
	VectorScale(npm->ps->velocity, vel, npm->ps->velocity);


	// don't do anything if standing still
	if (!npm->ps->velocity[0] && !npm->ps->velocity[1]) {
		return;
	}

	NPM_StepSlideMove( qfalse );

	//Com_Printf("velocity2 = %1.1f\n", VectorLength(pm->ps->velocity));

}


/*
==============
NPM_DeadMove
==============
*/
static void NPM_DeadMove( void ) {
	float	forward;

	if ( !npml.walking ) {
		return;
	}

	// extra friction

	forward = VectorLength (npm->ps->velocity);
	forward -= 20;
	if ( forward <= 0 ) {
		VectorClear (npm->ps->velocity);
	} else {
		VectorNormalize (npm->ps->velocity);
		VectorScale (npm->ps->velocity, forward, npm->ps->velocity);
	}
}


/*
===============
NPM_NoclipMove
===============

static void NPM_NoclipMove( void ) {
	float	speed, drop, friction, control, newspeed;
	int			i;
	vec3_t		wishvel;
	float		fmove, smove;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;

	npm->ps->viewheight = DEFAULT_VIEWHEIGHT;

	// friction

	speed = VectorLength (npm->ps->velocity);
	if (speed < 1)
	{
		VectorCopy (vec3_origin, npm->ps->velocity);
	}
	else
	{
		drop = 0;

		friction = npm_friction*1.5;	// extra friction
		control = speed < npm_stopspeed ? npm_stopspeed : speed;
		drop += control*friction*npml.frametime;

		// scale the velocity
		newspeed = speed - drop;
		if (newspeed < 0)
			newspeed = 0;
		newspeed /= speed;

		VectorScale (npm->ps->velocity, newspeed, npm->ps->velocity);
	}

	// accelerate
	scale = NPM_CmdScale( &npm->cmd );

	fmove = npm->cmd.forwardmove;
	smove = npm->cmd.rightmove;

	for (i=0 ; i<3 ; i++)
		wishvel[i] = npml.forward[i]*fmove + npml.right[i]*smove;
	wishvel[2] += npm->cmd.upmove;

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

	NPM_Accelerate( wishdir, wishspeed, npm_accelerate );

	// move
	VectorMA (npm->ps->origin, npml.frametime, npm->ps->velocity, npm->ps->origin);
}
*/
//============================================================================

/*
================
NPM_FootstepForSurface

Returns an event number apropriate for the groundsurface
================
*/
static int NPM_FootstepForSurface( void ) {
	if ( npml.groundTrace.surfaceFlags & SURF_NOSTEPS ) {
		return 0;
	}
	if ( npml.groundTrace.surfaceFlags & SURF_METALSTEPS ) {
		return EV_FOOTSTEP_METAL;
	}
	return EV_FOOTSTEP;
}


/*
=================
NPM_CrashLand

Check for hard landings that generate sound events
=================
*/
static void NPM_CrashLand( void ) {
	float		delta;
	float		dist;
	float		vel, acc;
	float		t;
	float		a, b, c, den;

	// decide which landing animation to use
/*	if ( npm->ps->pm_flags & PMF_BACKWARDS_JUMP ) {
		NPM_ForceLegsAnim( ANPC_LAND );
	} else {
*/		NPM_ForceLegsAnim( ANPC_LAND );
//	}

	npm->ps->legsTimer = npm->npc->animTimes[ANPC_LAND];
	npm->ns->dontMoveTime = level.time+npm->npc->animTimes[ANPC_LAND];
	npm->ns->toFire=0;

	// calculate the exact velocity on landing
	dist = npm->ps->origin[2] - npml.previous_origin[2];
	vel = npml.previous_velocity[2];
	acc = -npm->ps->gravity;

	a = acc / 2;
	b = vel;
	c = -dist;

	den =  b * b - 4 * a * c;
	if ( den < 0 ) {
		return;
	}
	t = (-b - sqrt( den ) ) / ( 2 * a );

	delta = vel + t * acc;
	delta = delta*delta * 0.0001;

	// ducking while falling doubles damage
	if ( npm->ps->pm_flags & PMF_DUCKED ) {
		delta *= 2;
	}

	// never take falling damage if completely underwater
	if ( npm->waterlevel == 3 ) {
		return;
	}

	// reduce falling damage if there is standing water
	if ( npm->waterlevel == 2 ) {
		delta *= 0.25;
	}
	if ( npm->waterlevel == 1 ) {
		delta *= 0.5;
	}

	if ( delta < 1 ) {
		return;
	}

	// create a local entity event to play the sound

	// SURF_NODAMAGE is used for bounce pads where you don't ever
	// want to take damage or play a crunch sound
	if ( !(npml.groundTrace.surfaceFlags & SURF_NODAMAGE) )  {
		if ( delta > 60 ) {
			NPM_AddEvent( EV_FALL_FAR );
		} else if ( delta > 40 ) {
			// this is a pain grunt, so don't play it if dead
			if ( npm->ps->stats[STAT_HEALTH] > 0 ) {
				NPM_AddEvent( EV_FALL_MEDIUM );
			}
		} else if ( delta > 7 ) {
			NPM_AddEvent( EV_FALL_SHORT );
		} else {
			NPM_AddEvent( NPM_FootstepForSurface() );
		}
	}

	// start footstep cycle over
	npm->ps->bobCycle = 0;
}

/*
=============
NPM_CheckStuck
=============
*/
/*
void PM_CheckStuck(void) {
	trace_t trace;

	pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, pm->ps->origin, pm->ps->clientNum, pm->tracemask);
	if (trace.allsolid) {
		//int shit = qtrue;
	}
}
*/

/*
=============
NPM_CorrectAllSolid
=============
*/
static int NPM_CorrectAllSolid( trace_t *trace ) {
	int			i, j, k;
	vec3_t		point;

	if ( npm->debugLevel ) {
		Com_Printf("%i:allsolid\n", npc_pmove);
	}

	// jitter around
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			for (k = -1; k <= 1; k++) {
				VectorCopy(npm->ps->origin, point);
				point[0] += (float) i;
				point[1] += (float) j;
				point[2] += (float) k;
				npm->trace (trace, point, npm->mins, npm->maxs, point, npm->ps->clientNum, npm->tracemask);
				if ( !trace->allsolid ) {
					point[0] = npm->ps->origin[0];
					point[1] = npm->ps->origin[1];
					point[2] = npm->ps->origin[2] - 0.25;

					npm->trace (trace, npm->ps->origin, npm->mins, npm->maxs, point, npm->ps->clientNum, npm->tracemask);
					npml.groundTrace = *trace;
					return qtrue;
				}
			}
		}
	}

	npm->ps->groundEntityNum = ENTITYNUM_NONE;
	npml.groundPlane = qfalse;
	npml.walking = qfalse;

	return qfalse;
}


/*
=============
NPM_GroundTraceMissed

The ground trace didn't hit a surface, so we are in freefall
=============
*/
static void NPM_GroundTraceMissed( void ) {
	trace_t		trace;
	vec3_t		point;

	if ( npm->ps->groundEntityNum != ENTITYNUM_NONE ) {
		// we just transitioned into freefall
		if ( npm->debugLevel ) {
			Com_Printf("%i:lift\n", npc_pmove);
		}

		// if they aren't in a jumping animation and the ground is a ways away, force into it
		// if we didn't do the trace, the player would be backflipping down staircases
		VectorCopy( npm->ps->origin, point );
		point[2] -= 64;
		npm->trace (&trace, npm->ps->origin, npm->mins, npm->maxs, point, npm->ps->clientNum, npm->tracemask);
		if ( trace.fraction == 1.0 ) {
			if (npm->ns->toFire!=2)		// metlar long jump?
			{
				if ( npm->cmd.forwardmove >= 0 ) {
					NPM_ForceLegsAnim( ANPC_JUMP );
					npm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
				} else {
					NPM_ForceLegsAnim( ANPC_JUMP );
					npm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
				}
			}
		}
	}

	npm->ps->groundEntityNum = ENTITYNUM_NONE;
	npml.groundPlane = qfalse;
	npml.walking = qfalse;
}


/*
=============
NPM_GroundTrace
=============
*/
static void NPM_GroundTrace( void ) {
	vec3_t		point;
	trace_t		trace;

	point[0] = npm->ps->origin[0];
	point[1] = npm->ps->origin[1];
	point[2] = npm->ps->origin[2] - 0.25;

	npm->trace (&trace, npm->ps->origin, npm->mins, npm->maxs, point, npm->ps->clientNum, npm->tracemask);
	npml.groundTrace = trace;

	// do something corrective if the trace starts in a solid...
	if ( trace.allsolid ) {
		if ( !NPM_CorrectAllSolid(&trace) )
			return;
	}

	// if the trace didn't hit anything, we are in free fall
	if ( trace.fraction == 1.0 ) {
		NPM_GroundTraceMissed();
		npml.groundPlane = qfalse;
		npml.walking = qfalse;
		return;
	}

	// do not allow to stand over another NPC or a player
	if (npml.can_do_back_move &&			// only once
		// Turtle Man: I put bracets around ET_NPC || ET_PLAYER, I think its right...
		(g_entities[trace.entityNum].s.eType == ET_NPC || g_entities[trace.entityNum].s.eType == ET_PLAYER)
		&& npm->ns->toFire != 2)
	{
		VectorScale(npm->ps->velocity,-1,npm->ps->velocity);
		npm->cmd.forwardmove=0;
		npm->cmd.rightmove=0;
		npm->cmd.upmove=15;
		npml.can_do_back_move=0;
		NPM_AirMove();
	}

	// check if getting thrown off the ground
	if ( npm->ps->velocity[2] > 0 && DotProduct( npm->ps->velocity, trace.plane.normal ) > 10 ) {
		if ( npm->debugLevel ) {
			Com_Printf("%i:kickoff\n", npc_pmove);
		}

		// go into jump animation
		if (npm->ns->toFire!=2)		// metlar jump attack?
		{
			if ( npm->cmd.forwardmove >= 0 ) {
				NPM_ForceLegsAnim( ANPC_JUMP );
				npm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
			} else {
				NPM_ForceLegsAnim( ANPC_JUMP );
				npm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
			}
		}

		npm->ps->groundEntityNum = ENTITYNUM_NONE;
		npml.groundPlane = qfalse;
		npml.walking = qfalse;
		return;
	}

	// slopes that are too steep will not be considered onground
	if ( trace.plane.normal[2] < MIN_WALK_NORMAL ) {
		if ( npm->debugLevel ) {
			Com_Printf("%i:steep\n", npc_pmove);
		}
		// FIXME: if they can't slide down the slope, let them
		// walk (sharp crevices)
		npm->ps->groundEntityNum = ENTITYNUM_NONE;
		npml.groundPlane = qtrue;
		npml.walking = qfalse;
		return;
	}

	npml.groundPlane = qtrue;
	npml.walking = qtrue;

	// hitting solid ground will end a waterjump
	if (npm->ps->pm_flags & PMF_TIME_WATERJUMP)
	{
		npm->ps->pm_flags &= ~(PMF_TIME_WATERJUMP | PMF_TIME_LAND);
		npm->ps->pm_time = 0;
	}

	if ( npm->ps->groundEntityNum == ENTITYNUM_NONE ) {
		// just hit the ground
		if ( npm->debugLevel ) {
			Com_Printf("%i:Land\n", npc_pmove);
		}

/*		if (npm->npc->npcType==NPC_METLAR)
		{
			G_Printf("%d metlar land %d\n",level.time,npm->ns->toFire);

		}
*/
		if ( ( npm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == ANPC_JUMP	// only do crash animation if it was jumping
			|| (/*( npm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == ANPC_ATTACK_FAR
				&& */ /*npm->ns->toFire==2*/ npm->npc->npcType == NPC_METLAR)  )
		{
			NPM_CrashLand();
//			G_Printf("crash\n");
		}

		// don't do landing time if we were just going down a slope
		if ( npml.previous_velocity[2] < -200 ) {
			// don't allow another jump for a little while
			npm->ps->pm_flags |= PMF_TIME_LAND;
			npm->ps->pm_time = 250;
		}
	}

	npm->ps->groundEntityNum = trace.entityNum;

	// don't reset the z velocity for slopes
//	pm->ps->velocity[2] = 0;

	NPM_AddTouchEnt( trace.entityNum );
}


/*
=============
NPM_SetWaterLevel	FIXME: avoid this twice?  certainly if not moving
=============
*/
static void NPM_SetWaterLevel( void ) {
	vec3_t		point;
	int			cont;
	int			sample1;
	int			sample2;

	//
	// get waterlevel, accounting for ducking
	//
	npm->waterlevel = 0;
	npm->watertype = 0;

	point[0] = npm->ps->origin[0];
	point[1] = npm->ps->origin[1];
	point[2] = npm->ps->origin[2] + MINS_Z + 1;
	cont = npm->pointcontents( point, npm->ps->clientNum );

	if ( cont & MASK_WATER ) {
		sample2 = npm->ps->viewheight - MINS_Z;
		sample1 = sample2 / 2;

		npm->watertype = cont;
		npm->waterlevel = 1;
		point[2] = npm->ps->origin[2] + MINS_Z + sample1;
		cont = npm->pointcontents (point, npm->ps->clientNum );
		if ( cont & MASK_WATER ) {
			npm->waterlevel = 2;
			point[2] = npm->ps->origin[2] + MINS_Z + sample2;
			cont = npm->pointcontents (point, npm->ps->clientNum );
			if ( cont & MASK_WATER ){
				npm->waterlevel = 3;
			}
		}
	}

}

/*
==============
NPM_CheckDuck

Sets mins, maxs, and pm->ps->viewheight
==============

static void NPM_CheckDuck (void)
{
	trace_t	trace;

	if ( npm->ps->powerups[PW_INVULNERABILITY] ) {
		if ( npm->ps->pm_flags & PMF_INVULEXPAND ) {
			// invulnerability sphere has a 42 units radius
			VectorSet( npm->mins, -42, -42, -42 );
			VectorSet( npm->maxs, 42, 42, 42 );
		}
		else {
			VectorSet( npm->mins, -15, -15, MINS_Z );
			VectorSet( npm->maxs, 15, 15, 16 );
		}
		npm->ps->pm_flags |= PMF_DUCKED;
		npm->ps->viewheight = CROUCH_VIEWHEIGHT;
		return;
	}
	npm->ps->pm_flags &= ~PMF_INVULEXPAND;

	npm->mins[0] = -15;
	npm->mins[1] = -15;

	npm->maxs[0] = 15;
	npm->maxs[1] = 15;

	npm->mins[2] = MINS_Z;

	if (npm->ps->pm_type == PM_DEAD)
	{
		npm->maxs[2] = -8;
		npm->ps->viewheight = DEAD_VIEWHEIGHT;
		return;
	}

	if (npm->cmd.upmove < 0)
	{	// duck
		npm->ps->pm_flags |= PMF_DUCKED;
	}
	else
	{	// stand up if possible
		if (npm->ps->pm_flags & PMF_DUCKED)
		{
			// try to stand up
			npm->maxs[2] = 32;
			npm->trace (&trace, npm->ps->origin, npm->mins, npm->maxs, npm->ps->origin, npm->ps->clientNum, npm->tracemask );
			if (!trace.allsolid)
				npm->ps->pm_flags &= ~PMF_DUCKED;
		}
	}

	if (npm->ps->pm_flags & PMF_DUCKED)
	{
		npm->maxs[2] = 16;
		npm->ps->viewheight = CROUCH_VIEWHEIGHT;
	}
	else
	{
		npm->maxs[2] = 32;
		npm->ps->viewheight = DEFAULT_VIEWHEIGHT;
	}
}
*/


//===================================================================


/*
===============
PM_Footsteps
===============
*/
static void NPM_Footsteps( void ) {
	float		bobmove;
	int			old;
	qboolean	footstep;

	//
	// calculate speed and cycle to be used for
	// all cyclic walking effects
	//
	npm->xyspeed = sqrt( npm->ps->velocity[0] * npm->ps->velocity[0]
		+  npm->ps->velocity[1] * npm->ps->velocity[1] );

	if ( npm->ps->groundEntityNum == ENTITYNUM_NONE ) {

/*		if ( npm->ps->powerups[PW_INVULNERABILITY] ) {
			NPM_ContinueLegsAnim( LEGS_IDLECR );
		}
		// airborne leaves position in cycle intact, but doesn't advance
		if ( pm->waterlevel > 1 ) {
			NPM_ContinueLegsAnim( LEGS_SWIM );
		}
*/		return;
	}

	// if not trying to move
	if ( !npm->cmd.forwardmove && !npm->cmd.rightmove ) {
		if (  npm->xyspeed < 5 ) {
			npm->ps->bobCycle = 0;	// start at beginning of cycle again
			if ( npm->ps->pm_flags & PMF_DUCKED ) {
//				NPM_ContinueLegsAnim( LEGS_IDLECR );
			} else {
/*				if (npm->ns->toFire)
					NPM_ContinueLegsAnim( ANPC_WALK);

				if (fabs(npm->cmd.angles[1])>1)
				{
					if (npm->npc->flags & NPCF_WALKANDFLY)
						NPM_ContinueLegsAnim( ANPC_RUN);
					else
						NPM_ContinueLegsAnim( ANPC_WALK);
				}
				else
*/				{
					if (npm->ns->command==NPC_COM_ATTACK)
						NPM_ContinueLegsAnim( ANPC_STANDING_ACTIVE );
					else
						NPM_ContinueLegsAnim( ANPC_STANDING );
				}

			}
		}
		return;
	}


	footstep = qfalse;

	if ( npm->ps->pm_flags & PMF_DUCKED ) {
		bobmove = 0.5;	// ducked characters bob much faster
/*		if ( npm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
			PM_ContinueLegsAnim( LEGS_BACKCR );
		}
		else {
			PM_ContinueLegsAnim( LEGS_WALKCR );
		}
*/		// ducked characters never play footsteps
	} else {
#if 1
		if (!(npm->cmd.buttons & BUTTON_WALKING))
#else
		if ( (!( npm->cmd.buttons & BUTTON_WALKING ) && npm->npc->npcType!=NPC_SEALORD)
			|| (npm->npc->npcType==NPC_SEALORD && npm->cmd.rightmove<0))
#endif
		{
			bobmove = 0.4f;	// faster speeds bob faster
			if ( npm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
				NPM_ContinueLegsAnim( ANPC_BACKPEDAL );
			}
			else {
				NPM_ContinueLegsAnim( ANPC_RUN );
			}
			footstep = qtrue;
		} else {
			bobmove = 0.3f;	// walking bobs slow
			if ( npm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
				NPM_ContinueLegsAnim( ANPC_BACKPEDAL );
			}
			else {
				NPM_ContinueLegsAnim( ANPC_WALK );
			}
		}
	}

	// check for footstep / splash sounds
	old = npm->ps->bobCycle;
	npm->ps->bobCycle = (int)( old + bobmove * npml.msec ) & 255;

	// if we just crossed a cycle boundary, play an apropriate footstep event
	if ( ( ( old + 64 ) ^ ( npm->ps->bobCycle + 64 ) ) & 128 ) {
		if ( npm->waterlevel == 0 ) {
			// on ground will only play sounds if running
			if ( footstep && !npm->noFootsteps ) {
				NPM_AddEvent( NPM_FootstepForSurface() );
			}
		} else if ( npm->waterlevel == 1 ) {
			// splashing
			NPM_AddEvent( EV_FOOTSPLASH );
		} else if ( npm->waterlevel == 2 ) {
			// wading / swimming at surface
			NPM_AddEvent( EV_SWIM );
		} else if ( npm->waterlevel == 3 ) {
			// no sound when completely underwater

		}
	}
}

/*
==============
NPM_WaterEvents

Generate sound events for entering and leaving water
==============
*/

static void NPM_WaterEvents( void ) {		// FIXME?
	//
	// if just entered a water volume, play a sound
	//
	if (!npml.previous_waterlevel && npm->waterlevel) {
		NPM_AddEvent( EV_WATER_TOUCH );
	}

	//
	// if just completely exited a water volume, play a sound
	//
	if (npml.previous_waterlevel && !npm->waterlevel) {
		NPM_AddEvent( EV_WATER_LEAVE );
	}

	//
	// check for head just going under water
	//
	if (npml.previous_waterlevel != 3 && npm->waterlevel == 3) {
		NPM_AddEvent( EV_WATER_UNDER );
	}

	//
	// check for head just coming out of water
	//
	if (npml.previous_waterlevel == 3 && npm->waterlevel != 3) {
		NPM_AddEvent( EV_WATER_CLEAR );
	}
}


/*
===============
NPM_BeginWeaponChange
===============

static void NPM_BeginWeaponChange( int weapon ) {
	if ( weapon <= WP_NONE || weapon >= WP_NUM_WEAPONS ) {
		return;
	}

	if ( !( npm->ps->stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
		return;
	}

	if ( npm->ps->weaponstate == WEAPON_DROPPING ) {
		return;
	}

	NPM_AddEvent( EV_CHANGE_WEAPON );
	npm->ps->weaponstate = WEAPON_DROPPING;
	npm->ps->weaponTime += 200;
	NPM_StartTorsoAnim( TORSO_DROP );
}
*/

/*
===============
NPM_FinishWeaponChange
===============

static void PM_FinishWeaponChange( void ) {
	int		weapon;

	weapon = pm->cmd.weapon;
	if ( weapon < WP_NONE || weapon >= WP_NUM_WEAPONS ) {
		weapon = WP_NONE;
	}

	if ( !( pm->ps->stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
		weapon = WP_NONE;
	}

	pm->ps->weapon = weapon;
	pm->ps->weaponstate = WEAPON_RAISING;
	pm->ps->weaponTime += 250;
	PM_StartTorsoAnim( TORSO_RAISE );
}
*/

/*
==============
PM_TorsoAnimation

==============

static void PM_TorsoAnimation( void ) {
	if ( pm->ps->weaponstate == WEAPON_READY ) {
		if ( pm->ps->weapon == WP_GAUNTLET ) {
			PM_ContinueTorsoAnim( TORSO_STAND2 );
		} else {
			PM_ContinueTorsoAnim( TORSO_STAND );
		}
		return;
	}
}

*/
/*
==============
PM_Weapon

Generates weapon events and modifes the weapon counter
==============
*/
/*
static void NPM_Weapon( void ) {
	int		addTime;

	// don't allow attack until all buttons are up
	if ( npm->ps->pm_flags & PMF_RESPAWNED ) {
		return;
	}

	// check for dead player
	if ( npm->ps->stats[STAT_HEALTH] <= 0 ) {
//		npm->ps->weapon = WP_NONE;
		return;
	}

	// make weapon function
	if ( npm->ps->weaponTime > 0 ) {
		npm->ps->weaponTime -= npml.msec;
	}

	if ( npm->ps->weaponTime > 0 ) {
		return;
	}

	// check for fire
	if ( ! (npm->cmd.buttons & BUTTON_ATTACK) ) {
		npm->ps->weaponTime = 0;
		npm->ps->weaponstate = WEAPON_READY;
		return;
	}

	npm->ps->weaponstate = WEAPON_FIRING;

	// fire weapon
	NPM_AddEvent( EV_FIRE_WEAPON );

	addTime=0;

	PONER ADD_TIME SEGUN EL NPC QUE DISPARE


	npm->ps->weaponTime += addTime;
}
  */

/*
================
PM_Animate
================


static void PM_Animate( void ) {
	if ( pm->cmd.buttons & BUTTON_GESTURE ) {
		if ( pm->ps->torsoTimer == 0 ) {
			PM_StartTorsoAnim( TORSO_GESTURE );
			pm->ps->torsoTimer = TIMER_GESTURE;
			PM_AddEvent( EV_TAUNT );
		}
	}
}
*/

/*
================
NPM_DropTimers
================
*/
static void NPM_DropTimers( void ) {
	// drop misc timing counter
	if ( npm->ps->pm_time ) {
		if ( npml.msec >= npm->ps->pm_time ) {
			npm->ps->pm_flags &= ~PMF_ALL_TIMES;
			npm->ps->pm_time = 0;
		} else {
			npm->ps->pm_time -= npml.msec;
		}
	}

	// drop animation counter
	if ( npm->ps->legsTimer > 0 ) {
		npm->ps->legsTimer -= npml.msec;
		if ( npm->ps->legsTimer < 0 ) {
			npm->ps->legsTimer = 0;
		}
	}

	if ( npm->ps->torsoTimer > 0 ) {
		npm->ps->torsoTimer -= npml.msec;
		if ( npm->ps->torsoTimer < 0 ) {
			npm->ps->torsoTimer = 0;
		}
	}
}


/*
================
NPM_UpdateViewAngles

This can be used as another entry point when only the viewangles
are being updated isntead of a full move
================


void NPM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd ) {
	short		temp;
	int		i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPINTERMISSION) {
		return;		// no view changes at all
	}

	if ( ps->pm_type != PM_SPECTATOR && ps->stats[STAT_HEALTH] <= 0 ) {
		return;		// no view changes at all
	}

	// circularly clamp the angles with deltas
	for (i=0 ; i<3 ; i++) {
		temp = cmd->angles[i] + ps->delta_angles[i];
		if ( i == PITCH ) {
			// don't let the player look up or down more than 90 degrees
			if ( temp > 16000 ) {
				ps->delta_angles[i] = 16000 - cmd->angles[i];
				temp = 16000;
			} else if ( temp < -16000 ) {
				ps->delta_angles[i] = -16000 - cmd->angles[i];
				temp = -16000;
			}
		}
		ps->viewangles[i] = SHORT2ANGLE(temp);
	}

}
*/

/*
================
NPmoveSingle

================
*/
void trap_SnapVector( float *v );

void NPmoveSingle (npmove_t *pmove) {
	npm = pmove;

	// this counter lets us debug movement problems with a journal
	// by setting a conditional breakpoint fot the previous frame
	npc_pmove++;

	// clear results
	npml.can_do_back_move=1;
	npm->numtouch = 0;
	npm->watertype = 0;
	npm->waterlevel = 0;
	npm->ps->viewheight = npm->npc->eye[2];

	if (npm->ns->toFire==2) npm->cmd.forwardmove=127;

/*	if ( npm->ps->stats[STAT_HEALTH] <= 0 ) {
		npm->tracemask &= ~CONTENTS_BODY;	// corpses can fly through bodies
	}

	// make sure walking button is clear if they are running, to avoid
	// proxy no-footsteps cheats
	if ( abs( npm->cmd.forwardmove ) > 64 || abs( npm->cmd.rightmove ) > 64 ) {
		npm->cmd.buttons &= ~BUTTON_WALKING;
	}

	// set the talk balloon flag
	if ( npm->cmd.buttons & BUTTON_TALK ) {
		npm->ps->eFlags |= EF_TALK;
	} else {
		npm->ps->eFlags &= ~EF_TALK;
	}


	// set the firing flag for continuous beam weapons
	if ( !(npm->ps->pm_flags & PMF_RESPAWNED) && npm->ps->pm_type != PM_INTERMISSION
		&& ( npm->cmd.buttons & BUTTON_ATTACK ) && npm->ps->ammo[ npm->ps->weapon ] ) {
		npm->ps->eFlags |= EF_FIRING;
	} else {
		npm->ps->eFlags &= ~EF_FIRING;
	}

	// clear the respawned flag if attack and use are cleared
	if ( npm->ps->stats[STAT_HEALTH] > 0 &&
		!( npm->cmd.buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE) ) ) {
		npm->ps->pm_flags &= ~PMF_RESPAWNED;
	}

	// if talk button is down, dissallow all other input
	// this is to prevent any possible intercept proxy from
	// adding fake talk balloons
	if ( pmove->cmd.buttons & BUTTON_TALK ) {
		// keep the talk button set tho for when the cmd.serverTime > 66 msec
		// and the same cmd is used multiple times in Pmove
		pmove->cmd.buttons = BUTTON_TALK;
		pmove->cmd.forwardmove = 0;
		pmove->cmd.rightmove = 0;
		pmove->cmd.upmove = 0;
	}
*/
	// clear all pmove local vars
	memset (&npml, 0, sizeof(npml));

	// determine the time
	npml.msec = pmove->cmd.serverTime - npm->ps->commandTime;
	if ( npml.msec < 1 ) {
		npml.msec = 1;
	} else if ( npml.msec > 200 ) {
		npml.msec = 200;
	}
	npm->ps->commandTime = pmove->cmd.serverTime;

	// save old org in case we get stuck
	VectorCopy (npm->ps->origin, npml.previous_origin);

	// save old velocity for crashlanding
	VectorCopy (npm->ps->velocity, npml.previous_velocity);

	npml.frametime = npml.msec * 0.001;

	// update the viewangles
//	NPM_UpdateViewAngles( npm->ps, &npm->cmd );

	AngleVectors (npm->ps->viewangles, npml.forward, npml.right, npml.up);

	if ( npm->cmd.upmove < 10 ) {
		// not holding jump
		npm->ps->pm_flags &= ~PMF_JUMP_HELD;
	}

	// decide if backpedaling animations should be used
	if ( npm->cmd.forwardmove < 0 ) {
		npm->ps->pm_flags |= PMF_BACKWARDS_RUN;
	} else if ( npm->cmd.forwardmove > 0 || ( npm->cmd.forwardmove == 0 && npm->cmd.rightmove ) ) {
		npm->ps->pm_flags &= ~PMF_BACKWARDS_RUN;
	}

	if ( npm->ps->pm_type >= PM_DEAD ) {
		npm->cmd.forwardmove = 0;
		npm->cmd.rightmove = 0;
		npm->cmd.upmove = 0;
	}

/*	if ( npm->ps->pm_type == PM_SPECTATOR ) {
		PM_CheckDuck ();
		PM_FlyMove ();
		PM_DropTimers ();
		return;
	}

	if ( pm->ps->pm_type == PM_NOCLIP ) {
		PM_NoclipMove ();
		PM_DropTimers ();
		return;
	}
*/
	if (npm->ps->pm_type == PM_FREEZE) {
		return;		// no movement at all
	}

/*	if ( npm->ps->pm_type == PM_INTERMISSION || npm->ps->pm_type == PM_SPINTERMISSION) {
		return;		// no movement at all
	}
*/
	// set watertype, and waterlevel
	NPM_SetWaterLevel();
	npml.previous_waterlevel = pmove->waterlevel;

	// set mins, maxs, and viewheight
//	NPM_CheckDuck ();

	// set groundentity
	NPM_GroundTrace();

	if ( npm->ps->pm_type == PM_DEAD ) {
//		G_Prinuf("dead");
		NPM_DeadMove ();
	}

	NPM_DropTimers();

	if ( npm->npc->flags & NPCF_WALKANDFLY && npm->ns->inFlight) {
		// flight powerup doesn't allow jump and has different friction
//		G_Printf("fly ");
		NPM_FlyMove();
/*	} else if (npm->ps->pm_flags & PMF_GRAPPLE_PULL) {
		NPM_GrappleMove();
		// We can wiggle a bit
		NPM_AirMove();
*/	} else if (npm->ps->pm_flags & PMF_TIME_WATERJUMP) {
//		G_Printf("water_jump ");
		NPM_WaterJumpMove();
	} else if ( npm->waterlevel > 1 ) {
		// swimming
//		G_Printf("water ");
		NPM_WaterMove();
	} else if ( npml.walking ) {
		// walking on ground
//		G_Printf("walk ");
		NPM_WalkMove();
	} else {
		// airborne
//		G_Printf("air ");
		NPM_AirMove();
	}

//	NPM_Animate();

	// set groundentity, watertype, and waterlevel
	NPM_GroundTrace();
	NPM_SetWaterLevel();

	// weapons
//	NPM_Weapon();

	// torso animation
//	PM_TorsoAnimation();

	// footstep events / legs animations
	NPM_Footsteps();

	// entering / leaving water splashes
	NPM_WaterEvents();

	// snap some parts of playerstate to save network bandwidth
	trap_SnapVector( npm->ps->velocity );
}


/*
================
NPmove

================
*/
void NPmove (npmove_t *pmove) {
	int			finalTime;

	finalTime = pmove->cmd.serverTime;

	if ( finalTime < pmove->ps->commandTime ) {
		return;	// should not happen
	}

	if ( finalTime > pmove->ps->commandTime + 1000 ) {
		pmove->ps->commandTime = finalTime - 1000;
	}

	pmove->ps->pmove_framecount = (pmove->ps->pmove_framecount+1) & ((1<<PS_PMOVEFRAMECOUNTBITS)-1);

	// chop the move up if it is too long, to prevent framerate
	// dependent behavior
	while ( pmove->ps->commandTime != finalTime ) {
		int		msec;

		msec = finalTime - pmove->ps->commandTime;

		if ( pmove->pmove_fixed ) {
			if ( msec > pmove->pmove_msec ) {
				msec = pmove->pmove_msec;
			}
		}
		else {
			if ( msec > 66 ) {
				msec = 66;
			}
		}
		pmove->cmd.serverTime = pmove->ps->commandTime + msec;
		NPmoveSingle( pmove );

		if ( pmove->ps->pm_flags & PMF_JUMP_HELD ) {
			pmove->cmd.upmove = 20;
		}
	}

	//PM_CheckStuck();

}

#endif
