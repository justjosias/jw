// SPDX-License-Identifier: BSD-2-Clause
#ifndef SEARCH_H_
#define SEARCH_H_

struct result {
	char path[256];
	//char first_text[51];
	//size_t occurances;
	//size_t placements[]; // where each occurance is found in the document
};

struct result *search(const char *notebook, const char *query, size_t *count);

#endif
