#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <poll.h>
#include <vector>

#define BUF_SIZE	(1024)
#define MAX_CLIENT	(5)

struct pollfd poll_fds[MAX_CLIENT+1]; // +1 : serv_sock

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void print_accept_info(int clnt_sock, struct sockaddr_in* clnt_addr)
{
	char clnt_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clnt_addr->sin_addr,clnt_ip, INET_ADDRSTRLEN);

	int clnt_port = ntohs(clnt_addr->sin_port);
	
	printf("================================\n");
	printf("[clnt:%d] %s:%d connected\n", clnt_sock, clnt_ip, clnt_port);
	printf("================================\n");
}

void send_to_clients(int idx, char msg[])
{
	for (int i=1; i<=MAX_CLIENT; i++)
	{
		if (poll_fds[i].fd == -1)
			break;
		
		if (i == idx)
			continue;

		int send_len = send(poll_fds[i].fd, msg, strlen(msg), 0);
		if (send_len < 0) err_handle("send() error");

		printf("send to clnt:%d success\n", poll_fds[i].fd);
	}
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

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet < 0) err_handle("bind() error");

	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet < 0) err_handle("listen() error");

	// poll_fds Initialize
	
	poll_fds[0].fd = serv_sock;
	poll_fds[0].events = POLLIN;

	for(int i=1; i<=MAX_CLIENT; i++) poll_fds[i].fd = -1;

	int client = 1;

	while(true)
	{
		// int fd_num = poll(struct pollfd*, int max_fds, timeout);
		int fd_num = poll(poll_fds, MAX_CLIENT+1, -1);
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

		// 모든 파일디스크립터 확인
		if (poll_fds[0].revents == POLLIN)
		{
			struct sockaddr_in clnt_addr;
			int clnt_addr_size = sizeof(clnt_addr);
			
			int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
			if (clnt_sock<0) err_handle("clnt_sock accept failed");

			print_accept_info(clnt_sock, &clnt_addr);

			poll_fds[client].fd = clnt_sock;
			poll_fds[client].events = POLLIN;

			client++;
		}
		for (int i=1; i<=MAX_CLIENT; i++)
		{
			if (poll_fds[i].revents != POLLIN)
				continue;
			printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAA (%d)\n", poll_fds[i].fd);

			char recv_buf[BUF_SIZE];
			memset(recv_buf, 0, BUF_SIZE);
			
			int recv_len = recv(poll_fds[i].fd, recv_buf, BUF_SIZE, 0);
			if (recv_len<0) err_handle("clnt recv failed");

			printf("(recv) : %s\n", recv_buf);

			send_to_clients(i, recv_buf);
			if (strncmp(recv_buf, "END", 3)==0)
			{
				close(poll_fds[i].fd);
			
				memset(&poll_fds[i], 0, sizeof(struct pollfd));
				poll_fds[i].fd = -1; 

				break;
			}
		}
		
	}

	close(serv_sock);
	
	return 0;
}
