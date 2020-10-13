#ifndef MAIN_H_
#define MAIN_H_

#include "utils.h" // for date

#define VERSION "0.1.0"
// the maximum length of the text extracted from the body for a title
#define FIRST_TEXT_LEN 30

// the cog directory relative to home
#define CONFIG_POST_DIR "jw"
// cog list relative to cog dir
#define CACHE_LIST_PATH "posts.txt"

typedef struct metadata {
	date date;
} metadata;

struct post {
	metadata metadata;
	char *file;
};

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

#endif
