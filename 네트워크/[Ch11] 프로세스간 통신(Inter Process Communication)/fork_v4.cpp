#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		printf("==== Child Proc ====\n");
		sleep(10);
		printf("==== Child exit() ====\n");
	}
	else
	{
		printf("==== Parent Proc ====\n");
		//wait(0);
		printf("==== Parent exit() ====\n");
	}
	return 0;
}
