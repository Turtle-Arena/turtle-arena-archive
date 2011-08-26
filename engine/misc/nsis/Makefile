ifndef VERSION
VERSION=0.5.2
endif
ifndef RELEASE
RELEASE=0
endif
ifndef ARCH
ARCH=x86
endif
ifndef ASSETPATH
ASSETPATH=.
endif
ifndef INSTALLDIR
INSTALLDIR=.
endif

ifeq ($(ARCH),x64)
  SDLDLL=SDL64.dll
else
  SDLDLL=SDL.dll
endif

all: turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe

turtlearena.$(ARCH).nsi: turtlearena.nsi.in
	sed 's|XXXVERSIONXXX|$(VERSION)|;s|XXXRELEASEXXX|$(RELEASE)|;s|x86|$(ARCH)|;s|XXXASSETPATHXXX|$(ASSETPATH)|;s|SDL.dll|$(SDLDLL)|g' < $< > $@

turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe: turtlearena.$(ARCH).nsi
	cp "../../../GPL-2.txt" .
	cp "../../../CC-BY-SA-3.0.txt" .
	cp "../../../COPYRIGHTS.txt" .
	cp "../../../CREDITS.txt" .
	cp "../../../INSTALLER_README.txt" "README.txt"
	todos "$(CURDIR)/*.txt"
	makensis turtlearena.$(ARCH).nsi

clean:
	rm -rf *.exe turtlearena.$(ARCH).nsi GPL-2.txt CC-BY-SA-3.0.txt COPYRIGHTS.txt CREDITS.txt README.txt

install:
	mkdir -p $(INSTALLDIR)
	mv turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe $(INSTALLDIR)

.PHONY: all clean

