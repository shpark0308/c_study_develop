#include "echo_store.h"

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handler("clnt sock() create failed");

	initialize_sockaddr();

	int bRet = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	char send_msg[BUF_SIZE], recv_msg[BUF_SIZE];

	while(true)
	{
		memset(send_msg, 0, BUF_SIZE);
		memset(recv_msg, 0, BUF_SIZE);

		fputs("Enter msg: ", stdout);
		fgets(send_msg, BUF_SIZE, stdin);

		send_msg[strlen(send_msg)-1] = 0;

		int send_len = write(serv_sock, send_msg, strlen(send_msg));
		if (strncmp(send_msg, "exit", 4)==0)
			break;

		int recv_len = read(serv_sock, recv_msg,  BUF_SIZE);
		printf("(recv): %s\n", recv_msg);

	}

	close(serv_sock);
	return 0;
}
