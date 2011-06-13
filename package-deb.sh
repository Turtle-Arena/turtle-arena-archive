#!/bin/sh
#
# Package debian source and binary packages for client/server, data, and wiimote
#    for Ubuntu PPA and ready-to-install debs.
#
# (Turtle Arena Ubuntu PPA: https://launchpad.net/~zturtleman/+archive/turtlearena-stable)
#
# NOTE: Version is gotten from changelogs
#		(debian_main/engine/debian/changelog, debian_main/data/debian/changelog,
#			and debian_main/wiimote/debian/changelog)
#
# NOTE: You must have the .orig.tar.gz files when creating
#  patches (0.2-2, 0.2-3, etc et) (You can download them from the Ubuntu PPA)
#

# data package contains the game assets used by the client and server
MAKE_DATA_DEB=1
# engine is used for client and server
MAKE_ENGINE_DEB=1
# Wiimote support for client
MAKE_WIIMOTE_DEB=1

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Directory to put the files for release
INSTALLDIR=install

# Directory containing base/assets0.pk3
DATADIR=install

DATA_DEB_CONFIG=debian_main/data/debian
ENGINE_DEB_CONFIG=debian_main/engine/debian
WIIMOTE_DEB_CONFIG=debian_main/wiimote/debian

GAMENAME="turtlearena"

# Build binary .debs, instead of just source
BIN=1

# Don't automaticly upload to the PPA
UPLOAD_TO_PPA=0

# PPA name (Only needed for --ppa)
PPA_NAME="ppa:zturtleman/turtlearena-stable"

# Key should be automatically found using name in changelog
KEY=

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


	if [ "$ARG" = "--no-data" ]
	then
		MAKE_DATA_DEB=0
		continue
	fi

	if [ "$ARG" = "--no-engine" ]
	then
		MAKE_ENGINE_DEB=0
		continue
	fi

	if [ "$ARG" = "--no-wiimote" ]
	then
		MAKE_WIIMOTE_DEB=0
		continue
	fi

	if [ "$ARG" = "--no-bin" ] || [ "$ARG" = "-n" ]
	then
		BIN=0
		continue
	fi

	if [ "$ARG" = "--ppa" ] || [ "$ARG" = "-p" ]
	then
		if [ $UPLOAD_TO_PPA -eq 0 ]
		then
			UPLOAD_TO_PPA=1
		fi
		# Optionally, specify ppa
	fi

	if [ "$ARG" = "--upload-only" ] || [ "$ARG" = "-u" ]
	then
		UPLOAD_TO_PPA=2
		continue
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

	if [ "$ARG" = "--ppa" ] || [ "$ARG" = "-p" ]
	then
		NEXT_ARG="--ppa"
		continue
	fi

	if [ "$ARG" = "--key" ] || [ "$ARG" = "-k" ]
	then
		NEXT_ARG="--key"
		continue
	fi

	case "$NEXT_ARG" in
		--installdir)
			INSTALLDIR="$ARG"
			NEXT_ARG=""
			;;
		--datadir)
			INSTALLDIR="$ARG"
			NEXT_ARG=""
			;;
		--ppa)
			PPA_NAME="$ARG"
			NEXT_ARG=""
			;;
		--key)
			KEY=-k"$ARG"
			NEXT_ARG=""
			;;
		*)
			echo "Unknown argument '$ARG'"
			USAGE=2
			NEXT_ARG=""
			;;
	esac

done

DEBINSTALL=$INSTALLDIR/deb

#
# Show usage, the user asked to see it.
#
if [ $USAGE -eq 1 ]
then
	echo "Usage: $0 OPTIONS..."
	echo "  Package Turtle Arena debian source and binary packages for release"
	echo "    into INSTALLDIR/deb"
	echo ""
	echo "  OPTIONS"
	echo " -h --help         Show this help"
	echo " -i --installdir [dir]  directory to put files"
	echo "                          (default: \"install\")"
	echo "    --datadir [dir]  directory where \"base/assets0.pk3\" is located"
	echo "                          (default: \"install\")"
	echo "    --no-data           Do not create data deb"
	echo "    --no-engine         Do not create engine debs"
	echo "    --no-wiimote        Do not create wiimote deb"
	echo " -n --no-bin            Do not build install debs, only source"
	echo " -p --ppa [ppa]         Upload source packages to [ppa]"
	echo "                          (default: \"ppa:zturtleman/turtlearena-stable\")"
	echo " -u --upload-only       Only upload source to PPA, don't build anything"
	echo " -k --key [key]         Signing key for dpkg-buildpackage"
	echo "                          (Example: \"85E7120F\")"

	exit 1
fi

