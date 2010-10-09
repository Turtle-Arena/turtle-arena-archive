#!/bin/sh
#
# Package debian source and binary packages for client/server and data,
#    for Ubuntu PPA and ready install.
#
# (Turtle Arena Ubuntu PPA: https://launchpad.net/~zturtleman/+archive/turtlearena-stable)
#
# NOTE: You must have the .orig.tar.gz files when creating
#  patches (0.2-2, 0.2-3, etc et) (You can download them from the Ubuntu PPA)

# FIXME: Doesn't find secrete key automatically on my computer...
#        If you are not Zack Middleton change/remove this
KEY=-k587E1968
#KEY=

MAKE_DATA_DEB=1
# engine is used for client and server
MAKE_ENGINE_DEB=1

STARTDIR=`pwd`
INSTALLDIR=debian_source
DATA_DEB_CONFIG=debian_main/data/debian
ENGINE_DEB_CONFIG=debian_main/engine/debian

GAMENAME="turtlearena"

#
# !!Must update debian_main if version, or deb_version if changed!!
#
# You must update the changelogs (debian_main/engine/debian/changelog and debian_main/data/debian/changelog)
# Optionally increase data requirement for engine in debian_main/engine/debian/control
#

# Version (Current Turtle Arena version)
VERSION=0.3
# For debian only fixes/changes update DEB_VERSION "turtlearena_$VERSION-$DATA_DEB_VERSION"
DATA_DEB_VERSION=1
ENGINE_DEB_VERSION=1

if [ $MAKE_DATA_DEB -eq 1 ]
then

	#
	# Create orig data source directory
	#
	if [ $DATA_DEB_VERSION -eq 1 ]
	then
		ORIGDIR=$GAMENAME-data-$VERSION.orig
	else
		ORIGDIR=$GAMENAME-data-$VERSION
	fi
	mkdir -p $INSTALLDIR/$ORIGDIR

	# FIXME: This script doesn't support updating assets0.pk3. It just doesn't work.
	# Build assets0.pk3 if not already built
	if [ ! -f $STARTDIR/install/base/assets0.pk3 ]
	then
		./package-assets.sh
	fi

	mkdir -p $INSTALLDIR/$ORIGDIR/base
	cp $STARTDIR/install/base/assets0.pk3 $INSTALLDIR/$ORIGDIR/base

	cd $INSTALLDIR/$ORIGDIR

	# Copy text files into $INSTALLDIR/$ORIGDIR/ like README, CREDITS, and stuff
	cp $STARTDIR/GAME_README.txt README
	cp $STARTDIR/CREDITS.txt CREDITS
	cp $STARTDIR/COPYRIGHTS.txt COPYRIGHTS
	cp $STARTDIR/COPYING.txt COPYING

	#
	# Create debian data source directory
	#
	cd $STARTDIR
	DEBDIR=$GAMENAME-data-$VERSION
	mkdir -p $INSTALLDIR/$DEBDIR/debian/

	if [ $DATA_DEB_VERSION -eq 1 ]
	then
		cp -r $INSTALLDIR/$ORIGDIR/* $INSTALLDIR/$DEBDIR/
	fi
	cp -r $DATA_DEB_CONFIG/* $INSTALLDIR/$DEBDIR/debian/

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $INSTALLDIR/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $KEY

	# Upload source to PPA
	# dput ppa:zturtleman/turtlearena turtlearena-data_0.2-1_source.changes

	# build .deb
	cd $GAMENAME-data-$VERSION
	dpkg-buildpackage -rfakeroot -uc -b
fi

if [ $MAKE_ENGINE_DEB -eq 1 ]
then

	#
	# Change to starting directory
	#
	cd $STARTDIR

	#
	# Create orig engine source directory
	#
	if [ $ENGINE_DEB_VERSION -eq 1 ]
	then
		ORIGDIR=$GAMENAME-$VERSION.orig
	else
		ORIGDIR=$GAMENAME-$VERSION
	fi
	mkdir -p $INSTALLDIR/$ORIGDIR

	# Avoid copying build directory.
	mkdir $INSTALLDIR/$ORIGDIR/code
	mkdir $INSTALLDIR/$ORIGDIR/misc
	mkdir $INSTALLDIR/$ORIGDIR/ui
	cp engine/* $INSTALLDIR/$ORIGDIR/
	cp -r engine/code/* $INSTALLDIR/$ORIGDIR/code
	cp -r engine/misc/* $INSTALLDIR/$ORIGDIR/misc
	cp -r engine/ui/* $INSTALLDIR/$ORIGDIR/ui

	#
	# File cleanup
	#
	# Remove SVN files
	find $INSTALLDIR/$ORIGDIR/ -type d -name '.svn' -exec rm -rf '{}' \; 2>/dev/null
	find $INSTALLDIR/$ORIGDIR/ -type f -name '.svnignore' -exec rm -rf '{}' \; 2>/dev/null
	find $INSTALLDIR/$ORIGDIR/ -type f -name '*~' -exec rm -rf '{}' \; 2>/dev/null

	# Remave lcc as debian calls it non-free and removes it
	rm -r $INSTALLDIR/$ORIGDIR/code/tools/lcc

	# Remove non-free header
	if [ -f $INSTALLDIR/$ORIGDIR/code/qcommon/wspiapi.h ]
	then
		rm $INSTALLDIR/$ORIGDIR/code/qcommon/wspiapi.h
	fi

	#
	# Create orig.tar.gz
	#	So that dpkg-buildpackage will make a .debian.tar.gz instead of
	#		failing to make .diff.gz
	#
	if [ $ENGINE_DEB_VERSION -eq 1 ]
	then
		cd $INSTALLDIR
		tar -pczf ${GAMENAME}_$VERSION.orig.tar.gz $ORIGDIR
	fi

	#
	# Create debian engine source directory
	#
	cd $STARTDIR
	DEBDIR=$GAMENAME-$VERSION
	mkdir -p $INSTALLDIR/$DEBDIR/debian/

	if [ $ENGINE_DEB_VERSION -eq 1 ]
	then
		cp -r $INSTALLDIR/$ORIGDIR/* $INSTALLDIR/$DEBDIR/
	fi
	cp -r $ENGINE_DEB_CONFIG/* $INSTALLDIR/$DEBDIR/debian/

	#
	# Build debian package data
	#
	cd $STARTDIR
	cd $INSTALLDIR/$DEBDIR

	dpkg-buildpackage -S -rfakeroot $KEY

	# Upload source to PPA
	# dput ppa:zturtleman/turtlearena turtlearena_0.2-1_source.changes

	# build .deb
	cd $GAMENAME-$VERSION
	dpkg-buildpackage -rfakeroot -uc -b
	rm -r build
fi

