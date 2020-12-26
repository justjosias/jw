// General standalone utilities not directly related to main functions.
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h> // for getenv
#include <string.h>

#include "main.h"
#include "utils.h"

struct utils_date utils_full_date(struct tm tm)
{
	struct utils_date date_ = {
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour + 1,
		tm.tm_min + 1,
		tm.tm_sec + 1,
	};

	return date_;
}

char *utils_timestamp(struct utils_date date)
{
	static char date_str[26]; // For example: 2020-05-23T16:01:58+03:00
	snprintf(date_str, 26, "%d-%02d-%02dT%02d:%02d:%02d+00:00", // TODO support other time zones
			date.year, date.mon, date.mday,
			date.hour, date.min, date.sec
			);

	return date_str;
}

// make sure a directory exists
int utils_ensure_dir(const char *path)
{
	struct stat st = {0};
	if (stat(path, &st) == -1) {
		return mkdir(path, 0700);
	}
	return 0;
}

// make directories recursively (must be able to modify path but is changed back)
// It ignores the last item unless it ends with a separator as well.
// "path/to/something" will make directories for "path" and "to", but not
// for "something", but "path/to/something/" will.
// It took way too long to write this function
int utils_mkdir(char *path)
{
	int err = 0;
	char *s;
	if ((s = strstr(path, SEPARATOR)) != NULL) {
		while (s != NULL) {
			*s = '\0';
			err -= utils_ensure_dir(path);
			*s = SEPARATOR[0]; // change it back
			s = strstr(s + 1, SEPARATOR);
			if (s == NULL)
				return 0;
		}
	} else {
		return utils_ensure_dir(path);
	}

	return 0;
}

// a simple hashing function. Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned long utils_djb2_hash(char *str)
{
	unsigned long hash = 5381;

	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	
	return hash;
}

// gets the default text editor based on environment variables, or else vi
char *utils_default_editor()
{
	char *editor;
	if ((editor = getenv("VISUAL")) != NULL) return	editor;
	if ((editor = getenv("EDITOR")) != NULL) return	editor;

	static char e[] = "vi";
	return e;
}

// Only gets basic yaml information in the form of `key: value`.
char *utils_mini_yaml(FILE *fp, const char *key, char value[512]) {
	fseek(fp, 0, SEEK_SET);
	if (fp == NULL)
		return NULL;

	char line[512];
	size_t key_len = strlen(key);
	while (fgets(line, 512, fp)) {
		if (strncmp(line, key, key_len) == 0) {
			size_t i = 0;
			while (line[i] != ':' && line[i] != '\n')
				i++;
			i++;
			if (line[i] == ' ')
				i++;
			if (line[i] == '\n')
				continue;

			size_t x = 0;
			while (line[i] != '\n' && x < sizeof(line) - 1) {
				value[x] = line[i];
				i++;
				x++;
			}

			value[x] = '\0';
			return value;
		}
	}

	return NULL;
}
