#!/bin/bash
#
# Package Turtle Arena for release.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Directory to put the files for release
INSTALLDIR=install

# Package Linux deb and copy Linux binaries into installdir
LINUX=1

# Package Win32 NSIS installer and copy Win32 binaries into installdir
WIN32=1


# Check for x86_64
if [ `uname -m` = "x86_64" ]
then
	ARCH=x86_64
else
	ARCH=i386
fi

# Platform as in 'engine/build/release-linux-x86_64/base/vm'
PLATFORM=linux-$ARCH

# Vars for command line handling
USAGE=0
NEXT_ARG=""

#
# Read command line arguments
#
for ARG in `echo $*`
do

	#
	# Single argument options
	#

	if [ "$ARG" = "--help" ] || [ "$ARG" = "-help" ] || [ "$ARG" = "-h" ]
	then
		USAGE=1
		break
	fi

	if [ "$ARG" = "--no-linux" ]
	then
		LINUX=0
		continue
	fi

	if [ "$ARG" = "--no-win32" ]
	then
		WIN32=0
		continue
	fi

	#
	# Arguments that have a token after it
	#

	if [ "$ARG" = "--installdir" ]
	then
		NEXT_ARG="$ARG"
		continue
	fi

	case "$NEXT_ARG" in
		--installdir)
			INSTALLDIR="$ARG"
			NEXT_ARG=""
			;;
		*)
			echo "Unknown argument '$ARG'"
			USAGE=2
			NEXT_ARG=""
			;;
	esac

done

#
# Show usage, the user asked to see it.
#
if [ $USAGE -eq 1 ]
then
	echo "Usage: $0 OPTIONS..."
	echo "  Package Turtle Arena for release. Creates assets0.pk3, compiles"
	echo "    and copies binaries into installdir"
	echo ""
	echo "  OPTIONS"
	echo "    --help         Show this help"
	echo "           -help"
	echo "           -h"
	echo "    --installdir [dir]  directory to copy files to for zip install"
	echo "                          (default: \"install\")"
	echo "    --no-linux          Don't package Linux"
	echo "    --no-win32          Don't package Win32"
	exit 1
fi

#
# Mini usage message for miss formated command.
#
if [ $USAGE -eq 2 ]
then
	echo "Try \`$0 --help' for more information."
	exit 1
fi


#
# Make sure everything is built
#

cd engine

if [ $LINUX -eq 1 ]
then
	make BUILD_GAME_SO=0 BUILD_GAME_QVM=1

	# If running x86_64, compile i386 too.
	if [ $ARCH = "x86_64" ]
	then
		make ARCH=i386 BUILD_GAME_SO=0
	fi
else
	# We need to at least build the QVMs!
	make BUILD_CLIENT=0 BUILD_CLIENT_SMP=0 BUILD_SERVER=0 BUILD_GAME_SO=0 BUILD_GAME_QVM=1
fi

if [ $WIN32 -eq 1 ]
then
	sh cross-make-mingw.sh ARCH=x86 BUILD_GAME_SO=0
fi

cd $STARTDIR

#
# Create install directory
#

mkdir -p $INSTALLDIR


#
# Copy client and server binaries
#

if [ $LINUX -eq 1 ] || [ $WIN32 -eq 1 ]
then

	echo "Coping binaries..."

	if [ $LINUX -eq 1 ]
	then
		cp engine/build/release-$PLATFORM/turtlearena.$ARCH $INSTALLDIR
		cp engine/build/release-$PLATFORM/turtlearena-server.$ARCH $INSTALLDIR

		if [ $ARCH = "x86_64" ]
		then
			cp engine/build/release-linux-i386/turtlearena.i386 $INSTALLDIR
			cp engine/build/release-linux-i386/turtlearena-server.i386 $INSTALLDIR
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		cp engine/build/release-mingw32-x86/turtlearena.x86.exe $INSTALLDIR
		cp engine/build/release-mingw32-x86/turtlearena-server.x86.exe $INSTALLDIR

		if [ ! -f $INSTALLDIR/SDL.dll ]
		then
			echo "  Warning: You need to manually copy SDL.dll version 1.2.14 into \"$INSTALLDIR\"!"
		fi
	fi

fi

#
# Create assets0.pk3
#

if [ ! -f $INSTALLDIR/base/assets0.pk3 ]
then
	echo "Data..."

	./package-assets.sh --installdir $INSTALLDIR

	if [ -f $INSTALLDIR/base/assets0.pk3 ]
	then
		#
		# Must manually update checksum
		#

		# OLD
		#echo "Go run Turtle Arena and update the checksum for assets0.pk3 near"
		#echo "    the top of engine/code/qcommon/files.c!"

		# NEW
		EDITOR=nano
		echo ""
		echo "You may need to update the assets0.pk3 checksum."
		echo " 1) Game server will run, copy checksum if shown (Ctrl+Shift+C), type quit"
		echo " 2) $EDITOR will open engine/code/qcommon/files.c, replace the old checksum"
		echo "  if one was shown (scroll down, paste using Ctrl+Shift+V), exit editor (Ctrl+X)"

		# Wait for keypress
		tput smso
		echo "Press any key"
		tput rmso
		oldstty=`stty -g`
		stty -icanon -echo min 1 time 0
		dd bs=1 count=1 >/dev/null 2>&1
		stty "$oldstty"
		echo ""

		# Run server
		cd $INSTALLDIR
		./turtlearena-server.$ARCH

		echo ""
		echo "Remember!"
		echo " 2) $EDITOR will open engine/code/qcommon/files.c, replace the old checksum"
		echo "  if one was shown (scroll down, paste using Ctrl+Shift+V), exit editor (Ctrl+X)"

		# Wait for keypress
		tput smso
		echo "Press any key"
		tput rmso
		oldstty=`stty -g`
		stty -icanon -echo min 1 time 0
		dd bs=1 count=1 >/dev/null 2>&1
		stty "$oldstty"
		echo ""

		# Open editor
		cd $STARTDIR
		$EDITOR engine/code/qcommon/files.c

		# Updated checksum in source, rerun script!
		./$0
	fi

	exit 1
fi

#
# Copy readme, COPYING, etc for zip
#

echo "Copying docs..."

if [ $LINUX -eq 1 ]
then
	# Copy Linux launcher too
	cp extras/turtlearena.sh $INSTALLDIR
fi

cp GAME_README.txt $INSTALLDIR/readme.txt
cp COPYING.txt $INSTALLDIR
cp COPYRIGHTS.txt $INSTALLDIR
cp CREDITS.txt $INSTALLDIR

# Convert to dos line ending
todos $INSTALLDIR/*.txt

# Copy all of the files other than base/ into turtlearena-src/ and zip it.
echo "Warning: You need to manually copy the source into $INSTALLDIR !"

# zip install?
# base/assets0.pk3 readme.txt COPYING.txt COPYRIGHTS.txt CREDITS.txt
# if Linux; turtlearena.sh
# if Linux x86_64; .x86_64 and .i386 binaries
# if Linux i386; .i386 binaries
# if Win32; .x86.exe binaries



#
# Build Win32 NSIS installer
#
if [ $WIN32 -eq 1 ]
then
	./package-nsis.sh --installdir $INSTALLDIR
fi

#
# Build DEB source and binary packages
#
if [ $LINUX -eq 1 ]
then
	./package-deb.sh --installdir $INSTALLDIR/deb
fi

echo "Done!"

