#!/bin/sh

export CC=i586-mingw32msvc-gcc
export WINDRES=i586-mingw32msvc-windres
export PLATFORM=mingw32
export ARCH=x86
exec make $*
