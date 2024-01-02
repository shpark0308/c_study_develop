#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <map>

#define BUF_SIZE (1024)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serv_sock<0) err_handle("serv_sock create() failed");

	std::map<int, struct sockaddr_in> clnt_map;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	const char* ip = "127.0.0.1";
	const char* port = "8088";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	// bind 과정은 필요
	int bRet = bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	for (int i=0; i<3; i++)
	{
		sleep(10);
		struct sockaddr_in clnt_addr;
		int clnt_addr_size = sizeof(clnt_addr);

		// recv
		int recv_len = recvfrom(serv_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, (socklen_t*) &clnt_addr_size);
		msg[recv_len] = 0;

		int clnt_port = ntohs(clnt_addr.sin_port);
		auto it = clnt_map.find(clnt_port);
		if (it==clnt_map.end())
		{
			clnt_map.insert(std::make_pair(clnt_port, clnt_addr));
		}

		printf("(%d) recv : %s\n", clnt_port, msg);

		// send
		for (auto iter = clnt_map.begin(); iter != clnt_map.end(); iter++)
		{
			int send_len = sendto(serv_sock, msg, strlen(msg), 0, (struct sockaddr*)&iter->second, sizeof(iter->second));
		}
	}
	close(serv_sock);
	return 0;
}

