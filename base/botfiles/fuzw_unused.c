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

// Changed for TMNT weapons, based on Open Arena's fuzw.c

// if (BG_WeapUseAmmo(wp)) {
//     return value;
// } else {
//     if (have wp)
//         return 0;
//     else
//         return value;
// }

weight "Default Weapon"
{
    return 0;
}

weight "Katanas"
{
	switch(INVENTORY_KATANAS)
	{
		case 1: return 0;
		default: return KTW;
	}
}

weight "Daisho"
{
	switch(INVENTORY_DAISHO)
	{
		case 1: return 0;
		default: return DAW;
	}
}

weight "Sais"
{
	switch(INVENTORY_SAIS)
	{
		case 1: return 0;
		default: return SIW;
	}
}

weight "Nunchuks"
{
	switch(INVENTORY_NUNCHUKS)
	{
		case 1: return 0;
		default: return NKW;
	}
}

weight "Hammer"
{
	switch(INVENTORY_HAMMER)
	{
		case 1: return 0;
		default: return HRW;
	}
}

weight "Axe"
{
	switch(INVENTORY_AXE)
	{
		case 1: return 0;
		default: return AXW;
	}
}

weight "Long Sword"
{
	switch(INVENTORY_LONGSWORD)
	{
		case 1: return 0;
		default: return LSW;
	}
}

weight "Baseball bat"
{
	switch(INVENTORY_BAT)
	{
		case 1: return 0;
		default: return BTW;
	}
}

weight "Bo"
{
	switch(INVENTORY_BO)
	{
		case 1: return 0;
		default: return BOW;
	}
}

weight "Bamboo Bo"
{
	switch(INVENTORY_BAMBOOBO)
	{
		case 1: return 0;
		default: return BBW;
	}
}

weight "Gun"
{
	//switch(INVENTORY_GUN)
	//{
		//case 1:
		//{
		    //switch(INVENTORY_AMMOGUN)
		    //{
		    //    case 1: return 0;
		    //    default: return GNW;
		    //}
		//}
		//default: return GNW;
	//}
	return GNW;
}

weight "Electric Launcher"
{
	//switch(INVENTORY_ELECTRIC_LAUNCHER)
	//{
		//case 1:
		//{
		    //switch(INVENTORY_AMMOELECTRIC)
		    //{
		    //    case 1: return 0;
		    //    default: return ELW;
		    //}
		//}
		//default: return ELW;
	//}
	return ELW;
}

weight "Homing Launcher"
{
	//switch(INVENTORY_HOMING_LAUNCHER)
	//{
		//case 1:
		//{
		    //switch(INVENTORY_AMMOHOMING)
		    //{
		    //    case 1: return 0;
		    //    default: return HLW;
		    //}
		//}
		//default: return HLW;
	//}
	return HLW;
}

weight "Rocket Launcher"
{
	//switch(INVENTORY_ROCKET_LAUNCHER)
	//{
		//case 1:
		//{
		    //switch(INVENTORY_AMMO)
		    //{
		    //    case 1: return 0;
		    //    default: return RLW;
		    //}
		//}
		//default: return RLW;
	//}
	return RLW;
}

weight "Grappling Hook"
{
	switch(INVENTORY_GRAPPLING_HOOK)
	{
		case 1: return 0;
		default: return GHW;
	}
}

