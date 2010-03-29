#!/bin/sh

# NOTE: This is currently NOT used to compile TMNT Arena maps.

# Use xreal script?:
# http://xreal.svn.sourceforge.net/viewvc/xreal/trunk/xreal/base/maps/xmap2_compile?revision=3652&view=markup

# ZTM: Based on trem-compile.sh
#-lightmapsize 512 -lightmapdir "lightmaps" -fast -shade -dirty -patchshadows -samples 3 -bounce 6 -bouncegrid
#-lightmapsize 512 -lightmapdir "lightmaps" -fast -super 2 -filter -bounce 8

# simple script to compile tremulous .map files with q3map2
# ingar@telenet.be

# For more information check
# http://en.wikibooks.org/wiki/Q3Map2

# CONFIGURATION -----------------------
#mapdir="${HOME}/.tremulous/base/maps"
#gamedir="/opt/games/tremulous"
#gamename="tremulous"
#q3map2bin="/opt/gtkradiant/q3map2"
mapdir="../base/maps"
gamedir=".."
#gamename="tmnt"
gamename="quake3"
q3map2bin="/host/Data/Zack_No_Backup/NetRadiant_linux2009_04_23/q3map2.x86"

# -------------------------------------

if [ "$1" = "-f" ] || [ "$1" = "--final" ]; then
	final="true"
	shift
fi

mapname="$1"
map="${mapdir}/${mapname}.map"

if [ -z "${mapname}" ]; then
	echo "usage: $0 [-f|--final] mapname"
	exit 1
fi

if [ ! -f "${map}" ]; then
	echo "file ${map} not found."
	exit 1
fi

q3map="${q3map2bin} -v -game ${gamename} -fs_basepath ${gamedir} -fs_game base"

if [ "${final}" = "true" ]; then
	echo "------- COMPILING WITH FINAL SETTINGS ----"
	# LIGHT options for final compile
	bspoptions="-samplesize 8"
	# -scale 1.1
	lightoptions="-lightmapsize 512 -fast -shade -dirty -patchshadows -samples 3 -bounce 6 -bouncegrid"
else
	echo "------- COMPILING WITH FAST SETTINGS ----"
	# LIGHT options for test compile
	bspoptions="-samplesize 16"
	#lightoptions="-lightmapsize 512 -faster -samples 2 -patchshadows"
	lightoptions="-light -lightmapsize 512 -fast -super 2 -filter -bounce 8"
fi

echo "------- ${map}"
echo "------- BSP ------------------------------"
# -leaktest -custinfoparms ${bspoptions} 
${q3map} -meta "${map}"
if [ ! "$?" = "0" ]; then
	exit 1;
fi

echo "------- VIS ------------------------------"
${q3map} -vis -saveprt "${map}"

echo "------- LIGHT ----------------------------"
# -scale 1.1
${q3map} -light ${lightoptions} "${map}"
