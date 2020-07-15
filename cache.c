// SPDX-License-Identifier: BSD-2-Clause
// manages the cache file that lists all of the cogs in linear form
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

char *cache_dir()
{
	static char path[70];
	strncpy(path, config_cog_dir_get(), 69);
	strncat(path, CACHE_LIST_PATH, 69);

	return path;
}

void cache_list_add(const struct cog cog)
{
	FILE *file = fopen(cache_dir(), "a");

	fprintf(file, "date: %s\nfilename: %s\n---\n",
		utils_timestamp(cog.metadata.date), cog.file);

	fclose(file);
}

int cache_list_find(const char *text)
{
	FILE *file = fopen(CACHE_LIST_PATH, "r");
	char line[100];

	while (fgets(line, 100, file)) {
		if (strncmp(line, text, strlen(text)) == 0) {
			return true;
		}
	}

	fclose(file);

	return false;
}
