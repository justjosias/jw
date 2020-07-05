#include <stdio.h>

#include <ctype.h> // for isalpha and isdigit
#include <stdlib.h> // for getenv
#include <string.h> // for strcpy
#include <sys/stat.h> // for stat
#include <time.h> // for time and gmtime

#include "write.h"
#include "config.h"

struct date {
	int year;
	int month;
	int day;
};

struct date get_full_date()
{
	time_t t = time(NULL);
	struct tm tm = *gmtime(&t);

	struct date date = {
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday + 1,
	};

	return date;
}

// Form: `2020/01/`, so it is always 8 characters
char *get_dir()
{
	struct date date = get_full_date();

	static char str_time[9];
	snprintf(str_time, 9, "%d/%02d/", date.year, date.month);
	return str_time;
}

void ensure_dir(char *path)
{
	struct stat st = {0};
	if (stat(path, &st) == -1) {
		mkdir(path, 0700);
	}
}

int write(char *text, char *title)
{
	char *home = getenv("HOME");

	char full_dir[100];
	snprintf(full_dir, 100, "%s/%s/%s", home, "cogs", get_dir());

	// form of 01-hello.md, where the text is no greater than FIRST_TEXT_LEN
	char filename[6 + FIRST_TEXT_LEN + 1];
	char tmp_first[FIRST_TEXT_LEN];

	strncpy(tmp_first, text, FIRST_TEXT_LEN);

	// modify first_text to remove non-letters and non-characters
	char first_text[FIRST_TEXT_LEN + 1];
	size_t i = 0;
	size_t n = 0; // index of first_text (may or may not increase per loop)
	while (i < FIRST_TEXT_LEN) {
		if (	!isalpha(tmp_first[i])
			&& !isdigit(tmp_first[i]) 
			&& tmp_first[i] != '\0'
		) {
			if (i == 0) {
				i++;
				continue;
			}

			if (	first_text[n - 1] != '-'
				&& tmp_first[i] != '\n'
			) {
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

	if (first_text[strlen(first_text) - 1] == '-') {
		first_text[strlen(first_text) - 2] = '\0';
	} else {
		first_text[FIRST_TEXT_LEN - 1] = '\0';
	}

	snprintf(filename, 6 + FIRST_TEXT_LEN, "%02d-%s.md", get_full_date().day, first_text);

	const size_t full_size = sizeof(full_dir) + sizeof(filename);
	char full_path[full_size];
	strncpy(full_path, full_dir, full_size);
	strncat(full_path, filename, full_size);

	// ensure the directories exist
	char cog_dir[40];
	strncpy(cog_dir, home, 40);
	strcat(cog_dir, "/cogs");
	ensure_dir(cog_dir);

	char year_path[44];
	snprintf(year_path, 44, "%s/%d", cog_dir, get_full_date().year);
	ensure_dir(year_path);

	ensure_dir(full_dir);

	// prepare pre-text
	char metadata[50];
	strncpy(metadata, "---\n", 49);
	if (strncmp(title, "", 1) != 0) {
		strncat(metadata, "title: ", 49);
		strncat(metadata, title, 49);
		strncat(metadata, "\n", 49);
	}

	time_t t = time(NULL);
	struct tm tm = *gmtime(&t);
	char date_str[26]; // For example: 2020-05-23T16:01:58+03:00
	snprintf(date_str, 26, "%d-%02d-%02dT%02d:%02d:%02d+00:00", // TODO support other time zones
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday + 1,
			tm.tm_hour + 1, tm.tm_min + 1, tm.tm_sec + 1 // TODO verify +1 here
			);
	strncat(metadata, "date: ", 49);
	strncat(metadata, date_str, 49);

	strncat(metadata, "\n---\n", 49);
	metadata[49] = '\0'; // I don't know if this is necessary or not

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

	printf("Saved to %s\n", full_path);

	return strlen(text);
}
