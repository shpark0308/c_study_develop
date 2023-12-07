#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>


#define BUF_SIZE (1024)

bool isExit;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void* recv_func(void* arg)
{
	int* serv_sock_point = (int*) arg;
	int serv_sock = *serv_sock_point;

	char recv_buff[BUF_SIZE];
	memset(recv_buff, 0, BUF_SIZE);
	while(true)
	{
		if (isExit)
			break;

		int recv_len = recv(serv_sock, recv_buff, BUF_SIZE, 0);
		if (recv_len<0)
			break;
		else if (strncmp(recv_buff, "END", 3) == 0)
			break;
		printf("(recv) : %s\n", recv_buff);
		memset(recv_buff, 0, BUF_SIZE);
	}
	isExit = true;
	pthread_exit(NULL);
	return NULL;
}

void* send_func(void* arg)
{
	int* serv_sock_point = (int*) arg;
        int serv_sock = *serv_sock_point;
	printf("serv_sock >> (%d)\n", serv_sock);

        char send_buff[BUF_SIZE];
        memset(send_buff, 0, BUF_SIZE);
        while(true)
        {
		if (isExit)
			break;

		printf("(send) : ");
		if (fgets(send_buff, BUF_SIZE, stdin)==NULL)
		{
			break;
		}
                int send_len = send(serv_sock, send_buff, BUF_SIZE, 0);
                if (send_len<0)
                        break;
                else if (strncmp(send_buff, "END", 3) == 0)
                        break;
                memset(send_buff, 0, BUF_SIZE);
        }

	isExit = true;
        pthread_exit(NULL);
        return NULL;
}

int main()
{
	// 1. 소켓 생성
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("socket() created failed");

	// 2. 연결 요청
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect() failed");

	// 3. data recv / send
	pthread_t recv_pth, send_pth;
	bRet = pthread_create(&recv_pth, NULL, recv_func, (void*)&serv_sock);
	if (bRet!=0) err_handle("recv_thread create failed()");

	bRet = pthread_create(&send_pth, NULL, send_func, (void*)&serv_sock);
        if (bRet!=0) err_handle("send_thread create failed()");

	pthread_join(recv_pth, NULL);
	
	pthread_cancel(send_pth);
	pthread_join(send_pth, NULL);

	// 4. close()
	close(serv_sock);

	return 0;
}
