#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <poll.h>

#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv socket() create failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet < 0) err_handle("connect() error");

	struct pollfd poll_fds[2];
	poll_fds[0].fd = serv_sock;
	poll_fds[0].events = POLLIN;

	poll_fds[1].fd = STDIN_FILENO;
	poll_fds[1].events = POLLIN;

	while(true)
	{
		int fd_num = poll(poll_fds, 2, -1);
		if (fd_num<0)
		{
			err_handle("poll_fds error");
			break;
		}
		else if (fd_num == 0)
		{
			err_handle("poll_fds timeout");
			break;
		}

		if (poll_fds[0].revents == POLLIN)
		{
			printf("-----------------------> \n\n");
			char recv_buf[BUF_SIZE];
			memset(recv_buf, 0, BUF_SIZE);

			int recv_len = recv(poll_fds[0].fd, recv_buf, BUF_SIZE, 0);
			if (recv_len<0) err_handle("clnt recv failed");

			printf("(recv) : %s\n", recv_buf);
			if (strncmp(recv_buf, "END", 3)==0)
			{
				break;
			}
		}

		if (poll_fds[1].revents == POLLIN)
		{
			char send_buf[BUF_SIZE];
			memset(send_buf, 0, BUF_SIZE);

			int read_len = read(STDIN_FILENO, send_buf, BUF_SIZE);

			int send_len = send(serv_sock, send_buf, strlen(send_buf), 0);
			if (send_len<0) err_handle("clnt send failed");

			if (strncmp(send_buf, "END", 3)==0)
			{
				break;
			}



		}

	}

	close(serv_sock);
	return 0;
}
