// g_npcthink.c
//
// JPL (jpl@ing.puc.cl) - The Dark Conjunction - ACE Team
//

#include "g_local.h"
#include "../qcommon/q_shared.h"

#ifdef SP_NPC

#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"


#define SEALORD_MAX_MOV	110
#define SEALORD_STEP	30
#define SEALORD_QUAKE_TIME	1300

int in_earthquake=0;

// JPL - NPC - Non Playable Characters (monsters, maybe "helpers")
// thinking code -> AI & some tweaks to emulate really thinking..

int CheckInObj(vec3_t pos,vec3_t obj)
{
	if (fabs(obj[0]-pos[0])<15 &&
		fabs(obj[1]-pos[1])<15 &&
		fabs(obj[2]-pos[2])<32)
			return 1;
	return 0;
}

void NPC_FindTarget(gentity_t *ent)
{
	npcData_t *npc=&ent->ns;
	vec3_t dir,new_pos;

	VectorSubtract(npc->enemy->r.currentOrigin,ent->r.currentOrigin,dir);
	vectoangles(dir,new_pos);
	npc->ideal_angle=new_pos[1];
}

float NPC_ChangeViewAngle(float angle, float ideal_angle, float speed)
{
	float move;

	angle = AngleMod(angle);
	ideal_angle = AngleMod(ideal_angle);
	if (angle == ideal_angle) return angle;
	move = ideal_angle - angle;
	if (ideal_angle > angle) {
		if (move > 180.0) move -= 360.0;
	}
	else {
		if (move < -180.0) move += 360.0;
	}
	if (move > 0) {
		if (move > speed) move = speed;
	}
	else {
		if (move < -speed) move = -speed;
	}
	return AngleMod(angle + move);
}

int NPC_PointAreaNum(vec3_t origin)
{
	int areanum, numareas, areas[10];
	vec3_t start,end;

	areanum = trap_AAS_PointAreaNum(origin);
	if (areanum) return areanum;
	VectorCopy(origin, start);
	start[2] -= 24;
	VectorCopy(origin, end);
	end[2] += 24;
	numareas = trap_AAS_TraceAreas(start, end, areas, NULL, 10);
	if (numareas > 0) return areas[0];
	return 0;
}

qboolean NPC_InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles)
{
	int i;
	float diff, angle;

	for (i = 0; i < 2; i++) {
		angle = AngleMod(viewangles[i]);
		angles[i] = AngleMod(angles[i]);
		diff = angles[i] - angle;
		if (angles[i] > angle) {
			if (diff > 180.0) diff -= 360.0;
		}
		else {
			if (diff < -180.0) diff += 360.0;
		}
		if (diff > 0) {
			if (diff > fov * 0.5) return qfalse;
		}
		else {
			if (diff < -fov * 0.5) return qfalse;
		}
	}
	return qtrue;
}

int NPC_IsVisible(gentity_t *viewer,gentity_t *ent)
{
	trace_t trace;
	vec3_t mins={-2,-2,-2},maxs={2,2,2};
	int fov_npc;
	float *eyes_npc,*ent_org;
	vec3_t eyes,target,temp;

	eyes_npc=viewer->npc->eye;
	fov_npc=viewer->npc->fov;

	VectorAdd(eyes_npc,viewer->r.currentOrigin,eyes);

	ent_org=ent->r.currentOrigin;

// check if its in NPC's field of vision

	VectorSubtract(ent_org,eyes,target);
	vectoangles(target,temp);
	if (!NPC_InFieldOfVision(viewer->ns.ps.viewangles, fov_npc, temp))
		return 0;

	VectorCopy(ent_org,target);
	VectorAdd(ent->r.mins,ent->r.maxs,temp);
	temp[0]/=2.0;
	temp[1]/=2.0;
	temp[2]/=2.0;
	VectorAdd(target,temp,target);

	// center

	trap_Trace(&trace, eyes, mins,maxs, target, viewer->s.number, CONTENTS_SOLID);
		// |CONTENTS_PLAYERCLIP  // what da hell?
	if (trace.fraction==1)
	{
		return 1;
	}

	// bottom

	target[2]-=temp[2];
	trap_Trace(&trace, eyes, mins,maxs, target, viewer->s.number, CONTENTS_SOLID);
		// |CONTENTS_PLAYERCLIP  // what da hell?
	if (trace.fraction==1)
	{
		return 1;
	}

	// top

	target[2]+=temp[2];
	target[2]+=temp[2];
	trap_Trace(&trace, eyes, mins,maxs, target, viewer->s.number, CONTENTS_SOLID);
		// |CONTENTS_PLAYERCLIP  // what da hell?
	if (trace.fraction==1)
	{
		return 1;
	}

	return 0;
}

