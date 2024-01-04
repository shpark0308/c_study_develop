#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE (1024)
#define MAX_CLIENT (5)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void print_clnt_info(struct sockaddr_in* clnt_addr)
{
	printf("client %s:%p (connected)\n", inet_ntoa(clnt_addr->sin_addr), ntohs(clnt_addr->sin_port));
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("tcp create()\n");

	struct sockaddr_in serv_addr, clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	printf("=======  TCP CONNECTION  =======\n");
	printf("Address : %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	printf("================================\n");

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet <0) err_handle("bind() error");

	listen(serv_sock, MAX_CLIENT);

	fd_set read_fds, temp_fds;

	FD_ZERO(&read_fds);
	FD_SET(serv_sock, &read_fds);

	int clnt_sock = 0;

	int fd_max = serv_sock;

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);
	while(true)
	{
		temp_fds = read_fds;

		struct timeval timeout;
		timeout.tv_sec = 0; // 5초
		timeout.tv_usec = 5000;

		int fd_num = select(fd_max + 1, &temp_fds, nullptr, nullptr, nullptr);
		if (fd_num == -1) err_handle("select() error");
		else if (fd_num == 0) 
		{
			//printf("select() timeout\n");
			continue;
		}

		if (FD_ISSET(serv_sock, &temp_fds))
		{
			printf("connection serv_sock react\n");
			
			clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
			if (clnt_sock<0) err_handle("accept() error");
			
			FD_SET(clnt_sock, &read_fds);
			if (clnt_sock>fd_max) fd_max = clnt_sock;

			print_clnt_info(&clnt_addr);
		}

		// clnt
		for (int i=0; i<fd_max+1; i++)
		{
			if (i!=clnt_sock) continue;

			int clnt_fd = i;

			printf("clnt_sock react : (%d)\n", i);
			int recv_len = recv(clnt_fd, msg, BUF_SIZE, 0);
			if (recv_len<0) break;

			msg[recv_len] = 0;
			printf("[recv] : %s\n", msg);

			char send_msg[BUF_SIZE];
			memset(send_msg, 0, BUF_SIZE);
			snprintf(send_msg, BUF_SIZE, "(serv echo) %s\n", msg);
			int send_len = send(clnt_fd, send_msg, strlen(send_msg), 0);
		}

	}

	close(clnt_sock);
	close(serv_sock);
	return 0;
}
