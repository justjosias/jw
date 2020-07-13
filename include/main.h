// SPDX-License-Identifier: BSD-2-Clause
#ifndef MAIN_H_
#define MAIN_H_

#include "utils.h"

typedef struct metadata {
	char *title;
	date date;
} metadata;

struct cog {
	metadata metadata;
	char *file;
};

#endif
