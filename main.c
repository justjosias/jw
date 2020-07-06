#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "write.h"

void print_help()
{
	fprintf(stderr, "Usage: eserm [OPTIONS] SUBCOMMAND TEXT\n");
	fprintf(stderr, "A simple micro-journaling tool\n\n");
	fprintf(stderr, "  help       display help text\n");
	fprintf(stderr, "  post       make a post (with optional title)\n");
}

void print_version_info()
{
	fprintf(stderr, "eserm %s\n", VERSION);
	fprintf(stderr, "Copyright (C) Josias Allestad\n");
	fprintf(stderr, "You may use this software under the terms of the BSD-2-Clause license\n");
}

char *get_text(char *text, int len, char *exit)
{
	int available = len;

	// TODO increase flexibility by possibly reading char by char
	char line[500];
	while ((fgets(line, 500, stdin))) {
		if (strncmp(line, exit, strlen(exit)) == 0) {
			break;
		}

		if (strlen(line) > available) {
			text = realloc(text, len + 500);
		}

		strncat(text, line, len - 1);
		available -= strlen(line);
	}

	return text;
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "Usage: eserm SUBCOMMAND\n");
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "post") == 0) {
		char *text = (char *)malloc(1000);
		int len = 1000;

		fprintf(stderr, "Write here. When you are done, type \"DONE\" and enter.\n");
		fprintf(stderr, "-----------------------------------------------------\n");

		if (argc == 3)
			write(get_text(text, len, "DONE"), argv[2]);
		else
			write(get_text(text, len, "DONE"), "");

		free(text);

	} else if (strcmp(argv[1], "help") == 0) {
		print_help();
	} else if (strcmp(argv[1], "version") == 0) {
		print_version_info();
	} else {
		fprintf(stderr, "Unknown option: %s\n", argv[1]);
	}

	return EXIT_SUCCESS;
}