void NPC_RoamGoal(gentity_t *ent, vec3_t obj,vec3_t goal,int spread,int mindist) {
	int pc, i;
	float len, rnd;
	vec3_t dir, bestorg, belowbestorg;
	trace_t trace;
	playerState_t *bs=&ent->ns.ps;

	for (i = 0; i < 10; i++) {
		//start at the NPC origin
		VectorCopy(obj, bestorg);
		rnd = random();
		if (rnd > 0.25) {
			//add a random value to the x-coordinate
			if (random() < 0.5) bestorg[0] -= spread * random() + mindist;
			else bestorg[0] += spread * random() + mindist;
		}
		if (rnd < 0.75) {
			//add a random value to the y-coordinate
			if (random() < 0.5) bestorg[1] -= spread * random() + mindist;
			else bestorg[1] += spread * random() + mindist;
		}
		//add a random value to the z-coordinate
		bestorg[2] += 2 * ent->npc->jumpHeight * crandom();
		//trace a line from the origin to the roam target
		trap_Trace(&trace, bs->origin, NULL, NULL, bestorg, ent->s.number, MASK_SOLID);
		//direction and length towards the roam target
		VectorSubtract(trace.endpos, bs->origin, dir);
		len = VectorNormalize(dir);
		//if the roam target is far away anough
		if (len > mindist) {
			//the roam target is in the given direction before walls
			VectorScale(dir, len * trace.fraction - 40, dir);
			VectorAdd(bs->origin, dir, bestorg);
			//get the coordinates of the floor below the roam target
			belowbestorg[0] = bestorg[0];
			belowbestorg[1] = bestorg[1];
			belowbestorg[2] = bestorg[2] - 800;
			trap_Trace(&trace, bestorg, NULL, NULL, belowbestorg, ent->s.number, MASK_SOLID);
			//
			if (!trace.startsolid) {
				trace.endpos[2]++;
				pc = trap_PointContents(trace.endpos, ent->s.number);
				if (!(pc & (CONTENTS_LAVA | CONTENTS_SLIME))) {
					VectorCopy(bestorg, goal);
					return;
				}
			}
		}
	}
	VectorCopy(bestorg, goal);
}

void NPC_CheckAround(gentity_t *ent)
{
	npcData_t	*npc;
	int i;
	int npcType=ent->npc->npcType;

	npc=&ent->ns;

	switch(npc->command)
	{
	case NPC_COM_ATTACK:
		if (npc->enemy->health<=0)
		{
			npc->command=NPC_COM_SEARCH;
			npc->enemy=NULL;
		}
		break;

	default:
		for (i=0;i<level.num_entities;i++)
		{
			if (!g_entities[i].inuse) continue;	// enemy must be an entity in use

			if (g_entities[i].s.eType!=ET_PLAYER && g_entities[i].s.eType!=ET_NPC) continue;

			if (g_entities[i].s.eType==ET_PLAYER && (npcType==NPC_SOLDIER2		// if im a solidier or pilot, do not attack the player
						|| npcType==NPC_SOLDIER1 || npcType==NPC_PILOT )) continue;

			if (g_entities[i].s.eType==ET_NPC && ( npcType==NPC_SOLDIER2	// if im a soldier or a pilot, do not attack
						|| npcType==NPC_SOLDIER1 || npcType==NPC_PILOT )		// other pilots or soldiers
					&& (g_entities[i].npc->npcType==NPC_SOLDIER2 || g_entities[i].npc->npcType==NPC_PILOT
					|| g_entities[i].npc->npcType==NPC_SOLDIER1)) continue;

			if (g_entities[i].s.eType==ET_NPC && ( npcType!=NPC_SOLDIER2	// if im a monster do not attack
						&& npcType!=NPC_SOLDIER1 && npcType!=NPC_PILOT )		// other monsters
					&& (g_entities[i].npc->npcType!=NPC_SOLDIER2 && g_entities[i].npc->npcType!=NPC_PILOT
					&& g_entities[i].npc->npcType!=NPC_SOLDIER1)) continue;

			if (g_entities[i].health<=0) continue;
			if (!NPC_IsVisible(ent,&g_entities[i])) continue;

			if (( ent->spawnflags & 8 )	&& !npc->inFlight)	// taunt when enemy appears
			{
				npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_TAUNT;
				npc->ps.legsTimer=ent->npc->animTimes[ANPC_TAUNT];
				npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_TAUNT];
				/*if (ent->npc->npcType==NPC_SEALORD)
					npc->quakeTime=level.time+SEALORD_QUAKE_TIME;*/
			}

			npc->command=NPC_COM_ATTACK;
			npc->enemy=&g_entities[i];
			VectorCopy(npc->enemy->r.currentOrigin,npc->real_obj);
			VectorCopy(ent->r.currentOrigin,npc->obj);
			NPC_FindTarget(ent);
			npc->toFire=0;
			npc->attackTime=0;
		}
	}
}

