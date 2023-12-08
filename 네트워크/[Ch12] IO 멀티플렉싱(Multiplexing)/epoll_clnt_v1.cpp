#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <sys/types.h>

#define BUF_SIZE (1024)


void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	// (1). 소켓 생성
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("socket create failed");

	// (2). connecton
	const char* serv_port = "8088";

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect failed");

	// (3). data recv / send
	fd_set read_fds, old_fds;
	char msg[BUF_SIZE];

	FD_ZERO(&read_fds);

	FD_SET(serv_sock, &read_fds); // serv_sock  에 해당하는 read_fds 의 비트를 1로 해준다.

	FD_SET(0, &read_fds);

	int max_fd = serv_sock;
	old_fds = read_fds;

	while(1)
	{
		read_fds = old_fds; // 이 작업을 왜 해주어야 하는지
		// 무슨생각으로 여기다가 select 를 사용해주어야 하는지
		// int select(int nfds, fd_set* read_fds, fd_set* write_fds, fd_set* except_fds, struct timeval* timeout);
		int fd_num = select(max_fd+1, &read_fds, NULL, NULL, NULL);
		if (FD_ISSET(serv_sock, &read_fds))
		{
			memset(msg, 0, BUF_SIZE);
			int recv_len = recv(serv_sock, msg, BUF_SIZE, 0);
			if (recv_len<0) err_handle("recv <0");

			fgets(msg, BUF_SIZE, stdin);
			printf(">%s\n", msg);


			int send_len = send(serv_sock, msg, BUF_SIZE, 0);

		}
		if (FD_ISSET(0, &read_fds))
		{
			memset(msg, 0, BUF_SIZE);
			fgets(msg, BUF_SIZE, stdin);
			printf(">%s\n", msg);

			int send_len = send(serv_sock, msg, BUF_SIZE, 0);
		}

	}

	close(serv_sock);




	return 0;
}
