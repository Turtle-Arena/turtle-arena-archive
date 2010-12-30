ifndef VERSION
VERSION=0.3
endif
ifndef RELEASE
RELEASE=0
endif

all:
	VERSION=$(VERSION) RELEASE=$(RELEASE) ./doit

sign:
	for i in *.run; do \
		gpg -bao $$i.asc $$i; \
	done

clean:
	rm -rf *.run image

.PHONY: all sign clean
