#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "main.h"
#include "config.h"
#include "notebook.h"
#include "search.h"
#include "utils.h"
#include "write.h"

static void print_help()
{
	printf(
		"Usage: jw [OPTIONS] COMMAND\n"
		"A simple micro-journaling tool\n\n"
		"  help                     display this help text\n"

		"  version                  display version information\n"
		"\n"
		"  new     NOTEBOOK         make a new notebook\n"
		"  post    NOTEBOOK         write a post\n"
		"  search  NOTEBOOK  QUERY  search for text in a post\n"
		"  stats   NOTEBOOK         print basic information about a notebook\n"
	);
}

static void print_version_info()
{
	printf("jw %s\n", VERSION);
	printf("Copyright (C) 2020, Josias Allestad\n"
		"You may use this software under the terms of the BSD-2-Clause license\n");
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
	char *text = (char *)malloc(file_size + 1);
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

	char *notebook_id = argv[2];
	int err = 0;
	struct notebook notebook = notebook_load(notebook_id, &err);

	bool exists = true;
	struct stat st = {0};
	if (stat(notebook.path, &st) == -1)
		exists = false;

	if (exists && err != 0) {
		fprintf(stderr, "Error opening notebook %s. Make sure notebook.yaml is available\n", notebook_id);
		return EXIT_FAILURE;
	}

	if (!exists && strcmp(argv[1], "new") != 0) {
		fprintf(stderr, "Notebook %s does not exist. Create it with `jw new %s`\n", notebook_id, notebook_id);
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "new") == 0) {
		err = notebook_new(notebook.id);
		if (err != 0) {
			if (err == -1)
				fprintf(stderr, "Notebook already exists\n");
			fprintf(stderr, "Failed to make notebook: %s\n", notebook_id);
			return EXIT_FAILURE;
		}
		fprintf(stdout, "Notebook %s created\n", notebook.id);
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
