// SPDX-License-Identifier: BSD-2-Clause
#ifndef NOTEBOOKS_H_
#define NOTEBOOKS_H_

struct stats {
	size_t posts;
	size_t words;
	size_t tags;
	// maybe add start date and end date if it's not too hard
};

int notebook_new(const char *);
struct stats notebook_stats(const char *);

#endif
