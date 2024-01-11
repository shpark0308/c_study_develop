#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (1024)

int main()
{
	int fds[2], fds1[2];
	pipe(fds);
	pipe(fds1);

	pid_t pid = fork();

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	if (pid == 0)
	{
		fputs("[Child]: ", stdout);
		fgets(msg, BUF_SIZE, stdin);

		write(fds[1], msg, strlen(msg));

		int read_size = read(fds1[0], msg, BUF_SIZE);
		msg[read_size] = 0;

		printf("[Child]: %s", msg);

		memset(msg, 0, BUF_SIZE);

		fputs("[Child]: ", stdout);
		fgets(msg, BUF_SIZE, stdin);

		write(fds[1], msg, strlen(msg));

		printf("=== child Process end() ===\n");
	}
	else
	{
		int read_size = read(fds[0], msg, BUF_SIZE);
		msg[read_size] = 0;

		printf("[Parent]: %s", msg);

		memset(msg, 0, BUF_SIZE);

		fputs("[Parent]: ", stdout);
		fgets(msg, BUF_SIZE, stdin);

		write(fds1[1], msg, strlen(msg));

		read_size = read(fds[0], msg, BUF_SIZE);
		msg[read_size] = 0;

		printf("[Parent]: %s", msg);

		printf("=== Parent Process end() ===\n");
	}
	return 0;
}
