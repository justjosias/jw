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
		"  tags    NOTEBOOK         list all hashtags in a notebook\n"
		"  list                     list all existing notebooks\n"
	);
}

static void print_version_info()
{
	printf("jw %s\n", VERSION);
	printf("Copyright (C) 2020, 2021, 2022 Josias Allestad\n"
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
	long int file_size = ftell(fp);
	char *text = (char *)malloc((size_t)file_size + 1);
	if (text == NULL) {
		return NULL;
	}
	fseek(fp, 0, SEEK_SET);

	int c;
	size_t pos = 0;
	while ((c = fgetc(fp)) != EOF) {
		text[pos] = (char)c;
		pos++;
	}
	text[pos] = '\0';

	remove(path);

	return text;
}

struct notebook open_notebook(const char *notebook_id, int *err) {
	*err = 0;
	struct notebook notebook = notebook_load(notebook_id, err);

	bool exists = true;
	struct stat st = {0};
	if (stat(notebook.path, &st) == -1)
		exists = false;

	if (exists && *err != 0) {
		fprintf(stderr, "Error opening notebook %s. Make sure notebook.yaml is available\n", notebook_id);
		*err = -1;
	} else if (!exists) {
		fprintf(stderr, "Notebook %s does not exist. Create it with `jw new %s`\n", notebook_id, notebook_id);
		*err = -1;
	}

	return notebook;
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
	}

	int err = 0;
	if (strcmp(argv[1], "new") == 0) {
		if (argc < 3) {
			fprintf(stderr, "Usage: jw new NOTEBOOK\n");
			return EXIT_FAILURE;
		}
		err = notebook_new(argv[2]);
		if (err != 0) {
			if (err == -1) {
				fprintf(stderr, "Notebook already exists\n");
			}
			fprintf(stderr, "Failed to make notebook: %s\n", argv[2]);
			return EXIT_FAILURE;
		}
		fprintf(stdout, "Notebook %s created\n", argv[2]);
	} else if (strcmp(argv[1], "post") == 0) {
		if (argc < 3) {
			fprintf(stderr, "Usage: jw post NOTEBOOK\n");
			return EXIT_FAILURE;
		}

		struct notebook notebook = open_notebook(argv[2], &err);
		if (err != 0) {
			return EXIT_FAILURE;
		}

		char *text = get_text();
		if (text == NULL) {
			fprintf(stderr, "Out of memory! Check /tmp/jw.* to recover post.\n");
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

		struct notebook notebook = open_notebook(argv[2], &err);
		if (err != 0) {
			return EXIT_FAILURE;
		}

		size_t count = 0;
		struct search_result *results = search(notebook, argv[3], &count);
		if (results && count > 0) {
			for (size_t i = 0; i < count; ++i) {
				printf("%s\n", results[i].path);
			}
		}
		printf("Found %zu results\n", count);
	} else if (strcmp(argv[1], "tags") == 0) {
		if (argc < 3) {
			fprintf(stderr, "Usage: jw tags NOTEBOOK\n");
			return EXIT_FAILURE;
		}

		struct notebook notebook = open_notebook(argv[2], &err);
		if (err != 0) {
			return EXIT_FAILURE;
		}

		size_t count = 0;
		struct search_hashtag *results = search_list_hashtags(notebook, &count);

		for (size_t i = 0; i < count; ++i) {
			printf("#%s: %d\n", results[i].tag, results[i].occurances);
		}
	} else if (strcmp(argv[1], "list") == 0) {
		notebook_list();
	} else {
		fprintf(stderr, "jw: command \"%s\" not found\n", argv[1]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
