// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h> // for snprintf
#include <stdlib.h> // for getenv
#include <string.h> // for strncat and strncpy

#include "config.h"

char *config_post_dir_get()
{
	char *home = getenv("HOME"); // TODO support other OSes
	static char path[257];
	strncpy(path, home, 255); // TODO is this the best?
	strncat(path, CONFIG_POST_DIR, 255);
	strncat(path, "/posts/", 255);
	strcat(path, "\0");
	path[256] = '\0'; // Just in case

	printf("Da path %s\n", path);
	return path;
}
