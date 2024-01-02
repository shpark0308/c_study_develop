#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
	if (serv_sock<0) err_handle("sock create() failed");

	struct sockaddr_in serv_addr, frm_addr;
	int frm_addr_size = sizeof(frm_addr);

	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8089";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	while(true)
	{
		fputs("[clnt] : ", stdout);
		fgets(msg, BUF_SIZE, stdin);

		//int send_len = sendto(serv_sock, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		int send_len = send(serv_sock, msg, strlen(msg), 0);
		printf("send to server : %s\n", msg);

		//int recv_len = recvfrom(serv_sock, msg, BUF_SIZE, 0,(struct sockaddr*)&frm_addr, (socklen_t*)&frm_addr_size);
		int recv_len = recv(serv_sock, msg, BUF_SIZE, 0);
		msg[recv_len] = 0;

		printf("[recv] : %s\n", msg);
	}

	return 0;
}