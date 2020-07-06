#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "cache.h"

bool test_cache_list_exists()
{
	return cache_list_exists("hi");
}

void test_cache_run_all()
{
	assert(test_cache_list_exists());

	printf("Passed all cache tests\n");
}
