#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctime>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

bool isReady = false;

void* provider(void* arg)
{
	sleep(10);
	pthread_mutex_lock(&mutex);
	printf("provider start --> \n");
	isReady = true;
	pthread_cond_signal(&cond);
	printf("provier end --> \n");
	pthread_mutex_unlock(&mutex);
	return nullptr;
}

void* consumer(void* arg)
{
	pthread_mutex_lock(&mutex);
	printf("consumer start --> \n");
	struct timespec timeout;
	
	while(!isReady)
	{
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec+=2;

		printf("consumer is waiting....\n");
		pthread_cond_timedwait(&cond, &mutex, &timeout);
		//pthread_cond_wait(&cond, &mutex);
		printf("timeout occurs \n");
	}

	printf("consumer data processing!\n");
	pthread_mutex_unlock(&mutex);

	pthread_exit(nullptr);
	return nullptr;
}

int main()
{
	pthread_t thread1, thread2;//,thread3;

	pthread_create(&thread1, nullptr, provider, nullptr);
	//pthread_create(&thread3, nullptr, provider, nullptr);
	pthread_create(&thread2, nullptr, consumer, nullptr);

	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);
	//pthread_join(thread3, nullptr);
	return 0;
}
