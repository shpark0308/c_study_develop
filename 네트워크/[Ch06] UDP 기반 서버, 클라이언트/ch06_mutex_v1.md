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

🔯 Busy Waiting & 상호 배제

(1). Busy Waiting ( 바쁜 대기 )
``` cpp
bool isRecv = false;

# thread 대기
void* threadFunc()
{
  while(!isRecv)
  {
    // 계속 대기
    continue;
  }
  // Recv 에 대한 처리
}

# main() 함수
int main()
{
  int recv_size = recv();
  if (recv_size>0)
    isRecv = true;
}
```
- OS 에서 원하는 자원을 얻기 위해 기다리는 것이 아니라, **권한을 얻을 때까지 확인** 하는 것을 의미
- CPU의 자원을 쓸데 없이 낭비하기 때문에, ( 계속 대기 상태 ) **좋지 않은 스레드 동기화 방식**.
⇒ 스레드 동기화를 위해서, **Busy Waiting Method** 가 아니라, Mutex, Semaphore 등 상호 배제 방식을 사용해야한다.
  - Busy Waiting
    - 계속 무한루프를 (대기) 돌면서, 계속 확인하는 방식
    - CPU 의 자원을 낭비
  - 상호 배제 ( Mutex Exclusion )
    - Monitoring
    - 어떤 스레드가 공유 자원을 획득 한 후, 다른 스레드는 그 스레드의 공유 자원을 모두 사용할 때까지 기다림
    - 또한 해당 스레드가, 공유 자원을 모두 사용하면, 그때, 기다리고 있던 다른 스레드를 꺠움
----

(2). BusyWaiting
- CPU 를 계속 소모
(3). DeadLock
- 블로킹 상태로, 시스템이 계속 진행되지 않을 수 있다 ( CPU 사용을 안하고 있음 )
- 무한 대기 상태

🔯 인터럽트 방식의 상호배제
- 대부분 [운영체제] 는 (( 시그널 / 이벤트 )) 와 같은 매커니즘 제공
- [특정 이벤트] 발생 시, 대기 중인 스레드에게 알람
- 스레드는 해당 이벤트에 대한 핸들러를 등록하고 핸들러가 호출되어 작업을 수행
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

(1). pthread_cond_signal
``` cpp
int pthread_cond_signal(pthread_cond_t *cond)
```
- cond 변수에 신호를 보내여, cond 변수를 기다리고 있는 ( 블로킹 ) 되어 있는 스레드를 다시 시작시키는 함수
- pthread_cond_signal 함수는 cond 변수를 기다리는 여러 스레드 중에서 하나만 다시 시작시킴
- 하지만 어느 스레드를 시작시킬지는 지정할 수 없음
- 만약, cond 변수를 기다리는 스레드가 없다면, 아무 일도 일어나지 않는다.

(2). pthread_cond_broadcast
```cpp
int pthread_cond_broadcast(pthread_cond_t *cond)
```
- 하나의 스레드만 다시 시작시키는 pthread_cond_signal 함수와 다르게, cond 변수를 기다리는 모든 스레드를 다시 시작
- 하지만, pthread_cond 는 **특정 스레드를 지정하여, 다시 시작할 수는 없다**

(3). pthread_cond_wait
```
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
```
- cond 변수가 신호를 받을 때까지, 기다리는 함수 ( 블로킹 )
- 이 함수를 실행하면, **mutex 의 lock 을 해제하고**, 다른 스레드는 mutex 변수의 lock 을 얻을 수 있다.
- [ 2가지 기능 ]
  - ① mutex lock 을 해제, 다른 스레드에서 mutex 사용
  - ② cond 신호가 올 때까지, **대기**
-------
- 

(4). pthread_cond_timedwait
```
int pthread_cond_timewait(pthread_cond_t *cond, pthread_mutex_t* mutex, const struct timespec* timeout)
```
- cond 변수가 신호를 받을 때까지, timeout만큼의 시간을 기다림
- timeout 이 발생하면, 대기가 해제되고, (( 다시 mutex 를 획득 ))하게 된다.
- 반환 값 : 성공 (0) timeout (ETIMEDOUT)
<br/>

#### 2️⃣ 병렬 처리 문제점
### Ⅱ. 동기화 방법
#### 1️⃣ Mutex

#### 2️⃣ Semaphore
- 이 둘은 어떻게 다른 것인지
- 


### Ⅲ. 기타
#### 1️⃣ 개발
✅ timespec
```cpp
#include <ctime>

struct timespec timeout;
clock_gettime(clockid_t clk_id, struct timespec *tp);
```
- clockid_t clk_id [ CLOCK_REALTIME / CLOCK_MONOTONIC ]
  - CLOCK_REALTIME : 현재의 실제 시간
  - CLOCK_MONOTONIC : 실제 시간이 아닌, 시스템이 부팅된 이후의 상대적인 시간
- struct timespec timeout;
  - tv_sec
    - 1초 단위
    - 1970년 1월 1일 00:00:00 (UTC) 를 기준으로 현재까지의 (( 초 )) 를 저장 ( **epoch** )
  - tv_nsec
    - Nano sec (1,000,000,000)
    - long 타입
<br/>

#### 2️⃣ 참조 사이트
✅ 사이트
- [Busy Waiting] (https://blog.system32.kr/80)









1000000
