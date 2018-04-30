#include <sys/stat.h>

#include <fcntl.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file fstatat";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[FILESIZE];
	char tmpfile[] = "/var/tmp/willitscale.XXXXXX";
	char *fname, *tmpfile2;
	int fd = mkstemp(tmpfile);
	int dfd;
	struct stat sb;

	tmpfile2 = strdup(tmpfile);
	fname = basename(tmpfile2);

	assert((dfd = open("/var/tmp", O_RDONLY, 0)) != -1);
	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	close(fd);
	while (1) {
		assert (fstatat(dfd, fname, &sb, 0) == 0);

		(*iterations)++;
	}
	unlink(tmpfile);
}
