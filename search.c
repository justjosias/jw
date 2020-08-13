// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h>
#include <string.h>

#include <tinydir.h>

#include "config.h"
#include "search.h"

// Strategy for searching:
// Open every post and determine whether or not a certain query
// is found within it. If so, get basic information about it and
// add it to the list.

struct result *search(const char *notebook, const char query[100], size_t *result_count)
{
	if (strcmp(query, "") == 0) {
		return NULL;
	}

	static struct result results[1000];
	size_t result_index = 0;

	char *str_dir = config_dir_get(notebook);
	tinydir_dir dir;
	tinydir_open(&dir, str_dir);

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (!file.is_dir)
		{

			FILE *fp = fopen(strcat(str_dir, file.name), "r");

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
				strncpy(r.path, str_dir, 255);
				strncat(r.path, file.name, 255);
				r.path[255] = '\0';

				results[result_index] = r;
				result_index++;
			}

			fclose(fp);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	*result_count = result_index;
	return results;
}
