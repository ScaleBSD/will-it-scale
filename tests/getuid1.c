#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "getuid";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		getuid();

		(*iterations)++;
	}
}