int NPC_TravelFlags(gentity_t *ent)
{
	int tfl;

	if (ent->npc->flags & NPCF_WALKANDFLY)
	{
		tfl=TFL_WALK|TFL_BARRIERJUMP|
			TFL_JUMP|TFL_LADDER|
			TFL_WALKOFFLEDGE|
			TFL_SWIM|TFL_WATERJUMP|	// ???
			TFL_AIR|TFL_WATER|TFL_FUNCBOB|
			TFL_FLIGHT ;
	}
	else
		tfl=TFL_WALK|TFL_BARRIERJUMP|
			TFL_JUMP|TFL_LADDER|
			TFL_WALKOFFLEDGE|
			TFL_SWIM|TFL_WATERJUMP|	// ???
			TFL_AIR|TFL_WATER|TFL_FUNCBOB;

	return tfl;
}

int NPC_RecalcLinearObj(gentity_t *ent)
{
	npcData_t *npc=&ent->ns;
	int obj_area=NPC_PointAreaNum(npc->real_obj);
	int my_area;
	aas_predictroute_t route;
	vec3_t *my_obj,dir;
	int len;
	vec3_t from_obj;
	int tries=10;
	int tfl;

	tfl=NPC_TravelFlags(ent);

	// if there is an old objective, calculate the new one from that position, and not from the exact current position
	// this avoids recalculating the same objective a lot of times when you are getting near the objective

	VectorCopy(ent->r.currentOrigin,from_obj);
retry:
	if ((!npc->obj[0] && !npc->obj[1] && !npc->obj[2]) || random()<0.25
		|| !CheckInObj(ent->r.currentOrigin,npc->obj))
	{
		VectorCopy(ent->r.currentOrigin,from_obj);
	}
	else
	{
		VectorSubtract(npc->obj,ent->r.currentOrigin,dir);
		VectorScale(dir,(rand()%4)+2,dir);
		VectorCopy(ent->r.currentOrigin,from_obj);
		VectorAdd(from_obj,dir,from_obj);
	}

	my_area=NPC_PointAreaNum(from_obj);

	if (my_area==obj_area || my_area==0)
	{
		my_obj=&npc->real_obj;
linearobj:

		VectorSubtract(*my_obj,from_obj,dir);
		len=VectorNormalize(dir);
		if (len>80)					// not to the goal directly if its far enough
		{
			VectorScale(dir,len/3.0,dir);
			VectorAdd(*my_obj,dir,*my_obj);
		}

//		NPC_RoamGoal(ent,*my_obj,npc->obj,20,0);

		if (CheckInObj(ent->r.currentOrigin,*my_obj) && tries>0)
		{
			VectorCopy(*my_obj,from_obj);
			tries--;
			goto retry;
		}

// check i can get there
/*		my_area=NPC_PointAreaNum(ent->r.currentOrigin);
		obj_area=NPC_PointAreaNum(*my_obj);

		trap_AAS_PredictRoute(&route, my_area, ent->r.currentOrigin,
			obj_area, tfl, 100, 5000,
			RSE_USETRAVELTYPE,
			0, 0, 0);

		if (route.time==0 && tries>0 && my_area!=obj_area)
		{
			tries--;
			goto retry;
		}
*/


		VectorCopy(*my_obj,npc->obj);

//		G_Printf("p: %s (%d)  ro: %s (%d)  o: %s \n",vtos(from_obj),my_area,vtos(npc->real_obj),obj_area,vtos(npc->obj));

		return 1;
	}

	// TODO: if im in contiguous areas, under certain circumnstances i can directly move on (not using AAS)

	trap_AAS_PredictRoute(&route, my_area, from_obj,
		obj_area, tfl, 1, 50,
		RSE_USETRAVELTYPE,
		0, 0, 0);

	if (route.time>0)
	{
		my_obj=&route.endpos;
		goto linearobj;
	}

	return 0;
}

