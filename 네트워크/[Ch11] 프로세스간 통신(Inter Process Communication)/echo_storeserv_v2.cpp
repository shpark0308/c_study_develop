#include <stdio.h>
#include <unistd.h>

int main()
{
	for (int i=1; i<=10; i++)
	{
		printf("[%d]\n", i);
		if (i==3)
		{
			pid_t pid = fork();
			if (pid !=0)
			{
				pid_t pid2 = fork();
				if (pid2==0)
				{
					printf("child proc #2\n");
					return 0;
				}
				else
				{
					while(true)
					{
						continue;
					}
				}
			}
			if (pid==0)
			{
				printf("child proc #1\n");
				return 0;
			}
		}
	}
	printf("==== Main Proc End ====\n");
	return 0;
}
