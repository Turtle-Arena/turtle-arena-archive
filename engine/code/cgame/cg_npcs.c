// JPL (jpl@ing.puc.cl) - ACE Team - The Dark Conjunction
//
// cg_npcs.c -- handle the media and animation for NPC entities

#include "cg_local.h"

#ifdef SP_NPC
/*
======================
CG_ParseAnimationNPCFile

Read a configuration file containing animation counts and rates
======================
*/
static qboolean	CG_ParseAnimationNPCFile( const char *filename, npcInfo_t *npc ) {
	int			len;
	int			i;
	char		*token;
	float		fps;
	char		text[10000], *text_p;
	fileHandle_t	f;
	animation_t *animations;

	animations = npc->animations;

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		CG_Printf( "File %s too long\n", filename );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;

	// read information for each frame
	for ( i = 0 ; i < MAX_ANIMATIONS_NPC ; i++ ) {

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		animations[i].firstFrame = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		animations[i].numFrames = atoi( token );

		animations[i].reversed = qfalse;
		animations[i].flipflop = qfalse;
		// if numFrames is negative the animation is reversed
		if (animations[i].numFrames < 0) {
			animations[i].numFrames = -animations[i].numFrames;
			animations[i].reversed = qtrue;
		}

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		animations[i].loopFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		fps = atof( token );
		if ( fps == 0 ) {
			fps = 1;
		}
		animations[i].frameLerp = 1000 / fps;
		animations[i].initialLerp = 1000 / fps;
	}

	if ( i != MAX_ANIMATIONS_NPC ) {
		CG_Printf( "Error parsing animation file: %s", filename );
		return qfalse;
	}

	return qtrue;
}

/*
======================
CG_ParseSoundsNPCFile

Read a configuration file containing sonuds for animations
======================
*/
static qboolean	CG_ParseSoundsNPCFile( const char *filename, npcInfo_t *npc ) {
	int			len;
	char		*token;
	char		text[10000],*text_p;
	fileHandle_t	f;
	npcSounds_t *sounds;
	int anim,pos,cnt;

	sounds = npc->sounds;

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		CG_Printf( "File %s too long\n", filename );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p=text;

	while (1)
	{
		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		if (atoi(token)==-1) return qtrue;

		anim=atoi(token)-1;
		if (anim<0 || anim>=MAX_ANIMATIONS_NPC)
			return qfalse;

		cnt=sounds[anim].count;
		if (cnt<MAX_SOUNDS_NPC)
			cnt++;
		pos=cnt-1;

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}

		sounds[anim].frame[pos]=atoi(token);

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}

		sounds[anim].sound[pos]=trap_S_RegisterSound( token, qfalse );

		sounds[anim].count=cnt;
	}

	CG_Printf( "Error parsing sounds file: %s", filename );
	return qfalse;
}

/*
=================
CG_RegisterNPCVisuals

The server says this NPC is used on this level
=================
*/
void CG_RegisterNPCVisuals( int npcNum ) {
	npcInfo_t		*npcInfo;
	bgnpc_t			*npc;
	char	*modelname;
	char	filename[MAX_QPATH];

	npcInfo = &cg_npcs[ npcNum ];
	if ( npcInfo->registered ) {
		return;
	}

	npc = &bg_npclist[ npcNum ];

	memset( npcInfo, 0, sizeof( &npcInfo ) );
	npcInfo->registered = qtrue;

	modelname=npc->classname+4;

	Com_sprintf( filename, sizeof( filename ), "models/npc/%s/%s.md3", modelname, modelname);
	npcInfo->model = trap_R_RegisterModel( filename );

	// load the animations
	Com_sprintf( filename, sizeof( filename ), "models/npc/%s/animation.txt", modelname);
	if ( !CG_ParseAnimationNPCFile( filename, npcInfo ) ) {
		Com_Printf( "Failed to load animation file %s\n", filename );
	}

	// load the animations sounds
	Com_sprintf( filename, sizeof( filename ), "models/npc/%s/sounds.txt", modelname);
	if ( !CG_ParseSoundsNPCFile( filename, npcInfo ) ) {
		Com_Printf( "Failed to load sound file %s\n", filename );
	}
}

