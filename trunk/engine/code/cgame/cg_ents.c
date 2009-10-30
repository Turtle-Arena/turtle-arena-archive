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
// cg_ents.c -- present snapshot entities, happens every single frame

#include "cg_local.h"


/*
======================
CG_PositionEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void CG_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName ) {
	int				i;
	orientation_t	lerped;
	
	// lerp the tag
	trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( lerped.axis, ((refEntity_t *)parent)->axis, entity->axis );
	entity->backlerp = parent->backlerp;
}


/*
======================
CG_PositionRotatedEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void CG_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName ) {
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];

//AxisClear( entity->axis );
	// lerp the tag
	trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( entity->axis, lerped.axis, tempAxis );
	MatrixMultiply( tempAxis, ((refEntity_t *)parent)->axis, entity->axis );
}



/*
==========================================================================

FUNCTIONS CALLED EACH FRAME

==========================================================================
*/

/*
======================
CG_SetEntitySoundPosition

Also called by event processing code
======================
*/
void CG_SetEntitySoundPosition( centity_t *cent ) {
	if ( cent->currentState.solid == SOLID_BMODEL ) {
		vec3_t	origin;
		float	*v;

		v = cgs.inlineModelMidpoints[ cent->currentState.modelindex ];
		VectorAdd( cent->lerpOrigin, v, origin );
		trap_S_UpdateEntityPosition( cent->currentState.number, origin );
	} else {
		trap_S_UpdateEntityPosition( cent->currentState.number, cent->lerpOrigin );
	}
}

/*
==================
CG_EntityEffects

Add continuous entity effects, like local entity emission and lighting
==================
*/
static void CG_EntityEffects( centity_t *cent ) {

	// update sound origins
	CG_SetEntitySoundPosition( cent );

	// add loop sound
	if ( cent->currentState.loopSound ) {
		if (cent->currentState.eType != ET_SPEAKER) {
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, 
				cgs.gameSounds[ cent->currentState.loopSound ] );
		} else {
			trap_S_AddRealLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, 
				cgs.gameSounds[ cent->currentState.loopSound ] );
		}
	}


	// constant light glow
	if ( cent->currentState.constantLight ) {
		int		cl;
		int		i, r, g, b;

		cl = cent->currentState.constantLight;
		r = cl & 255;
		g = ( cl >> 8 ) & 255;
		b = ( cl >> 16 ) & 255;
		i = ( ( cl >> 24 ) & 255 ) * 4;
		trap_R_AddLightToScene( cent->lerpOrigin, i, r, g, b );
	}

}


/*
==================
CG_General
==================
*/
static void CG_General( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// if set to invisible, skip
	if (!s1->modelindex) {
		return;
	}

	memset (&ent, 0, sizeof(ent));

	// set frame

	ent.frame = s1->frame;
	ent.oldframe = ent.frame;
	ent.backlerp = 0;

	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	ent.hModel = cgs.gameModels[s1->modelindex];

	// player model
	if (s1->number == cg.snap->ps.clientNum) {
		ent.renderfx |= RF_THIRD_PERSON;	// only draw from mirrors
	}

	// convert angles to axis
	AnglesToAxis( cent->lerpAngles, ent.axis );

	// add to refresh list
	trap_R_AddRefEntityToScene (&ent);
}

#ifdef TMNTENTSYS // MISC_OBJECT
static void CG_MiscObjectAnimate( centity_t *cent, int *oldframe, int *frame, float *backlerp ) {
	entityState_t		*s1;

	s1 = &cent->currentState;

	// Unanimated misc_object
	if (cent->miscObj.anim == -1)
	{
		*frame = s1->frame;
		*oldframe = *frame;
		*backlerp = 0;
	}
	else
	{
		// Animated misc_object

		// Check for frame change
		if (s1->modelindex2 != cent->miscObj.anim)
		{
			cent->miscObj.anim = s1->modelindex2;

			// Do we need to clear it?
			BG_ClearLerpFrame( &cent->miscObj.lerp, cent->miscObj.animations, cent->miscObj.anim, cg.time);
		}

		BG_RunLerpFrame( &cent->miscObj.lerp, cent->miscObj.animations, cent->miscObj.anim, cg.time, cent->miscObj.speed );

		*frame = cent->miscObj.lerp.frame;
		*oldframe = cent->miscObj.lerp.oldFrame;
		*backlerp = cent->miscObj.lerp.backlerp;
	}
}

// TODO: Make this / G_SetFileExt be Com_SetFileExt ?
void CG_SetFileExt(char *filename, char *ext)
{
	int i;

	for (i = 0; filename[i] && filename[i] != '.'; i++)
	{
		// just increase i.
	}
	filename[i] = 0;

	Q_strcat(filename, MAX_QPATH, ext);
}

