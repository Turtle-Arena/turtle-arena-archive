You first need to get the source code as described [here](BuildingSource.md).

# Introduction #

The development of Turtle Arena is done on Ubuntu Linux. Some things, like packaging for release, are only supported on Linux.

# Getting the Libraries #

You will need the following development libraries; SDL, GL, GLU, Ogg, Vorbis, and Theora.

In Ubuntu (and possibly Debian and other Debian based distributions) you can run the below in the terminal to install all of the needed libraries.

> sudo apt-get install libsdl1.2-dev libgl1-mesa-dev libglu1-mesa-dev libogg-dev libvorbis-dev libtheora-dev

# Compiling #
There are two options for compiling Turtle Arena on Linux; make and Code::Blocks. However, using make is the best option.

## Make ##
  1. In a terminal change to the Turtle Arena engine directory and run "make"
  1. Output will be in engine/build/dir/ (Where dir is something like "release-linux-i386")

### Make 32 bit on 64 bit host ###
To compile i386 binaries for 32 bit system on a 64 bit computer do the following; (Note: Only tested on Ubuntu 10.10)
  1. sudo apt-get install libc6-dev-i386
  1. sudo ln -s /usr/lib32/libSDL-1.2.so.0 /usr/lib32/libSDL.so
  1. In a terminal change to the Turtle Arena engine directory and run "make ARCH=i386"
  1. Output will be in engine/build/release-linux-i386/

## Code::Blocks ##
Using Code::Block for compiling is not recommenced as it is often out dated from the Makefile (used by "make"), but should be up-to-date for stable releases. It creates the game binary and game logic '.so' files, but not the cross-platform game logic '.qvm' files.

  1. Open engine/misc/code\_blocks/ioq3.workspace
  1. Select Platform
  1. Rebuild workspace
  1. Output will be in engine/build/

# Launching the game #

## Using a existing installation (Recommended for stable releases) ##
If you have Turtle Arena install and wish to uses your newly compiled code;

  1. Copy (or link) the game binary (i.e. turtlearena.i386) to path-where-Turtle-Arena-is-installed (you should rename it so it doesn't replace the original)
  1. Copy (or link) the game logic '.so' files to path-where-Turtle-Arena-is-installed/base (If there are already '.so' files in base you may want to move them instead of replacing them)
  1. Run your new game binary, './your-renamed-turtlearena.i386 +set sv\_pure 0 +set vm\_game 0 +set vm\_cgame 0 +set vm\_ui 0'
    * Setting sv\_pure to 0 allows the use of files not in a pk3.
    * Setting vm\_game/vm\_cgame/vm\_ui to 0 tells the game to use the '.so' files.

Note: By using links you will not have to copy the files after each time you compile the source code.

## Using without a existing installation ##
If you did a checkout from subversion you have all of the data needed to run the game, so you do not need a existing install of Turtle Arena.

If you checked out a stable release, you will not be able to play multiplayer on pure servers as they require zipped assets (base/assets0.pk3) with a specific checksum.

### Make ###
  1. Create a link to each of the 3 .qvm files in `engine/build/release-*/base/vm/*.qvm` in base/vm/ This way you don't have to copy the qvms after each compile.
    1. OR Copy the qvms files instead, to allow them to be used in Windows as well.
  1. In the terminal change to the directory where you did the checkout. Example: cd /home/$USER/TurtleArena/svn/
  1. Run in the terminal; (Example) ./engine/build/release-linux-i386/turtlearena.i386 +set fs\_basepath "."

### Code::Blocks ###
Code::Blocks does not build the game logic '.qvm', only the game logic '.so'. So it is slightly different from the instructions for "Make"

  1. Create a link to each of the 3 .so files in `engine/build/release-*/base/*.so` in base/ This way you don't have to copy the .so files after each compile.
  1. In the terminal change to the directory where you did the checkout. Example: cd /home/$USER/TurtleArena/svn/
  1. Run in the terminal; (Example) ./engine/build/release-linux-i386/turtlearena.i386 +set fs\_basepath "." +set vm\_game 0 +set vm\_cgame 0 +set vm\_ui 0
    * Setting vm\_game/vm\_cgame/vm\_ui to 0 tells the game to use the '.so' files.

### Playing on pure servers ###
To play on pure servers the server and client must have the same asset pk3 files.

#### Released version ####
To allow pure mode to be enable you should download the `base/assets*.pk3` files included with the 'portable zip' game download.

You could try creating the pk3s yourself but it is likely that the checksums will be different (which will break compatibility with the main release). You also need to check out the tagged source from subversion (which has the "base" directory containing the data files).

  1. make assets
  1. Copy the `assets*.pk3` files from ./install/base to ./base
  1. Run the game, if it shows warnings about the checksums in the console the generated pk3s are incompatible with the main release; download the pk3s from the main release instead

#### Subversion Trunk ####
If you checked out subversion trunk you need to create the `base/assets*.pk3` files and set the pk3 checksums in the engine to allow pure mode. It allows pure mode to be enable, but unable to play with people whose pk3s have a different checksum. This probably isn't useful, unless you distribute copies of server/client/pk3s to other people.

  1. make assets
  1. Copy the `assets*.pk3` files from ./install/base to ./base
  1. Run the game, it will (most likely) show warnings about incorrect pk3 checksums in the console (and the current pk3 checksums)
  1. Update checksums in engine/code/qcommon/files.c
  1. Rebuild client/server