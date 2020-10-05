// SPDX-License-Identifier: BSD-2-Clause
//
// Broad management of notebooks
// currently can only make new notebooks
#include <stdio.h>
#include <tinydir.h>

#include "config.h"
#include "utils.h"
#include "notebooks.h"

struct stats notebook_stats(const char *notebook) {
	struct stats stats;
	stats.posts = 0;
	stats.words = 0;

	char *str_dir = config_dir_get(notebook);
	tinydir_dir dir;
	tinydir_open(&dir, str_dir);

	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (!file.is_dir) {
			stats.posts++;

			char filepath[256];
			strncpy(filepath, str_dir, 255);
			strncat(filepath, file.name, 255);
			filepath[255] = '\0';

			FILE *fp = fopen(filepath, "r");

			if (fp == NULL)
				continue;

			int c = 0;
			size_t count = 0;
			while ((c = getc(fp)) != EOF) {
				if (c == '\n' || c == ' ')
					count++;
			}

			stats.words += count;
			fclose(fp);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	return stats;
}

int notebook_new(const char *notebook) {
	utils_ensure_dir(config_dir_get(notebook));

	return 0;
}
