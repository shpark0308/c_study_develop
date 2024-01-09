ⅠⅡⅢⅣⅤ1️⃣2️⃣3️⃣✅

### Ⅰ. IPC
#### 1️⃣ IPC 배경 지식
✅ Process
- 운영체제 실행의 가장 작은 단위
- 프로세스는 각 각의 독립된 메모리 공간을 가지고 있음

✅ IPC 종류
|IPC 종류|사용 시기|공유 매개체|통신 단위|통신 방향|통신 가능 범위|
|:------:|:------:|:--------:|:------:|:-------:|:-----------:|
|PIPE|[부모] ↔ [자식] 간의 단방향 통신|파일|stream|단방향|동일 시스템|
|Named PIPE|다른 프로세스와 단방향 통신|파일|stream|단방향|동일 시스템|
|Message Queue|다른 프로세스와 단방향 통신|메모리|구조체|단방향|동일 시스템|
|Shared Memory|다른 프로세스와 양방향 통신|메모리|구조체|양방향|동일 시스템|
|Memory Map|다른 프로세스와 양방향 통신|파일+메모리|페이지|양방향|동일 시스템|
|socket|다른 시스템과 양방향 통신|소켓|stream|양방향|동일+외부 시스템|

(1). 통신 방향
- 단방향 : 통신 경로를 2개 뚫어야 한다
  - [send] → [recv]
  - [recv] ← [send]
- 양방향 : socket 은 하나만 파일디스크립터를 생성 ex. shared memory, socket

(2). 공유 매개체
- 공유 매개체 : 파일 ⇨ 통신 단위 : stream
- 공유 매개체 : 메모리 ⇨ 통신 단위 : 구조체

✅ IPC 코드
```cpp
#include <sys/ipc.h>

int key_id = ftok(const char* pathname, int project_id);
int key_id = ftok("hello.txt", 'A');
```
- IPC 통신을 위해, 프로세스 간에 필요한 key_id 가 있어야 한다.
<br/>

### Ⅱ. Message Queue
#### 1️⃣ 메시지 큐
✅ 메시지 큐
- 사용 시기 : 다른 프로세스 간의 **단방향 통신**
- 공유 매게체 : 메모리
- 통신 단위 : 구조체
- 통신 방향 : **단방향**
- 통신 가능 범위 : 동일 시스템
---
- [단방향] 이기 때문에, 2개의 통로를 생성
  - [send] → [recv]
  - [recv] ← [send]

#### 2️⃣ 코드
✅ 메시지 정의
```cpp
typedef struct _MsgType
{
  long mtype;
  char mtext[16];
} MsgType;
```
- mtype : 첫번째 필드는 반드시 **long 타입의 변수**이어야 한다.
- mtext : 실제 사용 데이터
---
- Mesage 구조체 정의
- **어떤 데이터를 주고 받을 것인지** 정의 ( mtype )

✅ 함수

(1). msgget
```cpp
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

key_t key_id = 1234;
int msg_id = msgget(key_id, IPC_CREAT|0666);
```
- int msgget(key_t key_id, int flag);
- ( 송신자 )와 ( 수신자 ) 모두 동일한 Msg ID 를 가지고 있어야 한다.
- key_t key_id 가 동일하면 msg_id 도 동일하다

(2). msgsnd
```cpp
MsgType msg;

// 데이터 정의
msg.mtype = 1;
strcpy(msg.mtext, "hello.txt");

int bRet = msgsnd(msg_id, msg, sizeof(msg.mtext) , 0);
if (bRet == -1) err_handle("msgsnd error()");
```

(3). msgrcv
```cpp
MsgType msg;

size_t msg_size = msgrcv(msg_id, (void*)&msg, sizeof(msg.mtext), long type, IPC_NOWAIT);
```
- size_t size = msgrcv(int msg_id, (void*) &msg_data, sizeof(msg_data), int flag);
- IPC_NOWAIT : 데이터 블로킹을 방지
- msgsnd / msgrcv 모두 MsgType msg를 전달하지만, **실제 전달하는 데이터는 msg.mtext** 로 보내는 데이터 크기도 sizeof(msg.mtext) 이다.
- **long type** 에 맞는 데이터만 recv

(4). msgctl
```cpp
int msgctl(int msg_id, IPC_RMID, nullptr);
```
- 메시지 큐 종료
- **상대편이 메세지를 받기 전가지 msgctl(msg_id, IPC_RMID .. ) 로 msg_id 를 지우면 안된다.**

✅ 코드
```cpp
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys.msg.h>

int msgget(key_t key, int flag);
int msgsnd(int msg_id, const void* msg_q, size_t msg_size, int flag );
size_t msgrcv(int msg_id, void* msg_q, size_t msg_size, long msg_type, int flag);
```

✅ 명령어
```bash
$ ipcs
$ ipcrm -q [msg_id]
$ ipcrm -Q [key_id]
```
- 결괏값
```
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status

------ Semaphore Arrays --------
key        semid      owner      perms      nsems
```

### Ⅳ. 기타
#### 1️⃣ 파일 전송
✅ scp
```bash
$ scp [파일 명] root@[원격 IP 주소]:[받는 위치]

// scp configure.xml shpark0308@10.202.18.91:/svc/pandora
// scp -r directory shpark0308@10.202.18.91:/svc/pandora
```
- scp **-r** : 하위 디렉토리 포함하여, 전송
<br/>

#### 2️⃣ 참고 사이트
✅ 사이트
- [ IPC 종류(6가지) ] (https://dar0m.tistory.com/233)






