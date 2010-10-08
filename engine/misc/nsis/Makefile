VERSION=0.3
RELEASE=0
ARCH=x86

all: turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe

turtlearena.nsi: turtlearena.nsi.in
	sed 's/XXXVERSIONXXX/$(VERSION)/;s/XXXRELEASEXXX/$(RELEASE)/;s/x86/$(ARCH)/g' < $< > $@

turtlearena-$(VERSION)-$(RELEASE).$(ARCH).exe: turtlearena.nsi
	makensis turtlearena.nsi

clean:
	rm -rf *.exe turtlearena.nsi

.PHONY: all clean
