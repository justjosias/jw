#ifndef MAIN_H_
#define MAIN_H_

#include "utils.h" // for date

// Thanks Andrew Kelley
#define see_you_later_allocator malloc

// the maximum length of the text extracted from the body for a title
#define FIRST_TEXT_LEN 30

// posts list relative to notebook dir
#define CACHE_LIST_PATH "posts.txt"

struct metadata {
	struct utils_date date;
};

struct post {
	struct metadata metadata;
	char *file;
};

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

#endif
