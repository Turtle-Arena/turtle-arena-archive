Files that are missing or "TEMP:" if I haven't decide if its finished yet. List is incomplete.



# Sounds #
Note: Added a dummy sound at sound/feedback/hit.wav so unmodified Quake3 can start with only Turtle Arena sounds.

## Note: Copies ##
  * misc/menu1.ogg and misc/menu3.ogg are a louder-copy of player/footsteps/clank2.ogg

  * items/use\_nothing.ogg is a copy of misc/menu4.ogg

  * weapons/melee\_swing.ogg is a louder-version of weapons/change.ogg (For testing...)

  * player/raph/death2 and death3 are copies of death1
  * player/casey/death2 and death3 are copies of death1

## New sounds ##
Lock-on sounds like in LOZ:TP
  * TEMP: sound/misc/letterboxon
  * TEMP: sound/misc/letterboxoff

## Player Sounds ##
  * base/sound/player/leo/
  * base/sound/player/don/
  * TEMP: base/sound/player/raph/
  * base/sound/player/mike/

### List of Sounds ###
  * death1.wav
  * death2.wav
  * death3.wav
  * drown.wav
  * fall1.wav - hit ground
  * falling1.wav - bottom-less pit
  * gasp.wav
  * jump1.wav
  * pain100\_1.wav
  * pain75\_1.wav
  * pain50\_1.wav
  * pain25\_1.wav
  * taunt.wav

## Announcer / Feedback ##

  * TEMP: sound/feedback/1\_minute.wav - Announcer: "One minute remaining" ?
  * TEMP: sound/feedback/5\_minute.wav - Announcer: "Five minutes remaining" ?
  * TEMP: sound/feedback/sudden\_death.wav - Announcer: "Sudden Death."
  * TEMP: sound/feedback/1\_frag.wav - Announcer: "One knockout remaining" ?
  * TEMP: sound/feedback/2\_frags.wav - Announcer: "Two knockouts remaining" ?
  * TEMP: sound/feedback/3\_frags.wav - Announcer: "Three knockouts remaining" ?
  * TEMP: sound/feedback/three.wav - Announcer: "Three"
  * TEMP: sound/feedback/two.wav - Announcer: "Two"
  * TEMP: sound/feedback/one.wav - Announcer: "One"
  * TEMP: sound/feedback/fight.wav - Announcer: "Begin" // Should there be "Fight" and "Begin"? Non-battle gametypes would use "Begin".
  * TEMP: sound/feedback/prepare.wav - Announcer: "Prepare Your Selfs."
  * TEMP: sound/feedback/prepare\_team.wav - Announcer: "Prepare Your Team."

  * TEMP: sound/feedback/vote\_now.wav - Announcer: "Voting has begun."

  * TEMP: sound/feedback/vote\_passed.wav - Announcer: "Vote passed."
  * TEMP: sound/feedback/vote\_failed.wav - Announcer: "Vote failed."

  * TEMP: sound/feedback/takenlead.wav - Announcer: "You have taken the lead."
  * TEMP: sound/feedback/tiedlead.wav - Announcer: "You're tied for the lead."
  * TEMP: sound/feedback/lostlead.wav - Announcer: "You lost the lead."

  * TEMP: sound/feedback/assist.wav - Announcer: "Assist."
  * TEMP: sound/feedback/defense.wav - Announcer: "Defense."

### TEAM/CTF/1FCTF/Overload ###
  * TEMP: sound/feedback/redleads.wav - Announcer: "Red leads."
  * TEMP: sound/feedback/blueleads.wav - Announcer: "Blue leads."
  * TEMP: sound/feedback/teamstied.wav - Announcer: "Teams are tied."
  * sound/feedback/hit\_teammate.wav - sfx

  * TEMP: sound/teamplay/voc\_red\_scores.wav - Announcer: "Red scores."
  * TEMP: sound/teamplay/voc\_blue\_scores.wav - Announcer: "Blue scores."

  * sound/teamplay/flagcapture\_yourteam.wav - sfx
  * sound/teamplay/flagcapture\_opponent.wav - sfx

  * sound/teamplay/flagreturn\_yourteam.wav - sfx
  * sound/teamplay/flagreturn\_opponent.wav - sfx

  * sound/teamplay/flagtaken\_yourteam.wav - sfx 'alarm'
  * sound/teamplay/flagtaken\_opponent.wav - sfx 'different alarm'

