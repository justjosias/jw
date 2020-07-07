// SPDX-License-Identifier: BSD-2-Clause
#ifndef UTILS_H_
#define UTILS_H_

// human-readable dates
struct date {
	int year;
	int mon;
	int mday;
	int hour;
	int min;
	int sec;
};

unsigned long utils_djb2_hash(char *);
char *utils_timestamp(struct date);
void utils_ensure_dir(char *);
struct date utils_full_date(struct tm);

#endif
