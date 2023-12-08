#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/epoll.h>

#define BUF_SIZE	(1024)
#define MAX_CLIENT	(5) ///// ¸¸¾à¿¡ ÀÌ max Ä¡¸¦ ³ÑÀ¸¸é å¾î¶»°Ô µÇ´ÂÁö ±Ã±Ý
#define EPOLL_SIZE	(20)

int clnt_fd[1024];

struct uData
{
	int fd;
	char name[80];
};

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_to_clients(int clnt_sock_idx, char* clnt_name, char send_msg[])
{
	char send_buff[BUF_SIZE];
	memset(send_buff, 0, BUF_SIZE);

	for (int i=0; i<1024; i++)
	{
		if (i==clnt_sock_idx)
			continue;

		if (clnt_fd[i] !=1)
			continue;

		printf("clnt[%d] connected\n", i);
		int send_len = send(i, send_buff, strlen(send_buff), 0);
		if (send_len<0) err_handle("send to clients error!");


	}
}

int main()
{
	memset(clnt_fd, -1, sizeof(int)*1024);

	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( serv_sock<0) err_handle("socket create failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet < 0) err_handle("bind() failed");

	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet < 0) err_handle("listen() failed");

	struct epoll_event accept_event;
	accept_event.events = EPOLLIN;
	accept_event.data.fd = serv_sock;

	int epoll_fd = epoll_create(1);
	if (epoll_fd < 0) err_handle("epoll_create() failed");

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &accept_event);

	struct epoll_event* events = (struct epoll_event*) malloc(sizeof(epoll_event)*EPOLL_SIZE);

	char send_buff[BUF_SIZE];
	memset(send_buff, 0, BUF_SIZE);

	char recv_buff[BUF_SIZE];
	memset(recv_buff, 0, BUF_SIZE);

	bool isExit = false;

	while(!isExit)
	{
		// int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
		int event_count = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);
		if (event_count<0)
		{
			printf("event_count(%d) : err occurs \n", event_count);
			isExit = true;
			break;
		}
		else if (event_count==0)
		{
			printf("event_count(%d) : timeout occurs \n", event_count);
			isExit = true;
			break;
		}

		printf("[Input] event_count : %d\n", event_count);
		for (int i=0; i<event_count; i++)
		{
			if (events[i].data.fd == serv_sock) // accept
			{
				struct sockaddr_in clnt_addr;
				int clnt_addr_size = sizeof(clnt_addr);

				int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
				if (clnt_sock<0) err_handle("accept() failed");
				clnt_fd[clnt_sock] = 1;

				struct uData* uData = (struct uData*) malloc(sizeof(struct uData));
				uData->fd = clnt_sock;

				const char* send_msg = "First insert you nickname : ";
				int send_len = send(clnt_sock, send_msg, strlen(send_msg), 0);
				if (send_len<0) // error()
				{
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, uData->fd, events);
					clnt_fd[clnt_sock] = -1;
					close(uData->fd);

					free(uData);
					err_handle("accept-send func error");
					isExit = true;
					break;
				}


				int recv_len = recv(clnt_sock, uData->name, sizeof(uData->name), 0);

				int len = strlen(uData->name);
				uData->name[len-1] = 0;
				if (recv_len<0) // close()
				{
					free(uData);
					isExit = true;
					break;
				}

				// add clnt_sock event
				struct epoll_event clnt_event;
				clnt_event.events = EPOLLIN;
				clnt_event.data.fd = clnt_sock;
				clnt_event.data.ptr = uData;

				epoll_ctl(epoll_fd,EPOLL_CTL_ADD, clnt_sock, &clnt_event);
			}
			else // recv data
			{
				struct uData* eData = (struct uData*)events[i].data.ptr;

				int recv_len = recv(eData->fd, recv_buff, BUF_SIZE,0);
				if (recv_len<0)
				{
					free(eData);
					isExit = true;
					break;
				}

				printf("clnt[%d]:(%s):   %s\n",eData->fd, eData->name, recv_buff);

				snprintf(send_buff, BUF_SIZE, "clnt(%d:%s) %s", eData->fd, eData->name, recv_buff);
				send_to_clients(eData->fd, eData->name, send_buff);

				if (strncmp(recv_buff, "END", 3)==0) // close();
				{
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, eData->fd, events);
					clnt_fd[eData->fd] = -1;
					close(eData->fd);

					free(eData);
					isExit = true;
					break;
				}
			}

			memset(send_buff, 0, BUF_SIZE);
			memset(recv_buff, 0, BUF_SIZE);
		}


	}
	return 0;
}
