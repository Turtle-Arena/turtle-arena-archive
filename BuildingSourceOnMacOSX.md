You first need to get the source code as described [here](BuildingSource.md).

# Introduction #

Mac OS X is currently unsupported as the developer does not have a Mac computer. However, the engine Turtle Arena is based on, ioquake3, does support Mac OS X. The following is untested.

# Getting the Libraries #

You will need the following development libraries; SDL, GL, GLU, Ogg, Vorbis, and Theora.

# Compiling #
Compiling Turtle Arena on Mac OS has not been tested. It is known to work with ioquake3, but may need changes to compile Turtle Arena. See [Building ioquake3 on Mac](http://wiki.ioquake3.org/Building_ioquake3_on_Mac).

## Snow Leopard - Mac OS X 10.6.2 ##
  1. In a terminal change to the engine/ directory and run "CC=gcc-4.0 ./make-macosx-ub.sh"
  1. Output will be in engine/build/