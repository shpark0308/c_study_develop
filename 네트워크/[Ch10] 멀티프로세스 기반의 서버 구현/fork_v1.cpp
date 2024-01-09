#include <stdio.h>
#include <unistd.h>

int gvalue = 10;

int main()
{
	int lvalue = 20;
	lvalue +=5;

	gvalue ++;

	pid_t pid = fork();
	printf("pid: %d\n", pid);

	if (pid == 0)
		gvalue++;
	else
		lvalue++;

	printf("gval: %d lval: %d\n", gvalue, lvalue);
	return 0;
}
