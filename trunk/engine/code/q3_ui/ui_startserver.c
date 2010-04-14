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
=============================================================================

START SERVER MENU *****

=============================================================================
*/


#include "ui_local.h"


#define GAMESERVER_BACK0		"menu/art/back_0"
#define GAMESERVER_BACK1		"menu/art/back_1"
#define GAMESERVER_NEXT0		"menu/art/next_0"
#define GAMESERVER_NEXT1		"menu/art/next_1"
#define GAMESERVER_FRAMEL		"menu/art/frame2_l"
#define GAMESERVER_FRAMER		"menu/art/frame1_r"
#define GAMESERVER_SELECT		"menu/art/maps_select"
#define GAMESERVER_SELECTED		"menu/art/maps_selected"
#ifdef TA_MISC // NO_MENU_FIGHT
#define GAMESERVER_FIGHT0		"menu/art/play_0"
#define GAMESERVER_FIGHT1		"menu/art/play_1"
#else
#define GAMESERVER_FIGHT0		"menu/art/fight_0"
#define GAMESERVER_FIGHT1		"menu/art/fight_1"
#endif
#define GAMESERVER_UNKNOWNMAP	"menu/art/unknownmap"
#ifdef TA_DATA
#define GAMESERVER_ARROWS		"menu/art/arrows_horz_0"
#define GAMESERVER_ARROWSL		"menu/art/arrows_horz_left"
#define GAMESERVER_ARROWSR		"menu/art/arrows_horz_right"
#define GAMESERVER_ARROWS_WIDTH 128
#define GAMESERVER_ARROWS_HEIGHT 64
#else
#define GAMESERVER_ARROWS		"menu/art/gs_arrows_0"
#define GAMESERVER_ARROWSL		"menu/art/gs_arrows_l"
#define GAMESERVER_ARROWSR		"menu/art/gs_arrows_r"
#endif

#define MAX_MAPROWS		2
#define MAX_MAPCOLS		2
#define MAX_MAPSPERPAGE	4

#define MAX_NAMELENGTH	16
#define ID_GAMETYPE				10
#define ID_PICTURES				11	// 12, 13, 14
#define ID_PREVPAGE				15
#define ID_NEXTPAGE				16
#define ID_STARTSERVERBACK		17
#define ID_STARTSERVERNEXT		18

typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	framel;
	menubitmap_s	framer;

	menulist_s		gametype;
	menubitmap_s	mappics[MAX_MAPSPERPAGE];
	menubitmap_s	mapbuttons[MAX_MAPSPERPAGE];
	menubitmap_s	arrows;
	menubitmap_s	prevpage;
	menubitmap_s	nextpage;
	menubitmap_s	back;
	menubitmap_s	next;

	menutext_s		mapname;
	menubitmap_s	item_null;

	qboolean		multiplayer;
	int				currentmap;
	int				nummaps;
	int				page;
	int				maxpages;
	int 			maplist[MAX_ARENAS];
} startserver_t;

static startserver_t s_startserver;

#ifdef TURTLEARENA
// Gametype names
static const char *gametype_items[] = {
	"Free For All",
	"Duel", // tornament to duel // "Tournament",
	"Cooperative",
	"Team Deathmatch",
	"Capture the Flag",
#ifdef MISSIONPACK
	"1 Flag CTF",
	"Overload",
#ifdef MISSIONPACK_HARVESTER
	"Harvester",
#endif
#endif
	NULL
};

// Order of gametypes in "gametype select list"
static int gametype_remap[] = {GT_FFA, GT_TOURNAMENT, GT_SINGLE_PLAYER, GT_TEAM, GT_CTF
#ifdef MISSIONPACK
,GT_1FCTF,GT_OBELISK
#ifdef MISSIONPACK_HARVESTER
,GT_HARVESTER
#endif
#endif
};

/*
// Order of gametype_items, must
// ZTM: NOTE: Why does this need to be seperate from gametype_remap?
static int gametype_remap2[] = {
	0,		// Free For All
	1,		// Duel
	2,		// Cooperative
	3,		// Team Deathmatch
	4		// Capture the Flag
#ifdef MISSIONPACK
	,5		// 1 Flag CTF
	,6		// Overload
#ifdef MISSIONPACK_HARVESTER
	,7		// Harvester
#endif
#endif
};
*/
#else
static const char *gametype_items[] = {
	"Free For All",
	"Team Deathmatch",
#ifdef TA_MISC // tornament to duel
	"Duel",
#else
	"Tournament",
#endif
	"Capture the Flag",
#ifdef MISSIONPACK
	"1 Flag CTF",
	"Overload",
#ifdef MISSIONPACK_HARVESTER
	"Harvester",
#endif
#endif
	NULL
};

static int gametype_remap[] = {GT_FFA, GT_TEAM, GT_TOURNAMENT, GT_CTF
#ifdef MISSIONPACK
,GT_1FCTF,GT_OBELISK
#ifdef MISSIONPACK_HARVESTER
,GT_HARVESTER
#endif
#endif
};
static int gametype_remap2[] = {0, 2, 0, 1, 3
#ifdef MISSIONPACK
,4,5
#ifdef MISSIONPACK_HARVESTER
,6
#endif
#endif
};
#endif

#ifdef IOQUAKE3 // ZTM: punkbuster
// use ui_servers2.c definition
extern const char* punkbuster_items[];
#endif

static void UI_ServerOptionsMenu( qboolean multiplayer );


/*
=================
GametypeBits
=================
*/
static int GametypeBits( char *string ) {
	int		bits;
	char	*p;
	char	*token;

	bits = 0;
	p = string;
	while( 1 ) {
		token = COM_ParseExt( &p, qfalse );
		if( token[0] == 0 ) {
			break;
		}

		if( Q_stricmp( token, "ffa" ) == 0 ) {
			bits |= 1 << GT_FFA;
			continue;
		}

		if( Q_stricmp( token, "tourney" ) == 0 ) {
			bits |= 1 << GT_TOURNAMENT;
			continue;
		}

		if( Q_stricmp( token, "single" ) == 0 ) {
			bits |= 1 << GT_SINGLE_PLAYER;
			continue;
		}

		if( Q_stricmp( token, "team" ) == 0 ) {
			bits |= 1 << GT_TEAM;
			continue;
		}

		if( Q_stricmp( token, "ctf" ) == 0 ) {
			bits |= 1 << GT_CTF;
			continue;
		}
#ifdef MISSIONPACK // ZTM: Support MISSIONPACK gametypes!
		if( Q_stricmp( token, "oneflag" ) == 0 ) {
			bits |= 1 << GT_1FCTF;
			continue;
		}
		if( Q_stricmp( token, "overload" ) == 0 ) {
			bits |= 1 << GT_OBELISK;
			continue;
		}
#ifdef MISSIONPACK_HARVESTER
		if( Q_stricmp( token, "harvester" ) == 0 ) {
			bits |= 1 << GT_HARVESTER;
			continue;
		}
#endif
#endif
	}

	return bits;
}


/*
=================
StartServer_Update
=================
*/
static void StartServer_Update( void ) {
	int				i;
	int				top;
	static	char	picname[MAX_MAPSPERPAGE][64];
	const char		*info;
	char			mapname[MAX_NAMELENGTH];

	top = s_startserver.page*MAX_MAPSPERPAGE;

	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		if (top+i >= s_startserver.nummaps)
			break;

		info = UI_GetArenaInfoByNumber( s_startserver.maplist[ top + i ]);
		Q_strncpyz( mapname, Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
#ifndef IOQ3ZTM // SUPPORT_LINUX_NO_PAK
		Q_strupr( mapname );
#endif

#ifdef TA_DATA // TEAMARENA_LEVELSHOTS
		Com_sprintf( picname[i], sizeof(picname[i]), "levelshots/%s_small", mapname );
#else
 		Com_sprintf( picname[i], sizeof(picname[i]), "levelshots/%s", mapname );
#endif

		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_startserver.mappics[i].generic.name   = picname[i];
		s_startserver.mappics[i].shader         = 0;

		// reset
		s_startserver.mapbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
		s_startserver.mapbuttons[i].generic.flags &= ~QMF_INACTIVE;
	}

	for (; i<MAX_MAPSPERPAGE; i++)
	{
		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_startserver.mappics[i].generic.name   = NULL;
		s_startserver.mappics[i].shader         = 0;

		// disable
		s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
		s_startserver.mapbuttons[i].generic.flags |= QMF_INACTIVE;
	}


	// no servers to start
	if( !s_startserver.nummaps ) {
		s_startserver.next.generic.flags |= QMF_INACTIVE;

		// set the map name
		strcpy( s_startserver.mapname.string, "NO MAPS FOUND" );
	}
	else {
		// set the highlight
		s_startserver.next.generic.flags &= ~QMF_INACTIVE;
		i = s_startserver.currentmap - top;
		if ( i >=0 && i < MAX_MAPSPERPAGE ) 
		{
			s_startserver.mappics[i].generic.flags    |= QMF_HIGHLIGHT;
#ifndef TA_DATA // MENU
			s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
#endif
		}

		// set the map name
		info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
		Q_strncpyz( s_startserver.mapname.string, Info_ValueForKey( info, "map" ), MAX_NAMELENGTH);
	}
	
//#ifndef IOQ3ZTM // SUPPORT_LINUX_NO_PAK
	Q_strupr( s_startserver.mapname.string );
//#endif
}


/*
=================
StartServer_MapEvent
=================
*/
static void StartServer_MapEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED) {
		return;
	}

	s_startserver.currentmap = (s_startserver.page*MAX_MAPSPERPAGE) + (((menucommon_s*)ptr)->id - ID_PICTURES);
	StartServer_Update();
}


/*
=================
StartServer_GametypeEvent
=================
*/
static void StartServer_GametypeEvent( void* ptr, int event ) {
	int			i;
	int			count;
	int			gamebits;
	int			matchbits;
	const char	*info;

	if( event != QM_ACTIVATED) {
		return;
	}

	count = UI_GetNumArenas();
	s_startserver.nummaps = 0;
	matchbits = 1 << gametype_remap[s_startserver.gametype.curvalue];
#ifndef TA_SP // Single player has own gametype on net now.
	if( gametype_remap[s_startserver.gametype.curvalue] == GT_FFA ) {
		matchbits |= ( 1 << GT_SINGLE_PLAYER );
	}
#endif
	for( i = 0; i < count; i++ ) {
		info = UI_GetArenaInfoByNumber( i );
	
		gamebits = GametypeBits( Info_ValueForKey( info, "type") );
		if( !( gamebits & matchbits ) ) {
			continue;
		}

		s_startserver.maplist[ s_startserver.nummaps ] = i;
		s_startserver.nummaps++;
	}
	s_startserver.maxpages = (s_startserver.nummaps + MAX_MAPSPERPAGE-1)/MAX_MAPSPERPAGE;
	s_startserver.page = 0;
	s_startserver.currentmap = 0;

	StartServer_Update();
}


