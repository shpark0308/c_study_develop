#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/epoll.h>
#include <pthread.h>
#include <vector>

#define MAX_CLIENT			(1)
#define BUF_SIZE			(1024)
#define MAX_EPOLL_EVENTS	(20)
#define EPOLL_WAIT_TIMEOUT	(-1) // (5000) 5초

std::vector<int> clnt_vec;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void print_accept_info(int fd, struct sockaddr_in* clnt_addr)
{
	char clnt_ip[INET_ADDRSTRLEN];
	memset(clnt_ip, 0, INET_ADDRSTRLEN);

	inet_ntop(AF_INET, &clnt_addr->sin_addr, clnt_ip, INET_ADDRSTRLEN);

	int clnt_port = ntohs(clnt_addr->sin_port);

	printf("=============================\n");
	printf("[clnt:%d] %s:%d connected()\n", fd, clnt_ip, clnt_port);
	printf("=============================\n");
	
}

void send_to_clients(int clnt_sock, char msg[])
{
	for (auto iter = clnt_vec.begin(); iter != clnt_vec.end(); iter++)
	{
		if (*iter == clnt_sock)
			continue;

		printf("[clnt:%d] send msg : %s\n", *iter, msg);
		int send_len = send(*iter, msg, strlen(msg), 0);
		if (send_len<0) err_handle("send to clients failed");

	}
}

int main()
{		
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv_sock create failed");

	struct sockaddr_in serv_addr;

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, (socklen_t)sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet < 0) err_handle("listen() erro");

	// (1). epoll_create
	int epoll_fd = epoll_create(1);

	// (2). 관찰 대상으로 등록
	epoll_event serv_ev;
	serv_ev.events = EPOLLIN;
	serv_ev.data.fd = serv_sock;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, (struct epoll_event*)&serv_ev);

	epoll_event* observ_ev = (struct epoll_event*)malloc(sizeof(epoll_event)*MAX_EPOLL_EVENTS);
	memset(observ_ev, 0, sizeof(epoll_event)*MAX_EPOLL_EVENTS);
	
	int clnt_idx = 0; // map_key 값

	while(true)
	{
		int fd_num = epoll_wait(epoll_fd, (struct epoll_event*)observ_ev, MAX_EPOLL_EVENTS, EPOLL_WAIT_TIMEOUT );
		if (fd_num == -1)
		{
			err_handle("epoll_wait() failed");
		}
		else if (fd_num == 0)
		{
			printf("epoll_timeout()\n");
			break;
		}

		for (int i=0; i<fd_num; i++)
		{
			if (observ_ev[i].data.fd == serv_sock)
			{
				struct sockaddr_in clnt_addr;
				int clnt_addr_size = sizeof(clnt_addr);
				
				int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
				if (clnt_sock<0) err_handle("clnt_sock() accept failed");


				print_accept_info(clnt_sock, &clnt_addr);

				epoll_event clnt_event;
				clnt_event.events = EPOLLIN;
				clnt_event.data.fd = clnt_sock;

				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sock, &clnt_event);

				clnt_vec.push_back(clnt_sock);
				
			}
			else
			{
				int clnt_fd = observ_ev[i].data.fd;
					
				char recv_buff[BUF_SIZE];
				memset(recv_buff, 0, BUF_SIZE);

				int recv_len = recv(clnt_fd, recv_buff, BUF_SIZE, 0);
				if (recv_len<0) // close
				{
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clnt_fd, &observ_ev[i]);
					delete(observ_ev);
					
					close(clnt_fd);
					break;
				}

				send_to_clients(clnt_fd, recv_buff);
				if (strncmp(recv_buff,"END", 3) == 0)
				{
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clnt_fd, &observ_ev[i]); // 해당 client 만 종료
					delete(observ_ev);
					
					close(clnt_fd);
					break;
				}
				
			}
		}

		
	}

	close(serv_sock);

	if (observ_ev)
		free(observ_ev);
	
	return 0;
}
