#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>
#include <wait.h>

#define BUF_SIZE (1024)

void exit_handler(int flag)
{
	int status;

	int pid = waitpid(-1, &status, 0);
	printf("==== pid(%d) exit ====\n", pid);
}

int main()
{
	int fds1[2], fds2[2];
	pipe(fds1);
	pipe(fds2);

	struct sigaction sig_struct;
	sig_struct.sa_handler = exit_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	sigaction(SIGCHLD, &sig_struct, 0);

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	pid_t pid = fork();
	if (pid == 0)
	{
		strcpy(msg, "Who are you?");
		write(fds1[1], msg, strlen(msg));

		read(fds2[0], msg, BUF_SIZE);
		printf("Child proc output: %s\n", msg);
	}
	else
	{
		read(fds1[0], msg, BUF_SIZE);
		printf("Parent proc output: %s\n", msg);

		strcpy(msg, "Thank you for your message");
		write(fds2[1], msg, strlen(msg));
		sleep(3);
	}
	return 0;
}
