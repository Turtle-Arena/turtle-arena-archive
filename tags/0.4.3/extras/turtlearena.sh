#!/bin/sh
# Turtle Arena Linux Launcher
# Automaticly selects 32/64 bit version of Turtle Arena.
#
# Based on xreal ( http://www.xreal-project.net ) script

# Select binary
case `uname -m` in
	x86_64)
		BIN='turtlearena.x86_64'
		;;
	i386|i486|i586|i686)
		BIN='turtlearena.i386'
		;;
#	powerpc|powerpc64)
#		BIN='turtlearena.ppc'
#		;;
	*)
		echo "You are not running a supported platform, if you would like to play Turtle Arena on your platform, please visit http://code.google.com/p/turtlearena/ for contact information."
		exit 0
		;;
esac

if [ ! -f $BIN ]; then
	echo "Game binary '$BIN' not found!"
	exit 1
fi

# Run
./$BIN $@
