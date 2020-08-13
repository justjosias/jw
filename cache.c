// SPDX-License-Identifier: BSD-2-Clause
// manages the cache file that lists all of the posts in linear form
//
// line format:
// ```
// date: timestamp
// filename: /2020/07/01-test.md
// ---
// date: timestamp
// filename: /2020/07/02-another-test.md
// ---
// ```
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "config.h"
#include "utils.h"

// TODO test repeatedly to see if path works properly as static
char *cache_dir(const char *notebook)
{
	static char path[256];
	strcat(path, config_dir_get(notebook));
	strcat(path, CACHE_LIST_PATH);

	return path;
}

void cache_list_add(const char *notebook, const struct post post)
{
	FILE *file = fopen(cache_dir(notebook), "a");

	fprintf(file, "date: %s\nfilename: %s\n---\n",
		utils_timestamp(post.metadata.date), post.file);

	fclose(file);
}

int cache_list_find(const char *notebook, const char *text)
{
	FILE *file = fopen(cache_dir(notebook), "r");
	char line[100];

	while (fgets(line, 100, file)) {
		if (strncmp(line, text, strlen(text)) == 0) {
			return true;
		}
	}

	fclose(file);

	return false;
}
