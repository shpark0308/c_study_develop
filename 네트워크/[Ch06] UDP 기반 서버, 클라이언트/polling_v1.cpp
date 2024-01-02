#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

bool isReady = false;

void* threadFunc1(void* arg)
{
	while(!isReady)
	{
		sleep(1);
		printf("threadFunc1 is waiting....\n");
	}
	printf("****** thread1 data processing ******\n");
	return nullptr;
}

void* threadFunc2(void* arg)
{
	printf("threadFunc2 --> start\n");
	sleep(10);
	isReady = true;
	return nullptr;
}

int main()
{
	pthread_t thread1, thread2;

	pthread_create(&thread1, nullptr, threadFunc1, nullptr);
	pthread_create(&thread2, nullptr, threadFunc2, nullptr);

	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);

}
