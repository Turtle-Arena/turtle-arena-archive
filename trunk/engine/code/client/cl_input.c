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
// cl.input.c  -- builds an intended movement command to send to the server

#include "client.h"

unsigned	frame_msec;
int			old_com_frameTime;

/*
===============================================================================

KEY BUTTONS

Continuous button event tracking is complicated by the fact that two different
input sources (say, mouse button 1 and the control key) can both press the
same button, but the button should only be released when both of the
pressing key have been released.

When a key event issues a button command (+forward, +attack, etc), it appends
its key number as argv(1) so it can be matched up with the release.

argv(2) will be set to the time the event happened, which allows exact
control even at low framerates when the down and up events may both get qued
at the same time.

===============================================================================
*/


typedef struct
{
	kbutton_t	in_left, in_right, in_forward, in_back;
	kbutton_t	in_lookup, in_lookdown, in_moveleft, in_moveright;
#ifdef TURTLEARENA // NO_SPEED_KEY
	kbutton_t	in_strafe;
#else
	kbutton_t	in_strafe, in_speed;
#endif
	kbutton_t	in_up, in_down;

#ifdef TURTLEARENA // LOCKON
	kbutton_t	in_lockon;
#endif

	kbutton_t	in_buttons[16];

#ifdef TA_PATHSYS // 2DMODE
	int pathMode;
#endif

	// NOTE: in_mlooking should be moved here if multiple mice are supported.
} clientInput_t;

#ifdef TA_SPLITVIEW // CONTROLS
clientInput_t cis[MAX_SPLITVIEW];
#else
clientInput_t cis[1];
#endif


#ifdef USE_VOIP
kbutton_t	in_voiprecord;
#endif

qboolean	in_mlooking;


void IN_MLookDown( void ) {
	in_mlooking = qtrue;
}

void IN_MLookUp( void ) {
	in_mlooking = qfalse;
	if ( !cl_freelook->integer ) {
		IN_CenterView ();
	}
}

void IN_KeyDown( kbutton_t *b ) {
	int		k;
	char	*c;
	
	c = Cmd_Argv(1);
	if ( c[0] ) {
		k = atoi(c);
	} else {
		k = -1;		// typed manually at the console for continuous down
	}

	if ( k == b->down[0] || k == b->down[1] ) {
		return;		// repeating key
	}
	
	if ( !b->down[0] ) {
		b->down[0] = k;
	} else if ( !b->down[1] ) {
		b->down[1] = k;
	} else {
		Com_Printf ("Three keys down for a button!\n");
		return;
	}
	
	if ( b->active ) {
		return;		// still down
	}

	// save timestamp for partial frame summing
	c = Cmd_Argv(2);
	b->downtime = atoi(c);

	b->active = qtrue;
	b->wasPressed = qtrue;
}

void IN_KeyUp( kbutton_t *b ) {
	int		k;
	char	*c;
	unsigned	uptime;

	c = Cmd_Argv(1);
	if ( c[0] ) {
		k = atoi(c);
	} else {
		// typed manually at the console, assume for unsticking, so clear all
		b->down[0] = b->down[1] = 0;
		b->active = qfalse;
		return;
	}

	if ( b->down[0] == k ) {
		b->down[0] = 0;
	} else if ( b->down[1] == k ) {
		b->down[1] = 0;
	} else {
		return;		// key up without coresponding down (menu pass through)
	}
	if ( b->down[0] || b->down[1] ) {
		return;		// some other key is still holding it down
	}

	b->active = qfalse;

	// save timestamp for partial frame summing
	c = Cmd_Argv(2);
	uptime = atoi(c);
	if ( uptime ) {
		b->msec += uptime - b->downtime;
	} else {
		b->msec += frame_msec / 2;
	}

	b->active = qfalse;
}



/*
===============
CL_KeyState

Returns the fraction of the frame that the key was down
===============
*/
float CL_KeyState( kbutton_t *key ) {
	float		val;
	int			msec;

	msec = key->msec;
	key->msec = 0;

	if ( key->active ) {
		// still down
		if ( !key->downtime ) {
			msec = com_frameTime;
		} else {
			msec += com_frameTime - key->downtime;
		}
		key->downtime = com_frameTime;
	}

#if 0
	if (msec) {
		Com_Printf ("%i ", msec);
	}
#endif

	val = (float)msec / frame_msec;
	if ( val < 0 ) {
		val = 0;
	}
	if ( val > 1 ) {
		val = 1;
	}

	return val;
}



void IN_UpDown(void) {IN_KeyDown(&cis[0].in_up);}
void IN_UpUp(void) {IN_KeyUp(&cis[0].in_up);}
void IN_DownDown(void) {IN_KeyDown(&cis[0].in_down);}
void IN_DownUp(void) {IN_KeyUp(&cis[0].in_down);}
void IN_LeftDown(void) {IN_KeyDown(&cis[0].in_left);}
void IN_LeftUp(void) {IN_KeyUp(&cis[0].in_left);}
void IN_RightDown(void) {IN_KeyDown(&cis[0].in_right);}
void IN_RightUp(void) {IN_KeyUp(&cis[0].in_right);}
void IN_ForwardDown(void) {IN_KeyDown(&cis[0].in_forward);}
void IN_ForwardUp(void) {IN_KeyUp(&cis[0].in_forward);}
void IN_BackDown(void) {IN_KeyDown(&cis[0].in_back);}
void IN_BackUp(void) {IN_KeyUp(&cis[0].in_back);}
void IN_LookupDown(void) {IN_KeyDown(&cis[0].in_lookup);}
void IN_LookupUp(void) {IN_KeyUp(&cis[0].in_lookup);}
void IN_LookdownDown(void) {IN_KeyDown(&cis[0].in_lookdown);}
void IN_LookdownUp(void) {IN_KeyUp(&cis[0].in_lookdown);}
void IN_MoveleftDown(void) {IN_KeyDown(&cis[0].in_moveleft);}
void IN_MoveleftUp(void) {IN_KeyUp(&cis[0].in_moveleft);}
void IN_MoverightDown(void) {IN_KeyDown(&cis[0].in_moveright);}
void IN_MoverightUp(void) {IN_KeyUp(&cis[0].in_moveright);}

#ifndef TURTLEARENA // NO_SPEED_KEY
void IN_SpeedDown(void) {IN_KeyDown(&cis[0].in_speed);}
void IN_SpeedUp(void) {IN_KeyUp(&cis[0].in_speed);}
#endif
void IN_StrafeDown(void) {IN_KeyDown(&cis[0].in_strafe);}
void IN_StrafeUp(void) {IN_KeyUp(&cis[0].in_strafe);}
#ifdef TURTLEARENA // LOCKON
void IN_LockonDown(void) {IN_KeyDown(&cis[0].in_lockon);IN_KeyDown(&cis[0].in_strafe);}
void IN_LockonUp(void) {IN_KeyUp(&cis[0].in_lockon);IN_KeyUp(&cis[0].in_strafe);}
#endif

#ifdef TA_SPLITVIEW
void IN_2UpDown(void) {IN_KeyDown(&cis[1].in_up);}
void IN_2UpUp(void) {IN_KeyUp(&cis[1].in_up);}
void IN_2DownDown(void) {IN_KeyDown(&cis[1].in_down);}
void IN_2DownUp(void) {IN_KeyUp(&cis[1].in_down);}
void IN_2LeftDown(void) {IN_KeyDown(&cis[1].in_left);}
void IN_2LeftUp(void) {IN_KeyUp(&cis[1].in_left);}
void IN_2RightDown(void) {IN_KeyDown(&cis[1].in_right);}
void IN_2RightUp(void) {IN_KeyUp(&cis[1].in_right);}
void IN_2ForwardDown(void) {IN_KeyDown(&cis[1].in_forward);}
void IN_2ForwardUp(void) {IN_KeyUp(&cis[1].in_forward);}
void IN_2BackDown(void) {IN_KeyDown(&cis[1].in_back);}
void IN_2BackUp(void) {IN_KeyUp(&cis[1].in_back);}
void IN_2LookupDown(void) {IN_KeyDown(&cis[1].in_lookup);}
void IN_2LookupUp(void) {IN_KeyUp(&cis[1].in_lookup);}
void IN_2LookdownDown(void) {IN_KeyDown(&cis[1].in_lookdown);}
void IN_2LookdownUp(void) {IN_KeyUp(&cis[1].in_lookdown);}
void IN_2MoveleftDown(void) {IN_KeyDown(&cis[1].in_moveleft);}
void IN_2MoveleftUp(void) {IN_KeyUp(&cis[1].in_moveleft);}
void IN_2MoverightDown(void) {IN_KeyDown(&cis[1].in_moveright);}
void IN_2MoverightUp(void) {IN_KeyUp(&cis[1].in_moveright);}

#ifndef TURTLEARENA // NO_SPEED_KEY
void IN_2SpeedDown(void) {IN_KeyDown(&cis[1].in_speed);}
void IN_2SpeedUp(void) {IN_KeyUp(&cis[1].in_speed);}
#endif
void IN_2StrafeDown(void) {IN_KeyDown(&cis[1].in_strafe);}
void IN_2StrafeUp(void) {IN_KeyUp(&cis[1].in_strafe);}
#ifdef TURTLEARENA // LOCKON
void IN_2LockonDown(void) {IN_KeyDown(&cis[1].in_lockon);IN_KeyDown(&cis[1].in_strafe);}
void IN_2LockonUp(void) {IN_KeyUp(&cis[1].in_lockon);IN_KeyUp(&cis[1].in_strafe);}
#endif