static void CG_MiscObject( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// if set to invisible, skip
	if (!s1->modelindex) {
		return;
	}

	// Check if config was loaded
	if (cent->miscObj.speed < 1.0)
	{
		const char *modelName;
		char filename[MAX_QPATH];

		const char *misc_object_anim_names[MAX_MISC_OBJECT_ANIMATIONS] =
		{
			"OBJECT_NORMAL",
			"OBJECT_DAMAGED1",
			"OBJECT_DAMAGED2",
			"OBJECT_DAMAGED3",
			"OBJECT_KILLED"
		};

		cent->miscObj.speed = 1.0f;

		modelName = CG_ConfigString( CS_MODELS + s1->modelindex );

		Q_strncpyz(filename, modelName, MAX_QPATH);

		CG_SetFileExt(filename, ".cfg");

		if (BG_ParseObjectCFGFile(filename, misc_object_anim_names,
			cent->miscObj.animations, MAX_MISC_OBJECT_ANIMATIONS,
			NULL, NULL, NULL, NULL, &cent->miscObj.speed))
		{
			// Loaded file.
			cent->miscObj.anim = 0;
			BG_ClearLerpFrame( &cent->miscObj.lerp, cent->miscObj.animations, cent->miscObj.anim, cg.time);
		}
		else
		{
			cent->miscObj.anim = -1;
		}
	}

	memset (&ent, 0, sizeof(ent));

	// set frame
	CG_MiscObjectAnimate(cent, &ent.oldframe, &ent.frame, &ent.backlerp);

	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	ent.hModel = cgs.gameModels[s1->modelindex];

	// Flags for only drawing or not drawing a object in mirrors
	if (cent->miscObj.flags & MOF_ONLY_MIRROR)
	{
		ent.renderfx |= RF_ONLY_MIRROR;
	}
	else if (cent->miscObj.flags & MOF_NOT_MIRROR)
	{
		ent.renderfx |= RF_NOT_MIRROR;
	}

	// convert angles to axis
	AnglesToAxis( cent->lerpAngles, ent.axis );

	// add to refresh list
	trap_R_AddRefEntityToScene (&ent);
}
#endif

#ifdef SINGLEPLAYER // entity
static qboolean CG_ParseMD3AnimationFile( const char *filename, animation_t *anim ) {
	char		fname[MAX_QPATH];
	int			len;
	char		*token;
	float		fps;
	char		text[10000], *text_p;
	fileHandle_t	f;

	strcpy(fname,filename);
	text_p=fname+strlen(fname);
	while (*text_p!='.' && *text_p!='\\' && text_p!=fname)
		text_p--;
	if (*text_p=='.')
		*text_p=0;
	Q_strcat(fname,sizeof(fname),".anim");
	// load the file
	len = trap_FS_FOpenFile( fname, &f, FS_READ );
	if ( len <= 0 ) {
		CG_Printf( "File %s too short (exists?)\n", fname );
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		CG_Printf( "File %s too long\n", fname );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );
	// parse the text
	text_p=text;
	token = COM_Parse( &text_p );
	if ( !*token ) {
		CG_Printf("%s%s\n",fname);
		return qfalse;
	}
	anim->firstFrame = atoi( token );
		token = COM_Parse( &text_p );
	if ( !*token ) {
		CG_Printf("%s%s\n",fname);
		return qfalse;
	}
	anim->numFrames = atoi( token );
		anim->reversed = qfalse;
	anim->flipflop = qfalse;
	// if numFrames is negative the animation is reversed
	if (anim->numFrames < 0) {
		anim->numFrames = -anim->numFrames;
		anim->reversed = qtrue;
	}
		token = COM_Parse( &text_p );
	if ( !*token ) {
		CG_Printf("%s%s\n",fname);
		return qfalse;
	}
	anim->loopFrames = atoi( token );
		token = COM_Parse( &text_p );
	if ( !*token ) {
		CG_Printf("%s%s\n",fname);
		return qfalse;
	}
	fps = atof( token );
	if ( fps == 0 ) {
		fps = 1;
	}
	anim->frameLerp = 1000 / fps;
	anim->initialLerp = 1000 / fps;
	return qtrue;
}

static void CG_ModelAnimate( centity_t *cent, int *md3Old, int *md3, float *md3BackLerp ) {
	entityState_t		*s1;
	lerpFrame_t 		*model;

	s1 = &cent->currentState;
	model=&cent->md3.model;

	if (cent->currentState.eFlags & EF_FORCE_END_FRAME ) {
		cent->md3.state &= ~2;
		cent->md3.state &= ~4;
	}
	if (s1->modelindex2!=cent->md3.lastCode)	// are we being triggered from the server?
	{
		cent->md3.lastCode=s1->modelindex2;
		if (!(cent->md3.state & 2))	// is it animating?
		{
			cent->md3.state |= 4;	// show model
			cent->md3.state |= 2;	// if not, activate animation
			cent->md3.state ^= ANIM_TOGGLEBIT;
			BG_ClearLerpFrame(model, &cent->md3.anim, cent->md3.state & ANIM_TOGGLEBIT, cg.time);
		}
	}
	if (cent->currentState.eFlags & EF_FORCE_END_FRAME ) {
		model->frame = model->animation->firstFrame; // + model->animation->numFrames - 1;
		model->oldFrame = model->frame;
		model->backlerp = 0;
		cent->md3.state &= ~2;
	}
	if (cent->md3.state & 2)	// do we have to animate?
	{
		BG_RunLerpFrame( model, &cent->md3.anim, cent->md3.state & ANIM_TOGGLEBIT, cg.time, cent->md3.speed );
		if (model->oldFrame==model->frame &&
			model->frame==model->animation->numFrames+model->animation->firstFrame-1)	// finished?
		{
			cent->md3.state &= ~2;			// stop animation processing
			if (s1->generic1 & 2)		// dissapear if HIDDEN_END is turned on
				cent->md3.state &= ~4;
		}
	}
	*md3Old = model->oldFrame;
	*md3 = model->frame;
	*md3BackLerp = model->backlerp;
}

