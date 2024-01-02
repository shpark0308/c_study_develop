#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// shared
bool dataReady = false;

void* producer(void* arg)
{
	sleep(5);
	pthread_mutex_lock(&mutex);
	printf("Producer start\n");
	//sleep(5);
	dataReady = true;
	printf("[Producer] dataReady..\n");

	pthread_cond_signal(&cond);

	printf("Producer end\n");
	pthread_mutex_unlock(&mutex);
	return nullptr;
}

void* consumer(void* arg)
{
	pthread_mutex_lock(&mutex);
	printf("Consumer start\n");
	while(!dataReady)
	{
		printf("[Consumer] is waiting for data....\n");
		pthread_cond_wait(&cond, &mutex);

	}
	printf("[Consumer] Data Processing\n");
	printf("Consumer end\n");
	pthread_mutex_unlock(&mutex);
	return nullptr;
}

int main()
{
	pthread_t producerThread, consumerThread;

	// producerThread
	pthread_create(&producerThread, nullptr, producer, nullptr);

	// consumerThread
	pthread_create(&consumerThread, nullptr, consumer, nullptr);

	pthread_join(producerThread, nullptr);
	pthread_join(consumerThread, nullptr);
	return 0;
}
