TMNT Arena (working title) is a WIP TMNT fangame based on the ioquake3 engine.

== Version ==
TMNT Arena Alpha 1.0.4 May 4, 2009

== Credits ==
id software
ioquake3
Open Arena
GNU Freefonts
Turtle Man

== Installing ==
Extract TMNT Arena into a directory, such as
Windows:  C:\Program Files\TMNTArena\
Mac OS X: /Applications/TMNTArena/
Linux:    /usr/local/games/tmntarena/

== Uninstalling ==
Delete the directory where you extracted TMNT Arena.

TMNT Arena saves data outside of its directory, so you may wish to remove it.

Windows 95/98 singleuser: C:\Windows\Application Data\TMNTArena\
Windows 95/98 multi-user: C:\WINNT\Profiles\%USERNAME%\Application Data\TMNTArena\
Windows 2000/XP: C:\Documents and Settings\%USERNAME%\Application Data\TMNTArena\
Windows Vista: C:\Users\%USERNAME%\Application Data\TMNTArena\
Mac OS X: /Users/NAME/Library/Application Support/TMNTArena/
Linux: /home/NAME/.tmntarena/

== Compiling ==
Currently the easist (and only supported) way to compile TMNT Arena is GNU Make.

1) Unzip "tmnt-src-*.zip" into the main TMNT Arena directory (There is a directory named "src" inside zip)

=== GNU Make ===
2) In a terminal change to the src/engine/ directory and run "make"
3) Output will be in src/engine/build/*dir*/ (Where *dir* is something like "release-mingw32-x86")

=== Code::Blocks ===
NOTE: Compiling only works on windows.
NOTE: However it can be used to view and edit the source on linux (less code/sys/sys_unix.c)
2) Open src/engine/misc/code_blocks/ioq3.workspace
3) Rebuild workspace.
4) Output will be in src/engine/build

=== Microsoft Visual C++ ===
Currently unsupported. There are project files for ioquake3 in source/misc/msvc/.

Below are changes that "should" make the ioquake3 projects compile TMNT Arena. This is untested.

Worspace:
src/engine/code/splines/Splines.vcproj

Quake3 project:
Link to output of Splines project.

Cgame project:
Add below to project
src/tmnta-engine/code/cgame/cg_npcs.c

Game project:
Add below to project
src/engine/code/game/g_npc.c
src/engine/code/game/g_npcmove.c
src/engine/code/game/g_npcthink.c
src/engine/code/game/g_savestate.c
src/engine/code/game/g_paths.c

Define these in all project less Splines. NOTE TO SELF: Move the defining to a .h file...
: Thses maybe out of date, see Makefile for current ones.
STANDALONE
RANDOMBOT
CELSHADING
TMNT
TMNTHUD
TMNTHOLDABLE
TMNTWEAPSYS
TMNTWEAPSYS2
TMNTWEAPONS
TMNTSP
TMNTPLAYERS
TMNTSKIN
TMNT_SUPPORTQ3
TMNTENTITIES
TMNTPATHS
SINGLEPLAYER
SP_NPC
TMNTHOLDABLE2
RAVENMDR
CAMERASCRIPT

== Code ==
TMNT Arena using code directly or based on code from the following.
Currently not all of the code is released under a free software license,
	it is however mark in the code ("FIXME: NON-GPL").

=== GPL ===
* ioquake3 : http://ioquake3.org/
* OpenArena (OPENARENA): http://openarena.ws --Changed default sound.
* Tremulous (TREMULOUS): http://tremulous.net --G_FindRadius and G_Visible
* Xreal : http://www.xreal-project.net/
** Used Xreal to find what to change to add per-player bounding boxes (In the rev that fixes playerMins/playerMaxs, can't find...)
** Jeff "Jeffro11" Attwood (EXTERNAL_LIGHTMAPS): http://ioquake.org/forums/viewtopic.php?f=2&t=100
* Jay "TT1" Weisskopf (ASPECT_RATIO): https://bugzilla.icculus.org/show_bug.cgi?id=3475

=== Non-free ===
:''All of the non-free will be rewritten or removed.''

* Homing-Rocket by "Ukai Naoto" http://quakestyle.telefragged.com/quake3/tutorial12.htm

==== Disabled non-free ====
* rfactory (SINGLEPLAYER): http://www.rfactory.org
* The Dark Conjunction (via rfactory) (SP_NPC): http://tdc.planetquake.gamespy.com/news.htm

* Star Trek Voyager: Elite Forces [multiplayer] ("func_breakable" code) (TMNTENTITIES)

== Data ==
"Data" referes to the contants of "base/pak0.pk3"

There are source files for some of the data in tmnt-src-*.zip/base-src/

All data is license under a GNU General Public License.

All data, unless otherwise noted, is copyright(c) 2009 Turtle Man
	and is license under the GNU General Public License version 2 or later.

The following are from OpenArena,
	and are license under the GNU General Public License version 2 or later.
:''Later most or all of the images from Open Arena will be replaced.''
base/botfiles/bots/default_c.c
base/botfiles/bots/default_i.c
base/botfiles/bots/default_t.c
base/botfiles/bots/default_w.c
base/botfiles/fuzi.c
base/botfiles/fuzw.c
base/botfiles/items.c
base/botfiles/match.c
base/botfiles/rchat.c
base/botfiles/rnd.c
base/botfiles/syn.c
base/botfiles/teamplay.h
base/botfiles/weapons.h
base/gfx/2d/numbers/*.tga --All 11 files
base/gfx/2d/crosshaira.tga
base/gfx/2d/crosshairb.tga
base/gfx/2d/crosshairc.tga
base/gfx/2d/crosshaird.tga
base/gfx/2d/crosshaire.tga
base/gfx/2d/crosshairf.tga
base/gfx/2d/crosshairg.tga
base/gfx/2d/crosshairh.tga
base/gfx/2d/crosshairi.tga
base/gfx/2d/crosshairj.tga
base/gfx/2d/bigchars.tga
base/gfx/2d/lag.tga
base/gfx/2d/net.jpg
base/gfx/damage/bullet_mrk.jpg
base/gfx/damage/mulletmult.tga
base/gfx/damage/burn_med_mrk.tga
base/gfx/damage/hole_lg_mrk.tga
base/gfx/damage/plasma_mrk.tga
base/gfx/damage/shadow.tga
base/menu/art/font1_prop.tga
base/menu/art/font2_prop.tga
base/menu/art/fx_base.tga
base/menu/art/fx_blue.tga
base/menu/art/fx_cyan.tga
base/menu/art/fx_grn.tga
base/menu/art/fx_red.tga
base/menu/art/fx_teal.tga
base/menu/art/fx_white.tga
base/menu/art/fx_yel.tga
base/sprites/bubble.tga
base/sprites/plasmaa.tga
base/sprites/splash.tga
base/models/weapons2/shotgun/shotgun_hand.md3
:NOTE: The above file was renamed to base/models/weapons2/gun/gun_hand.md3

The following files were generated by q3font.exe using the font "FreeSans.ttf", which is part of GNU Freefonts,
	and are license under the GNU General Public License version 3 or later.

:''I should have use a ttf font, that is license under "GPL v2 or later"...''

base/fonts/fontImage_0_12.tga
base/fonts/fontImage_0_16.tga
base/fonts/fontImage_0_20.tga
base/fonts/fontImage_12.dat
base/fonts/fontImage_16.dat
base/fonts/fontImage_20.dat
