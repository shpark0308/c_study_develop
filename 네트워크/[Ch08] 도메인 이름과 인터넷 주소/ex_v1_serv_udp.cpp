#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUF_SIZE (1024)

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
	int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serv_sock<0) err_handle("udp socket() created failed");

	struct sockaddr_in serv_addr, clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	const char* ip = "127.0.0.1";
	const char* port = "8088";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	printf("=======  UDP CONNECTION  =======\n");
	printf("Address : %s:%d\n", inet_ntoa(serv_addr.sin_addr), htons(serv_addr.sin_port));
	printf("================================\n");

	fd_set read_fds, temp_fds;

	FD_ZERO(&read_fds);
	FD_SET(serv_sock, &read_fds);

	int fd_max = serv_sock;

	struct timeval timeout;

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	while(true)
	{
		temp_fds = read_fds;
		/*if (!FD_ISSET(serv_sock, &temp_fds))
		{
			printf("serv_sock, is not setted\n");
			FD_SET(serv_sock, &read_fds);
			continue;
		}*/

		timeout.tv_sec = 0;
		timeout.tv_usec = 0000;

		int fd_num = select(fd_max+1, &read_fds, nullptr, nullptr, nullptr);
		if (fd_num == -1) err_handle("select() error");
		else if (fd_num == 0)
		{
			//printf("select() timeout!\n");
			continue;
		}

		if (FD_ISSET(serv_sock, &read_fds))
		{
			printf("serv_sock reacted!\n");
			printf("I'am waiting....\n");
			int read_len = recvfrom(serv_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
			printf("read_len : %d\n", read_len);
			if (read_len<0)
				break;
			msg[read_len]=0;

			print_clnt_info(&clnt_addr);
			printf("[recv] : %s\n", msg);

			char send_msg[BUF_SIZE];
			memset(send_msg, 0, BUF_SIZE);

			snprintf(send_msg, BUF_SIZE, "(serv echo) %s\n", msg);
#if 0
			int send_len = sendto(serv_sock, send_msg, strlen(send_msg), 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
			printf("send_len : %d\n", send_len);
#endif
		}


	}

	close(serv_sock);
	return 0;
}
