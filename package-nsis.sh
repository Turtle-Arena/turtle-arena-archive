#!/bin/sh
#
# Package Turtle Arena Win32 NSIS installer for release.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Update version in engine/misc/nsis/Makefile

# Directory to put the files for release
INSTALLDIR=install

# Directory containing base/assets0.pk3
DATADIR=install

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

	#
	# Arguments that have a token after it
	#

	if [ "$ARG" = "--installdir" ] || [ "$ARG" = "-i" ]
	then
		NEXT_ARG="--installdir"
		continue
	fi

	if [ "$ARG" = "--datadir" ] || [ "$ARG" = "-d" ]
	then
		NEXT_ARG="--datadir"
		continue
	fi

	case "$NEXT_ARG" in
		--installdir)
			INSTALLDIR="$ARG"
			NEXT_ARG=""
			;;
		--datadir)
			DATADIR="$ARG"
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
	echo "  Package Turtle Arena Win32 NSIS installer for release"
	echo "    and copies binary into installdir"
	echo ""
	echo "  OPTIONS"
	echo " -h --help         Show this help"
	echo " -i --installdir [dir]  directory where to put installer"
	echo "                          (default: \"install\")"
	echo " -d --datadir [dir]  directory where \"base/assets0.pk3\" is located"
	echo "                          (default: \"install\")"
	exit 1
fi


#
# Build Win32 NSIS installer
#

# Build assets0.pk3 if not already built
if [ ! -f $DATADIR/base/assets0.pk3 ]
then
	./package-assets.sh --installdir $DATADIR

	echo "Go run Turtle Arena and update the checksum for assets0.pk3 if"
	echo "    needed, near the top of engine/code/qcommon/files.c!"
	exit 1
fi

# ZTM: TODO: Automaticly copy the needed DLLs to engine/misc/nsis/ (These are not kept in subversion currently...)
if [ ! -f engine/misc/nsis/SDL.dll ] || [ ! -f engine/misc/nsis/OpenAL32.dll ] || [ ! -f engine/misc/nsis/zlib1.dll ] || [ ! -f engine/misc/nsis/libcurl-4.dll ]
then
	echo "Error: You need to copy SDL.dll, OpenAL32.dll, zlib1.dll, and libcurl-4.dll"
	echo "       into engine/misc/nsis/ to build the Win32 NSIS installer"
	exit 1
fi

# Update client and server binaries
cd engine/
sh cross-make-mingw.sh ARCH=x86

# change to engine/misc/nsis/ and run make
cd misc/nsis/

# Clean up
make clean

# Build installer
make ASSETS="$STARTDIR/$DATADIR/base/assets0.pk3"

# Move to trunk
make install INSTALLDIR=$STARTDIR/$INSTALLDIR/nsis

# Don't leave it a mess
make clean

