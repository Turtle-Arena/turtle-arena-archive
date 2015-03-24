# Roadmap #
Work-in-progress plans for future releases.

## 0.6 series ##
See svn/branches/0.6/

### 0.6.1 ###
  * DONE: Fix symlink issue on Linux. (CVE-2012-3345)
  * DONE: Fix bot jump underwater issue. ([Issue 37](https://code.google.com/p/turtlearena/issues/detail?id=37))
  * DONE: Fix bots to wait at surface for air meter to fill. ([Issue 38](https://code.google.com/p/turtlearena/issues/detail?id=38))
  * DONE: Fix bots not going for air when they have no long term goal.

## 0.7 series ##
Focus on finalizing the game's branding (name, logo, etc), making it completely standalone from the Ninja Turtles, and solve all licensing concerns.

  * Make all 2D graphics be 4 times quake3 resolution.

### 0.7.0 ###
  * Make changes to player model system for future models.
  * Add head model animation support.
  * Make sure IQM support is fully working.
  * Create test single model IQM player that uses skeleton blending (for testing and demonstration new player model animation system).
  * Start making new player models.

### EBX 0.7.1 ###
  * Remove the TMNT player models.
  * Add a new player model designed for the game.
  * Add weapon for new player.

  * Rename the game to Extraordinary Beat X (EBX).
  * Replace logo.
  * Create new menu background.

### EBX 0.7.2 ###
  * Add new player model(s).
  * Add weapon(s) for new player(s) if needed.
  * Finish breakable/pushable objects and brushes code.

### EBX 0.7.3 ###
0.7.3 may be kind of a Release Candidate for 0.8.

  * (Finish four main player models)
  * Improve current maps (but only a little if at all)
  * Fix any issues that are found.

## EBX 0.8 ##
Optional goals:
  * Clean up / rewrite shaders (do not have clear licensing for them)?
  * Remake all (non-texture) images that do not have a svg source file?
  * Switch to missionpack ui?
  * Remake some of the sound effects / add some of the missing sounds effects?

## Complete Multiplayer Game (EBX 1.0) ##
Focus on map themes and multiplayer maps. The goal is the develop assets/skills for single player. The overall goal is to hopefully make Turtle Arena a complete (fully working) multiplayer game, comparable to commercial games.

  * Create scenery objects and brushes
  * Map Themes (Sets of textures, sky, models, misc\_objects, and music)
  * New multiplayer maps using the new map themes

  * Create new editor config that is CC-BY-SA-3.0, cannot reuse text from current config (that I didn't write).
  * Create HTML editing and entities documents (included in radiant config) (CC-BY-SA-3.0).
  * Create HTML BSPC document (CC-BY-SA-3.0)

  * Create all new textures for each map theme (5?).
  * Create a sky shader for each map theme (5?).
  * Create 6 music tracks, menu + one for each map theme (5?).
  * Add missing sound effects.

  * Create map objects.
  * Finish interactive map object code.

  * Try to improve the gameplay experience.

  * Create 10 new maps, one map for FFA/Duel/TDM and one for CTF/1Flag/Overload using each of the 5 map themes.

## Main Game Focus (EBX 1.2) ##
Complete the majority of the Main Game (Single Player) / Co-op code and have it playable.

  * Add proper co-op end of level screen.
  * Add main game end of game stuff.

  * Create NPC system.
  * Create NPCs (models, sounds, and scripts that tell game info about NPCs).
  * Update radiant config and HTML docs.

  * Remove current co-op maps.
  * Remove code for enemy co-op bots?

  * Create 3 new co-op maps (a single stage) using map theme from 0.8 with NPCs.
  * Create a second stage?

  * Add co-op bot AI.

  * Finish splitscreen sound support.

## Complete Main Game (EBX 1.4) ##
  * Create remaining main game maps (3 or 4 stages, each with 3 maps)
  * Add main game ending.

  * Add hidden items (one for each of the main characters in each main gmae map).

## Multiplayer Expanded (EBX 1.6) ##

  * Add more non-main-game maps (10 more maps?)
  * Improve existing non-co-op maps.

  * Add more non-main-game characters.

  * Expand map themes by adding more map objects, skies, etc?

## Completed (EBX 2.0) ##
  * Translation support.
  * Polish overall game.
  * Create complete editing documentation for the game.