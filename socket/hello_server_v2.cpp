#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENT (5)
#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main()
{
	// 1. socket 생성
	int serv_sock, clnt_sock;
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. bind()
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";
	
	struct sockaddr_in serv_addr, clnt_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*) &serv_addr, (socklen_t) sizeof(serv_addr));
	if (bRet < 0) err_handle("bind() error");

	// 3. listen
	bRet = listen(serv_sock, MAX_CLIENT);
	if (bRet < 0) err_handle("listen() failed");

	// 4. accept
	int clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &serv_addr, (socklen_t*)&clnt_addr_size);
	if (clnt_sock<0) err_handle("accept() fail");
	printf("===> accept success (%d)\n", clnt_sock);

	// 5. data recv/send
	char recv_buf[BUF_SIZE];
	char send_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);

	while(true)
	{
		int recv_len = recv(clnt_sock, recv_buf, BUF_SIZE, 0);
		if (recv_len <0)
			break;
		else if (strncmp(recv_buf, "END",3)==0)
			break;

		printf("(recv) : %s", recv_buf);

		printf("(send) : ");
		if (fgets(send_buf, BUF_SIZE, stdin)==NULL)
			break;

		int send_len = send(clnt_sock, send_buf, strlen(send_buf), 0);
		if (send_len<0)
			break;

		memset(send_buf, 0, BUF_SIZE);
		memset(recv_buf, 0, BUF_SIZE);
	}

	printf("===== serv sock end =====\n");

	close(serv_sock);
	close(clnt_sock);
	return 0;
}