# Upload only
if [ $UPLOAD_TO_PPA -eq 2 ]
then

	if [ $MAKE_DATA_DEB -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 ${DATA_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload data source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}-data_${versionAndSeries}_source.changes
	fi

	if [ $MAKE_ENGINE_DEB -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 ${ENGINE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload engine source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}_${versionAndSeries}_source.changes
	fi

	if [ $MAKE_WIIMOTE_DEB -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 ${WIIMOTE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload wiimote source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}-wiimote_${versionAndSeries}_source.changes
	fi

	exit 0
fi

if [ $MAKE_DATA_DEB -eq 1 ]
then

	#
	# Get version info from changelog
	#

	# Example: '0.2-1~maverick1' or '0.2-1'
	versionAndSeries=`head -n 1 ${DATA_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

	# Drop '~maverick1', leaves '0.2-1'
	versionWithDebVersion=`echo "${versionAndSeries}" | cut -s -f 1 -d '~'`

	# Drop '0.2', leaves '1'
	debVersion=`echo "${versionWithDebVersion}" | cut -s -f 2 -d '-'`

	# Drop '-1', leaves '0.2'
	version=`echo "${versionWithDebVersion}" | cut -s -f 1 -d '-'`

	#
	# Create orig data source directory
	#
	if [ $debVersion -eq 1 ]
	then
		ORIGDIR=$GAMENAME-data-$version.orig
	else
		ORIGDIR=$GAMENAME-data-$version
	fi

	# ZTM: Some of the music is non-commercial (CC-BY-NC and CC-BY-NC-SA)
	#        which is considered non-free by the Debian project.
	#      Should the "non-free" music be packaged in a separate deb (turtlearena-data-non-free)?
	#        (and in its own assets#.pk3?)

	# ZTM: FIXME: This script doesn't support updating assets0.pk3. It just doesn't work.
	# Build assets0.pk3 if not already built
	if [ ! -f $DATADIR/base/assets0.pk3 ]
	then
		make assets INSTALLDIR=$DATADIR

		echo "Go run Turtle Arena and update the checksum for assets0.pk3 if"
		echo "    needed, near the top of engine/code/qcommon/files.c"
		exit 1
	fi

	mkdir -p $DEBINSTALL/$ORIGDIR/base
	cp $DATADIR/base/assets*.pk3 $DEBINSTALL/$ORIGDIR/base

	cd $DEBINSTALL/$ORIGDIR

	# Copy text files into $DEBINSTALL/$ORIGDIR/ like README, CREDITS, and stuff
	cp $STARTDIR/INSTALLER_README.txt README
	cp $STARTDIR/CREDITS.txt CREDITS
	cp $STARTDIR/COPYRIGHTS.txt COPYRIGHTS
	cp $STARTDIR/CC-BY-SA-3.0.txt CC-BY-SA-3.0

	#
	# Create debian data source directory
	#
	cd $STARTDIR
	DEBDIR=$GAMENAME-data-$version
	mkdir -p $DEBINSTALL/$DEBDIR/debian/

	if [ $debVersion -eq 1 ]
	then
		cp -r $DEBINSTALL/$ORIGDIR/* $DEBINSTALL/$DEBDIR/
	fi
	cp -r $DATA_DEB_CONFIG/* $DEBINSTALL/$DEBDIR/debian/

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Upload source to PPA
		dput ${PPA_NAME} ${GAMENAME}-data_${versionAndSeries}_source.changes
	fi

	if [ $BIN -eq 1 ]
	then
		# build .deb
		dpkg-buildpackage -rfakeroot -uc -b $KEY
	fi
fi

if [ $MAKE_ENGINE_DEB -eq 1 ]
then

	#
	# Change to starting directory
	#
	cd $STARTDIR

	#
	# Get version info from changelog
	#

	# Example: '0.2-1~maverick1' or '0.2-1'
	versionAndSeries=`head -n 1 ${ENGINE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

	# Drop '~maverick1', leaves '0.2-1'
	versionWithDebVersion=`echo "${versionAndSeries}" | cut -s -f 1 -d '~'`

	# Drop '0.2', leaves '1'
	debVersion=`echo "${versionWithDebVersion}" | cut -s -f 2 -d '-'`

	# Drop '-1', leaves '0.2'
	version=`echo "${versionWithDebVersion}" | cut -s -f 1 -d '-'`

	#
	# Create orig engine source directory
	#
	if [ $debVersion -eq 1 ]
	then
		ORIGDIR=$GAMENAME-$version.orig
	else
		ORIGDIR=$GAMENAME-$version
	fi
	mkdir -p $DEBINSTALL/$ORIGDIR

	# Avoid copying build directory.
	mkdir $DEBINSTALL/$ORIGDIR/code
	mkdir $DEBINSTALL/$ORIGDIR/misc
	mkdir $DEBINSTALL/$ORIGDIR/ui
	cp engine/* $DEBINSTALL/$ORIGDIR/
	cp -r engine/code/* $DEBINSTALL/$ORIGDIR/code
	cp -r engine/misc/* $DEBINSTALL/$ORIGDIR/misc
	cp -r engine/ui/* $DEBINSTALL/$ORIGDIR/ui

	#
	# File cleanup
	#
	# Remove SVN and backup files
	find $DEBINSTALL/$ORIGDIR/ -type d -name '.svn' -exec rm -rf '{}' \; 2>/dev/null
	find $DEBINSTALL/$ORIGDIR/ -type f -name '.svnignore' -exec rm -rf '{}' \; 2>/dev/null
	find $DEBINSTALL/$ORIGDIR/ -type f -name '*~' -exec rm -rf '{}' \; 2>/dev/null

	# Remove files Code::Blocks creates
	find $DEBINSTALL/$ORIGDIR/misc/code_blocks/ -type f -name '*.layout' -exec rm -rf '{}' \; 2>/dev/null
	find $DEBINSTALL/$ORIGDIR/misc/code_blocks/ -type f -name '*.depend' -exec rm -rf '{}' \; 2>/dev/null

	# Remove lcc as Debian calls it non-free and removes it
	rm -r $DEBINSTALL/$ORIGDIR/code/tools/lcc

	# Remove non-free Windows header
	if [ -f $DEBINSTALL/$ORIGDIR/code/qcommon/wspiapi.h ]
	then
		rm $DEBINSTALL/$ORIGDIR/code/qcommon/wspiapi.h
	fi

	# Remove libs for win32 and mac
	rm -r $DEBINSTALL/$ORIGDIR/code/libs/

	#
	# Create orig.tar.gz
	#	So that dpkg-buildpackage will make a .debian.tar.gz instead of
	#		failing to make .diff.gz
	#
	if [ $debVersion -eq 1 ]
	then
		cd $DEBINSTALL
		tar -pczf ${GAMENAME}_$version.orig.tar.gz $ORIGDIR
	fi

	#
	# Create debian engine source directory
	#
	cd $STARTDIR
	DEBDIR=$GAMENAME-$version
	mkdir -p $DEBINSTALL/$DEBDIR/debian/

	if [ $debVersion -eq 1 ]
	then
		cp -r $DEBINSTALL/$ORIGDIR/* $DEBINSTALL/$DEBDIR/
	fi
	cp -r $ENGINE_DEB_CONFIG/* $DEBINSTALL/$DEBDIR/debian/

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Upload source to PPA
		dput ${PPA_NAME} ${GAMENAME}_${versionAndSeries}_source.changes
	fi

	if [ $BIN -eq 1 ]
	then
		# build .deb
		dpkg-buildpackage -rfakeroot -uc -b $KEY
		rm -r build
	fi
fi

if [ $MAKE_WIIMOTE_DEB -eq 1 ]
then

	#
	# Change to starting directory
	#
	cd $STARTDIR

	#
	# Get version info from changelog
	#

	# Example: '0.2-1~maverick1' or '0.2-1'
	versionAndSeries=`head -n 1 ${WIIMOTE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

	# Drop '~maverick1', leaves '0.2-1'
	versionWithDebVersion=`echo "${versionAndSeries}" | cut -s -f 1 -d '~'`

	# Drop '0.2', leaves '1'
	debVersion=`echo "${versionWithDebVersion}" | cut -s -f 2 -d '-'`

	# Drop '-1', leaves '0.2'
	version=`echo "${versionWithDebVersion}" | cut -s -f 1 -d '-'`

	#
	# Create orig wiimote source directory
	#
	if [ $debVersion -eq 1 ]
	then
		ORIGDIR=$GAMENAME-wiimote-$version.orig
	else
		ORIGDIR=$GAMENAME-wiimote-$version
	fi
	mkdir -p $DEBINSTALL/$ORIGDIR

	# Copy files
	cp extras/wminput/config/* $DEBINSTALL/$ORIGDIR/
	cp extras/wminput/launchers/* $DEBINSTALL/$ORIGDIR/
	cp extras/wminput/README $DEBINSTALL/$ORIGDIR/

	#
	# Create orig.tar.gz
	#	So that dpkg-buildpackage will make a .debian.tar.gz instead of
	#		failing to make .diff.gz
	#
	if [ $debVersion -eq 1 ]
	then
		cd $DEBINSTALL
		tar -pczf ${GAMENAME}-wiimote_$version.orig.tar.gz $ORIGDIR
	fi

	#
	# Create debian wiimote source directory
	#
	cd $STARTDIR
	DEBDIR=${GAMENAME}-wiimote-$version
	mkdir -p $DEBINSTALL/$DEBDIR/debian/

	if [ $debVersion -eq 1 ]
	then
		cp -r $DEBINSTALL/$ORIGDIR/* $DEBINSTALL/$DEBDIR/
	fi
	cp -r $WIIMOTE_DEB_CONFIG/* $DEBINSTALL/$DEBDIR/debian/

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Upload source to PPA
		dput ${PPA_NAME} ${GAMENAME}-wiimote_${versionAndSeries}_source.changes
	fi

	if [ $BIN -eq 1 ]
	then
		# build .deb
		dpkg-buildpackage -rfakeroot -uc -b $KEY
	fi
fi
