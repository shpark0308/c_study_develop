#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CONNECT (5)
#define BUF_SIZE   (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main()
{
	int serv_sock, clnt_sock;
	// 1. 소켓 생성
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( serv_sock<0) err_handle("socket() create failed");

	// 2. 소켓 bind()
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";
	
	struct sockaddr_in serv_addr, clnt_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));
	if (bRet<0) err_handle("bind() error");

	// 3. listen()
	bRet = listen(serv_sock, MAX_CONNECT);
	if(bRet <0) err_handle("listen() error");

	// 4. accept()
	int clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);
	if (clnt_sock<0) err_handle("accept() error");

	char clnt_ip[INET_ADDRSTRLEN];
	memset(clnt_ip, 0, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(clnt_addr.sin_addr), clnt_ip, INET_ADDRSTRLEN);
	printf("%s:%d connected\n", clnt_ip, ntohs(clnt_addr.sin_port));

	// 5. data recv / send()
	char recv_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);
	int recv_size = recv(clnt_sock, recv_buf, BUF_SIZE, 0);
	if (recv_size<0) err_handle("Recv < 0");

	printf("(recv) : %s (%d)\n", recv_buf, recv_size);

	const char* send_buf = "[SERV] Hello I am Server";
	int send_size = send(clnt_sock, send_buf, strlen(send_buf), 0);
	if (send_size<0) err_handle("Send < 0");

	printf("===== serv sock end =====\n");

	// 6. close()
	close(serv_sock);
	close(clnt_sock);
	return 0;
}
