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

// ZTM: Modified for Turtle Arena.

#define AMMO				20
#define HEALTH				20
#define POWERUP				20
#define WEAPON				20

//breaks compatibility with the existing q3a bots, sorry.
#define FP(x)	(x < 0 ? 1 : x)
#define AS(x)	balance($evalfloat(FP(x)), $evalfloat(FP(x/AMMO)), $evalfloat(FP(x*AMMO)))
#define HS(x)	balance($evalfloat(FP(FPH/x)), $evalfloat(FP(FPH/x-HEALTH)), $evalfloat(FP(FPH/x+HEALTH)))
#define PS(x)	balance($evalfloat(FP(x)), $evalfloat(FP(x/POWERUP)), $evalfloat(FP(x*POWERUP)))
#define WS(x)	balance($evalfloat(FP(x)), $evalfloat(FP(x/WEAPON)), $evalfloat(FP(x*WEAPON)))


weight "item_health_small"
{
	switch(INVENTORY_HEALTH)
	{
	case 10: return HS(100);
	case 20: return HS(90);
	case 30: return HS(85);
	case 40: return HS(80);
	case 50: return HS(75);
	case 60: return HS(70);
	case 70: return HS(65);
	case 80: return HS(60);
	case 90: return HS(55);
	case 100: return HS(50);
	default: return 0;
	}
}

weight "item_health"
{
	switch(INVENTORY_HEALTH)
	{
	case 10: return HS(110);
	case 20: return HS(100);
	case 30: return HS(90);
	case 40: return HS(80);
	case 50: return HS(70);
	case 60: return HS(60);
	case 70: return HS(50);
	case 80: return HS(40);
	case 90: return HS(30);
	case 100: return HS(30);
	default: return 0;
	}
}

weight "item_health_large"
{
	switch(INVENTORY_HEALTH)
	{
	case 10: return HS(120);
	case 20: return HS(110);
	case 30: return HS(100);
	case 40: return HS(90);
	case 50: return HS(80);
	case 60: return HS(70);
	case 70: return HS(60);
	case 80: return HS(50);
	case 90: return HS(40);
	case 100: return HS(30);
	default: return 0;
	}
}

weight "item_health_mega"
{
	switch(INVENTORY_HEALTH)
	{
	case 10: return HS(150);
	case 20: return HS(120);
	case 30: return HS(110);
	case 40: return HS(100);
	case 50: return HS(90);
	case 60: return HS(90);
	case 70: return HS(80);
	case 80: return HS(80);
	case 90: return HS(70);
	case 100: return HS(60);
	default: return 0;
	}
}

weight "weapon_default"
{
	return 0;
}

// TMNTWEAPSYS sets up the weapon item weights.
/*
weight "weapon_katanas"
{
	switch(INVENTORY_KATANAS)
	{
	case 1: return WS(KTW);
	default: return 0;
	}
}

weight "weapon_daisho"
{
	switch(INVENTORY_DAISHO)
	{
	case 1: return WS(DAW);
	default: return 0;
	}
}

weight "weapon_sais"
{
	switch(INVENTORY_SAIS)
	{
	case 1: return WS(SIW);
	default: return 0;
	}
}

weight "weapon_nunchucks"
{
	switch(INVENTORY_NUNCHUCKS)
	{
	case 1: return WS(NKW);
	default: return 0;
	}
}

weight "weapon_hammer"
{
	switch(INVENTORY_HAMMER)
	{
	case 1: return WS(HRW);
	default: return 0;
	}
}

weight "weapon_axe"
{
	switch(INVENTORY_AXE)
	{
	case 1: return WS(AXW);
	default: return 0;
	}
}

weight "weapon_longsword"
{
	switch(INVENTORY_LONGSWORD)
	{
	case 1: return WS(LSW);
	default: return 0;
	}
}

weight "weapon_bat"
{
	switch(INVENTORY_BAT)
	{
	case 1: return WS(BTW);
	default: return 0;
	}
}

weight "weapon_bo"
{
	switch(INVENTORY_BO)
	{
	case 1: return WS(BOW);
	default: return 0;
	}
}

weight "weapon_bamboobo"
{
	switch(INVENTORY_BAMBOOBO)
	{
	case 1: return WS(BBW);
	default: return 0;
	}
}

weight "weapon_gun"
{
	switch(INVENTORY_GUN)
	{
	case 1: return WS(GNW);
	default: return 1;
	}
}

weight "weapon_electriclauncher"
{
	switch(INVENTORY_ELECTRIC_LAUNCHER)
	{
	case 1: return WS(ELW);
	default: return 1;
	}
}

weight "weapon_rocketlauncher"
{
	switch(INVENTORY_ROCKET_LAUNCHER)
	{
	case 1: return WS(RLW);
	default: return 1;
	}
}

weight "weapon_hominglauncher"
{
	switch(INVENTORY_HOMING_LAUNCHER)
	{
	case 1: return WS(HLW);
	default: return 1;
	}
}

weight "weapon_grapplinghook"
{
	switch(INVENTORY_GRAPPLING_HOOK)
	{
	case 1: return WS(GHW);
	default: return 0;
	}
}
*/

