// SPDX-License-Identifier: BSD-2-Clause
//
// Broad management of notebooks
// currently can only make new notebooks
#include <stdio.h>

#include "config.h"
#include "utils.h"

int notebooks_new(const char *notebook) {
	utils_ensure_dir(config_dir_get(notebook));

	return 1;
}