/*
==================
CG_ModelAnim
==================
*/
static void CG_ModelAnim( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;
	s1 = &cent->currentState;
	// if set to invisible, skip
	if (!s1->modelindex) {
		return;
	}
	if (!(cent->md3.state & 1))  // was animation config loaded?
	{
		const char *modelName;
		modelName = CG_ConfigString( CS_MODELS+cg_entities[s1->number].currentState.modelindex );
		if (!CG_ParseMD3AnimationFile(modelName,&cent->md3.anim))
		{
			// for debugging
			cent->md3.anim.firstFrame=0;
			cent->md3.anim.flipflop=0;
			cent->md3.anim.frameLerp=100;
			cent->md3.anim.initialLerp=100;
			cent->md3.anim.loopFrames=10;
			cent->md3.anim.numFrames=10;
			cent->md3.anim.reversed=0;
		}
		cent->md3.state |= 1;
		if (!(s1->generic1 & 1))	// show if HIDDEN_START is turned off
			cent->md3.state|=4;

		cent->md3.speed = (float)(s1->generic1 & 0xF0) / 16;
	}
	memset (&ent, 0, sizeof(ent));
	CG_ModelAnimate(cent,&ent.oldframe,&ent.frame,&ent.backlerp);
	if (!(cent->md3.state & 4))		// do nothing if visible flag is not set
		return;
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);
	ent.hModel = cgs.gameModels[s1->modelindex];
	// convert angles to axis
	AnglesToAxis( cent->lerpAngles, ent.axis );
	// add to refresh list
	trap_R_AddRefEntityToScene (&ent);
}
#endif

/*
==================
CG_Speaker

Speaker entities can automatically play sounds
==================
*/
static void CG_Speaker( centity_t *cent ) {
	if ( ! cent->currentState.clientNum ) {	// FIXME: use something other than clientNum...
		return;		// not auto triggering
	}

	if ( cg.time < cent->miscTime ) {
		return;
	}

	trap_S_StartSound (NULL, cent->currentState.number, CHAN_ITEM, cgs.gameSounds[cent->currentState.eventParm] );

	//	ent->s.frame = ent->wait * 10;
	//	ent->s.clientNum = ent->random * 10;
	cent->miscTime = cg.time + cent->currentState.frame * 100 + cent->currentState.clientNum * 100 * crandom();
}

/*
==================
CG_Item
==================
*/
static void CG_Item( centity_t *cent ) {
	refEntity_t		ent;
	entityState_t	*es;
	gitem_t			*item;
	int				msec;
	float			frac;
	float			scale;
	weaponInfo_t	*wi;

	es = &cent->currentState;
	if ( es->modelindex >= bg_numItems ) {
		CG_Error( "Bad item index %i on entity", es->modelindex );
	}

	// if set to invisible, skip
	if ( !es->modelindex || ( es->eFlags & EF_NODRAW ) ) {
		return;
	}

	item = &bg_itemlist[ es->modelindex ];
#ifdef IOQ3ZTM // If missing item model, use item sprite.
	// Turtle Man: Not all of the items have models yet, so use the sprite!
	if ( (!cg_items[es->modelindex].models[0] || cg_simpleItems.integer) && item->giType != IT_TEAM )
#else
	if ( cg_simpleItems.integer && item->giType != IT_TEAM )
#endif
	{
		memset( &ent, 0, sizeof( ent ) );
		ent.reType = RT_SPRITE;
		VectorCopy( cent->lerpOrigin, ent.origin );
		ent.radius = 14;
		ent.customShader = cg_items[es->modelindex].icon;
		ent.shaderRGBA[0] = 255;
		ent.shaderRGBA[1] = 255;
		ent.shaderRGBA[2] = 255;
		ent.shaderRGBA[3] = 255;
		trap_R_AddRefEntityToScene(&ent);
		return;
	}

	// items bob up and down continuously
	scale = 0.005 + cent->currentState.number * 0.00001;
#ifdef TMNT // CRATE
	if (item->giType != IT_CRATE)
#endif
	cent->lerpOrigin[2] += 4 + cos( ( cg.time + 1000 ) *  scale ) * 4;

	memset (&ent, 0, sizeof(ent));

#ifdef TMNT // CRATE
	if (item->giType == IT_CRATE)
		; // Don't rotate automaticly.
	else
#endif
	// autorotate at one of two speeds
	// shurikenAngles
	if ( item->giType == IT_HEALTH ) {
		VectorCopy( cg.autoAnglesFast, cent->lerpAngles );
		AxisCopy( cg.autoAxisFast, ent.axis );
	} else {
		VectorCopy( cg.autoAngles, cent->lerpAngles );
		AxisCopy( cg.autoAxis, ent.axis );
	}

	wi = NULL;
	// the weapons have their origin where they attatch to player
	// models, so we need to offset them or they will rotate
	// eccentricly
	if ( item->giType == IT_WEAPON ) {
		wi = &cg_weapons[item->giTag];
		cent->lerpOrigin[0] -= 
			wi->weaponMidpoint[0] * ent.axis[0][0] +
			wi->weaponMidpoint[1] * ent.axis[1][0] +
			wi->weaponMidpoint[2] * ent.axis[2][0];
		cent->lerpOrigin[1] -= 
			wi->weaponMidpoint[0] * ent.axis[0][1] +
			wi->weaponMidpoint[1] * ent.axis[1][1] +
			wi->weaponMidpoint[2] * ent.axis[2][1];
		cent->lerpOrigin[2] -= 
			wi->weaponMidpoint[0] * ent.axis[0][2] +
			wi->weaponMidpoint[1] * ent.axis[1][2] +
			wi->weaponMidpoint[2] * ent.axis[2][2];

		cent->lerpOrigin[2] += 8;	// an extra height boost
	}

	ent.hModel = cg_items[es->modelindex].models[0];

	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	ent.nonNormalizedAxes = qfalse;

	// if just respawned, slowly scale up
	msec = cg.time - cent->miscTime;
	if ( msec >= 0 && msec < ITEM_SCALEUP_TIME ) {
		frac = (float)msec / ITEM_SCALEUP_TIME;
		VectorScale( ent.axis[0], frac, ent.axis[0] );
		VectorScale( ent.axis[1], frac, ent.axis[1] );
		VectorScale( ent.axis[2], frac, ent.axis[2] );
		ent.nonNormalizedAxes = qtrue;
	} else {
		frac = 1.0;
	}

	// items without glow textures need to keep a minimum light value
	// so they are always visible
#ifdef TMNTDATASYS
	ent.renderfx |= RF_MINLIGHT;
#else
	if ( ( item->giType == IT_WEAPON ) ||
		 ( item->giType == IT_ARMOR ) ) {
		ent.renderfx |= RF_MINLIGHT;
	}
#endif

	// increase the size of the weapons when they are presented as items
	if ( item->giType == IT_WEAPON ) {
		VectorScale( ent.axis[0], 1.5, ent.axis[0] );
		VectorScale( ent.axis[1], 1.5, ent.axis[1] );
		VectorScale( ent.axis[2], 1.5, ent.axis[2] );
		ent.nonNormalizedAxes = qtrue;
#ifdef MISSIONPACK
		trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.media.weaponHoverSound );
#endif
	}

