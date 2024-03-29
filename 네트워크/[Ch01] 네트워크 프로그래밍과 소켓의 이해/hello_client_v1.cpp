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
	// 1. 소켓 생성
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("socket() create failed");

	// 2. 소켓 connection 요청
	const char* serv_port = "8088";
	
	struct sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&sock_addr, (socklen_t) sizeof(sock_addr));
	if (bRet<0) err_handle("connect() failed");

	// 3. 소켓 recv / send
	const char* send_buf = "[CLNT] Hello I am from Client";
	int send_size = send(serv_sock, send_buf, strlen(send_buf), 0);
	if (send_size<0) err_handle("send() failed");

	char recv_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);
	
	int recv_size = recv(serv_sock, recv_buf, BUF_SIZE, 0);
	if (recv_size<0) err_handle("recv() failed");

	printf("(recv) : %s (%d)\n", recv_buf, recv_size);

	printf("===== clnt sock end =====\n");

	// 4.. close()
	close(serv_sock);
	return 0;
}
