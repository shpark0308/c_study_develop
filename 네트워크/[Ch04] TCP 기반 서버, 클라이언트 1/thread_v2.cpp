#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <pthread.h>

void* join_func(void* argc)
{
	pthread_t* pth = (pthread_t*) argc;
	//sleep(25);
	for (int i=0; i<5; i++)
	{
		sleep(1);
		printf("join_func:(%d) waiting...\n", (i+1));
		int bRet = pthread_join(*pth, NULL);
		printf("[%d]sec : thread_join result:(%d)\n", (i+1),bRet);
		if (bRet == 3) // success
		{
			break;
		}
	}
	//pthread_cancel(*pth);

	printf("     < join_func END >     \n");
	pthread_exit(NULL);
	return nullptr;
}

void* thread_func(void* argc)
{
	printf("thread:(%lu) start \n", pthread_self());
	for (int i=0; i<10; i++)
	{
		sleep(2);
		printf("==== thread:(%d)sec ====\n", (i+1));
	}

	printf("     < thread_func END >     \n");
	pthread_exit(NULL);
	return nullptr;
}

int main()
{
	pthread_t th_pth;
	pthread_t join_pth;

	pthread_create(&th_pth, NULL, thread_func, NULL);
	pthread_create(&join_pth, NULL, join_func, (void*)&th_pth);

	pthread_join(th_pth, NULL);
	// pthread_join(th_pth, NULL);
	// pthread_join(th_pth, NULL);
	// pthread_join(th_pth, NULL);
	// pthread_join(join_pth, NULL);

	printf("===== Main Func End =====\n");
	//printf("===== Main Func End 1 =====\n");

	//pthread_join(th_pth, NULL);
	//printf("===== Main Func End 2 =====\n");
	return 0;
}
