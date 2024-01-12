#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <poll.h>

#include <signal.h>
#include <wait.h>

#define BUF_SIZE (1024)
#define MAX_CLIENT (5)

struct sockaddr_in serv_addr, clnt_addr;
int clnt_addr_size = sizeof(clnt_addr);

void initialize_sockaddr()
{
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8088);
}

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
