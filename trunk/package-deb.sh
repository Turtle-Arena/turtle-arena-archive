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
#  patches (0.2-2, 0.2-3, etc) (You can download them from the Ubuntu PPA)
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

# Set to -sd for no *orig.tar.gz uploading or -sa for uploading
ORIGTAR=-sa

# Ubuntu distribution
SERIES=lucid

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

	if [ "$ARG" = "--no-tar" ] || [ "$ARG" = "-t" ]
	then
		ORIGTAR=-sd
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

	if [ "$ARG" = "--series" ] || [ "$ARG" = "-s" ]
	then
		NEXT_ARG="--series"
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
			DATADIR="$ARG"
			NEXT_ARG=""
			;;
		--series)
			SERIES="$ARG"
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

DEBINSTALL=$INSTALLDIR/deb/$SERIES

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
	echo " -d --datadir [dir]  directory where \"base/assets0.pk3\" is located"
	echo "                          (default: \"install\")"
	echo "    --no-data           Do not create data deb"
	echo "    --no-engine         Do not create engine debs"
	echo "    --no-wiimote        Do not create wiimote deb"
	echo " -n --no-bin            Do not build install debs, only source"
	echo " -s --series [dist]     Set Ubuntu distribution series"
	echo "                          (default: \"lucid\")"
	echo " -p --ppa [ppa]         Upload source packages to [ppa]"
	echo "                          (default: \"ppa:zturtleman/turtlearena-stable\")"
	echo " -u --upload-only       Only upload source to PPA, don't build anything"
	echo " -t --no-tar            Do not include *.orig.tar.gz in source upload"
	echo " -k --key [key]         Signing key for dpkg-buildpackage"
	echo "                          (Example: \"85E7120F\")"

	exit 1
fi

if [ $USAGE -eq 2 ]
then
	echo "Invalid usage, use $0 -h for help."
	exit 1
fi

