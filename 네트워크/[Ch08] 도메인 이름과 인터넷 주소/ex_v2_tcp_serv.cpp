#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include <pthread.h>

#define MAX_CLIENT  (5)
#define BUF_SIZE (1024)

int clnt_sock;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void* recv_func(void* arg)
{
	sleep(12);
	int idx = *(int*)arg;
	printf("[%d] recv_func start\n", idx);

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	while(true)
	{
		int recv_len = recv(clnt_sock, msg, BUF_SIZE, 0);
		if (recv_len<0) break;
		msg[recv_len] = 0;
		printf("(recv:%d) %s\n", idx, msg);
		break;
	}
	pthread_exit(nullptr);
	return nullptr;
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("tcp socket created() failed");

	struct sockaddr_in serv_addr, clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	printf("=======  TCP Connection =======\n");
	printf("Address : %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	printf("===============================\n");

	listen(serv_sock, MAX_CLIENT);

	fd_set read_fds, temp_fds;

	FD_ZERO(&read_fds);
	FD_SET(serv_sock, &read_fds);
	int fd_max = serv_sock;


	struct timeval timeout;

	pthread_t thread1, thread2, thread3;

	while(true)
	{
		temp_fds = read_fds;
		
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		int fd_num = select(fd_max + 1, &temp_fds, nullptr, nullptr, nullptr);
		if (fd_num == -1) err_handle("select() error");
		if (fd_num == 0)
		{
			// printf("select() timeout!\n");
			continue;
		}

		if (FD_ISSET(serv_sock, &temp_fds))
		{
			clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, (socklen_t*)&clnt_addr_size);

			printf("Address : %s:%d (connected)\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));			
			int idx1 = 1, idx2 = 2, idx3 = 3;
			pthread_create(&thread1, nullptr, recv_func, &idx1);
			//pthread_create(&thread2, nullptr, recv_func, &idx2);
			//pthread_create(&thread3, nullptr, recv_func, &idx3);

		}

	}
	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);
	pthread_join(thread3, nullptr);

	close(clnt_sock);
	close(serv_sock);
	return 0;
}