#if defined MISSIONPACK && !defined TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	if ( item->giType == IT_HOLDABLE && item->giTag == HI_KAMIKAZE ) {
		VectorScale( ent.axis[0], 2, ent.axis[0] );
		VectorScale( ent.axis[1], 2, ent.axis[1] );
		VectorScale( ent.axis[2], 2, ent.axis[2] );
		ent.nonNormalizedAxes = qtrue;
	}
#endif

	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);

#ifdef MISSIONPACK
	if ( item->giType == IT_WEAPON && wi->barrelModel ) {
		refEntity_t	barrel;

		memset( &barrel, 0, sizeof( barrel ) );

		barrel.hModel = wi->barrelModel;

		VectorCopy( ent.lightingOrigin, barrel.lightingOrigin );
		barrel.shadowPlane = ent.shadowPlane;
		barrel.renderfx = ent.renderfx;

		CG_PositionRotatedEntityOnTag( &barrel, &ent, wi->weaponModel, "tag_barrel" );

		AxisCopy( ent.axis, barrel.axis );
		barrel.nonNormalizedAxes = ent.nonNormalizedAxes;

		trap_R_AddRefEntityToScene( &barrel );
	}
#endif

#ifndef TMNTDATA // no extra models for items
	// accompanying rings / spheres for powerups
	if ( !cg_simpleItems.integer ) 
	{
		vec3_t spinAngles;

		VectorClear( spinAngles );

		if ( item->giType == IT_HEALTH || item->giType == IT_POWERUP )
		{
			if ( ( ent.hModel = cg_items[es->modelindex].models[1] ) != 0 )
			{
				if ( item->giType == IT_POWERUP )
				{
					ent.origin[2] += 12;
					spinAngles[1] = ( cg.time & 1023 ) * 360 / -1024.0f;
				}
				AnglesToAxis( spinAngles, ent.axis );
				
				// scale up if respawning
				if ( frac != 1.0 ) {
					VectorScale( ent.axis[0], frac, ent.axis[0] );
					VectorScale( ent.axis[1], frac, ent.axis[1] );
					VectorScale( ent.axis[2], frac, ent.axis[2] );
					ent.nonNormalizedAxes = qtrue;
				}
				trap_R_AddRefEntityToScene( &ent );
			}
		}
	}
#endif
}

//============================================================================

#ifdef TMNTHOLDABLE
/*
===============
RotateAroundDirection
===============
*/
/*
void RotateAroundDirection3( vec3_t axis[3], vec3_t angles ) {
	vec3_t	temp;

	// create an arbitrary axis[1]
	PerpendicularVector( axis[1], axis[0] );

#if 0
	// rotate it around axis[0] by pitch
	if ( angles[0] ) {
		//vec3_t	temp;

		VectorCopy( axis[0], temp );
		RotatePointAroundVector( axis[0], axis[0], temp, angles[0] );
	}
#else
	// rotate it around axis[1] by pitch
	if ( angles[0] ) {
		//vec3_t	temp;

		VectorCopy( axis[0], temp );
		RotatePointAroundVector( axis[0], axis[1], temp, angles[0] );
	}
#endif

	// cross to get axis[2]
	CrossProduct( axis[1], axis[0], axis[2] );

	// rotate it around axis[0] by yaw
	if ( angles[1] ) {
		//vec3_t	temp;

		VectorCopy( axis[1], temp );
		RotatePointAroundVector( axis[1], axis[0], temp, angles[1] );
	}

	// cross to get axis[2]
	CrossProduct( axis[0], axis[1], axis[2] );

	// rotate it around axis[0] by roll
	if ( angles[2] ) {
		//vec3_t	temp;

		VectorCopy( axis[2], temp );
		RotatePointAroundVector( axis[2], axis[0], temp, angles[2] );
	}
}*/

float vectoyaw( const vec3_t vec ) {
	float	yaw;

	if (vec[YAW] == 0 && vec[PITCH] == 0) {
		yaw = 0;
	} else {
		if (vec[PITCH]) {
			yaw = ( atan2( vec[YAW], vec[PITCH]) * 180 / M_PI );
		} else if (vec[YAW] > 0) {
			yaw = 90;
		} else {
			yaw = 270;
		}
		if (yaw < 0) {
			yaw += 360;
		}
	}

	return yaw;
}


