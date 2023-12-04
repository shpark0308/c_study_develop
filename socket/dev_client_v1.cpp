#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE (1024)

pthread_mutex_t mutex;
bool bExit;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void* recv_func(void* arg)
{
	int* serv_sock = (int*)arg;
	char recv_buf[BUF_SIZE];

	while(!bExit)
	{
		memset(recv_buf, 0, BUF_SIZE);
		int recv_len = recv(*serv_sock, recv_buf, BUF_SIZE, 0);
		if (recv_len<0)
			break;
		else if (strncmp(recv_buf, "END", 3)==0)
			break;
		//pthread_mutex_lock(&mutex);
		printf("(recv) : %s\n", recv_buf);
		//pthread_mutex_unlock(&mutex);
	}
	bExit = true;
	pthread_exit(NULL);

	return NULL;
}

void* send_func(void* arg)
{
	int* serv_sock = (int*)arg;
	char send_buf[BUF_SIZE];

	while(!bExit)
	{
		pthread_mutex_lock(&mutex);
		printf("(send) : ");
		if (fgets(send_buf, BUF_SIZE, stdin) ==NULL)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);

		int send_len = send(*serv_sock, send_buf, strlen(send_buf), 0);
		if (send_len<0)
			break;
		else if (strncmp(send_buf, "END", 3)==0)
			break;
	}
	bExit = true;
	pthread_exit(NULL);
	return NULL;
}

int main()
{
	bExit = false;
	// 1. socket() create
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. socket connect
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*) &serv_addr, (socklen_t) sizeof(serv_addr));
	if (bRet <0) err_handle("connect() failed\n");

	// 3. data recv / send
	pthread_mutex_init(&mutex, NULL);
	
	pthread_t recv_thread, send_thread;
	bRet = pthread_create(&recv_thread, NULL, recv_func, (void*)&serv_sock);
	if (bRet <0) err_handle("recv pthread create() failed");

	bRet = pthread_create(&send_thread, NULL, send_func, (void*) &serv_sock);
	if (bRet<0) err_handle("send pthread create() failed");

	pthread_join(recv_thread, NULL);
	pthread_join(send_thread, NULL);
	
	// 4. close
	printf("========== clnt socket closed ==========\n");
	close(serv_sock);

	pthread_mutex_destroy(&mutex);
	return 0;
}
