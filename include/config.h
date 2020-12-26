#ifndef CONFIG_H_
#define CONFIG_H_

// the configuration directory relative to home
#define CONFIG_DIR ".config/jw/"

struct config {
	// notebooks directory relative to home
	char root[256];
	// default text editor, otherwise $VISUAL or $EDITOR
	char editor[256];
};

struct config config_get(void);
char *config_root_get(const char*);

#endif