/*
===============
CG_Shuriken
Based on CG_MISSILE

Turtle Man: FIXME: Finish rotating.
===============
*/
static void CG_Shuriken( centity_t *cent, holdable_t holdable ) {
	refEntity_t			ent;
	entityState_t		*s1;
	vec3_t dir;

/*
	// add trails
	if ( weapon->missileTrailFunc )
	{
		weapon->missileTrailFunc( cent, weapon );
	}

	// add dynamic light
	if ( weapon->missileDlight ) {
		trap_R_AddLightToScene(cent->lerpOrigin, weapon->missileDlight,
			weapon->missileDlightColor[0], weapon->missileDlightColor[1], weapon->missileDlightColor[2] );
	}

	// add missile sound
	if ( weapon->missileSound ) {
		vec3_t	velocity;

		BG_EvaluateTrajectoryDelta( &cent->currentState.pos, cg.time, velocity );

		trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, velocity, weapon->missileSound );
	}
*/

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));

	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	// flicker between two skins
	ent.skinNum = cg.clientFrame & 1;

	switch (holdable)
	{
		case HI_SHURIKEN:
			ent.hModel = cgs.media.shurikenModel;
			break;
		case HI_FIRESHURIKEN:
			ent.hModel = cgs.media.shurikenFireModel;
			break;
		case HI_ELECTRICSHURIKEN:
			ent.hModel = cgs.media.shurikenElectricModel;
			break;
		case HI_LASERSHURIKEN:
			ent.hModel = cgs.media.shurikenLaserModel;
			break;
		default:
			ent.hModel = 0;
			break;
	}
	ent.renderfx = RF_NOSHADOW;

	// calculate the axis
	// convert direction of travel into axis
	if ( VectorNormalize2( s1->pos.trDelta, ent.axis[0] ) == 0 ) {
		ent.axis[0][2] = 1;
	}

	ent.nonNormalizedAxes = qfalse;

	//vectoangles( ent.axis[0], cent->lerpAngles );
#if 1
	VectorSubtract(s1->pos.trDelta, s1->pos.trBase, dir);
	cent->lerpAngles[0] = cg.shurikenAngles[0];
	cent->lerpAngles[1] = vectoyaw(dir)+90;//s1->angles[1];
	cent->lerpAngles[2] = 0;//s1->angles[2];
	AnglesToAxis( cent->lerpAngles, ent.axis );
#else
	//VectorCopy( s1->angles, cent->lerpAngles);
	VectorCopy( cg.shurikenAngles, cent->lerpAngles );
	AxisCopy( cg.shurikenAxis, ent.axis );
#endif

	// spin as it moves
	if ( s1->pos.trType != TR_STATIONARY ) {
		//RotateAroundDirection( ent.axis, cg.time / 4 );
		//RotateAroundDirection( ent.axis, cent->lerpAngles[1]);
	} else {
		// Turtle Man: TODO: Stuck in wall?
	}

	// add to refresh list, possibly with quad glow
	CG_AddRefEntityWithPowerups( &ent, s1, TEAM_FREE );
}
#endif

/*
===============
CG_Missile
===============
*/
static void CG_Missile( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;
	const weaponInfo_t		*weapon;
//	int	col;
#ifdef TMNTHOLDABLE
	int holdable;

	s1 = &cent->currentState;
	if ( s1->weapon >= WP_NUM_WEAPONS ) {
		holdable = s1->weapon - WP_NUM_WEAPONS;
		switch (holdable)
		{
			case HI_SHURIKEN:
			case HI_FIRESHURIKEN:
			case HI_ELECTRICSHURIKEN:
			case HI_LASERSHURIKEN:
				CG_Shuriken(cent, holdable);
				return;
			default:
				s1->weapon = 0;
		}
	}
#else
	s1 = &cent->currentState;
#ifdef IOQ3ZTM // IOQ3BUGFIX: Invalid weapon get run.
	if ( s1->weapon >= WP_NUM_WEAPONS ) {
#else
	if ( s1->weapon > WP_NUM_WEAPONS ) {
#endif
		s1->weapon = 0;
	}
#endif
	weapon = &cg_weapons[s1->weapon];

	// calculate the axis
	VectorCopy( s1->angles, cent->lerpAngles);

	// add trails
	if ( weapon->missileTrailFunc ) 
	{
		weapon->missileTrailFunc( cent, weapon );
	}
/*
	if ( cent->currentState.modelindex == TEAM_RED ) {
		col = 1;
	}
	else if ( cent->currentState.modelindex == TEAM_BLUE ) {
		col = 2;
	}
	else {
		col = 0;
	}

	// add dynamic light
	if ( weapon->missileDlight ) {
		trap_R_AddLightToScene(cent->lerpOrigin, weapon->missileDlight, 
			weapon->missileDlightColor[col][0], weapon->missileDlightColor[col][1], weapon->missileDlightColor[col][2] );
	}
*/
	// add dynamic light
	if ( weapon->missileDlight ) {
		trap_R_AddLightToScene(cent->lerpOrigin, weapon->missileDlight, 
			weapon->missileDlightColor[0], weapon->missileDlightColor[1], weapon->missileDlightColor[2] );
	}

	// add missile sound
	if ( weapon->missileSound ) {
		vec3_t	velocity;

		BG_EvaluateTrajectoryDelta( &cent->currentState.pos, cg.time, velocity );

		trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, velocity, weapon->missileSound );
	}

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

#ifdef TMNTWEAPONS
	if ( cent->currentState.weapon == WP_ELECTRIC_LAUNCHER ) {
#else
	if ( cent->currentState.weapon == WP_PLASMAGUN ) {
#endif
		ent.reType = RT_SPRITE;
		ent.radius = 16;
		ent.rotation = 0;
		ent.customShader = cgs.media.plasmaBallShader;
		trap_R_AddRefEntityToScene( &ent );
		return;
	}

	// flicker between two skins
	ent.skinNum = cg.clientFrame & 1;
	ent.hModel = weapon->missileModel;
	ent.renderfx = weapon->missileRenderfx | RF_NOSHADOW;

#if defined MISSIONPACK && !defined TMNTWEAPONS
	if ( cent->currentState.weapon == WP_PROX_LAUNCHER ) {
		if (s1->generic1 == TEAM_BLUE) {
			ent.hModel = cgs.media.blueProxMine;
		}
	}
#endif

	// convert direction of travel into axis
	if ( VectorNormalize2( s1->pos.trDelta, ent.axis[0] ) == 0 ) {
		ent.axis[0][2] = 1;
	}

	// spin as it moves
	if ( s1->pos.trType != TR_STATIONARY ) {
		RotateAroundDirection( ent.axis, cg.time / 4 );
	} else {
#if defined MISSIONPACK && !defined TMNTWEAPONS
		if ( s1->weapon == WP_PROX_LAUNCHER ) {
			AnglesToAxis( cent->lerpAngles, ent.axis );
		}
		else
#endif
		{
			RotateAroundDirection( ent.axis, s1->time );
		}
	}

	// add to refresh list, possibly with quad glow
	CG_AddRefEntityWithPowerups( &ent, s1, TEAM_FREE );
}

/*
===============
CG_Grapple

This is called when the grapple is sitting up against the wall
===============
*/
static void CG_Grapple( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;
	const weaponInfo_t		*weapon;

	s1 = &cent->currentState;
	if ( s1->weapon > WP_NUM_WEAPONS ) {
		s1->weapon = 0;
	}
	weapon = &cg_weapons[s1->weapon];

	// calculate the axis
	VectorCopy( s1->angles, cent->lerpAngles);

#if 0 // FIXME add grapple pull sound here..?
	// add missile sound
	if ( weapon->missileSound ) {
		trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->missileSound );
	}
#endif

	// Will draw cable if needed
	CG_GrappleTrail ( cent, weapon );

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	// flicker between two skins
	ent.skinNum = cg.clientFrame & 1;
	ent.hModel = weapon->missileModel;
	ent.renderfx = weapon->missileRenderfx | RF_NOSHADOW;

	// convert direction of travel into axis
	if ( VectorNormalize2( s1->pos.trDelta, ent.axis[0] ) == 0 ) {
		ent.axis[0][2] = 1;
	}

	trap_R_AddRefEntityToScene( &ent );
}