/* Removed from Turtle Arena
//teleport
#define TELW				190
weight "holdable_teleporter"
{
	switch(INVENTORY_TELEPORTER)
	{
	case 1: return PS(TELW);
	default: return 0;
	}
}
*/

weight "holdable_medkit"
{
	switch(INVENTORY_MEDKIT) // ZTM: BUGFIX?: Was INVENTORY_TELEPORTER
	{
	case 1: return PS(MEDW);
	default: return 0;
	}
}

/* Removed from Turtle Arena
//kamikaze
#define KAMW				200
weight "holdable_kamikaze"
{
	switch(INVENTORY_KAMIKAZE) // ZTM: BUGFIX?: Was INVENTORY_TELEPORTER
	{
	case 1: return PS(KAMW);
	default: return 0;
	}
}
*/

weight "holdable_portal"
{
	switch(INVENTORY_PORTAL) // ZTM: BUGFIX?: Was INVENTORY_TELEPORTER
	{
	case 1: return PS(PORW);
	default: return 0;
	}
}

/* Removed from Turtle Arena
weight "holdable_invulnerability"
{
	switch(INVENTORY_INVULNERABILITY) // ZTM: BUGFIX?: Was INVENTORY_TELEPORTER
	{
	case 1: return PS(IBW);
	default: return 0;
	}
}
*/

weight "holdable_shuriken"
{
	switch(INVENTORY_SHURIKEN)
	{
	case 1: return PS(SURW);
	default: return 1;
	}
}

weight "holdable_shurikenelectric"
{
	switch(INVENTORY_ELECTRICSHURIKEN)
	{
	case 1: return PS(SUREW);
	default: return 1;
	}
}

weight "holdable_shurikenfire"
{
	switch(INVENTORY_FIRESHURIKEN)
	{
	case 1: return PS(SURFW);
	default: return 1;
	}
}

weight "holdable_shurikenlaser"
{
	switch(INVENTORY_LASERSHURIKEN)
	{
	case 1: return PS(SURLW);
	default: return 1;
	}
}

weight "item_quad"
{
	return PS(QW);
}

weight "item_enviro"
{
	return PS(ENVW);
}

weight "item_haste"
{
	return PS(HAW);
}

weight "item_invisibility"
{
	return PS(INW);
}

weight "item_regen"
{
	return PS(REGW);
}

weight "item_flight"
{
	return PS(FLW);
}

//"item_invulnerability"
weight "item_invul"
{
	return PS(IBW);
}

weight "item_scout"
{
	//switch(INVENTORY_SCOUT)
	switch(INVENTORY_PERSISTANT_POWER)
	{
	case 1: return PS(SCW);
	default: return 0;
	}
}

weight "item_guard"
{
	//switch(INVENTORY_SCOUT)
	switch(INVENTORY_PERSISTANT_POWER)
	{
	case 1: return PS(GUW);
	default: return 0;
	}
}

weight "item_doubler"
{
	//switch(INVENTORY_SCOUT)
	switch(INVENTORY_PERSISTANT_POWER)
	{
	case 1: return PS(DUBW);
	default: return 0;
	}
}

weight "item_ammoregen"
{
	//switch(INVENTORY_SCOUT)
	switch(INVENTORY_PERSISTANT_POWER)
	{
	case 1: return PS(AMRW);
	default: return 0;
	}
}


//=============================================
// This is only used to pickup dropped CTF
// flags now. The logic in here makes no
// sense since the bot has specific CTF AI.
// yet without it the bots barely move towards
// the opposing flags due to low flag weights.
//=============================================

weight "team_CTF_redflag"
{
	switch(INVENTORY_REDFLAG)
	{
	case 1: return FGW;
	default: return 300;
	}
}

weight "team_CTF_blueflag"
{
	switch(INVENTORY_BLUEFLAG)
	{
	case 1: return FGW;
	default: return 300;
	}
}


weight "team_CTF_neutralflag"
{
	return FGW;
}

weight "item_botroam"
{
	return 1;
}

// For Team Arena Harvester gametype (Disabled in Turtle Arena)
weight "item_redcube"
{
	return 200;//REDCW;
}

weight "item_bluecube"
{
	return 200;//BLCW;
}

