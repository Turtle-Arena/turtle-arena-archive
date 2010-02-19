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
#define INVENTORY_HEALTH			55

//#ifdef TMNTWEAPSYS_EX
// The most weapons a player can have is 2!
//  (well they do have 3 when switching pickup for pickup and have a default weapon)
// Default weapon for the player,
// If INVENTORY_WAEPON is different, the default can be used by dropping current weapon.
#define INVENTORY_DEFAULTWEAPON		56
// Weapon the player is using
#define INVENTORY_WEAPON			57

#define INVENTORY_DEFAULTAMMO 58 // Ammo for default weapon.
#define INVENTORY_AMMO 		59 // Ammo for current weapon,
//#endif

//#ifdef TMNTWEAPONS
// weapons, currently there is a max of 16 - which I am over...
//#define INVENTORY_WP_NONE			0
#define INVENTORY_FISTS				1
#define INVENTORY_KATANAS			2
#define INVENTORY_DAISHO			3 // Usagi's weapons
#define INVENTORY_SAIS				4
#define INVENTORY_NUNCHUCKS			5
#define INVENTORY_HAMMER			6
#define INVENTORY_AXE				7
#define INVENTORY_LONGSWORD			8
#define INVENTORY_BAT				9
#define INVENTORY_BO				10
#define INVENTORY_BAMBOOBO			11
#define INVENTORY_GUN				12
#define INVENTORY_ELECTRIC_LAUNCHER	13
#define INVENTORY_HOMING_LAUNCHER	14
#define INVENTORY_ROCKET_LAUNCHER	15
#define INVENTORY_GRAPPLING_HOOK	16

#define INVENTORY_AMMOGUN			17
#define INVENTORY_AMMOELECTRIC		18
#define INVENTORY_AMMOHOMING		19
#define INVENTORY_AMMOROCKET		20
//#endif // TMNTWEAPONS

// holdable items, currently there is a max of 16
// Q3 holdable defines.
#define INVENTORY_TELEPORTER		21	// removed
#define INVENTORY_MEDKIT			22
//MISSIONPACK
#define INVENTORY_KAMIKAZE			23	// removed
#define INVENTORY_PORTAL			24
#define INVENTORY_INVULNERABILITY	25	// removed

// TMNT holdable defines.
// HI_SHURIKEN, HI_ELECTRICSHURIKEN, HI_FIRESHURIKEN, HI_LASERSHURIKEN
#define INVENTORY_SHURIKEN			26
#define INVENTORY_ELECTRICSHURIKEN	27
#define INVENTORY_FIRESHURIKEN		28
#define INVENTORY_LASERSHURIKEN		29
//#define INVENTORY_HOLDABLE9		30
//#define INVENTORY_HOLDABLE10		31
//#define INVENTORY_HOLDABLE11		32
//#define INVENTORY_HOLDABLE12		33
//#define INVENTORY_HOLDABLE13		34
//#define INVENTORY_HOLDABLE14		35
//#define INVENTORY_HOLDABLE15		36

// powerups, currently there is a max of 16
// Q3 powerup defines.
#define INVENTORY_QUAD				37
#define INVENTORY_ENVIRONMENTSUIT	38
#define INVENTORY_HASTE				39
#define INVENTORY_INVISIBILITY		40
#define INVENTORY_FLIGHT			41

// TMNT invulerability
#define INVENTORY_INVUL				42

#define INVENTORY_REDFLAG			43
#define INVENTORY_BLUEFLAG			44
#define INVENTORY_NEUTRALFLAG		45

// persistant powers MISSIONPACK defines
#define INVENTORY_SCOUT				46
#define INVENTORY_GUARD				47
#define INVENTORY_DOUBLER			48
#define INVENTORY_AMMOREGEN			49
//#define INVENTORY_POWERUP13			50
//#define INVENTORY_POWERUP14			51
//#define INVENTORY_POWERUP15			52

// other MISSIONPACK
#define INVENTORY_REDCUBE			53
#define INVENTORY_BLUECUBE			54

// 55-59 are at top of file.
#define INVENTORY_PERSISTANT_POWER 60

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

#define MODELINDEX_FISTS			5
#define MODELINDEX_KATANAS			6
#define MODELINDEX_DAISHO			7
#define MODELINDEX_SAIS				8
#define MODELINDEX_NUNCHUCKS		9
#define MODELINDEX_HAMMER			10
#define MODELINDEX_AXE				11
#define MODELINDEX_LONGSWORD		12
#define MODELINDEX_BAT				13
#define MODELINDEX_BO				14
#define MODELINDEX_BAMBOO			15
#define MODELINDEX_GUN				16
#define MODELINDEX_ELECTRICLAUNCHER	17
#define MODELINDEX_ROCKETLAUNCHER	18
#define MODELINDEX_HOMINGLAUNCHER	19
#define MODELINDEX_GRAPPLINGHOOK	20

#define MODELINDEX_TELEPORTER		0	// removed
#define MODELINDEX_MEDKIT			21
#define MODELINDEX_SHURIKEN			22
#define MODELINDEX_ELECTRICSHURIKEN	23
#define MODELINDEX_FIRESHURIKEN		24
#define MODELINDEX_LASERSHURIKEN	25

#define MODELINDEX_QUAD				26
#define MODELINDEX_ENVIRONMENTSUIT	27
#define MODELINDEX_HASTE			28
#define MODELINDEX_INVISIBILITY		29
#define MODELINDEX_REGEN			0	// removed
#define MODELINDEX_FLIGHT			30

// TMNT power
#define MODELINDEX_INVUL			31

#define MODELINDEX_REDFLAG			32
#define MODELINDEX_BLUEFLAG			33

// mission pack only defines

#define MODELINDEX_KAMIKAZE			0	// removed
#define MODELINDEX_PORTAL			34
#define MODELINDEX_INVULNERABILITY	0	// removed

#define MODELINDEX_SCOUT			35
#define MODELINDEX_GUARD			36
#define MODELINDEX_DOUBLER			37
#define MODELINDEX_AMMOREGEN		38

#define MODELINDEX_NEUTRALFLAG		39
//#ifdef MISSIONPACK_HARVESTER
//#define MODELINDEX_REDCUBE			0
//#define MODELINDEX_BLUECUBE			0
//#endif

//#define MODELINDEX_NAILGUN			0
//#define MODELINDEX_PROXLAUNCHER		0
//#define MODELINDEX_CHAINGUN			0

#define MODELINDEX_DEFAULTWEAPON	40 // weapon index -1

