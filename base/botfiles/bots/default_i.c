/*
===========================================================================
Copyright (C) 2006 Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com))

This file is part of Open Arena and is based upon Mr. Elusive's fuzzy logic
system found in Quake 3 Arena.

Open Arena is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Open Arena is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// Turtle Man: Modified for TMNT Arena.

#include "inv.h"

//health
#define FPH				2

//melee weapons
//katanas
#define KTW				200
//daisho
#define DAW				190
//sais
#define SIW				180
//nunchuks
#define NKW				170
//hammer
#define HRW				100
//axe
#define AXW				100
//long-sword
#define LSW				110
//bat
#define BTW				110
//bo
#define BOW				160
//bamboo-bo
#define BBW				160
//guns
//gun
#define GNW				150
//electric launcher
#define ELW				250
//rocket launcher
#define RLW				350
//homing-rocket launcher
#define HLW				400
//grappling hook
#define GHW				90

//holdable items
//teleport
#define TELW				190
//medkit
#define MEDW				50
//kamikaze
#define KAMW				200
//portal
#define PORW				300
//shuriken pack
#define SURW				150
//electric shuriken pack
#define SUREW				160
//fire shuriken pack
#define SURFW				160
//laser shuriken pack
#define SURLW				160

//individual powerups
//quad-damage
#define QW					500
//envirosuit
#define ENVW				50
//haste
#define HAW					50
//invisibility
#define INW					400
//regeneration
#define REGW				50
//flight
#define FLW					40
//invulnerability
#define IBW					150

//scout
#define SCW					100
//guard
#define GUW					200
//doubler
#define DUBW				100
//ammo-regen
#define AMRW				400

//red-cube
#define REDCW				200
//blue-cube
#define BLCW				200

//ctf flag weight
#define FGW				400

//
#include "fuzi.c"
