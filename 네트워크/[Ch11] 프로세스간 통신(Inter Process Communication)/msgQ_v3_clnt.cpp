#include "msgQ_v1.h"

#include <pthread.h>
#include <semaphore.h>

Msg msg;
sem_t semaphore;

int msg_id;

void* recv_thread(void* arg)
{
	int recv_type = *(int*)arg;

	while(true)
	{
		int recv_size = msgrcv(msg_id, &msg, sizeof(msg.text), recv_type, IPC_NOWAIT);
		printf(">>>>>>>>>>>> (%d)   (%d) \n", msg.type, recv_size);
		if (recv_size>0)
		{
			printf("(recv): %s from %d\n", msg.text, msg.type);
			if (strncmp(msg.text, "exit", 4)==0)
				break;

		}
		sem_post(&semaphore);
	}

	printf("====== clnt(%d) recv thread exit  =====\n");

	pthread_exit(nullptr);
	return nullptr;
}

void* send_thread(void*arg)
{
	int send_type = *(int*)arg;

	while(true)
	{
		sem_wait(&semaphore);

		msg.type = send_type;

		fgets(msg.text, sizeof(msg.text), stdin);
		int bRet = msgsnd(msg_id, &msg, sizeof(msg.text), 0);
		if (bRet<0) err_handle("msgsnd() error %s\n");

		printf("(send): %s from %d\n", msg.text, msg.type);
	}
}

int main(int argc, char* argv[])
{
	if (argc !=3 )
	{
		printf("Enter MsgType: Recv(%d) Send(%d)\n");
		exit(1);
	}

	sem_init(&semaphore, 0, 1);

	memset(&msg, 0, sizeof(Msg));

	int recv_type = atoi(argv[1]);
	int send_type = atoi(argv[2]);

	printf("=====  clnt Recv(%d) Send(%d) start  =====\n", recv_type, send_type);

	msg_id = msgget(IPC_KEY, IPC_CREAT|0666);
	if (msg_id<0) err_handle("msgget() failed\n");

	pthread_t recvThread, sendThread;
	pthread_create(&recvThread, nullptr, recv_thread, &recv_type);
	pthread_create(&sendThread, nullptr, send_thread, &send_type);



	pthread_join(recvThread, nullptr);

	pthread_cancel(sendThread);
	pthread_join(sendThread, nullptr);

	sem_destroy(&semaphore);

	return 0;
}
