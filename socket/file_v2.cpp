#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	const char* file_name = "write.txt";
	int fd = open(file_name, O_RDONLY);
	if (fd<0)
	{
		fputs("file open failed\n", stderr);
		exit(1);
	}

	printf("file descripter : (%d)\n",fd);

	char read_buf[100];
	int read_size = 0;
	while(read_size = read(fd, read_buf, sizeof(read_buf)))
	{
		int offset = lseek(fd, 0, SEEK_CUR);
		printf("현 위치 : (%d)\n", offset);
		if (offset==300)
			read_buf[0] = 'A';
		printf("file_descripter : [%d] %s\n", fd, read_buf);
	}

	close(fd);
	return 0;
}
