#include "msgQ_v1.h"

int main()
{
	int msg_id = msgget(IPC_KEY, IPC_CREAT|0666);
	if (msg_id<0) err_handle("msgget() error");

	MsgType msg;
	memset(&msg, 0, sizeof(msg));

	msg.type = 2;

	while(true)
	{
		//printf("[Step 1]\n");
		int recv_size = msgrcv(msg_id, (void*) &msg, sizeof(MsgData), msg.type, IPC_NOWAIT);
		//size_t recv_size = msgrcv(msg_id, (void*) &msg, sizeof(MsgData), msg.type, 0);
		if (recv_size>0)
		{
			printf("(serv) recv: age(%d) name(%s)\n", msg.data.age, msg.data.name);
			if (strncmp(msg.data.name, "exit", 4) == 0)
			{
				printf("==== exit ====\n");
				break;
			}
		}

		if (0)
		{
			printf("[Step 2]\n");
			printf("(serv) recv: age(%d) name(%s)\n", msg.data.age, msg.data.name);
			if (strncmp(msg.data.name, "exit", 4) == 0)
			{
				printf("==== exit ====\n");
				break;
			}
		}

	}

	msgctl(msg_id, IPC_RMID, nullptr);
	return 0;
}