/*
===============
CG_Mover
===============
*/
static void CG_Mover( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);
	AnglesToAxis( cent->lerpAngles, ent.axis );

	ent.renderfx = RF_NOSHADOW;

	// flicker between two skins (FIXME?)
	ent.skinNum = ( cg.time >> 6 ) & 1;

	// get the model, either as a bmodel or a modelindex
	if ( s1->solid == SOLID_BMODEL ) {
		ent.hModel = cgs.inlineDrawModel[s1->modelindex];
	} else {
		ent.hModel = cgs.gameModels[s1->modelindex];
	}

	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);

	// add the secondary model
	if ( s1->modelindex2 ) {
		ent.skinNum = 0;
		ent.hModel = cgs.gameModels[s1->modelindex2];
		trap_R_AddRefEntityToScene(&ent);
	}

}

/*
===============
CG_Beam

Also called as an event
===============
*/
void CG_Beam( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( s1->pos.trBase, ent.origin );
	VectorCopy( s1->origin2, ent.oldorigin );
	AxisClear( ent.axis );
	ent.reType = RT_BEAM;

	ent.renderfx = RF_NOSHADOW;

	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);
}


/*
===============
CG_Portal
===============
*/
static void CG_Portal( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin );
	VectorCopy( s1->origin2, ent.oldorigin );
	ByteToDir( s1->eventParm, ent.axis[0] );
	PerpendicularVector( ent.axis[1], ent.axis[0] );

	// negating this tends to get the directions like they want
	// we really should have a camera roll value
	VectorSubtract( vec3_origin, ent.axis[1], ent.axis[1] );

	CrossProduct( ent.axis[0], ent.axis[1], ent.axis[2] );
	ent.reType = RT_PORTALSURFACE;
	ent.oldframe = s1->powerups;
	ent.frame = s1->frame;		// rotation speed
	ent.skinNum = s1->clientNum/256.0 * 360;	// roll offset

	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);
}


/*
=========================
CG_AdjustPositionForMover

Also called by client movement prediction code
=========================
*/
void CG_AdjustPositionForMover( const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out ) {
	centity_t	*cent;
	vec3_t	oldOrigin, origin, deltaOrigin;
	vec3_t	oldAngles, angles, deltaAngles;

	if ( moverNum <= 0 || moverNum >= ENTITYNUM_MAX_NORMAL ) {
		VectorCopy( in, out );
		return;
	}

	cent = &cg_entities[ moverNum ];
	if ( cent->currentState.eType != ET_MOVER ) {
		VectorCopy( in, out );
		return;
	}

	BG_EvaluateTrajectory( &cent->currentState.pos, fromTime, oldOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, fromTime, oldAngles );

	BG_EvaluateTrajectory( &cent->currentState.pos, toTime, origin );
	BG_EvaluateTrajectory( &cent->currentState.apos, toTime, angles );

	VectorSubtract( origin, oldOrigin, deltaOrigin );
	VectorSubtract( angles, oldAngles, deltaAngles );

	VectorAdd( in, deltaOrigin, out );

	// FIXME: origin change when on a rotating object
}


/*
=============================
CG_InterpolateEntityPosition
=============================
*/
static void CG_InterpolateEntityPosition( centity_t *cent ) {
	vec3_t		current, next;
	float		f;

	// it would be an internal error to find an entity that interpolates without
	// a snapshot ahead of the current one
	if ( cg.nextSnap == NULL ) {
		CG_Error( "CG_InterpoateEntityPosition: cg.nextSnap == NULL" );
	}

	f = cg.frameInterpolation;

	// this will linearize a sine or parabolic curve, but it is important
	// to not extrapolate player positions if more recent data is available
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, current );
	BG_EvaluateTrajectory( &cent->nextState.pos, cg.nextSnap->serverTime, next );

	cent->lerpOrigin[0] = current[0] + f * ( next[0] - current[0] );
	cent->lerpOrigin[1] = current[1] + f * ( next[1] - current[1] );
	cent->lerpOrigin[2] = current[2] + f * ( next[2] - current[2] );

	BG_EvaluateTrajectory( &cent->currentState.apos, cg.snap->serverTime, current );
	BG_EvaluateTrajectory( &cent->nextState.apos, cg.nextSnap->serverTime, next );

	cent->lerpAngles[0] = LerpAngle( current[0], next[0], f );
	cent->lerpAngles[1] = LerpAngle( current[1], next[1], f );
	cent->lerpAngles[2] = LerpAngle( current[2], next[2], f );

}

