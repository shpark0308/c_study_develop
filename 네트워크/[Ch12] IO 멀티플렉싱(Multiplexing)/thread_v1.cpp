#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* recv_func(void* arg)
{
	int second = 0;
	while(true)
	{
		//sleep(5);
		printf("[0] sec continue... \n");
		//second++;
	}
	pthread_exit(NULL);
	return NULL;
}

int main()
{
	pthread_t pth;
	int bRet = pthread_create(&pth, NULL, recv_func, NULL);
	if (bRet  !=0) return -1;

	printf("[1]. thread start\n");
	pthread_cancel(pth);
	printf("[2]. thread cancle\n");
	pthread_join(pth, NULL);
	printf("[3]. thread join\n");
	printf("====== The END ======\n");
	return 0;
}
