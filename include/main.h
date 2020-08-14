// SPDX-License-Identifier: BSD-2-Clause
#ifndef MAIN_H_
#define MAIN_H_

#include "utils.h"

typedef struct metadata {
	date date;
} metadata;

struct post {
	metadata metadata;
	char *file;
};

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

#endif
