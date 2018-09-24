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

char *testcase_description = "poll of 128 fds";

void testcase_prepare(unsigned long nr_tasks)
{
	struct rlimit rlim;
	int nr_procs = sysconf(_SC_NPROCESSORS_CONF);

	getrlimit(RLIMIT_NOFILE, &rlim);
	rlim.rlim_cur = (NR_FILES + 10) * nr_procs;
	rlim.rlim_max = (NR_FILES + 10) * nr_procs;
	assert(setrlimit(RLIMIT_NOFILE, &rlim) == 0);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int i;
	int fds[NR_FILES*2];

	for (i = 0; i < NR_FILES; i++) {
		assert(socketpair(AF_UNIX, SOCK_DGRAM, 0, &fds[2*i]) >= 0);
		assert(fds[2*i] >= 0 && fds[2*i + 1] >= 0);
	}

	while (1) {
		struct pollfd pfd[NR_FILES];

		memset(&pfd, 0, sizeof(pfd));

		for (i = 0; i < NR_FILES; i++) {
			pfd[i].fd = fds[2*i];
			pfd[i].events = POLLOUT;
		}

		assert(poll(pfd, NR_FILES, 0) >= 0);

		(*iterations)++;
	}
}