/*
===============
CG_CalcEntityLerpPositions

===============
*/
static void CG_CalcEntityLerpPositions( centity_t *cent ) {

	// if this player does not want to see extrapolated players
	if ( !cg_smoothClients.integer ) {
		// make sure the clients use TR_INTERPOLATE
		if ( cent->currentState.number < MAX_CLIENTS ) {
			cent->currentState.pos.trType = TR_INTERPOLATE;
			cent->nextState.pos.trType = TR_INTERPOLATE;
		}
	}

	if ( cent->interpolate && cent->currentState.pos.trType == TR_INTERPOLATE ) {
		CG_InterpolateEntityPosition( cent );
		return;
	}

	// first see if we can interpolate between two snaps for
	// linear extrapolated clients
	if ( cent->interpolate && cent->currentState.pos.trType == TR_LINEAR_STOP &&
											cent->currentState.number < MAX_CLIENTS) {
		CG_InterpolateEntityPosition( cent );
		return;
	}

	// just use the current frame and evaluate as best we can
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.time, cent->lerpOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, cg.time, cent->lerpAngles );

	// adjust for riding a mover if it wasn't rolled into the predicted
	// player state
	if ( cent != &cg.predictedPlayerEntity ) {
		CG_AdjustPositionForMover( cent->lerpOrigin, cent->currentState.groundEntityNum, 
		cg.snap->serverTime, cg.time, cent->lerpOrigin );
	}
}

