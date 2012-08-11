#
# Turtle Arena packaging system
#
# See engine/Makefile for building the game.
#

COMPILE_PLATFORM=$(shell uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]'|sed -e 's/\//_/g')

COMPILE_ARCH=$(shell uname -m | sed -e s/i.86/i386/)

ifeq ($(COMPILE_PLATFORM),sunos)
  # Solaris uname and GNU uname differ
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif
ifeq ($(COMPILE_PLATFORM),darwin)
  # Apple does some things a little differently...
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif

ifeq ($(COMPILE_PLATFORM),mingw32)
  ifeq ($(COMPILE_ARCH),i386)
    COMPILE_ARCH=x86
  endif
  ifeq ($(COMPILE_ARCH),x86_64)
    COMPILE_ARCH=x64
  endif
endif

ifndef MINGWMAKE
ifeq ($(COMPILE_PLATFORM),mingw32)
MINGWMAKE=make ARCH=x86
else
MINGWMAKE=exec engine/cross-make-mingw.sh
endif
endif

ifndef MINGW64MAKE
ifeq ($(COMPILE_PLATFORM),mingw32)
MINGW64MAKE=make ARCH=x64
else
MINGW64MAKE=exec engine/cross-make-mingw64.sh
endif
endif

# Directory to put the files for release
ifndef INSTALLDIR
INSTALLDIR=install
endif

# Directory containing base/assets*.pk3
ifndef DATADIR
DATADIR=install
endif

# These currently only effect zip/full-source name
ifndef GAMENAME
GAMENAME=turtlearena
endif
ifndef VERSION
VERSION=0.6.1
endif
ifndef RELEASE
RELEASE=0
endif

# Used for zip directory name and full source archive name
ifndef NAME
NAME=$(GAMENAME)-$(VERSION)-$(RELEASE)
endif

#############################################################################
#
# If you require a different configuration from the defaults below, create a
# new file named "Makefile.local" in the same directory as this file and define
# your parameters there. This allows you to change configuration without
# causing problems with keeping up to date with the repository.
#
#############################################################################
-include Makefile.local

ifndef PLATFORM
PLATFORM=$(COMPILE_PLATFORM)
endif
export PLATFORM

ifeq ($(COMPILE_ARCH),powerpc)
  COMPILE_ARCH=ppc
endif
ifeq ($(COMPILE_ARCH),powerpc64)
  COMPILE_ARCH=ppc64
endif

ifndef ARCH
ARCH=$(COMPILE_ARCH)
endif
export ARCH

ifneq ($(PLATFORM),$(COMPILE_PLATFORM))
  CROSS_COMPILING=1
else
  CROSS_COMPILING=0

  ifneq ($(ARCH),$(COMPILE_ARCH))
    CROSS_COMPILING=1
  endif
endif
export CROSS_COMPILING

ifeq ($(V),1)
echo_cmd=@:
Q=
else
echo_cmd=@echo
Q=@
endif

ifndef WIN64
WIN64=0
ifeq ($(ARCH),x86_64)
	WIN64=1
endif
ifeq ($(ARCH),x64)
	WIN64=1
endif
endif

ifndef JOBS
JOBS=1
endif

ifndef BUILD_FINAL
BUILD_FINAL=1
endif


#
# Misc
#
default:
	@echo "This is the game's packaging makefile;"
	@echo "Run 'make -C engine' to build the game, or 'make help' for how to build packages"

help:
	@echo "Usage;"
	@echo "  Run 'make packages' to build all packages."
	@echo "  Run 'make clean' to delete all packages."
	@echo "  Run 'make *-clean' to delete a single package type"
	@echo "          (where * is dist, zip, etc. e.g. dist-clean)."
	@echo "  Run 'make engine-dist' to create engine source code tarball."
	@echo "  Run 'make dist' to create engine+data source tarball."
	@echo "  Run 'make distdata' to create engine+data source tarball,"
	@echo "          plus data 'base' directory."
	@echo "  Run 'make zip' to create portable install for zip archive distribution."
	@echo "  Run 'make nsis' to create win32 NSIS installer."
	@echo "  Run 'make nsis64' to create win64 NSIS installer."
	@echo "  Run 'make loki' to create linux loki-setup installer."
	@echo "  Run 'make deb' to create debian packages."

packages: dist zip nsis nsis64 loki deb

clean: dist-clean zip-clean nsis-clean loki-clean deb-clean

#
# QVMs
#
qvms:
	$(MAKE) -C engine BUILD_GAME_SO=0 BUILD_SERVER=0 BUILD_CLIENT=0 BUILD_GAME_QVM=1 BUILD_FINAL=$(BUILD_FINAL) --jobs=$(JOBS)

qvms-clean:
	$(MAKE) -C engine clean BUILD_GAME_SO=0 BUILD_SERVER=0 BUILD_CLIENT=0 BUILD_GAME_QVM=1 --jobs=$(JOBS)

#
# Asset packages (pk3 files)
#
assets: $(DATADIR)/base/assets0.pk3 $(DATADIR)/base/assets1-qvms.pk3 $(DATADIR)/base/assets2-music.pk3 $(DATADIR)/base/assets3-game-patch.pk3