void IN_3UpDown(void) {IN_KeyDown(&cis[2].in_up);}
void IN_3UpUp(void) {IN_KeyUp(&cis[2].in_up);}
void IN_3DownDown(void) {IN_KeyDown(&cis[2].in_down);}
void IN_3DownUp(void) {IN_KeyUp(&cis[2].in_down);}
void IN_3LeftDown(void) {IN_KeyDown(&cis[2].in_left);}
void IN_3LeftUp(void) {IN_KeyUp(&cis[2].in_left);}
void IN_3RightDown(void) {IN_KeyDown(&cis[2].in_right);}
void IN_3RightUp(void) {IN_KeyUp(&cis[2].in_right);}
void IN_3ForwardDown(void) {IN_KeyDown(&cis[2].in_forward);}
void IN_3ForwardUp(void) {IN_KeyUp(&cis[2].in_forward);}
void IN_3BackDown(void) {IN_KeyDown(&cis[2].in_back);}
void IN_3BackUp(void) {IN_KeyUp(&cis[2].in_back);}
void IN_3LookupDown(void) {IN_KeyDown(&cis[2].in_lookup);}
void IN_3LookupUp(void) {IN_KeyUp(&cis[2].in_lookup);}
void IN_3LookdownDown(void) {IN_KeyDown(&cis[2].in_lookdown);}
void IN_3LookdownUp(void) {IN_KeyUp(&cis[2].in_lookdown);}
void IN_3MoveleftDown(void) {IN_KeyDown(&cis[2].in_moveleft);}
void IN_3MoveleftUp(void) {IN_KeyUp(&cis[2].in_moveleft);}
void IN_3MoverightDown(void) {IN_KeyDown(&cis[2].in_moveright);}
void IN_3MoverightUp(void) {IN_KeyUp(&cis[2].in_moveright);}

#ifndef TURTLEARENA // NO_SPEED_KEY
void IN_3SpeedDown(void) {IN_KeyDown(&cis[2].in_speed);}
void IN_3SpeedUp(void) {IN_KeyUp(&cis[2].in_speed);}
#endif
void IN_3StrafeDown(void) {IN_KeyDown(&cis[2].in_strafe);}
void IN_3StrafeUp(void) {IN_KeyUp(&cis[2].in_strafe);}
#ifdef TURTLEARENA // LOCKON
void IN_3LockonDown(void) {IN_KeyDown(&cis[2].in_lockon);IN_KeyDown(&cis[2].in_strafe);}
void IN_3LockonUp(void) {IN_KeyUp(&cis[2].in_lockon);IN_KeyUp(&cis[2].in_strafe);}
#endif

void IN_4UpDown(void) {IN_KeyDown(&cis[3].in_up);}
void IN_4UpUp(void) {IN_KeyUp(&cis[3].in_up);}
void IN_4DownDown(void) {IN_KeyDown(&cis[3].in_down);}
void IN_4DownUp(void) {IN_KeyUp(&cis[3].in_down);}
void IN_4LeftDown(void) {IN_KeyDown(&cis[3].in_left);}
void IN_4LeftUp(void) {IN_KeyUp(&cis[3].in_left);}
void IN_4RightDown(void) {IN_KeyDown(&cis[3].in_right);}
void IN_4RightUp(void) {IN_KeyUp(&cis[3].in_right);}
void IN_4ForwardDown(void) {IN_KeyDown(&cis[3].in_forward);}
void IN_4ForwardUp(void) {IN_KeyUp(&cis[3].in_forward);}
void IN_4BackDown(void) {IN_KeyDown(&cis[3].in_back);}
void IN_4BackUp(void) {IN_KeyUp(&cis[3].in_back);}
void IN_4LookupDown(void) {IN_KeyDown(&cis[3].in_lookup);}
void IN_4LookupUp(void) {IN_KeyUp(&cis[3].in_lookup);}
void IN_4LookdownDown(void) {IN_KeyDown(&cis[3].in_lookdown);}
void IN_4LookdownUp(void) {IN_KeyUp(&cis[3].in_lookdown);}
void IN_4MoveleftDown(void) {IN_KeyDown(&cis[3].in_moveleft);}
void IN_4MoveleftUp(void) {IN_KeyUp(&cis[3].in_moveleft);}
void IN_4MoverightDown(void) {IN_KeyDown(&cis[3].in_moveright);}
void IN_4MoverightUp(void) {IN_KeyUp(&cis[3].in_moveright);}

#ifndef TURTLEARENA // NO_SPEED_KEY
void IN_4SpeedDown(void) {IN_KeyDown(&cis[3].in_speed);}
void IN_4SpeedUp(void) {IN_KeyUp(&cis[3].in_speed);}
#endif
void IN_4StrafeDown(void) {IN_KeyDown(&cis[3].in_strafe);}
void IN_4StrafeUp(void) {IN_KeyUp(&cis[3].in_strafe);}
#ifdef TURTLEARENA // LOCKON
void IN_4LockonDown(void) {IN_KeyDown(&cis[3].in_lockon);IN_KeyDown(&cis[3].in_strafe);}
void IN_4LockonUp(void) {IN_KeyUp(&cis[3].in_lockon);IN_KeyUp(&cis[3].in_strafe);}
#endif
#endif

#ifdef USE_VOIP
void IN_VoipRecordDown(void)
{
	IN_KeyDown(&in_voiprecord);
	Cvar_Set("cl_voipSend", "1");
}

void IN_VoipRecordUp(void)
{
	IN_KeyUp(&in_voiprecord);
	Cvar_Set("cl_voipSend", "0");
}
#endif

void IN_Button0Down(void) {IN_KeyDown(&cis[0].in_buttons[0]);}
void IN_Button0Up(void) {IN_KeyUp(&cis[0].in_buttons[0]);}
void IN_Button1Down(void) {IN_KeyDown(&cis[0].in_buttons[1]);}
void IN_Button1Up(void) {IN_KeyUp(&cis[0].in_buttons[1]);}
void IN_Button2Down(void) {IN_KeyDown(&cis[0].in_buttons[2]);}
void IN_Button2Up(void) {IN_KeyUp(&cis[0].in_buttons[2]);}
void IN_Button3Down(void) {IN_KeyDown(&cis[0].in_buttons[3]);}
void IN_Button3Up(void) {IN_KeyUp(&cis[0].in_buttons[3]);}
void IN_Button4Down(void) {IN_KeyDown(&cis[0].in_buttons[4]);}
void IN_Button4Up(void) {IN_KeyUp(&cis[0].in_buttons[4]);}
void IN_Button5Down(void) {IN_KeyDown(&cis[0].in_buttons[5]);}
void IN_Button5Up(void) {IN_KeyUp(&cis[0].in_buttons[5]);}
void IN_Button6Down(void) {IN_KeyDown(&cis[0].in_buttons[6]);}
void IN_Button6Up(void) {IN_KeyUp(&cis[0].in_buttons[6]);}
void IN_Button7Down(void) {IN_KeyDown(&cis[0].in_buttons[7]);}
void IN_Button7Up(void) {IN_KeyUp(&cis[0].in_buttons[7]);}
void IN_Button8Down(void) {IN_KeyDown(&cis[0].in_buttons[8]);}
void IN_Button8Up(void) {IN_KeyUp(&cis[0].in_buttons[8]);}
void IN_Button9Down(void) {IN_KeyDown(&cis[0].in_buttons[9]);}
void IN_Button9Up(void) {IN_KeyUp(&cis[0].in_buttons[9]);}
void IN_Button10Down(void) {IN_KeyDown(&cis[0].in_buttons[10]);}
void IN_Button10Up(void) {IN_KeyUp(&cis[0].in_buttons[10]);}
void IN_Button11Down(void) {IN_KeyDown(&cis[0].in_buttons[11]);}
void IN_Button11Up(void) {IN_KeyUp(&cis[0].in_buttons[11]);}
void IN_Button12Down(void) {IN_KeyDown(&cis[0].in_buttons[12]);}
void IN_Button12Up(void) {IN_KeyUp(&cis[0].in_buttons[12]);}
void IN_Button13Down(void) {IN_KeyDown(&cis[0].in_buttons[13]);}
void IN_Button13Up(void) {IN_KeyUp(&cis[0].in_buttons[13]);}
void IN_Button14Down(void) {IN_KeyDown(&cis[0].in_buttons[14]);}
void IN_Button14Up(void) {IN_KeyUp(&cis[0].in_buttons[14]);}
void IN_Button15Down(void) {IN_KeyDown(&cis[0].in_buttons[15]);}
void IN_Button15Up(void) {IN_KeyUp(&cis[0].in_buttons[15]);}

