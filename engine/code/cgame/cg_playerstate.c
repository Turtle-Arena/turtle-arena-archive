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
// cg_playerstate.c -- this file acts on changes in a new playerState_t
// With normal play, this will be done after local prediction, but when
// following another player or playing back a demo, it will be checked
// when the snapshot transitions like all the other entities

#include "cg_local.h"

#ifndef TURTLEARENA // NO_AMMO_WARNINGS
/*
==============
CG_CheckAmmo

If the ammo has gone low enough to generate the warning, play a sound
==============
*/
void CG_CheckAmmo( void ) {
	int		i;
	int		total;
	int		previous;
#ifndef TA_WEAPSYS_EX
	int		weapons;
#endif

	// see about how many seconds of ammo we have remaining
#ifndef TA_WEAPSYS_EX
	weapons = cg.cur_ps->stats[ STAT_WEAPONS ];
#endif
	total = 0;
#ifdef TA_WEAPSYS_EX
	i = cg.cur_ps->weapon;
#else
#ifdef TA_WEAPSYS
	for ( i = 1 ; i < BG_NumWeaponGroups() ; i++ )
#else
	for ( i = WP_MACHINEGUN ; i < WP_NUM_WEAPONS ; i++ )
#endif
#endif
	{
#ifdef TA_WEAPSYS
		if (!BG_WeapUseAmmo(i)) {
#ifndef TA_WEAPSYS_EX
			continue;
#endif
		}
#ifdef TA_WEAPSYS_EX
		else
#endif
#endif
#ifndef TA_WEAPSYS_EX
		if ( ! ( weapons & ( 1 << i ) ) ) {
			continue;
		}
#endif
#ifdef TA_WEAPSYS
		total += cg.cur_ps->ammo[i] * bg_weapongroupinfo[i].weapon[0]->attackDelay;
#else
		switch ( i ) {
		case WP_ROCKET_LAUNCHER:
		case WP_GRENADE_LAUNCHER:
		case WP_RAILGUN:
		case WP_SHOTGUN:
#ifdef MISSIONPACK
		case WP_PROX_LAUNCHER:
#endif
			total += cg.cur_ps->ammo[i] * 1000;
			break;
		default:
			total += cg.cur_ps->ammo[i] * 200;
			break;
		}
#endif
		if ( total >= 5000 ) {
			cg.cur_lc->lowAmmoWarning = 0;
			return;
		}
	}

	previous = cg.cur_lc->lowAmmoWarning;

	if ( total == 0 ) {
		cg.cur_lc->lowAmmoWarning = 2;
	} else {
		cg.cur_lc->lowAmmoWarning = 1;
	}

	// play a sound on transitions
	if ( cg.cur_lc->lowAmmoWarning != previous ) {
		trap_S_StartLocalSound( cgs.media.noAmmoSound, CHAN_LOCAL_SOUND );
	}
}
#endif

/*
==============
CG_DamageFeedback
==============
*/
void CG_DamageFeedback( int yawByte, int pitchByte, int damage ) {
	float		left, front, up;
	float		kick;
	int			health;
	float		scale;
	vec3_t		dir;
	vec3_t		angles;
	float		dist;
	float		yaw, pitch;

	// show the attacking player's head and name in corner
	cg.cur_lc->attackerTime = cg.time;

	// the lower on health you are, the greater the view kick will be
	health = cg.cur_ps->stats[STAT_HEALTH];
	if ( health < 40 ) {
		scale = 1;
	} else {
		scale = 40.0 / health;
	}
	kick = damage * scale;

	if (kick < 5)
		kick = 5;
	if (kick > 10)
		kick = 10;

	// if yaw and pitch are both 255, make the damage always centered (falling, etc)
	if ( yawByte == 255 && pitchByte == 255 ) {
		cg.cur_lc->damageX = 0;
		cg.cur_lc->damageY = 0;
		cg.cur_lc->v_dmg_roll = 0;
		cg.cur_lc->v_dmg_pitch = -kick;
	} else {
		// positional
		pitch = pitchByte / 255.0 * 360;
		yaw = yawByte / 255.0 * 360;

		angles[PITCH] = pitch;
		angles[YAW] = yaw;
		angles[ROLL] = 0;

		AngleVectors( angles, dir, NULL, NULL );
		VectorSubtract( vec3_origin, dir, dir );

		front = DotProduct (dir, cg.refdef.viewaxis[0] );
		left = DotProduct (dir, cg.refdef.viewaxis[1] );
		up = DotProduct (dir, cg.refdef.viewaxis[2] );

		dir[0] = front;
		dir[1] = left;
		dir[2] = 0;
		dist = VectorLength( dir );
		if ( dist < 0.1 ) {
			dist = 0.1f;
		}

		cg.cur_lc->v_dmg_roll = kick * left;
		
		cg.cur_lc->v_dmg_pitch = -kick * front;

		if ( front <= 0.1 ) {
			front = 0.1f;
		}
		cg.cur_lc->damageX = -left / front;
		cg.cur_lc->damageY = up / dist;
	}

	// clamp the position
	if ( cg.cur_lc->damageX > 1.0 ) {
		cg.cur_lc->damageX = 1.0;
	}
	if ( cg.cur_lc->damageX < - 1.0 ) {
		cg.cur_lc->damageX = -1.0;
	}

	if ( cg.cur_lc->damageY > 1.0 ) {
		cg.cur_lc->damageY = 1.0;
	}
	if ( cg.cur_lc->damageY < - 1.0 ) {
		cg.cur_lc->damageY = -1.0;
	}

	// don't let the screen flashes vary as much
	if ( kick > 10 ) {
		kick = 10;
	}
	cg.cur_lc->damageValue = kick;
	cg.cur_lc->v_dmg_time = cg.time + DAMAGE_TIME;
	cg.cur_lc->damageTime = cg.snap->serverTime;
}




