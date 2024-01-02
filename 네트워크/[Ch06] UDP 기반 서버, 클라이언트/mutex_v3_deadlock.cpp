#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sharedData = 0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* threadFunc1(void* arg)
{
	pthread_mutex_lock(&mutex1);
	//sleep(1);
	pthread_mutex_lock(&mutex2);

	printf("threadFunc1 is running.....\n");

	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);

	pthread_exit(nullptr);
	return nullptr;
}

void* threadFunc2(void* arg)
{
        pthread_mutex_lock(&mutex2);
        //sleep(1);
        pthread_mutex_lock(&mutex1);

        printf("threadFunc2 is running.....\n");

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);

        pthread_exit(nullptr);
        return nullptr;
}

int main()
{
	pthread_t pthread1, pthread2;

	pthread_create(&pthread1, nullptr, threadFunc1, nullptr);
	pthread_create(&pthread2, nullptr, threadFunc2, nullptr);
	pthread_join(pthread1, nullptr);
	pthread_join(pthread2, nullptr);
	return 0;
}
