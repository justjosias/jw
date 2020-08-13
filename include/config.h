// SPDX-License-Identifier: BSD-2-Clause
#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION "0.1.0"
// the maximum length of the text extracted from the body for a title
#define FIRST_TEXT_LEN 30

// the cog directory relative to home
#define CONFIG_POST_DIR "/jw"
// cog list relative to cog dir
#define CACHE_LIST_PATH "/posts.txt"

char *config_dir_get(const char*);

#endif
