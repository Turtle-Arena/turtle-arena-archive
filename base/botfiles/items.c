/*
===========================================================================
Copyright (C) 2006 Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com))
Copyright (C) 2009-2011 Zack "ZTurtleMan" Middleton

This file is part of Turtle Arena and is based upon botfiles by
Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com)) from Open Arena which
are based on Mr. Elusive's fuzzy logic system found in Quake 3 Arena.

This file is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This file is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Turtle Arena; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "inv.h"

// ZTM: NOTE: type and index are unused.

#define NOITEM		0
#define AMMOI		1
#define WEAPONI		2
#define HEALTHI		3
#define SCOREI		4
//#define ARMORI		4
#define POWERI		5
#define FLAGI		6
#define ROAMI		7

// Turtle Arena loads iteminfo.txt and weaponinfo.txt in 'game' and gives itemInfos to server to setup the items.
// (itemInfos could still be here, they would over-ride the automaticly created items)

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

