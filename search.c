#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cache.h"
#include "config.h"
#include "search.h"
#include "notebook.h"

// Strategy for searching:
// Open every post and determine whether or not a certain query
// is found within it. If so, get basic information about it and
// add it to the list.
struct search_result *search(struct notebook notebook, const char query[100], size_t *result_count)
{
	if (strcmp(query, "") == 0) {
		return NULL;
	}

	static struct search_result results[5000];
	size_t result_index = 0;

	char path[512];
	FILE *cache_file = fopen(cache_path(notebook.id), "r");
	if (cache_file == NULL)
		return NULL;
	while (fgets(path, 512, cache_file)) {
		path[strlen(path) - 1] = '\0'; // remove \n
		// skip blank lines and those starting with "#"
		if (strcmp(path, "") == 0 || strncmp(path, "#", 1) == 0)
			continue;

		char filepath[512];
		strncpy(filepath, notebook.path, 256);
		strncat(filepath, path, 256);
		filepath[256] = '\0';

		FILE *fp = fopen(filepath, "r");

		if (fp == NULL) {
			fprintf(stderr, "File not found but is listed in posts.txt: %s\n", filepath);
			continue;
		}

		int c;
		size_t count = 1;
		while ((c = getc(fp)) != EOF) {
			count++;
		}

		char *contents = malloc(count);
		size_t i = 0;
		fseek(fp, 0, SEEK_SET);
		while ((c = getc(fp)) != EOF && i < count) {
			contents[i] = (char)c;
			i++;
		}
		contents[i] = '\0';

		if (strstr(contents, query) != NULL) {
			struct search_result r;
			strncpy(r.path, notebook.path, 256);
			strncat(r.path, path, 256);
			r.path[256] = '\0';

			results[result_index] = r;
			result_index++;
		}

		free(contents);
		fclose(fp);
	}

	*result_count = result_index;
	return results;
}

struct search_hashtag *search_list_hashtags(struct notebook notebook, size_t *result_count) {
	static struct search_hashtag results[5000];
	size_t result_index = 0;

	char path[512];
	FILE *cache_file = fopen(cache_path(notebook.id), "r");
	if (cache_file == NULL)
		return NULL;
	while (fgets(path, 512, cache_file)) {
		path[strlen(path) - 1] = '\0'; // remove \n
		// skip blank lines and those starting with "#"
		if (strcmp(path, "") == 0 || strncmp(path, "#", 1) == 0)
			continue;

		char filepath[512];
		strncpy(filepath, notebook.path, 256);
		strncat(filepath, path, 256);
		filepath[256] = '\0';

		FILE *fp = fopen(filepath, "r");

		if (fp == NULL) {
			fprintf(stderr, "File not found but is listed in posts.txt: %s\n", filepath);
			continue;
		}

		bool in_tag = false;
		size_t tag_i = 0;
		int c = 0;
		size_t i = 0;
		char tag[512];
		while ((c = getc(fp)) != EOF) {
			if (in_tag) {
				if (c == ' ' || c == '\n') {
					in_tag = false;
					tag[tag_i] = '\0';


					size_t j = 0;
					for (; j < result_index; ++j) {
						if (strcmp(tag, results[j].tag) == 0) {
							break;
						}
					}
					// j will equal result_index if no results were already found
					if (j != result_index) {
						results[j].occurances += 1;
					} else {
						strcpy(results[result_index].tag, tag);
						results[result_index].occurances += 1;
						result_index += 1;
					}
					tag_i = 0;
				} else {
					tag[tag_i] = (char)c;
					tag_i += 1;
				}
			} else if (c == '#') {
				in_tag = true;
			}
			i++;
		}

		fclose(fp);
	}

	*result_count = result_index;
	return results;
}
