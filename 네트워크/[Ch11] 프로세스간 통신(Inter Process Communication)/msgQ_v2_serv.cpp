#include "msgQ_v1.h"

int main()
{
	int msg_id = msgget(IPC_KEY, IPC_CREAT | 0666 );
	if (msg_id < 0) err_handle("msgget() error");

	Msg msg;
	memset(&msg, 0, sizeof(msg));

	msg.type = 1;

	while(true)
	{
		int recv_size = msgrcv(msg_id, (void*)&msg, sizeof(msg.text), msg.type, 0); // ºí·ÎÅ·
		if (strncmp(msg.text, "exit", 4) == 0)
		{
			printf("====  exit ====\n");
			break;
		}
		printf("(serv) recv: %s", msg.text);

		for (int i=1; i<=MAX_CLIENT; i++)
		{
			if (i == msg.type)
				continue;

			msg.type = i;
			int send_len = msgsnd(msg_id, (void*)&msg, sizeof(msg.text), 0);
			printf("(serv) send: %s", msg.text);
		}

	}

	msgctl(msg_id, IPC_RMID, nullptr);
	return 0;
}
