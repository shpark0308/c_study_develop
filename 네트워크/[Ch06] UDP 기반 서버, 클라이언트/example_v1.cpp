#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <semaphore.h>

static int sharedData = 0;
static int count = 0;

static bool bRet = false;

std::vector<pthread_t> workers;

sem_t sem;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* manager_thread(void* arg)
{
	while(true)
	{
		if (count==10)
		{
			pthread_mutex_lock(&mutex);
			printf("sem_post()\n");
			for (int i=0; i<10; i++)  // 10번 해주어야 함
				sem_post(&sem);
			count = 0;
			pthread_mutex_unlock(&mutex);
		}
		sleep(5);
		printf("manager_thread: [%d] (count:%d)\n", sharedData, count);

		if (sharedData==20)
		{
			bRet = true;
			break;
		}
	}

	printf(">>>>>>>>>>>> (%d) (%d) \n", count, sharedData);
	for (int i=0; i<count; i++)
		sem_post(&sem);
	printf("======== manager thread end ========\n");
	pthread_exit(nullptr);

	return nullptr;
}


void* worker_thread(void* arg)
{
	int idx = *(int*)arg;
	
	while(!bRet)
	{
		// blocking 상태
		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		
		sharedData++;
		count++;
		printf("thread:(%d) running (%d) count (%d)\n", idx, sharedData, count);
		pthread_mutex_unlock(&mutex);
		
	}

	pthread_exit(nullptr);
	return nullptr;
}

int main()
{
	sem_init(&sem, 0, 10);
	pthread_mutex_init(&mutex, nullptr);

	pthread_t th;
	pthread_create(&th, nullptr, manager_thread, nullptr);
	
	for (int i=0; i<10; i++)
	{
		int* temp = (int*) malloc(sizeof(int));
		*temp = (i+1);

		pthread_t thread;
		pthread_create(&thread, nullptr, worker_thread, temp);

		workers.push_back(thread);
	}
	

	pthread_join(th, nullptr);

	for (auto iter = workers.begin(); iter!= workers.end(); iter++)
	{
		pthread_join(*iter, nullptr);
	}


	pthread_mutex_destroy(&mutex);
	sem_destroy(&sem);

	printf("The End\n");
	return 0;
}
