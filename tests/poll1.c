#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/socket.h>
#include <assert.h>

char *testcase_description = "poll of 1 fd";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fds[2];

	assert(socketpair(AF_UNIX, SOCK_DGRAM, 0, fds) >= 0);
	assert(fds[0] >= 0 && fds[1] >= 0);


	while (1) {
		struct pollfd pfd[1];

		memset(&pfd, 0, sizeof(pfd));
		pfd[0].fd = fds[0];
		pfd[0].events = POLLOUT;

		assert(poll(pfd, 1, 0) >= 0);

		(*iterations)++;
	}
}
