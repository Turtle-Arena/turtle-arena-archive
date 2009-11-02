// g_npc.c
//
// JPL (jpl@ing.puc.cl) - The Dark Conjunction - ACE Team
//

#include "g_local.h"
#include "../qcommon/q_shared.h"

#ifdef SP_NPC

#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"


// JPL - NPC - Non Playable Characters (monsters, maybe "helpers")

int npc_skill;

void SP_npcpath( gentity_t *self )
{
	if ( !self->targetname ) {
		G_Printf ("npcpath with no targetname at %s\n", vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}
}

void PainNPC(gentity_t *self, gentity_t *attacker, int damage)
{
	float NPCPainFreq = self->npc->painFreq;
	float factor;

	// soldiers and pilot don't fight themselves
	// I was attack and now I want to kill them,
	//  if its a NPC and they are my ally I should kill them
	//  if I already wanted to them them do nothing.
	if (attacker && !((attacker->s.eType == ET_NPC && (attacker->npc->flags & NPCF_ALLY) == (self->npc->flags & NPCF_ALLY))
		// The "good guys" shouldn't ever attack the players, add random?
		// They could call them names, or ask what there doing though.
		|| ((self->npc->flags & NPCF_ALLY) && (attacker->client)))
		&& self->ns.enemy != attacker)
	{
		self->ns.command=NPC_COM_ATTACK;
		self->ns.enemy=attacker;
		self->ns.attackTime=0;
		VectorCopy(self->ns.enemy->r.currentOrigin,self->ns.real_obj);
		VectorCopy(self->ns.enemy->r.currentOrigin,self->ns.obj);
	}

	self->ns.painAcum+=damage;

	factor=NPCPainFreq*(float)(self->ns.painAcum)/50*(float)(100-self->health)/50;

	if (random()<factor)		// do pain animation
	{
		int anim=ANPC_PAIN;

		if (self->npc->flags & NPCF_WALKANDFLY)
		{
			if (!self->ns.inFlight)
				anim=ANPC_BACKPEDAL;
			else
				self->ns.inFlight=0;			// do a forced landing
		}

		self->ns.ps.legsAnim=((self->ns.ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_PAIN;
		self->ns.ps.legsTimer=self->npc->animTimes[ANPC_PAIN];
		self->ns.dontMoveTime=level.time+self->npc->animTimes[ANPC_PAIN];
		self->ns.painAcum=0;
		self->ns.fireTime=0;
		self->ns.meleeTime=0;
	}
}

#define CORPSEWAIT 6500 // 6.5 sec wait before start of sinking
#define SINKTIME   3000 // 3 sec to sink

void NPCBodySink( gentity_t *ent ) {
	if ( level.time - ent->timestamp > (CORPSEWAIT + SINKTIME) ) {
		G_FreeEntity( ent );
		return;
	}
	ent->nextthink = level.time + 100;
	ent->s.pos.trBase[2] -= 1;
}

void DieNPC(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	int death;

	self->takedamage=qfalse;
	self->ns.state=NPC_ST_DEAD;

	if (self->npc->flags & NPCF_WALKANDFLY)
	{
		if (self->ns.inFlight)
			death=2;
		else
			death=rand()%2;
	}
	else
		death=rand()%3;

	self->s.modelindex2=ANPC_DEATH1+death;
	self->ns.ps.legsAnim=ANPC_DEATH1+death;

	self->r.maxs[2]=self->r.mins[2];
	self->r.contents=0;

	if ((self->npc->weapon > WP_NONE && self->npc->weapon < WP_NUM_WEAPONS)
		&& !(self->npc->flags & NPCF_NODROPWEAPON))
	{
		gitem_t *item = BG_FindItemForWeapon( self->npc->weapon );
		Drop_Item(self,item,45);
	}
	G_UseTargets(self,attacker);

	// Turtle Man: Death types
	switch (self->npc->deathType)
	{
		default:
		case NPCD_NONE:
			break; // Do nothing.
		case NPCD_SINK:
			self->nextthink = level.time + CORPSEWAIT;
			self->think = NPCBodySink;
			self->timestamp = level.time;
			self->physicsObject = qtrue;
			self->physicsBounce = 0;
			break;
		// Robots explode
		case NPCD_EXPLODE:
			break; // TODO
		// Ninjas dissapear in smoke?
		case NPCD_SMOKE:
			break; // TODO
	}
}

qboolean	npcRegistered[NPC_NUMNPCS];

void ClearRegisteredNPCs( void )
{
	memset( npcRegistered, 0, sizeof( npcRegistered ) );
}

void RegisterNPC( bgnpc_t *npc )
{
	char text[10000], *text_p, *token;
	fileHandle_t f;
	char filename[MAX_QPATH], npcname[MAX_NPCNAME];
	int len;
	int nf, fps, i;

	if ( !npc )
	{
		G_Error( "RegisterNPC: npc is NULL" );
	}

	if (npcRegistered[npc-bg_npclist])
		return;

	npcRegistered[ npc - bg_npclist ] = qtrue;

	// load OPTIONAL config file, it will overwrite the the NPC in bg_npclist.
	Com_sprintf(npcname,sizeof(npcname),"%s",npc->classname+4);
	Com_sprintf(filename,sizeof(filename),"models/npc/%s/config.txt",npcname);

	while (1) // This is so we can break from the loop.
	{
		len = trap_FS_FOpenFile( filename, &f, FS_READ );
		if ( len <= 0 )
		{
			// Turtle Man: Thoses "defaults" are per-NPC, no warning needed.
			//G_Printf(S_COLOR_YELLOW "Warngin: No config file found for %s , defaults will be used.\n",npcname);
			break;
		}
		if ( len >= sizeof( text ) - 1 )
		{
			G_Printf( S_COLOR_RED "Error: NPC config file is too long %s\n", filename );
			break;
		}
		trap_FS_Read( text, len, f );
		text[len] = 0;
		trap_FS_FCloseFile( f );

		text_p = text;

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->health = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->painFreq = atof( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->walkingSpeed = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->runningSpeed = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->fov = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->jumpHeight = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) goto error;
		npc->walkingRotSpd = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token )
		{
error:
			G_Printf(S_COLOR_RED "Error: Failed Parsing config file for NPC %s.\n",npcname);
			break;
		}
		npc->runningRotSpd = atoi( token );

		Parse1DMatrix (&text_p, 3, npc->mins);
		Parse1DMatrix (&text_p, 3, npc->maxs);
		Parse1DMatrix (&text_p, 3, npc->eye);

		npc->melee_dist=(abs(npc->maxs[0]-npc->mins[0])+abs(npc->maxs[1]-npc->mins[1]))*0.75;

		// Always exit the loop here.
		break;
	}

	// load animation times
	Com_sprintf(npcname,sizeof(npcname),"%s",npc->classname+4);
	Com_sprintf(filename,sizeof(filename),"models/npc/%s/animation.txt",npcname);

	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 )
	{
		G_Printf(S_COLOR_YELLOW "Warning: No NPC animation file found for %s, defaults will be used.\n",npcname);
		return;
	}
	if ( len >= sizeof( text ) - 1 )
	{
		G_Printf( "File %s too long\n", filename );
		return;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	text_p = text;

	for (i=0;i<MAX_ANIMATIONS_NPC;i++)
	{
		token = COM_Parse( &text_p );
		if ( !*token ) break;
		// start frame is unused!

		token = COM_Parse( &text_p );
		if ( !*token ) break;
		nf=atoi(token); // numframes

		token = COM_Parse( &text_p );
		if ( !*token ) break;
		// looping is unused!

		token = COM_Parse( &text_p );
		if ( !*token ) break;
		fps=atoi(token); // frames per second

		npc->animTimes[i]=1000*nf/fps;
	}

	// Check for error
	if (i != MAX_ANIMATIONS_NPC)
	{
		G_Printf(S_COLOR_RED "Error: Failed parsing animation file for NPC %s\n",npcname);
	}
}

void SaveRegisteredNPCs( void )
{
	char	string[NPC_NUMNPCS+1];
	int		i;
	int		count;

	count = 0;
	for ( i = 0 ; i < NPC_NUMNPCS ; i++ ) {
		if ( npcRegistered[i] ) {
			count++;
			string[i] = '1';
		} else {
			string[i] = '0';
		}
	}
	string[ i ] = 0;

	G_Printf( "%i NPCs registered\n", count );
	trap_SetConfigstring(CS_NPCS, string);
}

void FinishSpawningNPC( gentity_t *ent )
{
	trace_t		tr;
	vec3_t		dest;

	VectorCopy(ent->npc->mins,ent->r.mins);
	VectorCopy(ent->npc->maxs,ent->r.maxs);

	ent->s.eType = ET_NPC;
	ent->s.modelindex = ent->npc - bg_npclist;		// store NPC number in modelindex

	ent->r.contents = CONTENTS_BODY;

	if ( ent->spawnflags & 1 ) {
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		// drop to floor
		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
		if ( tr.startsolid ) {
			G_Printf ("FinishSpawningNPC: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
			G_FreeEntity( ent );
			return;
		}

		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;

		G_SetOrigin( ent, tr.endpos );
	}

	VectorCopy(  ent->s.pos.trBase,ent->ns.ps.origin );
	VectorCopy( ent->s.pos.trDelta,ent->ns.ps.velocity );

	VectorCopy( ent->s.apos.trBase,ent->ns.ps.viewangles );
	ent->ns.ideal_angle=ent->s.apos.trBase[1];
	ent->ns.state=NPC_ST_INACTIVE;
	ent->ns.command=NPC_COM_NONE;
	ent->health=ent->npc->health;
	if (npc_skill==-1)
		ent->health*=0.8;		// 20% weaker in easy level
	ent->takedamage=qtrue;
	ent->die=DieNPC;
	ent->pain=PainNPC;
	ent->ns.canStand=0;
	ent->ns.dontMoveTime=0;
	ent->ns.checkTime=0;
	ent->ns.painAcum=0;
	ent->ns.attackTime=0;
	ent->ns.meleeTime=0;
	ent->ns.fireTime=0;
	VectorCopy(ent->s.pos.trBase,ent->ns.initpos);
	ent->ns.ps.pm_time=0;
	ent->ns.ps.legsAnim=((ent->ns.ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_STANDING;
	ent->ns.ps.commandTime=level.time;
	ent->s.modelindex2=((ent->ns.ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_STANDING;

	if (ent->target)
	{
#ifdef TMNTPATHSYS
		if (G_SetupPath(ent, ent->target) != PATH_ERROR)
		{
			// Path is ok to use.
			ent->ns.command=NPC_COM_GOTO;
			VectorCopy(ent->nextTrain->s.pos.trBase,ent->ns.obj);
//			NPC_RecalcObj(ent);
		}
#else
		gentity_t		*path, *next, *start;
		//qboolean end_way = qfalse;

		ent->nextTrain = G_Find( NULL, FOFS(targetname), ent->target );
		if ( !ent->nextTrain ) {
			G_Printf( "npc with paths at %s with an unfound target\n",
				vtos(ent->r.absmin) );
			return;
		}

		start = NULL;
		for ( path = ent->nextTrain ; path != start && !end_way; path = next ) {
			if ( !start ) {
				start = path;
			}

			path->nextTrain = NULL;
			if ( !path->target ) {
				//end_way=qtrue;
			}
			next = NULL;
			do {
				next = G_Find( next, FOFS(targetname), path->target );
				if ( !next )
				{
					//end_way=qtrue;
					break;
				}
			} while ( strcmp( next->classname, "npcpath" ) );

			path->nextTrain = next;
		}
		ent->ns.command=NPC_COM_GOTO;
		VectorCopy(ent->nextTrain->s.pos.trBase,ent->ns.obj);
//		NPC_RecalcObj(ent);
#endif
	}

	trap_LinkEntity (ent);
}

void trig_FinishSpawningNPC( gentity_t *ent,gentity_t *a,gentity_t *b )
{
	FinishSpawningNPC(ent);
}

void SP_npc( gentity_t *ent, bgnpc_t *npc )
{
	float fa;

	// Turtle Man: NPCs are SP and Co-op only
	if (g_gametype.integer != GT_SINGLE_PLAYER)
	{
		return;
	}

	G_SpawnFloat("shot_factor","0.5",&ent->ns.shot_factor);

	G_SpawnFloat("flying_alt","100",&fa);
	ent->ns.flyingAlt=fa;

	RegisterNPC( npc );
	ent->npc = npc;

	if ( ent->spawnflags & 2 )
	{
		// disbled
		ent->use=trig_FinishSpawningNPC;
	}
	else
	{
		ent->nextthink = level.time + FRAMETIME * 2;
		ent->think = FinishSpawningNPC;
	}
	ent->s.clientNum = npc - bg_npclist;

	VectorCopy( ent->s.angles, ent->s.apos.trBase );
}


/*
================
G_RunNPC

================
*/

void G_RunNPC( gentity_t *ent )
{
	npmove_t	pm;
	usercmd_t	ucmd;
	npcData_t	*npc;
	int			oldEventSequence;


	G_RunThink( ent );
	if (ent->die!=DieNPC)
		return;

	npc=&ent->ns;
	if (npc->state==NPC_ST_DEAD)
	{
		if(ent->nextthink<level.time)
		{
			ent->think(ent);
			return;
		}
	}

	ucmd.serverTime=level.time;
	ucmd.angles[0]=ucmd.angles[1]=ucmd.angles[2]=0;
	ucmd.buttons=0;
#ifndef TMNTWEAPSYS2
	ucmd.weapon=0;
#endif
#ifdef TMNTHOLDSYS/*2*/
	ucmd.holdable = 0;
#endif
	ucmd.forwardmove=0;
	ucmd.rightmove=ucmd.upmove=0;

	if (npc->state!=NPC_ST_DEAD)
	{
		if (npc->quakeTime)
		{
			if (npc->quakeTime<=level.time)
			{
				//G_AddEvent(&g_entities[0], EV_EARTHQUAKE, 0xFF);
				if (g_entities[0].client->ps.groundEntityNum!=ENTITYNUM_NONE)
				{
					g_entities[0].slow_event=level.time+HULK_QUAKE_LEN;
				}
				npc->quakeTime=0;
			}
		}
		if (npc->meleeTime)
		{
			if (npc->meleeTime <= level.time)
			{
				npc->fireTime=0;
				/*if (ent->npc->npcType==NPC_SEALORD)		// slow and powerfull fireball
				{
					NPC_FireWeapon(ent,npc->enemy->r.currentOrigin);	// now inflict weapon damage
				}
				else*/
				{
					vec3_t dir,ang; // now inflict melee damage
					int len;

					VectorSubtract (npc->enemy->r.currentOrigin,ent->r.currentOrigin, dir);
					len=VectorNormalize(dir);
					vectoangles(dir,ang);
					if (ent->npc->npcType==NPC_HULK)
					{
						//G_AddEvent(&g_entities[0], EV_EARTHQUAKE, 0xFF);
						if (g_entities[0].client->ps.groundEntityNum!=ENTITYNUM_NONE)
						{
							g_entities[0].slow_event=level.time+HULK_QUAKE_LEN;
						}
					}
					if (NPC_InFieldOfVision(npc->ps.viewangles,120,ang) && len<=ent->npc->melee_dist*1.2)
					{
						G_Damage(npc->enemy,ent,ent,dir,npc->enemy->r.currentOrigin,ent->npc->melee_damage*(int)(1.0f+0.25f*(float)npc_skill),0,0);
					}
				}
				npc->meleeTime=0;
			}
		}
		else if (npc->fireTime)
		{
			NPC_FindTarget(ent);
			if (npc->fireTime <= level.time)
			{
				npc->meleeTime=0;
				/*if (ent->npc->npcType==NPC_HULK)
				{
					vec3_t dir,ang; // now inflict hulk far melee damage (bite)
					int len;

					VectorSubtract (npc->enemy->r.currentOrigin,ent->r.currentOrigin, dir);
					len=VectorNormalize(dir);
					vectoangles(dir,ang);
					if (NPC_InFieldOfVision(npc->ps.viewangles,90,ang) && len<=ent->npc->melee_dist*1.7)
					{
						G_Damage(npc->enemy,ent,ent,dir,npc->enemy->r.currentOrigin,ent->npc->melee_damage*(int)(1.0f+0.25f*(float)npc_skill)/2,0,0);
					}
				}
				else*/
				{
					NPC_FireWeapon(ent,npc->enemy->r.currentOrigin);	// now inflict weapon damage
				}

				/*if (ent->npc->npcType==NPC_SEALORD)
				{
					npc->fireCount++;
					if (npc->fireCount>=6)
					{
						npc->fireCount=0;
						npc->fireTime=0;
						npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_LAND;
						npc->ps.legsTimer=ent->npc->animTimes[ANPC_LAND];
						npc->dontMoveTime=level.time+ent->npc->animTimes[ANPC_LAND];
					}
					else
					{
						npc->ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_JUMP;
						npc->ps.legsTimer=500;
						npc->fireTime=level.time+500;
						npc->dontMoveTime=level.time+500;
					}
				}
				else*/
					npc->fireTime=0;
			}
		}
		else if (npc->dontMoveTime)	// NPC must not move
		{
			/*if (ent->npc->npcType==NPC_METLAR && npc->toFire==2)	// a jump attack?
			{
				vec3_t dir,ang;
				int len;

				VectorSubtract (npc->enemy->r.currentOrigin,ent->r.currentOrigin, dir);
				len=VectorNormalize(dir);
				vectoangles(dir,ang);
				if (NPC_InFieldOfVision(npc->ps.viewangles,120,ang) && len<=ent->npc->melee_dist*1.2)
				{
					G_Damage(npc->enemy,ent,ent,dir,npc->enemy->r.currentOrigin,ent->npc->melee_damage*(int)(1.0f+0.25f*(float)npc_skill),0,0);
					npc->toFire=0;
				}
			}*/
			if (npc->dontMoveTime <= level.time && npc->state!=NPC_ST_DEAD) // now, it can move
			{
				ent->ns.ps.legsAnim=((npc->ps.legsAnim & ANIM_TOGGLEBIT)^ANIM_TOGGLEBIT) | ANPC_STANDING;
				npc->dontMoveTime=0;
				if (npc->command==NPC_COM_ATTACK)
					npc->attackTime=0;
			}
		}
		else
		{
			NPC_ThinkMove(ent,&ucmd);
		}
		NPC_ThinkView(ent,&ucmd);
	}

	if ( ent->health <= 0 ) {
		npc->ps.pm_type = PM_DEAD;
	} else {
		npc->ps.pm_type = PM_NORMAL;
	}

	npc->ps.gravity = g_gravity.value;
	npc->ps.clientNum=ent->s.number;

	npc->ps.speed = g_speed.value;

	oldEventSequence = npc->ps.eventSequence;
	memset (&pm, 0, sizeof(pm));

	pm.ps = &npc->ps;
	pm.cmd = ucmd;

	if ( pm.ps->pm_type == PM_DEAD ) {
		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
		npc->inFlight=0;
	}
	else {
		pm.tracemask = MASK_PLAYERSOLID;
	}
	pm.trace = trap_Trace;
	pm.pointcontents = trap_PointContents;
	pm.debugLevel = g_debugMove.integer;
	pm.noFootsteps = ( g_dmflags.integer & DF_NO_FOOTSTEPS ) > 0;

	pm.pmove_fixed = pmove_fixed.integer;
	pm.pmove_msec = pmove_msec.integer;
	pm.npc=ent->npc;
	pm.ns=&ent->ns;

	VectorCopy (ent->r.mins, pm.mins);
	VectorCopy (ent->r.maxs, pm.maxs);

	NPmove (&pm);

	if ( npc->ps.eventSequence != oldEventSequence ) {
		ent->eventTime = level.time;
	}
	NPC_PlayerStateToEntityState( &npc->ps, &ent->s, qtrue );

	VectorCopy( ent->s.pos.trBase, ent->r.currentOrigin );

	ent->waterlevel = pm.waterlevel;
	ent->watertype = pm.watertype;

	trap_LinkEntity (ent);

	VectorCopy( npc->ps.origin, ent->r.currentOrigin );
}

// JPL - end

#endif