#ifdef TA_SPLITVIEW
void IN_2Button0Down(void) {IN_KeyDown(&cis[1].in_buttons[0]);}
void IN_2Button0Up(void) {IN_KeyUp(&cis[1].in_buttons[0]);}
void IN_2Button1Down(void) {IN_KeyDown(&cis[1].in_buttons[1]);}
void IN_2Button1Up(void) {IN_KeyUp(&cis[1].in_buttons[1]);}
void IN_2Button2Down(void) {IN_KeyDown(&cis[1].in_buttons[2]);}
void IN_2Button2Up(void) {IN_KeyUp(&cis[1].in_buttons[2]);}
void IN_2Button3Down(void) {IN_KeyDown(&cis[1].in_buttons[3]);}
void IN_2Button3Up(void) {IN_KeyUp(&cis[1].in_buttons[3]);}
void IN_2Button4Down(void) {IN_KeyDown(&cis[1].in_buttons[4]);}
void IN_2Button4Up(void) {IN_KeyUp(&cis[1].in_buttons[4]);}
void IN_2Button5Down(void) {IN_KeyDown(&cis[1].in_buttons[5]);}
void IN_2Button5Up(void) {IN_KeyUp(&cis[1].in_buttons[5]);}
void IN_2Button6Down(void) {IN_KeyDown(&cis[1].in_buttons[6]);}
void IN_2Button6Up(void) {IN_KeyUp(&cis[1].in_buttons[6]);}
void IN_2Button7Down(void) {IN_KeyDown(&cis[1].in_buttons[7]);}
void IN_2Button7Up(void) {IN_KeyUp(&cis[1].in_buttons[7]);}
void IN_2Button8Down(void) {IN_KeyDown(&cis[1].in_buttons[8]);}
void IN_2Button8Up(void) {IN_KeyUp(&cis[1].in_buttons[8]);}
void IN_2Button9Down(void) {IN_KeyDown(&cis[1].in_buttons[9]);}
void IN_2Button9Up(void) {IN_KeyUp(&cis[1].in_buttons[9]);}
void IN_2Button10Down(void) {IN_KeyDown(&cis[1].in_buttons[10]);}
void IN_2Button10Up(void) {IN_KeyUp(&cis[1].in_buttons[10]);}
void IN_2Button11Down(void) {IN_KeyDown(&cis[1].in_buttons[11]);}
void IN_2Button11Up(void) {IN_KeyUp(&cis[1].in_buttons[11]);}
void IN_2Button12Down(void) {IN_KeyDown(&cis[1].in_buttons[12]);}
void IN_2Button12Up(void) {IN_KeyUp(&cis[1].in_buttons[12]);}
void IN_2Button13Down(void) {IN_KeyDown(&cis[1].in_buttons[13]);}
void IN_2Button13Up(void) {IN_KeyUp(&cis[1].in_buttons[13]);}
void IN_2Button14Down(void) {IN_KeyDown(&cis[1].in_buttons[14]);}
void IN_2Button14Up(void) {IN_KeyUp(&cis[1].in_buttons[14]);}
void IN_2Button15Down(void) {IN_KeyDown(&cis[1].in_buttons[15]);}
void IN_2Button15Up(void) {IN_KeyUp(&cis[1].in_buttons[15]);}

void IN_3Button0Down(void) {IN_KeyDown(&cis[2].in_buttons[0]);}
void IN_3Button0Up(void) {IN_KeyUp(&cis[2].in_buttons[0]);}
void IN_3Button1Down(void) {IN_KeyDown(&cis[2].in_buttons[1]);}
void IN_3Button1Up(void) {IN_KeyUp(&cis[2].in_buttons[1]);}
void IN_3Button2Down(void) {IN_KeyDown(&cis[2].in_buttons[2]);}
void IN_3Button2Up(void) {IN_KeyUp(&cis[2].in_buttons[2]);}
void IN_3Button3Down(void) {IN_KeyDown(&cis[2].in_buttons[3]);}
void IN_3Button3Up(void) {IN_KeyUp(&cis[2].in_buttons[3]);}
void IN_3Button4Down(void) {IN_KeyDown(&cis[2].in_buttons[4]);}
void IN_3Button4Up(void) {IN_KeyUp(&cis[2].in_buttons[4]);}
void IN_3Button5Down(void) {IN_KeyDown(&cis[2].in_buttons[5]);}
void IN_3Button5Up(void) {IN_KeyUp(&cis[2].in_buttons[5]);}
void IN_3Button6Down(void) {IN_KeyDown(&cis[2].in_buttons[6]);}
void IN_3Button6Up(void) {IN_KeyUp(&cis[2].in_buttons[6]);}
void IN_3Button7Down(void) {IN_KeyDown(&cis[2].in_buttons[7]);}
void IN_3Button7Up(void) {IN_KeyUp(&cis[2].in_buttons[7]);}
void IN_3Button8Down(void) {IN_KeyDown(&cis[2].in_buttons[8]);}
void IN_3Button8Up(void) {IN_KeyUp(&cis[2].in_buttons[8]);}
void IN_3Button9Down(void) {IN_KeyDown(&cis[2].in_buttons[9]);}
void IN_3Button9Up(void) {IN_KeyUp(&cis[2].in_buttons[9]);}
void IN_3Button10Down(void) {IN_KeyDown(&cis[2].in_buttons[10]);}
void IN_3Button10Up(void) {IN_KeyUp(&cis[2].in_buttons[10]);}
void IN_3Button11Down(void) {IN_KeyDown(&cis[2].in_buttons[11]);}
void IN_3Button11Up(void) {IN_KeyUp(&cis[2].in_buttons[11]);}
void IN_3Button12Down(void) {IN_KeyDown(&cis[2].in_buttons[12]);}
void IN_3Button12Up(void) {IN_KeyUp(&cis[2].in_buttons[12]);}
void IN_3Button13Down(void) {IN_KeyDown(&cis[2].in_buttons[13]);}
void IN_3Button13Up(void) {IN_KeyUp(&cis[2].in_buttons[13]);}
void IN_3Button14Down(void) {IN_KeyDown(&cis[2].in_buttons[14]);}
void IN_3Button14Up(void) {IN_KeyUp(&cis[2].in_buttons[14]);}
void IN_3Button15Down(void) {IN_KeyDown(&cis[2].in_buttons[15]);}
void IN_3Button15Up(void) {IN_KeyUp(&cis[2].in_buttons[15]);}

void IN_4Button0Down(void) {IN_KeyDown(&cis[3].in_buttons[0]);}
void IN_4Button0Up(void) {IN_KeyUp(&cis[3].in_buttons[0]);}
void IN_4Button1Down(void) {IN_KeyDown(&cis[3].in_buttons[1]);}
void IN_4Button1Up(void) {IN_KeyUp(&cis[3].in_buttons[1]);}
void IN_4Button2Down(void) {IN_KeyDown(&cis[3].in_buttons[2]);}
void IN_4Button2Up(void) {IN_KeyUp(&cis[3].in_buttons[2]);}
void IN_4Button3Down(void) {IN_KeyDown(&cis[3].in_buttons[3]);}
void IN_4Button3Up(void) {IN_KeyUp(&cis[3].in_buttons[3]);}
void IN_4Button4Down(void) {IN_KeyDown(&cis[3].in_buttons[4]);}
void IN_4Button4Up(void) {IN_KeyUp(&cis[3].in_buttons[4]);}
void IN_4Button5Down(void) {IN_KeyDown(&cis[3].in_buttons[5]);}
void IN_4Button5Up(void) {IN_KeyUp(&cis[3].in_buttons[5]);}
void IN_4Button6Down(void) {IN_KeyDown(&cis[3].in_buttons[6]);}
void IN_4Button6Up(void) {IN_KeyUp(&cis[3].in_buttons[6]);}
void IN_4Button7Down(void) {IN_KeyDown(&cis[3].in_buttons[7]);}
void IN_4Button7Up(void) {IN_KeyUp(&cis[3].in_buttons[7]);}
void IN_4Button8Down(void) {IN_KeyDown(&cis[3].in_buttons[8]);}
void IN_4Button8Up(void) {IN_KeyUp(&cis[3].in_buttons[8]);}
void IN_4Button9Down(void) {IN_KeyDown(&cis[3].in_buttons[9]);}
void IN_4Button9Up(void) {IN_KeyUp(&cis[3].in_buttons[9]);}
void IN_4Button10Down(void) {IN_KeyDown(&cis[3].in_buttons[10]);}
void IN_4Button10Up(void) {IN_KeyUp(&cis[3].in_buttons[10]);}
void IN_4Button11Down(void) {IN_KeyDown(&cis[3].in_buttons[11]);}
void IN_4Button11Up(void) {IN_KeyUp(&cis[3].in_buttons[11]);}
void IN_4Button12Down(void) {IN_KeyDown(&cis[3].in_buttons[12]);}
void IN_4Button12Up(void) {IN_KeyUp(&cis[3].in_buttons[12]);}
void IN_4Button13Down(void) {IN_KeyDown(&cis[3].in_buttons[13]);}
void IN_4Button13Up(void) {IN_KeyUp(&cis[3].in_buttons[13]);}
void IN_4Button14Down(void) {IN_KeyDown(&cis[3].in_buttons[14]);}
void IN_4Button14Up(void) {IN_KeyUp(&cis[3].in_buttons[14]);}
void IN_4Button15Down(void) {IN_KeyDown(&cis[3].in_buttons[15]);}
void IN_4Button15Up(void) {IN_KeyUp(&cis[3].in_buttons[15]);}
#endif

#ifdef TA_SPLITVIEW
void IN_CenterView_Main(int localClientNum) {
	if (localClientNum >= MAX_SPLITVIEW || cl.snap.lcIndex[localClientNum] == -1) {
		return;
	}
	cl.localClients[localClientNum].viewangles[PITCH] = -SHORT2ANGLE(cl.snap.pss[cl.snap.lcIndex[localClientNum]].delta_angles[PITCH]);
}

void IN_CenterView (void) {
	IN_CenterView_Main(0);
}

void IN_2CenterView (void) {
	IN_CenterView_Main(1);
}

void IN_3CenterView (void) {
	IN_CenterView_Main(2);
}

void IN_4CenterView (void) {
	IN_CenterView_Main(3);
}
#else
void IN_CenterView (void) {
	cl.localClient.viewangles[PITCH] = -SHORT2ANGLE(cl.snap.ps.delta_angles[PITCH]);
}
#endif


//==========================================================================

cvar_t	*cl_upspeed;
cvar_t	*cl_forwardspeed;
cvar_t	*cl_sidespeed;

cvar_t	*cl_yawspeed;
cvar_t	*cl_pitchspeed;

#ifndef TURTLEARENA // ALWAYS_RUN
#ifdef TA_SPLITVIEW
cvar_t	*cl_run[MAX_SPLITVIEW];
#else
cvar_t	*cl_run;
#endif
#endif

cvar_t	*cl_anglespeedkey;


