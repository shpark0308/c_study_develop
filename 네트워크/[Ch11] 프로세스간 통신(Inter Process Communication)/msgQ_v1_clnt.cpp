#include "msgQ_v1.h"

int main()
{
	int msg_id = msgget(IPC_KEY, IPC_CREAT|0666);
	if (msg_id<0) err_handle("msgget() error");

	MsgType msg;
	memset(&msg, 0, sizeof(msg));

	int idx = 1;
	msg.type = idx;

	while(true)
	{
		fputs("Enter: ", stdout);
		scanf("%d %s", &msg.data.age, &msg.data.name);

		printf("(send): age(%d) name(%s)\n", msg.data.age, msg.data.name);
		int bRet = msgsnd(msg_id, (void*)&msg, sizeof(msg.data), 0);
		if (strncmp(msg.data.name, "exit", 4) == 0)
			break;
		if (msg.type == 1)
			msg.type = 2;
		else
			msg.type = 1;
	}
}
