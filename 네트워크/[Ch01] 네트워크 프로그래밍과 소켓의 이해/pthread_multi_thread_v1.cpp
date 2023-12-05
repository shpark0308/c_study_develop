#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#define NUM_THREADS (3)

int sharedResource = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadFunction(void* threadID)
{
	long tid = (long) threadID;

	for (int i=0; i<5; i++)
	{
		//pthread_mutex_lock(&mutex);

		sharedResource++;
		printf("스레드 %ld: 공유 자원 값 :%d\n", tid, sharedResource);

		//pthread_mutex_unlock(&mutex);

	}

	pthread_exit(NULL); // 현재 스레드를 종료
}

int main()
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	pthread_mutex_init(&mutex, NULL);

	for (int i=0; i<NUM_THREADS; i++)
	{
		rc = pthread_create(&threads[i], NULL, threadFunction, (void*)i);
		if (rc)
		{
			printf("반환 코드 : %d\n", rc);
			exit(1);
		}
	}

	for (int i=0; i<NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);

	printf("메인 스레드 종료\n");
	return 0;
}
