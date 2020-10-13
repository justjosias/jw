#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cache.h"
#include "config.h"
#include "search.h"
#include "notebook.h"

// Strategy for searching:
// Open every post and determine whether or not a certain query
// is found within it. If so, get basic information about it and
// add it to the list.

struct result *search(struct notebook notebook, const char query[100], size_t *result_count)
{
	if (strcmp(query, "") == 0) {
		return NULL;
	}

	static struct result results[1000];
	size_t result_index = 0;

	char path[512];
	FILE *cache_file = fopen(cache_path(notebook.id), "r");
	if (cache_file == NULL)
		return NULL;
	while (fgets(path, 512, cache_file)) {
		path[strlen(path) - 1] = '\0'; // remove \n
		if (strcmp(path, "") == 0)
			continue;

		char filepath[256];
		strncpy(filepath, notebook.path, 255);
		strncat(filepath, path, 255);
		filepath[255] = '\0';

		FILE *fp = fopen(filepath, "r");

		if (fp == NULL) {
			puts("No such thing");
			continue;
		}

		int c;
		size_t count = 0;
		while ((c = getc(fp)) != EOF) {
			count++;
		}

		char *contents = malloc(count);
		size_t i = 0;
		fseek(fp, 0, SEEK_SET);
		while ((c = getc(fp)) != EOF && i < count) {
			contents[i] = c;
			i++;
		}
		contents[i] = '\0';

		if (strstr(contents, query) != NULL) {
			struct result r;
			strncpy(r.path, notebook.path, 255);
			strncat(r.path, path, 255);
			r.path[255] = '\0';

			results[result_index] = r;
			result_index++;
		}

		fclose(fp);
	}

	*result_count = result_index;
	return results;
}
