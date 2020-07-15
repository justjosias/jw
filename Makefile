NAME=eserm
PREFIX=~/.local
INSTALL=install

CFLAGS=-Wall -Wextra -pedantic

CFILES=main.c write.c utils.c config.c cache.c
OFILES=$(CFILES:.c=.o)
TESTFILES=write.o utils.o config.o cache.o test/*.c

all: build

build: $(OFILES)
	$(CC) $(LDFLAGS) -o $(NAME) $(OFILES) $(LIBS)

.c.o:
	$(CC) -c -I ./include $(CFLAGS) $(OPTIONS) $<

clean:
	$(RM) $(OFILES) $(NAME)
	$(RM) test/test

test: build
	$(CC) $(LDFLAGS) -I ./include -o test/test $(TESTFILES) $(LIBS)
	./test/test

install: build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 $(NAME) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(NAME)
