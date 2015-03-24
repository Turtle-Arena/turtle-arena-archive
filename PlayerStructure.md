**Player** (playable character) structure. Players have three models that are attached using "tags". Weapons are also attached using tags.



# Locating the data #
All of the data files are stored in base/assets0.pk3, which is simply a renamed .zip file.
Pk3 files can be extracted with any zip program.

# Files #
Players are stored in "base/models/players/PLAYERNAME/", in the release the data is zipped into base/assets0.pk3 (which is simply a renamed .zip)

  * head.md3 - Head model, it will be attached to torso.md3
  * torso.md3 - Upper model, it will be attached to legs.md3
  * legs.md3 - Lower model
  * animation.cfg - Contains animations and other settings for the player

  * **Default skin**
  * icon\_default.png - Icon for the default skin
  * head\_default.skin - Shaders for head default skin
  * torso\_default.skin - Shaders for torso default skin
  * legs\_default.skin - Shaders for legs default skin

  * **Red teamplay skin**
  * icon\_red.png
  * head\_red.skin
  * torso\_red.skin
  * legs\_red.skin

  * **Blue teamplay skin**
  * icon\_blue.png
  * head\_blue.skin
  * torso\_blue.skin
  * legs\_blue.skin


# Skins #
Skins are made up of three ".skin" files and a icon image (.png/.tga/.jpg). The skins default, red, and blue should be include to support all gametypes. Extra skins can be include, for example Raph has a "fugitoid" skin.

  * icon\_SKINNAME.png - Icon for the skin
  * head\_SKINNAME.skin - Shaders for head
  * torso\_SKINNAME.skin - Shaders for torso
  * legs\_SKINNAME.skin - Shaders for legs

## Teamplay skins ##
In teamplay gametypes, there can be per-team skins stored in "models/players/PLAYERNAME/TEAMNAME/". TEAMNAME is set to cvar g\_blueTeam (default: Shell) or cvar g\_redTeam (default: Foot).

## .skin Format ##
**.skin** files set the shaders used by the model. MD3 models contain "meshes" (Groups of triangles), each must have a shader listed. The "tags" (points where other models are attached) are also listed.

Example: Raph's head\_default.skin
```
h_head,models/players/raph/head
h_head2,models/players/raph/head
h_eye_left,models/players/raph/white
h_eye_right,models/players/raph/white
h_mouth_left,models/players/raph/white
h_mouth_right,models/players/raph/white
tag_head,
```

Turtle Arena also supports using **$(dir)** instead of the directory where the player is.
Example: "$(dir)/head" instead of "models/players/raph/head".

# animation.cfg #
animation.cfg contains the animations and other settings. The animation.cfg is normally created when exporting a player model to MD3.

## soundpath ##
By placing "soundpath NAME" in animation.cfg the client will use sounds from "sound/NAME/" instead of "sound/HEADMODELNAME/". Soundpath defaults to the client's headmodel name (which is usually the player model directory name).

## jumpMult ##
By placing "jumpMult 1.5" in animation.cfg, the players jump velocity will be multiplied by what ever "jumpMult" was set to.

# Documentation #
## Tags ##
List of player tags by models.

### head.md3 ###
  * tag\_head

### upper.md3 ###
  * tag\_torso
  * tag\_head
  * tag\_hand\_primary   -- Player's right hand. Used for holding guns/melee weapons.
  * tag\_hand\_secondary -- Player's left hand. Used for holding CTF flags/melee weapons.
  * // Hold default weapon of the player when not in use.
  * tag\_wp\_away\_primary
  * tag\_wp\_away\_secondary
  * // Currently I just use tag\_weapon / tag\_flag for compatibility to and from quake3 and MD3 model exporters...
  * tag\_weapon (for Quake 3 capatibility) -- Use like tag\_hand\_primary
  * tag\_flag   (for Team Arena capatibility) -- Use like tag\_hand\_secondary

### lower.md3 ###
  * tag\_torso

## Full skin search info ##
Below is the search order for ".skin" files in Turtle Arena. It uses code from Quake III: Team Arena to allow per-team skins. So Team Arena Players work in team gametypes as well as Quake 3 players.

Based on http://rfactory.org/idteam.html#appendh with the changes I've made to ioquake3.

Note: "raph/fugitoid" is a example; raph is the model, fugitoid is the skin.

### Head ###
Note: If headmodel starts with `*` ("`*`raph/fugitoid") the "heads" directory is checked before the "players" directory is checked.

#### Team play ####
  * headmodel = raph/fugitoid
  * team = red
  * g\_redTeam cvar = Foot (blue team use g\_blueTeam cvar, default Shell)

  1. models/players/raph/fugitoid/Foot/head\_red.skin
  1. models/players/raph/Foot/head\_red.skin
  1. models/players/raph/fugitoid/head\_red.skin
  1. models/players/raph/head\_red.skin
  1. models/players/heads/raph/fugitoid/Foot/head\_red.skin
  1. models/players/heads/raph/Foot/head\_red.skin
  1. models/players/heads/raph/fugitoid/head\_red.skin
  1. models/players/heads/raph/head\_red.skin

#### Non-team play ####
  * headmodel = raph/fugitoid

  1. models/players/raph/head\_fugitoid.skin
  1. models/players/heads/raph/head\_fugitoid.skin

### Body (lower/upper) ###
#### Team play ####
  * model = raph/fugitoid
  * team = red
  * g\_redTeam = Foot

  1. models/players/raph/Foot/lower\_fugitoid\_red.skin
  1. models/players/raph/Foot/lower\_red.skin
  1. models/players/raph/lower\_fugitoid\_red.skin
  1. models/players/raph/lower\_red.skin
  1. models/players/characters/raph/Foot/lower\_fugitoid\_red.skin
  1. models/players/characters/raph/Foot/lower\_red.skin
  1. models/players/characters/raph/lower\_fugitoid\_red.skin
  1. models/players/characters/raph/lower\_red.skin

#### Non-team play ####
  * model = raph/fugitoid

  1. models/players/raph/lower\_fugitoid.skin
  1. models/players/characters/raph/lower\_fugitoid.skin