/*
=================
StartServer_MenuEvent
=================
*/
static void StartServer_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_PREVPAGE:
		if( s_startserver.page > 0 ) {
			s_startserver.page--;
			StartServer_Update();
		}
		break;

	case ID_NEXTPAGE:
		if( s_startserver.page < s_startserver.maxpages - 1 ) {
			s_startserver.page++;
			StartServer_Update();
		}
		break;

	case ID_STARTSERVERNEXT:
		trap_Cvar_SetValue( "g_gameType", gametype_remap[s_startserver.gametype.curvalue] );
#ifdef TA_MISC
		// If ingame, don't go to server options
		if (trap_Cvar_VariableValue("sv_running"))
		{
			// the wait commands will allow the dedicated to take effect
			const char *info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", Info_ValueForKey( info, "map" )));
		}
		else
#endif
		UI_ServerOptionsMenu( s_startserver.multiplayer );
		break;

	case ID_STARTSERVERBACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
StartServer_LevelshotDraw
===============
*/
static void StartServer_LevelshotDraw( void *self ) {
	menubitmap_s	*b;
	int				x;
	int				y;
	int				w;
	int				h;
	int				n;
	const char		*info;

	b = (menubitmap_s *)self;

	if( !b->generic.name ) {
		return;
	}

	if( b->generic.name && !b->shader ) {
		b->shader = trap_R_RegisterShaderNoMip( b->generic.name );
		if( !b->shader && b->errorpic ) {
			b->shader = trap_R_RegisterShaderNoMip( b->errorpic );
		}
	}

	if( b->focuspic && !b->focusshader ) {
		b->focusshader = trap_R_RegisterShaderNoMip( b->focuspic );
	}

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height;
	if( b->shader ) {
		UI_DrawHandlePic( x, y, w, h, b->shader );
	}

	x = b->generic.x;
	y = b->generic.y + b->height;
	UI_FillRect( x, y, b->width, 28, colorBlack );

	x += b->width / 2;
	y += 4;
	n = s_startserver.page * MAX_MAPSPERPAGE + b->generic.id - ID_PICTURES;

	info = UI_GetArenaInfoByNumber( s_startserver.maplist[ n ]);
	UI_DrawString( x, y, Info_ValueForKey( info, "map" ), UI_CENTER|UI_SMALLFONT, color_orange );

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height + 28;
	if( b->generic.flags & QMF_HIGHLIGHT ) {	
		UI_DrawHandlePic( x, y, w, h, b->focusshader );
	}
}


/*
=================
StartServer_MenuInit
=================
*/
static void StartServer_MenuInit( void ) {
	int	i;
	int	x;
	int	y;
	static char mapnamebuffer[64];
#ifdef TA_MISC
	int inGame = trap_Cvar_VariableValue("sv_running");
#endif

	// zero set all our globals
	memset( &s_startserver, 0 ,sizeof(startserver_t) );

	StartServer_Cache();

	s_startserver.menu.wrapAround = qtrue;
#ifdef TA_MISC
	if (!inGame)
#endif
	s_startserver.menu.fullscreen = qtrue;

	s_startserver.banner.generic.type  = MTYPE_BTEXT;
	s_startserver.banner.generic.x	   = 320;
	s_startserver.banner.generic.y	   = 16;
#ifdef TA_MISC
	if (inGame)
		s_startserver.banner.string    = "CHANGE MAP";
	else
#endif
	s_startserver.banner.string        = "GAME SERVER";
	s_startserver.banner.color         = text_banner_color;
	s_startserver.banner.style         = UI_CENTER;

	s_startserver.framel.generic.type  = MTYPE_BITMAP;
	s_startserver.framel.generic.name  = GAMESERVER_FRAMEL;
	s_startserver.framel.generic.flags = QMF_INACTIVE;
	s_startserver.framel.generic.x	   = 0;  
	s_startserver.framel.generic.y	   = 78;
	s_startserver.framel.width  	   = 256;
	s_startserver.framel.height  	   = 329;

	s_startserver.framer.generic.type  = MTYPE_BITMAP;
	s_startserver.framer.generic.name  = GAMESERVER_FRAMER;
	s_startserver.framer.generic.flags = QMF_INACTIVE;
	s_startserver.framer.generic.x	   = 376;
	s_startserver.framer.generic.y	   = 76;
	s_startserver.framer.width  	   = 256;
	s_startserver.framer.height  	   = 334;

	s_startserver.gametype.generic.type		= MTYPE_SPINCONTROL;
	s_startserver.gametype.generic.name		= "Game Type:";
	s_startserver.gametype.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_startserver.gametype.generic.callback	= StartServer_GametypeEvent;
	s_startserver.gametype.generic.id		= ID_GAMETYPE;
	s_startserver.gametype.generic.x		= 320 - 24;
	s_startserver.gametype.generic.y		= 368;
	s_startserver.gametype.itemnames		= gametype_items;
#ifdef TA_MISC
	if (inGame)
		s_startserver.gametype.curvalue		= trap_Cvar_VariableValue("g_gameType");
#endif

	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		x =	(i % MAX_MAPCOLS) * (128+8) + 188;
		y = (i / MAX_MAPROWS) * (128+8) + 96;

		s_startserver.mappics[i].generic.type   = MTYPE_BITMAP;
		s_startserver.mappics[i].generic.flags  = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
		s_startserver.mappics[i].generic.x	    = x;
		s_startserver.mappics[i].generic.y	    = y;
		s_startserver.mappics[i].generic.id		= ID_PICTURES+i;
		s_startserver.mappics[i].width  		= 128;
		s_startserver.mappics[i].height  	    = 96;
		s_startserver.mappics[i].focuspic       = GAMESERVER_SELECTED;
		s_startserver.mappics[i].errorpic       = GAMESERVER_UNKNOWNMAP;
		s_startserver.mappics[i].generic.ownerdraw = StartServer_LevelshotDraw;

		s_startserver.mapbuttons[i].generic.type     = MTYPE_BITMAP;
		s_startserver.mapbuttons[i].generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_NODEFAULTINIT;
		s_startserver.mapbuttons[i].generic.id       = ID_PICTURES+i;
		s_startserver.mapbuttons[i].generic.callback = StartServer_MapEvent;
		s_startserver.mapbuttons[i].generic.x	     = x - 30;
		s_startserver.mapbuttons[i].generic.y	     = y - 32;
		s_startserver.mapbuttons[i].width  		     = 256;
		s_startserver.mapbuttons[i].height  	     = 248;
		s_startserver.mapbuttons[i].generic.left     = x;
		s_startserver.mapbuttons[i].generic.top  	 = y;
		s_startserver.mapbuttons[i].generic.right    = x + 128;
		s_startserver.mapbuttons[i].generic.bottom   = y + 128;
		s_startserver.mapbuttons[i].focuspic         = GAMESERVER_SELECT;
	}

	s_startserver.arrows.generic.type  = MTYPE_BITMAP;
	s_startserver.arrows.generic.name  = GAMESERVER_ARROWS;
	s_startserver.arrows.generic.flags = QMF_INACTIVE;
	s_startserver.arrows.generic.x	   = 260;
	s_startserver.arrows.generic.y	   = 400;
#ifdef TA_DATA
	s_startserver.arrows.width  	   = GAMESERVER_ARROWS_WIDTH;
	s_startserver.arrows.height  	   = GAMESERVER_ARROWS_HEIGHT;
#else
	s_startserver.arrows.width  	   = 128;
	s_startserver.arrows.height  	   = 32;
#endif

	s_startserver.prevpage.generic.type	    = MTYPE_BITMAP;
	s_startserver.prevpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.prevpage.generic.callback = StartServer_MenuEvent;
	s_startserver.prevpage.generic.id	    = ID_PREVPAGE;
	s_startserver.prevpage.generic.x		= 260;
	s_startserver.prevpage.generic.y		= 400;
#ifdef TA_DATA
	s_startserver.prevpage.width  		    = GAMESERVER_ARROWS_WIDTH/2;
	s_startserver.prevpage.height  		    = GAMESERVER_ARROWS_HEIGHT;
#else
	s_startserver.prevpage.width  		    = 64;
	s_startserver.prevpage.height  		    = 32;
#endif
	s_startserver.prevpage.focuspic         = GAMESERVER_ARROWSL;

	s_startserver.nextpage.generic.type	    = MTYPE_BITMAP;
	s_startserver.nextpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.nextpage.generic.callback = StartServer_MenuEvent;
	s_startserver.nextpage.generic.id	    = ID_NEXTPAGE;
#ifdef TA_DATA
	s_startserver.nextpage.generic.x		= 260+GAMESERVER_ARROWS_WIDTH/2;
#else
	s_startserver.nextpage.generic.x		= 321;
#endif
	s_startserver.nextpage.generic.y		= 400;
#ifdef TA_DATA
	s_startserver.nextpage.width  		    = GAMESERVER_ARROWS_WIDTH/2;
	s_startserver.nextpage.height  		    = GAMESERVER_ARROWS_HEIGHT;
#else
	s_startserver.nextpage.width  		    = 64;
	s_startserver.nextpage.height  		    = 32;
