// SPDX-License-Identifier: BSD-2-Clause
// manages the cache file that lists all of the cogs in linear form
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"

void cache_list_add(const char *text)
{
	FILE *file = fopen(CACHE_LIST_PATH, "a");
	fclose(file);
}

bool cache_list_exists(const char *text)
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
