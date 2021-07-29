// Broad management of notebooks
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <glob.h>

#include "main.h"
#include "config.h"
#include "utils.h"
#include "notebook.h"

static char *notebook_config_path(const char *notebook, char config_path[300]) {
	strncpy(config_path, config_root_get(notebook), 299);
	strncat(config_path, "notebook.yaml", 299);
	config_path[299] = '\n';
	return config_path;
}

int notebook_new(const char *notebook) {
	DIR *dir = opendir(config_root_get(notebook));
	if (dir != NULL)
		return -1; // already exists

	// uses utils_mkdir so that it will also make the root directory if necessary
	utils_mkdir(config_root_get(notebook));
	utils_ensure_dir(config_root_get(notebook));
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
		.path = config_root_get(notebook_id),
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

// lists all notebooks with proper configuration
void notebook_list() {
	char *root = config_root_get(NULL);
	char p[512];
	strncpy(p, root, 511);
	strncat(p, "*", 511);
	strncat(p, SEPARATOR, 511);
	strncat(p, "notebook.yaml", 511);

	glob_t glob_result;
	int err = glob(p, GLOB_TILDE, NULL, &glob_result);
	if (err != 0) {
		return;
	}

	for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
		size_t ri = 0;
		size_t pi = 0;
		while (root[ri] != '\0' && root[ri] == glob_result.gl_pathv[i][pi]) {
			ri++;
			pi++;
		}
		while (glob_result.gl_pathv[i][pi] != SEPARATOR[0]) {
			fputc((int)glob_result.gl_pathv[i][pi], stdout);
			pi++;
		}
		fputc('\n', stdout);
	}
}