#endif
	s_startserver.nextpage.focuspic         = GAMESERVER_ARROWSR;

	s_startserver.mapname.generic.type  = MTYPE_PTEXT;
	s_startserver.mapname.generic.flags = QMF_CENTER_JUSTIFY|QMF_INACTIVE;
	s_startserver.mapname.generic.x	    = 320;
	s_startserver.mapname.generic.y	    = 440;
	s_startserver.mapname.string        = mapnamebuffer;
	s_startserver.mapname.style         = UI_CENTER|UI_BIGFONT;
	s_startserver.mapname.color         = text_color_normal;

	s_startserver.back.generic.type	    = MTYPE_BITMAP;
	s_startserver.back.generic.name     = GAMESERVER_BACK0;
	s_startserver.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.back.generic.callback = StartServer_MenuEvent;
	s_startserver.back.generic.id	    = ID_STARTSERVERBACK;
	s_startserver.back.generic.x		= 0;
	s_startserver.back.generic.y		= 480-64;
	s_startserver.back.width  		    = 128;
	s_startserver.back.height  		    = 64;
	s_startserver.back.focuspic         = GAMESERVER_BACK1;

	s_startserver.next.generic.type	    = MTYPE_BITMAP;
	s_startserver.next.generic.name     = GAMESERVER_NEXT0;
	s_startserver.next.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.next.generic.callback = StartServer_MenuEvent;
	s_startserver.next.generic.id	    = ID_STARTSERVERNEXT;
	s_startserver.next.generic.x		= 640;
	s_startserver.next.generic.y		= 480-64;
	s_startserver.next.width  		    = 128;
	s_startserver.next.height  		    = 64;
	s_startserver.next.focuspic         = GAMESERVER_NEXT1;

	s_startserver.item_null.generic.type	= MTYPE_BITMAP;
	s_startserver.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
	s_startserver.item_null.generic.x		= 0;
	s_startserver.item_null.generic.y		= 0;
	s_startserver.item_null.width			= 640;
	s_startserver.item_null.height			= 480;

	Menu_AddItem( &s_startserver.menu, &s_startserver.banner );
	Menu_AddItem( &s_startserver.menu, &s_startserver.framel );
	Menu_AddItem( &s_startserver.menu, &s_startserver.framer );

	Menu_AddItem( &s_startserver.menu, &s_startserver.gametype );
	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		Menu_AddItem( &s_startserver.menu, &s_startserver.mappics[i] );
		Menu_AddItem( &s_startserver.menu, &s_startserver.mapbuttons[i] );
	}

	Menu_AddItem( &s_startserver.menu, &s_startserver.arrows );
	Menu_AddItem( &s_startserver.menu, &s_startserver.prevpage );
	Menu_AddItem( &s_startserver.menu, &s_startserver.nextpage );
	Menu_AddItem( &s_startserver.menu, &s_startserver.back );
	Menu_AddItem( &s_startserver.menu, &s_startserver.next );
	Menu_AddItem( &s_startserver.menu, &s_startserver.mapname );
	Menu_AddItem( &s_startserver.menu, &s_startserver.item_null );

	StartServer_GametypeEvent( NULL, QM_ACTIVATED );
}


/*
=================
StartServer_Cache
=================
*/
void StartServer_Cache( void )
{
	int				i;
	const char		*info;
	qboolean		precache;
	char			picname[64];
	char			mapname[ MAX_NAMELENGTH ];

	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );	
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );	
	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT0 );	
	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT1 );	
	trap_R_RegisterShaderNoMip( GAMESERVER_FRAMEL );	
	trap_R_RegisterShaderNoMip( GAMESERVER_FRAMER );	
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );	
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECTED );	
	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWS );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSL );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSR );

	precache = trap_Cvar_VariableValue("com_buildscript");

	if( precache ) {
		for( i = 0; i < UI_GetNumArenas(); i++ ) {
			info = UI_GetArenaInfoByNumber( i );
			Q_strncpyz( mapname, Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
#ifndef IOQ3ZTM // SUPPORT_LINUX_NO_PAK
			Q_strupr( mapname );
#endif
	
#ifdef TA_DATA // TEAMARENA_LEVELSHOTS
			Com_sprintf( picname, sizeof(picname), "levelshots/%s_small", mapname );
#else
			Com_sprintf( picname, sizeof(picname), "levelshots/%s", mapname );
#endif
			trap_R_RegisterShaderNoMip(picname);
		}
	}
}


/*
=================
UI_StartServerMenu
=================
*/
void UI_StartServerMenu( qboolean multiplayer ) {
	StartServer_MenuInit();
	s_startserver.multiplayer = multiplayer;
	UI_PushMenu( &s_startserver.menu );
}



/*
=============================================================================

SERVER OPTIONS MENU *****

=============================================================================
*/

#define ID_PLAYER_TYPE			20
#define ID_MAXCLIENTS			21
#define ID_DEDICATED			22
#define ID_GO					23
#define ID_BACK					24

#define PLAYER_SLOTS			12


typedef struct {
	menuframework_s		menu;

	menutext_s			banner;

	menubitmap_s		mappic;
	menubitmap_s		picframe;

	menulist_s			dedicated;
	menufield_s			timelimit;
	menufield_s			fraglimit;
	menufield_s			flaglimit;
	menuradiobutton_s	friendlyfire;
	menufield_s			hostname;
	menuradiobutton_s	pure;
	menulist_s			botSkill;

	menutext_s			player0;
	menulist_s			playerType[PLAYER_SLOTS];
	menutext_s			playerName[PLAYER_SLOTS];
	menulist_s			playerTeam[PLAYER_SLOTS];

	menubitmap_s		go;
	menubitmap_s		next;
	menubitmap_s		back;

	qboolean			multiplayer;
	int					gametype;
	char				mapnamebuffer[32];
	char				playerNameBuffers[PLAYER_SLOTS][16];

	qboolean			newBot;
	int					newBotIndex;
	char				newBotName[16];
	
#ifdef IOQUAKE3 // ZTM: punkbuster
	menulist_s		punkbuster;
#endif
} serveroptions_t;

static serveroptions_t s_serveroptions;

static const char *dedicated_list[] = {
	"No",
	"LAN",
	"Internet",
	NULL
};

static const char *playerType_list[] = {
	"Open",
	"Bot",
	"----",
	NULL
};

static const char *playerTeam_list[] = {
	"Blue",
	"Red",
	NULL
};

static const char *botSkill_list[] = {
#ifdef TA_MISC // rip off SRB2 skills...
	"Easy",
	"Normal",
	"Hard",
	"Very Hard",
	"Ultimate",
	NULL
#else
	"I Can Win",
	"Bring It On",
	"Hurt Me Plenty",
	"Hardcore",
	"Nightmare!",
	NULL
#endif
};


/*
=================
BotAlreadySelected
=================
*/
static qboolean BotAlreadySelected( const char *checkName ) {
	int		n;

	for( n = 1; n < PLAYER_SLOTS; n++ ) {
		if( s_serveroptions.playerType[n].curvalue != 1 ) {
			continue;
		}
		if( (s_serveroptions.gametype >= GT_TEAM) &&
			(s_serveroptions.playerTeam[n].curvalue != s_serveroptions.playerTeam[s_serveroptions.newBotIndex].curvalue ) ) {
			continue;
		}
		if( Q_stricmp( checkName, s_serveroptions.playerNameBuffers[n] ) == 0 ) {
			return qtrue;
		}
	}

	return qfalse;
}


/*
=================
ServerOptions_Start
=================
*/
static void ServerOptions_Start( void ) {
	int		timelimit;
	int		fraglimit;
	int		maxclients;
	int		dedicated;
	int		friendlyfire;
	int		flaglimit;
	int		pure;
	int		skill;
	int		n;
	char	buf[64];
	const char *info;

	timelimit	 = atoi( s_serveroptions.timelimit.field.buffer );
	fraglimit	 = atoi( s_serveroptions.fraglimit.field.buffer );
	flaglimit	 = atoi( s_serveroptions.flaglimit.field.buffer );
	dedicated	 = s_serveroptions.dedicated.curvalue;
	friendlyfire = s_serveroptions.friendlyfire.curvalue;
	pure		 = s_serveroptions.pure.curvalue;
	skill		 = s_serveroptions.botSkill.curvalue + 1;

	//set maxclients
	for( n = 0, maxclients = 0; n < PLAYER_SLOTS; n++ ) {
		if( s_serveroptions.playerType[n].curvalue == 2 ) {
			continue;
		}
		if( (s_serveroptions.playerType[n].curvalue == 1) && (s_serveroptions.playerNameBuffers[n][0] == 0) ) {
			continue;
		}
		maxclients++;
	}

#ifdef TA_SP
	// Don't allow clients to join in non-multiplayer.
	if (!s_serveroptions.multiplayer)
		trap_Cvar_SetValue( "ui_singlePlayerActive", 2 );
	else
		trap_Cvar_SetValue( "ui_singlePlayerActive", 0 );
#endif

	switch( s_serveroptions.gametype ) {
	case GT_FFA:
	default:
#ifdef TA_MISC // frag to score
		trap_Cvar_SetValue( "ui_ffa_scorelimit", fraglimit );
#else
		trap_Cvar_SetValue( "ui_ffa_fraglimit", fraglimit );
#endif
		trap_Cvar_SetValue( "ui_ffa_timelimit", timelimit );
		break;

	case GT_TOURNAMENT:
#ifdef TA_MISC // frag to score
		trap_Cvar_SetValue( "ui_tourney_scorelimit", fraglimit );
#else
		trap_Cvar_SetValue( "ui_tourney_fraglimit", fraglimit );
#endif
		trap_Cvar_SetValue( "ui_tourney_timelimit", timelimit );
		break;

#ifdef TA_SP
	case GT_SINGLE_PLAYER:
		// Co-op settings
		break;
#endif

	case GT_TEAM:
#ifdef TA_MISC // frag to score
		trap_Cvar_SetValue( "ui_team_scorelimit", fraglimit );
#else
		trap_Cvar_SetValue( "ui_team_fraglimit", fraglimit );
#endif
		trap_Cvar_SetValue( "ui_team_timelimit", timelimit );
#ifdef IOQ3ZTM // IOQ3BUGFIX: Miss named cvar
		trap_Cvar_SetValue( "ui_team_friendly", friendlyfire );
#else
		trap_Cvar_SetValue( "ui_team_friendlt", friendlyfire );
#endif
		break;

	case GT_CTF:
#ifdef IOQ3ZTM // IOQ3BUGFIX: Wrong name used for Cvar "ui_ctf_fraglimit", also used fRaglimit not fLaglimit
		trap_Cvar_SetValue( "ui_ctf_capturelimit", flaglimit );
#else
		trap_Cvar_SetValue( "ui_ctf_fraglimit", fraglimit );
#endif
		trap_Cvar_SetValue( "ui_ctf_timelimit", timelimit );
#ifdef IOQ3ZTM // IOQ3BUGFIX: Miss named cvar
		trap_Cvar_SetValue( "ui_ctf_friendly", friendlyfire );
#else
		trap_Cvar_SetValue( "ui_ctf_friendlt", friendlyfire );
#endif
		break;

#ifdef MISSIONPACK // MP_GAMETYPES
	case GT_1FCTF:
		trap_Cvar_SetValue( "ui_1flag_capturelimit", flaglimit );
		trap_Cvar_SetValue( "ui_1flag_timelimit", timelimit );
		trap_Cvar_SetValue( "ui_1flag_friendly", friendlyfire );
		break;

	case GT_OBELISK:
		trap_Cvar_SetValue( "ui_obelisk_capturelimit", flaglimit );
		trap_Cvar_SetValue( "ui_obelisk_timelimit", timelimit );
		trap_Cvar_SetValue( "ui_obelisk_friendly", friendlyfire );
		break;

#ifdef MISSIONPACK_HARVESTER
	case GT_HARVESTER:
		trap_Cvar_SetValue( "ui_harvester_capturelimit", flaglimit );
		trap_Cvar_SetValue( "ui_harvester_timelimit", timelimit );
		trap_Cvar_SetValue( "ui_harvester_friendly", friendlyfire );
		break;
#endif
#endif
	}

	trap_Cvar_SetValue( "sv_maxclients", Com_Clamp( 0, 12, maxclients ) );
	trap_Cvar_SetValue( "dedicated", Com_Clamp( 0, 2, dedicated ) );
	trap_Cvar_SetValue ("timelimit", Com_Clamp( 0, timelimit, timelimit ) );
#ifdef TA_MISC // frag to score
	trap_Cvar_SetValue ("scorelimit", Com_Clamp( 0, fraglimit, fraglimit ) );
#else
	trap_Cvar_SetValue ("fraglimit", Com_Clamp( 0, fraglimit, fraglimit ) );
#endif
	trap_Cvar_SetValue ("capturelimit", Com_Clamp( 0, flaglimit, flaglimit ) );
	trap_Cvar_SetValue( "g_friendlyfire", friendlyfire );
	trap_Cvar_SetValue( "sv_pure", pure );
	trap_Cvar_Set("sv_hostname", s_serveroptions.hostname.field.buffer );
	
#ifdef IOQUAKE3 // ZTM: punkbuster
	trap_Cvar_SetValue( "sv_punkbuster", s_serveroptions.punkbuster.curvalue );
#endif

	// the wait commands will allow the dedicated to take effect
	info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", Info_ValueForKey( info, "map" )));

	// add bots
	trap_Cmd_ExecuteText( EXEC_APPEND, "wait 3\n" );
	for( n = 1; n < PLAYER_SLOTS; n++ ) {
		if( s_serveroptions.playerType[n].curvalue != 1 ) {
			continue;
		}
		if( s_serveroptions.playerNameBuffers[n][0] == 0 ) {
			continue;
		}
#ifndef RANDOMBOT
		if( s_serveroptions.playerNameBuffers[n][0] == '-' ) {
			continue;
		}
#endif
		if( s_serveroptions.gametype >= GT_TEAM ) {
			Com_sprintf( buf, sizeof(buf), "addbot %s %i %s\n", s_serveroptions.playerNameBuffers[n], skill,
				playerTeam_list[s_serveroptions.playerTeam[n].curvalue] );
		}
		else {
			Com_sprintf( buf, sizeof(buf), "addbot %s %i\n", s_serveroptions.playerNameBuffers[n], skill );
		}
		trap_Cmd_ExecuteText( EXEC_APPEND, buf );
	}

	// set player's team
	if( dedicated == 0 && s_serveroptions.gametype >= GT_TEAM ) {
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait 5; team %s\n", playerTeam_list[s_serveroptions.playerTeam[0].curvalue] ) );
	}
}


