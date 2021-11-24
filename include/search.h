#ifndef SEARCH_H_
#define SEARCH_H_

#include "notebook.h"

struct search_result {
	char path[512];
	//char first_text[51];
	//size_t occurances;
	//size_t placements[]; // where each occurance is found in the document
};

struct search_result *search(struct notebook notebook, const char query[100], size_t *count);

#endif
