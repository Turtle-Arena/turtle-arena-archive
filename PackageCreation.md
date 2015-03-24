Turtle Arena is released using a few different installers/packages, this page describes how to create them. To create any of the packages you need the Turtle Arena source code and the ability to compile Turtle Arena (using 'make').

## Types ##
Turtle Arena is currently released using four methods; zip archive, Win32 NSIS installer, Debian packages, and loki setup Linux Installer.

## All ##
To create all of the packages at once, you must use Linux.

  1. You need to have nsis installed to build to Win32 NSIS installer
  1. If you are building a released version (as apposed to from subversion trunk), copy `assests*.pk3` into install/base/
  1. Run make in the directory containing engine/ and extras/
  1. Packages will be in the install directory.

## Zip Archive ##
The zip archive can be created by running 'make zip' and manually zipping the proper files in the 'install' directory.

  1. If you are building a released version (as apposed to from subversion trunk), copy `assests*.pk3` into install/base/
  1. Run 'make all'
  1. Open 'install' directory
  1. Zip the 'turtlearena-x.x.x' directory (where x.x.x is the version, such as 0.4.2).

## Loki Setup ##
Linux Installer.

  1. If you are building a released version (as apposed to from subversion trunk), copy `assests*.pk3` into install/base/
  1. Run 'make loki'
  1. Open 'install/loki' directory

## Debian/Ubuntu Packages ##
If you are building from subversion trunk it is recommended you use the "All" method described above.

  1. If you are building a released version (as apposed to from subversion trunk), copy `assests*.pk3` into install/base/
  1. Run './package-deb.sh'
  1. Open 'install/deb' directory

## Windows NSIS Installer ##
To create the Windows NSIS Installer you need to install NSIS and the Turtle Arena source code.

  1. Install NSIS 2.44 (2.46 doesn't create a valid installer) (NOTE: Ubuntu 10.04 (lucid) has 2.46 in its package repository so you will need to install a older 2.44 package from Ubuntu 9.10 (karmic))
  1. Download the Turtle Arena source code if you do not already have it
  1. Copy the `assets*.pk3` files into the engine/misc/nsis/ directory inside of the Turtle Arena source
  1. Open a terminal and change to the engine/misc/nsis/ directory inside of the Turtle Arena source
  1. Run 'make'

There should now be a turtlearena-x.x.x-0.x86.exe in the engine/misc/nsis/ directory.

## Windows NSIS Mod Installer ##
Turtle Arena modifications (mods) can have their own Windows NSIS Installer.

  1. Browse to engine/misc/nsis/ directory inside of the Turtle Arena source
  1. Copy the files for the mod into the directory.
  1. Copy turtlearena-q3ctc.nsi and rename it to trutlearena-yourmodname.nsi
  1. Open the new file and modify the lines above 'no need to edit below in theory' to suit your mod
  1. Run 'makensis trutlearena-yourmodname.nsi'

If all went well, you should have a installer/uninstaller for your mod now.