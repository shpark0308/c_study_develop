#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/time.h>
#include <sys/select.h>
#include <pthread.h>

#define MAX_CLIENT (5)
#define BUF_SIZE (1024)

int send_func(int clnt_idx, char send_msg[]);

int clnt_sock[MAX_CLIENT];

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void* recv_func(void* arg)
{
	int* clnt_idx = (int*) arg;
	char recv_buff[BUF_SIZE];
	memset(recv_buff, 0, BUF_SIZE);

	while(true)
	{
		int recv_len = recv(clnt_sock[*clnt_idx], recv_buff, BUF_SIZE, 0);
		if (recv_len<0)
		{
			break;
		}
		
		int bRet = send_func(*clnt_idx, recv_buff);
		if (bRet <0)
		{
			break;
		}

		if (strncmp(recv_buff, "END", 3) == 0)
		{
			// 서버 종료
			printf("[AA] 종료\n");
			break;
		}

		memset(recv_buff, 0, BUF_SIZE);
	}
	pthread_exit(NULL);
	return NULL;
}

int send_func(int clnt_idx, char send_msg[])
{
	for (int i=0; i< MAX_CLIENT; i++)
	{
		if (i==clnt_idx)
			continue;
		if (clnt_sock[i] == 0)
			continue;

		int send_len = send(clnt_sock[i], send_msg, strlen(send_msg), 0);
		if (send_len<0)
			return -1;
	}
	return 0;
}

void print_accept_info(struct sockaddr_in* clnt_addr)
{
	char clnt_ip[INET_ADDRSTRLEN];
	memset(clnt_ip, 0, INET_ADDRSTRLEN);
	inet_ntop(clnt_addr->sin_family, &clnt_addr->sin_addr, clnt_ip, INET_ADDRSTRLEN);

	int clnt_port = ntohs(clnt_addr->sin_port);
	printf("==========================\n");
	printf("clnt sock : %s:%d (connected)\n", clnt_ip, clnt_port);
	printf("==========================\n");
}

void* accept_func(void* arg)
{
	int* serv_point = (int*)arg;
	int serv_sock = *serv_point;

	fd_set cpy_reads, reads;
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);

	int fd_max = serv_sock;
	int fd_num;

	struct timeval timeout;
	timeout.tv_sec = 180;
	timeout.tv_usec = 5000;

	int clnt_count = 0;
	while(true)
	{
		cpy_reads = reads;

		fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout);
		if (fd_num == -1)
		{
			break;
		}
		else if (fd_num == 0)
		{
			//printf("Timeout occurs!\n");
			continue;
		}
		else // 변화가 생김
		{
			for (int i=0; i<fd_max+1; i++)
			{
				if (i==serv_sock)
				{
					struct sockaddr_in clnt_addr;
					int clnt_addr_size = sizeof(clnt_addr);

					int* clnt_idx = (int*)malloc(sizeof(int));
					memset(clnt_idx, 0, sizeof(int));
					*clnt_idx = clnt_count;

					clnt_sock[clnt_count] = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
					if (clnt_sock[clnt_count] <0) err_handle("accept_failed");

					clnt_count++;

					print_accept_info(&clnt_addr);
					// recv / send 는 따로 thread 로 분리
					pthread_t recv_th;
					int bRet = pthread_create(&recv_th, NULL, recv_func, (void*)clnt_idx);
					if (bRet !=0) 
					{
						char err_th_msg[256];
						snprintf(err_th_msg, sizeof(err_th_msg), "[%d] recv thread create failed", *clnt_idx);
						err_handle(err_th_msg);
					}

					pthread_detach(recv_th);
				}
			}
		}

	}
	return NULL;
}
int main()
{
	// 1.  소켓 생성
	int serv_sock, clnt_sock;
        serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. 소켓 bind
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";


	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() failed");

	// 3. listen()
	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet <0) err_handle("listen() failed");

	// 4.. accept()
	pthread_t accept_th;

	bRet = pthread_create(&accept_th, NULL, accept_func, (void*) &serv_sock);
	if (bRet<0) err_handle("accept thread create failed");

	pthread_join(accept_th, NULL);

	// 5.. recv / send()
	printf("============ END ============\n");
	// 6. close()
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
