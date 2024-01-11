#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (30)

int main()
{
	int fds1[2], fds2[2];

	pipe(fds1);
	pipe(fds2);
	printf("fds1: (%d, %d) fds2: (%d, %d)\n", fds1[0], fds1[1], fds2[0], fds2[1]);
	pid_t pid = fork();

	if (pid == 0)
	{
		printf("==== Child start ====\n");

		const char* msg1 = "Who are you?";
		write(fds1[1], msg1, strlen(msg1));

		char msg2[BUF_SIZE];
		memset(msg2, 0, BUF_SIZE);
		read(fds2[0], msg2, BUF_SIZE);
		
		printf("Child Proc output: %s\n", msg2);
		
	}
	else
	{
		printf("==== Parent start ====\n");
		char msg3[BUF_SIZE];
		memset(msg3, 0, BUF_SIZE);

		read(fds1[0], msg3, BUF_SIZE);

		printf("Parent Proc output: %s\n", msg3);

		const char* msg4 = "Thank you for you message";
		write(fds2[1], msg4, strlen(msg4));
	}

	return 0;
}