/*
================
CL_AdjustAngles

Moves the local angle positions
================
*/
void CL_AdjustAngles( calc_t *lc, clientInput_t *ci ) {
	float	speed;
	
#ifdef TURTLEARENA // LOCKON // NO_SPEED_KEY
	if ( !ci->in_lockon.active )
#else
	if ( ci->in_speed.active )
#endif
	{
		speed = 0.001 * cls.frametime * cl_anglespeedkey->value;
	} else {
		speed = 0.001 * cls.frametime;
	}

	if ( !ci->in_strafe.active
#ifdef TA_PATHSYS // 2DMODE
		&& ci->pathMode != PATHMODE_SIDE && ci->pathMode != PATHMODE_BACK
#endif
		)
	{
		lc->viewangles[YAW] -= speed*cl_yawspeed->value*CL_KeyState (&ci->in_right);
		lc->viewangles[YAW] += speed*cl_yawspeed->value*CL_KeyState (&ci->in_left);
	}

	lc->viewangles[PITCH] -= speed*cl_pitchspeed->value * CL_KeyState (&ci->in_lookup);
	lc->viewangles[PITCH] += speed*cl_pitchspeed->value * CL_KeyState (&ci->in_lookdown);
}

/*
================
CL_KeyMove

Sets the usercmd_t based on key states
================
*/
void CL_KeyMove( clientInput_t *ci, usercmd_t *cmd ) {
	int		movespeed;
	int		forward, side, up;

	//
	// adjust for speed key / running
	// the walking flag is to keep animations consistant
	// even during acceleration and develeration
	//
#ifdef TURTLEARENA // LOCKON // ALWAYS_RUN // NO_SPEED_KEY
	if (!ci->in_lockon.active)
#else
#ifdef TA_SPLITVIEW
	if ( ci->in_speed.active ^ cl_run[ci-cis]->integer )
#else
	if ( ci->in_speed.active ^ cl_run->integer )
#endif
#endif
	{
		movespeed = 127;
		cmd->buttons &= ~BUTTON_WALKING;
	} else {
		cmd->buttons |= BUTTON_WALKING;
		movespeed = 64;
	}

	forward = 0;
	side = 0;
	up = 0;
	if ( ci->in_strafe.active
#ifdef TA_PATHSYS // 2DMODE
		|| ci->pathMode == PATHMODE_SIDE || ci->pathMode == PATHMODE_BACK
#endif
		)
	{
		side += movespeed * CL_KeyState (&ci->in_right);
		side -= movespeed * CL_KeyState (&ci->in_left);
	}

	side += movespeed * CL_KeyState (&ci->in_moveright);
	side -= movespeed * CL_KeyState (&ci->in_moveleft);


	up += movespeed * CL_KeyState (&ci->in_up);
	up -= movespeed * CL_KeyState (&ci->in_down);

	forward += movespeed * CL_KeyState (&ci->in_forward);
	forward -= movespeed * CL_KeyState (&ci->in_back);

	cmd->forwardmove = ClampChar( forward );
	cmd->rightmove = ClampChar( side );
	cmd->upmove = ClampChar( up );
}

/*
=================
CL_MouseEvent
=================
*/
void CL_MouseEvent( int dx, int dy, int time ) {
	if ( Key_GetCatcher( ) & KEYCATCH_UI ) {
		VM_Call( uivm, UI_MOUSE_EVENT, dx, dy );
	} else if (Key_GetCatcher( ) & KEYCATCH_CGAME) {
		VM_Call (cgvm, CG_MOUSE_EVENT, dx, dy);
	} else {
#ifdef TA_SPLITVIEW
		cl.localClients[0].mouseDx[cl.localClients[0].mouseIndex] += dx;
		cl.localClients[0].mouseDy[cl.localClients[0].mouseIndex] += dy;
#else
		cl.localClient.mouseDx[cl.localClient.mouseIndex] += dx;
		cl.localClient.mouseDy[cl.localClient.mouseIndex] += dy;
#endif
	}
}

/*
=================
CL_JoystickEvent

Joystick values stay set until changed
=================
*/
void CL_JoystickEvent( int axis, int value, int time ) {
	if ( axis < 0 || axis >= MAX_JOYSTICK_AXIS ) {
		Com_Error( ERR_DROP, "CL_JoystickEvent: bad axis %i", axis );
	}
#ifdef TA_SPLITVIEW
	cl.localClients[0].joystickAxis[axis] = value;
#else
	cl.localClient.joystickAxis[axis] = value;
#endif
}

/*
=================
CL_JoystickMove
=================
*/
void CL_JoystickMove( calc_t *lc, clientInput_t *ci, usercmd_t *cmd ) {
	int		movespeed;
	float	anglespeed;
#ifdef TA_SPLITVIEW
	size_t	lcNum = lc - cl.localClients;
#endif

#ifdef TURTLEARENA // LOCKON // ALWAYS_RUN // NO_SPEED_KEY
	if (!ci->in_lockon.active)
#else
#ifdef TA_SPLITVIEW
	if ( ci->in_speed.active ^ cl_run[ci-cis]->integer )
#else
	if ( ci->in_speed.active ^ cl_run->integer )
#endif
#endif
	{
		movespeed = 2;
	} else {
		movespeed = 1;
		cmd->buttons |= BUTTON_WALKING;
	}

#ifdef TURTLEARENA // LOCKON // NO_SPEED_KEY
	if ( !ci->in_lockon.active )
#else
	if ( ci->in_speed.active )
#endif
	{
		anglespeed = 0.001 * cls.frametime * cl_anglespeedkey->value;
	} else {
		anglespeed = 0.001 * cls.frametime;
	}

	if ( !ci->in_strafe.active
#ifdef TA_PATHSYS // 2DMODE
		&& ci->pathMode != PATHMODE_SIDE && ci->pathMode != PATHMODE_BACK
#endif
		)
	{
#ifdef TA_SPLITVIEW
		lc->viewangles[YAW] += anglespeed * j_yaw[lcNum]->value * lc->joystickAxis[j_yaw_axis[lcNum]->integer];
		cmd->rightmove = ClampChar( cmd->rightmove + (int) (j_side[lcNum]->value * lc->joystickAxis[j_side_axis[lcNum]->integer]) );
#else
		lc->viewangles[YAW] += anglespeed * j_yaw->value * lc->joystickAxis[j_yaw_axis->integer];
		cmd->rightmove = ClampChar( cmd->rightmove + (int) (j_side->value * lc->joystickAxis[j_side_axis->integer]) );
#endif
	} else {
#ifdef TA_SPLITVIEW
		lc->viewangles[YAW] += anglespeed * j_side[lcNum]->value * lc->joystickAxis[j_side_axis[lcNum]->integer];
		cmd->rightmove = ClampChar( cmd->rightmove + (int) (j_yaw[lcNum]->value * lc->joystickAxis[j_yaw_axis[lcNum]->integer]) );
#else
		lc->viewangles[YAW] += anglespeed * j_side->value * lc->joystickAxis[j_side_axis->integer];
		cmd->rightmove = ClampChar( cmd->rightmove + (int) (j_yaw->value * lc->joystickAxis[j_yaw_axis->integer]) );
#endif
	}

	if ( in_mlooking ) {
#ifdef TA_SPLITVIEW
		lc->viewangles[PITCH] += anglespeed * j_forward[lcNum]->value * lc->joystickAxis[j_forward_axis[lcNum]->integer];
		cmd->forwardmove = ClampChar( cmd->forwardmove + (int) (j_pitch[lcNum]->value * lc->joystickAxis[j_pitch_axis[lcNum]->integer]) );
#else
		lc->viewangles[PITCH] += anglespeed * j_forward->value * lc->joystickAxis[j_forward_axis->integer];
		cmd->forwardmove = ClampChar( cmd->forwardmove + (int) (j_pitch->value * lc->joystickAxis[j_pitch_axis->integer]) );
#endif
	} else {
#ifdef TA_SPLITVIEW
		lc->viewangles[PITCH] += anglespeed * j_pitch[lcNum]->value * lc->joystickAxis[j_pitch_axis[lcNum]->integer];
		cmd->forwardmove = ClampChar( cmd->forwardmove + (int) (j_forward[lcNum]->value * lc->joystickAxis[j_forward_axis[lcNum]->integer]) );
#else
		lc->viewangles[PITCH] += anglespeed * j_pitch->value * lc->joystickAxis[j_pitch_axis->integer];
		cmd->forwardmove = ClampChar( cmd->forwardmove + (int) (j_forward->value * lc->joystickAxis[j_forward_axis->integer]) );
#endif
	}

	cmd->upmove = ClampChar( cmd->upmove + lc->joystickAxis[AXIS_UP] );
}

/*
=================
CL_MouseMove
=================
*/

