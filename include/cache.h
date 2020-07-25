// SPDX-License-Identifier: BSD-2-Clause
#ifndef CACHE_H_
#define CACHE_H_

#include <stdbool.h>
#include "main.h"

bool cache_list_exists(char *);
void cache_list_add(const struct post);

#endif