/*
================
CG_Respawn

A respawn happened this snapshot
================
*/
#ifdef TA_SPLITVIEW
void CG_Respawn( int clientNum ) {
	int i;

	// no error decay on player movement
	cg.thisFrameTeleport = qtrue;

	for (i = 0; i < MAX_SPLITVIEW; i++) {
		if (clientNum != -1 && (cg.snap->lcIndex[i] == -1 || cg.snap->pss[cg.snap->lcIndex[i]].clientNum != clientNum)) {
			continue;
		}

#ifndef TA_WEAPSYS_EX
		// display weapons available
		cg.localClients[i].weaponSelectTime = cg.time;

		// select the weapon the server says we are using
		cg.localClients[i].weaponSelect = cg.snap->pss[cg.snap->lcIndex[i]].weapon;
#endif
#ifdef TA_HOLDSYS/*2*/
		cg.localClients[i].holdableSelect = cg.snap->pss[cg.snap->lcIndex[i]].holdableIndex;
#endif
#ifdef IOQ3ZTM // NEW_CAM
		cg.localClients[i].camRotDir = 0;
		cg.localClients[i].camLeft = qfalse;
		cg.localClients[i].camRight = qfalse;
		cg.localClients[i].camReseting = qfalse;
#ifdef TURTLEARENA // CAMERA
		cg.localClients[i].camDistance = 0;
#endif
#endif
	}
}
#else
void CG_Respawn( void ) {
	// no error decay on player movement
	cg.thisFrameTeleport = qtrue;

#ifndef TA_WEAPSYS_EX
	// display weapons available
	cg.localClient.weaponSelectTime = cg.time;

	// select the weapon the server says we are using
	cg.localClient.weaponSelect = cg.snap->ps.weapon;
#endif
#ifdef TA_HOLDSYS/*2*/
	cg.localClient.holdableSelect = cg.snap->ps.holdableIndex;
#endif
#ifdef IOQ3ZTM // NEW_CAM
	cg.localClient.camRotDir = 0;
	cg.localClient.camLeft = qfalse;
	cg.localClient.camRight = qfalse;
	cg.localClient.camReseting = qfalse;
#ifdef TURTLEARENA // CAMERA
	cg.localClient.camDistance = 0;
#endif
#endif
}
#endif

extern char *eventnames[];

