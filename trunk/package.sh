#!/bin/bash
#
# Package Turtle Arena for release.
#
#  Supports Zip releases.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Directory to put the files for release
INSTALLDIR=install

# Package Linux binaries
LINUX=1

# Create assets0.pk3
DATAFILES=1

# TODO: Zip the files? currently they have to be zipped /after/ running the script.
MAKEZIP=1

	# Package win32 binaries for zip install
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

	if [ "$ARG" = "--no-zip" ]
	then
		MAKEZIP=0
		NEXT_ARG=""
		continue
	fi

	if [ "$ARG" = "--no-data" ]
	then
		DATAFILES=0
		NEXT_ARG=""
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
	echo "    --no-zip            don't copy files for zip install to installdir"
	echo "    --no-data           don't create assests0.pk3"
	echo "    --installdir [dir]  directory to copy files to for zip install"
	echo "                          (default: \"install\")"
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
# Disable binary building if not zip
#
if [ $MAKEZIP -eq 0 ]
then
	LINUX=0
	WIN32=0
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

		echo "  Warning: You need to manually copy SDL.dll version 1.2.14 into \"$INSTALLDIR\"!"
	fi

fi


#
# Create assets0.pk3
#

if [ $DATAFILES -eq 1 ]
then
	echo "Data..."

	./package-assets.sh --installdir $INSTALLDIR
fi

#
# Copy readme, COPYING, etc for zip
#

if [ $MAKEZIP -eq 1 ]
then
	echo "Copying docs..."

	if [ $LINUX -eq 1 ]
	then
		# Copy linux launcher too
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

fi

echo "Done!"