static void CG_NPCAnimation( centity_t *cent, int *bodyOld, int *body, float *bodyBackLerp ) {

	npcInfo_t		*ni;
	int				npcNum;
	entityState_t	*es;
	int anim;

	es = &cent->currentState;

//	float			speedScale;

	npcNum = cent->currentState.clientNum;

	ni = &cg_npcs[ npcNum ];


//  BG_ClearLerpFrame is not yet used!!


	BG_RunLerpFrame( &cent->ne.body, &ni->animations[0], es->modelindex2, cg.time, 1.0 );

	anim=es->modelindex2 & ~ANIM_TOGGLEBIT;
	if (ni->sounds[anim].count)
	{
		int s=cent->ne.sound;

		if (s<0 && -s!=cent->ne.body.frame)
		{
			s=rand()%ni->sounds[anim].count;
//			CG_Printf("%d: select sound %d\n",cg.time,s);
			cent->ne.sound=s;
		}
		else if (s>=0)
		{
			s %= ni->sounds[anim].count;
			if (cent->ne.body.frame==ni->animations[anim].firstFrame+ni->sounds[anim].frame[s])
			{
				trap_S_StartSound(NULL,es->number,CHAN_BODY,ni->sounds[anim].sound[s]);
//				CG_Printf("%d: sonar %d\n",cg.time,s);
				cent->ne.sound=-cent->ne.body.frame;
			}
		}
	}
//
// changes for load/save
//
	if (cent->currentState.eFlags & EF_FORCE_END_FRAME) {
		cent->ne.body.frame = cent->ne.body.animation->firstFrame + cent->ne.body.animation->numFrames - 1;
		cent->ne.body.oldFrame = cent->ne.body.frame;
		cent->ne.body.backlerp = 0;
	}
//
// end changes
//
	*bodyOld = cent->ne.body.oldFrame;
	*body = cent->ne.body.frame;
	*bodyBackLerp = cent->ne.body.backlerp;
}

/*
==================
CG_NPC
==================
*/
void CG_NPC( centity_t *cent ) {
	refEntity_t		body;
	refEntity_t	gun;
	entityState_t	*es;
	bgnpc_t			*npc;
	weaponInfo_t	*weapon;

	es = &cent->currentState;
	if ( es->modelindex >= NPC_NUMNPCS ) {
		CG_Error( "Bad NPC index %i on entity", es->modelindex );
	}

	// if set to invisible, skip
	if (  es->eFlags & EF_NODRAW ) {
		return;
	}

	npc = &bg_npclist[ es->modelindex ];

	memset (&body, 0, sizeof(body));

	CG_NPCAnimation(cent,&body.oldframe,&body.frame,&body.backlerp);

	body.hModel = cg_npcs[es->modelindex].model;

	AnglesToAxis( cent->lerpAngles, body.axis );
	VectorCopy( cent->lerpOrigin, body.origin);
	VectorCopy( cent->lerpOrigin, body.oldorigin);

	body.nonNormalizedAxes = qfalse;

	// Add to refresh list
	trap_R_AddRefEntityToScene(&body);

	// Add the weapon.
	if ((npc->weapon > WP_NONE && npc->weapon < WP_NUM_WEAPONS)
		// the first three animations (0,1,2) are DEATH!
		&& (cent->currentState.modelindex2 & ~ANIM_TOGGLEBIT)>2)
	{
		weapon = &cg_weapons[npc->weapon];
		memset( &gun, 0, sizeof( gun ) );
		VectorCopy( body.lightingOrigin, gun.lightingOrigin );
		gun.shadowPlane = body.shadowPlane;
		gun.renderfx = body.renderfx;
		gun.hModel = weapon->weaponModel;
		if (!gun.hModel)
			return;
		CG_PositionEntityOnTag( &gun, &body, body.hModel, "tag_weapon");

		trap_R_AddRefEntityToScene(&gun);
	}
}
#endif
