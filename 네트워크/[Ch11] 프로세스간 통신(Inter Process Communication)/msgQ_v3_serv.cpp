#include "msgQ_v1.h"

#include <pthread.h>
#include <vector>

int msg_id;
int recv_arr[] = { 2, 4, 6 };
int send_arr[] = { 1, 3, 5 };

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool isExit = false;

void send_func(int idx, char* message);

void* recv_thread(void* arg)
{
	int idx = *(int*)arg;
	int recv_type = recv_arr[idx];

	Msg msg;

	msg.type = recv_type;

	printf("=====  recv_thread(%d) start  =====\n", recv_type);
	while(true)
	{
		int recv_size = msgrcv(msg_id, (void*)&msg, sizeof(msg.text), msg.type, 0);
		if (recv_size>0)
		{
			printf("recv(%d): %s\n", idx, msg.text);

			pthread_mutex_lock(&mutex);
			send_func(idx, msg.text);
			pthread_mutex_unlock(&mutex);
		}

		if (isExit)
			break;
	}

	pthread_exit(nullptr);
	return nullptr;
}

void send_func(int idx, char* message)
{
	if (strncmp(message, "exit", 4)==0)
	{
		isExit = true;
		return;
	}

	Msg msg;

	for (int i=0; i<MAX_CLIENT; i++)
	{
		if (i == idx)
			continue;

		msg.type = send_arr[i];

		memcpy(msg.text, message, strlen(message));
		msg.text[strlen(message)] = 0;

		int bRet = msgsnd(msg_id, (const void*)&msg, sizeof(msg.text), 0);
		printf("(send) to %d, %s\n", msg.type, msg.text);

	}
}

int main()
{
	std::vector<pthread_t> clnt_thread;

	pthread_mutex_init(&mutex, nullptr);

	msg_id = msgget(IPC_KEY, IPC_CREAT | 0666);
	if (msg_id<0) err_handle("msgget() error");

	for (int i=0; i<MAX_CLIENT; i++)
	{
		pthread_t th;
		int* idx = new int(i);
		pthread_create(&th, nullptr, recv_thread, idx);
		clnt_thread.push_back(th);
	}

	for (pthread_t th : clnt_thread)
	{
		pthread_join(th, nullptr);
	}

	msgctl(msg_id, IPC_RMID, 0);
	pthread_mutex_destroy(&mutex);
	return 0;
}
