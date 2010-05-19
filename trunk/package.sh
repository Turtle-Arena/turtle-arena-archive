#!/bin/bash
#
# Package Turtle Arena for release.
#
#  Supports Zip and Deb releases.
#

# Remember where we started the script, so we can return there.
STARTDIR=`pwd`

# Directory to put the files for release
INSTALLDIR=install

# Version (Current Turtle Arena version)
VERSION=0.2
# For deb only fixes update DEB_VERSION "turtlearena_$VERSION-$*DEB_VERSION"
CLIENT_DEB_VERSION=1
DATA_DEB_VERSION=1

# Package Linux binaries
LINUX=1

# Create assets0.pk3
DATAFILES=1

# TODO: Zip the files? currently they have to be zipped /after/ running the script.
MAKEZIP=1

	# Package win32 binaries for zip install
	WIN32=1
	# TODO: Support 64-bit Windows? (ioquake3 does)


# Make .deb package for Debian based systems
MAKEDEB=1

	# Game name for deb package
	GAMENAME="turtlearena"

	# Long Title
	LONGTITLE="Third-person arena game"

	# Description
	DESC="Turtle Arena is a Ninja Turtle fangame based on ioquake3."

	# Packager's name and email
	# Example: "John Smith <address@example.com>"
	NAME_AND_EMAIL=""


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
		cp engine/build/release-$PLATFORM/turtlearena-ded.$ARCH $INSTALLDIR

		if [ $ARCH = "x86_64" ]
		then
			cp engine/build/release-linux-i386/turtlearena.i386 $INSTALLDIR
			cp engine/build/release-linux-i386/turtlearena-ded.i386 $INSTALLDIR
		fi
	fi

	if [ $WIN32 -eq 1 ]
	then
		cp engine/build/release-mingw32-x86/turtlearena.x86.exe $INSTALLDIR
		cp engine/build/release-mingw32-x86/turtlearena-ded.x86.exe $INSTALLDIR

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

	# Remove shared libraries
	rm $INSTALLDIR/base_svn/*.so
	rm $INSTALLDIR/base_svn/*.dll

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
		cp extras/turtlearena.sh $INSTALLDIR
	fi

	cp GAME_README.txt $INSTALLDIR/readme.txt
	cp COPYING.txt $INSTALLDIR
	cp COPYRIGHTS.txt $INSTALLDIR
	cp CREDITS.txt $INSTALLDIR

	# Convert to dos line ending
	unix2dos $INSTALLDIR/*.txt

	# Copy all of the files other than base/ into turtlearena-src/ and zip it.
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
	echo "This package contains Turtle Arena client." > usr/share/doc/$GAMENAME/README

	# write the turtlearena menu file
	echo "?package($GAMENAME): \\" > usr/share/menu/$GAMENAME
	echo -e "\tneeds=\"X11\" \\" >> usr/share/menu/$GAMENAME
	echo -e "\tsection=\"Games/Action\" \\" >> usr/share/menu/$GAMENAME
	echo -e "\ttitle=\"Turtle Arena\" \\" >> usr/share/menu/$GAMENAME
	echo -e "\tlongtitle=\"$LONGTITLE\" \\" >> usr/share/menu/$GAMENAME
	echo -e "\ticon=\"/usr/share/pixmaps/${GAMENAME}32.png\" \\" >> usr/share/menu/$GAMENAME
	echo -e "\tcommand=\"/usr/games/turtlearena\"" >> usr/share/menu/$GAMENAME

	# write the turtlearena.desktop file
	echo "[Desktop Entry]" > usr/share/applications/$GAMENAME.desktop
	echo "Name=Turtle Arena" >> usr/share/applications/$GAMENAME.desktop
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
	echo "Depends: libc6 (>= 2.4), libcurl3-gnutls (>= 7.16.2-1), libgl1-mesa-glx | libgl1, libogg0 (>= 1.0rc3), libopenal1, libsdl1.2debian (>= 1.2.10-1), libspeex1 (>= 1.2~beta3-1), libspeexdsp1 (>= 1.2~beta3.2-1), libvorbis0a (>= 1.1.2), libvorbisfile3 (>= 1.1.2), ${GAMENAME}-data (>= $VERSION-1)" >> control
	echo "Maintainer: $NAME_AND_EMAIL" >> control
	echo "Homepage: http://turtlearena.googlecode.com/" >> control
	echo "Description: $LONGTITLE" >> control
	echo " $DESC" >> control

	# write the postinst file
	echo "#!/bin/sh" > postinst
	echo "set -e" >> postinst
	echo "# Automatically added by dh_installmenu" >> postinst
	echo "if [ "$1" = "configure" ] && [ -x "`which update-menus 2>/dev/null`" ]; then" >> postinst
	echo -e "\tupdate-menus" >> postinst
	echo "fi" >> postinst
	echo "# End automatically added section" >> postinst

	# write the postrm file
	echo "#!/bin/sh" > postrm
	echo "set -e" >> postrm
	echo "# Automatically added by dh_installmenu" >> postrm
	echo "if [ -x "`which update-menus 2>/dev/null`" ]; then update-menus ; fi" >> postrm
	echo "# End automatically added section" >> postrm

	fakeroot tar czf data.tar.gz ./usr/*

	# md5sums?
	fakeroot tar czf control.tar.gz ./control ./postinst ./postrm

	# write the debian-binary file
	echo "2.0" > debian-binary

	if [ -f ${GAMENAME}_$VERSION-${CLIENT_DEB_VERSION}_$DEBARCH.deb ]
	then
		rm ${GAMENAME}_$VERSION-${CLIENT_DEB_VERSION}_$DEBARCH.deb
	fi

	fakeroot ar -r ${GAMENAME}_$VERSION-${CLIENT_DEB_VERSION}_$DEBARCH.deb debian-binary control.tar.gz data.tar.gz

	mv ${GAMENAME}_$VERSION-${CLIENT_DEB_VERSION}_$DEBARCH.deb ..

	cd $STARTDIR

	rm -r $INSTALLDIR/deb-client/
}


# NOTE: I have made a change from the OpenArena client
#  * client binary is in /usr/games/ instead of /usr/games/turtlearena/
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
		echo "This package contains the required data for Turtle Arena." > usr/share/doc/$GAMENAME-data/README

		# write the control file
		echo "Package: $GAMENAME-data" > control
		echo "Version: $VERSION-1" >> control
		echo "Section: games" >> control
		echo "Priority: optional" >> control
		echo "Architecture: all" >> control
		echo "Installed-Size: `du -ks usr|cut -f 1`" >> control
		echo "Maintainer: $NAME_AND_EMAIL" >> control
		echo "Homepage: http://turtlearena.googlecode.com/" >> control
		echo "Description: Turtle Arena data files" >> control
		echo " Turtle Arena requires this in order to run." >> control

		fakeroot tar czf data.tar.gz ./usr/*

		fakeroot tar czf control.tar.gz ./control

		# write the debian-binary file
		echo "2.0" > debian-binary

		if [ -f ${GAMENAME}-data_$VERSION-${DATA_DEB_VERSION}_all.deb ]
		then
			rm ${GAMENAME}-data_$VERSION-${DATA_DEB_VERSION}_all.deb
		fi

		ar -r ${GAMENAME}-data_$VERSION-${DATA_DEB_VERSION}_all.deb debian-binary control.tar.gz data.tar.gz

		mv ${GAMENAME}-data_$VERSION-${DATA_DEB_VERSION}_all.deb ..

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

