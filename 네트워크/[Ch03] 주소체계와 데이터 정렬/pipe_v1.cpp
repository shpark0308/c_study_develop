#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (1024)

int main()
{
	int fds[2];
	pipe(fds);

	pid_t pid = fork();

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	if (pid==0)
	{
		read(fds[0], msg, BUF_SIZE);
		puts(msg);
		return 0;
	}

	strcpy(msg, "Who are you?");
	write(fds[1], msg, strlen(msg));

	return 0;
}
