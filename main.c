#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "write.h"

void print_help()
{
	printf("ESerm version %s\n\n", VERSION);
	printf("  help            display help text\n");
	printf("  puto [TITLE]    make a post (with optional title)\n");
}

char *get_text(char *text, int len)
{
	int available = len;

	char line[500]; // TODO increase flexibility
	while ((fgets(line, 500, stdin))) {
		if (strlen(line) > available) {
			text = realloc(text, len + 500);
		}

		strncat(text, line, len - 1);
		available -= strlen(line);
	}

	return text;
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "Usage: eserm ARGS\n");
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "puto") == 0) {
		char *text = (char *)malloc(1000);
		int len = 1000;

		if (argc == 3)
			write(get_text(text, len), argv[2]);
		else
			write(get_text(text, len), "");
	} else if (strcmp(argv[1], "help") == 0) {
		print_help();
	} else {
		fprintf(stderr, "Unknown option: %s\n", argv[1]);
	}

	return EXIT_SUCCESS;
}
