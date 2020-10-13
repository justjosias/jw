NAME=jw
PREFIX=~/.local
INSTALL=install

CFLAGS=-Wall -Wextra -pedantic

CFILES=main.c write.c utils.c config.c cache.c search.c notebook.c
OFILES=$(CFILES:.c=.o)
VERSION=0.1.0-$(shell git rev-parse --short HEAD)

all: build

build: $(OFILES)
	$(CC) $(LDFLAGS) -o $(NAME) $(OFILES) $(LIBS)
.c.o:
	$(CC) -c -I ./include $(CFLAGS) $(OPTIONS) $< -DVERSION=\"$(VERSION)\"


clean:
	$(RM) $(OFILES) $(NAME)

install: build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 $(NAME) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(NAME)
