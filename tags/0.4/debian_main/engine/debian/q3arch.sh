#!/bin/sh
# Usage: q3arch.sh arch|platform BUILD|HOST
#
# Output an architecture or platform name that Quake 3 could use for the
# build or host CPU architecture or operating system.

set -e

case $1 in

arch)
    # In the upstream Makefile the architecture is given by uname -m, with the
    # following substitutions:
    #
    # i.86 -> i386
    # powerpc -> ppc
    # alpha -> axp (on Linux and FreeBSD)
    #
    # However, for most architectures the build system doesn't actually care,
    # it's just "some other architecture", so we can get away with using the
    # GNU CPU as-is. i386, ppc and x86_64 are the unusual ones.
    #
    # (For instance, on Debian's HPPA buildd, uname -m says parisc64 whereas
    # the GNU CPU type is hppa, but that's not important because the Makefile
    # doesn't actually do anything different.)

    DEB_X_GNU_CPU=`dpkg-architecture -qDEB_$2_GNU_CPU`

    case ${DEB_X_GNU_CPU} in

    i?86)
        echo i386
        ;;

    powerpc)
        echo ppc
        ;;

    *)
        echo ${DEB_X_GNU_CPU}
        ;;

    esac
    ;;

platform)
    # In the upstream Makefile the platform is given by uname, with the
    # following substitutions:
    #
    # anything after _ removed
    # folded to lower case
    # / -> _
    #
    # This would result in Debian builds being done for linux, gnu_kfreebsd and
    # gnu.
    #
    # However, for most platform names the build system doesn't actually care,
    # it's just "some other platform", so we can get away with using the
    # GNU system as-is.
    #
    # (For instance, on Debian kFreeBSD buildd, uname says GNU/kFreeBSD whereas
    # the GNU CPU type is kfreebsd-gnu, but that's not important because the
    # Makefile doesn't actually do anything different.)

    DEB_X_GNU_SYSTEM=`dpkg-architecture -qDEB_$2_GNU_SYSTEM`

    case ${DEB_X_GNU_SYSTEM} in

    linux-gnu)
        echo linux
        ;;

    *)
        echo ${DEB_X_GNU_SYSTEM}
        ;;

    esac
    ;;

*)
    echo "Usage: sh q3arch.sh arch|platform BUILD|HOST" >&2
    exit 1
    ;;

esac

# Copyright 2009 Simon McVittie <smcv@debian.org>
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided this notice is preserved.
# This file is offered as-is, without any warranty.
