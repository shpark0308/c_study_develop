#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	if (argc!=2)
	{
		printf("Enter Mode : 0 (TCP) or 1(UDP) 2(BOTH)\n");
		exit(1);
	}

	int serv_sock;
	if (strcmp("0", argv[1])==0)
	{
		printf("[0]. TCP\n");
		serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (strcmp("1", argv[1]) == 0)
	{
		printf("[1]. UDP\n");
		serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else
	{
		printf("[2]. BOTH\n");
		serv_sock = socket(AF_INET, 0, 0);
	}

	struct sockaddr_in serv_addr, clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("8088"));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);
	while(true)
	{
		fputs("Enter msg : ", stdout);
		fgets(msg, BUF_SIZE, stdin);

		int send_len = send(serv_sock, msg, strlen(msg), 0);
		
#if 0
		int recv_len = recv(serv_sock, msg, BUF_SIZE, 0);
		if (recv_len<0)
			break;
		msg[recv_len] = 0;
		printf("(recv) : %s\n", msg);
#endif
	}
	close(serv_sock);
	return 0;
}