### CTF ###
  * TEMP: sound/teamplay/voc\_red\_returned.wav - Announcer: "Red flag return."
  * TEMP: sound/teamplay/voc\_blue\_returned.wav - Announcer: "Blue flag return."
  * TEMP: sound/teamplay/voc\_enemy\_flag.wav - Announcer: "Thee enemy has your flag."
  * TEMP: sound/teamplay/voc\_team\_flag.wav - Announcer: "Your team has the enemy flag."

### 1 Flag CTF ###
  * TEMP: sound/teamplay/voc\_team\_1flag.wav - Announcer: "Your team has the flag."
  * TEMP: sound/teamplay/voc\_enemy\_1flag.wav - Announcer: "Thee enemy has the flag."

### CTF and 1FCTF ###
  * TEMP: sound/teamplay/voc\_you\_flag.wav - Announcer: "You have the flag!"

### Overload ###
  * TEMP: sound/teamplay/voc\_base\_attack.wav - Announcer: "Your base is under attack!"

## Sound Effects ##
  * TEMP:	cgs.media.tracerSound = trap\_S\_RegisterSound( "sound/weapons/machinegun/buletby1.wav", qfalse );
  * TEMP:	cgs.media.selectSound = trap\_S\_RegisterSound( "sound/weapons/change.wav", qfalse );

  * TEMP:	cgs.media.obeliskHitSound1 = trap\_S\_RegisterSound( "sound/items/obelisk\_hit\_01.wav", qfalse );
  * TEMP:	cgs.media.obeliskHitSound2 = trap\_S\_RegisterSound( "sound/items/obelisk\_hit\_02.wav", qfalse );
  * TEMP:	cgs.media.obeliskHitSound3 = trap\_S\_RegisterSound( "sound/items/obelisk\_hit\_03.wav", qfalse );
  * TEMP:	cgs.media.obeliskRespawnSound = trap\_S\_RegisterSound( "sound/items/obelisk\_respawn.wav", qfalse );

  * TEMP:	cgs.media.teleInSound = trap\_S\_RegisterSound( "sound/world/telein.wav", qfalse );
  * TEMP:	cgs.media.teleOutSound = trap\_S\_RegisterSound( "sound/world/teleout.wav", qfalse );

// ZTM: NOTE: noAmmoSound is currently unused. Use or move to base\_ioq3ztm?
  * TEMP:	cgs.media.noAmmoSound = trap\_S\_RegisterSound( "sound/weapons/noammo.wav", qfalse );

  * TEMP:	cgs.media.talkSound = trap\_S\_RegisterSound( "sound/player/talk.wav", qfalse );
  * TEMP:	cgs.media.landSound = trap\_S\_RegisterSound( "sound/player/land1.wav", qfalse);

  * cgs.media.watrInSound = trap\_S\_RegisterSound( "sound/player/watr\_in.wav", qfalse);
  * cgs.media.watrOutSound = trap\_S\_RegisterSound( "sound/player/watr\_out.wav", qfalse);
  * cgs.media.watrUnSound = trap\_S\_RegisterSound( "sound/player/watr\_un.wav", qfalse);


  * cgs.media.respawnSound = trap\_S\_RegisterSound( "sound/items/respawn1.wav", qfalse );
  * cgs.media.hitSound = trap\_S\_RegisterSound( "sound/feedback/hit.wav", qfalse );
  * cgs.media.jumpPadSound = trap\_S\_RegisterSound ("sound/world/jumppad.wav", qfalse );


// FIXME: only needed with item
  * cgs.media.flightSound = trap\_S\_RegisterSound( "sound/items/flight.wav", qfalse );
  * cgs.media.medkitSound = trap\_S\_RegisterSound ("sound/items/use\_medkit.wav", qfalse);
#ifdef TA\_HOLDABLE
  * TEMP:	cgs.media.shurikenSound = trap\_S\_RegisterSound ("sound/items/use\_shuriken.wav", qfalse);
