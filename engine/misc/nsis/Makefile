VERSION=0.3
RELEASE=0
ifndef ARCH
ARCH=x86
endif
ifndef ASSETS
ASSETS=assets0.pk3
endif
ifndef INSTALLDIR
INSTALLDIR="."
endif

all: turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe

turtlearena.nsi: turtlearena.nsi.in
	sed 's/XXXVERSIONXXX/$(VERSION)/;s/XXXRELEASEXXX/$(RELEASE)/;s/x86/$(ARCH)/;s/XXXASSETSXXX/$(ASSETS)/g' < $< > $@

turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe: turtlearena.nsi
	cp "../../../COPYING.txt" .
	cp "../../../COPYRIGHTS.txt" .
	cp "../../../CREDITS.txt" .
	cp "../../../GAME_README.txt" "readme.txt"
#	cp "../../id-readme.txt" .
#   cp "../../voip-readme.txt" .
	todos "*.txt"
	makensis turtlearena.nsi

clean:
	rm -rf *.exe turtlearena.nsi COPYING.txt COPYRIGHTS.txt CREDITS.txt readme.txt

.PHONY: all clean

install:
	mkdir -p $(INSTALLDIR)
	mv turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe $(INSTALLDIR)

