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

# Extra args for make.
BUILD_FINAL=0

# Found clean arg, delete package files.
CLEAN=0

# Check for x86_64
if [ `uname -m` = "x86_64" ]
then
	ARCH=x86_64
else
	ARCH=i386
fi

OS=linux
# Platform as in 'linux-x86_64' ('engine/build/release-linux-x86_64/base/vm')
PLATFORM=$OS-$ARCH

# Set to 1 if there is a reason to not create zip
ZIPERROR=0

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

	if [ "$ARG" = "--help" ] || [ "$ARG" = "-h" ]
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

	if [ "$ARG" = "--final" ] || [ "$ARG" = "-f" ]
	then
		BUILD_FINAL=1
		continue
	fi

	if [ "$ARG" = "clean" ]
	then
		CLEAN=1
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
	echo "Usage: $0 OPTIONS... [clean]"
	echo "  Package Turtle Arena for release. Creates assets0.pk3, compiles"
	echo "    and copies binaries into installdir"
	echo ""
	echo "  OPTIONS"
	echo "    --help         Show this help"
	echo "           -h"
	echo "    --installdir [dir]  directory to copy files to for zip install"
	echo "                          (default: \"install\")"
	echo "    --no-linux          Don't package Linux"
	echo "    --no-win32          Don't package Win32"
	echo "    --final             Don't include svn or ioq3 revision in version"
	echo "           -f"
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
# Clean everything up
#
if [ $CLEAN -eq 1 ]
then
	rm -r $INSTALLDIR

	cd engine

	if [ $LINUX -eq 1 ]
	then
		make clean ARCH=$ARCH
		if [ $ARCH = "x86_64" ]
		then
			make clean ARCH=i386
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		sh cross-make-mingw.sh clean
	fi

	exit 1
fi

#
# Make sure everything is built
#

cd engine

if [ $LINUX -eq 1 ]
then
	make BUILD_FINAL=$BUILD_FINAL ARCH=$ARCH BUILD_GAME_QVM=1

	# If running x86_64, compile i386 too.
	if [ $ARCH = "x86_64" ]
	then
		make BUILD_FINAL=$BUILD_FINAL ARCH=i386
	fi
else
	# We need to at least build the QVMs!
	make BUILD_FINAL=$BUILD_FINAL BUILD_CLIENT=0 BUILD_CLIENT_SMP=0 BUILD_SERVER=0 BUILD_GAME_SO=0 BUILD_GAME_QVM=1
fi

if [ $WIN32 -eq 1 ]
then
	sh cross-make-mingw.sh BUILD_FINAL=$BUILD_FINAL
fi

cd $STARTDIR

#
# Create install directory
#

ZIPNAME=turtlearena-0.5.0
ZIPDIR=$INSTALLDIR/$ZIPNAME

mkdir -p $ZIPDIR/base


#
# Copy client and server binaries
#

