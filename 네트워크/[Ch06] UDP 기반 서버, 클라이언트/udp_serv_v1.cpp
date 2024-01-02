#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ( serv_sock == -1 ) err_handle("socker create() failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	// serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (bRet< 0) err_handle("bind error()");

	struct sockaddr_in clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	char msg[BUF_SIZE];

	while(1)
	{
		memset(msg, 0, BUF_SIZE);

		int recv_len = recvfrom(serv_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, (socklen_t*) &clnt_addr_size);
		printf("recv : %s\n", msg);

		std::string ipAddr = inet_ntoa(clnt_addr.sin_addr);
		int port = ntohs(clnt_addr.sin_port);
		printf(">>>>>>>>>> (%s) : (%d)\n", ipAddr.c_str(), port);

		int send_len = sendto(serv_sock, msg, strlen(msg), 0,(struct sockaddr*)&clnt_addr, clnt_addr_size );
	}

	close(serv_sock);

	return 0;
}