#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>

char *testcase_description = "poll of 32 fd";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char *tmpname, tmpfile[] = "/tmp/willitscale.XXXXXX";
	int i, fds[32];

	for (i = 0; i < 32; i++) {
		tmpname = strdup(tmpfile); 
		fds[i] = mkstemp(tmpname);
		assert(fds[i] >= 0);
		unlink(tmpname);
	}
	while (1) {
		struct pollfd pfd[32];

		memset(&pfd, 0, sizeof(pfd));
		for (i = 0; i < 32; i++) {
			pfd[i].fd = fds[i];
			pfd[i].events = POLLOUT;
		}
		assert(poll(pfd, 32, 0) >= 0);

		(*iterations)++;
	}
}
