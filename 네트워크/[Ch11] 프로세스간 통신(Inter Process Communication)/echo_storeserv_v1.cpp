#include "echo_store.h"
#include <signal.h>

void read_childproc(int flag)
{
	printf("Flag : (%d)\n", flag);
	printf("read_childproc\n");

	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);

	printf("removed proc id: %d\n", pid);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock<0) err_handler("serv sock() create failed");

	initialize_sockaddr();

	struct sigaction sig_struct;

	sig_struct.sa_flags = 0;
	sig_struct.sa_handler = read_childproc;
	sigemptyset(&sig_struct.sa_mask);

	int state = sigaction(SIGCHLD, &sig_struct, 0);

	int opt = 1;
	int sRet = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt));
	int bRet = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(serv_sock, MAX_CLIENT);

	int fds[2];

	pipe(fds);
	pid_t pid = fork();

	if (pid == 0) // write
	{
		printf("=== child (%d) ===\n", pid);
		FILE * fp = fopen("echmsg.txt", "wt");

		char msgbuf[BUF_SIZE];

		for (int i=0; i<20; i++)
		{
			int recv_len = read(fds[0], msgbuf, BUF_SIZE);
			/*if (recv_len<0 || strncmp(msgbuf, "exit", 4)==0)
				break;*/

			fwrite((void*)msgbuf, 1, recv_len, fp);

		}
		printf("==== child up level close ====\n", pid);
		close(serv_sock);
		return 0;
	}

	// parent proc
	printf("==== Main Proc start ====\n");

	pollfd poll_fds;

	poll_fds.fd = serv_sock;
	poll_fds.events = POLLIN;

	while(true)
	{
		printf("AAA \n");
		int fd_num = poll(&poll_fds, 1, -1);
		printf("BBB\n");
		//if (fd_num == -1) err_handler("poll() error");

		if (poll_fds.revents == POLLIN)
		{
			int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size);

			printf("Client %s:%d  (connected) \n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

			pid = fork();
			if (pid == 0)
			{
				printf("=== child (%d) ===\n", pid);
				while(true)
				{
					char buf[BUF_SIZE];
					memset(buf, 0, BUF_SIZE);

					int recv_len = read(clnt_sock, buf, BUF_SIZE);
					write(fds[1], buf, strlen(buf));
					if (recv_len<0 || strncmp(buf, "exit", 4)==0)
						break;

					printf("(recv): %s\n", buf);

					int send_len = write(clnt_sock, buf, BUF_SIZE);
					printf("(send): %s\n", buf);
				}
				close(clnt_sock);
				close(serv_sock);
				printf("==== child (%d) close ====\n", pid);
				return 0;
			}
			else
			{
				close(clnt_sock);
			}
		}

	}
	sleep(3);
	printf("==== Main Proc ===\n");
	close(serv_sock);
	return 0;

}

