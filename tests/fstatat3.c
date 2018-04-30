#include <sys/stat.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file fstatat";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpdir[] = "/var/tmp/willitscale.XXXXXX";
	char tmpdirrel[64];
	char *fname, *tmpdir2;
	int fd = mkstemp(tmpdir);
	int dfd;
	struct stat sb;

	unlink(tmpdir);
	close(fd);
	tmpdir2 = strdup(tmpdir);
	fname = basename(tmpdir2);
	snprintf(tmpdirrel, sizeof(tmpdirrel), "tmp/%s", fname);
	assert(mkdir(tmpdir, 0777) != -1);
	assert((dfd = open("/var", O_RDONLY, 0)) != -1);
	while (1) {
		assert (fstatat(dfd, tmpdirrel, &sb, 0) == 0);

		(*iterations)++;
	}
	unlink(tmpdir);
}
