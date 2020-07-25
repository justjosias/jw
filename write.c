// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h>

#include <ctype.h> // for isalpha and isdigit
#include <stdlib.h> // for getenv
#include <string.h> // for strcpy
#include <time.h> // for time and gmtime

#include "write.h"
#include "config.h"
#include "cache.h"
#include "utils.h"
#include "main.h"

// Form: `2020/01/`, so it is always 8 characters
static char *get_dir(struct date date)
{
	static char str_time[9];
	snprintf(str_time, 9, "%d/%02d/", date.year, date.mon);
	return str_time;
}

static size_t count_words(char *string)
{
	size_t spaces = 0;
	for (size_t i = 0; i < strlen(string); ++i) {
		if (string[i] == ' ')
			spaces++;
	}
	return spaces;
}

int write(char *text)
{

	time_t t = time(NULL);
	struct tm tm = *gmtime(&t);
	struct date date = utils_full_date(tm);

	char full_dir[100];
	snprintf(full_dir, 100, "%s/%s", config_post_dir_get(), get_dir(date));

	// form of 01-hello.md, where the text is no greater than FIRST_TEXT_LEN
	char filename[6 + FIRST_TEXT_LEN + 1];
	char tmp_first[FIRST_TEXT_LEN];

	strncpy(tmp_first, text, FIRST_TEXT_LEN);

	// modify first_text to remove non-letters and non-characters
	char first_text[FIRST_TEXT_LEN + 1];
	size_t i = 0;
	size_t n = 0; // index of first_text (may or may not increase per loop)
	while (i < FIRST_TEXT_LEN) {
		if (!isalpha(tmp_first[i]) && !isdigit(tmp_first[i]) && tmp_first[i] != '\0') {
			if (i == 0) {
				i++;
				continue;
			}

			if (first_text[n - 1] != '-' && tmp_first[i] != '\n') {
				first_text[n] = '-';
			} else {
				i++;
				continue;
			}
		} else {
			first_text[n] = tmp_first[i];
		}

		n++;
		i++;
	}

	if (first_text[strlen(first_text) - 2] == '-') {
		first_text[strlen(first_text) - 2] = '\0';
	} else {
		first_text[FIRST_TEXT_LEN - 1] = '\0';
	}

	snprintf(filename, 6 + FIRST_TEXT_LEN + 1, "%02d-%s.md", date.mday, first_text);

	const size_t full_size = sizeof(full_dir) + sizeof(filename);
	char full_path[full_size];
	strncpy(full_path, full_dir, full_size);
	strncat(full_path, filename, full_size);

	// ensure the directories exist
	utils_ensure_dir(config_post_dir_get());

	char year_path[44];
	snprintf(year_path, 44, "%s/%d", config_post_dir_get(), date.year);
	utils_ensure_dir(year_path);

	utils_ensure_dir(full_dir);

	// prepare pre-text
	char metadata[100];
	strncpy(metadata, "---\n", 99);

	char *date_str;
	date_str = utils_timestamp(date);

	strncat(metadata, "date: ", 99);
	strncat(metadata, date_str, 99);

	strncat(metadata, "\n---\n", 99);
	metadata[99] = '\0'; // I don't know if this is necessary or not TODO

	// write to file
	FILE *file = fopen(full_path, "a+");
	if (file == NULL) {
		fprintf(stderr, "Error opening file: %s\n", full_path);
		return -1;
	}

	fputs(metadata, file);
	fputc('\n', file);
	fputs(text, file);

	fclose(file);

	struct metadata post_metadata = {
		date,
	};

	char path_from_post[40]; // path to post relative to post dir
	snprintf(path_from_post, 40, "/%d/%d/%s", date.year, date.mon, filename);

	struct post post = {
		post_metadata,
		path_from_post,
	};

	cache_list_add(post);

	fprintf(stderr, "\nWords: %zu. Characters: %lu.\n", count_words(text), strlen(text));
	fprintf(stderr, "Saved to %s\n", full_path);

	return strlen(text);
}