void CL_MouseMove(calc_t *lc, clientInput_t *ci, usercmd_t *cmd)
{
	float mx, my;

	// allow mouse smoothing
	if (m_filter->integer)
	{
		mx = (lc->mouseDx[0] + lc->mouseDx[1]) * 0.5f;
		my = (lc->mouseDy[0] + lc->mouseDy[1]) * 0.5f;
	}
	else
	{
		mx = lc->mouseDx[lc->mouseIndex];
		my = lc->mouseDy[lc->mouseIndex];
	}
	
	lc->mouseIndex ^= 1;
	lc->mouseDx[lc->mouseIndex] = 0;
	lc->mouseDy[lc->mouseIndex] = 0;

	if (mx == 0.0f && my == 0.0f)
		return;
	
	if (cl_mouseAccel->value != 0.0f)
	{
		if(cl_mouseAccelStyle->integer == 0)
		{
			float accelSensitivity;
			float rate;
			
			rate = sqrt(mx * mx + my * my) / (float) frame_msec;

			accelSensitivity = cl_sensitivity->value + rate * cl_mouseAccel->value;
			mx *= accelSensitivity;
			my *= accelSensitivity;
			
			if(cl_showMouseRate->integer)
				Com_Printf("rate: %f, accelSensitivity: %f\n", rate, accelSensitivity);
		}
		else
		{
			float rate[2];
			float power[2];

			// sensitivity remains pretty much unchanged at low speeds
			// cl_mouseAccel is a power value to how the acceleration is shaped
			// cl_mouseAccelOffset is the rate for which the acceleration will have doubled the non accelerated amplification
			// NOTE: decouple the config cvars for independent acceleration setup along X and Y?

			rate[0] = fabs(mx) / (float) frame_msec;
			rate[1] = fabs(my) / (float) frame_msec;
			power[0] = powf(rate[0] / cl_mouseAccelOffset->value, cl_mouseAccel->value);
			power[1] = powf(rate[1] / cl_mouseAccelOffset->value, cl_mouseAccel->value);

			mx = cl_sensitivity->value * (mx + ((mx < 0) ? -power[0] : power[0]) * cl_mouseAccelOffset->value);
			my = cl_sensitivity->value * (my + ((my < 0) ? -power[1] : power[1]) * cl_mouseAccelOffset->value);

			if(cl_showMouseRate->integer)
				Com_Printf("ratex: %f, ratey: %f, powx: %f, powy: %f\n", rate[0], rate[1], power[0], power[1]);
		}
	}
	else
	{
		mx *= cl_sensitivity->value;
		my *= cl_sensitivity->value;
	}

	// ingame FOV
	mx *= lc->cgameSensitivity;
	my *= lc->cgameSensitivity;

	// add mouse X/Y movement to cmd
	if(ci->in_strafe.active
#ifdef TA_PATHSYS // 2DMODE
		|| ci->pathMode == PATHMODE_SIDE || ci->pathMode == PATHMODE_BACK
#endif
		)
	{
		cmd->rightmove = ClampChar(cmd->rightmove + m_side->value * mx);
#ifdef TURTLEARENA // LOCKON
		// if walking, don't go over 64 side move
		if (ci->in_lockon.active)
		{
			if (cmd->rightmove > 64)
				cmd->rightmove = 64;
			else if (cmd->rightmove < -64)
				cmd->rightmove = -64;
		}
#endif
	}
	else
		lc->viewangles[YAW] -= m_yaw->value * mx;

	if ((in_mlooking || cl_freelook->integer)
#ifdef TURTLEARENA // LOCKON
		&& (ci->in_lockon.active || !ci->in_strafe.active)
#else
		&& !ci->in_strafe.active
#endif
#ifdef TA_PATHSYS // 2DMODE
		&& ci->pathMode != PATHMODE_SIDE && ci->pathMode != PATHMODE_BACK
#endif
		)
		lc->viewangles[PITCH] += m_pitch->value * my;
	else
		cmd->forwardmove = ClampChar(cmd->forwardmove - m_forward->value * my);
}


/*
==============
CL_CmdButtons
==============
*/
void CL_CmdButtons( clientInput_t *ci, usercmd_t *cmd ) {
	int		i;

	//
	// figure button bits
	// send a button bit even if the key was pressed and released in
	// less than a frame
	//	
	for (i = 0 ; i < 15 ; i++) {
		if ( ci->in_buttons[i].active || ci->in_buttons[i].wasPressed ) {
			cmd->buttons |= 1 << i;
		}
		ci->in_buttons[i].wasPressed = qfalse;
	}

	if ( Key_GetCatcher( ) ) {
		cmd->buttons |= BUTTON_TALK;
	}

	// allow the game to know if any key at all is
	// currently pressed, even if it isn't bound to anything
	if ( anykeydown && Key_GetCatcher( ) == 0 ) {
		cmd->buttons |= BUTTON_ANY;
	}
}


/*
==============
CL_FinishMove
==============
*/
void CL_FinishMove( calc_t *lc, usercmd_t *cmd ) {
	int		i;

	// copy the state that the cgame is currently sending
#if !defined TA_WEAPSYS_EX || defined TA_WEAPSYS_EX_COMPAT
	cmd->weapon = lc->cgameUserCmdValue;
#endif
#ifdef TA_HOLDSYS/*2*/
	cmd->holdable = lc->cgameHoldableValue;
#endif

	// send the current server time so the amount of movement
	// can be determined without allowing cheating
	cmd->serverTime = cl.serverTime;

#ifdef ANALOG // Do analog move!
	if (cl_thirdPerson->integer && cl_thirdPersonAnalog->integer && cl_thirdPersonAngle->value)
	{
		if (cmd->forwardmove || cmd->rightmove || cmd->upmove)
		{
			lc->viewangles[YAW] -= cl_thirdPersonAngle->value;
			Cvar_Set("cg_thirdPersonAngle", "0");
		}
	}
#endif
	for (i=0 ; i<3 ; i++) {
		cmd->angles[i] = ANGLE2SHORT(lc->viewangles[i]);
	}
}


/*
=================
CL_CreateCmd
=================
*/
#ifdef TA_SPLITVIEW // CONTROLS
usercmd_t CL_CreateCmd( int localClientNum )
#else
usercmd_t CL_CreateCmd( void )
#endif
{
	usercmd_t	cmd;
	vec3_t		oldAngles;
	calc_t		*lc;
	clientInput_t *ci;
#ifdef TA_SPLITVIEW // CONTROLS
	lc = &cl.localClients[localClientNum];
	ci = &cis[localClientNum];
#else
	lc = &cl.localClient;
	ci = &cis[0];
#endif

#ifdef TA_PATHSYS // 2DMODE
#ifdef TA_SPLITVIEW
	ci->pathMode = cl.snap.pss[cl.snap.lcIndex[localClientNum]].pathMode;
#else
	ci->pathMode = cl.snap.ps.pathMode;
#endif
#endif

	VectorCopy( lc->viewangles, oldAngles );

	// keyboard angle adjustment
	CL_AdjustAngles (lc, ci);
	
	Com_Memset( &cmd, 0, sizeof( cmd ) );

	CL_CmdButtons( ci, &cmd );

	// get basic movement from keyboard
	CL_KeyMove( ci, &cmd );

	// get basic movement from mouse
	CL_MouseMove( lc, ci, &cmd );

	// get basic movement from joystick
	CL_JoystickMove( lc, ci, &cmd );

	// check to make sure the angles haven't wrapped
	if ( lc->viewangles[PITCH] - oldAngles[PITCH] > 90 ) {
		lc->viewangles[PITCH] = oldAngles[PITCH] + 90;
	} else if ( oldAngles[PITCH] - lc->viewangles[PITCH] > 90 ) {
		lc->viewangles[PITCH] = oldAngles[PITCH] - 90;
	} 

	// store out the final values
	CL_FinishMove( lc, &cmd );

	// draw debug graphs of turning for mouse testing
	if ( cl_debugMove->integer ) {
		if ( cl_debugMove->integer == 1 ) {
			SCR_DebugGraph( abs(lc->viewangles[YAW] - oldAngles[YAW]), 0 );
		}
		if ( cl_debugMove->integer == 2 ) {
			SCR_DebugGraph( abs(lc->viewangles[PITCH] - oldAngles[PITCH]), 0 );
		}
	}

	return cmd;
}


/*
=================
CL_CreateNewCommands

Create a new usercmd_t structure for this frame
=================
*/
void CL_CreateNewCommands( void ) {
#ifdef TA_SPLITVIEW // CONTROLS
	int			i;
#else
	usercmd_t	*cmd;
#endif
	int			cmdNum;

	// no need to create usercmds until we have a gamestate
	if ( cls.state < CA_PRIMED ) {
		return;
	}

	frame_msec = com_frameTime - old_com_frameTime;

	// if running less than 5fps, truncate the extra time to prevent
	// unexpected moves after a hitch
	if ( frame_msec > 200 ) {
		frame_msec = 200;
	}
	old_com_frameTime = com_frameTime;


	// generate a command for this frame
	cl.cmdNumber++;
	cmdNum = cl.cmdNumber & CMD_MASK;
#ifdef TA_SPLITVIEW // CONTROLS
	for (i = 0; i < MAX_SPLITVIEW; i++) {
		if (cl.snap.valid && cl.snap.lcIndex[i] == -1) {
			continue;
		}
		cl.cmdss[i][cmdNum] = CL_CreateCmd(i);
	}
#else
	cl.cmds[cmdNum] = CL_CreateCmd ();
	cmd = &cl.cmds[cmdNum];
#endif
}

/*
=================
CL_ReadyToSendPacket

Returns qfalse if we are over the maxpackets limit
and should choke back the bandwidth a bit by not sending
a packet this frame.  All the commands will still get
delivered in the next packet, but saving a header and
getting more delta compression will reduce total bandwidth.
=================
*/
qboolean CL_ReadyToSendPacket( void ) {
	int		oldPacketNum;
	int		delta;

	// don't send anything if playing back a demo
	if ( clc.demoplaying || cls.state == CA_CINEMATIC ) {
		return qfalse;
	}

	// If we are downloading, we send no less than 50ms between packets
	if ( *clc.downloadTempName &&
		cls.realtime - clc.lastPacketSentTime < 50 ) {
		return qfalse;
	}

	// if we don't have a valid gamestate yet, only send
	// one packet a second
	if ( cls.state != CA_ACTIVE && 
		cls.state != CA_PRIMED && 
		!*clc.downloadTempName &&
		cls.realtime - clc.lastPacketSentTime < 1000 ) {
		return qfalse;
	}

	// send every frame for loopbacks
	if ( clc.netchan.remoteAddress.type == NA_LOOPBACK ) {
		return qtrue;
	}

	// send every frame for LAN
	if ( cl_lanForcePackets->integer && Sys_IsLANAddress( clc.netchan.remoteAddress ) ) {
		return qtrue;
	}

	// check for exceeding cl_maxpackets
	if ( cl_maxpackets->integer < 15 ) {
		Cvar_Set( "cl_maxpackets", "15" );
	} else if ( cl_maxpackets->integer > 125 ) {
		Cvar_Set( "cl_maxpackets", "125" );
	}
	oldPacketNum = (clc.netchan.outgoingSequence - 1) & PACKET_MASK;
	delta = cls.realtime -  cl.outPackets[ oldPacketNum ].p_realtime;
	if ( delta < 1000 / cl_maxpackets->integer ) {
		// the accumulated commands will go out in the next packet
		return qfalse;
	}

	return qtrue;
}

