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

// Turtle Man: Changed for TMNT weapons.
// NOTE: If TMNTWEAPSYS is defined this file is not used.
//         Instead game uses data from "scripts/weaponinfo.txt" (and *.weap files)

#include "inv.h"

#define IMPACT_DAMAGE			1		//straight impact damage
#define SPLASH_DAMAGE			2		//splash damage

// Dummy -- Default weapon

projectileinfo
{
name				"defaultdamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Default Weapon"
number				0
projectile			"defaultdamage"
numprojectiles		1
speed				0
}

// Melee weapons

projectileinfo
{
name				"katanadamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Kantanas"
number				WEAPONINDEX_KATANAS
projectile			"katanadamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"daishodamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Daisho"
number				WEAPONINDEX_DAISHO
projectile			"daishodamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"saidamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Sais"
number				WEAPONINDEX_SAIS
projectile			"saidamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"nunchukdamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Nunchuks"
number				WEAPONINDEX_NUNCHUKS
projectile			"nunchukdamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"hammerdamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Hammer"
number				WEAPONINDEX_HAMMER
projectile			"hammerdamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"axedamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Axe"
number				WEAPONINDEX_AXE
projectile			"axedamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"longsworddamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Long Sword"
number				WEAPONINDEX_LONGSWORD
projectile			"longsworddamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"batdamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Baseball bat"
number				WEAPONINDEX_BAT
projectile			"batdamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"bodamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Bo"
number				WEAPONINDEX_BO
projectile			"bodamage"
numprojectiles			1
speed				0
}

projectileinfo
{
name				"bamboodamage"
damage				10
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Bamboo Bo"
number				WEAPONINDEX_BAMBOOBO
projectile			"bamboodamage"
numprojectiles			1
speed				0
}

// Guns

projectileinfo
{
name				"bullet"
damage				20
radius				20
damagetype			IMPACT_DAMAGE
}

weaponinfo
{
name				"Gun"
number				WEAPONINDEX_GUN
projectile			"bullet"
numprojectiles			1
speed				2000
}

projectileinfo
{
name				"electric"
damage				20
radius				20
damagetype			$evalint(IMPACT_DAMAGE|SPLASH_DAMAGE)
}

weaponinfo
{
name				"Electric Launcher"
number				WEAPONINDEX_ELECTRIC_LAUNCHER
projectile			"electric"
numprojectiles			1
speed				2000
}

projectileinfo
{
name				"rocket"
damage				50
radius				120
damagetype			$evalint(IMPACT_DAMAGE|SPLASH_DAMAGE)
}

weaponinfo
{
name				"Rocket Launcher"
number				WEAPONINDEX_ROCKET_LAUNCHER
projectile			"rocket"
numprojectiles			1
speed				900
}

projectileinfo
{
name				"homingrocket"
damage				100
radius				120
damagetype			$evalint(IMPACT_DAMAGE|SPLASH_DAMAGE)
}

weaponinfo
{
name				"Homing Launcher"
number				WEAPONINDEX_HOMING_LAUNCHER
projectile			"homingrocket"
numprojectiles			1
speed				900
}

