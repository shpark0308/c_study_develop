#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char msg[1024];
	memset(msg, 0, sizeof(msg));

	while (true)
	{
		fgets(msg, sizeof(msg), stdin);
		printf("msg: %s size: %d\n", msg, strlen(msg));

		if (strncmp(msg, "exit", 4)==0)
		{
			break;
		}
	}
	return 0;
}
