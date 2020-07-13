// SPDX-License-Identifier: BSD-2-Clause
#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>

// human-readable dates
typedef struct date {
	int year;
	int mon;
	int mday;
	int hour;
	int min;
	int sec;
} date;

unsigned long utils_djb2_hash(char *);
char *utils_timestamp(struct date);
void utils_ensure_dir(char *);
struct date utils_full_date(struct tm);

#endif
