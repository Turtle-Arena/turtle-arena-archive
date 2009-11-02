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
// bg_misc.c -- both games misc functions, all completely stateless

#include "../qcommon/q_shared.h"
#include "bg_misc.h"

#ifdef SP_NPC
bgnpc_t bg_npclist[] =
{
	// Turtle Man: TODO: Add my own NPCs.
	/*{
		"npc_flybot",				// class
		NPC_FLYBOT,
		WP_NONE,
		NPCF_FLYONLY,				// flags
		NPCD_EXPLODE,				// deathType
		20,							// health
		1.0,
		20,							// walkingSpeed
		60,							// runningSpeed
		180,						// fov
		0,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		25,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,24 },
		{ 0,0,30 },
	},
	{
		"npc_mouser",				// class
		NPC_MOUSER,
		WP_ROCKET_LAUNCHER,
		NPCF_NODROPWEAPON,			// flags
		NPCD_EXPLODE,				// deathType
		20,							// health
		1.0,
		20,							// walkingSpeed
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		25,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,24 },
		{ 0,0,30 },
	},*/


	/*{
		"npc_ank",					// class
		NPC_ANK,
		WP_NONE,
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		20,							// walkingSpeed
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		25,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,24 },
		{ 0,0,30 },
	},*/
	{
		"npc_bat",					// class
		NPC_BAT,
		WP_NONE,
		NPCF_WALKANDFLY,			// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		40,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		0,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},
	/*{
		"npc_hulk",					// class
		NPC_HULK,
		WP_NONE,
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		30,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		50,							// melee damage
		35,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,24 },
		{ 0,0,30 },
	},
	{
		"npc_metlar",					// class
		NPC_METLAR,
		WP_NONE,
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		20,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		25,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},*/
	{
		"npc_pilot",					// class
		NPC_PILOT,
#ifdef TMNTWEAPONS
		WP_GUN,
#else
		WP_MACHINEGUN,
#endif
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		20,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		0,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},
	/*{
		"npc_sealord",					// class
		NPC_SEALORD,
		WP_NONE,
		0,							// flags
		NPCD_SINK,					// deathType
		1000,						// health
		1.0,
		0,
		0,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		0,							// walkingRotSpd
		0,							// runningRotSpd
		0,							// melee distance
		25,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},*/
	{
		"npc_soldier1",					// class
		NPC_SOLDIER1,
#ifdef TMNTWEAPONS
		WP_GUN,
#else
		WP_MACHINEGUN,
#endif
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		20,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		0,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},
	/*{
		"npc_soldier2",					// class
		NPC_SOLDIER2,
#ifdef TMNTWEAPONS
		WP_GUN,
#else
		WP_MACHINEGUN,
#endif
		0,							// flags
		NPCD_SINK,					// deathType
		100,						// health
		1.0,
		20,
		60,							// runningSpeed
		180,						// fov
		50,							// jumpHeight
		20,							// walkingRotSpd
		75,							// runningRotSpd
		0,							// melee distance
		0,							// melee damage
		20,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ -24,-24,-24 },
		{ 24,24,40 },
		{ 0,0,30 },
	},*/
	{
		NULL,						// class
		NPC_NONE,
		WP_NONE,
		0,							// flags
		NPCD_NONE,					// deathType
		0,							// health
		0,
		0,
		0,							// runningSpeed
		0,							// fov
		0,							// jumpHeight
		0,							// walkingRotSpd
		0,							// runningRotSpd
		0,							// melee distance
		0,							// melee damage
		0,							// far damage
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0 },
		{ 0,0,0 },
		{ 0,0,0 },
	},
};
#endif

/*QUAKED item_***** ( 0 0 0 ) (-16 -16 -16) (16 16 16) suspended
DO NOT USE THIS CLASS, IT JUST HOLDS GENERAL INFORMATION.
The suspended flag will allow items to hang in the air, otherwise they are dropped to the next surface.

If an item is the target of another entity, it will not spawn in until fired.

An item fires all of its targets when it is picked up.  If the toucher can't carry it, the targets won't be fired.

"notfree" if set to 1, don't spawn in free for all games
"notteam" if set to 1, don't spawn in team games
"notsingle" if set to 1, don't spawn in single player games
"wait"	override the default wait before respawning.  -1 = never respawn automatically, which can be used with targeted spawning.
"random" random number of plus or minus seconds varied from the respawn time
"count" override quantity or duration on most items.
*/

// Turtle Man: FIXME: The weapon items in bg_itemlist should be unused
//      (Exept botlib loads items.c which has the modelindexes (indexes in bg_itemlist) )
//      But changing it causes it to crash?
#ifdef TMNTWEAPSYS_2
#define EMPTY_GITEM "item_health_small", \
		"sound/items/s_health.wav", \
        { "models/powerups/health/small.md3", \
		NULL, \
		NULL, NULL }, \
		"icons/iconh_small", \
		"5 Health", \
		5, \
		IT_HEALTH, \
		0, \
		"", \
		""
#endif

