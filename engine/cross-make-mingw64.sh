#!/bin/sh

export CC=amd64-mingw32msvc-gcc
export WINDRES=amd64-mingw32msvc-windres
export PLATFORM=mingw32
export ARCH=x64
exec make $*
