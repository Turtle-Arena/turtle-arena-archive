#!/bin/sh
# BSPC Linux Launcher
# Automaticly selects 32/64 bit version of BSPC.
#
# Based on xreal ( http://www.xreal-project.net ) script

# Select binary
case `uname -m` in
	x86_64)
		BIN='bspc.x86_64'
		;;
	i386|i486|i586|i686)
		BIN='bspc.x86'
		;;
#	powerpc|powerpc64)
#		BIN='bspc.ppc'
#		;;
	*)
		echo "You are not running a supported platform, if you would like to play TMNT Arena on your platform, please visit http://code.google.com/p/turtlearena/ for contact information."
		exit 0
		;;
esac

if [ ! -f $BIN ]; then
	echo "BSPC binary '$BIN' not found!"
	exit 1
fi

# Run # -nograpplereach
./$BIN -forcesidesvisible -bsp2aas "../../base/maps/*.bsp"
