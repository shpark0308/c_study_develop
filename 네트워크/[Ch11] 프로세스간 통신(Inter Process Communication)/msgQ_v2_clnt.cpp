#include "msgQ_v1.h"
#include <pthread.h>
#include <poll.h>

int msg_id;
bool isExit = false;

void* recv_thread(void* arg)
{
	int msg_type = *(int*)arg;

	Msg msg;
	memset(&msg, 0, sizeof(msg));

	msg.type = msg_type;

	while(true)
	{
		int recv_len = msgrcv(msg_id, (void*)&msg, sizeof(msg.text), msg.type, 0);
		if (recv_len>0)
		{
			printf("(recv) : %s", msg.text);
			if (strncmp(msg.text, "exit", 4)==0)
			{
				printf("====  exit  ====\n");
				isExit = true;
				break;
			}
		}
	}
	pthread_exit(nullptr);
	return nullptr;
}

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		printf("Enter MsgType: ...\n");
		exit(1);
	}

	msg_id = msgget(IPC_KEY, IPC_CREAT | 0666);
	if (msg_id<0) err_handle("msgget() error");

	int msg_type = atoi(argv[1]);
	printf("==== (%d) msgType start ====\n", msg_type);

	pthread_t recvThread;
	pthread_create(&recvThread, nullptr, recv_thread, &msg_type);

	struct pollfd poll_fd[1];

	poll_fd[0].fd = STDIN_FILENO;
	poll_fd[0].events = POLLIN;

	Msg msg;
	memset(&msg, 0, sizeof(msg));

	msg.type = msg_type;

	while(true)
	{
		int fd_num = poll(poll_fd, 1, -1); // -1: 무한 대기
		if (fd_num <0) err_handle("poll() error");

		if (poll_fd[0].revents == POLLIN)
		{
			fgets(msg.text, sizeof(msg.text), stdin);

			if (strlen(msg.text) > 1)
			{
				printf("(%d) send: %s", msg_type, msg.text);

				int send_len = msgsnd(msg_id, (const void*)&msg, sizeof(msg.text), 0);
			}
		}

		if (isExit)
			break;


	}


	printf("==== (%d) client exit ====\n", msg_type);
	pthread_join(recvThread, nullptr);


	return 0;
}
