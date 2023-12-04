#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* threadFunction1(void* arg)
{
	for(int i=0; i<5; i++)
	{
		printf("스레드 1에서 실행 중: %d\n", i);
		sleep(1);
	}
	return NULL;
}

void* threadFunction2(void* arg)
{
	for (int i=0; i<5; i++)
	{
		printf("스레드 2에서 실행 중: %d\n", i);
		sleep(2);
	}
	return NULL;
}

int main()
{
	pthread_t thread1, thread2;

	int value = 42;

	if (pthread_create(&thread1, NULL, threadFunction1, (void*)&value) !=0)
	{
	
		fputs("첫번째 스레드 생성 실패!\n", stderr);
		return 1;
	}
	if (pthread_create(&thread2, NULL, threadFunction2, (void*) &value) !=0)
	{
		fputs("두번째 스레드 생성 실패\n",stderr);
		return 1;
	}

	pthread_detach(thread1);
	pthread_detach(thread2);

	printf("모든 스레드 종료됨\n");

	return 0;
}