/*
=================
ServerOptions_InitPlayerItems
=================
*/
static void ServerOptions_InitPlayerItems( void ) {
	int		n;
	int		v;

	// init types
	if( s_serveroptions.multiplayer ) {
		v = 0;	// open
	}
	else {
		v = 1;	// bot
	}
	
	for( n = 0; n < PLAYER_SLOTS; n++ ) {
		s_serveroptions.playerType[n].curvalue = v;
	}

	if( s_serveroptions.multiplayer && (s_serveroptions.gametype < GT_TEAM) ) {
		for( n = 8; n < PLAYER_SLOTS; n++ ) {
			s_serveroptions.playerType[n].curvalue = 2;
		}
	}

	// if not a dedicated server, first slot is reserved for the human on the server
	if( s_serveroptions.dedicated.curvalue == 0 ) {
		// human
#ifdef TA_SP // SPMODEL
		s_serveroptions.playerType[0].curvalue = 0;
		if (!s_serveroptions.multiplayer)
		{
			trap_Cvar_VariableStringBuffer( "spmodel", s_serveroptions.playerNameBuffers[0], sizeof(s_serveroptions.playerNameBuffers[0]) );
			s_serveroptions.playerNameBuffers[0][0] = toupper(s_serveroptions.playerNameBuffers[0][0]);
		}
		else
		{
			s_serveroptions.playerType[0].generic.flags |= QMF_INACTIVE;
			trap_Cvar_VariableStringBuffer( "name", s_serveroptions.playerNameBuffers[0], sizeof(s_serveroptions.playerNameBuffers[0]) );
		}
#else
		s_serveroptions.playerType[0].generic.flags |= QMF_INACTIVE;
		s_serveroptions.playerType[0].curvalue = 0;
		trap_Cvar_VariableStringBuffer( "name", s_serveroptions.playerNameBuffers[0], sizeof(s_serveroptions.playerNameBuffers[0]) );
#endif
		Q_CleanStr( s_serveroptions.playerNameBuffers[0] );
	}

	// init teams
	if( s_serveroptions.gametype >= GT_TEAM ) {
		for( n = 0; n < (PLAYER_SLOTS / 2); n++ ) {
			s_serveroptions.playerTeam[n].curvalue = 0;
		}
		for( ; n < PLAYER_SLOTS; n++ ) {
			s_serveroptions.playerTeam[n].curvalue = 1;
		}
	}
	else {
		for( n = 0; n < PLAYER_SLOTS; n++ ) {
			s_serveroptions.playerTeam[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
		}
	}
}


/*
=================
ServerOptions_SetPlayerItems
=================
*/
static void ServerOptions_SetPlayerItems( void ) {
	int		start;
	int		n;

	// types
//	for( n = 0; n < PLAYER_SLOTS; n++ ) {
//		if( (!s_serveroptions.multiplayer) && (n > 0) && (s_serveroptions.playerType[n].curvalue == 0) ) {
//			s_serveroptions.playerType[n].curvalue = 1;
//		}
//	}

	// names
	if( s_serveroptions.dedicated.curvalue == 0 ) {
		s_serveroptions.player0.string = "Human";
#ifdef TA_SP
		if (!s_serveroptions.multiplayer)
			s_serveroptions.playerName[0].generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
		else
#endif
		s_serveroptions.playerName[0].generic.flags &= ~QMF_HIDDEN;

		start = 1;
	}
	else {
		s_serveroptions.player0.string = "Open";
		start = 0;
	}
	for( n = start; n < PLAYER_SLOTS; n++ ) {
		if( s_serveroptions.playerType[n].curvalue == 1 ) {
			s_serveroptions.playerName[n].generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
		}
		else {
			s_serveroptions.playerName[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
		}
	}

	// teams
	if( s_serveroptions.gametype < GT_TEAM ) {
		return;
	}
	for( n = start; n < PLAYER_SLOTS; n++ ) {
		if( s_serveroptions.playerType[n].curvalue == 2 ) {
			s_serveroptions.playerTeam[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
		}
		else {
			s_serveroptions.playerTeam[n].generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
		}
	}
}


/*
=================
ServerOptions_Event
=================
*/
static void ServerOptions_Event( void* ptr, int event ) {
	switch( ((menucommon_s*)ptr)->id ) {
	
	//if( event != QM_ACTIVATED && event != QM_LOSTFOCUS) {
	//	return;
	//}
	case ID_PLAYER_TYPE:
		if( event != QM_ACTIVATED ) {
			break;
		}
		ServerOptions_SetPlayerItems();
		break;

	case ID_MAXCLIENTS:
	case ID_DEDICATED:
		ServerOptions_SetPlayerItems();
		break;
	case ID_GO:
		if( event != QM_ACTIVATED ) {
			break;
		}
		ServerOptions_Start();
		break;

	case ID_STARTSERVERNEXT:
		if( event != QM_ACTIVATED ) {
			break;
		}
		break;
	case ID_BACK:
		if( event != QM_ACTIVATED ) {
			break;
		}
		UI_PopMenu();
		break;
	}
}


static void ServerOptions_PlayerNameEvent( void* ptr, int event ) {
	int		n;

	if( event != QM_ACTIVATED ) {
		return;
	}
	n = ((menutext_s*)ptr)->generic.id;
#ifdef TA_SP
	if (s_serveroptions.dedicated.curvalue == 0 && n == 0)
	{
		int i;

		for (i = 0; i < NUM_SPPLAYERS; i++)
		{
			if (Q_stricmp(s_serveroptions.playerNameBuffers[0], spPlayerNames[i]) == 0)
			{
				n = (i+1)%NUM_SPPLAYERS; // next model
			}
		}

		trap_Cvar_Set("spmodel", spPlayerNames[n]);
		strcpy(s_serveroptions.playerNameBuffers[0], spPlayerNames[n]);
		s_serveroptions.playerNameBuffers[0][0] = toupper(spPlayerNames[n][0]);
		return;
	}
#endif
	s_serveroptions.newBotIndex = n;
	UI_BotSelectMenu( s_serveroptions.playerNameBuffers[n] );
}


/*
=================
ServerOptions_StatusBar
=================
*/
static void ServerOptions_StatusBar( void* ptr ) {
	switch( ((menucommon_s*)ptr)->id ) {
	default:
		UI_DrawString( 320, 440, "0 = NO LIMIT", UI_CENTER|UI_SMALLFONT, colorWhite );
		break;
	}
}


/*
===============
ServerOptions_LevelshotDraw
===============
*/
static void ServerOptions_LevelshotDraw( void *self ) {
	menubitmap_s	*b;
	int				x;
	int				y;

	// strange place for this, but it works
	if( s_serveroptions.newBot ) {
		Q_strncpyz( s_serveroptions.playerNameBuffers[s_serveroptions.newBotIndex], s_serveroptions.newBotName, 16 );
		s_serveroptions.newBot = qfalse;
	}

	b = (menubitmap_s *)self;

	Bitmap_Draw( b );

	x = b->generic.x;
	y = b->generic.y + b->height;
	UI_FillRect( x, y, b->width, 40, colorBlack );

	x += b->width / 2;
	y += 4;
	UI_DrawString( x, y, s_serveroptions.mapnamebuffer, UI_CENTER|UI_SMALLFONT, color_orange );

	y += SMALLCHAR_HEIGHT;
#ifdef TURTLEARENA
	UI_DrawString( x, y, gametype_items[gametype_remap[s_serveroptions.gametype]], UI_CENTER|UI_SMALLFONT, color_orange );
#else
	UI_DrawString( x, y, gametype_items[gametype_remap2[s_serveroptions.gametype]], UI_CENTER|UI_SMALLFONT, color_orange );
#endif
}


static void ServerOptions_InitBotNames( void ) {
	int			count;
	int			n;
	const char	*arenaInfo;
	const char	*botInfo;
	char		*p;
	char		*bot;
	char		bots[MAX_INFO_STRING];

	if( s_serveroptions.gametype >= GT_TEAM ) {
#ifdef IOQ3ZTM
		// set the rest of the bot slots to "---"
		for( n = 1; n < PLAYER_SLOTS; n++ ) {
#ifdef RANDOMBOT
			strcpy( s_serveroptions.playerNameBuffers[n], "Random" );
#else
			strcpy( s_serveroptions.playerNameBuffers[n], "--------" );
#endif
		}
#endif
#ifdef TA_MISC // DEFAULT_PLAYER
		Q_strncpyz( s_serveroptions.playerNameBuffers[1], "Random", 16 ); // RaphBlue
		Q_strncpyz( s_serveroptions.playerNameBuffers[2], "Random", 16 ); // RaphBlue
		if( s_serveroptions.gametype == GT_TEAM ) {
			Q_strncpyz( s_serveroptions.playerNameBuffers[3], "Random", 16 ); // RaphBlue
		}
#else
		Q_strncpyz( s_serveroptions.playerNameBuffers[1], "grunt", 16 );
		Q_strncpyz( s_serveroptions.playerNameBuffers[2], "major", 16 );
		if( s_serveroptions.gametype == GT_TEAM ) {
			Q_strncpyz( s_serveroptions.playerNameBuffers[3], "visor", 16 );
		}
#endif
		else {
			s_serveroptions.playerType[3].curvalue = 2;
		}
		s_serveroptions.playerType[4].curvalue = 2;
		s_serveroptions.playerType[5].curvalue = 2;

#ifdef TA_MISC // DEFAULT_PLAYER
		Q_strncpyz( s_serveroptions.playerNameBuffers[6], "Random", 16 ); // RaphRed
		Q_strncpyz( s_serveroptions.playerNameBuffers[7], "Random", 16 ); // RaphRed
		Q_strncpyz( s_serveroptions.playerNameBuffers[8], "Random", 16 ); // RaphRed
		if( s_serveroptions.gametype == GT_TEAM ) {
			Q_strncpyz( s_serveroptions.playerNameBuffers[9], "Random", 16 ); // RaphRed
		}
#else
		Q_strncpyz( s_serveroptions.playerNameBuffers[6], "sarge", 16 );
		Q_strncpyz( s_serveroptions.playerNameBuffers[7], "grunt", 16 );
		Q_strncpyz( s_serveroptions.playerNameBuffers[8], "major", 16 );
		if( s_serveroptions.gametype == GT_TEAM ) {
			Q_strncpyz( s_serveroptions.playerNameBuffers[9], "visor", 16 );
		}
#endif
		else {
			s_serveroptions.playerType[9].curvalue = 2;
		}
		s_serveroptions.playerType[10].curvalue = 2;
		s_serveroptions.playerType[11].curvalue = 2;

		return;
	}

	count = 1;	// skip the first slot, reserved for a human

	// get info for this map
	arenaInfo = UI_GetArenaInfoByMap( s_serveroptions.mapnamebuffer );

	// get the bot info - we'll seed with them if any are listed
	Q_strncpyz( bots, Info_ValueForKey( arenaInfo, "bots" ), sizeof(bots) );
	p = &bots[0];
	while( *p && count < PLAYER_SLOTS ) {
		//skip spaces
		while( *p && *p == ' ' ) {
			p++;
		}
		if( !p ) {
			break;
		}

		// mark start of bot name
		bot = p;

		// skip until space of null
		while( *p && *p != ' ' ) {
			p++;
		}
		if( *p ) {
			*p++ = 0;
		}

		botInfo = UI_GetBotInfoByName( bot );
		if( !botInfo )
		{
			botInfo = UI_GetBotInfoByNumber( count );
		}
		bot = Info_ValueForKey( botInfo, "name" );

		Q_strncpyz( s_serveroptions.playerNameBuffers[count], bot, sizeof(s_serveroptions.playerNameBuffers[count]) );
		count++;
	}

	// set the rest of the bot slots to "---"
	for( n = count; n < PLAYER_SLOTS; n++ ) {
#ifdef RANDOMBOT
		strcpy( s_serveroptions.playerNameBuffers[n], "Random" );
#else
		strcpy( s_serveroptions.playerNameBuffers[n], "--------" );
#endif
	}

#ifdef RANDOMBOT
	// If no bots, open 3 Random bots.
	if (count == 1)
	{
		for( ;count < 4; count++ ) {
			s_serveroptions.playerType[count].curvalue = 1;
		}
	}
#endif

	// pad up to #8 as open slots
	for( ;count < 8; count++ ) {
		s_serveroptions.playerType[count].curvalue = 0;
	}

	// close off the rest by default
	for( ;count < PLAYER_SLOTS; count++ ) {
		if( s_serveroptions.playerType[count].curvalue == 1 ) {
			s_serveroptions.playerType[count].curvalue = 2;
		}
	}
}


/*
=================
ServerOptions_SetMenuItems
=================
*/
static void ServerOptions_SetMenuItems( void ) {
	static char picname[64];
	char		mapname[MAX_NAMELENGTH];
	const char	*info;

	switch( s_serveroptions.gametype ) {
	case GT_FFA:
	default:
#ifdef TA_MISC // frag to score
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "ui_ffa_scorelimit" ) ) );
#else
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ffa_fraglimit" ) ) );
#endif
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ffa_timelimit" ) ) );
		break;

	case GT_TOURNAMENT:
#ifdef TA_MISC // frag to score
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "ui_tourney_scorelimit" ) ) );
#else
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_tourney_fraglimit" ) ) );
#endif
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_tourney_timelimit" ) ) );
		break;

