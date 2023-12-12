#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/epoll.h>

#define BUF_SIZE (1024)
#define EPOLL_SIZE	(2)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv_sock create() failed");

	struct sockaddr_in serv_addr;

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (bRet <0) err_handle("connect failed");

	int epoll_fd = epoll_create(1);

	epoll_event serv_event;
	serv_event.events = EPOLLIN;
	serv_event.data.fd = serv_sock;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &serv_event);

	epoll_event stdin_event;
	stdin_event.events = EPOLLIN;
	stdin_event.data.fd = STDIN_FILENO;
	
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_event);

	epoll_event* observ_ev = (epoll_event*) malloc(sizeof(epoll_event)*2);
	
	while(true)
	{
		int fd_num = epoll_wait(epoll_fd, observ_ev, EPOLL_SIZE, -1);
		if(fd_num<0)
		{
			err_handle("epoll_wait failed");
		}
		else if (fd_num == 0)
		{
			err_handle("epoll timeout error");
		}

		for (int i=0; i<fd_num; i++)
		{
			if (observ_ev[i].data.fd == serv_sock)
			{
				char recv_buff[BUF_SIZE];
				memset(recv_buff, 0, BUF_SIZE);

				int recv_len = recv(serv_sock, recv_buff, BUF_SIZE, 0);
				if (recv_len<0) break;

				printf("(recv) : %s\n", recv_buff);
			}
			else if (observ_ev[i].data.fd == STDIN_FILENO);
			{
				char send_buff[BUF_SIZE];
				memset(send_buff, 0, BUF_SIZE);
				
				int read_len = read(STDIN_FILENO, send_buff, BUF_SIZE);

				int send_len = send(serv_sock, send_buff, strlen(send_buff), 0);
				printf("send success\n");
				if (send_len<0 || strncmp(send_buff, "END", 3)==0) break;
			}
		}
		
	}

	close(serv_sock);
	return 0;
}
