BSPC is the Quake III Arena bot Arena Awareness System (AAS) compiler.

== Changes from 2.1h ==
2.1h was included with the Quake3 1.32b source release.

Version is "2.1h [Turtle Arena]" instead of "2.1h"
Removed disabled BFG Jumping code.
"-grapplereach" is true by default, -nograpplereach disables it.
"-forcesidesvisible" is true by default, -noforcesidesvisible disables it.
Supports "func_breakable" entities.

== Source ==
The source is in engine/code/bspc/ , it also uses other files in engine/code/botlib/ and engine/code/qcommon/ directories, see engine/code/bspc/Makefile for which files.

Compile using the following;

	make -C engine/code/bspc

The executable will be placed in engine/build/bspc-[platform]-release/

== Usage ==
bspc -bsp2aas "mapname.bsp" (Create .aas for a single .bsp file)
bspc -bsp2aas "*.bsp"		(Create .aas for all of the .bsp files)

The log of bspc is saved to bspc.log