#ifdef TA_SP
	case GT_SINGLE_PLAYER:
		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", 0 );
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", 0 );
		s_serveroptions.friendlyfire.curvalue = 0;
		break;
#endif

	case GT_TEAM:
#ifdef TA_MISC // frag to score
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "ui_team_scorelimit" ) ) );
#else
		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_team_fraglimit" ) ) );
#endif
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_team_timelimit" ) ) );
		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_team_friendly" ) );
		break;

	case GT_CTF:
		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "ui_ctf_capturelimit" ) ) );
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ctf_timelimit" ) ) );
		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_ctf_friendly" ) );
		break;

#ifdef MISSIONPACK // MP_GAMETYPES
	case GT_1FCTF:
		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "ui_1flag_capturelimit" ) ) );
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_1flag_timelimit" ) ) );
		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_1flag_friendly" ) );
		break;

	case GT_OBELISK:
		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "ui_obelisk_capturelimit" ) ) );
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_obelisk_timelimit" ) ) );
		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_obelisk_friendly" ) );
		break;

#ifdef MISSIONPACK_HARVESTER
	case GT_HARVESTER:
		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "ui_harvester_capturelimit" ) ) );
		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_harvester_timelimit" ) ) );
		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_harvester_friendly" ) );
		break;
#endif
#endif
	}

	Q_strncpyz( s_serveroptions.hostname.field.buffer, UI_Cvar_VariableString( "sv_hostname" ), sizeof( s_serveroptions.hostname.field.buffer ) );
	s_serveroptions.pure.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "sv_pure" ) );

	// set the map pic
	info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
	Q_strncpyz( mapname, Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
	Q_strupr( mapname );
#ifdef TA_DATA // TEAMARENA_LEVELSHOTS
	Com_sprintf( picname, 64, "levelshots/%s_small", mapname );
#else
	Com_sprintf( picname, 64, "levelshots/%s", mapname );
#endif
	s_serveroptions.mappic.generic.name = picname;

	// set the map name
	strcpy( s_serveroptions.mapnamebuffer, s_startserver.mapname.string );
//#ifndef IOQ3ZTM // SUPPORT_LINUX_NO_PAK
	Q_strupr( s_serveroptions.mapnamebuffer );
//#endif

	// get the player selections initialized
	ServerOptions_InitPlayerItems();
	ServerOptions_SetPlayerItems();

	// seed bot names
	ServerOptions_InitBotNames();
	ServerOptions_SetPlayerItems();
}

/*
=================
PlayerName_Draw
=================
*/
static void PlayerName_Draw( void *item ) {
	menutext_s	*s;
	float		*color;
	int			x, y;
	int			style;
	qboolean	focus;

	s = (menutext_s *)item;

	x = s->generic.x;
	y =	s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if ( s->generic.flags & QMF_GRAYED )
		color = text_color_disabled;
	else if ( focus )
	{
		color = text_color_highlight;
		style |= UI_PULSE;
	}
	else if ( s->generic.flags & QMF_BLINK )
	{
		color = text_color_highlight;
		style |= UI_BLINK;
	}
	else
		color = text_color_normal;

	if ( focus )
	{
		// draw cursor
		UI_FillRect( s->generic.left, s->generic.top, s->generic.right-s->generic.left+1, s->generic.bottom-s->generic.top+1, listbar_color ); 
		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color);
	}

	UI_DrawString( x - SMALLCHAR_WIDTH, y, s->generic.name, style|UI_RIGHT, color );
	UI_DrawString( x + SMALLCHAR_WIDTH, y, s->string, style|UI_LEFT, color );
}


/*
=================
ServerOptions_MenuInit
=================
*/
#define OPTIONS_X	456

static void ServerOptions_MenuInit( qboolean multiplayer ) {
	int		y;
	int		n;

	memset( &s_serveroptions, 0 ,sizeof(serveroptions_t) );
	s_serveroptions.multiplayer = multiplayer;
#ifdef MISSIONPACK // ZTM: Allow all game types.
	s_serveroptions.gametype = (int)Com_Clamp( 0, GT_MAX_GAME_TYPE-1, trap_Cvar_VariableValue( "g_gametype" ) );
#elif defined IOQ3ZTM // IOQ3BUGFIX: Quake3 only had 4 gametypes
	s_serveroptions.gametype = (int)Com_Clamp( 0, 4, trap_Cvar_VariableValue( "g_gametype" ) );
#else
	s_serveroptions.gametype = (int)Com_Clamp( 0, 5, trap_Cvar_VariableValue( "g_gameType" ) );
#endif
#ifdef IOQUAKE3 // ZTM: punkbuster
	s_serveroptions.punkbuster.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "sv_punkbuster" ) );
