NAME=jw
PREFIX=~/.local
INSTALL=install

CFLAGS=-Wall -Wextra -pedantic

CFILES=main.c write.c utils.c config.c cache.c search.c
OFILES=$(CFILES:.c=.o)

all: build

build: $(OFILES)
	$(CC) $(LDFLAGS) -o $(NAME) $(OFILES) $(LIBS)

.c.o:
	$(CC) -c -I ./include -I ./lib $(CFLAGS) $(OPTIONS) $<

clean:
	$(RM) $(OFILES) $(NAME)
	$(RM) test/test

install: build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 $(NAME) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(NAME)