gitem_t	bg_itemlist[] = 
{
	{
		NULL,
		NULL,
		{ NULL,
		NULL,
		NULL, NULL} ,
/* icon */		NULL,
/* pickup */	NULL,
		0,
		0,
		0,
/* precache */ "",
/* sounds */ ""
	},	// leave index 0 alone

#ifndef TMNT // NOARMOR
	//
	// ARMOR
	//

/*QUAKED item_armor_shard (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_armor_shard", 
		"sound/misc/ar1_pkup.wav",
		{ "models/powerups/armor/shard.md3", 
		"models/powerups/armor/shard_sphere.md3",
		NULL, NULL} ,
/* icon */		"icons/iconr_shard",
/* pickup */	"Armor Shard",
		5,
		IT_ARMOR,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_armor_combat (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_armor_combat", 
		"sound/misc/ar2_pkup.wav",
        { "models/powerups/armor/armor_yel.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconr_yellow",
/* pickup */	"Armor",
		50,
		IT_ARMOR,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_armor_body (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_armor_body", 
		"sound/misc/ar2_pkup.wav",
        { "models/powerups/armor/armor_red.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconr_red",
/* pickup */	"Heavy Armor",
		100,
		IT_ARMOR,
		0,
/* precache */ "",
/* sounds */ ""
	},
#endif

	//
	// health
	//
/*QUAKED item_health_small (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_health_small",
		"sound/items/s_health.wav",
#ifdef TMNTDATA
        { "models/powerups/health/small.md3",
		NULL,
#else
        { "models/powerups/health/small_cross.md3", 
		"models/powerups/health/small_sphere.md3", 
#endif
		NULL, NULL },
#ifdef TMNTDATA
/* icon */		"icons/iconh_small",
#else
/* icon */		"icons/iconh_green",
#endif
/* pickup */	"5 Health",
		5,
		IT_HEALTH,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_health (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_health",
		"sound/items/n_health.wav",
#ifdef TMNTDATA
        { "models/powerups/health/medium.md3",
		NULL,
#else
        { "models/powerups/health/medium_cross.md3", 
		"models/powerups/health/medium_sphere.md3", 
#endif
		NULL, NULL },
#ifdef TMNTDATA
/* icon */		"icons/iconh_medium",
#else
/* icon */		"icons/iconh_yellow",
#endif
/* pickup */	"25 Health",
		25,
		IT_HEALTH,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_health_large (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_health_large",
		"sound/items/l_health.wav",
#ifdef TMNTDATA
        { "models/powerups/health/large.md3",
		NULL,
#else
        { "models/powerups/health/large_cross.md3", 
		"models/powerups/health/large_sphere.md3", 
#endif
		NULL, NULL },
#ifdef TMNTDATA
/* icon */		"icons/iconh_large",
#else
/* icon */		"icons/iconh_red",
#endif
/* pickup */	"50 Health",
		50,
		IT_HEALTH,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_health_mega (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_health_mega",
		"sound/items/m_health.wav",
#ifdef TMNTDATA
        { "models/powerups/health/mega.md3",
		NULL,
#else
        { "models/powerups/health/mega_cross.md3", 
		"models/powerups/health/mega_sphere.md3", 
#endif
		NULL, NULL },
/* icon */		"icons/iconh_mega",
#ifdef TMNTDATA
/* pickup */	"100 Health",
#else
/* pickup */	"Mega Health",
#endif
		100,
		IT_HEALTH,
		0,
/* precache */ "",
/* sounds */ ""
	},

	//
	// WEAPONS 
	//
#if defined TMNTWEAPSYS && defined TMNT // Turtle Man: FIXME: Breaks q3 bots' MODELINDEX_*s
/*QUAKED weapon_default (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_default",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/default/default.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_default",
/* pickup */	"Default Weapon",
		0,
		IT_WEAPON,
		WP_DEFAULT,
/* precache */ "",
/* sounds */ ""
	},
#endif

#if 0 // #ifdef TMNTWEAPSYS_2 // Keep the model index correct for items after weapons...
	{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },
	{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },
#ifdef TMNTWEAPONS
	{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },
	{ EMPTY_GITEM },
#endif
#elif defined TMNTWEAPONS
/*QUAKED weapon_fists (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_fists",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/fists/fists.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_fists",
/* pickup */	"Fists",
		0,
		IT_WEAPON,
		WP_FISTS,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_katanas (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_katanas",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/katana_leo/katanas_pickup.md3",
		NULL, "models/weapons2/katana_leo/katana_leo.md3", "models/weapons2/katana_leo/katana_leo.md3"},
/* icon */		"icons/iconw_katanas",
/* pickup */	"Katanas",
		0,
		IT_WEAPON,
		WP_KATANAS,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_daisho (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_daisho",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/daisho/daisho_pickup.md3",
		NULL, "models/weapons2/katana/katana.md3", "models/weapons2/wakizashi/wakizashi.md3"},
/* icon */		"icons/iconw_daisho",
/* pickup */	"Daisho",
		0,
		IT_WEAPON,
		WP_DAISHO,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_sais (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_sais",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/sai/sais_pickup.md3",
		NULL, "models/weapons2/sai/sai.md3", "models/weapons2/sai/sai.md3"},
/* icon */		"icons/iconw_sais",
/* pickup */	"Sais",
		0,
		IT_WEAPON,
		WP_SAIS,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_nunchuks (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_nunchuks",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/nunchuk/nunchuks_pickup.md3",
		NULL, "models/weapons2/nunchuk/nunchuk.md3", "models/weapons2/nunchuk/nunchuk.md3"},
/* icon */		"icons/iconw_nunchuks",
/* pickup */	"Nunchuks",
		0,
		IT_WEAPON,
		WP_NUNCHUKS,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_hammer (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_hammer",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/hammer/hammer.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_hammer",
/* pickup */	"Hammer",
		0,
		IT_WEAPON,
		WP_HAMMER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_axe (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_axe",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/axe/axe.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_axe",
/* pickup */	"Battle Axe", // "Axe",
		0,
		IT_WEAPON,
		WP_AXE,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_longsword (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_longsword",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/longsword/longsword.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_longsword",
/* pickup */	"Long Sword",
		0,
		IT_WEAPON,
		WP_LONGSWORD,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_bat (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_bat",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/bat/bat_pickup.md3",
		NULL, "models/weapons2/bat/bat.md3", NULL},
/* icon */		"icons/iconw_bat",
/* pickup */	"Baseball bat",
		0,
		IT_WEAPON,
		WP_BAT,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_bo (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_bo",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/bo/bo_pickup.md3",
		NULL, "models/weapons2/bo/bo.md3", NULL},
/* icon */		"icons/iconw_bo",
/* pickup */	"Bo",
		0,
		IT_WEAPON,
		WP_BO,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_bamboobo (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_bamboobo",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/bamboobo/bamboobo.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_bamboobo",
/* pickup */	"Bamboo Bo",
		0,
		IT_WEAPON,
		WP_BAMBOOBO,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_gun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_gun",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/gun/gun.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_gun",
/* pickup */	"Gun",
		100,
		IT_WEAPON,
		WP_GUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_electriclauncher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_electriclauncher",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/electricl/electricl.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_electric",
/* pickup */	"Electric Launcher",
		40,
		IT_WEAPON,
		WP_ELECTRIC_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_rocketlauncher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_rocketlauncher",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/rocketl/rocketl.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_rocket",
/* pickup */	"Rocket Launcher",
		10,
		IT_WEAPON,
		WP_ROCKET_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_hominglauncher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_hominglauncher",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/homingl/homingl.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_homing",
/* pickup */	"Homing Rocket Launcher", // "Homing Launcher"
		5,
		IT_WEAPON,
		WP_HOMING_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_grapplinghook (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_grapplinghook",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/grapple/grapple.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_grapple",
/* pickup */	"Grappling Hook",
		0,
		IT_WEAPON,
		WP_GRAPPLING_HOOK,
/* precache */ "",
/* sounds */ ""
	},
#else // !TMNTWEAPONS
/*QUAKED weapon_gauntlet (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_gauntlet", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/gauntlet/gauntlet.md3",
		NULL, NULL, NULL},
/* icon */		"icons/iconw_gauntlet",
/* pickup */	"Gauntlet",
		0,
		IT_WEAPON,
		WP_GAUNTLET,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_shotgun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_shotgun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/shotgun/shotgun.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_shotgun",
/* pickup */	"Shotgun",
		10,
		IT_WEAPON,
		WP_SHOTGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_machinegun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_machinegun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/machinegun/machinegun.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_machinegun",
/* pickup */	"Machinegun",
		40,
		IT_WEAPON,
		WP_MACHINEGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_grenadelauncher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_grenadelauncher",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/grenadel/grenadel.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_grenade",
/* pickup */	"Grenade Launcher",
		10,
		IT_WEAPON,
		WP_GRENADE_LAUNCHER,
/* precache */ "",
/* sounds */ "sound/weapons/grenade/hgrenb1a.wav sound/weapons/grenade/hgrenb2a.wav"
	},

/*QUAKED weapon_rocketlauncher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_rocketlauncher",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/rocketl/rocketl.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_rocket",
/* pickup */	"Rocket Launcher",
		10,
		IT_WEAPON,
		WP_ROCKET_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_lightning (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_lightning", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/lightning/lightning.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_lightning",
/* pickup */	"Lightning Gun",
		100,
		IT_WEAPON,
		WP_LIGHTNING,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_railgun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_railgun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/railgun/railgun.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_railgun",
/* pickup */	"Railgun",
		10,
		IT_WEAPON,
		WP_RAILGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_plasmagun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_plasmagun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons2/plasma/plasma.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_plasma",
/* pickup */	"Plasma Gun",
		50,
		IT_WEAPON,
		WP_PLASMAGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_bfg (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_bfg",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/bfg/bfg.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_bfg",
/* pickup */	"BFG10K",
		20,
		IT_WEAPON,
		WP_BFG,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_grapplinghook (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_grapplinghook",
		"sound/misc/w_pkup.wav",
        { "models/weapons2/grapple/grapple.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_grapple",
/* pickup */	"Grappling Hook",
		0,
		IT_WEAPON,
		WP_GRAPPLING_HOOK,
/* precache */ "",
/* sounds */ ""
	},
#endif // TMNTWEAPONS

#ifndef TMNTWEAPONS
	//
	// AMMO ITEMS
	//

/*QUAKED ammo_shells (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_shells",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/shotgunam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_shotgun",
/* pickup */	"Shells",
		10,
		IT_AMMO,
		WP_SHOTGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_bullets (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_bullets",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/machinegunam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_machinegun",
/* pickup */	"Bullets",
		50,
		IT_AMMO,
		WP_MACHINEGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_grenades (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_grenades",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/grenadeam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_grenade",
/* pickup */	"Grenades",
		5,
		IT_AMMO,
		WP_GRENADE_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_cells (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_cells",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/plasmaam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_plasma",
/* pickup */	"Cells",
		30,
		IT_AMMO,
		WP_PLASMAGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_lightning (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_lightning",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/lightningam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_lightning",
/* pickup */	"Lightning",
		60,
		IT_AMMO,
		WP_LIGHTNING,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_rockets (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_rockets",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/rocketam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_rocket",
/* pickup */	"Rockets",
		5,
		IT_AMMO,
		WP_ROCKET_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_slugs (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_slugs",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/railgunam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_railgun",
/* pickup */	"Slugs",
		10,
		IT_AMMO,
		WP_RAILGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_bfg (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_bfg",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/bfgam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_bfg",
/* pickup */	"Bfg Ammo",
		15,
		IT_AMMO,
		WP_BFG,
/* precache */ "",
/* sounds */ ""
	},
#endif // TMNTWEAPONS

#ifndef TMNTHOLDABLE // no q3 teleprter
	//
	// HOLDABLE ITEMS
	//
/*QUAKED holdable_teleporter (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_teleporter", 
		"sound/items/holdable.wav",
        { "models/powerups/holdable/teleporter.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/teleporter",
/* pickup */	"Personal Teleporter",
#ifdef TMNTHOLDSYS
		0,	// Only ever have one use, even if picked up 2 (or 100)
#else
		60,
#endif
		IT_HOLDABLE,
		HI_TELEPORTER,
/* precache */ "",
/* sounds */ ""
	},
#endif
/*QUAKED holdable_medkit (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_medkit", 
		"sound/items/holdable.wav",
        { 
		"models/powerups/holdable/medkit.md3", 
#ifdef TMNTHOLDABLE
		NULL,
#else
		"models/powerups/holdable/medkit_sphere.md3",
#endif
		NULL, NULL},
/* icon */		"icons/medkit",
#ifdef TMNTHOLDABLE
/* pickup */	"Pizza-to-Go",
#else
/* pickup */	"Medkit",
#endif
#ifdef TMNTHOLDSYS
		0,	// Only ever have one use, even if picked up 2 (or 100)
#else
		60,
#endif
		IT_HOLDABLE,
		HI_MEDKIT,
/* precache */ "",
/* sounds */ "sound/items/use_medkit.wav"
	},

#ifdef TMNTHOLDABLE
/*QUAKED holdable_shuriken (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_shuriken",
		"sound/items/holdable.wav",
        {
		"models/powerups/holdable/shuriken.md3",
		NULL, NULL, NULL},
/* icon */		"icons/shuriken",
/* pickup */	"Shurikens",
		10,
		IT_HOLDABLE,
		HI_SHURIKEN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED holdable_shurikenelectric (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_shurikenelectric",
		"sound/items/holdable.wav",
        {
		"models/powerups/holdable/shurikenelectric.md3",
		NULL, NULL, NULL},
/* icon */		"icons/shurikenelectric",
/* pickup */	"Electric Shurikens",
		10,
		IT_HOLDABLE,
		HI_ELECTRICSHURIKEN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED holdable_shurikenfire (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_shurikenfire",
		"sound/items/holdable.wav",
        {
		"models/powerups/holdable/shurikenfire.md3",
		NULL, NULL, NULL},
/* icon */		"icons/shurikenfire",
/* pickup */	"Fire Shurikens",
		10,
		IT_HOLDABLE,
		HI_FIRESHURIKEN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED holdable_shurikenlaser (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_shurikenlaser",
		"sound/items/holdable.wav",
        {
		"models/powerups/holdable/shurikenlaser.md3",
		NULL, NULL, NULL},
/* icon */		"icons/shurikenlaser",
/* pickup */	"Laser Shurikens",
		10,
		IT_HOLDABLE,
		HI_LASERSHURIKEN,
/* precache */ "",
/* sounds */ ""
	},
#endif

	//
	// POWERUP ITEMS
	//
/*QUAKED item_quad (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
#ifdef TMNT // POWERS
		"item_strength",
		"sound/items/strength.wav",
        { "models/powerups/instant/strength.md3",
        NULL, NULL, NULL },
/* icon */		"icons/strength",
/* pickup */	"Increased Strength",
#else
		"item_quad", 
		"sound/items/quaddamage.wav",
        { "models/powerups/instant/quad.md3", 
        "models/powerups/instant/quad_ring.md3",
		NULL, NULL },
/* icon */		"icons/quad",
/* pickup */	"Quad Damage",
#endif
		30,
		IT_POWERUP,
		PW_QUAD,
/* precache */ "",
/* sounds */ "sound/items/damage2.wav sound/items/damage3.wav"
	},

/*QUAKED item_enviro (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
#ifdef TMNT // POWERS
		"item_defense",
		"sound/items/defense.wav",
        { "models/powerups/instant/defense.md3",
		NULL, NULL, NULL },
/* icon */		"icons/defense",
/* pickup */	"Increased Defense",
#else
		"item_enviro",
		"sound/items/protect.wav",
        { "models/powerups/instant/enviro.md3", 
		"models/powerups/instant/enviro_ring.md3", 
		NULL, NULL },
/* icon */		"icons/envirosuit",
/* pickup */	"Battle Suit",
#endif
		30,
		IT_POWERUP,
		PW_BATTLESUIT,
/* precache */ "",
/* sounds */ "sound/items/airout.wav sound/items/protect3.wav"
	},

/*QUAKED item_haste (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
#ifdef TMNT // POWERS
		"item_speed",
		"sound/items/speed.wav",
        { "models/powerups/instant/speed.md3",
		NULL, NULL, NULL },
/* icon */		"icons/speed",
/* pickup */	"Increased Speed",
#else
		"item_haste",
		"sound/items/haste.wav",
        { "models/powerups/instant/haste.md3", 
		"models/powerups/instant/haste_ring.md3", 
		NULL, NULL },
/* icon */		"icons/haste",
/* pickup */	"Speed",
#endif
		30,
		IT_POWERUP,
		PW_HASTE,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_invis (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_invis",
		"sound/items/invisibility.wav",
        { "models/powerups/instant/invis.md3", 
		"models/powerups/instant/invis_ring.md3", 
		NULL, NULL },
/* icon */		"icons/invis",
/* pickup */	"Invisibility",
		30,
		IT_POWERUP,
		PW_INVIS,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_regen (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_regen",
		"sound/items/regeneration.wav",
        { "models/powerups/instant/regen.md3", 
		"models/powerups/instant/regen_ring.md3", 
		NULL, NULL },
/* icon */		"icons/regen",
/* pickup */	"Regeneration",
		30,
		IT_POWERUP,
		PW_REGEN,
/* precache */ "",
/* sounds */ "sound/items/regen.wav"
	},

/*QUAKED item_flight (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_flight",
		"sound/items/flight.wav",
        { "models/powerups/instant/flight.md3", 
		"models/powerups/instant/flight_ring.md3", 
		NULL, NULL },
/* icon */		"icons/flight",
/* pickup */	"Flight",
		60,
		IT_POWERUP,
		PW_FLIGHT,
/* precache */ "",
/* sounds */ "sound/items/flight.wav"
	},

#ifdef TMNT // POWERS
/*QUAKED item_invul (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_invul",
		"sound/items/invul.wav",
        { "models/powerups/instant/invul.md3",
        NULL, NULL, NULL },
/* icon */		"icons/invul",
/* pickup */	"Invulnerability",
		30,
		IT_POWERUP,
		PW_INVUL,
/* precache */ "",
/* sounds */ ""
	},
#endif

/*QUAKED team_CTF_redflag (1 0 0) (-16 -16 -16) (16 16 16)
Only in CTF games
*/
	{
		"team_CTF_redflag",
		NULL,
#ifdef TMNTDATASYS // FLAG_MODEL
        { "models/flag2/flagpole.md3",
		"models/flag2/flagflap3.md3", NULL, NULL },
#else
        { "models/flags/r_flag.md3",
		NULL, NULL, NULL },
#endif
/* icon */		"icons/iconf_red1",
/* pickup */	"Red Flag",
		0,
		IT_TEAM,
		PW_REDFLAG,
#ifdef TMNTDATASYS // FLAG_MODEL
		"models/flag2/red.skin"
#else
/* precache */ "",
#endif
/* sounds */ ""
	},

/*QUAKED team_CTF_blueflag (0 0 1) (-16 -16 -16) (16 16 16)
Only in CTF games
*/
	{
		"team_CTF_blueflag",
		NULL,
#ifdef TMNTDATASYS // FLAG_MODEL
        { "models/flag2/flagpole.md3",
		"models/flag2/flagflap3.md3", NULL, NULL },
#else
        { "models/flags/b_flag.md3",
		NULL, NULL, NULL },
#endif
/* icon */		"icons/iconf_blu1",
/* pickup */	"Blue Flag",
		0,
		IT_TEAM,
		PW_BLUEFLAG,
#ifdef TMNTDATASYS // FLAG_MODEL
		"models/flag2/blue.skin"
#else
/* precache */ "",
#endif
/* sounds */ ""
	},

#ifdef MISSIONPACK
#ifndef TMNTHOLDABLE // NO_KAMIKAZE_ITEM
/*QUAKED holdable_kamikaze (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_kamikaze", 
		"sound/items/holdable.wav",
        { "models/powerups/kamikazi.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/kamikaze",
/* pickup */	"Kamikaze",
#ifdef TMNTHOLDSYS
		0,	// Only ever has one use, even if picked up 2 (or 100)
#else
		60,
#endif
		IT_HOLDABLE,
		HI_KAMIKAZE,
/* precache */ "",
/* sounds */ "sound/items/kamikazerespawn.wav"
	},
#endif

/*QUAKED holdable_portal (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_portal", 
		"sound/items/holdable.wav",
        { "models/powerups/holdable/porter.md3",
		NULL, NULL, NULL},
/* icon */		"icons/portal",
/* pickup */	"Portal",
#ifdef TMNTHOLDSYS
		0,	// Only ever has one use, even if picked up 2 (or 100)
#else
		60,
#endif
		IT_HOLDABLE,
		HI_PORTAL,
/* precache */ "",
/* sounds */ ""
	},

#ifndef TMNT // POWERS
/*QUAKED holdable_invulnerability (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"holdable_invulnerability", 
		"sound/items/holdable.wav",
        { "models/powerups/holdable/invulnerability.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/invulnerability",
/* pickup */	"Invulnerability",
#ifdef TMNTHOLDSYS
		0,	// Only ever has one use, even if picked up 2 (or 100)
#else
		60,
#endif
		IT_HOLDABLE,
		HI_INVULNERABILITY,
/* precache */ "",
/* sounds */ ""
	},
#endif

#ifndef TMNTWEAPONS
/*QUAKED ammo_nails (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_nails",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/nailgunam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_nailgun",
/* pickup */	"Nails",
		20,
		IT_AMMO,
		WP_NAILGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_mines (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_mines",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/proxmineam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_proxlauncher",
/* pickup */	"Proximity Mines",
		10,
		IT_AMMO,
		WP_PROX_LAUNCHER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_belt (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_belt",
		"sound/misc/am_pkup.wav",
        { "models/powerups/ammo/chaingunam.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/icona_chaingun",
/* pickup */	"Chaingun Belt",
		100,
		IT_AMMO,
		WP_CHAINGUN,
/* precache */ "",
/* sounds */ ""
	},
#endif // !TMNTWEAPONS

	//
	// PERSISTANT POWERUP ITEMS
	//
/*QUAKED item_scout (.3 .3 1) (-16 -16 -16) (16 16 16) suspended redTeam blueTeam
*/
	{
		"item_scout",
		"sound/items/scout.wav",
        { "models/powerups/scout.md3", 
		NULL, NULL, NULL },
/* icon */		"icons/scout",
/* pickup */	"Scout",
		30,
		IT_PERSISTANT_POWERUP,
		PW_SCOUT,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_guard (.3 .3 1) (-16 -16 -16) (16 16 16) suspended redTeam blueTeam
*/
	{
		"item_guard",
		"sound/items/guard.wav",
        { "models/powerups/guard.md3", 
		NULL, NULL, NULL },
/* icon */		"icons/guard",
/* pickup */	"Guard",
		30,
		IT_PERSISTANT_POWERUP,
		PW_GUARD,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_doubler (.3 .3 1) (-16 -16 -16) (16 16 16) suspended redTeam blueTeam
*/
	{
		"item_doubler",
		"sound/items/doubler.wav",
        { "models/powerups/doubler.md3", 
		NULL, NULL, NULL },
/* icon */		"icons/doubler",
/* pickup */	"Doubler",
		30,
		IT_PERSISTANT_POWERUP,
		PW_DOUBLER,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_doubler (.3 .3 1) (-16 -16 -16) (16 16 16) suspended redTeam blueTeam
*/
	{
		"item_ammoregen",
		"sound/items/ammoregen.wav",
        { "models/powerups/ammo.md3",
		NULL, NULL, NULL },
/* icon */		"icons/ammo_regen",
/* pickup */	"Ammo Regen",
		30,
		IT_PERSISTANT_POWERUP,
		PW_AMMOREGEN,
/* precache */ "",
/* sounds */ ""
	},

	/*QUAKED team_CTF_neutralflag (0 0 1) (-16 -16 -16) (16 16 16)
Only in One Flag CTF games
*/
	{
		"team_CTF_neutralflag",
		NULL,
#ifdef TMNTDATASYS // FLAG_MODEL
        { "models/flag2/flagpole.md3",
		"models/flag2/flagflap3.md3", NULL, NULL },
#else
        { "models/flags/n_flag.md3",
		NULL, NULL, NULL },
#endif
/* icon */		"icons/iconf_neutral1",
/* pickup */	"Neutral Flag",
		0,
		IT_TEAM,
		PW_NEUTRALFLAG,
#ifdef TMNTDATASYS // FLAG_MODEL
		"models/flag2/white.skin",
#else
/* precache */ "",
#endif
/* sounds */ ""
	},

#ifdef MISSIONPACK_HARVESTER
	{
		"item_redcube",
		"sound/misc/am_pkup.wav",
        { "models/powerups/orb/r_orb.md3",
		NULL, NULL, NULL },
/* icon */		"icons/iconh_rorb",
/* pickup */	"Red Cube",
		0,
		IT_TEAM,
		0,
/* precache */ "",
/* sounds */ ""
	},

	{
		"item_bluecube",
		"sound/misc/am_pkup.wav",
        { "models/powerups/orb/b_orb.md3",
		NULL, NULL, NULL },
/* icon */		"icons/iconh_borb",
/* pickup */	"Blue Cube",
		0,
		IT_TEAM,
		0,
/* precache */ "",
/* sounds */ ""
	},
#endif
#ifndef TMNTWEAPONS
#if 0 // #ifdef TMNTWEAPSYS_2 // Keep the model index correct for items after weapons...
	{ EMPTY_GITEM },{ EMPTY_GITEM },{ EMPTY_GITEM },
#else
/*QUAKED weapon_nailgun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_nailgun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons/nailgun/nailgun.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_nailgun",
/* pickup */	"Nailgun",
		10,
		IT_WEAPON,
		WP_NAILGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_prox_launcher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_prox_launcher", 
		"sound/misc/w_pkup.wav",
        { "models/weapons/proxmine/proxmine.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_proxlauncher",
/* pickup */	"Prox Launcher",
		5,
		IT_WEAPON,
		WP_PROX_LAUNCHER,
/* precache */ "",
/* sounds */ "sound/weapons/proxmine/wstbtick.wav "
			"sound/weapons/proxmine/wstbactv.wav "
			"sound/weapons/proxmine/wstbimpl.wav "
			"sound/weapons/proxmine/wstbimpm.wav "
			"sound/weapons/proxmine/wstbimpd.wav "
			"sound/weapons/proxmine/wstbactv.wav"
	},

/*QUAKED weapon_chaingun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_chaingun", 
		"sound/misc/w_pkup.wav",
        { "models/weapons/vulcan/vulcan.md3", 
		NULL, NULL, NULL},
/* icon */		"icons/iconw_chaingun",
/* pickup */	"Chaingun",
		80,
		IT_WEAPON,
		WP_CHAINGUN,
/* precache */ "",
/* sounds */ "sound/weapons/vulcan/wvulwind.wav"
	},
#endif // TMNTWEAPSYS_2
#endif // TMNTWEAPONS
#endif

#ifdef TMNT // CRATE
	{
		"item_crate",
		NULL,
        { "models/crates/normal/normal.md3",
		NULL, NULL, NULL},
/* icon */		"icons/crate",
/* pickup */	"Crate",
		80,
		IT_CRATE,
		0,
/* precache */ "",
/* sounds */ ""  // Crate break sound?
	},
#endif

	// end of list marker
	{NULL}
};

int		bg_numItems = sizeof(bg_itemlist) / sizeof(bg_itemlist[0]) - 1;

#ifdef TMNTWEAPSYS
#ifdef TMNTWEAPSYS_2
bg_projectileinfo_t bg_projectileinfo[MAX_BG_PROJ];
bg_weaponinfo_t bg_weaponinfo[MAX_BG_WEAPONS];
bg_weapongroupinfo_t bg_weapongroupinfo[MAX_BG_WEAPON_GROUPS];
static qboolean bg_weaponsys_init = qfalse;

// These are in game, cgame, and ui, but not in bg - so its okay to use here...
int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void	trap_FS_Read( void *buffer, int len, fileHandle_t f );
void	trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void	trap_FS_FCloseFile( fileHandle_t f );
//#ifndef CGAME // FIXME
int		trap_FS_GetFileList( const char *path, const char *extension, char *listbuf, int bufsize );
//#endif

int BG_ProjectileIndexForName(const char *name)
{
	int i;
	for (i = 0; i < MAX_BG_PROJ; i++)
	{
		if ( !Q_stricmp( bg_projectileinfo[i].name, name))
		{
			return i;
		}
	}

	return 0;
}

int BG_WeaponIndexForName(const char *name)
{
	int i;
	for (i = 0; i < MAX_BG_WEAPONS; i++)
	{
		if ( !Q_stricmp( bg_weaponinfo[i].name, name))
		{
			return i;
		}
	}

	return 0;
}

int BG_WeaponGroupIndexForName(const char *name)
{
	int i;
	for (i = 0; i < MAX_BG_WEAPON_GROUPS; i++)
	{
		if ( !Q_stricmp( bg_weapongroupinfo[i].name, name))
		{
			return i;
		}
	}

	return 0;
}

int BG_NumProjectiles(void)
{
	int i;
	for (i = 0; i < MAX_BG_PROJ; i++)
	{
		if ( strlen( bg_projectileinfo[i].name ) < 1 )
		{
			return i;
		}
	}

	return 0;
}

int BG_NumWeapons(void)
{
	int i;
	for (i = 0; i < MAX_BG_WEAPONS; i++)
	{
		if ( strlen( bg_weaponinfo[i].name ) < 1 )
		{
			return i;
		}
	}

	return 0;
}

int BG_NumWeaponGroups(void)
{
	int i;
	for (i = 0; i < MAX_BG_WEAPON_GROUPS; i++)
	{
		if ( strlen( bg_weapongroupinfo[i].name ) < 1 )
		{
			return i;
		}
	}

	return 0;
}

//projectile trail types
const char *pt_names[] =
{
	"PT_NONE",
	"PT_PLASMA",
	"PT_ROCKET",
	"PT_GRENADE",
	"PT_GRAPPLE",
	"PT_NAIL",
	"PT_LIGHTNING",
	"PT_RAIL",
	"PT_BULLET",
	NULL
};

//projectile death types
const char *pd_names[] =
	{
	"PD_NONE",
	"PD_PLASMA",
	"PD_ROCKET",
	"PD_GRENADE",
	"PD_BULLET",
	"PD_RAIL",
	"PD_BFG",
	"PD_LIGHTNING",
	"PD_ROCKET_SMALL",
	NULL
};

//projectile death types
const char *ps_names[] =
{
	"PS_ROLL",
	"PS_NONE",
	"PS_PITCH",
	"PD_YAW",
	NULL
};

//projectile bounce types
const char *pb_names[] =
{
	"PB_NONE",
	"PB_HALF",
	"PB_FULL",
	NULL
};

static qboolean Projectile_Parse(char **p) {
	char *token;
	bg_projectileinfo_t projectile;
	int i;

	memset(&projectile, 0, sizeof(projectile));
	projectile.numProjectiles = 1;

	token = COM_ParseExt(p, qtrue);
	if ( !*token ) {
		Com_Printf("Warning: projectile missing name\n");
		return qfalse;
	}
	Com_sprintf(projectile.name, sizeof (projectile.name), "%s", token);

	//Com_Printf("Loading projectile [%s] ...\n", weapon.name);

	token = COM_ParseExt(p, qtrue);

	if (token[0] != '{') {
	return qfalse;
}

	while ( 1 ) {
		token = COM_ParseExt(p, qtrue);

		if (Q_stricmp(token, "}") == 0) {
			int num = BG_ProjectileIndexForName(projectile.name);
			if ((projectile.damagetype & DAMAGETYPE_RADIAL) && !projectile.splashDamage)
			{
				projectile.splashDamage = projectile.damage;
			}
			if (num)
{
				// Already loaded!
				memcpy(&bg_projectileinfo[num], &projectile, sizeof (projectile));
			}
			else
	{
				memcpy(&bg_projectileinfo[BG_NumProjectiles()], &projectile, sizeof (projectile));
			}
			//Com_Printf("Loaded projectile [%s]\n", projectile.name);
		return qtrue;
	}

		if ( !token || token[0] == 0 ) {
	return qfalse;
}

		if ( !Q_stricmp( token, "clone" ) ) {
			char name[MAX_QPATH]; // Save name
			int num;
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			num = BG_ProjectileIndexForName(token);
			if (num) {
				Q_strncpyz(name, projectile.name, MAX_QPATH); // backup name
				memcpy(&projectile, &bg_projectileinfo[num], sizeof (projectile));
				Q_strncpyz(projectile.name, name, MAX_QPATH); // use backed up name
			}
			continue;
		} else if ( !Q_stricmp( token, "model" ) ) {
			token = COM_Parse( p );
			if ( *token )
			{
				Com_sprintf(projectile.model, sizeof (projectile.model), "%s", token);
			}
			else
			{
				projectile.model[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "damagetype" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.damagetype = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "damage" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.damage = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "splashdamage" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.splashDamage = atoi( token );
			continue;
		} else if ( !Q_stricmp( token, "splashRadius" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.splashRadius = atof(token);
			continue;
		} else if ( !Q_stricmp( token, "speed" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.speed = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "timetolive" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.timetolive = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "shootable" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.shootable = (atoi(token) != 0);
			continue;
		} else if ( !Q_stricmp( token, "trailType" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; pt_names[i] != NULL; i++)
			{
				if ( !Q_stricmp( token, pt_names[i] ) ) {
					projectile.trailType = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "deathType" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; pd_names[i] != NULL; i++)
			{
				if ( !Q_stricmp( token, pd_names[i] ) ) {
					projectile.deathType = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "missileDlight" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.missileDlight = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "missileSound" ) ) {
			token = COM_Parse( p );
			if ( *token )
			{
				Com_sprintf(projectile.missileSoundName, sizeof (projectile.missileSoundName), "%s", token);
			}
			continue;
		} else if ( !Q_stricmp( token, "missileDlightColor" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( p );
				if ( !*token ) {
					break;
				}
				projectile.missileDlightColor[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "sprite" ) ) {
			token = COM_Parse( p );
			if ( *token )
			{
				Com_sprintf(projectile.sprite, sizeof (projectile.sprite), "%s", token);
			}
			else
			{
				projectile.sprite[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "spriteRadius" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.spriteRadius = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "spinType" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; ps_names[i] != NULL; i++)
			{
				if ( !Q_stricmp( token, ps_names[i] ) ) {
					projectile.spinType = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "bounceType" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; pb_names[i] != NULL; i++)
			{
				if ( !Q_stricmp( token, pb_names[i] ) ) {
					projectile.bounceType = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "maxBounces" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.maxBounces = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "wallmarkName" ) ) {
			token = COM_Parse( p );
			if ( *token )
			{
				Com_sprintf(projectile.wallmarkName, sizeof (projectile.wallmarkName), "%s", token);
			}
			else
			{
				projectile.wallmarkName[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "wallmarkRadius" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.wallmarkRadius = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "wallmarkFadeAlpha" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				projectile.flags |= PF_WALLMARK_FADE_ALPHA;
			else
				projectile.flags &= ~PF_WALLMARK_FADE_ALPHA;
			continue;
		} else if ( !Q_stricmp( token, "wallmarkColorize" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				projectile.flags |= PF_WALLMARK_COLORIZE;
			else
				projectile.flags &= ~PF_WALLMARK_COLORIZE;
			continue;
		} else if ( !Q_stricmp( token, "explosionColorize" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				projectile.flags |= PF_EXPLOSION_COLORIZE;
			else
				projectile.flags &= ~PF_EXPLOSION_COLORIZE;
			continue;
		} else if ( !Q_stricmp( token, "useGravity" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				projectile.flags |= PF_USE_GRAVITY;
			else
				projectile.flags &= ~PF_USE_GRAVITY;
			continue;
		} else if ( !Q_stricmp( token, "stickOnImpact" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.stickOnImpact = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "falltoground" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.falltoground = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "spdRndAdd" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.spdRndAdd = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "spread" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.spread = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "numProjectiles" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.numProjectiles = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "homing" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.homing = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "grappling" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.grappling = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "instantDamage" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			projectile.instantDamage = atoi(token);
			continue;
		}

		Com_Printf( "unknown token '%s' in projectile %s\n", token, projectile.name );
	}
	return qfalse;
}

static qboolean WeaponBlade_Parse(char **p, bg_weaponinfo_t *weapon) {
	char *token;
	int bladeNum = 0;
	bg_bladeinfo_t *blade = NULL;
	qboolean validBlade = qtrue;
	int i;

	token = COM_ParseExt(p, qtrue);

	if (token[0] >= '0' && token[0] <= '9')
	{
		bladeNum = atoi(token);
		token = COM_ParseExt(p, qtrue);
	}
	else
	{
		for (i=0; i < MAX_WEAPON_BLADES; i++)
		{
			if (weapon->blades[i].damage == 0)
			{
				// Make sure it is a clean blade.
				Com_Memset(&weapon->blades[i], 0, sizeof (bg_bladeinfo_t));
				break;
			}
		}
		bladeNum = i;
	}

	if (token[0] != '{') {
		return qfalse;
	}

	validBlade = (bladeNum < MAX_WEAPON_BLADES);
	if (!validBlade)
	{
		Com_Printf("Weapon [%s]: Invalid blade number %d\n", weapon->name, bladeNum);
	}
	else
	{
		blade = &weapon->blades[bladeNum];
	}

	while ( 1 ) {
		token = COM_ParseExt(p, qtrue);

		if (Q_stricmp(token, "}") == 0) {
			return qtrue;
		}

		if ( !token || token[0] == 0 ) {
			return qfalse;
		}

		// Just skipping this blade.
		if (!validBlade)
		{
			continue;
		}

		if ( !Q_stricmp( token, "origin" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( p );
				if ( !*token ) {
					break;
				}
				blade->origin[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "tip" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( p );
				if ( !*token ) {
					break;
				}
				blade->tip[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "damage" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			blade->damage = atoi( token );
			continue;
		} else if ( !Q_stricmp( token, "trailStyle" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}

			if (Q_stricmp(token, "none") == 0) {
				blade->trailStyle = TRAIL_NONE;
			}
			else if (Q_stricmp(token, "normal") == 0) {
				blade->trailStyle = TRAIL_NORMAL;
			}
			continue;
		}
		//Com_Printf( "unknown token '%s' [in blade] in %s\n", token, filename );
		Com_Printf( "unknown token '%s' [in blade]\n", token );
	}
	return qfalse;
}

// Copied from g_combat.c
char	*modNames[] = {
	"MOD_UNKNOWN",
#ifdef TMNTWEAPONS // MOD
	"MOD_FIST",
	"MOD_KATANA",
	"MOD_WAKIZASHI",
	"MOD_SAI",
	"MOD_NUNCHUK",
	"MOD_HAMMER",
	"MOD_AXE",
	"MOD_SWORD",
	"MOD_BAT",
	"MOD_BO",
	"MOD_BAMBOOBO",
	"MOD_GUN",
	"MOD_ELECTRIC",
	"MOD_ELECTRIC_SPLASH",
	"MOD_ROCKET",
	"MOD_ROCKET_SPLASH",
	"MOD_HOMING",
	"MOD_HOMING_SPLASH",
#else
	"MOD_SHOTGUN",
	"MOD_GAUNTLET",
	"MOD_MACHINEGUN",
	"MOD_GRENADE",
	"MOD_GRENADE_SPLASH",
	"MOD_ROCKET",
	"MOD_ROCKET_SPLASH",
	"MOD_PLASMA",
	"MOD_PLASMA_SPLASH",
	"MOD_RAILGUN",
	"MOD_LIGHTNING",
	"MOD_BFG",
	"MOD_BFG_SPLASH",
#endif
#ifdef TMNTHOLDABLE
	"MOD_SHURIKEN",
	"MOD_FIRESHURIKEN",
	"MOD_FIRESHURIKEN_EXPLOSION",
	"MOD_ELECTRICSHURIKEN",
	"MOD_LASERSHURIKEN",
#endif
	"MOD_WATER",
	"MOD_SLIME",
	"MOD_LAVA",
	"MOD_CRUSH",
	"MOD_TELEFRAG",
	"MOD_FALLING",
	"MOD_SUICIDE",
	"MOD_TARGET_LASER",
	"MOD_TRIGGER_HURT",
#ifdef TMNTENTSYS
	"MOD_EXPLOSION",
#endif
#ifdef MISSIONPACK
#ifndef TMNTWEAPONS // MOD
	"MOD_NAIL",
	"MOD_CHAINGUN",
	"MOD_PROXIMITY_MINE",
#endif
#ifndef TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	"MOD_KAMIKAZE",
#endif
#ifndef TMNTWEAPONS // MOD
	"MOD_JUICED",
#endif
#endif
	"MOD_GRAPPLE"
};
int modNamesSize = sizeof( modNames ) / sizeof( modNames[0] );

const char *weapontypeNames[WT_MAX] = {
	"WT_NONE",
	"WT_GAUNTLET",
	"WT_GUN",
	"WT_MELEE"
};

static qboolean Weapon_Parse(char **p) {
	char *token;
	bg_weaponinfo_t weapon;
	int i;

	memset(&weapon, 0, sizeof(weapon));

	weapon.attackDelay = 400;
	weapon.flashColor[0] = 1.0f;
	weapon.flashColor[1] = 1.0f;
	weapon.flashColor[2] = 1.0f;
	weapon.wallmarkRadius = 16;

	token = COM_ParseExt(p, qtrue);
	if ( !*token ) {
		Com_Printf("Warning: weapon missing name\n");
		return qfalse;
	}
	Com_sprintf(weapon.name, sizeof (weapon.name), "%s", token);

	//Com_Printf("Loading weapon [%s] ...\n", weapon.name);

	token = COM_ParseExt(p, qtrue);

	if (token[0] != '{') {
		return qfalse;
	}

	while ( 1 ) {
		token = COM_ParseExt(p, qtrue);

		if (Q_stricmp(token, "}") == 0) {
			int num = BG_WeaponIndexForName(weapon.name);
			if (!num)
			{
				// Use free slot
				num = BG_NumWeapons();
			}

			Com_Memcpy(&bg_weaponinfo[num], &weapon, sizeof (weapon));
			//Com_Printf("Loaded weapon [%s]\n", weapon.name);
			return qtrue;
		}

		if ( !token || token[0] == 0 ) {
			return qfalse;
		}

		if (Q_stricmp(token, "blade") == 0) {
			if (WeaponBlade_Parse(p, &weapon))
				continue;
			else
				break;
		} else if ( !Q_stricmp( token, "clone" ) ) {
			char name[MAX_QPATH]; // Save name
			int num;
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			num = BG_WeaponIndexForName(token);
			if (num) {
				Q_strncpyz(name, weapon.name, MAX_QPATH); // backup name
				memcpy(&weapon, &bg_weaponinfo[num], sizeof (weapon));
				Q_strncpyz(weapon.name, name, MAX_QPATH); // use backed up name
			}
			continue;
		} else if ( !Q_stricmp( token, "model" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.model, sizeof (weapon.model), "%s", token);
			}
			else
			{
				weapon.model[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "weapontype" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; i < WT_MAX; i++)
			{
				if ( !Q_stricmp( token, weapontypeNames[i] ) ) {
					weapon.weapontype = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "attackDelay" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			weapon.attackDelay = atoi( token );
			continue;
		} else if ( !Q_stricmp( token, "wallmarkName" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.wallmarkName, sizeof (weapon.wallmarkName), "%s", token);
			}
			else
			{
				weapon.wallmarkName[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "wallmarkRadius" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			weapon.wallmarkRadius = atoi( token );
			continue;
		} else if ( !Q_stricmp( token, "wallmarkFadeAlpha" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_WALLMARK_FADE_ALPHA;
			else
				weapon.flags &= ~WIF_WALLMARK_FADE_ALPHA;
			continue;
		} else if ( !Q_stricmp( token, "wallmarkColorize" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_WALLMARK_COLORIZE;
			else
				weapon.flags &= ~WIF_WALLMARK_COLORIZE;
			continue;
		} else if ( !Q_stricmp( token, "alwaysDamage" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_ALWAYS_DAMAGE;
			else
				weapon.flags &= ~WIF_ALWAYS_DAMAGE;
			continue;
		} else if ( !Q_stricmp( token, "continuousFlash" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_CONTINUOUS_FLASH;
			else
				weapon.flags &= ~WIF_CONTINUOUS_FLASH;
			continue;
		} else if ( !Q_stricmp( token, "ejectBrass" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_EJECT_BRASS;
			else
				weapon.flags &= ~WIF_EJECT_BRASS;
			continue;
		} else if ( !Q_stricmp( token, "ejectBrass2" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_EJECT_BRASS2;
			else
				weapon.flags &= ~WIF_EJECT_BRASS2;
			continue;
		} else if ( !Q_stricmp( token, "ejectSmoke" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			if (atoi(token) == 1)
				weapon.flags |= WIF_EJECT_SMOKE;
			else
				weapon.flags &= ~WIF_EJECT_SMOKE;
			continue;
		} else if ( !Q_stricmp( token, "mod" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; i < MOD_MAX; i++)
			{
				if ( !Q_stricmp( token, modNames[i] ) ) {
					weapon.mod = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "splashMod" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; i < MOD_MAX; i++)
			{
				if ( !Q_stricmp( token, modNames[i] ) ) {
					weapon.splashMod = i;
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "flashColor" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( p );
				if ( !*token ) {
					break;
				}
				weapon.flashColor[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "projectile" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				weapon.proj = 0;
				continue;
			}
			weapon.proj = BG_ProjectileIndexForName(token);
			continue;
		} else if ( !Q_stricmp( token, "readySound" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.readySoundName, sizeof (weapon.readySoundName), "%s", token);
			} else {
				weapon.readySoundName[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "firingSound" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.firingSoundName, sizeof (weapon.firingSoundName), "%s", token);
			} else {
				weapon.firingSoundName[0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "flashSound0" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.flashSoundName[0], sizeof (weapon.flashSoundName[0]), "%s", token);
			} else {
				weapon.flashSoundName[0][0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "flashSound1" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.flashSoundName[1], sizeof (weapon.flashSoundName[1]), "%s", token);
			} else {
				weapon.flashSoundName[1][0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "flashSound2" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.flashSoundName[2], sizeof (weapon.flashSoundName[2]), "%s", token);
			} else {
				weapon.flashSoundName[2][0] = '\0';
			}
			continue;
		} else if ( !Q_stricmp( token, "flashSound3" ) ) {
			token = COM_Parse( p );
			if ( *token ) {
				Com_sprintf(weapon.flashSoundName[3], sizeof (weapon.flashSoundName[3]), "%s", token);
			} else {
				weapon.flashSoundName[3][0] = '\0';
			}
			continue;
		}

		Com_Printf( "unknown token '%s' in weapon %s\n", token, weapon.name );
	}
	return qfalse;
}

// from code/qcommon/common.c
#define XSTRING(x)				STRING(x)
#define STRING(x)					#x
const char *playerAnimationNames[MAX_TOTALANIMATIONS] = {
	XSTRING(BOTH_DEATH1),
	XSTRING(BOTH_DEAD1),
	XSTRING(BOTH_DEATH2),
	XSTRING(BOTH_DEAD2),
	XSTRING(BOTH_DEATH3),
	XSTRING(BOTH_DEAD3),

	XSTRING(TORSO_GESTURE),

	XSTRING(TORSO_ATTACK),
	XSTRING(TORSO_ATTACK2),

	XSTRING(TORSO_DROP),
	XSTRING(TORSO_RAISE),

	XSTRING(TORSO_STAND),
	XSTRING(TORSO_STAND2),

	XSTRING(LEGS_WALKCR),
	XSTRING(LEGS_WALK),
	XSTRING(LEGS_RUN),
	XSTRING(LEGS_BACK),
	XSTRING(LEGS_SWIM),

	XSTRING(LEGS_JUMP),
	XSTRING(LEGS_LAND),

	XSTRING(LEGS_JUMPB),
	XSTRING(LEGS_LANDB),

	XSTRING(LEGS_IDLE),
	XSTRING(LEGS_IDLECR),

	XSTRING(LEGS_TURN),

	XSTRING(TORSO_GETFLAG),
	XSTRING(TORSO_GUARDBASE),
	XSTRING(TORSO_PATROL),
	XSTRING(TORSO_FOLLOWME),
	XSTRING(TORSO_AFFIRMATIVE),
	XSTRING(TORSO_NEGATIVE),

#ifdef TMNTPLAYERS // New TMNT player animations
	// Place default weapons somewhere on there person while there not used.
	// TORSO_***DEFAULT_SECONDARY for Don should be
	//  switching to/from two handed Bo to using one hand.
	// Turtle Man: NOTE: Currently first half to going to weapon spot, second half is moving back.
	// Turtle Man: TODO: Should I split into 2 animations?
	XSTRING(TORSO_PUTDEFAULT_BOTH),
	XSTRING(TORSO_PUTDEFAULT_PRIMARY),
	XSTRING(TORSO_PUTDEFAULT_SECONDARY),
	XSTRING(TORSO_GETDEFAULT_BOTH),
	XSTRING(TORSO_GETDEFAULT_PRIMARY),
	XSTRING(TORSO_GETDEFAULT_SECONDARY),

	// Pickup weapons should support CTF flags too?
	// TORSO_PUT_SECONDARY,
	// TORSO_GET_SECONDARY,

	// Gun-type standing animations
	//TORSO_STAND_GUNTLET, // Would be the same as TORSO_STAND2...
    //TORSO_STAND_GUN, // Would be the same as TORSO_STAND...
    XSTRING(TORSO_STAND_GUN_PRIMARY), // I could reuse TORSO_STAND2 even though its for the gauntlet...

	// Melee weapon standing animations
    XSTRING(TORSO_STAND_SWORD1_BOTH),
    XSTRING(TORSO_STAND_SWORD1_PRIMARY),

    XSTRING(TORSO_STAND_SWORD2),
    XSTRING(TORSO_STAND_DAISHO),
    XSTRING(TORSO_STAND_SAI2),
    XSTRING(TORSO_STAND_SAI1_PRIMARY),

    XSTRING(TORSO_STAND_BO),
    XSTRING(TORSO_STAND_BO_PRIMARY),

    XSTRING(TORSO_STAND_HAMMER),
    XSTRING(TORSO_STAND_HAMMER_PRIMARY),

    XSTRING(TORSO_STAND_NUNCHUKS),
    XSTRING(TORSO_STAND_NUNCHUKS1_PRIMARY),

	// Gun attacks
    //TORSO_ATTACK_GUNTLET, // Would be the same as TORSO_ATTACK2...
    //TORSO_ATTACK_GUN, // Would be the same as TORSO_ATTACK...
    XSTRING(TORSO_ATTACK_GUN_PRIMARY), // Can't reuse TORSO_ATTACK2 needs a new animation.

    // Turtle Man: TODO: Attack animations, how many anims per type?...
    //       Only one for WT_HAMMER and WT_GUN
    XSTRING(TORSO_ATTACK_SWORD1_BOTH),
    //TORSO_ATTACK_SWORD1_BOTH_2,
    //TORSO_ATTACK_SWORD1_BOTH_3,

    XSTRING(TORSO_ATTACK_SWORD1_PRIMARY),
    //TORSO_ATTACK_SWORD1_PRIMARY_2,
    //TORSO_ATTACK_SWORD1_PRIMARY_3,

    XSTRING(TORSO_ATTACK_SWORD2),
    XSTRING(TORSO_ATTACK_DAISHO),
    XSTRING(TORSO_ATTACK_SAI2),
    XSTRING(TORSO_ATTACK_SAI1_PRIMARY),

    XSTRING(TORSO_ATTACK_BO),
    XSTRING(TORSO_ATTACK_BO_PRIMARY),

    XSTRING(TORSO_ATTACK_HAMMER),
    XSTRING(TORSO_ATTACK_HAMMER_PRIMARY),

    XSTRING(TORSO_ATTACK_NUNCHUKS),
    XSTRING(TORSO_ATTACK_NUNCHUKS1_PRIMARY),
#endif

#ifndef IOQ3ZTM
	XSTRING(MAX_ANIMATIONS),
#endif

	XSTRING(LEGS_BACKCR),
	XSTRING(LEGS_BACKWALK),
#ifndef IOQ3ZTM // FLAG_ANIMATIONS
	XSTRING(FLAG_RUN),
	XSTRING(FLAG_STAND),
	XSTRING(FLAG_STAND2RUN),
#endif
};

static qboolean WeaponGroupAnims_Parse(char **p, bg_weapongroup_anims_t *anims) {
	char *token;
	int i;

	token = COM_ParseExt(p, qtrue);

	if (token[0] != '{') {
		return qfalse;
	}

	while ( 1 ) {
		token = COM_ParseExt(p, qtrue);

		if (Q_stricmp(token, "}") == 0) {
			return qtrue;
		}

		if ( !token || token[0] == 0 ) {
			return qfalse;
		}

		if (Q_stricmp(token, "standAnim") == 0) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; i < MAX_TOTALANIMATIONS; i++)
			{
				if (Q_stricmp(token, playerAnimationNames[i]) == 0)
				{
					anims->standAnim = i;
					break;
				}
			}
			if (i >= MAX_TOTALANIMATIONS)
			{
				// failed to find token.
				Com_Printf("unknown animation name %s in weapon group anims\n", token);
			}
			continue;
		} else if (Q_stricmp(token, "attackAnim") == 0) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			for (i = 0; i < MAX_TOTALANIMATIONS; i++)
			{
				if (Q_stricmp(token, playerAnimationNames[i]) == 0)
				{
					anims->attackAnim = i;
					break;
				}
			}
			if (i >= MAX_TOTALANIMATIONS)
			{
				// failed to find token.
				Com_Printf("unknown animation name %s in weapon group anims\n", token);
			}
			continue;
		}
		//Com_Printf( "unknown token '%s' [in animation black] in %s\n", token, filename );
		Com_Printf( "unknown token '%s' [in animation black]\n", token );
	}
	return qfalse;
}

static qboolean WeaponGroup_Parse(char **p) {
	char *token;
	bg_weapongroupinfo_t weaponGroup;

	memset(&weaponGroup, 0, sizeof(weaponGroup));

	weaponGroup.normalAnims.standAnim = TORSO_STAND;
	weaponGroup.normalAnims.attackAnim = TORSO_ATTACK;
	weaponGroup.primaryAnims.standAnim = TORSO_STAND2;
	weaponGroup.primaryAnims.attackAnim = TORSO_ATTACK2;

	// Setup item integers
	weaponGroup.item.quantity = 0; // ammo
	weaponGroup.item.giType = IT_WEAPON;
	weaponGroup.item.giTag = -1; // Doesn't have a weapon number yet.

	// Default sound names
	Com_sprintf(weaponGroup.pickupSound, sizeof (weaponGroup.pickupSound), "sound/misc/w_pkup.wav");

	// Set weapon pointers (so I don't always have to check if it is NULL).
	weaponGroup.weapon[0] = &bg_weaponinfo[0];
	weaponGroup.weapon[1] = &bg_weaponinfo[0];

	token = COM_ParseExt(p, qtrue);
	if ( !*token ) {
		Com_Printf("Warning: weapongroup missing name\n");
		return qfalse;
	}
	Com_sprintf(weaponGroup.name, sizeof (weaponGroup.name), "%s", token);

	token = COM_ParseExt(p, qtrue);
	if (token[0] != '{') {
		return qfalse;
	}

	while ( 1 ) {
		token = COM_ParseExt(p, qtrue);

		if (Q_stricmp(token, "}") == 0) {
			int num = BG_WeaponGroupIndexForName(weaponGroup.name);
			if (!num)
			{
				// Use free slot.
				num = BG_NumWeaponGroups();
			}

			Com_Memcpy(&bg_weapongroupinfo[num], &weaponGroup, sizeof (weaponGroup));

			// Setup item pointers
			bg_weapongroupinfo[num].item.classname = bg_weapongroupinfo[num].itemName;
			bg_weapongroupinfo[num].item.pickup_sound = bg_weapongroupinfo[num].pickupSound;
			bg_weapongroupinfo[num].item.world_model[0] = bg_weapongroupinfo[num].pickupModel;
			bg_weapongroupinfo[num].item.icon = bg_weapongroupinfo[num].iconName;
			bg_weapongroupinfo[num].item.pickup_name = bg_weapongroupinfo[num].pickupName;
			bg_weapongroupinfo[num].item.giTag = num;

			//Com_Printf("Loaded weapon group [%s]\n", weaponGroup.name);
			return qtrue;
		}

		if ( !token || token[0] == 0 ) {
			return qfalse;
		}

		if (Q_stricmp(token, "normalAnims") == 0) {
			if (WeaponGroupAnims_Parse(p, &weaponGroup.normalAnims))
				continue;
			else
				break;
		} else if (Q_stricmp(token, "primaryAnims") == 0) {
			if (WeaponGroupAnims_Parse(p, &weaponGroup.primaryAnims))
				continue;
			else
				break;
		} else if ( !Q_stricmp( token, "clone" ) ) {
			char name[MAX_QPATH]; // Save name
			int num;
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			num = BG_WeaponGroupIndexForName(token);
			if (num) {
				Q_strncpyz(name, weaponGroup.name, MAX_QPATH); // backup name
				memcpy(&weaponGroup, &bg_weapongroupinfo[num], sizeof (weaponGroup));
				Q_strncpyz(weaponGroup.name, name, MAX_QPATH); // use backed up name
			}
			continue;
		} else if ( !Q_stricmp( token, "itemName" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(weaponGroup.itemName, sizeof (weaponGroup.itemName), "%s", token);
			continue;
		} else if ( !Q_stricmp( token, "pickupSound" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(weaponGroup.pickupSound, sizeof (weaponGroup.pickupSound), "%s", token);
			continue;
		} else if ( !Q_stricmp( token, "pickupModel" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(weaponGroup.pickupModel, sizeof (weaponGroup.pickupModel), "%s", token);
			continue;
		} else if ( !Q_stricmp( token, "iconName" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(weaponGroup.iconName, sizeof (weaponGroup.iconName), "%s", token);
			continue;
		} else if ( !Q_stricmp( token, "pickupName" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(weaponGroup.pickupName, sizeof (weaponGroup.pickupName), "%s", token);
			continue;
		} else if ( !Q_stricmp( token, "pickupAmmo" ) ) {
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			weaponGroup.item.quantity = atoi(token);
			continue;
		} else if ( !Q_stricmp( token, "weapon_primary" )
			|| !Q_stricmp( token, "weapon_secondary" ) )
		{
			int w;
			int hand = !Q_stricmp( token, "weapon_secondary" );
			token = COM_Parse( p );
			if ( !*token ) {
				break;
			}
			w = BG_WeaponIndexForName(token);
			weaponGroup.weapon[hand] = &bg_weaponinfo[w];
			weaponGroup.weaponnum[hand] = w;
			if (weaponGroup.weapon[hand]->weapontype == WT_NONE)
			{
				// Easier checking for valid weapons.
				weaponGroup.weapon[hand] = &bg_weaponinfo[0];
				weaponGroup.weaponnum[hand] = 0;
			}
			if (!w)
			{
				Com_Printf("Can't find weapon [%s] for [%s]\n", token, weaponGroup.name);
			}
			continue;
		}
		Com_Printf( "unknown token '%s' in weapongroup %s\n", token, weaponGroup.name );
	}
	return qfalse;
}

/*
======================
BG_ParseWeaponInfoFile
======================
*/
qboolean BG_ParseWeaponInfoFile( const char *filename ) {
	char		*text_p;//, *prev;
	int			len;
	//int			i;
	char		*token;
	//int			skip;
	char		text[20000];
	fileHandle_t	f;

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		Com_Printf( "File %s too long\n", filename );
		trap_FS_FCloseFile( f );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;

	// read optional parameters
	while ( 1 ) {
		//prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );

		if ( !*token ) {
			break;
		}

		if ( Q_stricmp( token, "projectile" ) == 0 ) {
			if (!Projectile_Parse(&text_p))
			{
				break;
			}
		} else if ( Q_stricmp( token, "weapon" ) == 0 ) {
			if (!Weapon_Parse(&text_p))
			{
				break;
			}
		}
		else if ( Q_stricmp( token, "weapongroup" ) == 0 ) {
			if (!WeaponGroup_Parse(&text_p))
			{
				break;
			}
		}
		else
		{
			Com_Printf( "unknown token '%s' in %s\n", token, filename );
		}
	}

	return qtrue;
}

void BG_DumpWeaponInfo(void)
{
	int i, j;
	bg_projectileinfo_t *projectile;
	bg_weaponinfo_t *weapon;
	bg_bladeinfo_t *blade;
	bg_weapongroupinfo_t *weaponGroup;
	char text[256] = { 0 };
	fileHandle_t f;

	Com_Printf("Dumping weapon info\n");

	if (trap_FS_FOpenFile("weapondump.txt", &f, FS_WRITE) < 0) {
		Com_Printf("  Failed to open file for write.\n");
		return;
	}

#define FS_Printf1(x) { text[0] = 0; trap_FS_Write(x, strlen(x+1), f); }
#define FS_Printf2(x, y) { text[0] = 0; Q_snprintf(text, sizeof (text), x, y); trap_FS_Write(text, strlen(text+1), f); }
#define FS_Printf4(w, x, y, z) { text[0] = 0; Q_snprintf(text, sizeof (text), w, x, y, z); trap_FS_Write(text, strlen(text+1), f); }

	// Dump projectiles
	for (i = 0; i < MAX_BG_PROJ; i++)
	{
		projectile = &bg_projectileinfo[i];
		if (projectile->name[0] == '\0')
			continue;
		FS_Printf2("projectile \"%s\"\r\n", projectile->name);
		FS_Printf1("{\r\n");
		FS_Printf2("\tmodel \"%s\"\r\n", projectile->model);
		FS_Printf2("\tdamagetype %d\r\n", projectile->damagetype);
		FS_Printf2("\tdamage %d\r\n", projectile->damage);
		FS_Printf2("\tsplashdamage %d\r\n", projectile->splashDamage);
		FS_Printf2("\tsplashRadius %d\r\n", projectile->splashRadius);
		FS_Printf2("\tspeed %d\r\n", projectile->speed);
		FS_Printf2("\ttimetolive %d\r\n", projectile->timetolive);
		FS_Printf2("\tshootable %d\r\n", projectile->shootable);
		FS_Printf2("\ttrailType %d\r\n", projectile->trailType);
		FS_Printf2("\tdeathType %d\r\n", projectile->deathType);
		FS_Printf2("\tmissileDlight %d\r\n", projectile->missileDlight);
		FS_Printf4("\tmissileDlightColor %f %f %f\r\n", projectile->missileDlightColor[0], projectile->missileDlightColor[1], projectile->missileDlightColor[2]);
		FS_Printf2("\tmissileSound \"%s\"\r\n", projectile->missileSoundName);
		FS_Printf2("\tsprite \"%s\"\r\n", projectile->sprite);
		FS_Printf2("\tspriteRadius %d\r\n", projectile->spriteRadius);
		FS_Printf2("\tspinType %d\r\n", projectile->spinType);
		FS_Printf2("\tbounceType %d\r\n", projectile->bounceType);
		FS_Printf2("\tmaxBounces %d\r\n", projectile->maxBounces);
		FS_Printf2("\twallmarkName \"%s\"\r\n", projectile->wallmarkName);
		FS_Printf2("\twallmarkRadius %d\r\n", projectile->wallmarkRadius);

		FS_Printf2("\twallmarkFadeAlpha %d\r\n", (projectile->flags & PF_WALLMARK_FADE_ALPHA));
		FS_Printf2("\twallmarkColorize %d\r\n", (projectile->flags & PF_WALLMARK_COLORIZE));
		FS_Printf2("\texplosionColorize %d\r\n", (projectile->flags & PF_EXPLOSION_COLORIZE));
		FS_Printf2("\tuseGravity %d\r\n", (projectile->flags & PF_USE_GRAVITY));

		FS_Printf2("\tstickOnImpact %d\r\n", projectile->stickOnImpact);
		FS_Printf2("\tfalltoground %d\r\n", projectile->falltoground);
		FS_Printf2("\tspdRndAdd %d\r\n", projectile->spdRndAdd);
		FS_Printf2("\tspread %d\r\n", projectile->spread);
		FS_Printf2("\tnumProjectiles %d\r\n", projectile->numProjectiles);

		FS_Printf2("\thoming %d\r\n", projectile->homing);
		FS_Printf2("\tgrappling %d\r\n", projectile->grappling);
		FS_Printf2("\tinstantDamage %d\r\n", projectile->instantDamage);

		FS_Printf1("}\r\n\r\n");
	}
	projectile = NULL;

	// Dump weapons
	for (i = 0; i < MAX_BG_WEAPONS; i++)
	{
		weapon = &bg_weaponinfo[i];
		if (weapon->name[0] == '\0')
			continue;
		FS_Printf2("weapon \"%s\"\r\n", weapon->name);
		FS_Printf1("{\r\n");
		FS_Printf2("\tmodel \"%s\"\r\n", weapon->model);
		FS_Printf2("\tweapontype %s\r\n", weapontypeNames[weapon->weapontype]);
		FS_Printf2("\tmod %s\r\n", modNames[weapon->mod]);
		FS_Printf2("\tattackDelay %d\r\n", weapon->attackDelay);
		FS_Printf2("\twallmarkName \"%s\"\r\n", weapon->wallmarkName);
		FS_Printf2("\twallmarkRadius %d\r\n", weapon->wallmarkRadius);

		FS_Printf2("\twallmarkFadeAlpha %d\r\n", (weapon->flags & WIF_WALLMARK_FADE_ALPHA));
		FS_Printf2("\twallmarkColorize %d\r\n", (weapon->flags & WIF_WALLMARK_COLORIZE));
		FS_Printf2("\talwaysDamage %d\r\n", (weapon->flags & WIF_ALWAYS_DAMAGE));
		FS_Printf2("\tcontinuousFlash %d\r\n", (weapon->flags & WIF_CONTINUOUS_FLASH));
		FS_Printf2("\tejectBrass %d\r\n", (weapon->flags & WIF_EJECT_BRASS));
		FS_Printf2("\tejectBrass2 %d\r\n", (weapon->flags & WIF_EJECT_BRASS2));
		FS_Printf2("\tejectSmoke %d\r\n", (weapon->flags & WIF_EJECT_SMOKE));

		FS_Printf2("\tsplashMod %s\r\n", modNames[weapon->splashMod]);
		FS_Printf4("\tflashColor %f %f %f\r\n", weapon->flashColor[0], weapon->flashColor[1], weapon->flashColor[2]);
		FS_Printf2("\treadySound \"%s\"\r\n", weapon->readySoundName);
		FS_Printf2("\tfiringSound \"%s\"\r\n", weapon->firingSoundName);
		FS_Printf2("\tflashSound0 \"%s\"\r\n", weapon->flashSoundName[0]);
		FS_Printf2("\tflashSound1 \"%s\"\r\n", weapon->flashSoundName[1]);
		FS_Printf2("\tflashSound2 \"%s\"\r\n", weapon->flashSoundName[2]);
		FS_Printf2("\tflashSound3 \"%s\"\r\n", weapon->flashSoundName[3]);
		FS_Printf2("\tprojectile \"%s\"\r\n", bg_projectileinfo[weapon->proj].name);

		// Dump blades
		for (j = 0; j < MAX_WEAPON_BLADES; j++)
		{
			blade = &weapon->blades[j];
			FS_Printf1("\t{\r\n");
			FS_Printf2("\t\tdamage %d\r\n", blade->damage);
			FS_Printf4("\t\torigin %f %f %f\r\n", blade->origin[0], blade->origin[1], blade->origin[2]);
			FS_Printf4("\t\ttip %f %f %f\r\n", blade->tip[0], blade->tip[1], blade->tip[2]);
			FS_Printf2("\t\ttrailStyle %d\r\n", blade->trailStyle);
			FS_Printf1("\t}\r\n");
		}
		FS_Printf1("}\r\n\r\n");
	}
	weapon = NULL;

	// Dump weapon groups
	for (i = 0; i < MAX_BG_WEAPON_GROUPS; i++)
	{
		weaponGroup = &bg_weapongroupinfo[i];
		if (weaponGroup->name[0] == '\0')
			break;
		FS_Printf2("weapongroup \"%s\"\r\n", weaponGroup->name);
		FS_Printf1("{\r\n");
		FS_Printf2("\titemName \"%s\"\r\n", weaponGroup->itemName);
		FS_Printf2("\tpickupSound \"%s\"\r\n", weaponGroup->pickupSound);
		FS_Printf2("\tpickupModel \"%s\"\r\n", weaponGroup->pickupModel);
		FS_Printf2("\ticonName \"%s\"\r\n", weaponGroup->iconName);
		FS_Printf2("\tpickupName \"%s\"\r\n", weaponGroup->pickupName);
		FS_Printf2("\tpickupAmmo %i\r\n", weaponGroup->item.quantity);

		if (weaponGroup->weapon[0]) {
			FS_Printf2("\tweapon_primary \"%s\"\r\n", weaponGroup->weapon[0]->name);
		}
		if (weaponGroup->weapon[1]) {
			FS_Printf2("\tweapon_secondary \"%s\"\r\n", weaponGroup->weapon[1]->name);
		}

		// Animations
		FS_Printf1("\r\n");
		FS_Printf1("\tnormalAnims\r\n");
		FS_Printf1("\t{\r\n");
		FS_Printf2("\t\tstandAnim \"%s\"\r\n", playerAnimationNames[weaponGroup->normalAnims.standAnim]);
		FS_Printf2("\t\tattackAnim \"%s\"\r\n", playerAnimationNames[weaponGroup->normalAnims.attackAnim]);
		FS_Printf1("\t}\r\n");
		FS_Printf1("\tprimaryAnims\r\n");
		FS_Printf1("\t{\r\n");
		FS_Printf2("\t\tstandAnim \"%s\"\r\n", playerAnimationNames[weaponGroup->primaryAnims.standAnim]);
		FS_Printf2("\t\tattackAnim \"%s\"\r\n", playerAnimationNames[weaponGroup->primaryAnims.attackAnim]);
		FS_Printf1("\t}\r\n");

		FS_Printf1("}\r\n\r\n");
	}
	weaponGroup = NULL;

	trap_FS_FCloseFile(f);
}

/*
=========
BG_InitWeaponInfo
=========
*/
void BG_InitWeaponInfo(void)
{
	int			numdirs;
	char		filename[128];
	char		dirlist[1024];
	char*		dirptr;
	int			i;
	int			dirlen;

	if (bg_weaponsys_init)
		return;

	bg_weaponsys_init = qtrue;

	// Clear Data
	memset(bg_projectileinfo, 0, sizeof (bg_projectileinfo));
	memset(bg_weaponinfo, 0, sizeof (bg_weaponinfo));
	memset(bg_weapongroupinfo, 0, sizeof (bg_weapongroupinfo));

	// Setup dummys (0 means not valid or not found)
	strcpy(bg_projectileinfo[0].name, "p_none");

	strcpy(bg_weaponinfo[0].name, "w_none");
	bg_weaponinfo[0].weapontype = WT_NONE;
	bg_weaponinfo[0].attackDelay = 400;
	bg_weaponinfo[0].flashColor[0] = 1.0f;
	bg_weaponinfo[0].flashColor[1] = 1.0f;
	bg_weaponinfo[0].flashColor[2] = 1.0f;

	// WP_NONE
	strcpy(bg_weapongroupinfo[0].name, "wp_none");
	strcpy(bg_weapongroupinfo[0].pickupName, "None");
	bg_weapongroupinfo[0].normalAnims.standAnim = TORSO_STAND;
	bg_weapongroupinfo[0].normalAnims.attackAnim = TORSO_ATTACK;
	bg_weapongroupinfo[0].primaryAnims.standAnim = TORSO_STAND;
	bg_weapongroupinfo[0].primaryAnims.attackAnim = TORSO_ATTACK;

	// Load data.
	if (!BG_ParseWeaponInfoFile("scripts/weaponinfo.txt"))
	{
		// No weaponinfo.txt file or failed. Default to Quake3 weapons?
	}

	// get all weapons from .weap files
	numdirs = trap_FS_GetFileList("scripts", ".weap", dirlist, 1024 );
	dirptr  = dirlist;
	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		BG_ParseWeaponInfoFile(filename);
	}

	// So I can see if it is loading correctly.
	//BG_DumpWeaponInfo();
}

#else
// Weapons that use both hands to hold the same weapon really only use the primary.
#define HAND_SHARE (HAND_PRIMARY)

bg_weapontypeinfo_t bg_weapontypeinfo[WT_MAX] =
{
	// hands			one handed type				standing animation					attacking animation
	{ HAND_PRIMARY,		WT_NONE,					TORSO_STAND2,						TORSO_ATTACK2 }, // WT_NONE
#ifdef TMNTPLAYERS
	{ HAND_PRIMARY,		WT_GAUNTLET,				TORSO_STAND2,						TORSO_ATTACK2  }, // WT_GAUNTLET
	{ HAND_SHARE,		WT_GUN_PRIMARY,				TORSO_STAND,						TORSO_ATTACK  }, // WT_GUN
	{ HAND_PRIMARY,		WT_GUN_PRIMARY,				TORSO_STAND_GUN_PRIMARY,			TORSO_ATTACK_GUN_PRIMARY }, // WT_GUN_PRIMARY
#ifdef TMNTWEAPONS
	{ HAND_SHARE,		WT_SWORD1_PRIMARY,			TORSO_STAND_SWORD1_BOTH,			TORSO_ATTACK_SWORD1_BOTH }, // WT_SWORD1_BOTH
	{ HAND_PRIMARY,		WT_SWORD1_PRIMARY,			TORSO_STAND_SWORD1_PRIMARY,			TORSO_ATTACK_SWORD1_PRIMARY }, // WT_SWORD1_PRIMARY
	{ HAND_BOTH,		WT_SWORD1_PRIMARY,			TORSO_STAND_SWORD2,					TORSO_ATTACK_SWORD2 }, // WT_SWORD2
	{ HAND_BOTH,		WT_SWORD1_PRIMARY,			TORSO_STAND_DAISHO,					TORSO_ATTACK_DAISHO }, // WT_DAISHO
	{ HAND_BOTH,		WT_SAI1_PRIMARY,			TORSO_STAND_SAI2,					TORSO_ATTACK_SAI2 }, // WT_SAI2
	{ HAND_PRIMARY,		WT_SAI1_PRIMARY,			TORSO_STAND_SHORT_SWORD1_PRIMARY,	TORSO_ATTACK_SHORT_SWORD1_PRIMARY }, // WT_SAI1_PRIMARY
	{ HAND_SHARE,		WT_BO_PRIMARY,				TORSO_STAND_BO,						TORSO_ATTACK_BO }, // WT_BO
	{ HAND_PRIMARY,		WT_BO_PRIMARY,				TORSO_STAND_BO_PRIMARY,				TORSO_ATTACK_BO_PRIMARY }, // WT_BO_PRIMARY
	{ HAND_SHARE,		WT_HAMMER_PRIMARY,			TORSO_STAND_HAMMER,					TORSO_ATTACK_HAMMER }, // WT_HAMMER
	{ HAND_PRIMARY,		WT_HAMMER_PRIMARY,			TORSO_STAND_HAMMER_PRIMARY,			TORSO_ATTACK_HAMMER_PRIMARY }, // WT_HAMMER_PRIMARY
	{ HAND_BOTH,		WT_NUNCHUKS1_PRIMARY,		TORSO_STAND_NUNCHUKS,				TORSO_ATTACK_NUNCHUKS }, // WT_NUNCHUKS
	{ HAND_PRIMARY,		WT_NUNCHUKS1_PRIMARY,		TORSO_STAND_NUNCHUKS1_PRIMARY,		TORSO_ATTACK_NUNCHUKS1_PRIMARY }, // WT_NUNCHUKS1_PRIMARY
#endif
#else
	// hands			one handed type			standing animation					attacking animation
	{ HAND_PRIMARY,		WT_GAUNTLET,				TORSO_STAND2,	TORSO_ATTACK2 }, // WT_GAUNTLET
	{ HAND_SHARE,		WT_GUN_PRIMARY,				TORSO_STAND,	TORSO_ATTACK  }, // WT_GUN
	{ HAND_PRIMARY,		WT_GUN_PRIMARY,				TORSO_STAND2,	TORSO_ATTACK2 }, // WT_GUN_PRIMARY
#ifdef TMNTWEAPONS
	{ HAND_SHARE,		WT_SWORD1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_SWORD1_BOTH
	{ HAND_PRIMARY,		WT_SWORD1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_SWORD1_PRIMARY
	{ HAND_BOTH,		WT_SWORD1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_SWORD2
	{ HAND_BOTH,		WT_SWORD1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_DAISHO
	{ HAND_BOTH,		WT_SAI1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_SAI2
	{ HAND_PRIMARY,		WT_SAI1_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_SAI1_PRIMARY
	{ HAND_SHARE,		WT_BO_PRIMARY,				TORSO_STAND2,	TORSO_ATTACK2 }, // WT_BO
	{ HAND_PRIMARY,		WT_BO_PRIMARY,				TORSO_STAND2,	TORSO_ATTACK2 }, // WT_BO_PRIMARY
	{ HAND_SHARE,		WT_HAMMER_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_HAMMER
	{ HAND_PRIMARY,		WT_HAMMER_PRIMARY,			TORSO_STAND2,	TORSO_ATTACK2 }, // WT_HAMMER_PRIMARY
	{ HAND_BOTH,		WT_NUNCHUKS1_PRIMARY,		TORSO_STAND2,	TORSO_ATTACK2 }, // WT_NUNCHUKS
	{ HAND_PRIMARY,		WT_NUNCHUKS1_PRIMARY,		TORSO_STAND2,	TORSO_ATTACK2 }, // WT_NUNCHUKS1_PRIMARY
#endif
#endif
};
#undef HAND_SHARE

bg_weaponinfo_t bg_weaponinfo[WP_NUM_WEAPONS] =
{
	// weapontype				mod			mod2	damage damage2 (/mins maxs/) start end  start2 end2
#ifdef TMNTWEAPONS
	{ WT_NONE,					MOD_UNKNOWN, MOD_UNKNOWN, 0,0, /* {0,0,0}, {0,0,0},*/ 0, 0, /* */ 0, 0 }, // WP_NONE

	{ WT_SAI2,					MOD_FIST, MOD_FIST, 	10, 10, -5, 5, -5, 5 }, // WP_FISTS

	{ WT_SWORD2,				MOD_KATANA, MOD_KATANA, 10, 10, 0, 40, 0, 40 }, // WP_KATANAS
	{ WT_DAISHO,				MOD_KATANA, MOD_WAKIZASHI, 10, 8, 0, 40, 0, 20 }, // WP_DAISHO

	{ WT_SAI2, 					MOD_SAI,	MOD_SAI, 	10, 10, 0, 16, 0, 16 }, // WP_SAIS

	{ WT_NUNCHUKS, 				MOD_NUNCHUK,MOD_NUNCHUK, 10, 10, 0, 16, 0, 16 }, // WP_NUNCHUKS

	{ WT_HAMMER,				MOD_HAMMER, MOD_UNKNOWN, 20, 0, 0/*12*/, 20, 0, 0 }, // WP_HAMMER
	{ WT_HAMMER, 				MOD_AXE,	MOD_UNKNOWN, 20, 0, 0/*12*/, 20, 0, 0 }, // WP_AXE

	{ WT_SWORD1_BOTH, 			MOD_SWORD,	MOD_UNKNOWN, 10, 0, 0, 32, 0, 0 }, // WP_LONGSWORD
	{ WT_SWORD1_BOTH, 			MOD_BAT,	MOD_UNKNOWN, 10, 0, 0, 21, 0, 0 }, // WP_BAT

	{ WT_BO, 					MOD_BO,		MOD_UNKNOWN, 10, 0, -48, 48, 0, 0 }, // WP_BO
	{ WT_BO, 					MOD_BAMBOOBO,MOD_UNKNOWN,10, 0, -48, 48, 0, 0 }, // WP_BAMBOOBO

	{ WT_GUN,					MOD_GUN,	MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_GUN
	{ WT_GUN,					MOD_ELECTRIC,MOD_UNKNOWN,0, 0, 0, 0, 0, 0 }, // WP_ELECTRIC_LAUNCHER
	{ WT_GUN,					MOD_ROCKET, MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_ROCKET_LAUNCHER
	{ WT_GUN,					MOD_HOMING, MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_HOMING_LAUNCHER
	{ WT_GUN, 					MOD_GRAPPLE,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_GRAPPLING_HOOK
#else
	{ WT_NONE,MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_NONE
	{ WT_GAUNTLET, MOD_GAUNTLET, MOD_UNKNOWN, 50, 0, 0, 32, 0, 0 }, // WP_GAUNTLET
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_MACHINEGUN
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_SHOTGUN
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_GRENADE_LAUNCHER
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_ROCKET_LAUNCHER
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_LIGHTNING
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_RAILGUN
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_PLASMAGUN
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_BFG
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_GRAPPLING_HOOK
#ifdef MISSIONPACK
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_NAILGUN
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_PROX_LAUNCHER
	{ WT_GUN, MOD_UNKNOWN,MOD_UNKNOWN, 0, 0, 0, 0, 0, 0 }, // WP_CHAINGUN
#endif
#endif
};
#endif

// The below two functions should make adding more weapon types easier.

/*
==============
BG_TorsoStandForPlayerState
==============
*/
animNumber_t BG_TorsoStandForPlayerState(playerState_t *ps)
{
#ifndef TMNTWEAPSYS_2
	weapontype_t wt;
#endif

	if (ps == NULL || ps->weapon < 0 || ps->weapon >= WP_NUM_WEAPONS)
	{
		return TORSO_STAND;
	}

#ifdef TMNTWEAPSYS_2
	if (ps->eFlags & EF_PRIMARY_HAND)
	{
		return (animNumber_t)bg_weapongroupinfo[ps->weapon].primaryAnims.standAnim;
	}
	return (animNumber_t)bg_weapongroupinfo[ps->weapon].normalAnims.standAnim;
#else
	wt = bg_weaponinfo[ps->weapon].weapontype;

	if (wt != bg_weapontypeinfo[wt].primaryOnly)
	{
		if (ps->eFlags & EF_PRIMARY_HAND)
		{
			wt = bg_weapontypeinfo[wt].primaryOnly;
		}
	}

	return (animNumber_t)bg_weapontypeinfo[wt].standAnim;
#endif
}

/*
==============
BG_TorsoAttackForPlayerState
==============
*/
animNumber_t BG_TorsoAttackForPlayerState(playerState_t *ps)
{
#ifndef TMNTWEAPSYS_2
	weapontype_t wt;
#endif

	if (ps == NULL || ps->weapon < 0 || ps->weapon >= WP_NUM_WEAPONS)
	{
		return TORSO_ATTACK;
	}

#ifdef TMNTWEAPSYS_2
	if (ps->eFlags & EF_PRIMARY_HAND)
	{
		return (animNumber_t)bg_weapongroupinfo[ps->weapon].primaryAnims.attackAnim;
	}
	return (animNumber_t)bg_weapongroupinfo[ps->weapon].normalAnims.attackAnim;
#else
	wt = bg_weaponinfo[ps->weapon].weapontype;

	if (wt != bg_weapontypeinfo[wt].primaryOnly)
	{
		if (ps->eFlags & EF_PRIMARY_HAND)
		{
			wt = bg_weapontypeinfo[wt].primaryOnly;
		}
	}

	return (animNumber_t)bg_weapontypeinfo[wt].attackAnim;
#endif
}

/*
==============
BG_TorsoStandForWeapon
==============
*/
animNumber_t BG_TorsoStandForWeapon(weapon_t weaponnum)
{
#ifdef TMNTWEAPSYS_2
	return (animNumber_t)bg_weapongroupinfo[weaponnum].normalAnims.standAnim;
#else
	return (animNumber_t)bg_weapontypeinfo[BG_WeaponTypeForNum(weaponnum)].standAnim;
#endif
}

/*
==============
BG_TorsoAttackForWeapon
==============
*/
animNumber_t BG_TorsoAttackForWeapon(weapon_t weaponnum)
{
#ifdef TMNTWEAPSYS_2
	return (animNumber_t)bg_weapongroupinfo[weaponnum].normalAnims.attackAnim;
#else
	return (animNumber_t)bg_weapontypeinfo[BG_WeaponTypeForNum(weaponnum)].attackAnim;
#endif
}

/*
==============
BG_WeaponTypeForPlayerState
==============
*/
weapontype_t BG_WeaponTypeForPlayerState(playerState_t *ps)
{
	weapontype_t wt;

	if (ps == NULL || ps->weapon < 0 || ps->weapon >= WP_NUM_WEAPONS)
	{
		return WT_NONE;
	}

#ifdef TMNTWEAPSYS_2
	if (bg_weapongroupinfo[ps->weapon].weapon[0])
	{
		wt = bg_weapongroupinfo[ps->weapon].weapon[0]->weapontype;
		// What of bg_weapongroupinfo[ps->weapon].weapon[1] ?
	}
	else
	{
		wt = WT_NONE;
	}
#else
	wt = bg_weaponinfo[ps->weapon].weapontype;

	if (wt != bg_weapontypeinfo[wt].primaryOnly)
	{
		if (ps->eFlags & EF_PRIMARY_HAND)
		{
			// Not using secondary hand, holding flag or something.
			wt = bg_weapontypeinfo[wt].primaryOnly;
		}
	}
#endif

	return wt;
}

/*
==============
BG_WeaponTypeForEntityState
==============
*/
weapontype_t BG_WeaponTypeForEntityState(entityState_t *es)
{
	weapontype_t wt;

	if (es == NULL || es->weapon < 0 || es->weapon >= WP_NUM_WEAPONS)
	{
		return WT_NONE;
	}

#ifdef TMNTWEAPSYS_2
	if (bg_weapongroupinfo[es->weapon].weapon[0])
	{
		wt = bg_weapongroupinfo[es->weapon].weapon[0]->weapontype;
		// What of bg_weapongroupinfo[es->weapon].weapon[1] ?
	}
	else
	{
		wt = WT_NONE;
	}
#else
	wt = bg_weaponinfo[es->weapon].weapontype;

	if (wt != bg_weapontypeinfo[wt].primaryOnly)
	{
		if (es->eFlags & EF_PRIMARY_HAND)
		{
			// Not using secondary hand, holding flag or something.
			wt = bg_weapontypeinfo[wt].primaryOnly;
		}
	}
#endif

	return wt;
}

/*
==============
BG_WeaponTypeForNum
==============
*/
weapontype_t BG_WeaponTypeForNum(weapon_t weaponnum)
{
	//qboolean primaryOnly = qfalse; // Player has the flag so only use one hand.
	weapontype_t wt;

	if (weaponnum < 0 || weaponnum >= WP_NUM_WEAPONS)
	{
		return WT_NONE;
	}

#ifdef TMNTWEAPSYS_2
	if (bg_weapongroupinfo[weaponnum].weapon[0])
		wt = bg_weapongroupinfo[weaponnum].weapon[0]->weapontype;
	else
		wt = WT_NONE;
#else
	wt = bg_weaponinfo[weaponnum].weapontype;
#endif

	/*if (primaryOnly == qtrue)
	{
		wt = bg_weapontypeinfo[wt].primaryOnly;
	}*/

	return wt;
}

/*
==============
BG_WeaponHandsForPlayerState
==============
*/
int BG_WeaponHandsForPlayerState(playerState_t *ps)
{
#ifdef TMNTWEAPSYS_2
	if (ps->eFlags & EF_PRIMARY_HAND)
	{
		return HAND_PRIMARY;
	}

	if (bg_weapongroupinfo[ps->weapon].weapon[0]
		&& bg_weapongroupinfo[ps->weapon].weapon[1])
	{
		return HAND_BOTH;
	}
	else //if (bg_weapongroupinfo[ps->weapon].weapon[0])
	{
		return HAND_PRIMARY;
	}
#else
	return bg_weapontypeinfo[BG_WeaponTypeForPlayerState(ps)].hands;
#endif
}

/*
==============
BG_WeaponHandsForWeaponNum
==============
*/
int BG_WeaponHandsForWeaponNum(weapon_t weaponnum)
{
#ifdef TMNTWEAPSYS_2
	int rtn = 0;

	if (bg_weapongroupinfo[weaponnum].weapon[0])
		rtn |= HAND_PRIMARY;
	if (bg_weapongroupinfo[weaponnum].weapon[1])
		rtn |= HAND_SECONDARY;

	return rtn;
#else
	return bg_weapontypeinfo[BG_WeaponTypeForNum(weaponnum)].hands;
#endif
}


qboolean BG_WeapTypeIsMelee(weapontype_t wt)
{
#ifdef TMNTWEAPSYS_2
	return (wt == WT_MELEE || wt == WT_GAUNTLET);
#else
	return (wt != WT_NONE && wt != WT_GUN && wt != WT_GUN_PRIMARY);
#endif
}

qboolean BG_WeapUseAmmo(weapon_t w)
{
	weapontype_t wt = BG_WeaponTypeForNum(w);
#ifdef TMNTWEAPSYS_2
	// Guns use ammo, but grappling hook gun does not.
	return (wt == WT_GUN && !bg_projectileinfo[bg_weaponinfo[w].proj].grappling);
#else
	return (w != WP_GRAPPLING_HOOK && (wt == WT_GUN || wt == WT_GUN_PRIMARY));
#endif
}

qboolean BG_PlayerAttackAnim(int a)
{
#if 0
	int i;
	for (i = 0; i < WT_MAX; i++)
	{
		if (bg_weapontypeinfo[i].attackAnim == a)
		{
			return qtrue;
		}
	}
	return qfalse;
#else
	return (a == TORSO_ATTACK || a == TORSO_ATTACK2
#ifdef TMNTPLAYERS
	|| (a >= TORSO_ATTACK_GUN_PRIMARY && a <= TORSO_ATTACK_NUNCHUKS1_PRIMARY)
#endif
	);
#endif
}

qboolean BG_PlayerStandAnim(int a)
{
#if 0
	int i;
	for (i = 0; i < WT_MAX; i++)
	{
		if (bg_weapontypeinfo[i].standAnim == a)
		{
			return qtrue;
		}
	}
	return qfalse;
#else
	return (a == TORSO_STAND || a == TORSO_STAND2
#ifdef TMNTPLAYERS
	|| (a >= TORSO_STAND_GUN_PRIMARY && a <= TORSO_STAND_NUNCHUKS1_PRIMARY)
#endif
	);
#endif
}
#endif

#ifdef TMNTHOLDSYS
/*
==============
BG_ItemNumForHoldableNum

Returns the of the holdable item index in bg_itemlist
Returns 0 if not found.
==============
*/
int BG_ItemNumForHoldableNum(holdable_t holdablenum)
{
#ifdef TMNTWEAPSYS_2
	gitem_t	*it;
	int i;

	for (i = NUM_BG_ITEMS-1; i > 0; i--)
	{
		it = BG_ItemForItemNum(i);
		if (!it->classname)
			continue;
		if ( it->giType == IT_HOLDABLE
			&& it->giTag == holdablenum )
		{
			return i;
		}
	}
#else
	int i;
	for (i = 1; i < bg_numItems; ++i)
	{
		if (bg_itemlist[i].giType == IT_HOLDABLE
			&& bg_itemlist[i].giTag == holdablenum)
		{
			return i;
		}
	}
#endif
	return 0;
}
#endif

/*
==============
BG_FindItemForPowerup
==============
*/
gitem_t	*BG_FindItemForPowerup( powerup_t pw ) {
#ifdef TMNTWEAPSYS_2
	gitem_t	*it;
	int i;

	for (i = NUM_BG_ITEMS-1; i > 0; i--)
	{
		it = BG_ItemForItemNum(i);
		if (!it->classname)
			continue;
		if ( (it->giType == IT_POWERUP
				|| it->giType == IT_TEAM
				|| it->giType == IT_PERSISTANT_POWERUP)
			&& it->giTag == pw )
		{
			return it;
		}
	}
#else
	int		i;

	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( (bg_itemlist[i].giType == IT_POWERUP || 
					bg_itemlist[i].giType == IT_TEAM ||
					bg_itemlist[i].giType == IT_PERSISTANT_POWERUP) && 
			bg_itemlist[i].giTag == pw ) {
			return &bg_itemlist[i];
		}
	}
#endif

	return NULL;
}


/*
==============
BG_FindItemForHoldable
==============
*/
gitem_t	*BG_FindItemForHoldable( holdable_t pw ) {
#ifdef TMNTWEAPSYS_2
	gitem_t	*it;
	int i;

	for (i = NUM_BG_ITEMS-1; i > 0; i--)
	{
		it = BG_ItemForItemNum(i);
		if (!it->classname)
			continue;
		if ( it->giType == IT_HOLDABLE && it->giTag == pw ) {
			return it;
		}
	}
#else
	int		i;

	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( bg_itemlist[i].giType == IT_HOLDABLE && bg_itemlist[i].giTag == pw ) {
			return &bg_itemlist[i];
		}
	}
#endif

	Com_Error( ERR_DROP, "HoldableItem not found" );

	return NULL;
}


/*
===============
BG_FindItemForWeapon

===============
*/
gitem_t	*BG_FindItemForWeapon( weapon_t weapon ) {
#ifdef TMNTWEAPSYS_2
	gitem_t	*it;
	int i;

	for (i = NUM_BG_ITEMS-1; i > 0; i--)
	{
		it = BG_ItemForItemNum(i);
		if (!it->classname)
			continue;
#else
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++) {
#endif
		if ( it->giType == IT_WEAPON && it->giTag == weapon ) {
			return it;
		}
	}

	Com_Error( ERR_DROP, "Couldn't find item for weapon %i", weapon);
	return NULL;
}

/*
===============
BG_FindItem

===============
*/
gitem_t	*BG_FindItem( const char *pickupName ) {
#ifdef TMNTWEAPSYS_2
	gitem_t	*it;
	int i;

	for (i = NUM_BG_ITEMS-1; i > 0; i--)
	{
		it = BG_ItemForItemNum(i);
		if (!it->classname)
			continue;
#else
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
#endif
		if ( !Q_stricmp( it->pickup_name, pickupName ) )
			return it;
	}

	return NULL;
}

#ifdef TMNTWEAPSYS_2
int BG_ItemNumForItem( gitem_t *item )
{
	if (!item)
	{
		//Com_Printf("Returning type:NULL itemNum:0\n");
		return 0;
	}

	if (item->giType == IT_WEAPON)
	{
		int weaponNum;

		// Turtle Man: TODO: Have default weapon loaded in weaponinfo.txt?
		//if (item->giTag == -1)
		//	weaponNum = 0;
		//else
			weaponNum = item->giTag;

		// Turtle Man: Check weapon item address
		if (weaponNum >= 0 && weaponNum < BG_NumWeaponGroups()
			&& (gitem_t*)&bg_weapongroupinfo[weaponNum].item == item)
		{
			//Com_Printf("Returning type:weapon itemNum:%d\n", bg_numItems+weaponNum);
			return bg_numItems+weaponNum;
		}
	}

	// If address is in bg_itemlist
	if ((int)(item - bg_itemlist) < (int)bg_numItems
		&& (int)(item - bg_itemlist) >= 0)
	{
		//Com_Printf("Returning type:gitem itemNum:%d\n", (int)(item - bg_itemlist));
		return (int)(item - bg_itemlist);
	}

	// Failed
	//Com_Printf("Returning type:unknown itemNum:0\n");
	return 0;
}

gitem_t *BG_ItemForItemNum( int itemnum )
{
	if (itemnum >= 0 && itemnum < bg_numItems)
		return &bg_itemlist[itemnum];

	// It isn't in list must be weapon item,
	if (itemnum >= 0 && itemnum < bg_numItems+MAX_BG_WEAPON_GROUPS)
		return &bg_weapongroupinfo[itemnum-bg_numItems].item;

	return &bg_itemlist[0]; // Can't return NULL.
}
#endif

/*
============
BG_PlayerTouchesItem

Items can be picked up without actually touching their physical bounds to make
grabbing them easier
============
*/
qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime ) {
	vec3_t		origin;

	BG_EvaluateTrajectory( &item->pos, atTime, origin );

	// we are ignoring ducked differences here
	if ( ps->origin[0] - origin[0] > 44
		|| ps->origin[0] - origin[0] < -50
		|| ps->origin[1] - origin[1] > 36
		|| ps->origin[1] - origin[1] < -36
		|| ps->origin[2] - origin[2] > 36
		|| ps->origin[2] - origin[2] < -36 ) {
		return qfalse;
	}

	return qtrue;
}



/*
================
BG_CanItemBeGrabbed

Returns false if the item should not be picked up.
This needs to be the same for client side prediction and server use.
================
*/
qboolean BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps ) {
	gitem_t	*item;
#if (defined MISSIONPACK && !defined TMNTMISC) || defined TMNTHOLDSYS
	int		upperBound;
#endif

#ifdef TMNTWEAPSYS_2
	if ( ent->modelindex < 1 || ent->modelindex >= NUM_BG_ITEMS )
#else
	if ( ent->modelindex < 1 || ent->modelindex >= bg_numItems )
#endif
	{
		Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: index out of range" );
	}

#ifdef TMNTWEAPSYS_2
	item = BG_ItemForItemNum(ent->modelindex);
#else
	item = &bg_itemlist[ent->modelindex];
#endif

	switch( item->giType ) {
	case IT_WEAPON:
#ifdef TMNTWEAPSYS2 // DROP_WEAPON_FIX
		// If using/dropping/picking up a weapon
		// or it was dropped by this player and is still in there Bounding Box
		if (ps->weaponTime || ps->meleeTime ||
			(ent->modelindex2 == 1 && ent->generic1-1 == ps->clientNum)) {
			//Com_Printf("DEBUG: Player touched weapon they can't pickup!\n");
			return qfalse;
		}
#endif
#ifdef TMNTWEAPSYS
		// It the weapon is the same one as the current weapon and its not a gun,
		//  don't pick it up because theres no reason too.
		if (item->giTag == WP_DEFAULT)
		{
			if (ps->weapon == ps->stats[STAT_DEFAULTWEAPON]
				&& !BG_WeapUseAmmo(ps->stats[STAT_DEFAULTWEAPON]))
			{
				return qfalse;
			}
		}
		else if (ps->weapon == item->giTag && !BG_WeapUseAmmo(item->giTag))
		{
			return qfalse;
		}
#endif
		return qtrue;	// weapons are always picked up

	case IT_AMMO:
#ifdef TMNTWEAPSYS2
		{
			int stat = STAT_AMMO;
			if (item->giTag != ps->weapon)
			{
				if (item->giTag == ps->stats[STAT_NEWWEAPON])
					stat = STAT_NEWAMMO;
				else if (item->giTag == ps->stats[STAT_OLDWEAPON])
					stat = STAT_OLDAMMO;
				else if (item->giTag == ps->stats[STAT_DEFAULTWEAPON])
					stat = STAT_SAVEDAMMO;
				else
					return qfalse; // no where to put ammo.
			}
			if ( ps->stats[ stat ] >= 200 ) {
				return qfalse;		// can't hold any more
			}
		}
#else
		if ( ps->ammo[ item->giTag ] >= 200 ) {
			return qfalse;		// can't hold any more
		}
#endif
		return qtrue;

#ifndef TMNT // NOARMOR
	case IT_ARMOR:
#ifdef MISSIONPACK
#ifdef TMNTWEAPSYS_2
		if( BG_ItemForItemNum(ps->stats[STAT_PERSISTANT_POWERUP])->giTag == PW_SCOUT )
#else
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT )
#endif
		{
			return qfalse;
		}

		// we also clamp armor to the maxhealth for handicapping
#ifdef TMNTWEAPSYS_2
		if( BG_ItemForItemNum(ps->stats[STAT_PERSISTANT_POWERUP])->giTag == PW_GUARD )
#else
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD )
#endif
		{
			upperBound = ps->stats[STAT_MAX_HEALTH];
		}
		else {
			upperBound = ps->stats[STAT_MAX_HEALTH] * 2;
		}

		if ( ps->stats[STAT_ARMOR] >= upperBound ) {
			return qfalse;
		}
#else
		if ( ps->stats[STAT_ARMOR] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
			return qfalse;
		}
#endif
		return qtrue;
#endif

#ifdef TMNT // CRATE
	case IT_CRATE:
		return qfalse;
#endif

	case IT_HEALTH:
		// small and mega healths will go over the max, otherwise
		// don't pick up if already at max
#ifndef TMNTMISC // MAX_HEALTH should be the MAX...
#ifdef MISSIONPACK
#ifdef TMNTWEAPSYS_2
		if( BG_ItemForItemNum(ps->stats[STAT_PERSISTANT_POWERUP])->giTag == PW_GUARD )
#else
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD )
#endif
		{
			upperBound = ps->stats[STAT_MAX_HEALTH];
		}
		else
#endif
		if ( item->quantity == 5 || item->quantity == 100 ) {
			if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
				return qfalse;
			}
			return qtrue;
		}
#endif

		if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] ) {
			return qfalse;
		}
		return qtrue;

	case IT_POWERUP:
		return qtrue;	// powerups are always picked up

#ifdef MISSIONPACK
	case IT_PERSISTANT_POWERUP:
		// can only hold one item at a time
		if ( ps->stats[STAT_PERSISTANT_POWERUP] ) {
			return qfalse;
		}

		// check team only
		if( ( ent->generic1 & 2 ) && ( ps->persistant[PERS_TEAM] != TEAM_RED ) ) {
			return qfalse;
		}
		if( ( ent->generic1 & 4 ) && ( ps->persistant[PERS_TEAM] != TEAM_BLUE ) ) {
			return qfalse;
		}

		return qtrue;
#endif

	case IT_TEAM: // team items, such as flags
#ifdef MISSIONPACK		
		if( gametype == GT_1FCTF ) {
			// neutral flag can always be picked up
			if( item->giTag == PW_NEUTRALFLAG ) {
				return qtrue;
			}
			if (ps->persistant[PERS_TEAM] == TEAM_RED) {
				if (item->giTag == PW_BLUEFLAG  && ps->powerups[PW_NEUTRALFLAG] ) {
					return qtrue;
				}
			} else if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if (item->giTag == PW_REDFLAG  && ps->powerups[PW_NEUTRALFLAG] ) {
					return qtrue;
				}
			}
		}
#endif
		if( gametype == GT_CTF ) {
			// ent->modelindex2 is non-zero on items if they are dropped
			// we need to know this because we can pick up our dropped flag (and return it)
			// but we can't pick up our flag at base
			if (ps->persistant[PERS_TEAM] == TEAM_RED) {
				if (item->giTag == PW_BLUEFLAG ||
					(item->giTag == PW_REDFLAG && ent->modelindex2) ||
					(item->giTag == PW_REDFLAG && ps->powerups[PW_BLUEFLAG]) )
					return qtrue;
			} else if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if (item->giTag == PW_REDFLAG ||
					(item->giTag == PW_BLUEFLAG && ent->modelindex2) ||
					(item->giTag == PW_BLUEFLAG && ps->powerups[PW_REDFLAG]) )
					return qtrue;
			}
		}

#ifdef MISSIONPACK_HARVESTER
		if( gametype == GT_HARVESTER ) {
			return qtrue;
		}
#endif
		return qfalse;

	case IT_HOLDABLE:
#ifdef TMNTHOLDSYS
		// Single use holdable item
		if (item->quantity == 0)
		{
			upperBound = 1;
		}
		else
		{
			// Multiple use holdable item
			upperBound = MAX_SHURIKENS;
		}

		// Check use limit.
		if (ps->holdable[item->giTag] >= upperBound)
		{
				return qfalse;
		}
#else
		// can only hold one item at a time
		if ( ps->stats[STAT_HOLDABLE_ITEM] ) {
			return qfalse;
		}
#endif
		return qtrue;

        case IT_BAD:
            Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: IT_BAD" );
        default:
#ifndef Q3_VM
#ifndef NDEBUG
          Com_Printf("BG_CanItemBeGrabbed: unknown enum %d\n", item->giType );
#endif
#endif
         break;
	}

	return qfalse;
}

//======================================================================

/*
================
BG_EvaluateTrajectory

================
*/
void BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result ) {
	float		deltaTime;
	float		phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorCopy( tr->trBase, result );
		break;
	case TR_LINEAR:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = sin( deltaTime * M_PI * 2 );
		VectorMA( tr->trBase, phase, tr->trDelta, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		if ( deltaTime < 0 ) {
			deltaTime = 0;
		}
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectory: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
================
BG_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result ) {
	float	deltaTime;
	float	phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorClear( result );
		break;
	case TR_LINEAR:
		VectorCopy( tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
		phase *= 0.5;
		VectorScale( tr->trDelta, phase, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			VectorClear( result );
			return;
		}
		VectorCopy( tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= DEFAULT_GRAVITY * deltaTime;		// FIXME: local gravity...
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectoryDelta: unknown trType: %i", tr->trTime );
		break;
	}
}

char *eventnames[] = {
	"EV_NONE",

	"EV_FOOTSTEP",
	"EV_FOOTSTEP_METAL",
	"EV_FOOTSPLASH",
	"EV_FOOTWADE",
	"EV_SWIM",

	"EV_STEP_4",
	"EV_STEP_8",
	"EV_STEP_12",
	"EV_STEP_16",

	"EV_FALL_SHORT",
	"EV_FALL_MEDIUM",
	"EV_FALL_FAR",

	"EV_JUMP_PAD",			// boing sound at origin", jump sound on player

	"EV_JUMP",
	"EV_WATER_TOUCH",	// foot touches
	"EV_WATER_LEAVE",	// foot leaves
	"EV_WATER_UNDER",	// head touches
	"EV_WATER_CLEAR",	// head leaves

	"EV_ITEM_PICKUP",			// normal item pickups are predictable
	"EV_GLOBAL_ITEM_PICKUP",	// powerup / team sounds are broadcast to everyone

#ifdef TMNTWEAPSYS2
	"EV_DROP_WEAPON",
#else
	"EV_NOAMMO",
#endif
	"EV_CHANGE_WEAPON",
	"EV_FIRE_WEAPON",

	"EV_USE_ITEM0",
	"EV_USE_ITEM1",
	"EV_USE_ITEM2",
	"EV_USE_ITEM3",
	"EV_USE_ITEM4",
	"EV_USE_ITEM5",
	"EV_USE_ITEM6",
	"EV_USE_ITEM7",
	"EV_USE_ITEM8",
	"EV_USE_ITEM9",
	"EV_USE_ITEM10",
	"EV_USE_ITEM11",
	"EV_USE_ITEM12",
	"EV_USE_ITEM13",
	"EV_USE_ITEM14",
	"EV_USE_ITEM15",

	"EV_ITEM_RESPAWN",
	"EV_ITEM_POP",
	"EV_PLAYER_TELEPORT_IN",
	"EV_PLAYER_TELEPORT_OUT",

#ifdef TMNTHOLDABLE
	"EV_LASERSHURIKEN_BOUNCE",
#else
	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex
#endif

	"EV_GENERAL_SOUND",
	"EV_GLOBAL_SOUND",		// no attenuation
	"EV_GLOBAL_TEAM_SOUND",

#ifndef TMNTWEAPSYS_2
	"EV_BULLET_HIT_FLESH",
	"EV_BULLET_HIT_WALL",
#endif

	"EV_MISSILE_HIT",
	"EV_MISSILE_MISS",
	"EV_MISSILE_MISS_METAL",
	"EV_RAILTRAIL",
#if !defined TMNTWEAPONS && !defined TMNTWEAPSYS_2
	"EV_SHOTGUN",
#endif
	"EV_BULLET",				// otherEntity is the shooter

#ifdef TMNTWEAPSYS
	"EV_WEAPON_HIT",
	"EV_WEAPON_MISS",
	"EV_WEAPON_MISS_METAL",
#endif

	"EV_PAIN",
	"EV_DEATH1",
	"EV_DEATH2",
	"EV_DEATH3",
	"EV_OBITUARY",

	"EV_POWERUP_QUAD",
	"EV_POWERUP_BATTLESUIT",
	"EV_POWERUP_REGEN",

#ifndef NOTRATEDM // No gibs.
	"EV_GIB_PLAYER",			// gib a previously living player
#endif
	"EV_SCOREPLUM",			// score plum

//#ifdef MISSIONPACK
#ifndef TMNTWEAPONS
	"EV_PROXIMITY_MINE_STICK",
	"EV_PROXIMITY_MINE_TRIGGER",
#endif
#ifndef TMNTHOLDABLE // NO_KAMIKAZE_ITEM
	"EV_KAMIKAZE",			// kamikaze explodes
#endif
	"EV_OBELISKEXPLODE",		// obelisk explodes
#ifdef IOQ3ZTM // IOQ3BUGFIX: EV_OBELISKPAIN was missing here!
	"EV_OBELISKPAIN",		// obelisk pain
#endif
#ifndef TMNT // POWERS
	"EV_INVUL_IMPACT",		// invulnerability sphere impact
	"EV_JUICED",				// invulnerability juiced effect
	"EV_LIGHTNINGBOLT",		// lightning bolt bounced of invulnerability sphere
#endif
//#endif

	"EV_DEBUG_LINE",
#ifdef TMNTMISC // DEBUG_ORIGIN
	"EV_DEBUG_ORIGIN",
#endif
	"EV_STOPLOOPINGSOUND",
	"EV_TAUNT"

};

/*
===============
BG_AddPredictableEventToPlayerstate

Handles the sequence numbers
===============
*/

void	trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );

void BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps ) {

#ifdef _DEBUG
	{
		char buf[256];
		trap_Cvar_VariableStringBuffer("showevents", buf, sizeof(buf));
		if ( atof(buf) != 0 ) {
#ifdef QAGAME
			Com_Printf(" game event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#else
			Com_Printf("Cgame event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#endif
		}
	}
#endif
	ps->events[ps->eventSequence & (MAX_PS_EVENTS-1)] = newEvent;
	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS-1)] = eventParm;
	ps->eventSequence++;
}

/*
========================
BG_TouchJumpPad
========================
*/
void BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad ) {
	vec3_t	angles;
	float p;
	int effectNum;

	// spectators don't use jump pads
	if ( ps->pm_type != PM_NORMAL ) {
		return;
	}

	// flying characters don't hit bounce pads
	if ( ps->powerups[PW_FLIGHT] ) {
		return;
	}

	// if we didn't hit this same jumppad the previous frame
	// then don't play the event sound again if we are in a fat trigger
	if ( ps->jumppad_ent != jumppad->number ) {

		vectoangles( jumppad->origin2, angles);
		p = fabs( AngleNormalize180( angles[PITCH] ) );
		if( p < 45 ) {
			effectNum = 0;
		} else {
			effectNum = 1;
		}
		BG_AddPredictableEventToPlayerstate( EV_JUMP_PAD, effectNum, ps );
	}
	// remember hitting this jumppad this frame
	ps->jumppad_ent = jumppad->number;
	ps->jumppad_frame = ps->pmove_framecount;
	// give the player the velocity from the jumppad
	VectorCopy( jumppad->origin2, ps->velocity );
}

/*
========================
BG_PlayerStateToEntityState

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap ) {
	int		i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
#ifndef NOTRATEDM // No gibs.
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
#endif
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

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
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			s->powerups |= 1 << i;
		}
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
#ifdef TMNTWEAPSYS
	// cgame needs the weaponHands for all clients.
	s->weaponHands = ps->weaponHands;
#endif
}

/*
========================
BG_PlayerStateToEntityStateExtraPolate

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap ) {
	int		i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
#ifndef NOTRATEDM // No gibs.
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
#endif
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_LINEAR_STOP;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction and linear prediction
	VectorCopy( ps->velocity, s->pos.trDelta );
	// set the time for linear prediction
	s->pos.trTime = time;
	// set maximum extra polation time
	s->pos.trDuration = 50; // 1000 / sv_fps (default = 20)

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			s->powerups |= 1 << i;
		}
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
#ifdef TMNTWEAPSYS
	// cgame needs the weaponHands for all clients.
	s->weaponHands = ps->weaponHands;
#endif
}

#ifdef IOQ3ZTM // LERP_FRAME_CLIENT_LESS
// Turtle Man: TODO: Remove cg_debugAnim and cg_animSpeed ?
/*
===============
BG_SetLerpFrameAnimation

Based on ioquake3's CG_SetLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
void BG_SetLerpFrameAnimation( lerpFrame_t *lf, animation_t *animations, int newAnimation ) {
	animation_t	*anim;

	lf->animationNumber = newAnimation;
	newAnimation &= ~ANIM_TOGGLEBIT;

	if ( newAnimation < 0 ) {
		Com_Error( ERR_DROP, "Bad animation number: %i", newAnimation );
	}

	// Turtle Man: TDC's NPC version used "anim = animations+newAnimation;"
	//               shouldn't make a difference.
	anim = &animations[ newAnimation ];

	lf->animation = anim;
	lf->animationTime = lf->frameTime + anim->initialLerp;

	//if ( cg_debugAnim.integer ) {
	//	CG_Printf( "Anim: %i\n", newAnimation );
	//}
}

/*
===============
BG_RunLerpFrame


Based on CG_RunLerpFrame

Sets cg.snap, cg.oldFrame, and cg.backlerp
cg.time should be between oldFrameTime and frameTime after exit
===============
*/
void BG_RunLerpFrame( lerpFrame_t *lf, animation_t *animations, int newAnimation, int time, float speedScale ) {
	int			f, numFrames;
	animation_t	*anim;

	// debugging tool to get no animations
	//if ( cg_animSpeed.integer == 0 ) {
	//	lf->oldFrame = lf->frame = lf->backlerp = 0;
	//	return;
	//}

	// see if the animation sequence is switching
	if ( newAnimation != lf->animationNumber || !lf->animation ) {
		BG_SetLerpFrameAnimation( lf, animations, newAnimation );
	}

	// if we have passed the current frame, move it to
	// oldFrame and calculate a new frame
	if ( time >= lf->frameTime ) {
		lf->oldFrame = lf->frame;
		lf->oldFrameTime = lf->frameTime;

		// get the next frame based on the animation
		anim = lf->animation;
		if ( !anim->frameLerp ) {
			return;		// shouldn't happen
		}
		if ( time < lf->animationTime ) {
			lf->frameTime = lf->animationTime;		// initial lerp
		} else {
			lf->frameTime = lf->oldFrameTime + anim->frameLerp;
		}
		f = ( lf->frameTime - lf->animationTime ) / anim->frameLerp;
		f *= speedScale;		// adjust for haste, etc

		numFrames = anim->numFrames;
		if (anim->flipflop) {
			numFrames *= 2;
		}
		if ( f >= numFrames ) {
			f -= numFrames;
			if ( anim->loopFrames ) {
				f %= anim->loopFrames;
				f += anim->numFrames - anim->loopFrames;
			} else {
				f = numFrames - 1;
				// the animation is stuck at the end, so it
				// can immediately transition to another sequence
				lf->frameTime = time;
			}
		}
		if ( anim->reversed ) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - f;
		}
		else if (anim->flipflop && f >= anim->numFrames) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - (f%anim->numFrames);
		}
		else {
			lf->frame = anim->firstFrame + f;
		}
		if ( time > lf->frameTime ) {
			lf->frameTime = time;
			//if ( cg_debugAnim.integer ) {
			//	CG_Printf( "Clamp lf->frameTime\n");
			//}
		}
	}

	if ( lf->frameTime > time + 200 ) {
		lf->frameTime = time;
	}

	if ( lf->oldFrameTime > time ) {
		lf->oldFrameTime = time;
	}
	// calculate current lerp value
	if ( lf->frameTime == lf->oldFrameTime ) {
		lf->backlerp = 0;
	} else {
		lf->backlerp = 1.0 - (float)( time - lf->oldFrameTime ) / ( lf->frameTime - lf->oldFrameTime );
	}
}


/*
===============
BG_ClearLerpFrame
Based on CG_ClearLerpFrame
===============
*/
void BG_ClearLerpFrame( lerpFrame_t *lf, animation_t *animations, int animationNumber, int time ) {
	lf->frameTime = lf->oldFrameTime = time; // time was cg.time
	BG_SetLerpFrameAnimation( lf, animations, animationNumber );
	lf->oldFrame = lf->frame = lf->animation->firstFrame;
}
#endif

#ifdef TMNTPLAYERSYS
/*
===============
BG_AnimationTime

Returns the msec to play the whole animation once.
===============
*/
int BG_AnimationTime(animation_t *anim)
{
	int time = 100 * 10; // 10 fps
	if (!anim)
		return time;

	// Based on SMOKIN_GUNS' PM_AnimLength(int anim)
	time = anim->frameLerp * anim->numFrames-1 + anim->initialLerp;
	if (anim->flipflop)
	{
		time *= 2;
		time -= anim->initialLerp;
	}

	return time;
}

#ifndef TMNTWEAPSYS_2
// These are in game, cgame, and ui, but not in bg - so its okay to use here...
int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void	trap_FS_Read( void *buffer, int len, fileHandle_t f );
void	trap_FS_FCloseFile( fileHandle_t f );
#endif

/*
===============
BG_SetDefaultAnimation

loadedAnim maybe NULL here, if it is,
	we assum all animations up to LEGS_TURN were loaded.
	(As that is how many there are

Now players only "need" four animations
	(BOTH_DEATH1, BOTH_DEAD1 LEGS_IDLE, and TORSO_STAND)
	Well I think players still need LEGS_WALKCR to set the
		legs frame skip correctly.

Returns qtrue if the animation has a default which could be set.
===============
*/
qboolean BG_SetDefaultAnimation(qboolean loadedAnim[], int index, animation_t *animations)
{
#define MAX_DEFAULT_ANIMATIONS 4
	int anim[MAX_DEFAULT_ANIMATIONS]; // Animations to use, best to default.
	qboolean reversed;
	qboolean flipflop;
	int i;

	memset(anim, -1, MAX_DEFAULT_ANIMATIONS * sizeof (int));
	reversed = qfalse;
	flipflop = qfalse;

	// Set defaults for quake3 and missionpack animations
	switch (index)
	{
		default:
			// no defaults.
			break;

		// Set defaults for quake3 animations
		case BOTH_DEATH1:
		case BOTH_DEAD1:
			// no defaults.
			break;

		case BOTH_DEATH2:
			anim[0] = BOTH_DEATH1;
			break;
		case BOTH_DEAD2:
			anim[0] = BOTH_DEAD1;
			break;
		case BOTH_DEATH3:
			anim[0] = BOTH_DEATH1;
			anim[1] = BOTH_DEATH2;
			break;
		case BOTH_DEAD3:
			anim[0] = BOTH_DEAD1;
			anim[1] = BOTH_DEAD2;
			break;

		case TORSO_GESTURE:
		case TORSO_ATTACK:
			anim[0] = TORSO_STAND;
			break;

		case TORSO_ATTACK2:
		case TORSO_DROP:
		case TORSO_RAISE:
			anim[0] = TORSO_STAND2;
			anim[1] = TORSO_STAND;
			break;

		case TORSO_STAND:
			// no default.
			break;

		case TORSO_STAND2:
			anim[0] = TORSO_STAND;
			break;

		case LEGS_WALKCR:
			anim[0] = LEGS_IDLECR;
			anim[1] = LEGS_IDLE;
			break;

		case LEGS_WALK:
			anim[0] = LEGS_IDLE;
			break;

		case LEGS_RUN:
			anim[0] = LEGS_WALK;
			anim[1] = LEGS_IDLE;
			break;
		case LEGS_BACK: // LEGS_BACKRUN
			anim[0] = LEGS_RUN;
			anim[1] = LEGS_WALK;
			anim[2] = LEGS_IDLE;
			reversed = qtrue;
			break;
		case LEGS_SWIM:
			anim[0] = LEGS_WALK;
			anim[1] = LEGS_IDLE;
			break;

		case LEGS_JUMP:
		case LEGS_LAND:
			anim[0] = LEGS_IDLE;
			break;

		case LEGS_JUMPB:
			anim[0] = LEGS_JUMP;
			anim[1] = LEGS_IDLE;
			break;
		case LEGS_LANDB:
			anim[0] = LEGS_LAND;
			anim[1] = LEGS_IDLE;
			break;

		case LEGS_IDLE:
			// no default.
			break;

		case LEGS_IDLECR:
		case LEGS_TURN:
			anim[0] = LEGS_IDLE;
			break;

		case LEGS_BACKCR:
			anim[0] = LEGS_WALKCR;
			anim[1] = LEGS_IDLECR;
			anim[2] = LEGS_IDLE;
			reversed = qtrue;
			break;
		case LEGS_BACKWALK:
			anim[0] = LEGS_WALK;
			anim[1] = LEGS_IDLE;
			reversed = qtrue;
			break;

		// Set defaults for missionpack animations
		case TORSO_GETFLAG:
		case TORSO_GUARDBASE:
		case TORSO_PATROL:
		case TORSO_FOLLOWME:
		case TORSO_AFFIRMATIVE:
		case TORSO_NEGATIVE:
			anim[0] = TORSO_GESTURE;
			anim[1] = TORSO_STAND2;
			anim[2] = TORSO_STAND;
			flipflop = qtrue;
			break;
		}


#ifdef TMNTPLAYERS
	// Set defaults for TMNT animations
		// default weapon, put away
	if (index >= TORSO_PUTDEFAULT_BOTH && index <= TORSO_PUTDEFAULT_SECONDARY)
		{
		anim[0] = TORSO_DROP;
		anim[1] = TORSO_STAND2;
		anim[2] = TORSO_STAND;
		}

		// default weapon, get out
	if (index >= TORSO_GETDEFAULT_BOTH && index <= TORSO_GETDEFAULT_SECONDARY)
		{
		anim[0] = TORSO_RAISE;
		anim[1] = TORSO_STAND2;
		anim[2] = TORSO_STAND;
		}

		// standing defaults
	if (index >= TORSO_STAND_GUN_PRIMARY && index <= TORSO_STAND_NUNCHUKS1_PRIMARY)
		{
		anim[0] = TORSO_STAND2;
		anim[1] = TORSO_STAND;
		}

		// attacking defaults
	if (index >= TORSO_ATTACK_GUN_PRIMARY && index <= TORSO_ATTACK_NUNCHUKS1_PRIMARY)
		{
		anim[0] = TORSO_ATTACK2;
		anim[1] = TORSO_ATTACK;
		}
#endif

	for (i = 0; i < MAX_DEFAULT_ANIMATIONS; i++)
	{
		if (anim[i] == -1)
			break;

		// Turtle Man: FIXME: Can't use a animation for this one,
		//    if it hasn't loaded it. I could "flag" it and load it
		//    after all the animtions have loaded...
		if (!loadedAnim && index <= anim[i])
		{
			Com_Printf("BG_SetDefaultAnimation: Failed. (index=%i, anim=%i)\n", index, anim[i]);
			continue;
		}

		if (!loadedAnim || (loadedAnim && loadedAnim[anim[i]] == qtrue))
		{
			//Com_Printf("BG_SetDefaultAnimation: Copy (index=%i, anim=%i)\n", index, anim[i]);
			memcpy(&animations[index], &animations[anim[i]], sizeof (animation_t));
			if (reversed) {
				animations[index].reversed = !animations[index].reversed;
			}
			//animations[i].flipflop = flipflop;
			return qtrue;
		}
	}

	//Com_Printf("BG_SetDefaultAnimation: Fail to set default animation for index %i.\n", index);
		return qfalse;
#undef MAX_DEFAULT_ANIMATIONS
}

/*
===============
BG_LoadAnimation

Returns qtrue if the animation loaded with out error.
===============
*/
int BG_LoadAnimation(char **text_p, int i, animation_t *animations, int *skip)
{
	char		*token;
	float		fps;

	token = COM_Parse( text_p );
	if ( !*token ) {
		//Com_Printf("DEBUG: Animtion missing first frame.\n");
		return -1;
	}
	animations[i].firstFrame = atoi( token );

	// This is odd with how the models are setup.
	// Why not just say the frame you want? --still here for compatibility.
	if (skip != NULL)
	{
		// leg only frames are adjusted to not count the upper body only frames
		if ( i == LEGS_WALKCR ) {
			*skip = animations[LEGS_WALKCR].firstFrame - animations[TORSO_GESTURE].firstFrame;
		}
		if ( i >= LEGS_WALKCR && i <= LEGS_TURN) {
			animations[i].firstFrame -= *skip;
		}
	}

	token = COM_Parse( text_p );
	if ( !*token ) {
		//Com_Printf("DEBUG: Animtion missing numFrame.\n");
		return 0;
	}
	animations[i].numFrames = atoi( token );

	animations[i].reversed = qfalse;
	animations[i].flipflop = qfalse;
	// if numFrames is negative the animation is reversed
	if (animations[i].numFrames < 0) {
		animations[i].numFrames = -animations[i].numFrames;
		animations[i].reversed = qtrue;
	}

	token = COM_Parse( text_p );
	if ( !*token ) {
		//Com_Printf("DEBUG: Animtion missing loopFrame.\n");
		return 0;
	}

	// Turtle Man: NOTE: After loading all frames we check if Elite Force style,
	//                     and convert it to Quake3 style loopFrames if needed.
		animations[i].loopFrames = atoi( token );

	token = COM_Parse( text_p );
	if ( !*token ) {
		//Com_Printf("DEBUG: Animtion missing fps.\n");
		return 0;
	}
	fps = atof( token );
	if ( fps == 0 ) {
		fps = 1;
	}
	animations[i].frameLerp = 1000 / fps;
	animations[i].initialLerp = 1000 / fps;

	return 1;
}

/*
===============
BG_ConvertEFAnimationsToQ3

Convert Elite Force loopFrames to Quake3 loopFrames
===============
*/
static void BG_ConvertEFAnimationsToQ3(animation_t *animations, int numanims)
{
	int			i;
	qboolean	ef_style = qfalse;

	// Check if ef style
	for ( i = 0 ; i < numanims ; i++ )
	{
		if (animations[i].loopFrames == -1)
		{
			// No quake3 skins should have this, only Elite Force.
			ef_style = qtrue;
		}
		else if (animations[i].loopFrames == 0)
		{
			// Shared by q3 and ef.
		}
		else
		{
			// EF only uses -1 and 0.
			ef_style = qfalse;
			break;
		}
	}

	if (ef_style)
	{
		// Convert to q3 style.
		for ( i = 0 ; i < MAX_ANIMATIONS ; i++ )
		{
			if (animations[i].loopFrames == -1)
			{
				animations[i].loopFrames = 0;
				break;
			}
			else if (animations[i].loopFrames == 0)
			{
				animations[i].loopFrames = animations[i].numFrames;
			}
		}
	}
}


/*
======================
BG_ParsePlayerCFGFile

Based on Quake3's CG_ParseAnimationFile
  This was been moved to BG because its used in game, cgame, and ui,
  This way it is a lot easier to make changes to animation.cfg loading...

Read a configuration file containing animation and game config for the player.
models/players/raph/animation.cfg, etc

Supports Quake3 and Elite Force animation styles.

Quake3 style (also used by Elite Force MP): (TMNT_SUPPORTQ3 must be defined)
0	31	0	20		// BOTH_DEATH1
30	1	0	20		// BOTH_DEAD1
31	31	0	20		// BOTH_DEATH2
61	1	0	20		// BOTH_DEAD2
62	33	0	20		// BOTH_DEATH3
94	1	0	20		// BOTH_DEAD3

Elite Force (SP) style:
BOTH_DEATH1		0	31	0	20
BOTH_DEAD1		30	1	0	20
BOTH_DEATH2		31	31	0	20
BOTH_DEAD2		61	1	0	20
BOTH_DEATH3		62	33	0	20
BOTH_DEAD3		94	1	0	20

Both styles also support Elite Force (SP/MP) "loopFrames" (-1 = no loop, 0 = loop all frames.)
  NOTE: q3 style "loopFrames" (0 = no loop, # = loop num frames, normally equal to numframes.)

Turtle Man: NOTE: I changed it to parse animation name instead of having a defined order
			(and a comment with the name.) and then found out it was the same
			in Elite Force Single Player, so I added support for elite force "loopFrames"
======================
*/
qboolean BG_ParsePlayerCFGFile(const char *filename, bg_playercfg_t *playercfg ) {
	char		*text_p, *prev;
	int			len;
	int			i;
	char		*token;
	int			skip;
	char		text[20000];
	fileHandle_t	f;
	animation_t *animations;
	qboolean foundAnim;
	qboolean loadedAnim[MAX_TOTALANIMATIONS];
	int rtn = 0;

	animations = playercfg->animations;
	foundAnim = qfalse;
	memset(loadedAnim, qfalse, MAX_TOTALANIMATIONS * sizeof (qboolean));

	// Defaults moved to BG_LoadPlayerCFGFile

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		Com_Printf( "File %s too long\n", filename );
		trap_FS_FCloseFile( f );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;
	skip = 0;	// quite the compiler warning

	// read optional parameters
	while ( 1 ) {
		prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );

		// Turtle Man: NOTE: Must check for (!*token) because (!token) is ALWAYS false.
		//       This is a bug in the Quake3 source.
		//       It doesn't cause a problem in the animation.cfg parse
		//          due to special checking/breaking for animations.
		if ( !*token ) {
			break;
		}

		// Skip animation keywords.
		if ( !Q_stricmp( token, "footsteps" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			if ( !Q_stricmp( token, "default" ) || !Q_stricmp( token, "normal" ) ) {
				playercfg->footsteps = FOOTSTEP_NORMAL;
			} else if ( !Q_stricmp( token, "boot" ) ) {
				playercfg->footsteps = FOOTSTEP_BOOT;
			} else if ( !Q_stricmp( token, "flesh" ) ) {
				playercfg->footsteps = FOOTSTEP_FLESH;
			} else if ( !Q_stricmp( token, "mech" ) ) {
				playercfg->footsteps = FOOTSTEP_MECH;
			} else if ( !Q_stricmp( token, "energy" ) ) {
				playercfg->footsteps = FOOTSTEP_ENERGY;
			// Turtle Man: Hey why not, its valid. (Not in Q3)
			} else if ( !Q_stricmp( token, "splash" ) ) {
				playercfg->footsteps = FOOTSTEP_SPLASH;
#ifdef TMNT_SUPPORTQ3 // TMNT_SUPPORTEF // Turtle Man: Elite Force
			} else if ( !Q_stricmp( token, "borg" ) ) {
				playercfg->footsteps = FOOTSTEP_MECH;
			} else if ( !Q_stricmp( token, "reaver" ) ) {
				playercfg->footsteps = FOOTSTEP_NORMAL;
			} else if ( !Q_stricmp( token, "species" ) ) {
				playercfg->footsteps = FOOTSTEP_FLESH;
			} else if ( !Q_stricmp( token, "warbot" ) ) {
				playercfg->footsteps = FOOTSTEP_NORMAL;
#endif
			} else {
				Com_Printf( "Bad footsteps parm in %s: %s\n", filename, token );
			}
			continue;
		} else if ( !Q_stricmp( token, "headoffset" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !*token ) {
					break;
				}
				playercfg->headOffset[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "sex" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			if ( token[0] == 'f' || token[0] == 'F' ) {
				playercfg->gender = GENDER_FEMALE;
			} else if ( token[0] == 'n' || token[0] == 'N' ) {
				playercfg->gender = GENDER_NEUTER;
			} else {
				playercfg->gender = GENDER_MALE;
			}
			continue;
		} else if ( !Q_stricmp( token, "fixedlegs" ) ) {
			playercfg->fixedlegs = qtrue;
			continue;
		} else if ( !Q_stricmp( token, "fixedtorso" ) ) {
			playercfg->fixedtorso = qtrue;
			continue;
		}

		// TMNT_SUPPORTEF
		// Turtle Man: Support Elite Force Stuff
		//
		else if ( !Q_stricmp( token, "soundpath" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			Com_sprintf(playercfg->soundpath, sizeof (playercfg->soundpath), "%s", token);
			continue;
		}

		//
		// NEW TMNT STUFF
		//

		// boundingbox -15 -15 -24 15 15 32
		else if ( !Q_stricmp( token, "boundingbox" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !*token ) {
					break;
				}
				playercfg->bbmins[i] = atof( token );
			}
			if (i == 3) // found all tokens
			{
				for ( i = 0 ; i < 3 ; i++ ) {
					token = COM_Parse( &text_p );
					if ( !*token ) {
						break;
					}
					playercfg->bbmaxs[i] = atof( token );
				}
			}
			continue;
		}

		// Turtle Man: TODO: Add more per-player stuff.

		// Turtle Man: TODO: Jump Percent 1-100 ?
		else if ( !Q_stricmp( token, "jumpheight" ) ) {
		}
		// Turtle Man: TODO: Hud color in SP (and non-team gametypes? or use "color1" in non-team?)
		// color1 and color2 were used for the railgun blast color
		else if ( !Q_stricmp( token, "prefcolor1" ) ) {
			// Like color1
		}
		//else if ( !Q_stricmp( token, "prefcolor2" ) ) {
		//	// Like color2
		//}
		else if ( !Q_stricmp( token, "ability" ) ) {
			// Leo: Blade (Cut metal and wood, ect)
			///     --This should be weapon flag, not player flag
			///        there should be something else leo can do with "any" weapon held.
			// Don: Tech (Operate computers to open doors and stuff)
			// Raph: Strength (Break rocks, lift/push heavy objects)
			//     -- Use Sais to climb walls?
			// Mike: Speed (run faster)
			///     --This shouldn't be ability...
			///       Don't really have a clue what it should be... not be flying.
		}
		// "speed" is the max speed that the
		// player runs without powerups.
		// Based on SRB2's "normalspeed"
		else if ( !Q_stricmp( token, "speed" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			playercfg->max_speed = atoi( token );
			continue;
		}
		// Speed to start run animtion.
		else if ( !Q_stricmp( token, "runspeed" ) ) {
		}
		// Accel when move starts.
		else if ( !Q_stricmp( token, "accelstart" ) ) {
		}
		else if ( !Q_stricmp( token, "acceleration" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			playercfg->accelerate_speed = atof( token );
			continue;
		}

		//else if ( !Q_stricmp( token, "thrustfactor" ) ) {
		//}

#ifdef TMNTWEAPSYS // Turtle Man: DEFAULT_WEAPON
		else if ( !Q_stricmp( token, "default_weapon" ) ) {
#ifdef TMNTWEAPSYS_2
			int j;
#endif
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
#ifdef TMNTWEAPSYS_2
			j = BG_WeaponGroupIndexForName(token);
			if (j)
			{
				// GUNS_AS_DEFAULT
				if (bg_weapongroupinfo[j].weapon[0] && bg_weapongroupinfo[j].weapon[0]->weapontype == WT_GUN)
				{
					Com_Printf( "Bad default_weapon parm, guns not allowed, in %s\n", filename );
				}
				else
				{
					playercfg->default_weapon = j;
				}
			}
			else {
				Com_Printf( "Bad default_weapon parm in %s: %s\n", filename, token );
			}
#else
			if ( !Q_stricmp( token, "WP_NONE" ) ) {
				playercfg->default_weapon = WP_NONE;
			}
#ifdef TMNTWEAPONS
			else if ( !Q_stricmp( token, "WP_FISTS" ) ) {
				playercfg->default_weapon = WP_FISTS;
			} else if ( !Q_stricmp( token, "WP_KATANAS" ) ) {
				playercfg->default_weapon = WP_KATANAS;
			} else if ( !Q_stricmp( token, "WP_DAISHO" ) ) {
				playercfg->default_weapon = WP_DAISHO;
			} else if ( !Q_stricmp( token, "WP_SAIS" ) ) {
				playercfg->default_weapon = WP_SAIS;
			} else if ( !Q_stricmp( token, "WP_NUNCHUKS" ) ) {
				playercfg->default_weapon = WP_NUNCHUKS;
			} else if ( !Q_stricmp( token, "WP_HAMMER" ) ) {
				playercfg->default_weapon = WP_HAMMER;
			} else if ( !Q_stricmp( token, "WP_AXE" ) ) {
				playercfg->default_weapon = WP_AXE;
			} else if ( !Q_stricmp( token, "WP_LONGSWORD" ) ) {
				playercfg->default_weapon = WP_LONGSWORD;
			} else if ( !Q_stricmp( token, "WP_BAT" ) ) {
				playercfg->default_weapon = WP_BAT;
			} else if ( !Q_stricmp( token, "WP_BO" ) ) {
				playercfg->default_weapon = WP_BO;
			} else if ( !Q_stricmp( token, "WP_BAMBOOBO" ) ) {
				playercfg->default_weapon = WP_BAMBOOBO;
			}
			// GUNS_AS_DEFAULT
#if 1 // Turtle Man: Guns are not allowed as default weapons.
			else if ( !Q_stricmp( token, "WP_GUN" )
				|| !Q_stricmp( token, "WP_ELECTRIC_LAUNCHER" )
				|| !Q_stricmp( token, "WP_ROCKET_LAUNCHER" )
				|| !Q_stricmp( token, "WP_HOMING_LAUNCHER" ) ) {
				Com_Printf( "Bad default_weapon parm, guns not allowed, in %s\n", filename );
			}
#else
			else if ( !Q_stricmp( token, "WP_GUN" ) ) {
				playercfg->default_weapon = WP_GUN;
			} else if ( !Q_stricmp( token, "WP_ELECTRIC_LAUNCHER" ) ) {
				playercfg->default_weapon = WP_ELECTRIC_LAUNCHER;
			} else if ( !Q_stricmp( token, "WP_ROCKET_LAUNCHER" ) ) {
				playercfg->default_weapon = WP_ROCKET_LAUNCHER;
			} else if ( !Q_stricmp( token, "WP_HOMING_LAUNCHER" ) ) {
				playercfg->default_weapon = WP_HOMING_LAUNCHER;
			}
#endif
			else if ( !Q_stricmp( token, "WP_GRAPPLING_HOOK" ) ) {
				playercfg->default_weapon = WP_GRAPPLING_HOOK;
			}
#else
			// Quake 3 players don't have a "default_weapon" field in there config
			//  so there is no point in adding the Q3 weapons here...
			//  But I did it anyway... Turtle Man
			else if ( !Q_stricmp( token, "WP_GAUNTLET" ) ) {
				playercfg->default_weapon = WP_GAUNTLET;
			} else if ( !Q_stricmp( token, "WP_MACHINEGUN" ) ) {
				playercfg->default_weapon = WP_MACHINEGUN;
			} else if ( !Q_stricmp( token, "WP_SHOTGUN" ) ) {
				playercfg->default_weapon = WP_SHOTGUN;
			} else if ( !Q_stricmp( token, "WP_GRENADE_LAUNCHER" ) ) {
				playercfg->default_weapon = WP_GRENADE_LAUNCHER;
			} else if ( !Q_stricmp( token, "WP_ROCKET_LAUNCHER" ) ) {
				playercfg->default_weapon = WP_ROCKET_LAUNCHER;
			} else if ( !Q_stricmp( token, "WP_LIGHTNING" ) ) {
				playercfg->default_weapon = WP_LIGHTNING;
			} else if ( !Q_stricmp( token, "WP_RAILGUN" ) ) {
				playercfg->default_weapon = WP_RAILGUN;
			} else if ( !Q_stricmp( token, "WP_PLASMAGUN" ) ) {
				playercfg->default_weapon = WP_PLASMAGUN;
			} else if ( !Q_stricmp( token, "WP_BFG" ) ) {
				playercfg->default_weapon = WP_BFG;
			} else if ( !Q_stricmp( token, "WP_GRAPPLING_HOOK" ) ) {
				playercfg->default_weapon = WP_GRAPPLING_HOOK;
			}
			// Don't allow MISSIONPACK weapons as default...
#endif
			else {
				Com_Printf( "Bad default_weapon parm in %s: %s\n", filename, token );
			}
#endif
			continue;
		}
#endif

#if 0 //#ifdef TMNTPLAYERSYS_2
		else if ( !Q_strncasecmp( token, "BOTH_", 5 ) ) {
			//
		}
		else if ( !Q_strncasecmp( token, "TORSO_", 6 ) ) {
			//
		}
		else if ( !Q_strncasecmp( token, "LEGS_", 5 ) ) {
			//
		}
#else
	// Turtle Man: I know this is bad way to code...
// x = BOTH_DEATH1 (ect)
#define LOADANIM(x) \
		else if ( !Q_stricmp( token, #x ) ) { \
			foundAnim = qtrue; \
			rtn = BG_LoadAnimation(&text_p, x, animations, &skip);	\
			if (rtn == -1)	\
			{ \
				BG_SetDefaultAnimation(loadedAnim, x, animations);	\
				loadedAnim[x] = qtrue;	\
			} else if (rtn == 0) {	\
				Com_Printf("BG_ParsePlayerCFGFile: Anim %s: Failed loading.\n", #x); \
			} \
			else {	\
				loadedAnim[x] = qtrue;	\
			}	\
			continue; \
		}

// x = BOTH_DEATH1 (ect) or int
// y = string holding name, "BOTH_DEATH1"
#define LOADANIM_(x, y) \
		else if ( !Q_stricmp( token, y ) ) { \
			foundAnim = qtrue; \
			rtn = BG_LoadAnimation(&text_p, x, animations, &skip);	\
			if (rtn == -1) \
			{ \
				BG_SetDefaultAnimation(loadedAnim, x, animations);	\
				loadedAnim[x] = qtrue;	\
			} else if (rtn == 0) {	\
				Com_Printf("BG_ParsePlayerCFGFile: Anim %s: Failed loading.\n", y); \
			} \
			else {	\
				loadedAnim[x] = qtrue;	\
			}	\
			continue; \
		}

		// Turtle Man: New animation loading.
		LOADANIM(BOTH_DEATH1)
		LOADANIM(BOTH_DEAD1)
		LOADANIM(BOTH_DEATH2)
		LOADANIM(BOTH_DEAD2)
		LOADANIM(BOTH_DEATH3)
		LOADANIM(BOTH_DEAD3)

		LOADANIM(TORSO_GESTURE)

#if !defined TMNT || defined TMNT_SUPPORTQ3 // animation.cfg
		LOADANIM(TORSO_ATTACK)
		LOADANIM(TORSO_ATTACK2)
#endif
		LOADANIM_(TORSO_ATTACK, "TORSO_ATTACK_GUN")
		LOADANIM_(TORSO_ATTACK2, "TORSO_ATTACK_GUNTLET")

		LOADANIM(TORSO_DROP)
		LOADANIM(TORSO_RAISE)

#if !defined TMNT || defined TMNT_SUPPORTQ3 // animation.cfg
		LOADANIM(TORSO_STAND)
		LOADANIM(TORSO_STAND2)
#endif
		LOADANIM_(TORSO_STAND, "TORSO_STAND_GUN")
		LOADANIM_(TORSO_STAND2, "TORSO_STAND_GUNTLET")

		// MISSIONPACK animations.
		LOADANIM(TORSO_GETFLAG)
		LOADANIM(TORSO_GUARDBASE)
		LOADANIM(TORSO_PATROL)
		LOADANIM(TORSO_FOLLOWME)
		LOADANIM(TORSO_AFFIRMATIVE)
		LOADANIM(TORSO_NEGATIVE)

		LOADANIM(LEGS_WALKCR)
		LOADANIM(LEGS_WALK)
		LOADANIM(LEGS_RUN)
		LOADANIM(LEGS_BACK)
		LOADANIM(LEGS_SWIM)
		LOADANIM(LEGS_JUMP)
		LOADANIM(LEGS_LAND)
		LOADANIM(LEGS_JUMPB)
		LOADANIM(LEGS_LANDB)
		LOADANIM(LEGS_IDLE)
		LOADANIM(LEGS_IDLECR)
		LOADANIM(LEGS_TURN)

		// Quake3 doesn't load these
		// but instead uses the forward ones in reverse
		LOADANIM(LEGS_BACKCR)
		LOADANIM(LEGS_BACKWALK)

#ifdef TMNTPLAYERS
        // Turtle Man: New TMNT Animations

		LOADANIM(TORSO_PUTDEFAULT_BOTH)
		LOADANIM(TORSO_PUTDEFAULT_PRIMARY)
		LOADANIM(TORSO_PUTDEFAULT_SECONDARY)
		LOADANIM(TORSO_GETDEFAULT_BOTH)
		LOADANIM(TORSO_GETDEFAULT_PRIMARY)
		LOADANIM(TORSO_GETDEFAULT_SECONDARY)

		// Pickup weapons should support CTF flags too?
		// TORSO_PUT_SECONDARY,
		// TORSO_GET_SECONDARY,

		// Gun-type standing animations
		//TORSO_STAND_GUNTLET, // Would be the same as TORSO_STAND2...
		//TORSO_STAND_GUN, // Would be the same as TORSO_STAND...
		LOADANIM(TORSO_STAND_GUN_PRIMARY) // I could reuse TORSO_STAND2 even though its for the gauntlet...

		// Melee weapon standing animations
		LOADANIM(TORSO_STAND_SWORD1_BOTH)
		LOADANIM(TORSO_STAND_SWORD1_PRIMARY)

		LOADANIM(TORSO_STAND_SWORD2)
		LOADANIM(TORSO_STAND_DAISHO)

		LOADANIM(TORSO_STAND_SAI2)
		LOADANIM(TORSO_STAND_SAI1_PRIMARY)

		LOADANIM(TORSO_STAND_BO)
		LOADANIM(TORSO_STAND_BO_PRIMARY)

		LOADANIM(TORSO_STAND_HAMMER)
		LOADANIM(TORSO_STAND_HAMMER_PRIMARY)

		LOADANIM(TORSO_STAND_NUNCHUKS)
		LOADANIM(TORSO_STAND_NUNCHUKS1_PRIMARY)

		// Gun attacks
		//TORSO_ATTACK_GUNTLET, // Would be the same as TORSO_ATTACK2...
		//TORSO_ATTACK_GUN, // Would be the same as TORSO_ATTACK...
		LOADANIM(TORSO_ATTACK_GUN_PRIMARY) // Can't reuse TORSO_ATTACK2 needs a new animation.

		// Turtle Man: TODO: Attack animations, how many anims per type?...
		//       Only one for WT_HAMMER and WT_GUN
		LOADANIM(TORSO_ATTACK_SWORD1_BOTH)
		//LOADANIM(TORSO_ATTACK_SWORD1_BOTH_2)
		//LOADANIM(TORSO_ATTACK_SWORD1_BOTH_3)

		LOADANIM(TORSO_ATTACK_SWORD1_PRIMARY)
		//LOADANIM(TORSO_ATTACK_SWORD1_PRIMARY_2)
		//LOADANIM(TORSO_ATTACK_SWORD1_PRIMARY_3)

		LOADANIM(TORSO_ATTACK_SWORD2)
		LOADANIM(TORSO_ATTACK_DAISHO)

		LOADANIM(TORSO_ATTACK_SAI2)
		LOADANIM(TORSO_ATTACK_SAI1_PRIMARY)

		LOADANIM(TORSO_ATTACK_BO)
		LOADANIM(TORSO_ATTACK_BO_PRIMARY)

		LOADANIM(TORSO_ATTACK_HAMMER)
		LOADANIM(TORSO_ATTACK_HAMMER_PRIMARY)

		LOADANIM(TORSO_ATTACK_NUNCHUKS)
		LOADANIM(TORSO_ATTACK_NUNCHUKS1_PRIMARY)
#endif // TMNTPLAYERS

#undef LOADANIM
#undef LOADANIM_
#endif

#if !defined TMNT || defined TMNT_SUPPORTQ3 // animation.cfg
		// if it is a number, start parsing animations
		if ( token[0] >= '0' && token[0] <= '9' ) {
			text_p = prev;	// unget the token
			break;
		}
#endif

		Com_Printf( "unknown token '%s' in %s\n", token, filename );
	}

	// Found an Elite Foce (SP) style animation.
	if (foundAnim)
	{
		// Check for missing animations and load there defaults.
		for ( i = 0 ; i < MAX_TOTALANIMATIONS ; i++ )
		{
			if (loadedAnim[i] == qfalse)
			{
				// Load the default for this animation.
				BG_SetDefaultAnimation(loadedAnim, i, animations);
			}
		}
	}
#if !defined TMNT || defined TMNT_SUPPORTQ3 // animation.cfg
	else
	{
		// Assume Quake3 (or Elite Force MP) player.
		// read information for each frame
#if 0 // #ifdef IOQ3ZTM // now could load LEGS_BACKCR and LEGS_BACKWALK
		for ( i = 0 ; i < MAX_TOTALANIMATIONS ; i++ )
#else
		for ( i = 0 ; i < MAX_ANIMATIONS ; i++ )
#endif
		{
			rtn = BG_LoadAnimation(&text_p, i, animations, &skip);
			if (rtn == -1)
			{
				BG_SetDefaultAnimation(loadedAnim, i, animations);
				loadedAnim[i] = qtrue;
			} else if (rtn == 0) {
				Com_Printf("BG_ParsePlayerCFGFile: Animation %d: Failed loading.\n", i);
				break;
			}
			else {
				loadedAnim[i] = qtrue;
			}
		}

#if 0 // #ifdef IOQ3ZTM
		if ( i != MAX_TOTALANIMATIONS )
#else
		// These are not loaded from quake3 players.
		BG_SetDefaultAnimation(loadedAnim, LEGS_BACKCR, animations);
		BG_SetDefaultAnimation(loadedAnim, LEGS_BACKWALK, animations);

		if ( i != MAX_ANIMATIONS )
#endif
		{
			BG_ConvertEFAnimationsToQ3(animations, MAX_TOTALANIMATIONS);
			Com_Printf( "Error parsing animation file: %s", filename );
			return qfalse;
		}
	}
#else
	// Didn't find any animations.
	if (foundAnim == qfalse)
	{
		Com_Printf( "Error: No animations in file: %s", filename );
		return qfalse;
	}
#endif

#ifdef TMNT_SUPPORTQ3 // TMNT_SUPPORTEF
	BG_ConvertEFAnimationsToQ3(animations, MAX_TOTALANIMATIONS);
#endif

	return qtrue;
}

/*
===============
BG_LoadPlayerCFGFile

Load animation.cfg for model into playercfg
===============
*/
qboolean BG_LoadPlayerCFGFile(bg_playercfg_t *playercfg, const char *model, const char *headModel)
{
	char filename[MAX_QPATH];
	int i;

	if (!model)
	{
		model = DEFAULT_MODEL;
	}

	Q_strncpyz(playercfg->model, model, MAX_QPATH);

	// Setup defaults
	playercfg->footsteps = FOOTSTEP_NORMAL;
	VectorClear( playercfg->headOffset );
	playercfg->gender = GENDER_MALE;
	playercfg->fixedlegs = qfalse;
	playercfg->fixedtorso = qfalse;

	// Use the head model name for the default soundpath.
	Q_strncpyz(playercfg->soundpath, headModel, sizeof (playercfg->soundpath));

#ifdef TMNTWEAPSYS
#ifdef TMNTWEAPSYS_2 // Turtle Man: TODO: Should default weapon be loaded from weaponinfo.txt?
	playercfg->default_weapon = BG_WeaponGroupIndexForName(DEFAULT_DEFAULT_WEAPON);
	// If the default weapon wasn't found...
	if (!playercfg->default_weapon)
	{
		int max = BG_NumWeaponGroups();
		for (i = 1; i < max; i++)
		{
			if (bg_weapongroupinfo[i].weapon[0] &&
				bg_weapongroupinfo[i].weapon[0]->weapontype == WT_GUN)
			{
				continue;
			}
			playercfg->default_weapon = i;
			break;
		}
		if (i == max && max > 0)
		{
			// No melee weapon so fall back to first weapon.
			playercfg->default_weapon = 1;
		}
	}
#else
	playercfg->default_weapon = DEFAULT_DEFAULT_WEAPON;
#endif
#endif
	// Default to Q3 bounding box (If changed update game's SpotWouldTelefrag)
	VectorSet(playercfg->bbmins,-15, -15, -24); // playerMins
	VectorSet(playercfg->bbmaxs, 15,  15,  32); // playerMaxs

	playercfg->max_speed = 320;
	playercfg->accelerate_speed = 10.0f;

	// Use first frame for all animations.
	for (i = 0; i < MAX_TOTALANIMATIONS; i++)
	{
		playercfg->animations[i].firstFrame = 0;
		playercfg->animations[i].numFrames = 1;
		playercfg->animations[i].loopFrames = 0;
		playercfg->animations[i].frameLerp = 100; // 10 fps
		playercfg->animations[i].initialLerp = 100; // 10 fps
		playercfg->animations[i].reversed = 0;
		playercfg->animations[i].flipflop = 0;
	}

#ifndef IOQ3ZTM // FLAG_ANIMATIONS
	//
	// Setup flag animations. For the flag not player.
	//
	// flag moving fast
	animations[FLAG_RUN].firstFrame = 0;
	animations[FLAG_RUN].numFrames = 16;
	animations[FLAG_RUN].loopFrames = 16;
	animations[FLAG_RUN].frameLerp = 1000 / 15;
	animations[FLAG_RUN].initialLerp = 1000 / 15;
	animations[FLAG_RUN].reversed = qfalse;
	// flag not moving or moving slowly
	animations[FLAG_STAND].firstFrame = 16;
	animations[FLAG_STAND].numFrames = 5;
	animations[FLAG_STAND].loopFrames = 0;
	animations[FLAG_STAND].frameLerp = 1000 / 20;
	animations[FLAG_STAND].initialLerp = 1000 / 20;
	animations[FLAG_STAND].reversed = qfalse;
	// flag speeding up
	animations[FLAG_STAND2RUN].firstFrame = 16;
	animations[FLAG_STAND2RUN].numFrames = 5;
	animations[FLAG_STAND2RUN].loopFrames = 1;
	animations[FLAG_STAND2RUN].frameLerp = 1000 / 15;
	animations[FLAG_STAND2RUN].initialLerp = 1000 / 15;
	animations[FLAG_STAND2RUN].reversed = qtrue;
#endif

#ifdef IOQ3ZTM // PLAYER_DIR
	// load animation.cfg
	for (i=0; bg_playerDirs[i] != NULL; i++)
	{
		Com_sprintf( filename, sizeof( filename ), "%s/%s/animation.cfg", bg_playerDirs[i], model );
		if ( BG_ParsePlayerCFGFile(filename, playercfg) )
		{
			return qtrue;
		}
	}

	Com_Printf( "Failed to load animation.cfg for %s\n", model );
	return qfalse;
#else
	// load animation.cfg
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/animation.cfg", model );
	if ( !BG_ParsePlayerCFGFile(filename, playercfg) ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/animation.cfg", model );
		if ( !BG_ParsePlayerCFGFile(filename, playercfg) ) {
			Com_Printf( "Failed to load animation.cfg for %s\n", model );
			return qfalse;
		}
	}
	return qtrue;
#endif
}
#endif

#ifdef TMNTENTSYS // MISC_OBJECT
/*
// example file for misc_object
boundingbox -16 -16 0 16 16 32
health 15
wait 0
speed 1
OBJECT_NORMAL 0 10 10 10
OBJECT_DAMAGED1 20 10 0 10
OBJECT_DAMAGED2 30 10 0 10
OBJECT_DAMAGED3 40 10 0 10
OBJECT_KILLED 50 10 0 10
*/

/*
===============
BG_ParseObjectCFGFile
===============
*/
qboolean BG_ParseObjectCFGFile(const char *filename, const char *anim_names[],
	animation_t *animations, int num_anim, vec3_t *mins, vec3_t *maxs, int *health, int *wait,
	float *speed, int *lerpframes)
{
	char		*text_p, *prev;
	int			len;
	int			i;
	char		*token;
	char		text[20000];
	fileHandle_t	f;
	qboolean foundAnim;

	foundAnim = qfalse;

	if (!animations) {
		num_anim = 0;
	}

	// Use first frame for all animations.
	for (i = 0; i < num_anim; i++)
	{
		animations[i].firstFrame = 0;
		animations[i].numFrames = 1;
		animations[i].loopFrames = 0;
		animations[i].frameLerp = 100; // 10 fps
		animations[i].initialLerp = 100; // 10 fps
		animations[i].reversed = 0;
		animations[i].flipflop = 0;
	}

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		Com_Printf( "File %s too long\n", filename );
		trap_FS_FCloseFile( f );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;

	// read optional parameters
	while ( 1 ) {
		prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}

		// boundingbox -15 -15 -24 15 15 32
		if ( !Q_stricmp( token, "boundingbox" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !*token ) {
					break;
				}
				if (mins)
				{
					*mins[i] = atof( token );
				}
			}
			if (i == 3) // found all min tokens
			{
				for ( i = 0 ; i < 3 ; i++ ) {
					token = COM_Parse( &text_p );
					if ( !*token ) {
						break;
					}
					if (maxs)
					{
						*maxs[i] = atof( token );
					}
				}
			}
			continue;
		}
		// spawnhealth
		else if ( !Q_stricmp( token, "health" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			// store health
			if (health)
				*health = atoi(token);
			continue;
		}
		else if ( !Q_stricmp( token, "wait" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			// store wait
			if (wait)
				*wait = atoi(token);
			continue;
		}
		else if ( !Q_stricmp( token, "speed" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
			}
			// store speed
			if (speed)
				*speed = atof(token);
			continue;
			}
		else if ( !Q_stricmp( token, "lerpframes" ) ) {
			token = COM_Parse( &text_p );
			if ( !*token ) {
				break;
		}
			// store whether to lerpframes
			if (lerpframes)
				*lerpframes = atoi(token);
			continue;
		}
		else
		{
			qboolean animName = qfalse;
		// Turtle Man: New animation loading.
		for (i = 0; i < num_anim; i++)
		{
				if ( !Q_stricmp( token, anim_names[i] ) )
				{
					animName = qtrue;
					foundAnim = qtrue;
					if (BG_LoadAnimation(&text_p, i, animations, NULL) != 1)
					{
						Com_Printf("BG_ParseObjectCFGFile: Anim %s: Failed loading.\n", anim_names[i]);
					}
					break;
				}
			}
			if (animName)
				continue;
		}

#if 0
		// if it is a number, start parsing animations
		if ( token[0] >= '0' && token[0] <= '9' ) {
			text_p = prev;	// unget the token
			break;
		}
#endif

		Com_Printf( "unknown token '%s' in %s\n", token, filename );
	}

#if 0
	// Didn't find any Elite Foce (SP) style animations.
	if (foundAnim == qfalse)
	{
		// Assume Quake3 (or Elite Force MP) style.
		// read information for each frame
		for ( i = 0 ; i < num_anim; i++ )
		{
			if (BG_LoadAnimation(&text_p, i, animations, NULL) != 1)
			{
				Com_Printf("BG_ParseObjectCFGFile: Animation %d: Failed loading.\n", i);
				break;
			}
		}
	}
#endif

	return qtrue;
}
#endif

#ifdef IOQ3ZTM // PLAYER_DIR
// Quake3 (Team Arena) atemps to load players from
//   "models/players" and "models/players/characters" but in the final game
//   there is no "models/players/characters"
// Elite Force has the Holomatch (Multiplayer) players in "models/players2"
//   so I generlized the player dir code.
//
// Last pointer is NULL
// NOTE: Only the first two are listed in the player select menus
//  q3_ui: PlayerModel_BuildList    ui: UI_BuildQ3Model_List
const char *bg_playerDirs[MAX_PLAYER_DIRS] =
{
	"models/players",
#ifdef TMNT_SUPPORTQ3 // TMNT_SUPPORTEF
	"models/players2",
#endif
#ifndef TMNT
	// Turtle Man: Do any players use this? There isn't any reason to,
	//             unless you want to have players that aren't in the player select menu.
	"models/players/characters",
#endif
	NULL
};
#endif
