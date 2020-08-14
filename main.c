// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "config.h"
#include "notebooks.h"
#include "write.h"
#include "search.h"

static void print_help()
{
	fprintf(stderr, "Usage: jw [OPTIONS] COMMAND\n");
	fprintf(stderr, "A simple micro-journaling tool\n\n");
	fprintf(stderr, "  help                     display this help text\n");
	fprintf(stderr, "  version                  display version information\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  new     NOTEBOOK         make a new notebook\n");
	fprintf(stderr, "  post    NOTEBOOK         write a post\n");
	fprintf(stderr, "  search  NOTEBOOK  QUERY  search for text in a post\n");
}

static void print_version_info()
{
	fprintf(stderr, "jw %s\n", VERSION);
	fprintf(stderr, "Copyright (C) 2020, Josias Allestad\n");
	fprintf(stderr, "You may use this software under the terms of the BSD-2-Clause license\n");
}

static char *get_text(char *text, size_t len, char *exit)
{
	size_t available = len;

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
		fprintf(stderr, "Usage: jw COMMAND\n");
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "help") == 0) {
		print_help();
		return EXIT_SUCCESS;
	} else if (strcmp(argv[1], "version") == 0) {
		print_version_info();
		return EXIT_SUCCESS;
	} else if (argc < 3) {
		fprintf(stderr, "jw: command %s doesn't exist or needs argument\n", argv[1]);
		return EXIT_FAILURE;
	}

	char *notebook = argv[2];
	DIR *dir = opendir(config_dir_get(notebook));
	if (!dir && strcmp(argv[1], "new") != 0) {
		fprintf(stderr, "jw: notebook %s doesn't exist. Create it with `jw new %s`\n", notebook, notebook);
		return EXIT_FAILURE;
	}
	closedir(dir);

	if (strcmp(argv[1], "new") == 0) {
		notebooks_new(notebook);
		fprintf(stderr, "Notebook %s created\n", notebook);
	} else if (strcmp(argv[1], "post") == 0) {
		char *text = (char *)malloc(1000);
		int len = 1000;

		fprintf(stderr, "Write here. When you are done, type \"DONE\" and enter.\n");
		fprintf(stderr, "-----------------------------------------------------\n");

		write(notebook, get_text(text, len, "DONE"));

		free(text);

	} else if (strcmp(argv[1], "search") == 0) {
		if (argc < 4) {
			fprintf(stderr, "Usage: jw search NOTEBOOK QUERY\n");
			return EXIT_FAILURE;
		}

		size_t count = 0;
		struct result *results = search(notebook, argv[2], &count);
		if (results && count > 0) {
			for (size_t i = 0; i < count; ++i) {
				printf("%s\n", results[i].path);
			}
		}
	} else {
		fprintf(stderr, "jw: unknown option: %s\n", argv[1]);
	}

	return EXIT_SUCCESS;
}
