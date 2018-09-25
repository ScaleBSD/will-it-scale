#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/socket.h>
#include <assert.h>

char *testcase_description = "select of 1 fd";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fds[2];
	fd_set readfds;
	struct timeval timeout;

	FD_ZERO(&readfds);
	assert(socketpair(AF_UNIX, SOCK_DGRAM, 0, fds) >= 0);
	assert(fds[0] >= 0 && fds[1] >= 0);
	FD_SET(fds[0], &readfds);
	bzero(&timeout, sizeof(timeout));
	
	while (1) {

		assert(select(1, &readfds, NULL, NULL, &timeout) >= 0);

		(*iterations)++;
	}
}
