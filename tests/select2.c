#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <assert.h>

#define NR_FILES 128

char *testcase_description = "select of 128 fds";

void testcase_prepare(unsigned long nr_tasks)
{
	struct rlimit rlim;
	int nr_procs = sysconf(_SC_NPROCESSORS_CONF);

	getrlimit(RLIMIT_NOFILE, &rlim);
	rlim.rlim_cur = (2*NR_FILES + 10) * nr_procs;
	rlim.rlim_max = (2*NR_FILES + 10) * nr_procs;
	assert(setrlimit(RLIMIT_NOFILE, &rlim) == 0);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int i;
	int fds[NR_FILES*2];
	fd_set readfds;
	struct timeval timeout;

	FD_ZERO(&readfds);
	bzero(&timeout, sizeof(timeout));
	for (i = 0; i < NR_FILES; i++) {
		assert(socketpair(AF_UNIX, SOCK_DGRAM, 0, &fds[2*i]) >= 0);
		assert(fds[2*i] >= 0 && fds[2*i + 1] >= 0);
		FD_SET(fds[2*i], &readfds);
	}

	while (1) {

		assert(select(NR_FILES, &readfds, NULL, NULL, &timeout) >= 0);

		(*iterations)++;
	}
}
