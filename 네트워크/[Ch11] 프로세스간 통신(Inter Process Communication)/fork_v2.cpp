#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (30)

int main()
{
	int fds[2];
	pipe(fds);

	pid_t pid = fork();

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	if (pid == 0)
	{
		printf("====  Child Proc start  ====\n");

		strcpy(msg, "Who are you?");
		write(fds[1], msg, strlen(msg));

		//sleep(5);
		memset(msg, 0, BUF_SIZE);
		read(fds[0], msg, sizeof(msg));
                printf("Child Proc output: %s\n", msg);

		printf("====  Child Proc stop  ====\n");
	}
	else
	{
		printf("====  Parent Proc start  ====\n");

		read(fds[0], msg, sizeof(msg));
		printf("Parent Proc output: %s\n", msg);

		memset(msg, 0, BUF_SIZE);
		strcpy(msg, "Thank you for you message");

		//write(fds[1], msg, strlen(msg));

		printf("==== Parent Proc stop  ====\n");
	}
	return 0;
}
