#include <stdio.h>
#include <stdlib.h> // for getenv
#include <string.h> // for strncat and strncpy

#include "config.h"
#include "utils.h"
#include "main.h"

// Returns the root storage directory for jw plus an optional subdirectory
char *config_root_get(const char *subdir)
{
#ifdef _WIN32
	char *home = getenv("USER");
#else
	char *home = getenv("HOME");
#endif
	static char path[512];
	strncpy(path, home, 511);
	strcat(path, SEPARATOR);
	struct config config = config_get();
	strncat(path, config.root, 511);
	if (subdir != NULL && strncmp(subdir, SEPARATOR, 1) != 0) {
		strcat(path, SEPARATOR);
		strncat(path, subdir, 511);
	}
	strncat(path, SEPARATOR, 511);

	return path;
}

// Returns the global configuration directory, making it if necessary
char *config_dir()
{
	char *home = getenv("HOME");
	static char path[512];
	strncpy(path, home, 511);
	strncat(path, SEPARATOR, 511);
	strncat(path, CONFIG_DIR, 511);

	char temp[512];
	strcpy(temp, path);
	utils_mkdir(temp);

	return path;
}

struct config config_get()
{
	char path[512];
	strncpy(path, config_dir(), 511);
	strncat(path, "config.yaml", 511);

	// the default configuration
	struct config config = {
		.root = "jw",
		.editor = "vi",
	};

	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		return config;
	}
	strncpy(config.editor, utils_default_editor(), 511); // for some reason this causes path to be blank. TODO debug and fix

	char value[512];
	if (utils_mini_yaml(fp, "root", value) != NULL)
		strncpy(config.root, value, 511);
	if (utils_mini_yaml(fp, "editor", value) != NULL) {
		strncpy(config.editor, value, 511);
	}

	return config;
}
