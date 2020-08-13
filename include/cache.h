// SPDX-License-Identifier: BSD-2-Clause
#ifndef CACHE_H_
#define CACHE_H_

#include <stdbool.h>
#include "main.h"

bool cache_list_exists(const char *notebook, const char *);
void cache_list_add(const char *notebook, const struct post);

#endif
