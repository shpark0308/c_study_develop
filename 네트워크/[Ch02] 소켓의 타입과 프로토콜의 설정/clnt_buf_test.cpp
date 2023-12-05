#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	if(serv_sock<0) err_handle("socket() created failed");

	// 2. 연결 요청
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8088";
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect() failed");

	// 3. data recv / send
	int recv_buf_size, send_buf_size;
        socklen_t recv_opt_size = sizeof(recv_buf_size);
        socklen_t send_opt_size = sizeof(send_buf_size);

        bRet = getsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &recv_opt_size);
        if (bRet <0) err_handle("recv_buf_size error!");

        bRet = getsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &send_opt_size);
        if (bRet <0) err_handle("recv_buf_size error!");

        printf("[serv_sock] recv_size : %d send_size : %d\n", recv_buf_size, send_buf_size);
	
	char recv_buf[0];
	//int recv_size = recv(serv_sock, recv_buf, sizeof(recv_buf), 0);
	//printf("recv_buf_size : %d\n", recv_size);
#if 1
	int idx = 0;
	while(int recv_size = recv(serv_sock, recv_buf, sizeof(recv_buf), 0))
	{
		//printf("[recv] : %s\n", recv_buf);
		printf("[%d] recv_buf_size : %d\n", idx, recv_size);
		idx++;
	}
#endif
	printf("====== 마무리 ======\n");
	// 4. close()
	close(serv_sock);

	return 0;
}
