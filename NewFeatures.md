Incomplete list of editing information for new features not found in ioquake3.

## Shaders ##

### if ###
_Search for "IOQ3ZTM // EF2\_SHADERS" in the code to see implantation._

Support for "if zzz ... endif" shader keywords. Any console variable can be used, but there are some hardcoded options; vextexlight/novertexlight and blood/noblood. (Note: Blood is currently completely disabled in Turtle Arena)

Example shader:
```
textures/name
{
if novertexlight // Or if !r_vertexlight // ! means not
	{
		map textures/name_novertexlight
	}
	{
		map textures/name_1
		blendFunc BLEND
	}
	{
		map textures/name_2
		blendFunc BLEND
	}
if blood
	{
		map textures/name_bloody
		blendFunc BLEND
	}
endif
endif
if vertexlight // Or if r_vertexlight
	{
		map textures/name_vertexlight
	}
endif
}
```


## Skins ##
Skins are used for players and CTF flags.

### $(dir) ###
_Search for "IOQ3ZTM // $DIR\_IN\_SKIN" in the code to see implantation._

$(dir) can be used to specify the directory that the skin file is in.

Example skin:
```
l_leg_right,$(dir)/legs.png
l_leg_left,$(dir)/legs.png
tag_torso,
```

### Damage Shaders ###
_Search for "IOQ3ZTM\_NO\_COMPAT // DAMAGE\_SKINS" in the code to see implantation._

Player/NPC/misc\_object skins can have extra shaders that are used when the player is damaged. There can be up to 10 shaders.

If there are 3 shaders, the first is used when health is 66-100, the second is used when health is 33-66, the last is used when health is 0-33. If there are 4 shaders, the first is 75-100, the second is 50-75, the third is 25-50, the last is 0-25. This continues for up to 10 shaders.

Each mesh can have a different number of shaders.

Example skin:
```
l_legs,players/raph/legs,players/raph/legs_damaged1,players/raph/legs_damaged2
l_mesh,players/raph/mesh,players/raph/mesh_damaged
l_static_mesh,players/raph/show_no_damage
tag_torso,
```

## Damage AlphaGen ##
_Search for "IOQ3ZTM\_NO\_COMPAT // DAMAGE\_SKINS\_AGEN" in the code to see implantation._

Instead of having separate damage shaders (for players, NPCs, and misc\_objects) you can have a single shader and have one image become more (or less) visible as damaged.

"alphaGen damage" causes image to become more visible as damaged. There is also "alphaGen oneMinusDamage", which is the inverse of "alphaGen damage", becomes less visible as damaged.

Example shader:
```
models/players/raph/body
{
	{
		map models/players/raph/body
		rgbGen lightingDiffuse
	}
if blood // Blood is currently always disabled in Turtle Arena, so you will want to remove this check.
	{
		map models/players/raph/body_blood.png
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen damage
	}
endif
}
```

## animations.cfg ##
Player model config file.

### soundpath ###
_Search for "soundpath" in the code to see implantation, it is added by the define TA\_PLAYERSYS._

By placing "soundpath NAME" in animation.cfg the client will use sounds from "sound/NAME/" instead of "sound/HEADMODELNAME/". Soundpath defaults to the client's headmodel name (which is usually the player model directory name).

soundpath is used in Star Trek: Voyager - Elite Force.

Example:
```
soundpath raph
```