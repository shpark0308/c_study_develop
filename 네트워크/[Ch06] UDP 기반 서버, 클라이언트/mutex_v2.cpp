#include <stdio.h>
#include <pthread.h>

int sharedData = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* incrementData(void* arg)
{
	for (int i=0; i<1000000; i++)
	{
		pthread_mutex_lock(&mutex);
		sharedData++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(nullptr);
}

int main()
{
	pthread_mutex_init(&mutex, nullptr);

	pthread_t pthread1, pthread2;

	pthread_create(&pthread1, nullptr, incrementData, nullptr);
	pthread_create(&pthread2, nullptr, incrementData, nullptr);

	pthread_join(pthread1, nullptr);
	pthread_join(pthread2, nullptr);

	printf("==> sharedData: (%d)\n", sharedData);
	pthread_mutex_destroy(&mutex);
	return 0;
}
