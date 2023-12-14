#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>
#include <poll.h>

#define BUF_SIZE	(1024)

pollfd poll_fds[2];

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int clnt_sock = socket(	AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clnt_sock<0) err_handle("socket create() failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect() failed");

	poll_fds[0].fd = clnt_sock;
	poll_fds[0].events = POLLIN;

	poll_fds[1].fd = STDIN_FILENO;
	poll_fds[1].events = POLLIN;

	while(true)
	{
		int poll_fd = poll(poll_fds, 2, -1);
		if (poll_fd == -1)
			err_handle("poll error");
		else if (poll_fd == 0)
		{
			printf("==== timeout ==== \n");
			break;
		}

		if (poll_fds[0].revents == POLLIN)
		{
			char recv_buff[BUF_SIZE];
			memset(recv_buff, 0, BUF_SIZE);

			int recv_len = recv(clnt_sock, recv_buff, BUF_SIZE, 0);
			if (recv_len <0)
			{
				break;
			}

			printf("(recv) : %s\n", recv_buff);
		}

		if (poll_fds[1].revents == POLLIN)
		{
			char send_buff[BUF_SIZE];
			memset(send_buff, 0, BUF_SIZE);

			int read_len = read(STDIN_FILENO, send_buff, BUF_SIZE);

			int send_len = send(clnt_sock, send_buff, strlen(send_buff), 0);

		}
	}

	close(clnt_sock);


	return 0;
}