/*
==============
CG_CheckPlayerstateEvents
==============
*/
void CG_CheckPlayerstateEvents( playerState_t *ps, playerState_t *ops ) {
	int			i;
	int			event;
	centity_t	*cent;

	if ( ps->externalEvent && ps->externalEvent != ops->externalEvent ) {
		cent = &cg_entities[ ps->clientNum ];
		cent->currentState.event = ps->externalEvent;
		cent->currentState.eventParm = ps->externalEventParm;
		CG_EntityEvent( cent, cent->lerpOrigin );
	}

	cent = &cg.cur_lc->predictedPlayerEntity; // cg_entities[ ps->clientNum ];
	// go through the predictable events buffer
	for ( i = ps->eventSequence - MAX_PS_EVENTS ; i < ps->eventSequence ; i++ ) {
		// if we have a new predictable event
		if ( i >= ops->eventSequence
			// or the server told us to play another event instead of a predicted event we already issued
			// or something the server told us changed our prediction causing a different event
			|| (i > ops->eventSequence - MAX_PS_EVENTS && ps->events[i & (MAX_PS_EVENTS-1)] != ops->events[i & (MAX_PS_EVENTS-1)]) ) {

			event = ps->events[ i & (MAX_PS_EVENTS-1) ];
			cent->currentState.event = event;
			cent->currentState.eventParm = ps->eventParms[ i & (MAX_PS_EVENTS-1) ];
			CG_EntityEvent( cent, cent->lerpOrigin );

			cg.cur_lc->predictableEvents[ i & (MAX_PREDICTED_EVENTS-1) ] = event;

			cg.cur_lc->eventSequence++;
		}
	}
}

/*
==================
CG_CheckChangedPredictableEvents
==================
*/
void CG_CheckChangedPredictableEvents( playerState_t *ps ) {
	int i;
	int event;
	centity_t	*cent;

	cent = &cg.cur_lc->predictedPlayerEntity;
	for ( i = ps->eventSequence - MAX_PS_EVENTS ; i < ps->eventSequence ; i++ ) {
		//
		if (i >= cg.cur_lc->eventSequence) {
			continue;
		}
		// if this event is not further back in than the maximum predictable events we remember
		if (i > cg.cur_lc->eventSequence - MAX_PREDICTED_EVENTS) {
			// if the new playerstate event is different from a previously predicted one
			if ( ps->events[i & (MAX_PS_EVENTS-1)] != cg.cur_lc->predictableEvents[i & (MAX_PREDICTED_EVENTS-1) ] ) {

				event = ps->events[ i & (MAX_PS_EVENTS-1) ];
				cent->currentState.event = event;
				cent->currentState.eventParm = ps->eventParms[ i & (MAX_PS_EVENTS-1) ];
				CG_EntityEvent( cent, cent->lerpOrigin );

				cg.cur_lc->predictableEvents[ i & (MAX_PREDICTED_EVENTS-1) ] = event;

				if ( cg_showmiss.integer ) {
					CG_Printf("WARNING: changed predicted event\n");
				}
			}
		}
	}
}

/*
==================
pushReward
==================
*/
#ifdef TA_MISC // COMIC_ANNOUNCER
static void pushReward(int annoucement, qhandle_t shader, int rewardCount)
#else
static void pushReward(sfxHandle_t sfx, qhandle_t shader, int rewardCount)
#endif
{
	if (cg.cur_lc->rewardStack < (MAX_REWARDSTACK-1)) {
		cg.cur_lc->rewardStack++;
#ifdef TA_MISC // COMIC_ANNOUNCER
		cg.cur_lc->rewardAnnoucement[cg.cur_lc->rewardStack] = annoucement;
#else
		cg.cur_lc->rewardSound[cg.cur_lc->rewardStack] = sfx;
#endif
		cg.cur_lc->rewardShader[cg.cur_lc->rewardStack] = shader;
		cg.cur_lc->rewardCount[cg.cur_lc->rewardStack] = rewardCount;
	}
}

