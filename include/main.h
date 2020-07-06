#ifndef MAIN_H_
#define MAIN_H_

struct cog {
	struct metadata metadata;
	char *file;
};

struct metadata {
	char *title;
	struct date date;
}

#endif