#define NML_WALK	0
#define NML_RUN		1
#define NML_FLY		2

#define STEP_HEIGHT	8

// returns 0 if i could move (even jumping), 1 if its not possible to move forward

int NPC_CheckForJump(gentity_t *ent,usercmd_t *ucmd,int move_type,vec3_t dir)
{
	trace_t trace;
	vec3_t start,target,forward,mins,maxs;

	mins[0]=1;
	mins[1]=1;
	mins[2]=ent->r.mins[2]+2;
	maxs[0]=1;
	maxs[1]=1;
	maxs[2]=ent->r.maxs[2]-STEP_HEIGHT-1;

	VectorCopy(dir,forward);
	VectorNormalize(forward);
	VectorScale(forward,ucmd->forwardmove,forward);		// how much forward do we look

	// first test, see if we can walk forward (or move on stair steps)

	VectorCopy(ent->r.currentOrigin,start);
	start[2]+=STEP_HEIGHT-1;		// height of the NPCS jumping caps
	VectorAdd(start,forward,target);
	trap_Trace(&trace, start, mins,maxs, target, ent->s.number, CONTENTS_SOLID); // |CONTENTS_PLAYERCLIP
	if (trace.fraction==1)
	{
		return 0;	// no obstacle, can go on!
	}

	if (ent->npc->jumpHeight && !ent->ns.inFlight)
	{
		// second try, do we have to make a short jump?

		start[2]+=(ent->npc->jumpHeight-STEP_HEIGHT)/2;
		target[2]+=(ent->npc->jumpHeight-STEP_HEIGHT)/2;
		trap_Trace(&trace, start, mins,maxs, target, ent->s.number, CONTENTS_SOLID); // |CONTENTS_PLAYERCLIP
		if (trace.fraction==1)
		{
			ucmd->upmove=ent->npc->jumpHeight*0.75;
			return 0;
		}

		// third test, may i jump the max i can?

		start[2]+=(ent->npc->jumpHeight-STEP_HEIGHT)/2;
		target[2]+=(ent->npc->jumpHeight-STEP_HEIGHT)/2;
		trap_Trace(&trace, start, mins,maxs, target, ent->s.number, CONTENTS_SOLID); // |CONTENTS_PLAYERCLIP
		if (trace.fraction==1)
		{
			ucmd->upmove=ent->npc->jumpHeight*1.25;
			return 0;
		}
	}

	if ((level.time/10)%2)
	{
		if (!ent->ns.inFlight)
			ucmd->upmove=0;
		ucmd->forwardmove=0;
		ucmd->rightmove=0;
		return 1;
	}

	return 0;
}

int NPC_CalcAltitude(gentity_t *ent)
{
	trace_t trace;
	vec3_t start,target,mins,maxs;

	mins[0]=ent->r.mins[0];
	mins[1]=ent->r.mins[1];
	mins[2]=-2;
	maxs[0]=ent->r.maxs[0];
	maxs[1]=ent->r.maxs[1];
	maxs[2]=2;

	VectorCopy(ent->r.currentOrigin,start);
	start[2]+=ent->r.mins[2]+4;
	VectorCopy(start,target);
	target[2]-=100;
	trap_Trace(&trace, start, mins,maxs, target, ent->s.number, CONTENTS_SOLID); // |CONTENTS_PLAYERCLIP
	if (trace.fraction<1)
	{
		return ((int)(trace.fraction*100));	// altitude between 1 and 100
	}
	target[2]-=900;
	trap_Trace(&trace, start, mins,maxs, target, ent->s.number, CONTENTS_SOLID); // |CONTENTS_PLAYERCLIP
	if (trace.fraction<1)
	{
		return ((int)(trace.fraction*1000));	// altitude between 1 and 1000
	}
	return 1000;
}


