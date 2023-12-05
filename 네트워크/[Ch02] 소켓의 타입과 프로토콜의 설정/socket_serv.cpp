#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
	// 1.  소켓 생성
	int serv_sock, clnt_sock;
        serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. 소켓 bind
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";


	struct sockaddr_in serv_addr, clnt_addr;
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
	int clnt_sock_size = sizeof(clnt_addr);
	
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,(socklen_t*) &clnt_sock_size);
	if (clnt_sock<0) err_handle("accept() failed");

	// 5.. recv / send()
	
	// 6. close()
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
