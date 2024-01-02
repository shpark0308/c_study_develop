#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>
#include <vector>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<int> client_sockets;

#define BUF_SIZE (1024)
#define MAX_CLEINT	(5)

void* handle_client(void* arg)
{
	int clnt_sock = *(int*)arg;
	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	sleep(5);
	int recv_len = recv(clnt_sock, msg, BUF_SIZE, 0);
	printf("serv recv : %s\n");

	pthread_exit(nullptr);
	return nullptr;
}

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock, clnt_sock;
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket create() failed");

	struct sockaddr_in serv_addr, clnt_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("8088"));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() failed");

	bRet = listen(serv_sock, MAX_CLEINT);
	if (bRet<0) err_handle("listen() failed");

	fd_set read_fds;
	while(true)
	{
		FD_ZERO(&read_fds);
		FD_SET(serv_sock, &read_fds);

		/////////////////////////////////////////////
		///// FD_SET setting
		/////////////////////////////////////////////

		int max_fd = serv_sock;

		pthread_mutex_lock(&mutex); // client_sockets µ¿±âÈ­

		for (auto& client:client_sockets)
		{
			FD_SET(client, &read_fds);
			if (client>max_fd)
				max_fd = client;
		}

		pthread_mutex_unlock(&mutex);

		/////////////////////////////////////////////
		///// select()
		/////////////////////////////////////////////

		int act = select(max_fd+1, &read_fds, nullptr, nullptr, nullptr);
		if (act <0 ) err_handle("select() error");

		if (FD_ISSET(serv_sock, &read_fds)) // accept
		{
			int clnt_addr_size = sizeof(clnt_addr);
			clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);

			pthread_mutex_lock(&mutex);
			client_sockets.push_back(clnt_sock);

			pthread_mutex_unlock(&mutex);

			pthread_t thread;

			pthread_create(&thread, nullptr, handle_client, &clnt_sock);
			pthread_detach(thread);
		}


	}

	close(serv_sock);
	return 0;
}
