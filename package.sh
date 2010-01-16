#!/bin/bash
#
# Package TMNT Arena for release.
#
#  Supports Zip and Deb releases.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Directory to put the files for release
INSTALLDIR=install

# Version (Cuurently YYYYMMDD)
VERSION=`date +%Y%m%d`


# Package Linux binaries
LINUX=1

# Create assets0.pk3
DATAFILES=1

# TODO: Zip the files? currently they have to be zipped /after/ running the script.
MAKEZIP=1

	# Package win32 binaries for zip install
	WIN32=1


# Make .deb package for Debian based systems
MAKEDEB=1

	# Game name for deb package
	GAMENAME="tmntarena"

	# Long Title
	LONGTITLE="Third-person arena game"

	# Description
	DESC="TMNT Arena is a Ninja Turtle fangame based on ioquake3."

	# Packager's name and email
	# Example: "John Smith <address@example.com>"
	NAME_AND_EMAIL="Zack Middleton <ZTurtleMan@gmail.com>"


# Check for x86_64
if [ `uname -m` = "x86_64" ]
then
	ARCH=x86_64
else
	ARCH=i386
fi

# Platform as in 'engine/build/release-linux-x86_64/base/vm'
PLATFORM=linux-$ARCH

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
		make ARCH=i386
	fi
else
	# We need to at least build the QVMs!
	make BUILD_CLIENT=0 BUILD_CLIENT_SMP=0 BUILD_SERVER=0 BUILD_GAME_SO=0 BUILD_GAME_QVM=1
fi

if [ $WIN32 -eq 1 ]
then
	sh cross-make-mingw.sh BUILD_GAME_SO=0
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
		cp engine/build/release-$PLATFORM/tmntarena.$ARCH $INSTALLDIR
		cp engine/build/release-$PLATFORM/tmntarena-ded.$ARCH $INSTALLDIR

		if [ $ARCH = "x86_64" ]
		then
			cp engine/build/release-linux-i386/tmntarena.i386 $INSTALLDIR
			cp engine/build/release-linux-i386/tmntarena-ded.i386 $INSTALLDIR
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		cp engine/build/release-mingw32-x86/tmntarena.x86.exe $INSTALLDIR
		cp engine/build/release-mingw32-x86/tmntarena-ded.x86.exe $INSTALLDIR

		echo "  Warning: You need to manually copy SDL.dll vserion 1.2.14 into $INSTALLDIR !"
	fi

fi


#
# Create assets0.pk3
#

