#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>

#define BUF_SIZE (1024)

int main()
{
	fd_set reads, temps;

	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);

	FD_ZERO(&reads);
	FD_SET(STDIN_FILENO, &reads);

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;

	temps = reads;

	while(true)
	{
		reads = temps;
		printf("%u %u \n", timeout.tv_sec, timeout.tv_usec); // 여기 timeout 을 reset 해주어야 한다.

		int fd_num = select(STDIN_FILENO+1, &reads, NULL, NULL, &timeout);
		if (fd_num == -1)
		{
			printf("select() error\n");
			break;
		}
		else if (fd_num == 0)
		{
			printf("select() timeout\n");
			break;
		}
		else
		{
			if (FD_ISSET(STDIN_FILENO, &reads))
			{
				int read_size = read(0, buffer, BUF_SIZE);
				buffer[read_size] = 0;
				printf("message : %s\n", buffer);
			}
		}
	}
	return 0;
}