# Upload only
if [ $UPLOAD_TO_PPA -eq 2 ]
then

	if [ $MAKE_DATA_DEB -eq 1 ]
	then
		# Get version from main changelog. Example: 0.4-1
		versionWithDebVersion=`head -n 1 ${DATA_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')' | cut -s -f 1 -d '~'`

		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		version=`head -n 1 ${GAMENAME}-data-${versionWithDebVersion}/debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload data source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}-data_${version}_source.changes
	fi

	if [ $MAKE_ENGINE_DEB -eq 1 ]
	then
		# Get version from main changelog. Example: 0.4-1
		versionWithDebVersion=`head -n 1 ${ENGINE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')' | cut -s -f 1 -d '~'`

		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		version=`head -n 1 ${GAMENAME}-${versionWithDebVersion}/debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload engine source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}_${version}_source.changes
	fi

	if [ $MAKE_WIIMOTE_DEB -eq 1 ]
	then
		# Get version from main changelog. Example: 0.4-1
		versionWithDebVersion=`head -n 1 ${WIIMOTE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')' | cut -s -f 1 -d '~'`

		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		version=`head -n 1 ${GAMENAME}-wiimote-${versionWithDebVersion}/debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload wiimote source to PPA
		dput ${PPA_NAME} $DEBINSTALL/${GAMENAME}-wiimote_${version}_source.changes
	fi

	exit 0
fi

#
# Get data version info from changelog
#

# Example: '0.2-1~maverick1' or '0.2-1'
dataVersionAndSeries=`head -n 1 ${DATA_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

# Drop '~maverick1', leaves '0.2-1'
dataVersionWithDebVersion=`echo "${dataVersionAndSeries}" | cut -s -f 1 -d '~'`

# Drop '0.2', leaves '1'
dataDebVersion=`echo "${dataVersionWithDebVersion}" | cut -s -f 2 -d '-'`

# Drop '-1', leaves '0.2'
dataVersion=`echo "${dataVersionWithDebVersion}" | cut -s -f 1 -d '-'`

#
# Get engine version info from changelog
#

# Example: '0.2-1~maverick1' or '0.2-1'
engineVersionAndSeries=`head -n 1 ${ENGINE_DEB_CONFIG}/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

# Drop '~maverick1', leaves '0.2-1'
engineVersionWithDebVersion=`echo "${engineVersionAndSeries}" | cut -s -f 1 -d '~'`

# Drop '0.2', leaves '1'
engineDebVersion=`echo "${engineVersionWithDebVersion}" | cut -s -f 2 -d '-'`

# Drop '-1', leaves '0.2'
engineVersion=`echo "${engineVersionWithDebVersion}" | cut -s -f 1 -d '-'`

if [ $MAKE_DATA_DEB -eq 1 ]
then

	#
	# Set version
	#
	versionAndSeries=$dataVersionAndSeries
	versionWithDebVersion=$dataVersionWithDebVersion
	debVersion=$dataDebVersion
	version=$dataVersion

	#
	# Create orig data source directory
	#
	if [ $debVersion -eq 1 ]
	then
		ORIGDIR=$GAMENAME-data-$version.orig
	else
		ORIGDIR=$GAMENAME-data-$version
	fi

	# ZTM: FIXME: This script doesn't support creating updated deb with new assets. It just doesn't work.
	# Build assets if not already built
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
	sed -e 's/ENGINE_VERSION/'${engineVersion}'/g;' -i $DEBINSTALL/$DEBDIR/debian/control
	sed -e 's/lucid/'${SERIES}'/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	if [ ${SERIES} != 'lucid' ]
	then
		sed -e 's/(0.2-1)/(0.2-1~'${SERIES}'1)/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	fi

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $ORIGTAR $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload source to PPA
		dput ${PPA_NAME} ../${GAMENAME}-data_${versionAndSeries}_source.changes
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
	# Set version
	#
	versionAndSeries=$engineVersionAndSeries
	versionWithDebVersion=$engineVersionWithDebVersion
	debVersion=$engineDebVersion
	version=$engineVersion

	#
	# Create orig engine source directory
	#
	if [ $debVersion -eq 1 ]
	then
		ORIGDIR=$GAMENAME-$version.orig
	else
		ORIGDIR=$GAMENAME-$version
	fi

	# Avoid copying build directory.
	if [ -d .svn ]
	then
		mkdir -p $DEBINSTALL
		svn export engine $DEBINSTALL/$ORIGDIR/
	else
		mkdir -p $DEBINSTALL/$ORIGDIR
		mkdir $DEBINSTALL/$ORIGDIR/code
		mkdir $DEBINSTALL/$ORIGDIR/misc
		mkdir $DEBINSTALL/$ORIGDIR/ui
		cp engine/* $DEBINSTALL/$ORIGDIR/
		cp -r engine/code/* $DEBINSTALL/$ORIGDIR/code
		cp -r engine/misc/* $DEBINSTALL/$ORIGDIR/misc
		cp -r engine/ui/* $DEBINSTALL/$ORIGDIR/ui
	fi

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
	rm -rf $DEBINSTALL/$ORIGDIR/code/tools/lcc

	# Remove non-free Windows header
	if [ -f $DEBINSTALL/$ORIGDIR/code/qcommon/wspiapi.h ]
	then
		rm $DEBINSTALL/$ORIGDIR/code/qcommon/wspiapi.h
	fi

	# Remove libs for win32 and mac
	rm -rf $DEBINSTALL/$ORIGDIR/code/libs/

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
	sed -e 's/DATA_VERSION/'${dataVersion}'/g;' -i $DEBINSTALL/$DEBDIR/debian/control
	sed -e 's/lucid/'${SERIES}'/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	if [ ${SERIES} != 'lucid' ]
	then
		sed -e 's/(0.2-1)/(0.2-1~'${SERIES}'1)/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	fi

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $ORIGTAR $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload source to PPA
		dput ${PPA_NAME} ../${GAMENAME}_${versionAndSeries}_source.changes
	fi

	if [ $BIN -eq 1 ]
	then
		# build .deb
		dpkg-buildpackage -rfakeroot -uc -b $KEY
		rm -rf build
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
	rm -rf $DEBINSTALL/$DEBDIR/debian/.svn
	sed -e 's/lucid/'${SERIES}'/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	if [ ${SERIES} != 'lucid' ]
	then
		sed -e 's/(0.2-1)/(0.2-1~'${SERIES}'1)/g;' -i $DEBINSTALL/$DEBDIR/debian/changelog
	fi

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $DEBINSTALL/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $ORIGTAR $KEY

	if [ $UPLOAD_TO_PPA -eq 1 ]
	then
		# Get version (w/series) from changelog. Example: 0.4-1~maverick1 or 0.4-1
		versionAndSeries=`head -n 1 debian/changelog | cut -s -f 2 -d '(' | cut -s -f 1 -d ')'`

		# Upload source to PPA
		dput ${PPA_NAME} ../${GAMENAME}-wiimote_${versionAndSeries}_source.changes
	fi

	if [ $BIN -eq 1 ]
	then
		# build .deb
		dpkg-buildpackage -rfakeroot -uc -b $KEY
	fi
fi
