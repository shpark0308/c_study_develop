#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/times.h>
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
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (serv_sock<0) err_handle("socket() create failed");

	const char* serv_port = "8088";

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, (socklen_t)sizeof(serv_addr));
	if (bRet <0) err_handle("connect() failed");

	fd_set read_fds, old_fds;

	FD_ZERO(&read_fds);

	FD_SET(serv_sock, &read_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	int fds_max = serv_sock;
	old_fds = read_fds;

	char recv_buff[BUF_SIZE];
	char send_buff[BUF_SIZE];
	memset(recv_buff, 0, BUF_SIZE);
	memset(send_buff, 0, BUF_SIZE);

	// connection
	while(true)
	{
		read_fds = old_fds;

		int fd_num = select(fds_max+1, &read_fds, NULL, NULL, NULL);
		if (fd_num<0) err_handle("select failed");
		if (FD_ISSET(serv_sock, &read_fds))
		{
			printf("====> NOT comes here?\n");
			int recv_len = recv(serv_sock, recv_buff, BUF_SIZE, 0);
			if (recv_len<0) break;

			printf("(recv) %s", recv_buff);
		}
		if (FD_ISSET(STDIN_FILENO, &read_fds))
		{
			fgets(send_buff, BUF_SIZE, stdin);

			int send_len = send(serv_sock, send_buff, strlen(send_buff), 0);
			if (send_len<0) err_handle("send msg error");

			if (strncmp(send_buff, "END", 3)==0)
			{
				break;
			}
		}
	}

	close(serv_sock);
	return 0;
}
