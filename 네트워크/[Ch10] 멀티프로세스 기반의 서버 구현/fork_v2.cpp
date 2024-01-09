#include <stdio.h>
#include <unistd.h>

int gvalue = 10;

int main()
{
	pid_t pid;
	int lvalue = 20;
	gvalue++; lvalue+=5;

	pid = fork();
	if (pid == 0)
	{
		gvalue+=2; lvalue+=2;
	}
	else
	{
		gvalue-=2; lvalue-=2;
	}

	if (pid==0)
	{
		printf("Child Proc: [%d, %d]\n", gvalue, lvalue);
		/*while(true)
		{
			continue;
		}*/
	}
	else
	{
		printf("Parent Proc: [%d, %d]\n", gvalue, lvalue);
		sleep(30);
	}

	//sleep(60);
	printf("pid:%d process end\n",pid);
	return 0;
}
