BSPC is the Quake III Arena bot Arena Awareness System (AAS) compiler.

== Changes from 2.1h ==
2.1h was the version that was include with the Quake3 1.32b source release.

Version is "2.1h [Turtle Arena]" instead of "2.1h"
Removed disabled BFG Jumping code.
"-grapplereach" is true by default, -nograpplereach disables it.
"-forcesidesvisible" is true by default, -noforcesidesvisible disables it.
Supports "func_breakable" entities.

== Source ==
The source is in ../../engine/code/bspc/ , it also uses other files in ../../engine/code/botlib/ and ../../engine/code/qcommon/ directories, see ../../engine/code/bspc/Makefile for which files.

== Usage ==
For Windows use bspc.x86.exe, for GNU/Linux use bspc.i386 or bspc.x86_64

bspc -bsp2aas "mapname.bsp" (Create .aas for a single .bsp file)
bspc -bsp2aas "*.bsp"		(Create .aas for all of the .bsp files)

The log of bspc is saved to bspc.log