if [ $DATAFILES -eq 1 ]
then
	echo "Data..."

	echo "  Coping base data (temp files)..."
	mkdir -p $INSTALLDIR/base_svn
	cp -r base/* $INSTALLDIR/base_svn

	# Copy QVMs to $INSTALLDIR/base_svn
	echo "  Coping QVMs..."
	rm $INSTALLDIR/base_svn/vm/*.qvm
	cp engine/build/release-$PLATFORM/base/vm/*.qvm $INSTALLDIR/base_svn/vm

	# Cleanup install/base_svn directory (Remove .svn files)
	echo "  Removing svn and backup files..."

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
	zip -r ../../$INSTALLDIR/base/assets0.pk3 * | grep -v "adding"
	cd $STARTDIR

	# Remove base_svn
	echo "  Removing temp files..."
	rm -r $INSTALLDIR/base_svn/

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
		cp tmntarena.sh $INSTALLDIR
	fi

	cp tmnt-readme.txt $INSTALLDIR/readme.txt
	cp COPYING.txt $INSTALLDIR
	cp COPYRIGHTS.txt $INSTALLDIR

	# write version file
	echo "TMNT Arena Alpha $VERSION" > $INSTALLDIR/version.txt
	#echo "Packaged by: $NAME_AND_EMAIL" >> $INSTALLDIR/version.txt

	# Copy all of the files other then base/ into tmntarena-src/ and zip it.
	echo "Warning: You need to manually copy the source into $INSTALLDIR !"

	# zip install?

fi


# Set $DEBARCH before calling
function make_client_deb() {
	mkdir -p $INSTALLDIR/deb-client/usr/games
	mkdir -p $INSTALLDIR/deb-client/usr/share/doc/$GAMENAME
	mkdir -p $INSTALLDIR/deb-client/usr/share/applications
	mkdir -p $INSTALLDIR/deb-client/usr/share/icons/hicolor/256x256/apps/
	mkdir -p $INSTALLDIR/deb-client/usr/share/menu
	mkdir -p $INSTALLDIR/deb-client/usr/share/pixmaps

	# copy the bindary file
	if [ $DEBARCH = "amd64" ]
	then
		EXENAME="$GAMENAME.x86_64"
	else
		EXENAME="$GAMENAME.i386"
	fi
	cp $INSTALLDIR/$EXENAME $INSTALLDIR/deb-client/usr/games/$GAMENAME

	# Copy the icons
	cp engine/misc/quake3-tango.png $INSTALLDIR/deb-client/usr/share/icons/hicolor/256x256/apps/${GAMENAME}256.png
	cp engine/misc/quake3.png $INSTALLDIR/deb-client/usr/share/pixmaps/${GAMENAME}32.png

	cd $INSTALLDIR/deb-client/

	# write the README file
	echo "This package contains TMNT Arena client." > usr/share/doc/$GAMENAME/README

	# write the tmntarena menu file
	echo "?package($GAMENAME): \\" > usr/share/menu/$GAMENAME
	echo "\tneeds=\"X11\" \\" >> usr/share/menu/$GAMENAME
	echo "\tsection=\"Games/Action\" \\" >> usr/share/menu/$GAMENAME
	echo "\ttitle=\"TMNT Arena\" \\" >> usr/share/menu/$GAMENAME
	echo "\tlongtitle=\"$LONGTITLE\" \\" >> usr/share/menu/$GAMENAME
	echo "\ticon=\"/usr/share/pixmaps/${GAMENAME}32.png\" \\" >> usr/share/menu/$GAMENAME
	echo "\tcommand=\"/usr/games/tmntarena\"" >> usr/share/menu/$GAMENAME

	# write the tmntarena.desktop file
	echo "[Desktop Entry]" > usr/share/applications/$GAMENAME.desktop
	echo "Name=TMNT Arena" >> usr/share/applications/$GAMENAME.desktop
	echo "Comment=$LONGTITLE" >> usr/share/applications/$GAMENAME.desktop
	echo "Exec=$GAMENAME" >> usr/share/applications/$GAMENAME.desktop
	echo "Terminal=false" >> usr/share/applications/$GAMENAME.desktop
	echo "Icon=/usr/share/icons/hicolor/256x256/apps/${GAMENAME}256.png" >> usr/share/applications/$GAMENAME.desktop
	echo "Type=Application" >> usr/share/applications/$GAMENAME.desktop
	echo "Categories=Game;ArcadeGame;" >> usr/share/applications/$GAMENAME.desktop

	# write the control file
	echo "Package: $GAMENAME" > control
	echo "Version: $VERSION-1" >> control
	echo "Section: games" >> control
	echo "Priority: optional" >> control
	echo "Architecture: $DEBARCH" >> control
	echo "Installed-Size: `du -ks usr|cut -f 1`" >> control
	# Depends taken from openarena_0.8.1-4_amd64.deb
	echo "Depends: libc6 (>= 2.4), libcurl3-gnutls (>= 7.16.2-1), libgl1-mesa-glx | libgl1, libogg0 (>= 1.0rc3), libopenal1, libsdl1.2debian (>= 1.2.10-1), libspeex1 (>= 1.2~beta3-1), libspeexdsp1 (>= 1.2~beta3.2-1), libvorbis0a (>= 1.1.2), libvorbisfile3 (>= 1.1.2), tmntarena-data (>= $VERSION)" >> control
	echo "Maintainer: $NAME_AND_EMAIL" >> control
	echo "Homepage: http://turtlearena.googlecode.com/" >> control
	echo "Description: $LONGTITLE" >> control
	echo " $DESC" >> control

	# write the postinst file
	echo "#!/bin/sh" > postinst
	echo "set -e" >> postinst
	echo "# Automatically added by dh_installmenu" >> postinst
	echo "if [ "$1" = "configure" ] && [ -x "`which update-menus 2>/dev/null`" ]; then" >> postinst
	echo "	update-menus" >> postinst
	echo "fi" >> postinst
	echo "# End automatically added section" >> postinst

	# write the postrm file
	echo "#!/bin/sh" > postrm
	echo "set -e" >> postrm
	echo "# Automatically added by dh_installmenu" >> postrm
	echo "if [ -x "`which update-menus 2>/dev/null`" ]; then update-menus ; fi" >> postrm
	echo "# End automatically added section" >> postrm

	tar czf data.tar.gz usr/*

	# md5sums?
	tar czf control.tar.gz control postinst postrm

	# write the debian-binary file
	echo "2.0" > debian-binary

	if [ -f ${GAMENAME}_$VERSION-1_$DEBARCH.deb ]
	then
		rm ${GAMENAME}_$VERSION-1_$DEBARCH.deb
	fi

	ar -r ${GAMENAME}_$VERSION-1_$DEBARCH.deb debian-binary control.tar.gz data.tar.gz

	mv ${GAMENAME}_$VERSION-1_$DEBARCH.deb ..

	cd $STARTDIR

	rm -r $INSTALLDIR/deb-client/
}


# NOTE: I have made a change from the OpenArena client
#  * client binary is in /usr/games/ instead of /usr/games/tmntarena/
#
# https://synthesize.us/HOWTO_make_a_deb_archive_without_dpkg
if [ $MAKEDEB -eq 1 ]
then

	if [ "$NAME_AND_EMAIL" = "" ]
	then
		echo "Warning: NAME_AND_EMAIL is unset for deb!"
	fi

	if [ $DATAFILES -eq 1 ]
	then
		# Make data package
		echo "Making data deb file..."

		mkdir -p $INSTALLDIR/deb-data/usr/share/games/$GAMENAME/base
		mkdir -p $INSTALLDIR/deb-data/usr/share/doc/$GAMENAME-data

		# copy the data file
		cp $INSTALLDIR/base/assets0.pk3 $INSTALLDIR/deb-data/usr/share/games/$GAMENAME/base

		cd $INSTALLDIR/deb-data/

		# write the README file
		echo "This package contains the required data for TMNT Arena." > usr/share/doc/$GAMENAME-data/README

		# write the control file
		echo "Package: $GAMENAME-data" > control
		echo "Version: $VERSION-1" >> control
		echo "Section: games" >> control
		echo "Priority: optional" >> control
		echo "Architecture: all" >> control
		echo "Installed-Size: `du -ks usr|cut -f 1`" >> control
		echo "Maintainer: $NAME_AND_EMAIL" >> control
		echo "Homepage: http://turtlearena.googlecode.com/" >> control
		echo "Description: TMNT Arena data files" >> control
		echo " TMNT Arena requires this in order to run." >> control

		tar czf data.tar.gz usr/*

		tar czf control.tar.gz control

		# write the debian-binary file
		echo "2.0" > debian-binary

		if [ -f ${GAMENAME}-data_$VERSION-1_all.deb ]
		then
			rm ${GAMENAME}-data_$VERSION-1_all.deb
		fi

		ar -r ${GAMENAME}-data_$VERSION-1_all.deb debian-binary control.tar.gz data.tar.gz

		mv ${GAMENAME}-data_$VERSION-1_all.deb ..

		cd $STARTDIR

		rm -r $INSTALLDIR/deb-data/
	fi

	if [ $LINUX -eq 1 ]
	then
		echo "Making binary deb file(s)..."
		# Make client binary packages
		if [ $ARCH = "x86_64" ]
		then
			DEBARCH="amd64"
		else
			DEBARCH="i386"
		fi

		make_client_deb;

		if [ $ARCH = "x86_64" ]
		then
			DEBARCH="i386"
			make_client_deb;
		fi

	fi
fi


echo "Done!"

