#include <stdio.h>
#include <pthread.h>

int sharedData = 0;

void* incrementData(void* arg)
{
	for (int i=0; i<1000000; i++)
	{
		sharedData++;
	}

	pthread_exit(NULL);
	return nullptr;
}

int main()
{
	pthread_t thread1, thread2;

	pthread_create(&thread1, nullptr, incrementData, nullptr);
	pthread_create(&thread2, nullptr, incrementData, nullptr);

	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);

	printf("==> sharedData : %d\n", sharedData);

	return 0;
}
