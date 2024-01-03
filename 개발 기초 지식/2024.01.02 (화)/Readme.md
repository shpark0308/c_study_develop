### Ⅰ. 폴링과 인터럽트
#### 1️⃣ 외부 이벤트 처리
- [컴퓨터 시스템] ⇦ 외부 이벤트 처리
  - (1) 폴링
  - (2) 인터럽트

✅ 외부 이벤트

(1). 하드웨어 이벤트
- CPU 가 하드웨어 디바이스로부터 인터럽트를 받는 경우
- I/O 로부터 데이터 도작 등

(2). 키보드 또는 마우스 입력

(3). **네트워크 이벤트**
- 데이터 패킷 도착, 연결 요청, 연결 종료 등
- 소켓 tcp 처리 등

(4). **시간 이벤트**
- 특정 시간이 지났을 때 발생하는 이벤트

(5). **소프트웨어 이벤트**
- 다른 소프트웨어 모듈에서 발생하는 이벤트
- 다른 프로세스로부터 메시지 수진, 파일 시스템에서의 변경등 이벤트

(6). 사용자 이벤트
- 버튼 클릭 등

✅ 스레드
- [외부 이벤트] 발생 → (( **인터럽트** )) → 현재 스레드 중단 → 특정 [인터럽트 서비스 루틴] 실행 → 원래 스레드 복귀
---
- 인터럽트 발생 시, (( OS )) 는 현재 실행 중인 스레드 중단
- 그 후, 인터럽트 서비스 루틴 ( ISR ) 실행
- ISR은 외부 이벤트에 대한 처리 수행
- ISR 이 끝나면, 원래 스레드로 복귀

🔯 스레드 간의 통신
- 같은 [프로세스] 내에서, 메모리를 공유하고 있기 때문에, 공유 데이터를 함께 사용할 수 있다.
- 따라서, (( 동기화 )) 작업을 잘 해야한다.

🔯 스레드

① 같은 프로세스 내 데이터 공유 <br/>
② 병렬 처리  ⇒ **동기화 작업**이 중요


✅ 프로세스
- [외부 이벤트] 발생 → (( **인터럽트** )) → 현재 프로세스 중단 → 특정 [인터럽트 서비스 루틴] 실행
---
- 인터럽트 발생 시, (( OS )) 는 현재 실행 중인 프로세스 중단
- 그 후, 특정 인터럽트 서비스 루틴 ( ISR ) 실행
- [프로세스] 가 여러 (( 스레드 )) 로 이루어져 있을 경우,
  - 인터럽트는 전체 프로세스를 중단 시키는 것이 아님 ❌ 
  - 해당 프로세스 내의 **특정 스레드에게만 전달** ✔
- ISR 은 외부 이벤트에 대한 처리 수행
- ISR 이 끝나면, 원래 스레드로 복귀

🔯 프로세스 간의 통신
- 프로세스는 각각의 [ 독립된 메모리 공간 ]을 갖기 때문에, 데이터를 공유하기 위해서, IPC 통신을 해야한다.
- 프로세스는 [자신의 메모리 영역]을 가지고, 이를 다른 프로세스가 접근하여 침범하려고 하면, 커널은 경고 (( **시그널** ))을 보낸다

🔯 IPC - SharedMemory
- 프로세스는 각각의 독립된 메모리를 가지고 있음에도 불구하고, IPC 통신으로 **SharedMemory** 를 구현
---
- [동일한 물리적인 메모리 영역]을 공유하여, 데이터를 주고 받음
- 이를 통해, 프로세스 간의 빠르게 데이터 공유 및 통신

### Ⅱ. SharedMemory
#### 1️⃣ SharedMemory
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/aa03a3ef-d31f-47ec-ad13-1c5076e67360)

✅ SharedMemory

(1). 여러 프로세스 간의 데이터 공유
- [프로세스] 는 각각의 독립된 메모리를 가졌음에도 불구하고, (( 동일한 물리적인 메모리 영역 ))을 공유
- IPC 방법 중에서 가장 빠르다.

(2). 하나의 프로세스 사용
- 공유하지 않을 것임에도 불구하고, 하나의 프로세스에서 사용하는 이유
- 프로세스가 재시작해도, 해당 데이터를 그대로 사용하고 싶은 경우, Shared Memory를 이용