int NPC_IsFlying(gentity_t *ent)
{
	ent->ns.altitude=NPC_CalcAltitude(ent);
	if (ent->npc->flags & NPCF_FLYONLY)
		return 1;
	if (!(ent->npc->flags & NPCF_WALKANDFLY))
		return 0;

	if (ent->ns.altitude<=STEP_HEIGHT)
		return 0;

	return 1;
}

void NPC_MoveLinear(gentity_t *ent,usercmd_t *ucmd,int move_type)
{
	npcData_t *npc=&ent->ns;
	vec3_t dir,new_pos;

	VectorSubtract(npc->obj,ent->r.currentOrigin,dir);
	vectoangles(dir,new_pos);

	if (move_type==NML_WALK)		// walking
	{
		ucmd->buttons=BUTTON_WALKING;
		ucmd->forwardmove=ent->npc->walkingSpeed;
	}
	else if (move_type==NML_RUN)	// running
	{
		ucmd->forwardmove=ent->npc->runningSpeed;
	}
	else if (move_type==NML_FLY)	// flying
	{
		VectorSubtract(npc->real_obj,ent->r.currentOrigin,dir);
		vectoangles(dir,new_pos);
		ucmd->forwardmove=ent->npc->walkingSpeed;
	}
	npc->ideal_angle=new_pos[1];

	if (NPC_CheckForJump(ent,ucmd,move_type,dir))
	{
		NPC_RecalcLinearObj(ent);
	}

}

int NPC_IdealDist(gentity_t *ent)
{
	int dist;
	int type = ent->npc->npcType;

	if (type == NPC_ANK)
		dist = 800;
	else if (type == NPC_SOLDIER1 || type == NPC_PILOT)
		dist = 1200;
	else if (type == NPC_BAT)
		dist = 1200;
	else
		dist = 0;

	dist *= ent->ns.shot_factor;
	return dist;
}


