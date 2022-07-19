#ifndef CACHE_H_
#define CACHE_H_

#include "main.h"

char *cache_path(const char *notebook);
void cache_list_add(const char *notebook, const char *post_path);
char *cache_list_last(const char *notebook, long long last);

#endif
