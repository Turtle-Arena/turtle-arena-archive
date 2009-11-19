TMNT Arena is a work-in-progress Ninja Turtle fangame based on the ioquake3 engine.

== Version ==
TMNT Arena Alpha (November 6th 2009)

== Credits ==
* id software			(For making and releasing Quake III Arena's source code)
* ioquake3 devoplers	(For improving, fixing, and cleaning up Quake 3's source code)
* ioSTVEF				(For "noise" and "random" waveform keywords, and RF_FORCE_ENT_ALPHA render flag)
* Open Arena			(For botfiles, images, ...)
* Xreal project			(For Homing rockets and shootable rockets)
* Smokin' Guns			(For a minor animation time fix)
* GNU FreeFonts			(For FreeSans.ttf, FreeSansBold.ttf, and FreeMono.ttf)
* Mercenaries Guild Client for Tremulous (Fixed up freetype code and use TTF in console)
* Zack "ZTurtleMan" Middleton

Also see "Code" section for full list.

== Installing ==
Extract TMNT Arena into a directory. It can be placed anywhere, but suggestions below

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
Mac OS X: /Users/$USERNAME/Library/Application Support/TMNTArena/
Linux: /home/$USERNAME/.tmntarena/

== Compiling ==
See tmnt-compiling.txt

== Code ==
TMNT Arena uses code directly or based on the following.

=== GPL ===
* ioquake3 (Base engine): http://ioquake3.org/

* ioSTVEF (IOSTVEF): http://thilo.kickchat.com/efport-progress/
** (IOSTVEF_NOISE) Adds support for "noise" and "random" waveform keywords
** RF_FORCE_ENT_ALPHA, it will be used so items/weapons fad out when dispearing.

* OpenArena (OPENARENA): http://openarena.ws
** Changed default sound filename. Load Ogg sounds (when no wav).
** (OA_BLOOM) OpenArena's bloom effect, disabled by default. (r_bloom 1 to enable)

* Xreal (XREAL): http://www.xreal-project.net/
** Homing rockets (Xreal svn r2785)
** Fired rockets and homing-rockets can be destroyed (Xreal svn r > 2785)
** Used Xreal to find what to change to add per-player bounding boxes (Xreal svn r3050)
** Jeff "Jeffro11" Attwood (EXTERNAL_LIGHTMAPS): http://ioquake.org/forums/viewtopic.php?f=2&t=100

* Smokin' Guns (SMOKIN_GUNS): http://www.smokin-guns.net/
** Fixed animation timing based on PM_AnimLength from Smokin' Guns' bg_pmove.c

* Mercenaries Guild Client 1.011 for Tremulous: http://projects.mercenariesguild.net/projects/mgclient
** (USE_FREETYPE) Lakitu7-freetype+consolecustomize.patch  (Not all of the patch was used, modified some parts, ...)

=== Other ===
: Code based on guides, etc.

* rfactory: http://www.rfactory.org/
** The Dark Conjunction (via rfactory): http://tdc.planetquake.gamespy.com/news.htm
* "inolan.com", basic breakable brush code: http://www.quake3hut.co.uk/q3coding/Breakable%20Glass.htm

== License ==
The TMNT Arena source code and data are licensed under the GNU General Public License version 2 or later.
	(See COPYING.txt for full license)

See COPYRIGHTS for data per-file copyrights.

== Software used ==
Software used to create the TMNT Arena data files.

== Text ==
* Code::Blocks
* gedit (GNOME Text editor)
* Microsoft Windows Notepad/Wordpad

=== 3D Models ===
* Wings3D
* Misfit Model 3D

=== 2D Graphics ==
* GIMP

