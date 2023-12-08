#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/times.h>

#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	// 1. 소켓 생성
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("socket() created failed");

	// 2. 연결 요청
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect() failed");

	// 3. data recv / send
	fd_set read_fds, old_fds;

	FD_ZERO(&read_fds);
	FD_SET(serv_sock, &read_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	int fd_max = serv_sock;

	struct timeval timeout;

	old_fds = read_fds;

	while(true)
	{
		read_fds = old_fds;

		timeout.tv_sec = 60;
		timeout.tv_usec = 5000;

		int fd_num = select(fd_max, &read_fds, NULL, NULL, NULL);
		if (fd_num<0)
		{
			printf("select() error\n");
			break;
		}
		else if (fd_num==0)
		{
			printf("select() timeout");
			break;
		}
		else // recv
		{
			if (FD_ISSET(serv_sock, &read_fds))
			{
				char recv_buf[BUF_SIZE];
				memset(recv_buf, 0, BUF_SIZE);

				int recv_len = recv(serv_sock, recv_buf, BUF_SIZE, 0);
				if(recv_len<0)
					break;

				printf("(recv) : %s\n", recv_buf);


			}
			else if (FD_ISSET(STDIN_FILENO, &read_fds))
			{
				printf("(send) : ");
				char send_buf[BUF_SIZE];
				int recv_len = read(STDIN_FILENO, send_buf, BUF_SIZE);
				printf("read from console : %s\n", send_buf);

				int send_len = send(serv_sock, send_buf, strlen(send_buf), 0);

			}
		}
	}

	// 4. close()
	close(serv_sock);

	return 0;
}