void NPC_ThinkMove(gentity_t *ent,usercmd_t *ucmd)
{
	npcData_t *npc=&ent->ns;
	int do_move=1;

	npc->inFlight=NPC_IsFlying(ent);
//	if (ent->npc->flags & NPCF_WALKANDFLY)
//		G_Printf("current altitude: %d     inflight = %d\n",npc->altitude,npc->inFlight);

	if (level.time>npc->checkTime)
	{
		NPC_CheckAround(ent);
		npc->checkTime=level.time+500+rand()%500;
	}


	switch (npc->command)
	{
	case NPC_COM_NONE:
		npc->command=NPC_COM_WAIT;
		break;
	case NPC_COM_WAIT:
		{


		}
		break;
	case NPC_COM_GOTO:
		{
			if (!ent->npc->walkingSpeed)
			{
				npc->command=NPC_COM_WAIT;
				break;
			}
			if (CheckInObj(npc->obj,ent->r.currentOrigin))
			{
				if (!ent->nextTrain)
				{
					npc->command=NPC_COM_WAIT;
				}
				else
				{
					// Turtle Man: TODO: TMNTPATHSYS, G_NextPath
					VectorCopy(ent->nextTrain->s.pos.trBase,npc->obj);
					ent->nextTrain=ent->nextTrain->nextTrain;
				}
				if (random()<0.3)
				{
					npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_STANDING;
					npc->ps.legsTimer=ent->npc->animTimes[ANPC_STANDING];
					npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_STANDING];
					break;
				}
			}
			NPC_MoveLinear(ent,ucmd,NML_WALK);
		}
		break;
	case NPC_COM_ATTACK:
		{
			vec3_t dir;
			int enemy_dist,ideal_dist,enemy_visible;

			VectorSubtract(npc->enemy->r.currentOrigin,ent->r.currentOrigin,dir);
			enemy_dist=VectorNormalize(dir);
			ideal_dist=NPC_IdealDist(ent);
			enemy_visible=NPC_IsVisible(ent,npc->enemy);

			if (npc->toFire==1)
			{
				NPC_FindTarget(ent);
				if (fabs(npc->ps.viewangles[1]-npc->ideal_angle)<(rand()%7) || 	ent->npc->npcType==NPC_SEALORD)
				{
					npc->toFire=0;
//					G_Printf("melee dist: %d   enemy_dist: %d",ent->npc->melee_dist,enemy_dist);

					if (!enemy_visible)
					{
						// if i cant see target, DO NOT FIRE
					}
					else if (ent->npc->npcType==NPC_SEALORD)
					{
						if (random()>0.7)
						{
							npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_ATTACK_MELEE;
							npc->ps.legsTimer=ent->npc->animTimes[ANPC_ATTACK_MELEE];
							npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_ATTACK_MELEE];
							npc->meleeTime=level.time+ent->npc->animTimes[ANPC_ATTACK_MELEE]*0.75;
						}
						else
						{
							npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_ATTACK_FAR;
							npc->ps.legsTimer=ent->npc->animTimes[ANPC_ATTACK_FAR];
							npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_ATTACK_FAR];
							npc->fireTime=level.time+ent->npc->animTimes[ANPC_ATTACK_FAR];
						}
					}
					else if ((enemy_dist<=ent->npc->melee_dist && ent->npc->melee_damage) || npc->shot_factor==0
						|| (enemy_dist>=ent->npc->melee_dist*2 && ent->npc->npcType==NPC_HULK)	// far earthquake
						|| ((ent->npc->flags & NPCF_WALKANDFLY) && npc->inFlight))	// bat fire from air
					{
						npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_ATTACK_MELEE;
						npc->ps.legsTimer=ent->npc->animTimes[ANPC_ATTACK_MELEE];
						npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_ATTACK_MELEE];
						if (ent->npc->flags & NPCF_WALKANDFLY && npc->inFlight)
							npc->fireTime=level.time+ent->npc->animTimes[ANPC_ATTACK_MELEE]*0.8;
						else
							npc->meleeTime=level.time+ent->npc->animTimes[ANPC_ATTACK_MELEE]*0.8;
					}
					else
					{
						if (ent->npc->npcType==NPC_METLAR)		// metlar jumps instead of firing
						{
							npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_ATTACK_FAR;
							npc->ps.legsTimer=ent->npc->animTimes[ANPC_ATTACK_FAR];
							npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_ATTACK_FAR]*0.8;
							npc->toFire=2;
							ucmd->upmove=ent->npc->jumpHeight*0.7;
							ucmd->forwardmove=127;
						}
						else
						{
							npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_ATTACK_FAR;
							npc->ps.legsTimer=ent->npc->animTimes[ANPC_ATTACK_FAR];
							npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_ATTACK_FAR];
							npc->fireTime=level.time+ent->npc->animTimes[ANPC_ATTACK_FAR]*((ent->npc->npcType==NPC_ANK)?0.8:(ent->npc->npcType==NPC_BAT?0.2:0.5));
							npc->fireCount=0;
						}
					}
				}
			}
			else
			{
				npc->toFire=0;
				npc->attackTime--;

//				G_Printf("att: %3d ",npc->attackTime);

				if ((CheckInObj(npc->obj,ent->r.currentOrigin) && ent->npc->npcType!=NPC_SEALORD) || npc->attackTime<=0)
				{
					float r;

					r=random();

					npc->attackTime=15+random()*10;
					// ANK or any walking NPC
					if (r>0.9 && npc->canStand && !npc->inFlight)	// do standing active animation?
					{
						int an;
						if (random()>0.3 && ent->npc->npcType==NPC_SEALORD)
						{
							an=ANPC_TAUNT;
							npc->quakeTime=level.time+SEALORD_QUAKE_TIME;
//							G_Printf("do: lv=%d qt=%d\n",level.time,npc->quakeTime);
						}
						else
							an=ANPC_STANDING_ACTIVE;

						npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | an;
						npc->ps.legsTimer=ent->npc->animTimes[an];
						npc->dontMoveTime=level.time+ent->npc->animTimes[an];
						do_move=0;
						npc->canStand=0;
//						G_Printf("STAND \n");
					}
					else if (
								(
									(
										r<npc->shot_factor
										&& ent->npc->npcType!=NPC_HULK
										&& ent->npc->npcType!=NPC_METLAR
									)
									||
									enemy_dist<=ent->npc->melee_dist ||
									(enemy_dist<=1.5*ent->npc->melee_dist && ent->npc->npcType==NPC_HULK && !in_earthquake) ||
									(r<0.1 && ent->npc->npcType==NPC_HULK && !in_earthquake) ||
									(r<0.75 && enemy_dist<=4*ent->npc->melee_dist && ent->npc->npcType==NPC_METLAR) ||
									(npc->goingBack && enemy_dist<ideal_dist+100)
								)
								&& (npc->ps.groundEntityNum!=ENTITYNUM_NONE || npc->inFlight) // only bat can fire on air
								&& enemy_visible
							)
							// may i attack?
					{
//						G_Printf("FIRE \n");
						NPC_FindTarget(ent);
						npc->toFire=1;
						do_move=0;
						npc->canStand=1;
						npc->goingBack=0;
					}
					else		 // get closer to enemy...
					{
						if (ent->npc->npcType==NPC_SEALORD)
						{
							npc->attackTime/=2;
							do_move=1;
						}
						else if (npc->inFlight)
						{
							if (enemy_visible)
							{
								VectorCopy(npc->enemy->r.currentOrigin,npc->real_obj);
							}
							else
							{
								NPC_FindTarget(ent);
							}
							do_move=1;
						}
						else
						{
							vec3_t *enemy_pos=&(npc->enemy->r.currentOrigin);
							int enemy_area=NPC_PointAreaNum(*enemy_pos);
							int my_area=NPC_PointAreaNum(ent->r.currentOrigin);
							aas_predictroute_t route;

							trap_AAS_PredictRoute(&route, my_area, ent->r.currentOrigin,
								enemy_area, NPC_TravelFlags(ent), 1, 50,
								RSE_USETRAVELTYPE,
								0, 0, 0);
							if (route.time>0 || enemy_area==my_area)	// only update enemy position if its reachable
							{
								VectorCopy(*enemy_pos,npc->real_obj);
	//							G_Printf("%d updated enemy position %s\n",level.time,vtos(*enemy_pos));
							}
							do_move=NPC_RecalcLinearObj(ent);
						}
						npc->canStand=0;
					}
				}

				if (do_move && npc->shot_factor<1)
				{
					npc->goingBack=0;

					if (ent->npc->npcType==NPC_SEALORD)
					{
//
// Warning fixup
//
						//vec3_t dist;
//
// end fixup
//
						int len, dif;

						// UGLY CODE, sealord can only move along the X axis

						NPC_FindTarget(ent);

						len=npc->initpos[0]-ent->r.currentOrigin[0];

						dif=ent->r.currentOrigin[0]-npc->enemy->r.currentOrigin[0];
						if (fabs(dif)>SEALORD_STEP/3)
						{
							if (len<SEALORD_MAX_MOV && dif>0)
							{
								if (dif<SEALORD_STEP)
									ucmd->rightmove=dif;
								else
									ucmd->rightmove=SEALORD_STEP;
							}
							else if (len>-SEALORD_MAX_MOV && dif<0)
							{
								if (dif>-SEALORD_STEP)
									ucmd->rightmove=dif;
								else
									ucmd->rightmove=-SEALORD_STEP;
							}
						}

/*						if (ucmd->rightmove!=0)
						{
							G_AddEvent(&g_entities[0], EV_EARTHQUAKE, 253);
						}
*/
//						G_Printf("rt: %d\n",(int)(ucmd->rightmove));
//						G_Printf("my_pos %s       enemy_pos %s\n",vtos(ent->r.currentOrigin),vtos(npc->enemy->r.currentOrigin));

					}
					else if (enemy_dist<ideal_dist	// decent distance?
						&& (enemy_dist>ent->npc->melee_dist*1.5 || ent->npc->melee_damage==0)
						&& enemy_visible)
					{
						if (enemy_dist<ideal_dist*0.75	// too near?
							&& (ent->npc->npcType==NPC_ANK || ent->npc->npcType==NPC_SOLDIER1	// only them can step back
							 || ent->npc->npcType==NPC_PILOT))
						{
							vec3_t dir,new_pos,up,right;

							ucmd->forwardmove=-ent->npc->runningSpeed*0.75;	// backwards is slower
							NPC_FindTarget(ent);
							npc->ideal_angle+=((((ent->s.number+(level.time/3000))%2)*2)-1)*25;

							new_pos[0]=0;
							new_pos[1]=npc->ideal_angle;
							new_pos[2]=0;
							AngleVectors(new_pos,dir,right,up);

							if (NPC_CheckForJump(ent,ucmd,NML_RUN,dir))
								NPC_FindTarget(ent);
						}
						npc->goingBack=1;
						npc->canStand=0;
					}
					else  if (ent->npc->npcType==NPC_METLAR)
					{
						if (npc->ps.groundEntityNum!=ENTITYNUM_NONE)
						{
							NPC_MoveLinear(ent,ucmd,NML_RUN);
							ucmd->upmove=ent->npc->jumpHeight;
						}
					}
					else if (npc->inFlight)
						NPC_MoveLinear(ent,ucmd,NML_FLY);
					else
						NPC_MoveLinear(ent,ucmd,NML_RUN);
				}
				else
					NPC_FindTarget(ent);
			}
		}
		break;
	case NPC_COM_SEARCH:
		{
			npc->attackTime--;
			if (npc->attackTime<=0 || CheckInObj(npc->obj,ent->r.currentOrigin))
			{
				double r=random();

				if (r>0.5 || ent->npc->walkingSpeed==0 || ent->npc->flags & NPCF_WALKANDFLY)	// look anywhere?
				{
					if (!npc->inFlight)
					{
						npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_STANDING_ACTIVE;
						npc->ps.legsTimer=ent->npc->animTimes[ANPC_STANDING_ACTIVE];
					}
					else
						npc->ideal_angle=(random()*360)-180;
					npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_STANDING_ACTIVE];
				}
				else			// go to a random place
				{
					VectorCopy(ent->r.currentOrigin,npc->real_obj);
					npc->real_obj[0]+=random()*1000-500;
					npc->real_obj[1]+=random()*1000-500;
					if (NPC_RecalcLinearObj(ent))
						npc->attackTime=random()*60+20;
				}
			}
			else if (npc->attackTime>0 && ent->npc->walkingSpeed>0 && !(ent->npc->flags & NPCF_WALKANDFLY))
				NPC_MoveLinear(ent,ucmd,NML_WALK);
		}
		break;
	}
}

