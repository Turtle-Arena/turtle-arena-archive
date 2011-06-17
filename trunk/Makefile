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
ifeq ($(PLATFORM),mingw32)
MINGWMAKE=make
else
MINGWMAKE=exec engine/cross-make-mingw.sh
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

ifndef ZIPNAME
ZIPNAME=turtlearena-0.5.0
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

ifndef JOBS
JOBS=1
endif

ifndef BUILD_FINAL
BUILD_FINAL=
endif

ASSETS0=1
ASSETS1=1
ASSETS2=1

if [ -f $(DATADIR)/base/assets0.pk3 ]; then \
	export ASSETS0=0; \
	ASSETS0=0; \
fi
if [ -f $(DATADIR)/base/assets1-qvms.pk3 ]; then \
	export ASSETS1=0; \
	ASSETS1=0; \
fi
if [ -f $(DATADIR)/base/assets2-music.pk3 ]; then \
	export ASSETS2=0; \
	ASSETS2=0; \
fi

ASSETS=
ifeq ($(ASSETS0),1)
	ASSETS+=$(DATADIR)/base/assets0.pk3
endif
ifeq ($(ASSETS1),1)
	ASSETS+=$(DATADIR)/base/assets1-qvms.pk3
endif
ifeq ($(ASSETS2),1)
	ASSETS+=$(DATADIR)/base/assets2-music.pk3
endif


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
assets: $(ASSETS)

assets-clean: qvms-clean
	rm -fr $(DATADIR)/base/

$(DATADIR)/base/assets0.pk3:
	$(Q)echo "  Coping base data (temporary files)..."
	@if [ -d base/.svn ]; then \
		echo "  NOTE: Using data files from svn, unversioned files not included."; \
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

$(DATADIR)/base/assets1-qvms.pk3: qvms
	$(Q)mkdir -p $(DATADIR)/base/assets1/vm
	$(Q)cp engine/build/release-$(COMPILE_PLATFORM)-$(COMPILE_ARCH)/base/vm/*.qvm $(DATADIR)/base/assets1/vm
	$(Q)cd $(DATADIR)/base/assets1/ && zip -qmr ../assets1-qvms.pk3 .
	$(Q)rm -r $(DATADIR)/base/assets1/

$(DATADIR)/base/assets2-music.pk3:
	$(Q)echo "  Coping music files (temporary files)..."
	@if [ -d base/.svn ]; then \
		echo "  NOTE: Using music files from svn, unversioned files not included."; \
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


#
# Create portable zip for win32 and linux32/64
#
zip: assets
	$(Q)mkdir -p $(INSTALLDIR)/$(ZIPNAME)/base/
	$(Q)cp $(INSTALLDIR)/base/*.pk3 $(INSTALLDIR)/$(ZIPNAME)/base/
	$(Q)cp INSTALLER_README.txt $(INSTALLDIR)/$(ZIPNAME)/README.txt
	$(Q)cp GPL-2.txt $(INSTALLDIR)/$(ZIPNAME)/
	$(Q)cp CC-BY-SA-3.0.txt $(INSTALLDIR)/$(ZIPNAME)/
	$(Q)cp COPYRIGHTS.txt $(INSTALLDIR)/$(ZIPNAME)/
	$(Q)cp CREDITS.txt $(INSTALLDIR)/$(ZIPNAME)/
	$(Q)todos $(INSTALLDIR)/$(ZIPNAME)/*.txt
	$(Q)mkdir -p $(INSTALLDIR)/$(ZIPNAME)/settings
	$(Q)echo "yes" > $(INSTALLDIR)/$(ZIPNAME)/settings/portable
	$(MAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MAKE) -C engine copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(ZIPNAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
	$(Q)cp engine/misc/nsis/*.dll $(INSTALLDIR)/$(ZIPNAME)/
ifneq ($(PLATFORM),mingw32)
ifeq ($(ARCH),x86_64)
	$(MAKE) -C engine ARCH=i386 BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MAKE) -C engine ARCH=i386 copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(ZIPNAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
endif
	$(MINGWMAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) BUILD_GAME_SO=0 BUILD_GAME_QVM=0 --jobs=$(JOBS)
	$(MINGWMAKE) -C engine copyfiles COPYDIR="$(CURDIR)/$(INSTALLDIR)/$(ZIPNAME)" BUILD_GAME_SO=0 --jobs=$(JOBS)
	$(Q)cp extras/turtlearena.sh $(INSTALLDIR)/$(ZIPNAME)/
endif

zip-clean:
	$(Q)rm -fr $(INSTALLDIR)/$(ZIPNAME)


#
# Create win32 NSIS installer
#
nsis: assets
	$(MINGWMAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) --jobs=$(JOBS)
	$(MINGWMAKE) -C engine/misc/nsis ASSETPATH="$(CURDIR)/$(DATADIR)/base/" --jobs=$(JOBS)
	$(MINGWMAKE) -C engine/misc/nsis install INSTALLDIR="$(CURDIR)/$(INSTALLDIR)/nsis" --jobs=$(JOBS)

nsis-clean: assets-clean
	$(Q)rm -fr "$(CURDIR)/$(INSTALLDIR)/nsis"
	$(MINGWMAKE) -C engine/misc/nsis clean --jobs=$(JOBS)

#
# Create linux32/64 installer
#
loki: assets
ifeq ($(PLATFORM),mingw32)
	@echo "Loki setup creation is not supported on this platform."
else
	$(MAKE) -C engine BUILD_FINAL=$(BUILD_FINAL) --jobs=$(JOBS)
ifeq ($(ARCH),x86_64)
	$(MAKE) -C engine ARCH=i386 BUILD_FINAL=$(BUILD_FINAL) --jobs=$(JOBS)
endif
	$(MAKE) -C engine/misc/setup --jobs=$(JOBS)
endif

loki-clean:
ifeq ($(PLATFORM),mingw32)
	@echo "Loki setup cleaning is not supported on this platform."
else
	$(MAKE) -C engine/misc/setup clean --jobs=$(JOBS)
endif


#
# Create Debian/Ubuntu packages
# ZTM: TODO: Rewrite deb packaging to use Makefile?
#
deb: assets
ifeq ($(PLATFORM),mingw32)
	@echo "Debian/Ubuntu package creation is not supported on this platform."
else
	exec package-deb.sh --installdir $(INSTALLDIR) --datadir $(DATADIR)
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
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src
	$(Q)svn export . $(INSTALLDIR)/$(ZIPNAME)-src
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src/base
	$(Q)tar -C $(INSTALLDIR) --owner=root --group=root --force-local -cjf $(INSTALLDIR)/$(ZIPNAME)-src.tar.bz2 $(ZIPNAME)-src
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src

# Includes base data directory.
distdata:
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src
	$(Q)svn export . $(INSTALLDIR)/$(ZIPNAME)-src
	$(Q)tar -C $(INSTALLDIR) --owner=root --group=root --force-local -cjf $(INSTALLDIR)/$(ZIPNAME)-src.tar.bz2 $(ZIPNAME)-src
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src

dist-clean:
	$(Q)rm -rf $(INSTALLDIR)/$(ZIPNAME)-src/
	$(Q)rm -f $(INSTALLDIR)/$(ZIPNAME)-src.tar.bz2


#
# Defaults
#
clean: dist-clean zip-clean nsis-clean loki-clean deb-clean

package: dist zip nsis loki deb

default: package

