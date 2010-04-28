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

#define INVENTORY_NONE				0
#define INVENTORY_HEALTH			1

//#ifdef TA_WEAPSYS_EX
// The most weapons a player can have is 2!
//  (well they do have 3 when switching pickup for pickup and have a default weapon)
// Default weapon for the player,
// If INVENTORY_WAEPON is different, the default can be used by dropping current weapon.
#define INVENTORY_DEFAULTWEAPON		2
// Weapon the player is using
#define INVENTORY_WEAPON			3
// Ammo for current weapon,
#define INVENTORY_AMMO 				4
//#endif

// MISSIONPACK
#define INVENTORY_PERSISTANT_POWER	5
//#ifdef MISSIONPACK_HARVESTER
//#define INVENTORY_REDCUBE			0
//#define INVENTORY_BLUECUBE		0
//#endif

// holdable items
// Q3 holdables
#define INVENTORY_MEDKIT			15
// MISSIONPACK holdables
#define INVENTORY_PORTAL			16

// TA_HOLDABLE
#define INVENTORY_SHURIKEN			17
#define INVENTORY_ELECTRICSHURIKEN	18
#define INVENTORY_FIRESHURIKEN		19
#define INVENTORY_LASERSHURIKEN		20

// powerups
#define INVENTORY_QUAD				30
#define INVENTORY_ENVIRONMENTSUIT	31
#define INVENTORY_HASTE				32
#define INVENTORY_INVISIBILITY		33
#define INVENTORY_FLIGHT			34
#define INVENTORY_INVUL				35 // TURTLEARENA // POWERS
#define INVENTORY_REDFLAG			36
#define INVENTORY_BLUEFLAG			37
#define INVENTORY_NEUTRALFLAG		38

// MISSIONPACK persistant powers
#define INVENTORY_SCOUT				39
#define INVENTORY_GUARD				40
#define INVENTORY_DOUBLER			41
#define INVENTORY_AMMOREGEN			42

// 45-49 are free

//TA_WEAPSYS
#define INVENTORY_WEAPON_START 50 // 31 slots starting here
#define INVENTORY_AMMO_START 81 // 31 slots starting here

//enemy stuff
#define ENEMY_HORIZONTAL_DIST		200
#define ENEMY_HEIGHT				201
#define NUM_VISIBLE_ENEMIES			202
#define NUM_VISIBLE_TEAMMATES		203

// MODELINDEX_*
//item numbers (make sure they are in sync with bg_itemlist in bg_misc.c)
#define MODELINDEX_HEALTHSMALL		1
#define MODELINDEX_HEALTH			2
#define MODELINDEX_HEALTHLARGE		3
#define MODELINDEX_HEALTHMEGA		4

#define MODELINDEX_MEDKIT			5
#define MODELINDEX_SHURIKEN			6
#define MODELINDEX_ELECTRICSHURIKEN	7
#define MODELINDEX_FIRESHURIKEN		8
#define MODELINDEX_LASERSHURIKEN	9

#define MODELINDEX_QUAD				10
#define MODELINDEX_ENVIRONMENTSUIT	11
#define MODELINDEX_HASTE			12
#define MODELINDEX_INVISIBILITY		13
#define MODELINDEX_FLIGHT			14
#define MODELINDEX_INVUL			15 // TURTLEARENA // POWERS
#define MODELINDEX_REDFLAG			16
#define MODELINDEX_BLUEFLAG			17

// mission pack only defines
#define MODELINDEX_PORTAL			18

#define MODELINDEX_SCOUT			19
#define MODELINDEX_GUARD			20
#define MODELINDEX_DOUBLER			21
#define MODELINDEX_AMMOREGEN		22

#define MODELINDEX_NEUTRALFLAG		23
//#ifdef MISSIONPACK_HARVESTER
//#define MODELINDEX_REDCUBE			0
//#define MODELINDEX_BLUECUBE			0
//#endif

#define MODELINDEX_DEFAULTWEAPON	24 // weapon index -1

/*
// Weapon Info dependent defines (Not defined in the engine)
// weapons start at INVENTORY_WEAPON_START
#define INVENTORY_FISTS				45
#define INVENTORY_KATANAS			46
#define INVENTORY_DAISHO			47
#define INVENTORY_SAIS				48
#define INVENTORY_NUNCHUCKS			49
#define INVENTORY_HAMMER			50
#define INVENTORY_AXE				51
#define INVENTORY_LONGSWORD			52
#define INVENTORY_BAT				53
#define INVENTORY_BO				54
#define INVENTORY_BAMBOO			55
#define INVENTORY_GUN				56
#define INVENTORY_ELECTRIC_LAUNCHER	57
#define INVENTORY_HOMING_LAUNCHER	58
#define INVENTORY_ROCKET_LAUNCHER	59
#define INVENTORY_GRAPPLING_HOOK	59

// ammo start at INVENTORY_AMMO_START
#define INVENTORY_AMMOGUN			76
#define INVENTORY_AMMOELECTRIC		77
#define INVENTORY_AMMOHOMING		78
#define INVENTORY_AMMOROCKET		79
*/