void NPC_ThinkView(gentity_t *ent, usercmd_t *ucmd)
{
	npcData_t *npc=&ent->ns;
	int new_angle;

	if (npc->inFlight)
	{
		if (npc->enemy)
			NPC_FindTarget(ent);
		if (npc->altitude > npc->flyingAlt+10 || ent->health < ent->npc->health*0.10)
			ucmd->upmove=-10;
		else if (npc->altitude < npc->flyingAlt-10 && (ent->health > ent->npc->health*0.25))
			ucmd->upmove=5;
		else
			ucmd->upmove=0;
//		if (ent->npc->flags & NPCF_WALKANDFLY)
//			G_Printf("alt: %d npc->flyingAlt: %d   upmove: %d\n",npc->altitude,npc->flyingAlt,(int)ucmd->upmove);
	}
	else
	{
		if (npc->ps.groundEntityNum==ENTITYNUM_NONE)	// cannot turn around when jumping or falling
			return;
	}

	new_angle=NPC_ChangeViewAngle(npc->ps.viewangles[1],npc->ideal_angle,
		(ucmd->forwardmove>ent->npc->runningSpeed-5)?ent->npc->runningRotSpd:ent->npc->walkingRotSpd);

	ucmd->angles[1]=new_angle;
	npc->ps.viewangles[1]=new_angle;

	if (fabs(npc->ps.viewangles[1]-new_angle)>45)
	{
		ucmd->forwardmove=0;
		if (!npc->inFlight)
			ucmd->upmove=0;
	}
//	G_Printf("rt2: %d\n",(int)(ucmd->rightmove));
}

// JPL - end

#endif
