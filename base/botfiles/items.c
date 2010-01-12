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

#include "inv.h"

// Turtle Man: NOTE: type and index seem to be unused.

#define NOITEM		0
#define AMMOI		1
#define WEAPONI		2
#define HEALTHI		3
#define ARMORI		4
#define POWERI		5
#define FLAGI		6
#define ROAMI		7

// respawntime
#define	RESPAWN_ARMOR		25
#define	RESPAWN_HEALTH		35
#define	RESPAWN_AMMO		40
//#ifdef TMNTHOLDABLE
#define RESPAWN_HOLDABLE	35
//#else
//#define RESPAWN_HOLDABLE	60
//#endif
#define	RESPAWN_MEGAHEALTH	35//120
#define	RESPAWN_POWERUP		120

/*
iteminfo "holdable_teleporter"
{
name		"Personal Teleporter"
model		"models/powerups/holdable/teleporter.md3"
modelindex	MODELINDEX_TELEPORTER
type		POWERI
index		INVENTORY_TELEPORTER
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}
*/

iteminfo "holdable_medkit"
{
name		"Medkit"
model		"models/powerups/holdable/medkit.md3"
modelindex	MODELINDEX_MEDKIT
type		HEALTHI
index		INVENTORY_MEDKIT
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

/*
iteminfo "holdable_kamikaze"
{
name		"Kamikaze"
model		"models/powerups/kamikazi.md3"
modelindex	MODELINDEX_KAMIKAZE
type		POWERI
index		INVENTORY_KAMIKAZE
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "holdable_invulnerability"
{
name		"Invulnerability"
model		"models/powerups/holdable/invulnerability.md3"
modelindex	MODELINDEX_INVULNERABILITY
type		POWERI
index		INVENTORY_PORTAL
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}
*/

iteminfo "holdable_portal"
{
name		"Portal"
model		"models/powerups/holdable/teleporter.md3"
modelindex	MODELINDEX_PORTAL
type		POWERI
index		INVENTORY_PORTAL
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "holdable_shuriken"
{
name		"Shurikens"
model		"models/powerups/holdable/shuriken.md3"
modelindex	MODELINDEX_SHURIKEN
type		POWERI
index		INVENTORY_SHURIKEN
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "holdable_shurikenelectric"
{
name		"Electric Shurikens"
model		"models/powerups/holdable/shurikenelectric.md3"
modelindex	MODELINDEX_ELECTRICSHURIKEN
type		POWERI
index		INVENTORY_ELECTRICSHURIKEN
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "holdable_shurikenfire"
{
name		"Fire Shurikens"
model		"models/powerups/holdable/shurikenfire.md3"
modelindex	MODELINDEX_FIRESHURIKEN
type		POWERI
index		INVENTORY_FIRESHURIKEN
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "holdable_shurikenlaser"
{
name		"Laser Shurikens"
model		"models/powerups/holdable/shurikenlaser.md3"
modelindex	MODELINDEX_LASERSHURIKEN
type		POWERI
index		INVENTORY_LASERSHURIKEN
respawntime	RESPAWN_HOLDABLE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_health_small"
{
name		"5% Health"
model		"models/powerups/health/small.md3"
modelindex	MODELINDEX_HEALTHSMALL
type		HEALTHI
index		INVENTORY_HEALTH
respawntime	RESPAWN_HEALTH
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_health"
{
name		"25% Health"
model		"models/powerups/health/medium.md3"
modelindex	MODELINDEX_HEALTH
type		HEALTHI
index		INVENTORY_HEALTH
respawntime	RESPAWN_HEALTH
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_health_large"
{
name		"50% Health"
model		"models/powerups/health/large.md3"
modelindex	MODELINDEX_HEALTHLARGE
type		HEALTHI
index		INVENTORY_HEALTH
respawntime	RESPAWN_HEALTH
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_health_mega"
{
name		"100% Health"
model		"models/powerups/health/mega.md3"
modelindex	MODELINDEX_HEALTHMEGA
type		HEALTHI
index		INVENTORY_HEALTH
respawntime	RESPAWN_MEGAHEALTH
mins		{-13,-13,-13}
maxs		{13,13,13}
}

// TMNTWEAPSYS uses weaponinfo.txt to setup the weapon items.
// (Weapons could still be here, but they would over-ride weaponinfo.txt items)
/*
//weapon_default?

iteminfo "weapon_katanas"
{
name		"Katanas"
model		"models/weapons2/katana/katana_pickup.md3"
modelindex	MODELINDEX_KATANAS
type		WEAPONI
index		INVENTORY_KATANAS
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_daisho"
{
name		"Daisho"
model		"models/weapons2/daisho/daisho_pickup.md3"
modelindex	MODELINDEX_DAISHO
type		WEAPONI
index		INVENTORY_DAISHO
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_sais"
{
name		"Sais"
model		"models/weapons2/sai/sai_pickup.md3"
modelindex	MODELINDEX_SAIS
type		WEAPONI
index		INVENTORY_SAIS
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_nunchuks"
{
name		"Nunchuks"
model		"models/weapons2/nunchuk/nunchuk_pickup.md3"
modelindex	MODELINDEX_NUNCHUKS
type		WEAPONI
index		INVENTORY_NUNCHUKS
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_hammer"
{
name		"Hammer"
model		"models/weapons2/hammer/hammer.md3"
modelindex	MODELINDEX_HAMMER
type		WEAPONI
index		INVENTORY_HAMMER
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_axe"
{
name		"Axe"
model		"models/weapons2/axe/axe.md3"
modelindex	MODELINDEX_AXE
type		WEAPONI
index		INVENTORY_AXE
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_longsword"
{
name		"Long Sword"
model		"models/weapons2/longsword/longsword.md3"
modelindex	MODELINDEX_LONGSWORD
type		WEAPONI
index		INVENTORY_LONGSWORD
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_bat"
{
name		"Baseball Bat"
model		"models/weapons2/bat/bat.md3"
modelindex	MODELINDEX_BAT
type		WEAPONI
index		INVENTORY_BAT
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_bo"
{
name		"Bo"
model		"models/weapons2/bo/bo.md3"
modelindex	MODELINDEX_BO
type		WEAPONI
index		INVENTORY_BO
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_bamboobo"
{
name		"Bamboo"
model		"models/weapons2/bamboobo/bamboobo.md3"
modelindex	MODELINDEX_BAMBOOBO
type		WEAPONI
index		INVENTORY_BAMBOOBO
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_gun"
{
name		"Gun"
model		"models/weapons2/gun/gun.md3"
modelindex	MODELINDEX_GUN
type		WEAPONI
index		INVENTORY_GUN
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_electriclauncher"
{
name		"Electric Launcher"
model		"models/weapons2/electricl/electricl.md3"
modelindex	MODELINDEX_ELECTRICLAUNCHER
type		WEAPONI
index		INVENTORY_ELECTRIC_LAUNCHER
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_rocketlauncher"
{
name		"Rocket Launcher"
model		"models/weapons2/rocketl/rocketl.md3"
modelindex	MODELINDEX_ROCKETLAUNCHER
type		WEAPONI
index		INVENTORY_ROCKET_LAUNCHER
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_hominglauncher"
{
name		"Homing-rocket Launcher"
model		"models/weapons2/homingl/homingl.md3"
modelindex	MODELINDEX_HOMINGLAUNCHER
type		WEAPONI
index		INVENTORY_HOMING_LAUNCHER
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "weapon_grapplinghook"
{
name		"Grappling Hook"
model		"models/weapons2/grapple/grapple.md3"
modelindex	MODELINDEX_GRAPPLINGHOOK
type		WEAPONI
index		INVENTORY_GRAPPLING_HOOK
respawntime	30
mins		{-13,-13,-13}
maxs		{13,13,13}
}
*/

iteminfo "item_strength"
{
name		"Increased Strength"
model		"models/powerups/instant/strength.md3"
modelindex	MODELINDEX_QUAD
type		POWERI
index		INVENTORY_QUAD
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_defense"
{
name		"Increased Defense"
model		"models/powerups/instant/defense.md3"
modelindex	MODELINDEX_ENVIRONMENTSUIT
type		POWERI
index		INVENTORY_ENVIRONMENTSUIT
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_speed"
{
name		"Increased Speed"
model		"models/powerups/instant/speed.md3"
modelindex	MODELINDEX_HASTE
type		POWERI
index		INVENTORY_HASTE
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_invisibility"
{
name		"Invisibility"
model		"models/powerups/instant/invis.md3"
modelindex	MODELINDEX_INVISIBILITY
type		POWERI
index		INVENTORY_INVISIBILITY
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_regen"
{
name		"Regeneration"
model		"models/powerups/instant/regen.md3"
modelindex	MODELINDEX_REGEN
type		POWERI
index		INVENTORY_REGEN
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_flight"
{
name		"Flight"
model		"models/powerups/instant/flight.md3"
modelindex	MODELINDEX_FLIGHT
type		POWERI
index		INVENTORY_FLIGHT
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_invul"
{
name		"Invulnerability"
model		"models/powerups/instant/invul.md3"
modelindex	MODELINDEX_INVUL
type		POWERI
index		INVENTORY_INVUL
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_scout"
{
name		"Scout"
model		"models/powerups/scout.md3"
modelindex	MODELINDEX_SCOUT
type		POWERI
index		INVENTORY_SCOUT
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_guard"
{
name		"Guard"
model		"models/powerups/guard.md3"
modelindex	MODELINDEX_GUARD
type		POWERI
index		INVENTORY_GUARD
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_doubler"
{
name		"Doubler"
model		"models/powerups/doubler.md3"
modelindex	MODELINDEX_DOUBLER
type		POWERI
index		INVENTORY_DOUBLER
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "item_ammoregen"
{
name		"Ammo Regen"
model		"models/powerups/ammo.md3"
modelindex	MODELINDEX_AMMOREGEN
type		POWERI
index		INVENTORY_AMMOREGEN
respawntime	RESPAWN_POWERUP
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "team_CTF_redflag"
{
name		"Red Flag"
model		"models/flags/r_flag.md3"
modelindex	MODELINDEX_REDFLAG
type		FLAGI
index		INVENTORY_REDFLAG
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "team_CTF_blueflag"
{
name		"Blue Flag"
model		"models/flags/b_flag.md3"
modelindex	MODELINDEX_BLUEFLAG
type		FLAGI
index		INVENTORY_BLUEFLAG
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "team_CTF_neutralflag"
{
name		"Neutral Flag"
model		"models/flags/n_flag.md3"
modelindex	MODELINDEX_NEUTRALFLAG
type		FLAGI
index		INVENTORY_NEUTRALFLAG
mins		{-13,-13,-13}
maxs		{13,13,13}
} 

iteminfo "team_redobelisk"
{
name		"Red Obelisk"
model		"models/powerups/obelisk/obelisk.md3"
modelindex	0
type		NOITEM
index		0
mins		{-14, -14, 1}
maxs		{14, 14, 87}
} 

iteminfo "team_blueobelisk"
{
name		"Blue Obelisk"
model		"models/powerups/obelisk/obelisk.md3"
modelindex	0
type		NOITEM
index		0
mins		{-14, -14, 1}
maxs		{14, 14, 87}
} 

iteminfo "team_neutralobelisk"
{
name		"Neutral Obelisk"
model		"models/powerups/obelisk/obelisk.md3"
modelindex	0
type		NOITEM
index		0
mins		{-14, -14, 1}
maxs		{14, 14, 87}
} 

/*
// For MISSIONPACK Harvester gametype (Turtle Man: Removed from TMNT Arena. #ifdef MISSIONPACK_HARVESTER)
iteminfo "item_redcube"
{
name		"Red Cube"
model		"models/powerups/orb/r_orb.md3"
modelindex	MODELINDEX_REDCUBE
type		FLAGI
index		INVENTORY_REDCUBE
mins		{-13,-13,-13}
maxs		{13,13,13}
}

iteminfo "item_bluecube"
{
name		"Blue Cube"
model		"models/powerups/orb/b_orb.md3"
modelindex	MODELINDEX_BLUECUBE
type		FLAGI
index		INVENTORY_BLUECUBE
mins		{-13,-13,-13}
maxs		{13,13,13}
}
*/

iteminfo "item_botroam"
{
name		"Bot Roam Goal"
model		""
modelindex	0
type		ROAMI
index		0
mins		{-13,-13,-13}
maxs		{13,13,13}
}