/*
===============
CG_TeamBase
===============
*/
static void CG_TeamBase( centity_t *cent ) {
	refEntity_t model;
#ifdef MISSIONPACK
	vec3_t angles;
	int t, h;
	float c;

	if ( cgs.gametype == GT_CTF || cgs.gametype == GT_1FCTF ) {
#else
	if ( cgs.gametype == GT_CTF) {
#endif
		// show the flag base
		memset(&model, 0, sizeof(model));
		model.reType = RT_MODEL;
		VectorCopy( cent->lerpOrigin, model.lightingOrigin );
		VectorCopy( cent->lerpOrigin, model.origin );
		AnglesToAxis( cent->currentState.angles, model.axis );
		if ( cent->currentState.modelindex == TEAM_RED ) {
			model.hModel = cgs.media.redFlagBaseModel;
		}
		else if ( cent->currentState.modelindex == TEAM_BLUE ) {
			model.hModel = cgs.media.blueFlagBaseModel;
		}
		else {
			model.hModel = cgs.media.neutralFlagBaseModel;
		}
		trap_R_AddRefEntityToScene( &model );
	}
#ifdef MISSIONPACK
	else if ( cgs.gametype == GT_OBELISK ) {
		// show the obelisk
		memset(&model, 0, sizeof(model));
		model.reType = RT_MODEL;
		VectorCopy( cent->lerpOrigin, model.lightingOrigin );
		VectorCopy( cent->lerpOrigin, model.origin );
		AnglesToAxis( cent->currentState.angles, model.axis );

		model.hModel = cgs.media.overloadBaseModel;
		trap_R_AddRefEntityToScene( &model );
		// if hit
		if ( cent->currentState.frame == 1) {
			// show hit model
			// modelindex2 is the health value of the obelisk
			c = cent->currentState.modelindex2;
			model.shaderRGBA[0] = 0xff;
			model.shaderRGBA[1] = c;
			model.shaderRGBA[2] = c;
			model.shaderRGBA[3] = 0xff;
			//
			model.hModel = cgs.media.overloadEnergyModel;
			trap_R_AddRefEntityToScene( &model );
		}
		// if respawning
		if ( cent->currentState.frame == 2) {
			if ( !cent->miscTime ) {
				cent->miscTime = cg.time;
			}
			t = cg.time - cent->miscTime;
			h = (cg_obeliskRespawnDelay.integer - 5) * 1000;
			//
			if (t > h) {
				c = (float) (t - h) / h;
				if (c > 1)
					c = 1;
			}
			else {
				c = 0;
			}
			// show the lights
			AnglesToAxis( cent->currentState.angles, model.axis );
			//
			model.shaderRGBA[0] = c * 0xff;
			model.shaderRGBA[1] = c * 0xff;
			model.shaderRGBA[2] = c * 0xff;
			model.shaderRGBA[3] = c * 0xff;

			model.hModel = cgs.media.overloadLightsModel;
			trap_R_AddRefEntityToScene( &model );
			// show the target
			if (t > h) {
				if ( !cent->muzzleFlashTime ) {
					trap_S_StartSound (cent->lerpOrigin, ENTITYNUM_NONE, CHAN_BODY,  cgs.media.obeliskRespawnSound);
					cent->muzzleFlashTime = 1;
				}
				VectorCopy(cent->currentState.angles, angles);
				angles[YAW] += (float) 16 * acos(1-c) * 180 / M_PI;
				AnglesToAxis( angles, model.axis );

				VectorScale( model.axis[0], c, model.axis[0]);
				VectorScale( model.axis[1], c, model.axis[1]);
				VectorScale( model.axis[2], c, model.axis[2]);

				model.shaderRGBA[0] = 0xff;
				model.shaderRGBA[1] = 0xff;
				model.shaderRGBA[2] = 0xff;
				model.shaderRGBA[3] = 0xff;
				//
				model.origin[2] += 56;
				model.hModel = cgs.media.overloadTargetModel;
				trap_R_AddRefEntityToScene( &model );
			}
			else {
				//FIXME: show animated smoke
			}
		}
		else {
			cent->miscTime = 0;
			cent->muzzleFlashTime = 0;
			// modelindex2 is the health value of the obelisk
			c = cent->currentState.modelindex2;
			model.shaderRGBA[0] = 0xff;
			model.shaderRGBA[1] = c;
			model.shaderRGBA[2] = c;
			model.shaderRGBA[3] = 0xff;
			// show the lights
			model.hModel = cgs.media.overloadLightsModel;
			trap_R_AddRefEntityToScene( &model );
			// show the target
			model.origin[2] += 56;
			model.hModel = cgs.media.overloadTargetModel;
			trap_R_AddRefEntityToScene( &model );
		}
	}
#ifdef MISSIONPACK_HARVESTER
	else if ( cgs.gametype == GT_HARVESTER ) {
		// show harvester model
		memset(&model, 0, sizeof(model));
		model.reType = RT_MODEL;
		VectorCopy( cent->lerpOrigin, model.lightingOrigin );
		VectorCopy( cent->lerpOrigin, model.origin );
		AnglesToAxis( cent->currentState.angles, model.axis );

		if ( cent->currentState.modelindex == TEAM_RED ) {
			model.hModel = cgs.media.harvesterModel;
			model.customSkin = cgs.media.harvesterRedSkin;
		}
		else if ( cent->currentState.modelindex == TEAM_BLUE ) {
			model.hModel = cgs.media.harvesterModel;
			model.customSkin = cgs.media.harvesterBlueSkin;
		}
		else {
			model.hModel = cgs.media.harvesterNeutralModel;
			model.customSkin = 0;
		}
		trap_R_AddRefEntityToScene( &model );
	}
#endif
#endif
}

/*
===============
CG_AddCEntity

===============
*/
static void CG_AddCEntity( centity_t *cent ) {
	// event-only entities will have been dealt with already
	if ( cent->currentState.eType >= ET_EVENTS ) {
		return;
	}

	// calculate the current origin
	CG_CalcEntityLerpPositions( cent );

	// add automatic effects
	CG_EntityEffects( cent );

	switch ( cent->currentState.eType ) {
	default:
		CG_Error( "Bad entity type: %i\n", cent->currentState.eType );
		break;
	case ET_INVISIBLE:
	case ET_PUSH_TRIGGER:
	case ET_TELEPORT_TRIGGER:
		break;
	case ET_GENERAL:
		CG_General( cent );
		break;
	case ET_PLAYER:
		CG_Player( cent );
		break;
	case ET_ITEM:
		CG_Item( cent );
		break;
	case ET_MISSILE:
		CG_Missile( cent );
		break;
	case ET_MOVER:
		CG_Mover( cent );
		break;
	case ET_BEAM:
		CG_Beam( cent );
		break;
	case ET_PORTAL:
		CG_Portal( cent );
		break;
	case ET_SPEAKER:
		CG_Speaker( cent );
		break;
	case ET_GRAPPLE:
		CG_Grapple( cent );
		break;
	case ET_TEAM:
		CG_TeamBase( cent );
		break;
#ifdef SP_NPC
	case ET_NPC:
		CG_NPC( cent );
		break;
#endif
#ifdef SINGLEPLAYER // entity
	case ET_MODELANIM:
		CG_ModelAnim( cent );
		break;
#endif
#ifdef TMNTENTSYS // MISC_OBJECT
	case ET_MISCOBJECT:
		CG_MiscObject( cent );
		break;
#endif
	}
}

/*
===============
CG_AddPacketEntities

===============
*/
void CG_AddPacketEntities( void ) {
	int					num;
	centity_t			*cent;
	playerState_t		*ps;

	// set cg.frameInterpolation
	if ( cg.nextSnap ) {
		int		delta;

		delta = (cg.nextSnap->serverTime - cg.snap->serverTime);
		if ( delta == 0 ) {
			cg.frameInterpolation = 0;
		} else {
			cg.frameInterpolation = (float)( cg.time - cg.snap->serverTime ) / delta;
		}
	} else {
		cg.frameInterpolation = 0;	// actually, it should never be used, because 
									// no entities should be marked as interpolating
	}

	// the auto-rotating items will all have the same axis
	cg.autoAngles[0] = 0;
	cg.autoAngles[1] = ( cg.time & 2047 ) * 360 / 2048.0;
	cg.autoAngles[2] = 0;

	cg.autoAnglesFast[0] = 0;
	cg.autoAnglesFast[1] = ( cg.time & 1023 ) * 360 / 1024.0f;
	cg.autoAnglesFast[2] = 0;

	AnglesToAxis( cg.autoAngles, cg.autoAxis );
	AnglesToAxis( cg.autoAnglesFast, cg.autoAxisFast );

#ifdef TMNTHOLDABLE
	cg.shurikenAngles[0] = ( cg.time & 2047 ) * 360 / 2048.0;
	cg.shurikenAngles[1] = 0;
	cg.shurikenAngles[2] = 0;

	AnglesToAxis( cg.shurikenAngles, cg.shurikenAxis );
#endif

	// generate and add the entity from the playerstate
	ps = &cg.predictedPlayerState;
	BG_PlayerStateToEntityState( ps, &cg.predictedPlayerEntity.currentState, qfalse );
	CG_AddCEntity( &cg.predictedPlayerEntity );

	// lerp the non-predicted value for lightning gun origins
	CG_CalcEntityLerpPositions( &cg_entities[ cg.snap->ps.clientNum ] );

	// add each entity sent over by the server
	for ( num = 0 ; num < cg.snap->numEntities ; num++ ) {
		cent = &cg_entities[ cg.snap->entities[ num ].number ];
		CG_AddCEntity( cent );
	}
}

