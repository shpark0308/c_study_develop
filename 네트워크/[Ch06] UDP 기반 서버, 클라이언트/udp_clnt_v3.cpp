#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serv_sock<0) err_handle("serv_sock create() failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("8088"));

	char msg[BUF_SIZE];

	struct sockaddr_in sock_addr;
	int sock_addr_size = sizeof(sock_addr);

	for (int i=0; i<3; i++)
	{
		memset(msg, 0, BUF_SIZE);

		snprintf(msg, BUF_SIZE, "clnt %d send", (i+1));
		if (strcmp(msg, "q\n") == 0)
			break;

		int send_len = sendto(serv_sock, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		if (send_len<0) err_handle("send error()");
		printf("Send to server : %s\n", msg);

		//int serv_addr_size = sizeof(serv_addr);
		//int recv_len = recvfrom(serv_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&sock_addr, (socklen_t*) &sock_addr_size);
		//msg[recv_len] = 0;

		//printf("Message from server : %s\n", msg);
	}

	close(serv_sock);
	return 0;
}

