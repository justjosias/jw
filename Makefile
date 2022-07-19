NAME=jw
PREFIX=~/.local
INSTALL=install

CFLAGS=-g -Werror -Wall -Wextra -Wshadow -Wcast-align -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wundef -Wpointer-arith -Wunreachable-code -Wwrite-strings -Winit-self -Wstrict-overflow -Wmaybe-uninitialized

LDFLAGS=$(shell pkg-config --libs libbsd)

CFILES=main.c write.c utils.c config.c cache.c search.c notebook.c
OFILES=$(CFILES:.c=.o)
COMMIT=$(shell git rev-parse --short HEAD)
VERSION=0.1.0-$(COMMIT)

all: build

build: $(OFILES)
	$(CC) -o $(NAME) $(OFILES) $(LIBS) $(LDFLAGS)
.c.o:
	$(CC) -c -I ./include $(CFLAGS) $(OPTIONS) $< -DVERSION=\"$(VERSION)\"


clean:
	$(RM) $(OFILES) $(NAME)

install: clean build
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) -m755 $(NAME) $(PREFIX)/bin
	
	$(INSTALL) -d $(PREFIX)/share/bash-completion/completions/
	$(INSTALL) -m755 $(NAME).bash-completion $(PREFIX)/share/bash-completion/completions/$(NAME)

uninstall:
	$(RM) $(PREFIX)/bin/$(NAME)
	$(RM) $(PREFIX)/share/bash-completion/completions/$(NAME)
