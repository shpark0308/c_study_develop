#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstdint>

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
	int recv_buf_size, send_buf_size;
	socklen_t recv_opt_size = sizeof(recv_buf_size);
	socklen_t send_opt_size = sizeof(send_buf_size);

	bRet = getsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &recv_opt_size);
	if (bRet <0) err_handle("recv_buf_size error!");

	bRet = getsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &send_opt_size);
	if (bRet <0) err_handle("recv_buf_size error!");

	printf("[serv_sock] recv_size : %d send_size : %d\n", recv_buf_size, send_buf_size);

	bRet = getsockopt(clnt_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &recv_opt_size);
        if (bRet <0) err_handle("recv_buf_size error!");

        bRet = getsockopt(clnt_sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &send_opt_size);
        if (bRet <0) err_handle("recv_buf_size error!");

	printf("[clnt_sock] recv_size : %d send_size : %d\n", recv_buf_size, send_buf_size);

	const char* file_name = "lyrics.txt";
	int fread = open(file_name, O_RDONLY);
	if (fread<0) err_handle("lyrics.txt file open failed");

	std::size_t max_size = SIZE_MAX; // 배열의 최대값 지정
	printf("1. send_buf_size * send_buf_size = (%d) size_t max : (%ld)\n", send_buf_size*send_buf_size, max_size);
	char read_buf[2000000];
	bRet = read(fread, read_buf, sizeof(read_buf));
	printf("2. read buffer : (%d)\n", bRet);
#if 0
	while(read(fread, read_buf, sizeof(read_buf)))
	{
		//int send_len = send(clnt_sock, read_buf, sizeof(read_buf),0);
		//if (send_len<0) err_handle("[serv] send_failed");
		break;
	}
#endif
	printf(" (%d)\n" ,sizeof(read_buf));
	int send_len = send(clnt_sock, read_buf, sizeof(read_buf),0);
	printf("send_len = (%d)\n", send_len);
	if (send_len<0) err_handle("[serv] send_failed");
	// 6. close()
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
