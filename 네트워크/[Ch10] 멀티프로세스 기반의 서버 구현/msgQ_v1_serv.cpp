#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUF_SIZE (1024)
#define IPC_KEY  (1234)
#define MSG_KEY  (5678)

//const key_t IPC_KEY = 1234;

typedef struct _MsgType
{
	long mtype;
	char mtext[16];
} MsgType;

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int msg_id = msgget(IPC_KEY, IPC_CREAT | 0666);
	if (msg_id == -1) err_handle("msgget() error");

	printf("msg_id: %d\n", msg_id);
	MsgType msg;
	memset(&msg, 0, sizeof(msg));

	msg.mtype = 1;

	while(true)
	{
		size_t recv_size = msgrcv(msg_id, (void*)&msg, sizeof(msg.mtext), msg.mtype, -1);

		printf("(serv) recv: %s\n", msg.mtext);
		if (strncmp(msg.mtext, "exit", 4) == 0)
		{
			printf("==== exit ====\n");
			break;
		}
		break;
	}

	msgctl(msg_id, IPC_RMID, 0);
	return 0;
}
