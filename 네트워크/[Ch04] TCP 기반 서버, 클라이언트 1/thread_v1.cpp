#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <pthread.h>

void* thread_func(void* argc)
{
	printf("thread:(%lu) start \n", pthread_self());
	for (int i=0; i<10; i++)
	{
		sleep(1);
		printf("==== thread:(%d)sec ====\n", (i+1));
	}

	pthread_exit(NULL);
	return nullptr;
}

int main()
{
	pthread_t* pth = (pthread_t*)malloc(sizeof(pthread_t));

	int bRet = pthread_create(pth, NULL, thread_func, NULL);
	if (bRet !=0) exit(1);

	printf("[1]. Before Join : %p\n", pth);

	pthread_cancel(*pth);

	printf("[2]. Cancle : %p\n", pth);
	//free(pth);

	pthread_join(*pth, NULL);

	printf("[3]. After Join : %p\n", pth);
	return 0;
}
