#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	const char* read_file_name = "read.txt";
	
	int fd = open(read_file_name, O_RDONLY);
	if (fd<0) err_handle("file open failed");

	char read_buf[1024];
	memset(read_buf, 0, sizeof(read_buf));
	int read_size = read(fd, read_buf, sizeof(read_buf));
	printf("(read) : %s (%d)\n", read_buf, read_size);

	close(fd);

	const char* write_file_name = "write.txt";

	int wd = open(write_file_name, O_WRONLY);
	if(wd<0) err_handle("file open failed");

	memcpy(read_buf + read_size+300, "ABCDEFG", 7);
	printf("read_buf : %s\n", read_buf);

	int write_size = write(wd, read_buf, sizeof(read_buf));
	close(wd);

	fd = open(write_file_name, O_RDONLY);
	if (fd<0) err_handle("file open failed");

	memset(read_buf, 0, sizeof(read_buf));
	read_size = read(fd, read_buf, sizeof(read_buf));
	printf("(read) : %s (%d)\n", read_buf, read_size);

	printf("[%c:%d]  [%c:%d]  [%c:%d]\n", read_buf[2], read_buf[2], read_buf[12], read_buf[12], read_buf[1000], read_buf[1000]);

	close(fd);


	return 0;
}