✅ 구현 방법

(1). 공유 메모리 (( 생성 ))
```cpp
int shmget(key_t key, size_t size, int shmflg);
// size_t size : 공유 메모리 크기
// -1 ( 실패 )

int shm_id = shmget(key_id, sizeof(shm_t), IPC_CREAT|0666);
```
- 하나의 프로세스가 공유할 메모리를 생성
- 해당 메모리는 (( 다른 프로세스 )) 와 공유하고자 하는 데이터 구조 또는 변수 저장

(2). 키 할당
- 공유 메모리를 식별하기 위해 Key (키) 를 할당
- 키는 일반적으로 **정수**로 구성하며, 시스템 전체에서 유일해야한다.
  - 이것을 어떻게 시스템 OS 에서 유일한 정수 값을 부여한는지

(3). 공유 메모리 접근
```cpp
void* shmat(int shmid, const void* shmaddr, int shmflg);
// const void* shmaddr : 원하는 메모리 주소를 지정하는데 사용되는 인자
```
- shmat 은 공유 메모리 접근 및 성공 ( 연결된 메모리 세그먼트의 시작 주소 반환 ), 실패 ( -1 )
- const void* shmaddr
  - 원하는 메모리 주소를 지정하는데 사용되는 인자
  - 일반적으로, NULL 을 전달하면, 시스템이 적절한 주소를 선택
- shmflg : 옵션 플래그 0 ( 보통 ), SHM_RDONLY
---
- 다른 프로세스는 (( **동일한 키** )) 를 사용하여, 공유 메모리에 접근한다.
  - 다른 프로세스는 이 동일한 키를 어떻게 전달받는가?
- 이떄, (( 동일한 키 )) 를 사용하여, OS 의 함수나 시스템 콜을 사용하여, 공유 메모리에 attach (연결) 한다.

```cpp
int shmdt(const void* shm_addr);
```
- shmdt : 공유 메모리 연결 / 접근 해제
- const void* shm_addr : 공유 메모리 세그먼트의 시작 주소

(4). 데이터 교환
- 그럼 shared memory 에 대한 동기화 작업은 어떻게 이루어져 있는가

(5). 공유 메모리 삭제
```cpp
int shmctl(int shmid, int cmd, struct shmid_ds *buf);

int bRet = shmctl(shm_id, IPC_RMID, NULL);
```
- int cmd : 수행할 제어 연산을 지정
  - IPC_STAT ( buf 에 전달된 구조체에 현재 공유 메모리 상태 정보 저장 )
  - IPC_SET ( buf 에 전달된 구조체의 정보를 기반으로 공유 메모리 속성 설정 )
  - IPC_RMID ( 공유 메모리 세그먼트 삭제 )

✅ 코드
``` cpp
#include <sys/shm.h>
#include <sys/ipc.h>

int main()
{
  // (1). IPC 키 생성
  key_t key_id = ftok("hello.txt", 'R');

  // (2). SHM_ID 생성
  int shm_id = shmget(key_id, SHM_SIZE, IPC_CREAT | 0666 );

  // (3). 공유 메모리 접근 ( attach )
  void * shm_addr = shmat(shm_id, NULL, 0);
  if ( shm_addr == (void*) -1) err_handle("shmat() error");

  // (4). 공유 메모리 해제 ( deattach )
  int bRet = shmdt(shm_addr);
  if ( bRet == -1 ) err_handle("shmdt() error");

  // (5). 공유 메모리 삭제
  shmctl(shm_id, IPC_RMID, NULL);
}
```

#### 2️⃣ 동기화
✅ mutexattr
(1). pthread_mutexattr_setpshared
```cpp
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
```
- int pshared = process shared
  - PTHREAD_PROCESS_PRIVATE : mutex 를 프로세스 간에 공유하지 않음
  - PTHREAD_PROCESS_SHARED  : mutex 를 프로세스 간에 공유
- 성공 ( 0 ) 실패 ( 에러 코드 )

### Ⅲ. 기타
#### 1️⃣ 참조 사이트
✅ 사이트
- [프로세스 동기화] (https://velog.io/@mardi2020/%ED%94%84%EB%A1%9C%EC%84%B8%EC%8A%A4-%EB%8F%99%EA%B8%B0%ED%99%94)
