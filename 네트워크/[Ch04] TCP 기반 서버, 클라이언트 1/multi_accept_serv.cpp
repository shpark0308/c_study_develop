#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <map>

#define MAX_CLIENT	(5)
#define BUF_SIZE	(1024)

static bool bExit;

typedef struct clnt_info
{
	int fd;
	unsigned long pthread_id;
	pthread_t* recv_pth;
	bool bExit;
} clnt_info_t;

std::map<int,clnt_info_t*> clnt_map;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void print_accept_info(int clnt_sock, struct sockaddr_in* clnt_addr)
{
	char clnt_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clnt_addr->sin_addr, clnt_ip, INET_ADDRSTRLEN);

	int clnt_port = ntohs(clnt_addr->sin_port);
	printf("==============================\n");
	printf("[clnt:%d]       %s:%d connected()\n", clnt_sock, clnt_ip, clnt_port);
	printf("==============================\n");

}

void send_to_clients(int clnt_sock, char recv_buf[])
{
	char send_buf[BUF_SIZE];
	memset(send_buf, 0, BUF_SIZE);

	snprintf(send_buf, BUF_SIZE, "[clnt:%d] %s",clnt_sock, recv_buf);

	clnt_info_t* info;
	for (auto iter = clnt_map.begin(); iter!= clnt_map.end(); iter++)
	{
		info = iter->second;

		if (info->bExit)
			continue;

		if (iter->first == clnt_sock)
			continue;

		printf("(send) to clnt(%d)\n", info->fd);

		int send_len = send(info->fd, send_buf, strlen(send_buf), 0);
		if (send_len<0)
			err_handle("send() err");
	}
}
void* recv_func(void* arg)
{
	clnt_info_t* info = (clnt_info_t*)arg;
	int clnt_sock = info->fd;

	info->pthread_id = pthread_self();

	char recv_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);

	while(!info->bExit)
	{
		memset(recv_buf, 0, BUF_SIZE);

		int recv_len = recv(clnt_sock, recv_buf, BUF_SIZE, 0);
		if (recv_len < 0)
		{
			info->bExit = true;
			break;
		}
		else if (strncmp(recv_buf, "END", 3)==0)
		{
			info->bExit = true;
			break;
		}

		printf("[clnt:%d](recv) <%s>\n", clnt_sock, recv_buf);

		send_to_clients(clnt_sock, recv_buf);

	}

	printf("========== [clnt:%d] recv func ==========\n", clnt_sock);

	pthread_exit(0);
	close(clnt_sock);

	return nullptr;
}

void* clnt_garbage_func(void* arg)
{
	while(!bExit)
	{
		if (clnt_map.empty())
		{
			sleep(10);
			continue;
		}

		// check clnt
		for (auto iter = clnt_map.begin(); iter != clnt_map.end(); iter++)
		{
			clnt_info_t* info = iter->second;

			if (info->bExit == true)
			{
				pthread_join(*info->recv_pth, NULL);

				int clnt_fd = info->fd;

				free(info->recv_pth);
				free(info);

				clnt_map.erase(clnt_fd);
			}
		}
	}

	printf("========== clnt_garbage_func ==========\n");
	pthread_exit(0);
	return nullptr;
}

void* accept_func(void* arg)
{
	int* serv_sock = (int*)arg;

	while(!bExit)
	{
		struct sockaddr_in clnt_addr;
		int clnt_addr_size = sizeof(clnt_addr);

		int clnt_sock = accept(*serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
		if (clnt_sock<0) err_handle("clnt accept() failed");

		print_accept_info(clnt_sock, &clnt_addr);

		pthread_t* recv_pth = (pthread_t*)malloc(sizeof(pthread_t));

		clnt_info_t* info = (clnt_info_t*)malloc(sizeof(clnt_info_t));
		memset(info, 0, sizeof(clnt_info_t));

		info->fd = clnt_sock;
		info->recv_pth = recv_pth;
		info->bExit = false;

		clnt_map.insert(std::pair<int, clnt_info_t*>(clnt_sock, info));

		int bRet = pthread_create(recv_pth, NULL, recv_func, info);
		if (bRet != 0)
		{
			char err_msg[BUF_SIZE];
			memset(err_msg, 0, BUF_SIZE);

			snprintf(err_msg, BUF_SIZE, "[clnt:%d] recv_thread() created faild\n");
			err_handle(err_msg);
		}

		pthread_detach(*recv_pth );

	}


	printf("========== accept_func ==========\n");
	pthread_exit(0);

	return nullptr;
}


int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv() sock create failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet < 0) err_handle("bind() error");

	bRet = listen(serv_sock, MAX_CLIENT); // ´ë±â Å¥
	if (bRet < 0) err_handle("listen() error");

	bExit = false;

	pthread_t accept_th;

	bRet = pthread_create(&accept_th, NULL, accept_func, &serv_sock);
	if (bRet!=0) err_handle("accept thread() create failed");

	pthread_join(accept_th, NULL);
	close(serv_sock);

	return 0;
}
