#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENT (3)
#define BUF_SIZE   (1024)

int clnt_sock_arr[MAX_CLIENT];
bool bExit;
pthread_mutex_t mutex;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_func(char recv_buf[], int clnt_sock_idx)
{
	printf("clnt thread (%d) --> send start!\n", clnt_sock_idx);

	for (int i=0; i<MAX_CLIENT; i++)
	{
		if (i==clnt_sock_idx)
			continue;

		int clnt_sock = clnt_sock_arr[i];
		if (clnt_sock ==0)
			break;

		int send_len = send(clnt_sock, recv_buf, strlen(recv_buf), 0);
		if (send_len<0) err_handle("(serv) send_msg err");
	}
}

void* recv_func(void* arg)
{
	int* idx = (int*)arg;
	int clnt_sock = clnt_sock_arr[*idx];

	char recv_buf[BUF_SIZE];
	while(!bExit)
	{
		memset(recv_buf, 0, BUF_SIZE);
		
		int recv_len = recv(clnt_sock, recv_buf, BUF_SIZE, 0);
		if (recv_len <=0) 
		{
			bExit = true;
			break;
		}
		else if (strncmp(recv_buf, "END", 3)==0)
		{
			send_func(recv_buf, *idx);
			bExit = true;
			break;
		}


		printf("(serv recv) : %s\n", recv_buf);

		//pthread_mutex_lock(&mutex);
		send_func(recv_buf, *idx);
		//pthread_mutex_unlock(&mutex);
	}

	printf("[%d] thread exit \n", *idx);
	pthread_exit(NULL);
}

void print_accept_info(struct sockaddr_in* sock_addr)
{
	char ip[INET_ADDRSTRLEN];
	memset(ip, 0, INET_ADDRSTRLEN);

	inet_ntop(AF_INET, &(sock_addr->sin_addr), ip, INET_ADDRSTRLEN);
	
	int port = ntohs(sock_addr->sin_port);
	printf("==============================\n");
	printf("clnt : %s:%d (connected)\n", ip, port);
	printf("==============================\n");
}

void* accept_func(void* arg)
{
	int* serv_sock = (int*) arg;

	for (int i=0; i<MAX_CLIENT && !bExit ; i++)
	{
		int* idx = (int*) malloc(sizeof(int));
		memset(idx, 0, sizeof(int));

		*idx = i;
		struct sockaddr_in clnt_addr;
		int clnt_addr_size = sizeof(clnt_addr);

		clnt_sock_arr[i] = accept(*serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);

		if (clnt_sock_arr[i]<0) 
		{
			char err_msg[100];
			memcpy(err_msg, 0, sizeof(err_msg));

			snprintf(err_msg, sizeof(err_msg), "Idx[%d] clnt sock failed", (i+1));
			err_handle(err_msg);
		
		}

		print_accept_info(&clnt_addr);

		pthread_t pth;
		int bRet = pthread_create(&pth, NULL, recv_func, (void*)idx);
		if (bRet<0) err_handle("accpet > recv_func create failed");

		pthread_detach(pth);
	}

	return NULL;
}

void* accept_exit_func(void* arg)
{
	pthread_t* pth = (pthread_t*)arg;
	while(true)
	{
		if (bExit)
		{
			pthread_cancel(*pth);
			printf("Accpet thread 종료\n");
			break;
		}
	}
	pthread_exit(NULL);
	return NULL;
}

int main()
{
	bExit = false;
	pthread_mutex_init(&mutex, NULL);

	// 1. socket 생성
	int serv_sock;

	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. bind
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	struct sockaddr_in serv_addr, clnt_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*) &serv_addr, (socklen_t)sizeof(serv_addr));
	if (bRet <0) err_handle("bind() failed");

	// 3. listen
	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet <0) err_handle("listen() failed");

	// 4. accpet
	pthread_t accept_thread;

	bRet = pthread_create(&accept_thread, NULL, accept_func, (void*)&serv_sock);
	if (bRet!=0) err_handle("(accept) pthread_create failed");

	pthread_t accept_exit_thread;

	bRet = pthread_create(&accept_exit_thread, NULL, accept_exit_func, (void*)&accept_thread);
        if (bRet!=0) err_handle("(accept_exit) pthread_create failed");

	pthread_join(accept_thread, NULL);
	pthread_join(accept_exit_thread, NULL);

	// 6. close()
	printf("여기까지는 왔는가?\n");
	for (int i=0; i<MAX_CLIENT; i++)
	{
		close(clnt_sock_arr[i]);
	}

	close(serv_sock);

	printf("========== serv sock end ==========\n");

	pthread_mutex_destroy(&mutex);
	return 0;
}