assets-clean: qvms-clean
	rm -fr $(DATADIR)/base/

$(DATADIR)/base/assets0.pk3:
	$(Q)echo "  Coping base data (temporary files)..."
	@if [ -d base/.svn ]; then \
		echo "  NOTE: Using data files from svn, unversioned files/changes not included."; \
		mkdir -p $(DATADIR)/base; \
		rm -fr $(DATADIR)/base/assets0; \
		svn export base $(DATADIR)/base/assets0; \
	else \
		mkdir -p $(DATADIR)/base/assets0; \
		cp -r base/* $(DATADIR)/base/assets0; \
	fi
	$(Q)echo "  Removing unneeded temporary files..."
	$(Q)rm -f $(DATADIR)/base/assets0/*.so
	$(Q)rm -f $(DATADIR)/base/assets0/*.dll
	$(Q)rm -f $(DATADIR)/base/assets0/*.pk3
	$(Q)rm -f $(DATADIR)/base/assets0/*.zip
	$(Q)rm -rf $(DATADIR)/base/assets0/vm/ # qvms are stored in assets1-qvms.pk3
	$(Q)rm -rf $(DATADIR)/base/assets0/music/ # music is stored in assets2-music.pk3
	$(Q)echo "  Zipping assets..."
	$(Q)cd $(DATADIR)/base/assets0/ && zip -qmr ../assets0.pk3 .
	$(Q)rm -r $(DATADIR)/base/assets0/
#
# You may need to update the checksum for assets0.pk3 in engine/code/qcommon/files.c
#    to play on pure servers.
#

$(DATADIR)/base/assets1-qvms.pk3:
	$(Q)echo "Get assets1-qvms.pk3 from Turtle Arena 0.6!"

$(DATADIR)/base/assets2-music.pk3:
	$(Q)echo "  Coping music files (temporary files)..."
	@if [ -d base/.svn ]; then \
		echo "  NOTE: Using music files from svn, unversioned files/changes not included."; \
		mkdir -p $(DATADIR)/base/assets2; \
		rm -fr $(DATADIR)/base/assets2/music; \
		svn export base/music $(DATADIR)/base/assets2/music; \
	else \
		mkdir -p $(DATADIR)/base/assets0/music; \
		cp -r base/music/* $(DATADIR)/base/assets0/music; \
	fi
	$(Q)echo "  Zipping assets..."
	$(Q)cd $(DATADIR)/base/assets2/ && zip -qmr ../assets2-music.pk3 .
	$(Q)rm -r $(DATADIR)/base/assets2/

$(DATADIR)/base/assets3-game-patch.pk3: qvms
	$(Q)mkdir -p $(DATADIR)/base/assets3/vm
	$(Q)cp engine/build/release-$(COMPILE_PLATFORM)-$(COMPILE_ARCH)/base/vm/qagame.qvm $(DATADIR)/base/assets3/vm
	$(Q)cd $(DATADIR)/base/assets3/ && zip -qmr ../assets3-game-patch.pk3 .
	$(Q)rm -r $(DATADIR)/base/assets3/


#
# Create portable zip for windows x86/x64 and linux i386/x86_64
#
zip: assets
	$(Q)mkdir -p $(INSTALLDIR)/$(NAME)/base/
	$(Q)cp $(INSTALLDIR)/base/*.pk3 $(INSTALLDIR)/$(NAME)/base/
	$(Q)cp INSTALLER_README.txt $(INSTALLDIR)/$(NAME)/README.txt
	$(Q)cp GPL-2.txt $(INSTALLDIR)/$(NAME)/
	$(Q)cp CC-BY-SA-3.0.txt $(INSTALLDIR)/$(NAME)/
	$(Q)cp COPYRIGHTS.txt $(INSTALLDIR)/$(NAME)/
	$(Q)cp CREDITS.txt $(INSTALLDIR)/$(NAME)/
	$(Q)todos $(INSTALLDIR)/$(NAME)/*.txt
	$(Q)mkdir -p $(INSTALLDIR)/$(NAME)/settings
	$(Q)echo "yes" > $(INSTALLDIR)/$(NAME)/settings/portable
	@if [ -f engine/misc/nsis/OpenAL32.dll ]; then \
		cp engine/misc/nsis/*.dll $(INSTALLDIR)/$(NAME)/; \
	fi
ifneq ($(PLATFORM),mingw32)
	$(MAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MAKE) -C engine copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(NAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
ifeq ($(ARCH),x86_64)
	$(MAKE) -C engine ARCH=i386 BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MAKE) -C engine ARCH=i386 copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(NAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
endif
endif
	$(MINGWMAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MINGWMAKE) -C engine copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(NAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
ifeq ($(WIN64),1)
	$(MINGW64MAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MINGW64MAKE) -C engine copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(NAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
endif

zip-clean:
	$(Q)rm -fr $(INSTALLDIR)/$(NAME)


#
# Create win32 NSIS installer
#
nsis: assets
	$(MINGWMAKE) -C engine installer BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 \
			ASSETPATH="$(CURDIR)/$(DATADIR)/base/" --jobs=$(JOBS)
	$(MINGWMAKE) -C engine/misc/nsis install INSTALLDIR="$(CURDIR)/$(INSTALLDIR)/nsis/"

nsis-clean: assets-clean
	$(Q)rm -fr "$(CURDIR)/$(INSTALLDIR)/nsis"
	$(MINGWMAKE) -C engine/misc/nsis clean --jobs=$(JOBS)

#
# Create win64 NSIS installer
#
nsis64: assets
ifeq ($(WIN64),1)
	$(MINGW64MAKE) -C engine installer BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 \
			ASSETPATH="$(CURDIR)/$(DATADIR)/base/" --jobs=$(JOBS)
	$(MINGW64MAKE) -C engine/misc/nsis install INSTALLDIR="$(CURDIR)/$(INSTALLDIR)/nsis64/"
else
	@echo "Win64 NSIS creation is not supported on this platform."
endif

nsis64-clean: assets-clean
ifeq ($(WIN64),1)
	$(Q)rm -fr "$(CURDIR)/$(INSTALLDIR)/nsis64"
	$(MINGW64MAKE) -C engine/misc/nsis clean --jobs=$(JOBS)
else
	@echo "Win64 NSIS cleaning is not supported on this platform."
endif


#
# Create linux i386/x86_64 installer
#
loki: assets
ifeq ($(PLATFORM),mingw32)
	@echo "Loki setup creation is not supported on this platform."
else
	$(MAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
ifeq ($(ARCH),x86_64)
	$(MAKE) -C engine ARCH=i386 BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
endif
	$(MAKE) -C engine/misc/setup --jobs=$(JOBS)
	$(Q)mkdir -p $(INSTALLDIR)/loki/
	$(Q)mv engine/misc/setup/*.run $(INSTALLDIR)/loki/
endif

loki-clean:
ifeq ($(PLATFORM),mingw32)
	@echo "Loki setup cleaning is not supported on this platform."
else
	$(MAKE) -C engine/misc/setup clean --jobs=$(JOBS)
	$(Q)rm -rf $(INSTALLDIR)/loki/
endif


#
# Create Debian/Ubuntu packages
# ZTM: TODO: Rewrite deb packaging to use Makefile?
#
deb: assets
ifeq ($(PLATFORM),mingw32)
	@echo "Debian/Ubuntu package creation is not supported on this platform."
else
	exec ./package-deb.sh --installdir $(INSTALLDIR) --datadir $(DATADIR) $(DEBARGS)
endif

deb-clean:
ifeq ($(PLATFORM),mingw32)
	@echo "Debian/Ubuntu package cleaning is not supported on this platform."
else
	$(Q)rm -rf $(INSTALLDIR)/deb/
endif


#
# Create source code tarball
#
engine-dist:
	$(MAKE) -C engine dist BUILD_FINAL=$(BUILD_FINAL)
	$(Q)mkdir -p $(INSTALLDIR)/tarball
	$(Q)mv engine/*.tar.bz2 $(INSTALLDIR)/engine-tarball

engine-dist-clean:
	$(Q)rm -rf $(INSTALLDIR)/engine-tarball/


#
# Create source tarball
#
# Does not include base directory as data is in base/assets.pk3
#   uses a lot less space this way.
dist:
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src
	@if [ -d .svn ]; then \
		echo "  NOTE: Using files from svn, unversioned files/changes not included."; \
		mkdir -p $(INSTALLDIR); \
		svn export . $(INSTALLDIR)/$(NAME)-src; \
	else \
		mkdir -p $(INSTALLDIR)/$(NAME)-src; \
		cp -r . $(INSTALLDIR)/$(NAME)-src; \
		rm -fr $(INSTALLDIR)/$(NAME)-src/engine/build; \
	fi
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src/base
	$(Q)tar -C $(INSTALLDIR) --owner=root --group=root --force-local -cjf $(INSTALLDIR)/$(NAME)-src.tar.bz2 $(NAME)-src
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src

dist-clean:
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src/
	$(Q)rm -f $(INSTALLDIR)/$(NAME)-src.tar.bz2

# Include 'base' data directory.
distdata:
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src-with-data
	@if [ -d .svn ]; then \
		echo "  NOTE: Using files from svn, unversioned files/changes not included."; \
		mkdir -p $(INSTALLDIR); \
		svn export . $(INSTALLDIR)/$(NAME)-src-with-data; \
	else \
		mkdir -p $(INSTALLDIR)/$(NAME)-src-with-data; \
		cp -r . $(INSTALLDIR)/$(NAME)-src-with-data; \
		rm -fr $(INSTALLDIR)/$(NAME)-src-with-data/engine/build; \
	fi
	$(Q)tar -C $(INSTALLDIR) --owner=root --group=root --force-local -cjf $(INSTALLDIR)/$(NAME)-src-with-data.tar.bz2 $(NAME)-src-with-data
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src-with-data

distdata-clean:
	$(Q)rm -rf $(INSTALLDIR)/$(NAME)-src-with-data/
	$(Q)rm -f $(INSTALLDIR)/$(NAME)-src-with-data.tar.bz2

