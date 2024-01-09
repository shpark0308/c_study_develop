#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <pthread.h>
#include <semaphore.h>

const key_t SHARED_MEMORY_KEY= 1234;
const int SHARED_MEMORY_SIZE = 1024;

sem_t semaphore;

typedef struct _SharedData
{
	char message[SHARED_MEMORY_SIZE];

} SharedData;

void* writeThread(void* arg)
{
	SharedData* sharedData = (SharedData*)arg;

	while(true)
	{
		fgets(sharedData->message, SHARED_MEMORY_SIZE, stdin);
		
		sem_post(&semaphore);

		if (strncmp(sharedData->message, "exit",4)==0)
		{
			break;
		}
	}
	printf("writeThread (stop)\n");
	pthread_exit(nullptr);
	return nullptr;
}

void* readThread(void*arg)
{
	SharedData* sharedData = (SharedData*)arg;
	
	printf("readThread (start)\n");
	while(true)
	{
		sem_wait(&semaphore);
		printf("(read) : %s\n", sharedData->message);

		if (strncmp(sharedData->message, "exit",4)==0)
		{
			printf("readThread break()\n");
			break;
		}
	}
	printf("readThread (end)\n");
	pthread_exit(nullptr);
	return nullptr;
}

int main()
{
	sem_init(&semaphore, 0, 0);

	int shmid = shmget(SHARED_MEMORY_KEY, sizeof(SharedData), IPC_CREAT | 0666);
	if (shmid == -1)
	{
		printf("shmget() error\n");
		exit(1);
	}

	SharedData* sharedData = (SharedData*) shmat(shmid, nullptr, 0);
	if (sharedData == nullptr)
	{
		printf("shmat() error\n");
		exit(1);
	}

	pthread_t thread_wr, thread_rd;

	pthread_create(&thread_wr, nullptr, writeThread, sharedData);
       	pthread_create(&thread_rd, nullptr, readThread, sharedData);

	pthread_join(thread_wr, nullptr);
	pthread_join(thread_rd, nullptr);

	int bRet = shmctl(shmid, IPC_RMID, nullptr);
	if (bRet == -1)
	{
		printf("shmctl() error\n");
		exit(1);
	}

	sem_destroy(&semaphore);

	return 0;
}