/*
==================
CG_CheckLocalSounds
==================
*/
void CG_CheckLocalSounds( playerState_t *ps, playerState_t *ops ) {
#ifdef TURTLEARENA // NOARMOR
	int			highScore, reward;
#else
	int			highScore, health, armor, reward;
#endif
#ifndef TURTLEARENA // AWARDS
	sfxHandle_t sfx;
#endif

	// don't play the sounds if the player just changed teams
	if ( ps->persistant[PERS_TEAM] != ops->persistant[PERS_TEAM] ) {
		return;
	}

	// hit changes
	if ( ps->persistant[PERS_HITS] > ops->persistant[PERS_HITS] ) {
#ifndef TURTLEARENA // NOARMOR
		armor  = ps->persistant[PERS_ATTACKEE_ARMOR] & 0xff;
		health = ps->persistant[PERS_ATTACKEE_ARMOR] >> 8;
#endif
#if defined MISSIONPACK && !defined TURTLEARENA // NOARMOR
		if (armor > 50 ) {
			trap_S_StartLocalSound( cgs.media.hitSoundHighArmor, CHAN_LOCAL_SOUND );
		} else if (armor || health > 100) {
			trap_S_StartLocalSound( cgs.media.hitSoundLowArmor, CHAN_LOCAL_SOUND );
		} else {
			trap_S_StartLocalSound( cgs.media.hitSound, CHAN_LOCAL_SOUND );
		}
#else
		trap_S_StartLocalSound( cgs.media.hitSound, CHAN_LOCAL_SOUND );
#endif
	} else if ( ps->persistant[PERS_HITS] < ops->persistant[PERS_HITS] ) {
		trap_S_StartLocalSound( cgs.media.hitTeamSound, CHAN_LOCAL_SOUND );
	}

	// health changes of more than -1 should make pain sounds
	if ( ps->stats[STAT_HEALTH] < ops->stats[STAT_HEALTH] - 1 ) {
		if ( ps->stats[STAT_HEALTH] > 0 ) {
			CG_PainEvent( &cg.cur_lc->predictedPlayerEntity, ps->stats[STAT_HEALTH] );
		}
	}


	// if we are going into the intermission, don't start any voices
	if ( cg.intermissionStarted ) {
		return;
	}

	// reward sounds
	reward = qfalse;
	if (ps->persistant[PERS_CAPTURES] != ops->persistant[PERS_CAPTURES]) {
#ifdef TA_MISC // COMIC_ANNOUNCER
		pushReward(ANNOUNCE_CAPTURE, cgs.media.medalCapture, ps->persistant[PERS_CAPTURES]);
#else
		pushReward(cgs.media.captureAwardSound, cgs.media.medalCapture, ps->persistant[PERS_CAPTURES]);
#endif
		reward = qtrue;
		//Com_Printf("capture\n");
	}
#ifndef TURTLEARENA // AWARDS
	if (ps->persistant[PERS_IMPRESSIVE_COUNT] != ops->persistant[PERS_IMPRESSIVE_COUNT]) {
#ifdef MISSIONPACK
		if (ps->persistant[PERS_IMPRESSIVE_COUNT] == 1) {
			sfx = cgs.media.firstImpressiveSound;
		} else {
			sfx = cgs.media.impressiveSound;
		}
#else
		sfx = cgs.media.impressiveSound;
#endif
		pushReward(sfx, cgs.media.medalImpressive, ps->persistant[PERS_IMPRESSIVE_COUNT]);
		reward = qtrue;
		//Com_Printf("impressive\n");
	}
	if (ps->persistant[PERS_EXCELLENT_COUNT] != ops->persistant[PERS_EXCELLENT_COUNT]) {
#ifdef MISSIONPACK
		if (ps->persistant[PERS_EXCELLENT_COUNT] == 1) {
			sfx = cgs.media.firstExcellentSound;
		} else {
			sfx = cgs.media.excellentSound;
		}
#else
		sfx = cgs.media.excellentSound;
#endif
		pushReward(sfx, cgs.media.medalExcellent, ps->persistant[PERS_EXCELLENT_COUNT]);
		reward = qtrue;
		//Com_Printf("excellent\n");
	}
	if (ps->persistant[PERS_GAUNTLET_FRAG_COUNT] != ops->persistant[PERS_GAUNTLET_FRAG_COUNT]) {
#ifdef MISSIONPACK
		if (ops->persistant[PERS_GAUNTLET_FRAG_COUNT] == 1) {
			sfx = cgs.media.firstHumiliationSound;
		} else {
			sfx = cgs.media.humiliationSound;
		}
#else
		sfx = cgs.media.humiliationSound;
#endif
		pushReward(sfx, cgs.media.medalGauntlet, ps->persistant[PERS_GAUNTLET_FRAG_COUNT]);
		reward = qtrue;
		//Com_Printf("guantlet frag\n");
	}
#endif
	if (ps->persistant[PERS_DEFEND_COUNT] != ops->persistant[PERS_DEFEND_COUNT]) {
#ifdef TA_MISC // COMIC_ANNOUNCER
		pushReward(ANNOUNCE_DEFENSE, cgs.media.medalDefend, ps->persistant[PERS_DEFEND_COUNT]);
#else
		pushReward(cgs.media.defendSound, cgs.media.medalDefend, ps->persistant[PERS_DEFEND_COUNT]);
#endif
		reward = qtrue;
		//Com_Printf("defend\n");
	}
	if (ps->persistant[PERS_ASSIST_COUNT] != ops->persistant[PERS_ASSIST_COUNT]) {
#ifdef TA_MISC // COMIC_ANNOUNCER
		pushReward(ANNOUNCE_ASSIST, cgs.media.medalAssist, ps->persistant[PERS_ASSIST_COUNT]);
#else
		pushReward(cgs.media.assistSound, cgs.media.medalAssist, ps->persistant[PERS_ASSIST_COUNT]);
#endif
		reward = qtrue;
		//Com_Printf("assist\n");
	}
#if !defined NOTRATEDM || !defined TURTLEARENA // AWARDS
	// if any of the player event bits changed
	if (ps->persistant[PERS_PLAYEREVENTS] != ops->persistant[PERS_PLAYEREVENTS]) {
#ifndef TURTLEARENA // AWARDS
		if ((ps->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_DENIEDREWARD) !=
				(ops->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_DENIEDREWARD)) {
			trap_S_StartLocalSound( cgs.media.deniedSound, CHAN_ANNOUNCER );
		} else if ((ps->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_GAUNTLETREWARD) !=
				(ops->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_GAUNTLETREWARD)) {
			trap_S_StartLocalSound( cgs.media.humiliationSound, CHAN_ANNOUNCER );
		}
#endif
#ifndef NOTRATEDM // Disable strong lang.
		else if ((ps->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_HOLYSHIT) !=
				(ops->persistant[PERS_PLAYEREVENTS] & PLAYEREVENT_HOLYSHIT)) {
			trap_S_StartLocalSound( cgs.media.holyShitSound, CHAN_ANNOUNCER );
		}
#endif
		reward = qtrue;
	}
#endif

	// check for flag pickup
	if ( cgs.gametype >= GT_TEAM ) {
		if ((ps->powerups[PW_REDFLAG] != ops->powerups[PW_REDFLAG] && ps->powerups[PW_REDFLAG]) ||
			(ps->powerups[PW_BLUEFLAG] != ops->powerups[PW_BLUEFLAG] && ps->powerups[PW_BLUEFLAG]) ||
			(ps->powerups[PW_NEUTRALFLAG] != ops->powerups[PW_NEUTRALFLAG] && ps->powerups[PW_NEUTRALFLAG]) )
		{
			trap_S_StartLocalSound( cgs.media.youHaveFlagSound, CHAN_ANNOUNCER );
		}
	}

	// lead changes
	if (!reward
#ifdef TA_SP // Don't talk about lead changes in single player/co-op
		&& cgs.gametype != GT_SINGLE_PLAYER
#endif
		) {
		//
		if ( !cg.warmup ) {
			// never play lead changes during warmup
			if ( ps->persistant[PERS_RANK] != ops->persistant[PERS_RANK] ) {
				if ( cgs.gametype < GT_TEAM) {
					if (  ps->persistant[PERS_RANK] == 0 ) {
#ifdef TA_MISC // COMIC_ANNOUNCER
#ifdef TA_SPLITVIEW
						CG_AddAnnouncement(ANNOUNCE_YOUHAVETAKENTHELEAD, cg.cur_lc-cg.localClients);
#else
						CG_AddAnnouncement(ANNOUNCE_YOUHAVETAKENTHELEAD);
#endif
#else
						CG_AddBufferedSound(cgs.media.takenLeadSound);
#endif
					} else
#ifdef TA_SPLITVIEW // ZTM: Don't play tied or lost lead when there are multiple local clients, multiple sounds play and its annoying.
					if (cg.snap->numPSs <= 1) {
#endif
					if ( ps->persistant[PERS_RANK] == RANK_TIED_FLAG ) {
#ifdef TA_MISC // COMIC_ANNOUNCER
#ifdef TA_SPLITVIEW
						CG_AddAnnouncement(ANNOUNCE_YOURTIEDFORTHELEAD, cg.cur_lc-cg.localClients);
#else
						CG_AddAnnouncement(ANNOUNCE_YOURTIEDFORTHELEAD);
#endif
#else
						CG_AddBufferedSound(cgs.media.tiedLeadSound);
#endif
					} else if ( ( ops->persistant[PERS_RANK] & ~RANK_TIED_FLAG ) == 0 ) {
#ifdef TA_MISC // COMIC_ANNOUNCER
#ifdef TA_SPLITVIEW
						CG_AddAnnouncement(ANNOUNCE_YOULOSTTHELEAD, cg.cur_lc-cg.localClients);
#else
						CG_AddAnnouncement(ANNOUNCE_YOULOSTTHELEAD);
#endif
#else
						CG_AddBufferedSound(cgs.media.lostLeadSound);
#endif
					}
#ifdef TA_SPLITVIEW
					}
#endif
				}
			}
		}
	}

	// timelimit warnings
	if ( cgs.timelimit > 0 ) {
		int		msec;

		msec = cg.time - cgs.levelStartTime;
		if ( !( cg.timelimitWarnings & 4 ) && msec > ( cgs.timelimit * 60 + 2 ) * 1000 ) {
			cg.timelimitWarnings |= 1 | 2 | 4;
			trap_S_StartLocalSound( cgs.media.suddenDeathSound, CHAN_ANNOUNCER );
		}
		else if ( !( cg.timelimitWarnings & 2 ) && msec > (cgs.timelimit - 1) * 60 * 1000 ) {
			cg.timelimitWarnings |= 1 | 2;
			trap_S_StartLocalSound( cgs.media.oneMinuteSound, CHAN_ANNOUNCER );
		}
		else if ( cgs.timelimit > 5 && !( cg.timelimitWarnings & 1 ) && msec > (cgs.timelimit - 5) * 60 * 1000 ) {
			cg.timelimitWarnings |= 1;
			trap_S_StartLocalSound( cgs.media.fiveMinuteSound, CHAN_ANNOUNCER );
		}
	}

	// fraglimit warnings
	if ( cgs.fraglimit > 0 && cgs.gametype < GT_CTF) {
		highScore = cgs.scores1;
		if ( !( cg.fraglimitWarnings & 4 ) && highScore == (cgs.fraglimit - 1) ) {
			cg.fraglimitWarnings |= 1 | 2 | 4;
			CG_AddBufferedSound(cgs.media.oneFragSound);
		}
		else if ( cgs.fraglimit > 2 && !( cg.fraglimitWarnings & 2 ) && highScore == (cgs.fraglimit - 2) ) {
			cg.fraglimitWarnings |= 1 | 2;
			CG_AddBufferedSound(cgs.media.twoFragSound);
		}
		else if ( cgs.fraglimit > 3 && !( cg.fraglimitWarnings & 1 ) && highScore == (cgs.fraglimit - 3) ) {
			cg.fraglimitWarnings |= 1;
			CG_AddBufferedSound(cgs.media.threeFragSound);
		}
	}
}

