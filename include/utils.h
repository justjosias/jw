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
int utils_ensure_dir(const char *);
int utils_mkdir(char *);
struct date utils_full_date(struct tm);
char *utils_default_editor();
char *utils_mini_yaml(FILE *, const char *, char [512]);

#endif
