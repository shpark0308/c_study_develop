ⅠⅡⅢ1️⃣2️⃣3️⃣4️⃣✅
### Ⅰ. 동기화
#### 1️⃣ 병렬 처리
✅ 동기화
- [다중 스레드 환경] ( 병렬 처리 ) ⇒ 데이터 / 자원을 안전하게 접근하기 위함
- **동시에** 공유 데이터에 접근 시, 예기치 못한 문제가 발생
  - 경쟁 조건 ( Race Condition )
  - 데드락 
<br/>

#### 2️⃣ 공유 자원의 문제점
- (( 병렬 처리 )) 다중 스레드 상황에서 자원을 공유할 시, 발생하는 문제점

✅ 경쟁 조건 ( Race Condition )
- 여러 스레드가 **동시에** 공유 데이터를 (( **변경** )) 하고자 함
----
```cpp
int sharedData = 0;

pthread_create(&pthread_1, nullptr, incrementData, nullptr); // pthread_1
pthread_create(&pthread_2, nullptr, incrementData, nullptr); // pthread_2
```
- sharedData 에 동시에 접근하여, 잘못된 데이터로 **변경** 될 수 있다.

✅ 데드락 ( DeadLock )
- 서로 상대방의 자원을 기다리면서 무한히 대기하는 현상
-----
```cpp
# thread1
void* threadFunc1(void* arg)
{
  pthread_mutex_lock(&mutex1);
  pthread_mutex_lock(&mutex2);

  printf("threadFunc1 is running....");

  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);
}
```
```cpp
# thread2
void* threadFunc2(void* arg)
{
  pthread_mutex_lock(&mutex2);
  pthread_mutex_lock(&mutex1);

  printf("threadFunc2 is running....");

  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex2);
}
```
- thread1 은 mutex1 를 thread2 로부터 기다림
- thread2 는 mutex2 를 thread1 로부터 기다림
-----
- 발생 가능한 시나리오
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/cd9817c4-0415-4fd0-8cba-6c72ffa7e804)
<br/>

#### 3️⃣ 상호 배제
✅ 상호 배제 ( Mutual Exclusion )
- (( 여러 스레드 )) 또는 (( 여러 프로세스 )) 가 [ 공유된 자원 ] 에 동시에 접근하지 못하도록 하는 원리
- [ 하나의 스레드 ] 에서 공유 자원을 사용하고 있으면, 다른 스레드는 (( 대기 )) 하도록 제어
- 동기화 방법
  - ① 뮤텍스
  - ② 세마포어
  - ③ 조건 변수
<br/>
 
#### 4️⃣ 동기화 3가지
✅ 뮤텍스 ( Mutex )
- [ 공유 자원 (Critical Section) ]에 대한 (( **접근** ))을 제어 ( 상호 배제 )
- 공유 자원에 대한 접근을 **한 번에 하나의 스레드만 허용**

🔯 이진 세마포어
- 상태가 Lock / UnLock 여부만 확인
<br/>

✅ 세마포어 ( Semaphore )
- 뮤텍스의 확장으로, (1). 상호 배제 뿐만 아니라 (2). **스레드 간의 특정 개수만큼 리소스를 공유**
- 여러 스레드가 동시에 접근을 허용할 수 있음
- 아직도 의미가 추상적임, 실제 사용하는 것을 봐야지 알 수 있을것같음

🔯 Counting 세마포어
- 특정 리소스 개수를 가지며 ( 여러 스레드간의 공유 ), 동시에 접근 가능할 수 있는 허용 가능한 리소스 개수를 나타냄

(1). sem_init
``` cpp
int sem_init(sem_t *sem, int pshared, unsigned int value);
# sem : 초기화 할 세마포어의 포인터
# pshared : 세마포어를 공유할 지 여부 ( 0 : 현재 프로세스 내에서만 공유 )
# value : 세마포어의 초기 값
```
- 세마포어 관련 함수의 리턴값 : 성공 (0), 오류 (-1)

(2). sem_wait
``` cpp
int sem_wait(sem_t *sem);
```
- 세마포어 값을 **감소** 시키고, 값이 0보다 작아지면, **대기**한다.
- 다른 스레드 / 프로세스에서 세마포어 값을 증가시킬 때까지, 대기 상태 유지

(3). sem_post
``` cpp
int sem_post(sem_t *sem);
```
- 세마포어 값을 증가시킨다.
- 대기 중인 다른 스레드나 프로세스가 있으면, 하나를 꺠우게 된다.

(4). sem_destroy
```cpp
int sem_destroy(sem_t *sem)
```
- 세마포어 제거
- 해당 세마포어에 대한 자원이 해제

----
```cpp
void* threadFunc(void* arg)
{
  sem_wait(&sem)
  // critical section
  sem_post(&sem)
}

sem_init(&sem, 0, 1)
```
- sem_post(&sem) 을 하지 않으면, 다른 스레드에서 저 critical section 을 수행할 수 없음
- sem_init(&sem, 0, **0**)
  - 만약에 초기값을 0 으로 설정하면, sem_post 하여 1이 증가되기 전까지 다른 스레드들은 저 critical section 을 할 수 없음
<br/>

✅ 조건 변수 ( Condition Variable )
- 스레드 간의 특정 조건에 대한 (( 통신 ))을 도와줌
- 스레드 간의 특정 조건이 충족되기를 기다리는 (( **동기화** )) 기술
<br/>

#### 2️⃣ 병렬 처리 문제점
### Ⅱ. 동기화 방법
#### 1️⃣ Mutex

#### 2️⃣ Semaphore
- 이 둘은 어떻게 다른 것인지
- 










1000000
