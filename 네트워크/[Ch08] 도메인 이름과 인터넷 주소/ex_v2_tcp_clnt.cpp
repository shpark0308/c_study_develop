#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("serv_sock, created() failed");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8088);

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("connect error");

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	for (int i=1; ; i++)
	{
		sleep(1);
		snprintf(msg, BUF_SIZE, "send to serv (%d)", i);
		
		int send_len = send(serv_sock, msg, strlen(msg), 0);
		printf("%s\n", msg);
		if (i==11)
			break;
	}
	//sleep(600000000);
	printf("==== finished ====\n");
	close(serv_sock);
	return 0;
}