if [ $LINUX -eq 1 ] || [ $WIN32 -eq 1 ]
then

	echo "Coping binaries..."

	if [ $LINUX -eq 1 ]
	then
		cp engine/build/release-$PLATFORM/turtlearena.$ARCH $ZIPDIR
		cp engine/build/release-$PLATFORM/turtlearena-server.$ARCH $ZIPDIR
		cp engine/build/release-$PLATFORM/base/cgame$ARCH.so $ZIPDIR/base
		cp engine/build/release-$PLATFORM/base/qagame$ARCH.so $ZIPDIR/base
		cp engine/build/release-$PLATFORM/base/ui$ARCH.so $ZIPDIR/base

		if [ $ARCH = "x86_64" ]
		then
			cp engine/build/release-$OS-i386/turtlearena.i386 $ZIPDIR
			cp engine/build/release-$OS-i386/turtlearena-server.i386 $ZIPDIR
			cp engine/build/release-$OS-i386/base/cgamei386.so $ZIPDIR/base
			cp engine/build/release-$OS-i386/base/qagamei386.so $ZIPDIR/base
			cp engine/build/release-$OS-i386/base/uii386.so $ZIPDIR/base
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		cp engine/build/release-mingw32-x86/turtlearena.x86.exe $ZIPDIR
		cp engine/build/release-mingw32-x86/turtlearena-server.x86.exe $ZIPDIR
		cp engine/build/release-mingw32-x86/base/cgamex86.dll $ZIPDIR/base
		cp engine/build/release-mingw32-x86/base/qagamex86.dll $ZIPDIR/base
		cp engine/build/release-mingw32-x86/base/uix86.dll $ZIPDIR/base

		if [ ! -f $ZIPDIR/libcurl-4.dll ]
		then
			if [ -f engine/misc/nsis/libcurl-4.dll ]
			then
				cp engine/misc/nsis/libcurl-4.dll $ZIPDIR
			else
				echo "  Warning: You need to manually copy libcurl-4.dll into \"$ZIPDIR\"!"
				ZIPERROR=1
			fi
		fi
		if [ ! -f $ZIPDIR/OpenAL32.dll ]
		then
			if [ -f engine/misc/nsis/OpenAL32.dll ]
			then
				cp engine/misc/nsis/OpenAL32.dll $ZIPDIR
			else
				echo "  Warning: You need to manually copy OpenAL32.dll into \"$ZIPDIR\"!"
				ZIPERROR=1
			fi
		fi
		if [ ! -f $ZIPDIR/SDL.dll ]
		then
			if [ -f engine/misc/nsis/SDL.dll ]
			then
				cp engine/misc/nsis/SDL.dll $ZIPDIR
			else
				echo "  Warning: You need to manually copy SDL.dll version 1.2.14 into \"$ZIPDIR\"!"
				ZIPERROR=1
			fi
		fi
		if [ ! -f $ZIPDIR/zlib1.dll ]
		then
			if [ -f engine/misc/nsis/zlib1.dll ]
			then
				cp engine/misc/nsis/zlib1.dll $ZIPDIR
			else
				echo "  Warning: You need to manually copy zlib1.dll into \"$ZIPDIR\"!"
				ZIPERROR=1
			fi
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
		#ensure EDITOR is set
		if [ -z "${EDITOR}" ]
		then
			EDITOR=nano
		fi
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
		cd $ZIPDIR
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
# Copy README, COPYING, etc for zip
#

echo "Copying files for zip..."

if [ ! -d $ZIPDIR/base ]
then
	mkdir $ZIPDIR/base
fi
if [ -f $ZIPDIR/base/assets0.pk3 ]
then
	rm $ZIPDIR/base/assets0.pk3
fi
cp $INSTALLDIR/base/assets0.pk3 $ZIPDIR/base

if [ $LINUX -eq 1 ]
then
	# Copy Linux launcher too
	cp extras/turtlearena.sh $ZIPDIR
fi

cp GAME_README.txt $ZIPDIR/README.txt
cp COPYING.txt $ZIPDIR
cp COPYRIGHTS.txt $ZIPDIR
cp CREDITS.txt $ZIPDIR

# Convert to dos line ending
todos $ZIPDIR/*.txt

# Enable portable app mode!
mkdir $ZIPDIR/settings
echo "yes" > $ZIPDIR/settings/portable

# Create zip
if [ $ZIPERROR -eq 0 ]
then
	cd $INSTALLDIR
	if [ ! -f $ZIPNAME.zip ]
	then
		zip -r $ZIPNAME.zip $ZIPNAME
	fi
	cd $STARTDIR
fi

#
# Package tarball
#
if [ $LINUX -eq 1 ]
then
	cd engine
	make dist BUILD_FINAL=$BUILD_FINAL
	cd ..
	mkdir $INSTALLDIR/tarball
	mv engine/*.tar.bz2 $INSTALLDIR/tarball
fi

#
# Build Loki setup .run
#
if [ $LINUX -eq 1 ]
then
	cd engine
	make installer BUILD_FINAL=$BUILD_FINAL
	cd ..
	mkdir $INSTALLDIR/run
	mv engine/misc/setup/*.run $INSTALLDIR/run
fi

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
	./package-deb.sh --installdir $INSTALLDIR
fi

echo "Done!"

