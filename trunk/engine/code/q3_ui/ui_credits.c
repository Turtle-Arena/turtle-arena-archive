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
/*
=======================================================================

CREDITS

=======================================================================
*/


#include "ui_local.h"


typedef struct {
	menuframework_s	menu;
	int frame;
} creditsmenu_t;

static creditsmenu_t	s_credits;


/*
===============
UI_CreditMenu_Draw_ioq3
===============
*/
static void UI_CreditMenu_Draw_ioq3( void ) {
	int		y;
	int		i;

	// These are all people that have made commits to Subversion, and thus
	//  probably incomplete.
	// (These are in alphabetical order, for the defense of everyone's egos.)
	static const char *names[] = {
		"Tim Angus",
		"Vincent Cojot",
		"Ryan C. Gordon",
		"Aaron Gyes",
		"Ludwig Nussel",
		"Julian Priestley",
		"Scirocco Six",
		"Thilo Schulz",
		"Zachary J. Slater",
		"Tony J. White",
		"...and many, many others!",  // keep this one last.
		NULL
	};

#ifdef IOQ3ZTM // Center text verticly on screen!
	y = (SCREEN_HEIGHT - ARRAY_LEN(names) * (1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE)) / 2;
#else
	y = 12;
#endif
	UI_DrawProportionalString( 320, y, "ioquake3 contributors:", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;

	for (i = 0; names[i]; i++) {
		UI_DrawProportionalString( 320, y, names[i], UI_CENTER|UI_SMALLFONT, color_white );
		y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	}

	UI_DrawString( 320, 459, "http://www.ioquake3.org/", UI_CENTER|UI_SMALLFONT, color_red );
}


/*
=================
UI_CreditMenu_Key
=================
*/
static sfxHandle_t UI_CreditMenu_Key( int key ) {
	if( key & K_CHAR_FLAG ) {
		return 0;
	}

#ifndef TURTLEARENA
	s_credits.frame++;
	if (s_credits.frame == 1) {
		s_credits.menu.draw = UI_CreditMenu_Draw_ioq3;
	} else
#endif
	{
		trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
	}
	return 0;
}

#ifdef TURTLEARENA
/*
===============
UI_CreditMenu_Draw_Legal
===============
*/
static void UI_CreditMenu_Draw_Legal( void ) {
	int		y;
	int		numItems = 6; // 5 text lines, 1 blank line

	y = (SCREEN_HEIGHT - numItems * (1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE)) / 2;

	// This is a unoffial game
	UI_DrawProportionalString( 320, y, "Turtle Arena developer(s) are in", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "no way affiliated with", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Viacom, Ubisoft, or Mirage Studios.", UI_CENTER|UI_SMALLFONT, color_white );

	// Copyright
	y += (1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE) * 2;
	UI_DrawProportionalString( 320, y, "TMNT(c) Viacom", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Turtle Arena(c) 2009-2011 ZTurtleMan", UI_CENTER|UI_SMALLFONT, color_white );

	// Website
	UI_DrawString( 320, 459, "http://turtlearena.googlecode.com/", UI_CENTER|UI_SMALLFONT, color_red );
}
#endif


/*
===============
UI_CreditMenu_Draw
===============
*/
static void UI_CreditMenu_Draw( void ) {
	int		y;

#ifdef TURTLEARENA
	// Credit id software and ioquake3, and legal stuff.
	y = 12;

	UI_DrawProportionalString( 320, y, "Credits", UI_CENTER, color_white );
	y += 1.42 * PROP_HEIGHT;

	UI_DrawProportionalString( 320, y, "Code", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "id Software, ioquake3, ioSTVEF, OpenArena,", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Xreal, Smokin' Guns, Mercenaries Guild,", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, " Wolf-ET, The Reaction Factory, \"inolan.com\",", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Zack \"ZTurtleMan\" Middleton", UI_CENTER|UI_SMALLFONT, color_white );

	y += (PROP_HEIGHT * PROP_SMALL_SIZE_SCALE) * 2;

	UI_DrawProportionalString( 320, y, "Data", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "OpenArena", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Zack \"ZTurtleMan\" Middleton", UI_CENTER|UI_SMALLFONT, color_white );

	y += (PROP_HEIGHT * PROP_SMALL_SIZE_SCALE) * 2;

	UI_DrawProportionalString( 320, y, "Music", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "The Mulletz - ccmixter.org/people/NeilCrowe/", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "zero-project - zero-project.gr", UI_CENTER|UI_SMALLFONT, color_white );

	y += (PROP_HEIGHT * PROP_SMALL_SIZE_SCALE) * 2;

	UI_DrawProportionalString( 320, y, "Fonts", UI_CENTER|UI_SMALLFONT, color_white );
	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "GNU FreeFonts", UI_CENTER|UI_SMALLFONT, color_white );
#else
	y = 12;
	UI_DrawProportionalString( 320, y, "id Software is:", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Programming", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "John Carmack, Robert A. Duffy, Jim Dose'", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Art", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Adrian Carmack, Kevin Cloud,", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Kenneth Scott, Seneca Menard, Fred Nilsson", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Game Designer", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Graeme Devine", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Level Design", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Tim Willits, Christian Antkow, Paul Jaquays", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "CEO", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Todd Hollenshead", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Director of Business Development", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Marty Stratton", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Biz Assist and id Mom", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Donna Jackson", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Development Assistance", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Eric Webb", UI_CENTER|UI_SMALLFONT, color_white );

	y += 1.35 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawString( 320, y, "To order: 1-800-idgames     www.quake3arena.com     www.idsoftware.com", UI_CENTER|UI_SMALLFONT, color_red );
	y += SMALLCHAR_HEIGHT;
	UI_DrawString( 320, y, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color_red );
#endif
}


/*
===============
UI_CreditMenu

Exit game credits
===============
*/
void UI_CreditMenu( void ) {
#ifndef IOQ3ZTM // I fixed widescreen background, hack no longer needed.
	/* This UI_FillRect() hack will blank the borders if you're in widescreen,
	   so you get a completely black background instead of stripes from the
	   previous frame on each side of the credits.. */
	const float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	UI_FillRect(0 - uis.bias, 0, (640.0f / uis.xscale) * 2.0f, 480.0f / uis.yscale, black);
#endif

	memset( &s_credits, 0 ,sizeof(s_credits) );

#ifdef TURTLEARENA
	s_credits.menu.draw = UI_CreditMenu_Draw_Legal;
#else
	s_credits.menu.draw = UI_CreditMenu_Draw;
#endif
	s_credits.menu.key = UI_CreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	UI_PushMenu ( &s_credits.menu );
}

#ifdef TURTLEARENA // LONG_CREDITS


/*
=================
UI_LongCreditMenu_Key
=================
*/
static sfxHandle_t UI_LongCreditMenu_Key( int key ) {
	if( key & K_CHAR_FLAG ) {
		return 0;
	}

	switch (++s_credits.frame)
	{
		case 1:
			s_credits.menu.draw = UI_CreditMenu_Draw_ioq3;
			break;
		case 2:
			s_credits.menu.draw = UI_CreditMenu_Draw_Legal;
			break;
		default:
			UI_PopMenu();
			break;
	}
	return 0;
}

/*
===============
UI_LongCreditMenu

Long credit screen
===============
*/
void UI_LongCreditMenu( void )
{
	memset( &s_credits, 0 ,sizeof(s_credits) );

	s_credits.menu.draw = UI_CreditMenu_Draw;
	s_credits.menu.key = UI_LongCreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	UI_PushMenu ( &s_credits.menu );
}
#endif
