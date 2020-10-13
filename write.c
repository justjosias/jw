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
#include "notebook.h"

static size_t count_words(const char *string)
{
	size_t spaces = 0;
	for (size_t i = 0; i < strlen(string); ++i) {
		if (string[i] == ' ')
			spaces++;
	}
	return spaces;
}

// Finds the first occurance of a query and replaces it in the resulting buffer
// All strings (except the buffer) must be null-terminated
static char *find_and_replace(const char *haystack, const char *needle,
		const char *replacement,
		char *buffer, size_t buf_size)
{
	char *ptr = strstr(haystack, needle);
	if (ptr == NULL)
		return NULL;

	size_t start = ptr - haystack;
	size_t len = strlen(replacement);

	if (buf_size < strlen(haystack) + len)
		return NULL;

	size_t buf_pos = 0;
	size_t hay_pos = 0;
	size_t replacement_pos = 0;
	while (hay_pos < strlen(haystack) + len) {
		if (hay_pos >= start && replacement_pos < len) {
			while (replacement_pos < len) {
				buffer[buf_pos] = replacement[replacement_pos];
				buf_pos++;
				replacement_pos++;
			}
			hay_pos += strlen(needle);
			continue;
		}

		buffer[buf_pos] = haystack[hay_pos];
		buf_pos++;

		hay_pos++;
	}

	buffer[buf_pos] = '\0';
	return buffer;
}

// returns the filename of a post in the form of
// 2020-01-01-first-text.md
// relative to $HOME/jw/[notebook]
static char *get_filename(const char *template, struct date date,
			  const char first_text[FIRST_TEXT_LEN + 1])
{
	// First text length, plus date, plus extra, plus null
	static char filename[FIRST_TEXT_LEN + 10 + 10 + 1];
	strncpy(filename, template, sizeof(filename) - 1);

	for (size_t i = 0; filename[i] != '\0' && i < sizeof(filename); i++) {
		if (filename[i] == '%')
			filename[i] = '`';
			// this should be fine because non-alphanumeric symbols
			// are removed from first_text, and are unlikely to occur
			// in the template (TODO: note this in docs)
	}

	char buf[sizeof(filename)];
	if (find_and_replace(filename, "`Y", "%02d", buf, sizeof(buf)) != NULL)
		snprintf(filename, sizeof(filename), buf, date.year);
	if (find_and_replace(filename, "`m", "%02d", buf, sizeof(buf)) != NULL)
		snprintf(filename, sizeof(filename), buf, date.mon);
	if (find_and_replace(filename, "`d", "%02d", buf, sizeof(buf)) != NULL)
		snprintf(filename, sizeof(filename), buf, date.mday);
	if (find_and_replace(filename, "`s", "%s", buf, sizeof(buf)) != NULL)
		snprintf(filename, sizeof(filename), buf, first_text);

	// Switch the remaining characters back
	for (size_t i = 0; filename[i] != '\0' && i < sizeof(filename); i++) {
		if (filename[i] == '`')
			filename[i] = '~';
	}

	return filename;
}

static char *get_full_path(const char *filename, const char *notebook)
{
	static char full_path[257];
	strncpy(full_path, config_dir_get(notebook), 256);
	strncat(full_path, filename, 256);

	return full_path;
}

// Writes a post to the directory related to the notebook
int write_post(struct notebook notebook, const char *text)
{
	time_t t = time(NULL);
	struct tm tm = *gmtime(&t);
	struct date date = utils_full_date(tm);

	// form of 01-hello.md, where the text is no greater than FIRST_TEXT_LEN
	char filename[FIRST_TEXT_LEN + 17];
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

	strcpy(filename, get_filename(notebook.config.post_path, date, first_text));

	const char *full_path = get_full_path(filename, notebook.id);


	// write to file
	FILE *file = fopen(full_path, "a+");
	if (file == NULL) {
		fprintf(stderr, "Error opening file: %s\n", full_path);
		return -1;
	}

	if (notebook.config.metadata == true) {
		char metadata[100];
		strncpy(metadata, "---\n", 99);

		char *timestamp_str = utils_timestamp(date);

		strncat(metadata, "timestamp: ", 99);
		strncat(metadata, timestamp_str, 99);

		strncat(metadata, "\n---\n", 99);
		metadata[99] = '\0'; // I don't know if this is necessary or not TODO
		fputs(metadata, file);
	}
	fputs(text, file);

	fclose(file);

	struct metadata post_metadata = {
		date,
	};

	struct post post = {
		post_metadata,
		filename,
	};

	cache_list_add(notebook.id, post);

	fprintf(stderr, "Words: %zu. Characters: %lu.\n", count_words(text), strlen(text));
	fprintf(stderr, "Saved to %s\n", full_path);

	return strlen(text);
}
