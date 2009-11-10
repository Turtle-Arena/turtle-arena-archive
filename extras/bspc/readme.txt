BSPC is the Quake3 bot aas compiler.

== Changes from 2.1h ==
:2.1h was the version that was include with Quake3 1.32b source release

Version is "2.1h [TMNT Arena]" instead of "2.1h"
Removed disabled BFG Jumping code.
"-grapplereach" is true by default, -nograpplereach disables it.
Supports "func_breakable" entities.
The function AAS_Accelerate uses "q3" accle instead of "q2" (that was used in Quake3) ...Does BSPC use this function?

== Source ==
The source is in ../engine/code/bspc/ , it also uses other files in /source/engine/code/botlib/ and /source/engine/code/qcommon directories, see source/engine/code/bspc/Makefile for which files.

== Usage ==
:In Windows use bspc.x86.exe, linux use bspc.x86[_64]

bspc -forcesidesvisible -bsp2aas "mapname.bsp"
bspc -forcesidesvisible -bsp2aas "*.bsp"		--Create .aas for all of the .bsp files

Currently must have "-forcesidesvisible" for it to work. (Why I do not know.)
   If it is always required, why isn't it true by default?

Also see output of bspc[.x86.exe|.x86|.x86_64], bspc.log

