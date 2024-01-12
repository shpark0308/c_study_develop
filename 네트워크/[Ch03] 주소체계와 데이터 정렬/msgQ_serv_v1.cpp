#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUF_SIZE (1024)
#define IPC_KEY  (1234)

typedef struct _msgData
{
	long type;
	char text[BUF_SIZE];

} msgData;

int main()
{
	int msg_id = msgget(IPC_KEY, IPC_CREAT | 0666);

	int recv_type = 1;
	int send_type = 2;

	msgData rcvMsg, sndMsg;
	memset(&rcvMsg, 0, sizeof(rcvMsg));
	memset(&sndMsg, 0, sizeof(sndMsg));

	rcvMsg.type = recv_type;
	sndMsg.type = send_type;

	while(true)
	{
		int recv_len = msgrcv(msg_id, (void*)&rcvMsg, sizeof(rcvMsg.text), recv_type, 0);
		printf("(recv): %s\n", rcvMsg.text);
		if (strncmp(rcvMsg.text, "exit", 4)==0)
		{
			break;
		}

		fputs("(send): ", stdout);
		fgets(sndMsg.text, BUF_SIZE, stdin);

		sndMsg.text[strlen(sndMsg.text)-1] = 0;
		int send_len = msgsnd(msg_id, (const void*)&sndMsg, strlen(sndMsg.text), 0);
	}

	msgctl(msg_id, IPC_RMID, 0);
	printf("====  Exit(0)  ====\n");
	return 0;
}
