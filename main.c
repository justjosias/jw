// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "config.h"
#include "notebooks.h"
#include "search.h"
#include "utils.h"
#include "write.h"

static void print_help()
{
	fprintf(stdout, "Usage: jw [OPTIONS] COMMAND\n");
	fprintf(stdout, "A simple micro-journaling tool\n\n");
	fprintf(stdout, "  help                     display this help text\n");
	fprintf(stdout, "  version                  display version information\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "  new     NOTEBOOK         make a new notebook\n");
	fprintf(stdout, "  post    NOTEBOOK         write a post\n");
	fprintf(stdout, "  search  NOTEBOOK  QUERY  search for text in a post\n");
}

static void print_version_info()
{
	fprintf(stdout, "jw %s\n", VERSION);
	fprintf(stdout, "Copyright (C) 2020, Josias Allestad\n");
	fprintf(stdout, "You may use this software under the terms of the BSD-2-Clause license\n");
}

// gets text using the default text editor and a temporary file
// Returns NULL on memory allocation error
// NOTE: It would be useful to make sure the temporary file is safe
// even when the program crashes, so it can be restored
static char *get_text()
{
	char path[] = "/tmp/jw.XXXXXX";
	int fd = mkstemp(path);
	FILE *fp = fdopen(fd, "a+");

	const char *editor = utils_default_editor();

	char command[256];
	strncpy(command, editor, 255);
	strncat(command, " ", 255);
	strncat(command, path, 255);
	system(command);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	char *text = (char *)malloc(file_size);
	if (text == NULL) {
		return NULL;
	}
	fseek(fp, 0, SEEK_SET);

	int c;
	size_t pos = 0;
	while ((c = fgetc(fp)) != EOF) {
		text[pos] = c;
		pos++;
	}
	text[pos] = '\0';

	remove(path);

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
		fprintf(stdout, "Notebook %s created\n", notebook);
	} else if (strcmp(argv[1], "post") == 0) {
		char *text = get_text();
		if (text == NULL) {
			fprintf(stderr, "Out of memory!\n");
			return EXIT_FAILURE;
		}

		if (text[0] == '\0') {
			fprintf(stderr, "No text was entered.\n");
			return EXIT_FAILURE;
		}

		write_post(notebook, text);

		free(text);

	} else if (strcmp(argv[1], "search") == 0) {
		if (argc < 4) {
			fprintf(stderr, "Usage: jw search NOTEBOOK QUERY\n");
			return EXIT_FAILURE;
		}

		size_t count = 0;
		struct result *results = search(notebook, argv[3], &count);
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
