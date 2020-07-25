// SPDX-License-Identifier: BSD-2-Clause
// General standalone utilities not directly related to main functions.
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include "utils.h"

struct date utils_full_date(struct tm tm)
{
	struct date date = {
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday + 1,
		tm.tm_hour + 1,
		tm.tm_min + 1,
		tm.tm_sec + 1,
	};

	return date;
}

char *utils_timestamp(struct date date)
{
	static char date_str[26]; // For example: 2020-05-23T16:01:58+03:00
	snprintf(date_str, 26, "%d-%02d-%02dT%02d:%02d:%02d+00:00", // TODO support other time zones
			date.year, date.mon, date.mday,
			date.hour, date.min, date.sec
			);

	return date_str;
}

// make sure a directory exists
void utils_ensure_dir(char *path)
{
	struct stat st = {0};
	if (stat(path, &st) == -1) {
		mkdir(path, 0700);
	}
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

