**Turtle Arena "to do"s.** This is a list of ideas (which should be moved to the issue tracker). For more "to do"s see the [road map](http://code.google.com/p/turtlearena/wiki/RoadMap) and [issue tracker](http://code.google.com/p/turtlearena/issues).

# engine #
## Weapons (and attacking) ##
  * TODO: Add attack movement flags? Stop, cut speed in half, double speed, turn right?, etc.

  * TODO: Remove code for tag\_hand\_primary/secondary and just use tag\_weapon/flag ?
    * Logic-wise `tag_hand_*` make more sense, but I still support tag\_weapon/flag (My players use tag\_weapon/flag ...)

  * TODO: Add PD\_CUSTOM projectile death type
    * pdModel (string), pdShader (string), pdSprite (bool), pdDuration (int), pdLight (int) (vec3), pdExpBase (int), pdExpAdd (int), pdExpOnPlayer (bool)
    * pdExpOnPlayer would set whether or not to explode on players in CG\_MissileHitPlayer

  * TODO: Add PT\_CUSTOM projectile trail type.

  * TODO: Per-weapon run animations (Support `LEGS_*`, `BOTH_*`, and `TORSO_*`)
    * Idea for supporting `TORSO_*`
      * PM\_ContinueAnim(run);
      * If (!(animations[run](run.md).prefixType & AP\_LEGS)) PM\_ContinueAnim(LEGS\_RUN);

## Teamplay ##
  * TODO: Per-team flag skins? (Team icon is on flag skin)
    * Would a shader remap work?

  * TODO: Have a extra team. (Currently needed for missionpack ui, though I could probably fix it so it isn't needed.)

## New Entities ##
  * TODO: Add misc\_bubbles entity.

  * TODO: Add misc\_dripper entity.

## Bots ##
  * TODO: Support bots using func\_use (use via holdable item button)

## Entities ##
  * TODO: Improve misc object physics. (Use code from Q3Rally? bg\_physics.c/h)

## NiGHTS mode ##
  * STARTED (NIGHTSMODE): NiGHTS into Dreams mode.

  * TODO: Use different model for player when in NiGHTS mode.

  * TODO: NiGHTS HUD mode, check for nights\_start to see if level is a NiGHTS level to use NiGHTS HUD.

  * TODO: When player is in bonus time, change blue sphere to golden (change model/skin and icon).

  * Data: TODO: Add models for star item.

  * TODO: Have NiGHTS-like link score plums? or just use floating numbers like quake3?
    * Supported by game, just needs cgame code and graphics.

## Misc ##
  * TODO: Add optional run animations for torso.

  * STARTED (TA\_PATHSYS): Finish new path system "g\_paths.c" for NPCs
  * TODO: Support path\_axis (orbit around ent), base on TR\_SINE?

  * TODO (TA\_PATHSYS): Modify G\_SetupPath to allow starting at either end and in the middle of any path.
    * (If there is a super smash bros. level, we need to be able to spawn players in the middle of path.)

  * TODO: Support models/players/NAME/animsounds.cfg used in STV:Elite Force (SP) (Also used in Jedi Knight 2) ?
    * Started see bg\_sound\_t and Sounds\_Parse; it is similar to animsounds, but different.
    * It currently doesn't load sounds and is for NPC and misc objects only.

  * TODO: Support focusing on 4 players with one camera, for a local 4 player game (splitscreen would be optional).

  * TODO: Improve camera system.
    * DONE: Controls for moving chase cam (left, right, up, down)
    * (NiGHTS: Journey of Dreams) Move camera if holding 'z' and move analog stick, can't look around while moving
    * BETTER(not done?): When moving the camera around the player, focus on the player. (Like in NiGHTS: Journey of Dreams)

  * TODO: STARTED(TURTLEARENA // LOCKON): When attacking a player (or NPC) focus on them?  (Like LoZ:TP 'z' target lock-on)

  * TODO: Merge Quake3 bot chat and Team Arena voice chat systems?
    * base/botfiles/bots/raph\_t.c hold the "Q3 bot chats" for non-teamplay (Team-only Team Arena players have dummy files)
    * base/botfiles/teamplay.h holds the "Q3 bot chats" for teamplay
    * base/scripts/male1.voice Team Arena added voice chats that are only used in teamplay (Some are usable by both humans and bots)
    * Have everywhere that starts a "Q3 bot chat" use voice chats instead?
      * (Unless it uses vars and can't be a voice chat, like "whois")
    * TeamPlayIsOn() is used to check if in teamplay for which chats to use and stuff
    * (Turtle Arena allows 64 voice chat files, so we could allow bots to have custom chats)

  * TODO: Allow voicechat filename to be set in animations.cfg

  * TODO: Add support for using a single model for players.
    * Add support for animation blending for IQM.
  * TODO-after-above: Re-rig and reanimate players in Blender for single IQM? (and/or add IQM export to Maverick Model 3D?)

  * TODO: Tell which clients are splitscreen players
    * Special icon on HUD?
    * Automaticly name splitscreen players (mainClientName[spilt#](spilt#.md)) and don't allow custom names?

  * Splitview mode for viewing multiple players online
    * Need someway to tell server which clients we want in splitview.
      * Probably use a command, list clientNums, client names, or teams. (Like "splitview 1 john blue" for three splitview clients)
        * If teams are supported add a command for changing to next team member? Or automaticly cycle through team members?

## Objects ##
Most of the mapobjects should be damageable.

  * TODO: Make more map objects
    * Clump of tall grass (Like in TMNT2: Battle Nexus (GC) level 2 and LoZ:TP)
    * Animated flower bud
    * Trees, bushes, flowers, tall-grasses
    * Traffic barrel, Traffic barricade (single slat and possibly others)
    * Fire hydrant, street light
    * street-signs (one-way, 3rd street, Stop, Yield, people crossing, speed limit)
    * Torch Flame, wall torches, etc
    * Lamps and other light things

## Testing! ##
  * TODO: Test/use the "noise" and "random" waveforms that I copied from IOSTVEF.

  * STARTED (TA\_HOLDABLE): Make shurikens/holdable-projectiles general like weapons groups.
    * (So they can be totally configured from .weap/.item, and add new ones)

## Don't do? (Dead Ideas?) ##
  * TODO: Feet tags, for attack checking? And/or for attaching hoverboard to player?

  * TODO: Have Announcer use "voice chats" so that the text is printed as well as playing a sound.
    * TODO: Add option to disable Announcer voice and/or text
    * TODO: Have Announcer text shown in a comic-style naration box
  * TODO: Allow multiple Announcers? Have a cg\_announcer cvar { "none", "male", "female", etc } to control voice chat file.

  * TODO-after-adding-ET\_WEAPON: "Weapon Surface flags" so metal rubbing on metal makes sparks and stuff?
    * Guns too? (Running next to a wall and gun is touching wall make sparks and wall marks O.O; )

  * TODO: Throw shurikens with secondary hand, so can't with CTF flag? Have only one attack animation.
  * TODO-after-above: Must put secondary weapon away to throw shuriken, then get it back put?
    * So Don can throw shurikens sooner than Leo/Raph/Mike?

  * TODO: Per-triangle collision for player damage, use lowest LOD model or have a model special for collision?
    * Using a special model for collision we could use group names to set what part of player is hit,
    * so shell takes less damage, banndana tails takes no damage, etc. We could say where a player got hit like Smokin' Guns?...

# Other #
  * TODO: Make some sort of manual to tell how to play, control the player (using default controls). Have it be simple.

  * TODO: Portable (zip): Add a note (either on the website or in the readme) that removing the "settings/portable" file will make it use the normal user app data directory, i.e. $HOME/.turtlearena

# Data #
  * TODO: Have a intro video (video/intro.roq or ogm) before the menu.

  * TODO: Have shurikens in all multi-player maps? (Having them be part of standard gameplay?)

  * TODO: Get free scenery objects from the Internet. Make most damageable.
    * http://virtualworlds.wikia.com