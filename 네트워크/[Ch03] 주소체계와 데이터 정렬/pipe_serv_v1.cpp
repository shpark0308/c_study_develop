#include "pipe_v1.h"

void chld_exit(int flag)
{
	int status;

	pid_t pid = waitpid(-1, &status, WNOHANG );
	printf("==== remove (%d) ====\n", pid);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handle("serv_sock() create failed");

	initialize_sockaddr();

	int opt = 1;

	int sRet = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void*) &opt, sizeof(opt));
	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (bRet<0) err_handle("bind() error");

	listen(serv_sock, MAX_CLIENT);

	int fds[2];
	pipe(fds);

	pid_t pid = fork();

	struct sigaction sig_struct;
	sig_struct.sa_handler = chld_exit;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	sigaction(SIGCHLD, &sig_struct, 0);

	char msg[BUF_SIZE];
	memset(msg, 0, BUF_SIZE);

	if (pid == 0)
	{
		FILE* fp = fopen("echo_msg.txt", "wt");

		int count = 0;
		while(count<3)
		{
			int r_size = read(fds[0], msg, BUF_SIZE);
			if (strncmp(msg, "exit", 4)==0)
			{
				count++;

				char last_indx = '\n';
				fwrite((void*)&last_indx, 1, sizeof(last_indx), fp);

				continue;
			}

			fwrite((void*) msg, 1,  strlen(msg), fp);
		}

		fclose(fp);
		close(serv_sock);

		printf("===== frwite finished() =====\n");

		return 0;

	}

	pollfd poll_fds;
	poll_fds.fd = serv_sock;
	poll_fds.events = POLLIN;

	while(true)
	{
		int fd_num = poll(&poll_fds, 1, -1);
		if (fd_num<0) continue;

		if (poll_fds.revents == POLLIN)
		{
			int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);

			printf("Client %s:%d     (connected)\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

			pid = fork();

			if (pid == 0)
			{
				while(true)
				{
					int r_len = read(clnt_sock, msg, BUF_SIZE);
					printf("(recv): %s\n", msg);

					write(fds[1], msg, strlen(msg));
					if (r_len<0 || strncmp(msg, "exit", 4) == 0)
						break;
					write(clnt_sock, msg, strlen(msg));

					printf("(send): %s\n", msg);
				}

				printf("===== clnt disconnected .... =====\n");

				close(clnt_sock);
				close(serv_sock);
				return 0;
			}
			else
			{
				close(clnt_sock);
			}
		}
	}

	close(serv_sock);
	return 0;
}


