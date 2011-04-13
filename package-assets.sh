#!/bin/bash
#
# Package Turtle Arena assets0.pk3 for release.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Check for x86_64
if [ `uname -m` = "x86_64" ]
then
	ARCH=x86_64
else
	ARCH=i386
fi

# Platform as in 'engine/build/release-linux-x86_64/base/vm'
PLATFORM=linux-$ARCH

# Whether to overwrite assets0.pk3 or not
OVERWRITE=0

# Directory to put the files for release
INSTALLDIR=install

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

	if [ "$ARG" = "--overwrite" ]
	then
		OVERWRITE=1
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
	echo "  Package Turtle Arena assets0.pk3 for release"
	echo "    and copies it into installdir"
	echo ""
	echo "  OPTIONS"
	echo "    --help         Show this help"
	echo "           -help"
	echo "           -h"
	echo "    --overwrite         Overwrite existing assets0.pk3"
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
# Check if assets0.pk3 already exists
#
if [ -f $INSTALLDIR/base/assets0.pk3 ]
then
	if [ $OVERWRITE -eq 0 ]
	then
		echo "assets0.pk3 already exists, not overwriting"
		exit 1
	fi
fi

#
# Build assets0.pk3
#
echo "  Coping base data (temporary files)..."
mkdir -p $INSTALLDIR/base_svn
cp -r base/* $INSTALLDIR/base_svn

# Remove shared libraries
rm $INSTALLDIR/base_svn/*.so
rm $INSTALLDIR/base_svn/*.dll

# Remove files sometimes used for testing
rm $INSTALLDIR/base_svn/*.pk3
rm $INSTALLDIR/base_svn/*.zip

# Copy QVMs to $INSTALLDIR/base_svn
echo "  Coping QVMs to temporary files..."
rm $INSTALLDIR/base_svn/vm/*.qvm
cp engine/build/release-$PLATFORM/base/vm/*.qvm $INSTALLDIR/base_svn/vm

# Cleanup install/base_svn directory (Remove .svn files)
echo "  Removing svn and backup files from temporary files..."

find $INSTALLDIR/base_svn/ -type d -name '.svn' -exec rm -rf '{}' \; 2>/dev/null
find $INSTALLDIR/base_svn/ -type f -name '.svnignore' -exec rm -rf '{}' \; 2>/dev/null
find $INSTALLDIR/base_svn/ -type f -name '*~' -exec rm -rf '{}' \; 2>/dev/null

# ZIP all of the files in install/base_svn into install/base/assests0.pk3
echo "  Zipping assets..."
mkdir -p $INSTALLDIR/base
if [ -f $INSTALLDIR/base/assets0.pk3 ]
then
	rm $INSTALLDIR/base/assets0.pk3
fi

cd $INSTALLDIR/base_svn/
zip -r assets0.pk3 * | grep -v "adding"
cd $STARTDIR
mv $INSTALLDIR/base_svn/assets0.pk3 $INSTALLDIR/base
cd $STARTDIR

# Remove base_svn
echo "  Removing temporary files..."
rm -r $INSTALLDIR/base_svn/

#
# You may need to update the checksum for assets0.pk3 in engine/code/qcommon/files.c
#    to play on pure servers.
#
