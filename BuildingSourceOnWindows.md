You first need to get the source code as described [here](BuildingSource.md).

# Compiling the source #
Currently the best way to compile Turtle Arena is from a terminal using "make".

## Make ##
See [Building ioquake3 on Windows - mingw and msys](http://wiki.ioquake3.org/Building_ioquake3_on_Windows_-_mingw_and_msys).

## Code::Blocks ##
  1. Open engine/misc/code\_blocks/ioq3.workspace
  1. Select Platform
  1. Rebuild workspace
  1. Output will be in engine/build/

# Launching the game #

## Make ##
1. Copy the qvms files from engine\build\release-mingw32-x86\base\vm\ to base\vm\

2. In the command prompt change to the directory where you did the checkout. Example: cd "C:\\Program Files\TurtleArena\svn\"

3. Run in the command prompt: engine\build\release-mingw32-x86\turtlearena.x86 +set fs\_basepath "."

## Code::Blocks ##
The projects do not compile QVMs, only DLLs. The DLLs are used for testing/debugging, while QVMs are used for release. "make" compiles DLLs as well as QVMs.

  1. Copy the DLLs files (engine\build\cgame\_release\cgamex86.dll, engine\build\game\_release\qagamex86.dll, engine\build\q3\_ui\_release\uix86.dll) into base\
  1. In the command prompt change to the directory where you did the checkout. Example: cd "C:\\Program Files\TurtleArena\svn\"
  1. Run in the command prompt: engine\build\release-mingw32-x86\turtlearena.x86 +set fs\_basepath "." +set sv\_pure 0 +set vm\_game 0 +set vm\_cgame 0 +set vm\_ui 0