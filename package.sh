#!/bin/sh
#
# Package TMNT Arena for release.
#
#  Supports x86 and x86_64 GNU/Linux.
#

# Directory to put the files for release
INSTALLDIR=install

# Make and copy linux x86 and if possible x86_64
LINUX=1

# Cross-make and copy mingw32 client and dedicated server
WIN32=1

# Copy and zip the data
DATAFILES=1

# Readme, COPYING, etc
DOCFILES=1



# Check for x86_64
if [ `uname -m` = "x86_64" ]
then
	ARCH=x86_64
else
	ARCH=x86
fi

# Platform as in 'engine/build/release-linux-x86_64/base/vm'
PLATFORM=linux-$ARCH

#
# Make sure everything is built
#

if [ $LINUX -eq 1 ] || [ $WIN32 -eq 1 ]
then

	cd engine

	if [ $LINUX -eq 1 ]
	then
		make BUILD_GAME_SO=0 BUILD_GAME_QVM=1

		# If running x86_64, compile x86 too.
		if [ $ARCH = "x86_64" ]
		then
			make ARCH=x86 BUILD_GAME_SO=0
		fi
	else
		# We need to build QVMs
		make BUILD_CLIENT=0 BUILD_CLIENT_SMP=0 BUILD_SERVER=0 BUILD_GAME_SO=0 BUILD_GAME_QVM=1
	fi

	if [ $WIN32 -eq 1 ]
	then
		sh cross-make-mingw.sh BUILD_GAME_SO=0
	fi

	cd ..

fi

#
# Create install directory
#

if [ ! -d $INSTALLDIR ]
then
	mkdir $INSTALLDIR
fi

#
# Copy client and server binarys
#

if [ $LINUX -eq 1 ] || [ $WIN32 -eq 1 ]
then

	echo "Coping binarys..."

	if [ $LINUX -eq 1 ]
	then
		cp engine/build/release-$PLATFORM/tmntarena.$ARCH $INSTALLDIR
		cp engine/build/release-$PLATFORM/tmntarena-ded.$ARCH $INSTALLDIR

		if [ $ARCH = "x86_64" ]
		then
			cp engine/build/release-linux-x86/tmntarena.x86 $INSTALLDIR
			cp engine/build/release-linux-x86/tmntarena-ded.x86 $INSTALLDIR
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		cp engine/build/release-mingw32-x86/tmntarena.x86.exe $INSTALLDIR
		cp engine/build/release-mingw32-x86/tmntarena-ded.x86.exe $INSTALLDIR

		echo "  Warning: You need to manually copy SDL.dll vserion 1.2.14 into $INSTALLDIR !"
	fi

	# Copy linux launcher too
	cp tmntarena.sh $INSTALLDIR

fi


#
# Copy readme, COPYING, etc
#

if [ $DOCFILES -eq 1 ]
then
	echo "Copying docs..."

	cp tmnt-readme.txt $INSTALLDIR/readme.txt
	cp COPYING.txt $INSTALLDIR
	cp COPYRIGHTS.txt $INSTALLDIR
	echo "TMNT Arena Alpha `date +%Y%m%d`" > $INSTALLDIR/version.txt

fi


#
# Copy base to install/base_svn
#

if [ $DATAFILES -eq 1 ]
then
	echo "Data..."

	echo "  Coping base data (temp files)..."
	if [ ! -d $INSTALLDIR/base_svn ]
	then
		mkdir $INSTALLDIR/base_svn
	fi
	cp -r base/* $INSTALLDIR/base_svn

	# Copy QVMs to $INSTALLDIR/base_svn
	echo "  Coping QVMs..."
	rm $INSTALLDIR/base_svn/vm/*.qvm
	cp engine/build/release-$PLATFORM/base/vm/*.qvm $INSTALLDIR/base_svn/vm

	# Cleanup install/base_svn directory (Remove .svn files)
	echo "  Removing .svn directories and backup files..."

	find $INSTALLDIR/base_svn/ -type d -name '.svn' -exec rm -rf '{}' \; 2>/dev/null
	find $INSTALLDIR/base_svn/ -type f -name '*~' -exec rm -rf '{}' \; 2>/dev/null

	# ZIP all of the files in install/base_svn into install/base/assests0.pk3
	echo "  Zipping assets..."
	if [ ! -d $INSTALLDIR/base ]
	then
		mkdir $INSTALLDIR/base
	fi
	rm $INSTALLDIR/base/assests0.pk3

	cd $INSTALLDIR/base_svn/
	zip -r ../../$INSTALLDIR/base/assests0.pk3 * | grep -v "adding"
	cd ../../

	# Remove base_svn
	echo "  Removing temp files..."
	rm -r $INSTALLDIR/base_svn/

fi

# zip install?

echo "Done!"

