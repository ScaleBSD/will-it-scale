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
	char buf[FILESIZE];
	char tmpdir[] = "/var/tmp/willitscale.XXXXXX";
	char tmpfile[64];
	char *fname, *tmpfile2;
	int fd = mkstemp(tmpdir);
	int dfd;
	struct stat sb;

	unlink(tmpdir);
	close(fd);
	assert(mkdir(tmpdir, 0777) != -1);
	snprintf(tmpfile, sizeof(tmpfile), "%s/tmp.XXXXX", tmpdir);
	assert((fd = mkstemp(tmpfile)) > 0);
	tmpfile2 = strdup(tmpfile);
	fname = basename(tmpfile2);
	assert((dfd = open(tmpdir, O_RDONLY, 0)) != -1);
	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	close(fd);
	while (1) {
		assert (fstatat(dfd, fname, &sb, 0) == 0);

		(*iterations)++;
	}
	unlink(tmpfile);
	unlink(tmpdir);
}