/*
===============
CG_TransitionPlayerState

===============
*/
void CG_TransitionPlayerState( playerState_t *ps, playerState_t *ops ) {
	// check for changing follow mode
	if ( ps->clientNum != ops->clientNum ) {
		cg.thisFrameTeleport = qtrue;
		// make sure we don't get any unwanted transition effects
		*ops = *ps;
	}

	// damage events (player is getting wounded)
	if ( ps->damageEvent != ops->damageEvent && ps->damageCount ) {
		CG_DamageFeedback( ps->damageYaw, ps->damagePitch, ps->damageCount );
	}

	// respawning
	if ( ps->persistant[PERS_SPAWN_COUNT] != ops->persistant[PERS_SPAWN_COUNT] ) {
#ifdef TA_SPLITVIEW // Don't reset all clients
		CG_Respawn(ps->clientNum);
#else
		CG_Respawn();
#endif
	}

	if ( cg.mapRestart ) {
#ifdef TA_SPLITVIEW // Reset all clients
		CG_Respawn(-1);
#else
		CG_Respawn();
#endif
		cg.mapRestart = qfalse;
	}

	if ( cg.cur_ps->pm_type != PM_INTERMISSION 
		&& ps->persistant[PERS_TEAM] != TEAM_SPECTATOR ) {
		CG_CheckLocalSounds( ps, ops );
	}

#ifndef TA_WEAPSYS // ZTM: No ammo warnings.
	// check for going low on ammo
	CG_CheckAmmo();
#endif

	// run events
	CG_CheckPlayerstateEvents( ps, ops );

	// smooth the ducking viewheight change
	if ( ps->viewheight != ops->viewheight ) {
		cg.cur_lc->duckChange = ps->viewheight - ops->viewheight;
		cg.cur_lc->duckTime = cg.time;
	}
}

