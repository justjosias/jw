NAME ?= eserm
PREFIX ?= ~/.local
INSTALL ?= install

CFLAGS=-Wall

CFILES=main.c write.c utils.c
OFILES=$(CFILES:.c=.o)
all: build

build: $(OFILES)
	$(CC) -o $(NAME) *.c
	$(CC) $(LDFLAGS) -o $(NAME) $(OFILES) $(LIBS)

.c.o:
	$(CC) -c -I . $(CFLAGS) $(OPTIONS) $<

clean:
	$(RM) $(OFILES) $(NAME)

install: build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 $(NAME) $(PREFIX)/bin


uninstall:
	$(RM) $(PREFIX)/bin/$(NAME)
