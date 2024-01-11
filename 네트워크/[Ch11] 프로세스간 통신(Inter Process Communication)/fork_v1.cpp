#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (30)

int main()
{
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];

	pipe(fds);
	pid_t pid = fork();
	if (pid != 0)
	{
		printf("===== Child Process =====\n");
		//sleep(10);
		write(fds[1], str, strlen(str));
	}
	else
	{
		printf("===== Parent Process =====\n");
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	printf(" %d exit \n", pid);
	return 0;
}
