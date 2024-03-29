// manages the cache file that lists all of the posts in linear form
//
// line format:
// ```
// 2020/07/01-test.md
// 2020-07-02-another-test.md
// ```
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "config.h"
#include "utils.h"

// TODO test repeatedly to see if path works properly as static
char *cache_path(const char *notebook)
{
	static char path[512];
	strcpy(path, config_root_get(notebook));
	strcat(path, SEPARATOR);
	strcat(path, CACHE_LIST_PATH);

	return path;
}

bool cache_post_exists(FILE *fp, const char *post_path) {
	char line[512];
	while (fgets(line, 512, fp)) {
		if (strcmp(line, post_path) == 0) {
			return true;
		}
	}
	return false;
}

void cache_list_add(const char *notebook, const char *post_path)
{
	FILE *fp = fopen(cache_path(notebook), "a");
	if (fp == NULL)
		return;
	if (cache_post_exists(fp, post_path))
		return;
	fputs(post_path, fp);
	fputc('\n', fp);
	fclose(fp);
}

char *cache_list_last(const char *notebook, long long last)
{
	FILE *fp = fopen(cache_path(notebook), "r+");
	if (fp == NULL)
		return NULL;

	char line[1024];

	if (last < 1) {
		char last_line[1024];
		do {
			strcpy(last_line, line);
		} while (fgets(line, 1023, fp));
	} else {
		for (size_t i = 0; i < (size_t)last; ++i) {
			if (fgets(line, 1023, fp) == NULL) {
				fclose(fp);
				return NULL;
			}
		}
	}
	fclose(fp);
	static char path[1024];
	strcpy(path, config_root_get(notebook));
	strcat(path, line);
	return path;
}
