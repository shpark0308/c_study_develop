#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <poll.h>


#define MAX_CLIENT (1)
#define BUF_SIZE (1024)

struct pollfd poll_fds[MAX_CLIENT+1];

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_to_client(int idx, char recv_msg[])
{
	char send_msg[BUF_SIZE];
	snprintf(send_msg, BUF_SIZE,"(%d) %s", idx, recv_msg);

	for (int i=1; i<=MAX_CLIENT; i++)
	{
		if (poll_fds[i].fd> 0)
		{
			printf("send to %d\n", poll_fds[i].fd);
			int send_len = send(poll_fds[i].fd, send_msg, strlen(send_msg), 0);
			if (send_len<0) err_handle("send error()");
		}
	}
}

int main()
{
	// 1.  ì†Œì¼“ ìƒì„±
	int serv_sock, clnt_sock;
        serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("socket() create failed");

	// 2. ì†Œì¼“ bind
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

	poll_fds[0].fd = serv_sock;
	poll_fds[0].events = POLLIN;

	for (int i=1; i<MAX_CLIENT+1; i++)
	{
		poll_fds[i].fd = -1;
	}

	char recv_buf[BUF_SIZE];
	memset(recv_buf, 0, BUF_SIZE);

	while(true)
	{
		int fd_num = poll(poll_fds, MAX_CLIENT+1, -1);
		if (fd_num<0)
		{
			printf("poll() error\n");
			break;
		}
		else if (fd_num==0)
		{
			printf("poll() timeout\n");
			break;
		}
		else
		{
			if (poll_fds[0].revents == POLLIN)
			{
				int clnt_sock_size = sizeof(clnt_addr);

				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,(socklen_t*) &clnt_sock_size);
				if (clnt_sock<0) err_handle("accept() failed");

				for (int i=1; i<=MAX_CLIENT; i++) // poll ¿¡¤À Ãß°¡È÷
				{
					if (poll_fds[i].fd == -1)
					{
						poll_fds[i].fd = clnt_sock;
						poll_fds[i].events = POLLIN;
						break;
					}
				}
			}
			else // ±×°Ô ¾Æ´Ï¸é recv ÀÏå¶§
			{
				for (int i=1; i<=MAX_CLIENT; i++)
				{
					switch(poll_fds[i].revents)
					{
					case 0:
						break;
					case POLLIN:
						{
							memset(recv_buf, 0, BUF_SIZE);
							int recv_len = recv(poll_fds[i].fd, recv_buf, BUF_SIZE, 0);

							recv_buf[recv_len] = '\0';
							printf("(recv) %s\n", recv_buf);

							send_to_client(i, recv_buf);
						}
					default:
						close(poll_fds[i].fd);
						memset(&poll_fds[i], 0, sizeof(struct pollfd));

					}
				}
			}



		}
	}


	// 5.. recv / send()

	// 6. close()
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

