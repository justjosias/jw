// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h> // for snprintf
#include <stdlib.h> // for getenv
#include <string.h> // for strncat and strncpy

#include "config.h"
#include "main.h"

// Returns the directory for jw plus an optional subdirectory
char *config_dir_get(const char *subdir)
{
#ifdef _WIN32
	char *home = getenv("USER");
#else
	char *home = getenv("HOME"); // TODO support other OSes
#endif
	static char path[257];
	strncpy(path, home, 255); // TODO is this the best?
	strcat(path, SEPARATOR);
	strncat(path, CONFIG_POST_DIR, 255);
	if (strncmp(subdir, SEPARATOR, 1) != 0)
		strcat(path, SEPARATOR);

	strncat(path, subdir, 255);
	strcat(path, SEPARATOR);
	strcat(path, "\0");
	path[256] = '\0'; // Just in case

	return path;
}
