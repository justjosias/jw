// SPDX-License-Identifier: BSD-2-Clause
//
// Broad management of notebooks
#include <stdio.h>
#include <tinydir.h>

#include "main.h"
#include "config.h"
#include "utils.h"
#include "notebook.h"

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

static char *notebook_config_path(const char *notebook, char config_path[300]) {
	strncpy(config_path, config_dir_get(notebook), 299);
	strncat(config_path, "notebook.yaml", 299);
	config_path[299] = '\n';
	return config_path;
}

int notebook_new(const char *notebook) {
	DIR *dir = opendir(config_dir_get(notebook));
	if (dir != NULL)
		return -1; // already exists

	utils_ensure_dir(config_dir_get(notebook));
	char config_path[300];
	FILE *fp = fopen(notebook_config_path(notebook, config_path), "w");
	if (fp == NULL)
		return -2;

	fputs("name: ", fp);
	fputs(notebook, fp);
	fputc('\n', fp);
	fputs("description: ", fp);
	fputs("\n", fp);
	fputs("post_path: %Y-%m-%d-%s.md\n", fp);
	fputs("metadata: true\n", fp);

	return 0;
}

struct notebook notebook_load(const char *notebook_id, int *err) {
	struct notebook_config config = {
		.name = "",
		.description = "",
		.post_path = "",
	};
	struct notebook notebook = {
		.config = config,
		.id = notebook_id,
		.path = config_dir_get(notebook_id),
	};

	char config_path[300];
	FILE *fp = fopen(notebook_config_path(notebook_id, config_path), "r");
	if (fp == NULL) {
		*err = -1;
		return notebook;
	}

	char value[512];
	config.metadata = false;
	if (utils_mini_yaml(fp, "name", value) != NULL)
		strcpy(config.name, value);
	if (utils_mini_yaml(fp, "description", value) != NULL)
		strcpy(config.description, value);
	if (utils_mini_yaml(fp, "post_path", value) != NULL)
		strcpy(config.post_path, value);
	if (utils_mini_yaml(fp, "metadata", value) != NULL) {
		if (strcmp(value, "true") == 0) {
			config.metadata = true;
		} else {
			config.metadata = false;
		}
	}

	notebook.config = config;

	return notebook;
}
