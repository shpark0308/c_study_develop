#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <poll.h>

#define BUF_SIZE 1024

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serv_sock<0) err_handle("sock create() failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8089";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	// serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (bRet< 0) err_handle("bind error()");

	struct sockaddr_in clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	struct pollfd poll_fd[1];
	poll_fd[0].fd = serv_sock;
	poll_fd[0].events = POLLIN;

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	while(1)
	{
		int retval = poll(poll_fd, 1, -1);

		printf("retval : %d\n", retval);

		if (poll_fd[0].revents == POLLIN)
		{
			int recv_len = recvfrom(poll_fd[0].fd, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
			msg[recv_len] = 0;
			printf("recv : %s\n", msg);

			int send_len = sendto(poll_fd[0].fd, msg, strlen(msg), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
		}
	}

	close(serv_sock);
	return 0;
}