#endif

	ServerOptions_Cache();

	s_serveroptions.menu.wrapAround = qtrue;
	s_serveroptions.menu.fullscreen = qtrue;

	s_serveroptions.banner.generic.type			= MTYPE_BTEXT;
	s_serveroptions.banner.generic.x			= 320;
	s_serveroptions.banner.generic.y			= 16;
	s_serveroptions.banner.string  				= "GAME SERVER";
	s_serveroptions.banner.color  				= text_banner_color;
	s_serveroptions.banner.style  				= UI_CENTER;

	s_serveroptions.mappic.generic.type			= MTYPE_BITMAP;
	s_serveroptions.mappic.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
	s_serveroptions.mappic.generic.x			= 352;
	s_serveroptions.mappic.generic.y			= 80;
	s_serveroptions.mappic.width				= 160;
	s_serveroptions.mappic.height				= 120;
	s_serveroptions.mappic.errorpic				= GAMESERVER_UNKNOWNMAP;
	s_serveroptions.mappic.generic.ownerdraw	= ServerOptions_LevelshotDraw;

	s_serveroptions.picframe.generic.type		= MTYPE_BITMAP;
	s_serveroptions.picframe.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE|QMF_HIGHLIGHT;
	s_serveroptions.picframe.generic.x			= 352 - 38;
	s_serveroptions.picframe.generic.y			= 80 - 40;
	s_serveroptions.picframe.width  			= 320;
	s_serveroptions.picframe.height  			= 320;
	s_serveroptions.picframe.focuspic			= GAMESERVER_SELECT;

	y = 272;
#ifdef TA_SP
	if (s_serveroptions.gametype != GT_SINGLE_PLAYER)
	{
#endif
#ifdef MISSIONPACK // TA_MISSIONPACK
	if( s_serveroptions.gametype <= GT_TEAM )
#else
	if( s_serveroptions.gametype != GT_CTF )
#endif
	{
		s_serveroptions.fraglimit.generic.type       = MTYPE_FIELD;
#ifdef TA_MISC // frag to score
		s_serveroptions.fraglimit.generic.name       = "Score Limit:";
#else
		s_serveroptions.fraglimit.generic.name       = "Frag Limit:";
#endif
		s_serveroptions.fraglimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.fraglimit.generic.x	         = OPTIONS_X;
		s_serveroptions.fraglimit.generic.y	         = y;
		s_serveroptions.fraglimit.generic.statusbar  = ServerOptions_StatusBar;
#ifdef TA_MISC // frag to score
		s_serveroptions.fraglimit.field.widthInChars = 4;
		s_serveroptions.fraglimit.field.maxchars     = 4;
#else
		s_serveroptions.fraglimit.field.widthInChars = 3;
		s_serveroptions.fraglimit.field.maxchars     = 3;
#endif
	}
	else {
		s_serveroptions.flaglimit.generic.type       = MTYPE_FIELD;
		s_serveroptions.flaglimit.generic.name       = "Capture Limit:";
		s_serveroptions.flaglimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.flaglimit.generic.x	         = OPTIONS_X;
		s_serveroptions.flaglimit.generic.y	         = y;
		s_serveroptions.flaglimit.generic.statusbar  = ServerOptions_StatusBar;
		s_serveroptions.flaglimit.field.widthInChars = 3;
		s_serveroptions.flaglimit.field.maxchars     = 3;
	}

	y += BIGCHAR_HEIGHT+2;
	s_serveroptions.timelimit.generic.type       = MTYPE_FIELD;
	s_serveroptions.timelimit.generic.name       = "Time Limit:";
	s_serveroptions.timelimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.timelimit.generic.x	         = OPTIONS_X;
	s_serveroptions.timelimit.generic.y	         = y;
	s_serveroptions.timelimit.generic.statusbar  = ServerOptions_StatusBar;
	s_serveroptions.timelimit.field.widthInChars = 3;
	s_serveroptions.timelimit.field.maxchars     = 3;

#ifdef TA_SP
	}
#endif

	if( s_serveroptions.gametype >= GT_TEAM ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.friendlyfire.generic.type     = MTYPE_RADIOBUTTON;
		s_serveroptions.friendlyfire.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.friendlyfire.generic.x	      = OPTIONS_X;
		s_serveroptions.friendlyfire.generic.y	      = y;
#ifdef TA_MISC
		s_serveroptions.friendlyfire.generic.name	  = "Team Damage:";
#else
		s_serveroptions.friendlyfire.generic.name	  = "Friendly Fire:";
#endif
	}

	y += BIGCHAR_HEIGHT+2;
	s_serveroptions.pure.generic.type			= MTYPE_RADIOBUTTON;
	s_serveroptions.pure.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.pure.generic.x				= OPTIONS_X;
	s_serveroptions.pure.generic.y				= y;
	s_serveroptions.pure.generic.name			= "Pure Server:";
#if defined STANDALONE && defined IOQ3ZTM // FS_PURE
	if (!trap_Cvar_VariableValue( "fs_pure" ))
	{
		// Don't let users think they can modify sv_pure, it won't work.
		s_serveroptions.pure.generic.flags |= QMF_GRAYED;
	}
#endif

	if( s_serveroptions.multiplayer ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.dedicated.generic.type		= MTYPE_SPINCONTROL;
		s_serveroptions.dedicated.generic.id		= ID_DEDICATED;
		s_serveroptions.dedicated.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.dedicated.generic.callback	= ServerOptions_Event;
		s_serveroptions.dedicated.generic.x			= OPTIONS_X;
		s_serveroptions.dedicated.generic.y			= y;
		s_serveroptions.dedicated.generic.name		= "Dedicated:";
		s_serveroptions.dedicated.itemnames			= dedicated_list;
	}

	if( s_serveroptions.multiplayer ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.hostname.generic.type       = MTYPE_FIELD;
		s_serveroptions.hostname.generic.name       = "Hostname:";
		s_serveroptions.hostname.generic.flags      = QMF_SMALLFONT;
		s_serveroptions.hostname.generic.x          = OPTIONS_X;
		s_serveroptions.hostname.generic.y	        = y;
		s_serveroptions.hostname.field.widthInChars = 18;
		s_serveroptions.hostname.field.maxchars     = 64;
	}

#ifdef IOQUAKE3 // ZTM: punkbuster
	y += BIGCHAR_HEIGHT+2;
	s_serveroptions.punkbuster.generic.type			= MTYPE_SPINCONTROL;
	s_serveroptions.punkbuster.generic.name			= "Punkbuster:";
	s_serveroptions.punkbuster.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.punkbuster.generic.id			= 0;
	s_serveroptions.punkbuster.generic.x				= OPTIONS_X;
	s_serveroptions.punkbuster.generic.y				= y;
	s_serveroptions.punkbuster.itemnames				= punkbuster_items;
#endif
	
	y = 80;
	s_serveroptions.botSkill.generic.type			= MTYPE_SPINCONTROL;
	s_serveroptions.botSkill.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.botSkill.generic.name			= "Bot Skill:  ";
	s_serveroptions.botSkill.generic.x				= 32 + (strlen(s_serveroptions.botSkill.generic.name) + 2 ) * SMALLCHAR_WIDTH;
	s_serveroptions.botSkill.generic.y				= y;
	s_serveroptions.botSkill.itemnames				= botSkill_list;
	s_serveroptions.botSkill.curvalue				= 1;

	y += ( 2 * SMALLCHAR_HEIGHT );
	s_serveroptions.player0.generic.type			= MTYPE_TEXT;
	s_serveroptions.player0.generic.flags			= QMF_SMALLFONT;
	s_serveroptions.player0.generic.x				= 32 + SMALLCHAR_WIDTH;
	s_serveroptions.player0.generic.y				= y;
	s_serveroptions.player0.color					= color_orange;
	s_serveroptions.player0.style					= UI_LEFT|UI_SMALLFONT;

	for( n = 0; n < PLAYER_SLOTS; n++ ) {
		s_serveroptions.playerType[n].generic.type		= MTYPE_SPINCONTROL;
		s_serveroptions.playerType[n].generic.flags		= QMF_SMALLFONT;
		s_serveroptions.playerType[n].generic.id		= ID_PLAYER_TYPE;
		s_serveroptions.playerType[n].generic.callback	= ServerOptions_Event;
		s_serveroptions.playerType[n].generic.x			= 32;
		s_serveroptions.playerType[n].generic.y			= y;
		s_serveroptions.playerType[n].itemnames			= playerType_list;

		s_serveroptions.playerName[n].generic.type		= MTYPE_TEXT;
		s_serveroptions.playerName[n].generic.flags		= QMF_SMALLFONT;
		s_serveroptions.playerName[n].generic.x			= 96;
		s_serveroptions.playerName[n].generic.y			= y;
		s_serveroptions.playerName[n].generic.callback	= ServerOptions_PlayerNameEvent;
		s_serveroptions.playerName[n].generic.id		= n;
		s_serveroptions.playerName[n].generic.ownerdraw	= PlayerName_Draw;
		s_serveroptions.playerName[n].color				= color_orange;
		s_serveroptions.playerName[n].style				= UI_SMALLFONT;
		s_serveroptions.playerName[n].string			= s_serveroptions.playerNameBuffers[n];
		s_serveroptions.playerName[n].generic.top		= s_serveroptions.playerName[n].generic.y;
		s_serveroptions.playerName[n].generic.bottom	= s_serveroptions.playerName[n].generic.y + SMALLCHAR_HEIGHT;
		s_serveroptions.playerName[n].generic.left		= s_serveroptions.playerName[n].generic.x - SMALLCHAR_HEIGHT/ 2;
		s_serveroptions.playerName[n].generic.right		= s_serveroptions.playerName[n].generic.x + 16 * SMALLCHAR_WIDTH;

		s_serveroptions.playerTeam[n].generic.type		= MTYPE_SPINCONTROL;
		s_serveroptions.playerTeam[n].generic.flags		= QMF_SMALLFONT;
		s_serveroptions.playerTeam[n].generic.x			= 240;
		s_serveroptions.playerTeam[n].generic.y			= y;
		s_serveroptions.playerTeam[n].itemnames			= playerTeam_list;

		y += ( SMALLCHAR_HEIGHT + 4 );
	}

	s_serveroptions.back.generic.type	  = MTYPE_BITMAP;
	s_serveroptions.back.generic.name     = GAMESERVER_BACK0;
	s_serveroptions.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_serveroptions.back.generic.callback = ServerOptions_Event;
	s_serveroptions.back.generic.id	      = ID_BACK;
	s_serveroptions.back.generic.x		  = 0;
	s_serveroptions.back.generic.y		  = 480-64;
	s_serveroptions.back.width  		  = 128;
	s_serveroptions.back.height  		  = 64;
	s_serveroptions.back.focuspic         = GAMESERVER_BACK1;

	s_serveroptions.next.generic.type	  = MTYPE_BITMAP;
	s_serveroptions.next.generic.name     = GAMESERVER_NEXT0;
	s_serveroptions.next.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_INACTIVE|QMF_GRAYED|QMF_HIDDEN;
	s_serveroptions.next.generic.callback = ServerOptions_Event;
	s_serveroptions.next.generic.id	      = ID_STARTSERVERNEXT;
	s_serveroptions.next.generic.x		  = 640;
	s_serveroptions.next.generic.y		  = 480-64-72;
	s_serveroptions.next.generic.statusbar  = ServerOptions_StatusBar;
	s_serveroptions.next.width  		  = 128;
	s_serveroptions.next.height  		  = 64;
	s_serveroptions.next.focuspic         = GAMESERVER_NEXT1;

	s_serveroptions.go.generic.type	    = MTYPE_BITMAP;
	s_serveroptions.go.generic.name     = GAMESERVER_FIGHT0;
	s_serveroptions.go.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_serveroptions.go.generic.callback = ServerOptions_Event;
	s_serveroptions.go.generic.id	    = ID_GO;
	s_serveroptions.go.generic.x		= 640;
	s_serveroptions.go.generic.y		= 480-64;
	s_serveroptions.go.width  		    = 128;
	s_serveroptions.go.height  		    = 64;
	s_serveroptions.go.focuspic         = GAMESERVER_FIGHT1;

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.banner );

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.mappic );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.picframe );

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.botSkill );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.player0 );
	for( n = 0; n < PLAYER_SLOTS; n++ ) {
		if( n != 0 ) {
			Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerType[n] );
		}
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerName[n] );
		if( s_serveroptions.gametype >= GT_TEAM ) {
			Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerTeam[n] );
		}
	}

