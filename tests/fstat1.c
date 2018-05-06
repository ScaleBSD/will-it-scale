#include <sys/stat.h>

#include <fcntl.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file fstat";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[FILESIZE];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	struct stat sb;

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	unlink(tmpfile);

	while (1) {
		assert(fstat(fd, &sb) == 0);

		(*iterations)++;
	}
}
