#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void* sharedResourceAccess(void* arg)
{
	int threadId = *(int*)arg;

	sem_wait(&sem);

	printf("Thread(%d) is accessing the shared resource\n", threadId);
	
	sem_post(&sem);

	return nullptr;
}

int main()
{
	sem_init(&sem, 0, 0); // 같은 프로세스 내에서 세마포어 초기값은 1로 셋팅

	pthread_t thread1, thread2, thread3;
	int id1 = 1, id2 = 2, id3 = 3;

	pthread_create(&thread1, nullptr, sharedResourceAccess, &id1);
	pthread_create(&thread2, nullptr, sharedResourceAccess, &id2);
	pthread_create(&thread3, nullptr, sharedResourceAccess, &id3);

	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);
	pthread_join(thread3, nullptr);

	sem_destroy(&sem); // 세마포어 제거

	return 0;
}
