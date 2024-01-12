#include "pipe_v1.h"

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serv_sock<0) err_handle("serv_sock() create failed");

	initialize_sockaddr();

	connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	while(true)
	{
		memset(msg, 0, BUF_SIZE);

		printf("Enter msg: ");
		fgets(msg, BUF_SIZE, stdin);

		msg[strlen(msg)-1] = 0;
		write(serv_sock, msg, strlen(msg));
		if (strncmp(msg, "exit", 4)==0)
			break;

		int r_size = read(serv_sock, msg, BUF_SIZE);
		printf("(recv): %s\n", msg);
	}
	return 0;
}