#ifdef TA_SP
	if (s_serveroptions.gametype != GT_SINGLE_PLAYER)
	{
#endif
#ifdef MISSIONPACK // TA_MISSIONPACK
	if( s_serveroptions.gametype <= GT_TEAM )
#else
	if( s_serveroptions.gametype != GT_CTF )
#endif
	{
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.fraglimit );
	}
	else {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.flaglimit );
	}
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.timelimit );
#ifdef TA_SP
	}
#endif
	if( s_serveroptions.gametype >= GT_TEAM ) {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.friendlyfire );
	}
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.pure );
	if( s_serveroptions.multiplayer ) {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.dedicated );
	}
	if( s_serveroptions.multiplayer ) {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.hostname );
	}

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.back );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.next );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.go );

#ifdef IOQUAKE3 // ZTM: punkbuster
	Menu_AddItem( &s_serveroptions.menu, (void*) &s_serveroptions.punkbuster );
#endif
	
	ServerOptions_SetMenuItems();
}

/*
=================
ServerOptions_Cache
=================
*/
void ServerOptions_Cache( void ) {
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );
	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
}


/*
=================
UI_ServerOptionsMenu
=================
*/
static void UI_ServerOptionsMenu( qboolean multiplayer ) {
	ServerOptions_MenuInit( multiplayer );
	UI_PushMenu( &s_serveroptions.menu );
}



/*
=============================================================================

BOT SELECT MENU *****

=============================================================================
*/


#define BOTSELECT_BACK0			"menu/art/back_0"
#define BOTSELECT_BACK1			"menu/art/back_1"
#define BOTSELECT_ACCEPT0		"menu/art/accept_0"
#define BOTSELECT_ACCEPT1		"menu/art/accept_1"
#define BOTSELECT_SELECT		"menu/art/opponents_select"
#define BOTSELECT_SELECTED		"menu/art/opponents_selected"
#ifdef TA_DATA
#define BOTSELECT_ARROWS		"menu/art/arrows_horz_0"
#define BOTSELECT_ARROWSL		"menu/art/arrows_horz_left"
#define BOTSELECT_ARROWSR		"menu/art/arrows_horz_right"
#define BOTSELECT_ARROWS_WIDTH 128
#define BOTSELECT_ARROWS_HEIGHT 64
#else
#define BOTSELECT_ARROWS		"menu/art/gs_arrows_0"
#define BOTSELECT_ARROWSL		"menu/art/gs_arrows_l"
#define BOTSELECT_ARROWSR		"menu/art/gs_arrows_r"
#endif

#define PLAYERGRID_COLS			4
#define PLAYERGRID_ROWS			4
#define MAX_MODELSPERPAGE		(PLAYERGRID_ROWS * PLAYERGRID_COLS)


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;

	menubitmap_s	pics[MAX_MODELSPERPAGE];
	menubitmap_s	picbuttons[MAX_MODELSPERPAGE];
	menutext_s		picnames[MAX_MODELSPERPAGE];

	menubitmap_s	arrows;
	menubitmap_s	left;
	menubitmap_s	right;

	menubitmap_s	go;
	menubitmap_s	back;

	int				numBots;
	int				modelpage;
	int				numpages;
	int				selectedmodel;
	int				sortedBotNums[MAX_BOTS];
	char			boticons[MAX_MODELSPERPAGE][MAX_QPATH];
	char			botnames[MAX_MODELSPERPAGE][16];
} botSelectInfo_t;

static botSelectInfo_t	botSelectInfo;


/*
=================
UI_BotSelectMenu_SortCompare
=================
*/
static int QDECL UI_BotSelectMenu_SortCompare( const void *arg1, const void *arg2 ) {
	int			num1, num2;
	const char	*info1, *info2;
	const char	*name1, *name2;

	num1 = *(int *)arg1;
	num2 = *(int *)arg2;

	info1 = UI_GetBotInfoByNumber( num1 );
	info2 = UI_GetBotInfoByNumber( num2 );

	name1 = Info_ValueForKey( info1, "name" );
	name2 = Info_ValueForKey( info2, "name" );

#ifdef RANDOMBOT // ZTM: Random bot
    // ZTM: Random bot comes first on the list.
    if (Q_stricmp(name1, "Random") == 0 || Q_stricmp(name2, "Random") == 0)
    {
        return Q_stricmp( Q_stricmp(name1, "Random") == 0 ? "0" : "1",
                Q_stricmp(name2, "Random") == 0 ? "0" : "1" );
    }
#endif
	return Q_stricmp( name1, name2 );
}


/*
=================
UI_BotSelectMenu_BuildList
=================
*/
static void UI_BotSelectMenu_BuildList( void ) {
	int		n;

	botSelectInfo.modelpage = 0;
	botSelectInfo.numBots = UI_GetNumBots();
	botSelectInfo.numpages = botSelectInfo.numBots / MAX_MODELSPERPAGE;
	if( botSelectInfo.numBots % MAX_MODELSPERPAGE ) {
		botSelectInfo.numpages++;
	}

	// initialize the array
	for( n = 0; n < botSelectInfo.numBots; n++ ) {
		botSelectInfo.sortedBotNums[n] = n;
	}

	// now sort it
	qsort( botSelectInfo.sortedBotNums, botSelectInfo.numBots, sizeof(botSelectInfo.sortedBotNums[0]), UI_BotSelectMenu_SortCompare );
}


/*
=================
ServerPlayerIcon
=================
*/
static void ServerPlayerIcon( const char *modelAndSkin, char *iconName, int iconNameMaxSize ) {
	char	*skin;
	char	model[MAX_QPATH];

#ifdef RANDOMBOT // ZTM: Random bot
    // ZTM: Random bot's icon is in a different spot then a normal player.
    if (Q_stricmp(modelAndSkin, "Random") == 0)
    {
        Com_sprintf(iconName, iconNameMaxSize, "menu/art/randombot_icon.tga");

        trap_R_RegisterShaderNoMip( iconName );
        return;
    }
#endif

	Q_strncpyz( model, modelAndSkin, sizeof(model));
	skin = Q_strrchr( model, '/' );
	if ( skin ) {
		*skin++ = '\0';
	}
	else {
		skin = "default";
	}

	Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_%s.tga", model, skin );

	if( !trap_R_RegisterShaderNoMip( iconName ) && Q_stricmp( skin, "default" ) != 0 ) {
		Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_default.tga", model );
	}
}


/*
=================
UI_BotSelectMenu_UpdateGrid
=================
*/
static void UI_BotSelectMenu_UpdateGrid( void ) {
	const char	*info;
	int			i;
    int			j;

	j = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
	for( i = 0; i < (PLAYERGRID_ROWS * PLAYERGRID_COLS); i++, j++) {
		if( j < botSelectInfo.numBots ) { 
			info = UI_GetBotInfoByNumber( botSelectInfo.sortedBotNums[j] );
			ServerPlayerIcon( Info_ValueForKey( info, "model" ), botSelectInfo.boticons[i], MAX_QPATH );
			Q_strncpyz( botSelectInfo.botnames[i], Info_ValueForKey( info, "name" ), 16 );
			Q_CleanStr( botSelectInfo.botnames[i] );
 			botSelectInfo.pics[i].generic.name = botSelectInfo.boticons[i];
			if( BotAlreadySelected( botSelectInfo.botnames[i] ) ) {
#ifdef TURTLEARENA
				botSelectInfo.picnames[i].color = color_white;
#else
				botSelectInfo.picnames[i].color = color_red;
#endif
			}
			else {
				botSelectInfo.picnames[i].color = color_orange;
			}
			botSelectInfo.picbuttons[i].generic.flags &= ~QMF_INACTIVE;
		}
		else {
			// dead slot
 			botSelectInfo.pics[i].generic.name         = NULL;
			botSelectInfo.picbuttons[i].generic.flags |= QMF_INACTIVE;
			botSelectInfo.botnames[i][0] = 0;
		}

 		botSelectInfo.pics[i].generic.flags       &= ~QMF_HIGHLIGHT;
 		botSelectInfo.pics[i].shader               = 0;
 		botSelectInfo.picbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	// set selected model
	i = botSelectInfo.selectedmodel % MAX_MODELSPERPAGE;
	botSelectInfo.pics[i].generic.flags |= QMF_HIGHLIGHT;
	botSelectInfo.picbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;

	if( botSelectInfo.numpages > 1 ) {
		if( botSelectInfo.modelpage > 0 ) {
			botSelectInfo.left.generic.flags &= ~QMF_INACTIVE;
		}
		else {
			botSelectInfo.left.generic.flags |= QMF_INACTIVE;
		}

		if( botSelectInfo.modelpage < (botSelectInfo.numpages - 1) ) {
			botSelectInfo.right.generic.flags &= ~QMF_INACTIVE;
		}
		else {
			botSelectInfo.right.generic.flags |= QMF_INACTIVE;
		}
	}
	else {
		// hide left/right markers
		botSelectInfo.left.generic.flags |= QMF_INACTIVE;
		botSelectInfo.right.generic.flags |= QMF_INACTIVE;
	}
}


/*
=================
UI_BotSelectMenu_Default
=================
*/
static void UI_BotSelectMenu_Default( char *bot ) {
	const char	*botInfo;
	const char	*test;
	int			n;
	int			i;

	for( n = 0; n < botSelectInfo.numBots; n++ ) {
		botInfo = UI_GetBotInfoByNumber( n );
		test = Info_ValueForKey( botInfo, "name" );
		if( Q_stricmp( bot, test ) == 0 ) {
			break;
		}
	}
	if( n == botSelectInfo.numBots ) {
		botSelectInfo.selectedmodel = 0;
		return;
	}

	for( i = 0; i < botSelectInfo.numBots; i++ ) {
		if( botSelectInfo.sortedBotNums[i] == n ) {
			break;
		}
	}
	if( i == botSelectInfo.numBots ) {
		botSelectInfo.selectedmodel = 0;
		return;
	}

	botSelectInfo.selectedmodel = i;
}


/*
=================
UI_BotSelectMenu_LeftEvent
=================
*/
static void UI_BotSelectMenu_LeftEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	if( botSelectInfo.modelpage > 0 ) {
		botSelectInfo.modelpage--;
		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
		UI_BotSelectMenu_UpdateGrid();
	}
}


