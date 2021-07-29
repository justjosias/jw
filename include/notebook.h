#ifndef NOTEBOOKS_H_
#define NOTEBOOKS_H_

#include <stdbool.h>

#define NOTEBOOK_CONFIG_SIZE 512

// The configuration parsed from notebook.yaml
struct notebook_config {
	char name[NOTEBOOK_CONFIG_SIZE];
	char description[NOTEBOOK_CONFIG_SIZE];
	char post_path[NOTEBOOK_CONFIG_SIZE];
	bool metadata;
};

// Information about a notebook returned when loaded
struct notebook {
	struct notebook_config config;
	const char *id;
	const char *path; // the full path to the directory
};

int notebook_new(const char *);
struct notebook notebook_load(const char*, int*);
void notebook_list();

#endif
