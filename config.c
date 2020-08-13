// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h> // for snprintf
#include <stdlib.h> // for getenv
#include <string.h> // for strncat and strncpy

#include "config.h"

// Returns the directory for jw plus an optional subdirectory
char *config_dir_get(const char *subdir)
{
	char *home = getenv("HOME"); // TODO support other OSes
	static char path[257];
	strncpy(path, home, 255); // TODO is this the best?
	strncat(path, CONFIG_POST_DIR, 255);
	if (subdir[0] != '/' && subdir[0] != '\\')
		strcat(path, "/");

	strncat(path, subdir, 255);
	strcat(path, "/\0");
	path[256] = '\0'; // Just in case

	return path;
}
