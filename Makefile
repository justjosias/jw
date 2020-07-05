PREFIX ?= ~/.local
INSTALL ?= install

all: build

build:
	$(CC) -o eserm *.c

install: build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 eserm $(PREFIX)/bin