/*
===================
CL_WritePacket

Create and send the command packet to the server
Including both the reliable commands and the usercmds

During normal gameplay, a client packet will contain something like:

4	sequence number
2	qport
4	serverid
4	acknowledged sequence number
4	clc.serverCommandSequence
<optional reliable commands>
1	clc_move or clc_moveNoDelta
1	command count
<count * usercmds>

===================
*/
void CL_WritePacket( void ) {
	msg_t		buf;
	byte		data[MAX_MSGLEN];
	int			i, j;
	usercmd_t	*cmd, *oldcmd;
	usercmd_t	nullcmd;
	int			packetNum;
	int			oldPacketNum;
	int			count, key;

	// don't send anything if playing back a demo
	if ( clc.demoplaying || cls.state == CA_CINEMATIC ) {
		return;
	}

	Com_Memset( &nullcmd, 0, sizeof(nullcmd) );
	oldcmd = &nullcmd;

	MSG_Init( &buf, data, sizeof(data) );

	MSG_Bitstream( &buf );
	// write the current serverId so the server
	// can tell if this is from the current gameState
	MSG_WriteLong( &buf, cl.serverId );

	// write the last message we received, which can
	// be used for delta compression, and is also used
	// to tell if we dropped a gamestate
	MSG_WriteLong( &buf, clc.serverMessageSequence );

	// write the last reliable message we received
	MSG_WriteLong( &buf, clc.serverCommandSequence );

	// write any unacknowledged clientCommands
	for ( i = clc.reliableAcknowledge + 1 ; i <= clc.reliableSequence ; i++ ) {
		MSG_WriteByte( &buf, clc_clientCommand );
		MSG_WriteLong( &buf, i );
		MSG_WriteString( &buf, clc.reliableCommands[ i & (MAX_RELIABLE_COMMANDS-1) ] );
	}

	// we want to send all the usercmds that were generated in the last
	// few packet, so even if a couple packets are dropped in a row,
	// all the cmds will make it to the server
	if ( cl_packetdup->integer < 0 ) {
		Cvar_Set( "cl_packetdup", "0" );
	} else if ( cl_packetdup->integer > 5 ) {
		Cvar_Set( "cl_packetdup", "5" );
	}
	oldPacketNum = (clc.netchan.outgoingSequence - 1 - cl_packetdup->integer) & PACKET_MASK;
	count = cl.cmdNumber - cl.outPackets[ oldPacketNum ].p_cmdNumber;
	if ( count > MAX_PACKET_USERCMDS ) {
		count = MAX_PACKET_USERCMDS;
		Com_Printf("MAX_PACKET_USERCMDS\n");
	}

#ifdef USE_VOIP
	if (clc.voipOutgoingDataSize > 0) {  // only send if data.
		// Move cl_voipSendTarget from a string to the bitmasks if needed.
		if (cl_voipSendTarget->modified) {
			char buffer[32];
			const char *target = cl_voipSendTarget->string;

			if (Q_stricmp(target, "attacker") == 0) {
				int player = VM_Call( cgvm, CG_LAST_ATTACKER );
				Com_sprintf(buffer, sizeof (buffer), "%d", player);
				target = buffer;
			} else if (Q_stricmp(target, "crosshair") == 0) {
				int player = VM_Call( cgvm, CG_CROSSHAIR_PLAYER );
				Com_sprintf(buffer, sizeof (buffer), "%d", player);
				target = buffer;
			}

			if ((*target == '\0') || (Q_stricmp(target, "all") == 0)) {
				const int all = 0x7FFFFFFF;
				clc.voipTarget1 = clc.voipTarget2 = clc.voipTarget3 = all;
			} else if (Q_stricmp(target, "none") == 0) {
				clc.voipTarget1 = clc.voipTarget2 = clc.voipTarget3 = 0;
			} else {
				const char *ptr = target;
				clc.voipTarget1 = clc.voipTarget2 = clc.voipTarget3 = 0;
				do {
					if ((*ptr == ',') || (*ptr == '\0')) {
						const int val = atoi(target);
						target = ptr + 1;
						if ((val >= 0) && (val < 31)) {
							clc.voipTarget1 |= (1 << (val-0));
						} else if ((val >= 31) && (val < 62)) {
							clc.voipTarget2 |= (1 << (val-31));
						} else if ((val >= 62) && (val < 93)) {
							clc.voipTarget3 |= (1 << (val-62));
						}
					}
				} while (*(ptr++));
			}
			cl_voipSendTarget->modified = qfalse;
		}

		MSG_WriteByte (&buf, clc_EOF);  // placate legacy servers.
		MSG_WriteByte (&buf, clc_extension);
		MSG_WriteByte (&buf, clc_voip);
		MSG_WriteByte (&buf, clc.voipOutgoingGeneration);
		MSG_WriteLong (&buf, clc.voipOutgoingSequence);
		MSG_WriteByte (&buf, clc.voipOutgoingDataFrames);
		MSG_WriteLong (&buf, clc.voipTarget1);
		MSG_WriteLong (&buf, clc.voipTarget2);
		MSG_WriteLong (&buf, clc.voipTarget3);
		MSG_WriteShort (&buf, clc.voipOutgoingDataSize);
		MSG_WriteData (&buf, clc.voipOutgoingData, clc.voipOutgoingDataSize);

		// If we're recording a demo, we have to fake a server packet with
		//  this VoIP data so it gets to disk; the server doesn't send it
		//  back to us, and we might as well eliminate concerns about dropped
		//  and misordered packets here.
		if ( clc.demorecording && !clc.demowaiting ) {
			const int voipSize = clc.voipOutgoingDataSize;
			msg_t fakemsg;
			byte fakedata[MAX_MSGLEN];
			MSG_Init (&fakemsg, fakedata, sizeof (fakedata));
			MSG_Bitstream (&fakemsg);
			MSG_WriteLong (&fakemsg, clc.reliableAcknowledge);
			MSG_WriteByte (&fakemsg, svc_EOF);
			MSG_WriteByte (&fakemsg, svc_extension);
			MSG_WriteByte (&fakemsg, svc_voip);
			MSG_WriteShort (&fakemsg, clc.clientNum);
			MSG_WriteByte (&fakemsg, clc.voipOutgoingGeneration);
			MSG_WriteLong (&fakemsg, clc.voipOutgoingSequence);
			MSG_WriteByte (&fakemsg, clc.voipOutgoingDataFrames);
			MSG_WriteShort (&fakemsg, clc.voipOutgoingDataSize );
			MSG_WriteData (&fakemsg, clc.voipOutgoingData, voipSize);
			MSG_WriteByte (&fakemsg, svc_EOF);
			CL_WriteDemoMessage (&fakemsg, 0);
		}

		clc.voipOutgoingSequence += clc.voipOutgoingDataFrames;
		clc.voipOutgoingDataSize = 0;
		clc.voipOutgoingDataFrames = 0;
	} else
#endif

	if ( count >= 1 ) {
		if ( cl_showSend->integer ) {
			Com_Printf( "(%i)", count );
		}

		// begin a client move command
		if ( cl_nodelta->integer || !cl.snap.valid || clc.demowaiting
			|| clc.serverMessageSequence != cl.snap.messageNum ) {
			MSG_WriteByte (&buf, clc_moveNoDelta);
		} else {
			MSG_WriteByte (&buf, clc_move);
		}

		// write the command count
		MSG_WriteByte( &buf, count );

		// use the checksum feed in the key
		key = clc.checksumFeed;
		// also use the message acknowledge
		key ^= clc.serverMessageSequence;
		// also use the last acknowledged server command in the key
		key ^= MSG_HashKey(clc.serverCommands[ clc.serverCommandSequence & (MAX_RELIABLE_COMMANDS-1) ], 32);

		// write all the commands, including the predicted command
		for ( i = 0 ; i < count ; i++ ) {
			j = (cl.cmdNumber - count + i + 1) & CMD_MASK;
#ifdef TA_SPLITVIEW // CONTROLS
			cmd = &cl.cmdss[0][j];
#else
			cmd = &cl.cmds[j];
#endif
			MSG_WriteDeltaUsercmdKey (&buf, key, oldcmd, cmd);
			oldcmd = cmd;
		}

#ifdef TA_SPLITVIEW
		{
			int lc;

			for (lc = 1; lc < MAX_SPLITVIEW; lc++) {
				if (cl.snap.valid && cl.snap.lcIndex[lc] == -1) {
					continue;
				}

				MSG_WriteByte (&buf, clc_EOF);  // placate legacy servers.
				MSG_WriteByte (&buf, clc_extension);

				// begin a client move command
				if ( cl_nodelta->integer || !cl.snap.valid || clc.demowaiting
					|| clc.serverMessageSequence != cl.snap.messageNum ) {
					MSG_WriteByte (&buf, clc_moveLocalNoDelta);
				} else {
					MSG_WriteByte (&buf, clc_moveLocal);
				}

				MSG_WriteByte (&buf, lc-1); // localClient-1

				// write the command count
				MSG_WriteByte( &buf, count );

				Com_Memset( &nullcmd, 0, sizeof(nullcmd) );
				oldcmd = &nullcmd;

				// write all the commands, including the predicted command
				for ( i = 0 ; i < count ; i++ ) {
					j = (cl.cmdNumber - count + i + 1) & CMD_MASK;
#if 0
					if (qtrue) // Have all clients use the same usercmd_t
						cmd = &cl.cmdss[0][j];
					else
#endif
						cmd = &cl.cmdss[lc][j];
					MSG_WriteDeltaUsercmdKey (&buf, key, oldcmd, cmd);
					oldcmd = cmd;
				}
			}
		}
#endif
	}

	//
	// deliver the message
	//
	packetNum = clc.netchan.outgoingSequence & PACKET_MASK;
	cl.outPackets[ packetNum ].p_realtime = cls.realtime;
	cl.outPackets[ packetNum ].p_serverTime = oldcmd->serverTime;
	cl.outPackets[ packetNum ].p_cmdNumber = cl.cmdNumber;
	clc.lastPacketSentTime = cls.realtime;

	if ( cl_showSend->integer ) {
		Com_Printf( "%i ", buf.cursize );
	}

	CL_Netchan_Transmit (&clc.netchan, &buf);	

	// clients never really should have messages large enough
	// to fragment, but in case they do, fire them all off
	// at once
	// TTimo: this causes a packet burst, which is bad karma for winsock
	// added a WARNING message, we'll see if there are legit situations where this happens
	while ( clc.netchan.unsentFragments ) {
		Com_DPrintf( "WARNING: #462 unsent fragments (not supposed to happen!)\n" );
		CL_Netchan_TransmitNextFragment( &clc.netchan );
	}
}

