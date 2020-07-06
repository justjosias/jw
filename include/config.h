#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION "0.1.0"
// the maximum length of the text extracted from the body for a title
// (if another title is not provided)
#define FIRST_TEXT_LEN 20

// the cog directory relative to home
#define CONFIG_COG_DIR "/cogs"
// cog list relative to cog dir
#define CACHE_LIST_PATH "/cogs.txt"

char *config_cog_dir_get(void);

#endif