/*
=================
UI_BotSelectMenu_RightEvent
=================
*/
static void UI_BotSelectMenu_RightEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	if( botSelectInfo.modelpage < botSelectInfo.numpages - 1 ) {
		botSelectInfo.modelpage++;
		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
		UI_BotSelectMenu_UpdateGrid();
	}
}


/*
=================
UI_BotSelectMenu_BotEvent
=================
*/
static void UI_BotSelectMenu_BotEvent( void* ptr, int event ) {
	int		i;

	if( event != QM_ACTIVATED ) {
		return;
	}

	for( i = 0; i < (PLAYERGRID_ROWS * PLAYERGRID_COLS); i++ ) {
 		botSelectInfo.pics[i].generic.flags &= ~QMF_HIGHLIGHT;
 		botSelectInfo.picbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	// set selected
	i = ((menucommon_s*)ptr)->id;
	botSelectInfo.pics[i].generic.flags |= QMF_HIGHLIGHT;
	botSelectInfo.picbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
	botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE + i;
}


/*
=================
UI_BotSelectMenu_BackEvent
=================
*/
static void UI_BotSelectMenu_BackEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
=================
UI_BotSelectMenu_SelectEvent
=================
*/
static void UI_BotSelectMenu_SelectEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();

	s_serveroptions.newBot = qtrue;
	Q_strncpyz( s_serveroptions.newBotName, botSelectInfo.botnames[botSelectInfo.selectedmodel % MAX_MODELSPERPAGE], 16 );
}


/*
=================
UI_BotSelectMenu_Cache
=================
*/
void UI_BotSelectMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( BOTSELECT_BACK0 );
	trap_R_RegisterShaderNoMip( BOTSELECT_BACK1 );
	trap_R_RegisterShaderNoMip( BOTSELECT_ACCEPT0 );
	trap_R_RegisterShaderNoMip( BOTSELECT_ACCEPT1 );
	trap_R_RegisterShaderNoMip( BOTSELECT_SELECT );
	trap_R_RegisterShaderNoMip( BOTSELECT_SELECTED );
	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWS );
	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWSL );
	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWSR );
}


static void UI_BotSelectMenu_Init( char *bot ) {
	int		i, j, k;
	int		x, y;

	memset( &botSelectInfo, 0 ,sizeof(botSelectInfo) );
	botSelectInfo.menu.wrapAround = qtrue;
	botSelectInfo.menu.fullscreen = qtrue;

	UI_BotSelectMenu_Cache();

	botSelectInfo.banner.generic.type	= MTYPE_BTEXT;
	botSelectInfo.banner.generic.x		= 320;
	botSelectInfo.banner.generic.y		= 16;
	botSelectInfo.banner.string			= "SELECT BOT";
	botSelectInfo.banner.color			= text_banner_color;
	botSelectInfo.banner.style			= UI_CENTER;

	y =	80;
	for( i = 0, k = 0; i < PLAYERGRID_ROWS; i++) {
		x =	180;
		for( j = 0; j < PLAYERGRID_COLS; j++, k++ ) {
			botSelectInfo.pics[k].generic.type				= MTYPE_BITMAP;
			botSelectInfo.pics[k].generic.flags				= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
			botSelectInfo.pics[k].generic.x					= x;
			botSelectInfo.pics[k].generic.y					= y;
 			botSelectInfo.pics[k].generic.name				= botSelectInfo.boticons[k];
			botSelectInfo.pics[k].width						= 64;
			botSelectInfo.pics[k].height					= 64;
			botSelectInfo.pics[k].focuspic					= BOTSELECT_SELECTED;
#ifndef TA_DATA
			botSelectInfo.pics[k].focuscolor				= colorRed;
#endif

			botSelectInfo.picbuttons[k].generic.type		= MTYPE_BITMAP;
			botSelectInfo.picbuttons[k].generic.flags		= QMF_LEFT_JUSTIFY|QMF_NODEFAULTINIT|QMF_PULSEIFFOCUS;
			botSelectInfo.picbuttons[k].generic.callback	= UI_BotSelectMenu_BotEvent;
			botSelectInfo.picbuttons[k].generic.id			= k;
			botSelectInfo.picbuttons[k].generic.x			= x - 16;
			botSelectInfo.picbuttons[k].generic.y			= y - 16;
			botSelectInfo.picbuttons[k].generic.left		= x;
			botSelectInfo.picbuttons[k].generic.top			= y;
			botSelectInfo.picbuttons[k].generic.right		= x + 64;
			botSelectInfo.picbuttons[k].generic.bottom		= y + 64;
			botSelectInfo.picbuttons[k].width				= 128;
			botSelectInfo.picbuttons[k].height				= 128;
			botSelectInfo.picbuttons[k].focuspic			= BOTSELECT_SELECT;
#ifndef TA_DATA
			botSelectInfo.picbuttons[k].focuscolor			= colorRed;
#endif

			botSelectInfo.picnames[k].generic.type			= MTYPE_TEXT;
			botSelectInfo.picnames[k].generic.flags			= QMF_SMALLFONT;
			botSelectInfo.picnames[k].generic.x				= x + 32;
			botSelectInfo.picnames[k].generic.y				= y + 64;
			botSelectInfo.picnames[k].string				= botSelectInfo.botnames[k];
			botSelectInfo.picnames[k].color					= color_orange;
			botSelectInfo.picnames[k].style					= UI_CENTER|UI_SMALLFONT;

			x += (64 + 6);
		}
		y += (64 + SMALLCHAR_HEIGHT + 6);
	}

	botSelectInfo.arrows.generic.type		= MTYPE_BITMAP;
	botSelectInfo.arrows.generic.name		= BOTSELECT_ARROWS;
	botSelectInfo.arrows.generic.flags		= QMF_INACTIVE;
	botSelectInfo.arrows.generic.x			= 260;
#ifdef TA_DATA
	botSelectInfo.arrows.generic.y			= 400;
	botSelectInfo.arrows.width				= BOTSELECT_ARROWS_WIDTH;
	botSelectInfo.arrows.height				= BOTSELECT_ARROWS_HEIGHT;
#else
	botSelectInfo.arrows.generic.y			= 440;
	botSelectInfo.arrows.width				= 128;
	botSelectInfo.arrows.height				= 32;
#endif

	botSelectInfo.left.generic.type			= MTYPE_BITMAP;
	botSelectInfo.left.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	botSelectInfo.left.generic.callback		= UI_BotSelectMenu_LeftEvent;
	botSelectInfo.left.generic.x			= 260;
#ifdef TA_DATA
	botSelectInfo.left.generic.y			= 400;
	botSelectInfo.left.width  				= BOTSELECT_ARROWS_WIDTH/2;
	botSelectInfo.left.height  				= BOTSELECT_ARROWS_HEIGHT;
#else
	botSelectInfo.left.generic.y			= 440;
	botSelectInfo.left.width  				= 64;
	botSelectInfo.left.height  				= 32;
#endif
	botSelectInfo.left.focuspic				= BOTSELECT_ARROWSL;

	botSelectInfo.right.generic.type	    = MTYPE_BITMAP;
	botSelectInfo.right.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	botSelectInfo.right.generic.callback	= UI_BotSelectMenu_RightEvent;
#ifdef TA_DATA
	botSelectInfo.right.generic.x			= 260+BOTSELECT_ARROWS_WIDTH/2;
	botSelectInfo.right.generic.y			= 400;
	botSelectInfo.right.width  				= BOTSELECT_ARROWS_WIDTH/2;
	botSelectInfo.right.height  		    = BOTSELECT_ARROWS_HEIGHT;
#else
	botSelectInfo.right.generic.x			= 321;
	botSelectInfo.right.generic.y			= 440;
	botSelectInfo.right.width  				= 64;
	botSelectInfo.right.height  		    = 32;
#endif
	botSelectInfo.right.focuspic			= BOTSELECT_ARROWSR;

	botSelectInfo.back.generic.type		= MTYPE_BITMAP;
	botSelectInfo.back.generic.name		= BOTSELECT_BACK0;
	botSelectInfo.back.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	botSelectInfo.back.generic.callback	= UI_BotSelectMenu_BackEvent;
	botSelectInfo.back.generic.x		= 0;
	botSelectInfo.back.generic.y		= 480-64;
	botSelectInfo.back.width			= 128;
	botSelectInfo.back.height			= 64;
	botSelectInfo.back.focuspic			= BOTSELECT_BACK1;

	botSelectInfo.go.generic.type		= MTYPE_BITMAP;
	botSelectInfo.go.generic.name		= BOTSELECT_ACCEPT0;
	botSelectInfo.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	botSelectInfo.go.generic.callback	= UI_BotSelectMenu_SelectEvent;
	botSelectInfo.go.generic.x			= 640;
	botSelectInfo.go.generic.y			= 480-64;
	botSelectInfo.go.width				= 128;
	botSelectInfo.go.height				= 64;
	botSelectInfo.go.focuspic			= BOTSELECT_ACCEPT1;

	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.banner );
	for( i = 0; i < MAX_MODELSPERPAGE; i++ ) {
		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.pics[i] );
		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.picbuttons[i] );
		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.picnames[i] );
	}
	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.arrows );
	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.left );
	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.right );
	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.back );
	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.go );

	UI_BotSelectMenu_BuildList();
	UI_BotSelectMenu_Default( bot );
	botSelectInfo.modelpage = botSelectInfo.selectedmodel / MAX_MODELSPERPAGE;
	UI_BotSelectMenu_UpdateGrid();
}


/*
=================
UI_BotSelectMenu
=================
*/
void UI_BotSelectMenu( char *bot ) {
	UI_BotSelectMenu_Init( bot );
	UI_PushMenu( &botSelectInfo.menu );
}