/*
=================
CL_SendCmd

Called every frame to builds and sends a command packet to the server.
=================
*/
void CL_SendCmd( void ) {
	// don't send any message if not connected
	if ( cls.state < CA_CONNECTED ) {
		return;
	}

	// don't send commands if paused
	if ( com_sv_running->integer && sv_paused->integer && cl_paused->integer ) {
		return;
	}

	// we create commands even if a demo is playing,
	CL_CreateNewCommands();

	// don't send a packet if the last packet was sent too recently
	if ( !CL_ReadyToSendPacket() ) {
		if ( cl_showSend->integer ) {
			Com_Printf( ". " );
		}
		return;
	}

	CL_WritePacket();
}

/*
============
CL_InitInput
============
*/
void CL_InitInput( void ) {
	Cmd_AddCommand ("centerview",IN_CenterView);

	Cmd_AddCommand ("+moveup",IN_UpDown);
	Cmd_AddCommand ("-moveup",IN_UpUp);
	Cmd_AddCommand ("+movedown",IN_DownDown);
	Cmd_AddCommand ("-movedown",IN_DownUp);
	Cmd_AddCommand ("+left",IN_LeftDown);
	Cmd_AddCommand ("-left",IN_LeftUp);
	Cmd_AddCommand ("+right",IN_RightDown);
	Cmd_AddCommand ("-right",IN_RightUp);
	Cmd_AddCommand ("+forward",IN_ForwardDown);
	Cmd_AddCommand ("-forward",IN_ForwardUp);
	Cmd_AddCommand ("+back",IN_BackDown);
	Cmd_AddCommand ("-back",IN_BackUp);
	Cmd_AddCommand ("+lookup", IN_LookupDown);
	Cmd_AddCommand ("-lookup", IN_LookupUp);
	Cmd_AddCommand ("+lookdown", IN_LookdownDown);
	Cmd_AddCommand ("-lookdown", IN_LookdownUp);
	Cmd_AddCommand ("+strafe", IN_StrafeDown);
	Cmd_AddCommand ("-strafe", IN_StrafeUp);
	Cmd_AddCommand ("+moveleft", IN_MoveleftDown);
	Cmd_AddCommand ("-moveleft", IN_MoveleftUp);
	Cmd_AddCommand ("+moveright", IN_MoverightDown);
	Cmd_AddCommand ("-moveright", IN_MoverightUp);
#ifndef TURTLEARENA // NO_SPEED_KEY
	Cmd_AddCommand ("+speed", IN_SpeedDown);
	Cmd_AddCommand ("-speed", IN_SpeedUp);
#endif
#ifdef TURTLEARENA // LOCKON
	Cmd_AddCommand ("+lockon", IN_LockonDown);
	Cmd_AddCommand ("-lockon", IN_LockonUp);
#endif
	Cmd_AddCommand ("+attack", IN_Button0Down);
	Cmd_AddCommand ("-attack", IN_Button0Up);
	Cmd_AddCommand ("+button0", IN_Button0Down);
	Cmd_AddCommand ("-button0", IN_Button0Up);
	Cmd_AddCommand ("+button1", IN_Button1Down);
	Cmd_AddCommand ("-button1", IN_Button1Up);
	Cmd_AddCommand ("+button2", IN_Button2Down);
	Cmd_AddCommand ("-button2", IN_Button2Up);
	Cmd_AddCommand ("+button3", IN_Button3Down);
	Cmd_AddCommand ("-button3", IN_Button3Up);
	Cmd_AddCommand ("+button4", IN_Button4Down);
	Cmd_AddCommand ("-button4", IN_Button4Up);
	Cmd_AddCommand ("+button5", IN_Button5Down);
	Cmd_AddCommand ("-button5", IN_Button5Up);
	Cmd_AddCommand ("+button6", IN_Button6Down);
	Cmd_AddCommand ("-button6", IN_Button6Up);
	Cmd_AddCommand ("+button7", IN_Button7Down);
	Cmd_AddCommand ("-button7", IN_Button7Up);
	Cmd_AddCommand ("+button8", IN_Button8Down);
	Cmd_AddCommand ("-button8", IN_Button8Up);
	Cmd_AddCommand ("+button9", IN_Button9Down);
	Cmd_AddCommand ("-button9", IN_Button9Up);
	Cmd_AddCommand ("+button10", IN_Button10Down);
	Cmd_AddCommand ("-button10", IN_Button10Up);
	Cmd_AddCommand ("+button11", IN_Button11Down);
	Cmd_AddCommand ("-button11", IN_Button11Up);
	Cmd_AddCommand ("+button12", IN_Button12Down);
	Cmd_AddCommand ("-button12", IN_Button12Up);
	Cmd_AddCommand ("+button13", IN_Button13Down);
	Cmd_AddCommand ("-button13", IN_Button13Up);
	Cmd_AddCommand ("+button14", IN_Button14Down);
	Cmd_AddCommand ("-button14", IN_Button14Up);
	Cmd_AddCommand ("+mlook", IN_MLookDown);
	Cmd_AddCommand ("-mlook", IN_MLookUp);

#ifdef USE_VOIP
	Cmd_AddCommand ("+voiprecord", IN_VoipRecordDown);
	Cmd_AddCommand ("-voiprecord", IN_VoipRecordUp);
#endif

#ifdef TA_SPLITVIEW
	Cmd_AddCommand ("2centerview",IN_2CenterView);

	Cmd_AddCommand ("+2moveup",IN_2UpDown);
	Cmd_AddCommand ("-2moveup",IN_2UpUp);
	Cmd_AddCommand ("+2movedown",IN_2DownDown);
	Cmd_AddCommand ("-2movedown",IN_2DownUp);
	Cmd_AddCommand ("+2left",IN_2LeftDown);
	Cmd_AddCommand ("-2left",IN_2LeftUp);
	Cmd_AddCommand ("+2right",IN_2RightDown);
	Cmd_AddCommand ("-2right",IN_2RightUp);
	Cmd_AddCommand ("+2forward",IN_2ForwardDown);
	Cmd_AddCommand ("-2forward",IN_2ForwardUp);
	Cmd_AddCommand ("+2back",IN_2BackDown);
	Cmd_AddCommand ("-2back",IN_2BackUp);
	Cmd_AddCommand ("+2lookup", IN_2LookupDown);
	Cmd_AddCommand ("-2lookup", IN_2LookupUp);
	Cmd_AddCommand ("+2lookdown", IN_2LookdownDown);
	Cmd_AddCommand ("-2lookdown", IN_2LookdownUp);
	Cmd_AddCommand ("+2strafe", IN_2StrafeDown);
	Cmd_AddCommand ("-2strafe", IN_2StrafeUp);
	Cmd_AddCommand ("+2moveleft", IN_2MoveleftDown);
	Cmd_AddCommand ("-2moveleft", IN_2MoveleftUp);
	Cmd_AddCommand ("+2moveright", IN_2MoverightDown);
	Cmd_AddCommand ("-2moveright", IN_2MoverightUp);

#ifndef TURTLEARENA // NO_SPEED_KEY
	Cmd_AddCommand ("+2speed", IN_2SpeedDown);
	Cmd_AddCommand ("-2speed", IN_2SpeedUp);
#endif
#ifdef TURTLEARENA // LOCKON
	Cmd_AddCommand ("+2lockon", IN_2LockonDown);
	Cmd_AddCommand ("-2lockon", IN_2LockonUp);
#endif
	Cmd_AddCommand ("+2attack", IN_2Button0Down);
	Cmd_AddCommand ("-2attack", IN_2Button0Up);
	Cmd_AddCommand ("+2button0", IN_2Button0Down);
	Cmd_AddCommand ("-2button0", IN_2Button0Up);
	Cmd_AddCommand ("+2button1", IN_2Button1Down);
	Cmd_AddCommand ("-2button1", IN_2Button1Up);
	Cmd_AddCommand ("+2button2", IN_2Button2Down);
	Cmd_AddCommand ("-2button2", IN_2Button2Up);
	Cmd_AddCommand ("+2button3", IN_2Button3Down);
	Cmd_AddCommand ("-2button3", IN_2Button3Up);
	Cmd_AddCommand ("+2button4", IN_2Button4Down);
	Cmd_AddCommand ("-2button4", IN_2Button4Up);
	Cmd_AddCommand ("+2button5", IN_2Button5Down);
	Cmd_AddCommand ("-2button5", IN_2Button5Up);
	Cmd_AddCommand ("+2button6", IN_2Button6Down);
	Cmd_AddCommand ("-2button6", IN_2Button6Up);
	Cmd_AddCommand ("+2button7", IN_2Button7Down);
	Cmd_AddCommand ("-2button7", IN_2Button7Up);
	Cmd_AddCommand ("+2button8", IN_2Button8Down);
	Cmd_AddCommand ("-2button8", IN_2Button8Up);
	Cmd_AddCommand ("+2button9", IN_2Button9Down);
	Cmd_AddCommand ("-2button9", IN_2Button9Up);
	Cmd_AddCommand ("+2button10", IN_2Button10Down);
	Cmd_AddCommand ("-2button10", IN_2Button10Up);
	Cmd_AddCommand ("+2button11", IN_2Button11Down);
	Cmd_AddCommand ("-2button11", IN_2Button11Up);
	Cmd_AddCommand ("+2button12", IN_2Button12Down);
	Cmd_AddCommand ("-2button12", IN_2Button12Up);
	Cmd_AddCommand ("+2button13", IN_2Button13Down);
	Cmd_AddCommand ("-2button13", IN_2Button13Up);
	Cmd_AddCommand ("+2button14", IN_2Button14Down);
	Cmd_AddCommand ("-2button14", IN_2Button14Up);

	Cmd_AddCommand ("3centerview",IN_3CenterView);

	Cmd_AddCommand ("+3moveup",IN_3UpDown);
	Cmd_AddCommand ("-3moveup",IN_3UpUp);
	Cmd_AddCommand ("+3movedown",IN_3DownDown);
	Cmd_AddCommand ("-3movedown",IN_3DownUp);
	Cmd_AddCommand ("+3left",IN_3LeftDown);
	Cmd_AddCommand ("-3left",IN_3LeftUp);
	Cmd_AddCommand ("+3right",IN_3RightDown);
	Cmd_AddCommand ("-3right",IN_3RightUp);
	Cmd_AddCommand ("+3forward",IN_3ForwardDown);
	Cmd_AddCommand ("-3forward",IN_3ForwardUp);
	Cmd_AddCommand ("+3back",IN_3BackDown);
	Cmd_AddCommand ("-3back",IN_3BackUp);
	Cmd_AddCommand ("+3lookup", IN_3LookupDown);
	Cmd_AddCommand ("-3lookup", IN_3LookupUp);
	Cmd_AddCommand ("+3lookdown", IN_3LookdownDown);
	Cmd_AddCommand ("-3lookdown", IN_3LookdownUp);
	Cmd_AddCommand ("+3strafe", IN_3StrafeDown);
	Cmd_AddCommand ("-3strafe", IN_3StrafeUp);
	Cmd_AddCommand ("+3moveleft", IN_3MoveleftDown);
	Cmd_AddCommand ("-3moveleft", IN_3MoveleftUp);
	Cmd_AddCommand ("+3moveright", IN_3MoverightDown);
	Cmd_AddCommand ("-3moveright", IN_3MoverightUp);

#ifndef TURTLEARENA // NO_SPEED_KEY
	Cmd_AddCommand ("+3speed", IN_3SpeedDown);
	Cmd_AddCommand ("-3speed", IN_3SpeedUp);
#endif
#ifdef TURTLEARENA // LOCKON
	Cmd_AddCommand ("+3lockon", IN_3LockonDown);
	Cmd_AddCommand ("-3lockon", IN_3LockonUp);
#endif
	Cmd_AddCommand ("+3attack", IN_3Button0Down);
	Cmd_AddCommand ("-3attack", IN_3Button0Up);
	Cmd_AddCommand ("+3button0", IN_3Button0Down);
	Cmd_AddCommand ("-3button0", IN_3Button0Up);
	Cmd_AddCommand ("+3button1", IN_3Button1Down);
	Cmd_AddCommand ("-3button1", IN_3Button1Up);
	Cmd_AddCommand ("+3button2", IN_3Button2Down);
	Cmd_AddCommand ("-3button2", IN_3Button2Up);
	Cmd_AddCommand ("+3button3", IN_3Button3Down);
	Cmd_AddCommand ("-3button3", IN_3Button3Up);
	Cmd_AddCommand ("+3button4", IN_3Button4Down);
	Cmd_AddCommand ("-3button4", IN_3Button4Up);
	Cmd_AddCommand ("+3button5", IN_3Button5Down);
	Cmd_AddCommand ("-3button5", IN_3Button5Up);
	Cmd_AddCommand ("+3button6", IN_3Button6Down);
	Cmd_AddCommand ("-3button6", IN_3Button6Up);
	Cmd_AddCommand ("+3button7", IN_3Button7Down);
	Cmd_AddCommand ("-3button7", IN_3Button7Up);
	Cmd_AddCommand ("+3button8", IN_3Button8Down);
	Cmd_AddCommand ("-3button8", IN_3Button8Up);
	Cmd_AddCommand ("+3button9", IN_3Button9Down);
	Cmd_AddCommand ("-3button9", IN_3Button9Up);
	Cmd_AddCommand ("+3button10", IN_3Button10Down);
	Cmd_AddCommand ("-3button10", IN_3Button10Up);
	Cmd_AddCommand ("+3button11", IN_3Button11Down);
	Cmd_AddCommand ("-3button11", IN_3Button11Up);
	Cmd_AddCommand ("+3button12", IN_3Button12Down);
	Cmd_AddCommand ("-3button12", IN_3Button12Up);
	Cmd_AddCommand ("+3button13", IN_3Button13Down);
	Cmd_AddCommand ("-3button13", IN_3Button13Up);
	Cmd_AddCommand ("+3button14", IN_3Button14Down);
	Cmd_AddCommand ("-3button14", IN_3Button14Up);

	Cmd_AddCommand ("4centerview",IN_4CenterView);

	Cmd_AddCommand ("+4moveup",IN_4UpDown);
	Cmd_AddCommand ("-4moveup",IN_4UpUp);
	Cmd_AddCommand ("+4movedown",IN_4DownDown);
	Cmd_AddCommand ("-4movedown",IN_4DownUp);
	Cmd_AddCommand ("+4left",IN_4LeftDown);
	Cmd_AddCommand ("-4left",IN_4LeftUp);
	Cmd_AddCommand ("+4right",IN_4RightDown);
	Cmd_AddCommand ("-4right",IN_4RightUp);
	Cmd_AddCommand ("+4forward",IN_4ForwardDown);
	Cmd_AddCommand ("-4forward",IN_4ForwardUp);
	Cmd_AddCommand ("+4back",IN_4BackDown);
	Cmd_AddCommand ("-4back",IN_4BackUp);
	Cmd_AddCommand ("+4lookup", IN_4LookupDown);
	Cmd_AddCommand ("-4lookup", IN_4LookupUp);
	Cmd_AddCommand ("+4lookdown", IN_4LookdownDown);
	Cmd_AddCommand ("-4lookdown", IN_4LookdownUp);
	Cmd_AddCommand ("+4strafe", IN_4StrafeDown);
	Cmd_AddCommand ("-4strafe", IN_4StrafeUp);
	Cmd_AddCommand ("+4moveleft", IN_4MoveleftDown);
	Cmd_AddCommand ("-4moveleft", IN_4MoveleftUp);
	Cmd_AddCommand ("+4moveright", IN_4MoverightDown);
	Cmd_AddCommand ("-4moveright", IN_4MoverightUp);

#ifndef TURTLEARENA // NO_SPEED_KEY
	Cmd_AddCommand ("+4speed", IN_4SpeedDown);
	Cmd_AddCommand ("-4speed", IN_4SpeedUp);
#endif
#ifdef TURTLEARENA // LOCKON
	Cmd_AddCommand ("+4lockon", IN_4LockonDown);
	Cmd_AddCommand ("-4lockon", IN_4LockonUp);
#endif
	Cmd_AddCommand ("+4attack", IN_4Button0Down);
	Cmd_AddCommand ("-4attack", IN_4Button0Up);
	Cmd_AddCommand ("+4button0", IN_4Button0Down);
	Cmd_AddCommand ("-4button0", IN_4Button0Up);
	Cmd_AddCommand ("+4button1", IN_4Button1Down);
	Cmd_AddCommand ("-4button1", IN_4Button1Up);
	Cmd_AddCommand ("+4button2", IN_4Button2Down);
	Cmd_AddCommand ("-4button2", IN_4Button2Up);
	Cmd_AddCommand ("+4button3", IN_4Button3Down);
	Cmd_AddCommand ("-4button3", IN_4Button3Up);
	Cmd_AddCommand ("+4button4", IN_4Button4Down);
	Cmd_AddCommand ("-4button4", IN_4Button4Up);
	Cmd_AddCommand ("+4button5", IN_4Button5Down);
	Cmd_AddCommand ("-4button5", IN_4Button5Up);
	Cmd_AddCommand ("+4button6", IN_4Button6Down);
	Cmd_AddCommand ("-4button6", IN_4Button6Up);
	Cmd_AddCommand ("+4button7", IN_4Button7Down);
	Cmd_AddCommand ("-4button7", IN_4Button7Up);
	Cmd_AddCommand ("+4button8", IN_4Button8Down);
	Cmd_AddCommand ("-4button8", IN_4Button8Up);
	Cmd_AddCommand ("+4button9", IN_4Button9Down);
	Cmd_AddCommand ("-4button9", IN_4Button9Up);
	Cmd_AddCommand ("+4button10", IN_4Button10Down);
	Cmd_AddCommand ("-4button10", IN_4Button10Up);
	Cmd_AddCommand ("+4button11", IN_4Button11Down);
	Cmd_AddCommand ("-4button11", IN_4Button11Up);
	Cmd_AddCommand ("+4button12", IN_4Button12Down);
	Cmd_AddCommand ("-4button12", IN_4Button12Up);
	Cmd_AddCommand ("+4button13", IN_4Button13Down);
	Cmd_AddCommand ("-4button13", IN_4Button13Up);
	Cmd_AddCommand ("+4button14", IN_4Button14Down);
	Cmd_AddCommand ("-4button14", IN_4Button14Up);
#endif

	cl_nodelta = Cvar_Get ("cl_nodelta", "0", 0);
	cl_debugMove = Cvar_Get ("cl_debugMove", "0", 0);
}