#endif
  * cgs.media.quadSound = trap\_S\_RegisterSound("sound/items/damage3.wav", qfalse);
  * cgs.media.sfx\_ric1 = trap\_S\_RegisterSound ("sound/weapons/machinegun/ric1.wav", qfalse);
  * cgs.media.sfx\_ric2 = trap\_S\_RegisterSound ("sound/weapons/machinegun/ric2.wav", qfalse);
  * cgs.media.sfx\_ric3 = trap\_S\_RegisterSound ("sound/weapons/machinegun/ric3.wav", qfalse);
  * TEMP: cgs.media.sfx\_rockexp = trap\_S\_RegisterSound ("sound/weapons/rocket/rocklx1a.wav", qfalse);
#ifdef MISSIONPACK
  * cgs.media.weaponHoverSound = trap\_S\_RegisterSound( "sound/weapons/weapon\_hover.wav", qfalse );
#endif

  * cgs.media.protectSound = trap\_S\_RegisterSound("sound/items/protect3.wav", qfalse);

## Footstep sounds ##
Footsteps sounds are in sound/player/footsteps/
NOTE: %d is replaced with 1 through 4. (i.e. sound/player/footsteps/step1.wav, sound/player/footsteps/step2.wav, ...)

  * step%d (default)
  * boot%d
  * DONE: clank%d
  * DONE: flesh%d
  * splash%d


# Models #
## Weapons ##
  * models/weapons2/daisho/ (Daisho Pickup)
  * models/weapons2/katana/ (Daisho)
  * models/weapons2/wakizashi/ (Daisho)

## Powerup models ##
  * models/powerups/holdable/
    * TEMP: shurikenelectric.md3
    * TEMP: shurikenfire.md3
    * TEMP: shurikenlaser.md3

  * models/powerups/instant/
    * TEMP: invul.md3  Invulnerability - ?

## MISSIONPACK CTF flags ##
  * TEMP: models/mapobjects/flagbase/red\_base.md3
  * TEMP: models/mapobjects/flagbase/blue\_base.md3
  * TEMP: models/mapobjects/flagbase/ntrl\_base.md3

# Team Arena UI #
Only used the scriptable UI HUD. (Which currently isn't used.)
NOTE: Currently I have the cursors point to the normal q3 cursor image.

  * cgs.media.selectCursor = trap\_R\_RegisterShaderNoMip( "ui/assets/selectcursor.tga" );
  * cgs.media.sizeCursor = trap\_R\_RegisterShaderNoMip( "ui/assets/sizecursor.tga" );

# Quake3 #
## Demo Quake3's pak0.pk3 ##
While replacing stuff I was deleting what I had replaced or was no longer used in the demoq3 pak0.pk3
the following were not removed from the pk3; (I have a list so I can delete the pk3)

  * models/gibs/						--unused by turtlearena
  * models/powerups/					--replaced/unused by turtlearena
  * models/weapons2/					--replaced/unused by turtlearena
  * models/weaphits/
  * models/weaphits/bfg.md3				--unused by turtlearena
  * models/weaphits/crackle.md3			--unused by turtlearena
  * models/weaphits/boom01.md3			--replaced by turtlearena
  * models/weaphits/ring02.md3			--replaced by turtlearena
  * models/weaphits/bullet.md3			--replaced by turtlearena


Replaced in base\_ioq3ztm

  * TEMP: menu/medals/medal\_impressive.tga	--unused by turtlearena --Kill two (or more) players with one railgun shot.
  * TEMP: menu/medals/medal\_excellent.tga	--unused by turtlearena --Kill two players close together (time wise)
  * TEMP: menu/medals/medal\_accuracy.tga	--unused by turtlearena
  * TEMP: menu/medals/medal\_gauntlet.tga	--unused by turtlearena
  * TEMP: menu/medals/medal\_victory.tga		--unused by turtlearena

## Demo TA pak0.pk3 ##
There is (or maybe) more, I didn't delete stuff as I replaced stuff.

  * models/powerups/  --replaced/unused by turtlearena
  * models/powerups/pop.md3 --unused by turtlearena --Just copy models/misc/telep.md3 and move it so its offset correctly?