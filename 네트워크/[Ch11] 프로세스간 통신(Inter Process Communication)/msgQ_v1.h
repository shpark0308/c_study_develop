#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define IPC_KEY (1024)
#define MAX_CLIENT (2)

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

typedef struct _MsgData
{
	int age;
	char name[16];

} MsgData;

typedef struct _MsgType
{
	long type;
	MsgData data;

} MsgType;

typedef struct _Msg
{
	long type;
	char text[1024];
} Msg;