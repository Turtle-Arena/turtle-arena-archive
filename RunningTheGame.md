See the new building guide [here](BuildingSource.md)

# Introduction #

**Building the game** from source is split into four parts.

  * Getting the Libraries
  * Getting the source
  * Compiling the source
  * Final setup and execution

## Getting the Libraries (Linux, Mac) ##
You will need the following development libraries; SDL, GL, GLU, Ogg, Vorbis, and Theora.

In Debian and Debian based distributions (such as Ubuntu) you can run the below in the terminal.
> sudo apt-get install libsdl1.2-dev libgl1-mesa-dev libglu1-mesa-dev libogg-dev libvorbis-dev libtheora-dev

## Getting the Libraries (Windows) ##
All of the need libraries are included in the source.

## Getting the source ##
You can get the latest development source from the subversion repository or use the source from a released version.

### Subversion ###
You will need a subversion client to get the source, such as [TortoiseSVN](http://tortoisesvn.tigris.org/) (Windows), [RabbitVCS](http://www.rabbitvcs.org) (Linux), or the command line program "svn"

In Debian/Ubuntu you can install the command line program using
> sudo apt-get install subversion

You will need to do a source checkout (Project members see http://code.google.com/p/turtlearena/source/checkout)

> svn checkout http://turtlearena.googlecode.com/svn/trunk/ turtlearena-read-only

### Zipped ###
A zip file containing the source is included with the release, simply unzip it.


## Compiling the source ##
Currently the best way to compile Turtle Arena is from a terminal using "make".

### Linux - Make ###
  1. In a terminal change to the engine/ directory and run "make"
  1. Output will be in engine/build/**dir**/ (Where **dir** is something like "release-linux-i386")

### Windows - Mingw ###
See [Building ioquake3 on Windows - mingw and msys](http://wiki.ioquake3.org/Building_ioquake3_on_Windows_-_mingw_and_msys)

### Windows/Linux - Code::Blocks ###
Currently compiling using Code::Blocks is only supported on Windows and Linux. However it can be used to view and edit the source on all platforms Code::Blocks supports.

  1. Open engine/misc/code\_blocks/ioq3.workspace
  1. Select Platform
  1. Rebuild workspace
  1. Output will be in engine/build/

### Mac [1](http://wiki.ioquake3.org/Building_ioquake3_on_Mac) ###
Compiling Turtle Arena on Mac OS has not been tested. It is known to work with ioquake3, but may need changes to compile Turtle Arena.

#### Snow Leopard - Mac OS X 10.6.2 ####
  1. In a terminal change to the engine/ directory and run "CC=gcc-4.0 ./make-macosx-ub.sh"
  1. Output will be in engine/build/

### Microsoft Visual C++ ###
Currently unsupported. There are project files for ioquake3 in engine/misc/msvc/.

Below are changes that ''should'' make the ioquake3 projects compile Turtle Arena. This is untested.

Workspace:
Add below to workspace
engine/code/splines/Splines.vcproj

Quake3 project:
Link to the output library of Splines project.

Cgame project:
Add below to project
  * engine/code/cgame/cg\_npcs.c

Game project:
Add below to project
  * engine/code/game/g\_npc.c
  * engine/code/game/g\_npcmove.c
  * engine/code/game/g\_npcthink.c
  * engine/code/game/g\_save.c
  * engine/code/game/g\_paths.c

Q3\_UI project:
Add below to project
  * engine/code/q3\_ui/ui\_spplayer.c

Copy the defines from the Turtle Arena config section of the engine/Makefile into all projects (Except Splines and BSPC).

### Windows - Visual Studio 2005 and 2008 ###
See Building ioquake3 on Windows - Visual Studio [2005](http://wiki.ioquake3.org/Building_ioquake3_on_Windows_-_Visual_Studio_2005) and [2008](http://wiki.ioquake3.org/Building_ioquake3_on_Windows_-_Visual_Studio_2008)


# Final setup and execution #

## Linux - Make ##
1. Create a link to each of the 3 qvm files in `engine/build/release-*/base/vm/*.qvm` in base/vm/
This way you don't have to copy the qvms after each compile.

OR Copy the qvms files instead, to allow running in Windows as well.

2. In the terminal change to the directory where you did the checkout. Example: cd /home/$USER/TurtleArena/svn/

3. Run in the terminal; (Example) ./engine/build/release-linux-x86/turtlearena.x86 +set fs\_basepath "."

## Windows - Mingw ##
1. Copy the qvms files from engine\build\release-mingw32-x86\base\vm\ to base\vm\

2. In the command prompt change to the directory where you did the checkout. Example: cd "C:\\Program Files\TurtleArena\svn\"

3. Run in the command prompt: engine\build\release-mingw32-x86\turtlearena.x86 +set fs\_basepath "."

## Windows (Other) ##
Other being Code::Blocks or Microsoft Visual C++.

The Code::Blocks and Microsoft Visual C++ projects do not compile QVMs, only DLLs.
The DLLs are used for testing/debugging, while QVMs are used for release. Mingw also compiles DLLs as well as QVMs.

1. Copy the DLLs files from

  * engine\build\cgame\_release\cgamex86.dll
  * engine\build\game\_release\qagamex86.dll
  * engine\build\q3\_ui\_release\uix86.dll

to base\

2. In the command prompt change to the directory where you did the checkout. Example: cd "C:\\Program Files\TurtleArena\svn\"

3. Run in the command prompt: engine\build\release-mingw32-x86\turtlearena.x86 +set fs\_basepath "." +set vm\_game 0 +set vm\_cgame 0 +set vm\_ui 0