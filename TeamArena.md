Additions from Quake III: Team Arena, the plan is to remove and integrate all parts of the MISSIONPACK define into base Turtle Arena.

## Supported and/or used ##
  * One Flag CTF gametype: Take the center flag to the other team's base.
  * Overload gametype: Defend object at your base and attack other teams base object.

  * Persistant Powerups (For Team gametypes)
    * Scout (Incressed speed, and no armor? (armor is removed in Turtle Arena) ) (speed is 1.5, haste speed is 1.3)
      * ZTM: NOTE: I cut start health and pickup health in half.
    * Guard (Incressed health, and Health regen)           (2x health, PW\_BATTLESUIT is .5 damage receive)
      * ZTM: NOTE: I disabled Health regen (Seemed overpowered) and give 2x health from pickup items.
    * Doubler (Player weapons do double damage.)           (Like Q3 PW\_QUAD, as I set it to 2x damage)
    * Ammo Regen (Player ammo/shuriken counts slowly increment)

  * Chaingun (Supported)
  * Nailgun (Supported)
  * Prox Launcher (Support everything but sticking mines to players)

  * Voice chats for bots and humans
  * Per-team skins for players (Supported, but not currently used)

  * Portal holdable item (Not used in final version of Team Arena)
    * ZTM: I did some fixing to the code and added models and an icon, it is usable.

## Disabled/unused Code ##
  * Scriptable UI/HUD
    * (UI only) Teams: 5 bots, team-name, and logo-gfxs.
  * Harvester --Disabled (!defined MISSIONPACK\_HARVESTER)
    * kill people, then a skull (of the attacker's team color) will spawn at the center object, then attacker's team must take the skull to the other team's base, if killee's team touch it (object that spawned at center), it is removed.)
  * Kamikaze holdable item (Disabled)
  * Invul holdable item (Disabled)
  * Prox Launcher (Sticking mines to players is unsupported)