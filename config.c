// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h> // for snprintf
#include <stdlib.h> // for getenv
#include <string.h> // for strncat and strncpy

#include "config.h"

char *config_cog_dir_get()
{
	char *home = getenv("HOME"); // TODO support other OSes
	static char path[50]; // TODO don't use arbitrary number
	strncpy(path, home, 49); // TODO is this really how it works? (49)
	strncat(path, CONFIG_COG_DIR, 49);
	path[49] = '\0'; // Is this right?

	return path;
}
