### Ⅰ. 프로세스
#### 1️⃣ 프로세스 개념
✅ 배경 지식
- 운영체제의 가장 작은 실행 단위

✅ 메모리 영역
- 각 프로세스는 서로 독립된 메모리 영역을 가진다
- 따라서, 메모리를 공유하기 위해서는, **동시에** 접근이 가능한 메모리 영역이 필요하다 ⇨ [ **메인 메모리** ]
- fork() 를 통한, 부모 ↔ 자식 간의 **프로세스도 메모리를 공유하지 않는다.**
<br/>

#### 2️⃣ 좀비 프로세스
✅ 좀비 프로세스

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/16954122-8264-4527-9e57-4e81ff89701c)

- 자식 프로세스가 종료되면, 운영체제는 부모 프로세스에게 반환
- 이 경우, **부모 프로세스가 자식 프로세스의 종료를 리턴해줄 것을 요청** 해야지, 반환된다.
- 그렇지 않은 경우, 자식 프로세스는 좀비 프로세스가 된다.
  - ex. 부모 프로세스가 자식 프로세스에 비해 먼저 죽을 경우
<br/>

### Ⅱ. 프로세스 개발
#### 1️⃣ 시그널
✅ sigaction
- signal 함수보다 더 많은 기능을 지원

✅ sigaction 구조체
```cpp
struct sigaction
{
  void (*sa_handler)(int);                        // signal 핸들러 상수
  void (*sa_sigaction)(int, siginfo_t*, void*);   // sa_flags = SA_SIGINFO, sa_handler 대신 동작
  sigset_t sa_mask;                               // 시그널 처리 시, 블록 지정할 시그널 마스크
  int sa_flags;
  void (*sa_restorer)(void);
}
```
- sa_flags : 시그널이 들어오면 어떤 동작 ( sa_handler / sa_sigaction ) 을 할지 지정
  - ( sa_flags == SA_SIGINFO ) : sa_sigaction
  - ( sa_flags != SA_SIGINFO ) : sa_handler
- signal_set : 블록할 signal set

✅ 시그널 핸들러 함수
```cpp
void ft_signal_handler(...)
{
  // 시그널 핸들러
}

struct sigaction sig_struct;
sig_struct.sa_sigaction = ft_signal_handler;
sig_struct.sa_flags = SA_SIGINFO;

sigaction(SIGUSR1, sig_struct, NULL);
```

✅ 시그널

(1). SIGCHLD
```
int bRet = sigaction(SIGCHLD, &sig_struct, 0);
```
- 자식 프로세스가 종료되었을 때, 나오는 신호

(2). WNOHANG
```
int bRet = waitpid(-1, &status, WNOHANG);
```
- -1 : 모든 자식 프로세스 종료
- WNOHANG ( int option )
  - 기다리는 PID 가 종료되지 않아서 즉시 종료 상태를 회수 할 수 없는 상황에서
  - 호출자는 차단되지 않고 반환값으로 0을 받음
<br/>

#### 2️⃣ 프로세스 함수
✅ fork()
```cpp
#include <unistd.h>

========  메모리 복사  ========

pid_t pid = fork();
if (pid == 0) {
  // 자식 프로세스
}
else {
  pid = 자식 프로세스의 pid
  // 부모 프로세스
}
```
- 자식 프로세스 생성
- fork() 를 통해 프로세스 생성 및 메모리 복사

✅ wait() / waitpid()
- 자식 프로세스의 상태 변화를 기다리는 시스템 콜 함수
- 좀비 프로세스

(1). wait
```cpp
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int* status)
```
- 자식 프로세스의 상태 변화를 기다림
- 자식 프로세스가 종료된 다음에 자식 프로세스의 종료 상태를 가져올 떄까지, 부모 프로세스는 기다림

(2). waitpid
```cpp
pid_t waitpid(pid_t pid, int* status, int options);
```
- pid_t pid ( 자식 프로세스의 ID )
  - pid  < -1 : pid 의 절대값과 동일한 자식 프로세스 pid
  - pid == -1 : **임의의 자식 프로세스의 종료**
  - pid == 0  : 현재 프로세스의 pid 와 같은 프로세스 그룹 id 를 가지는 모든 자식 프로세스의 종료
- int options
  - options == 0 : wait() 함수와 같이 자식 프로세스의 종료를 기다림
<br/>

#### 3️⃣ 시나리오
✅ for 문 안에 fork()
```cpp
for (int i=1; i<=10; i++)
{
  printf("[%d]", i);
  if (i==3)
    fork();
}
```
- 예상 정답
> 1 <br/> 2 <br/> 3 <br/> 4 ... 10 **( 부모 프로세스 )**<br/> 4 ... 10 **( 자식 프로세스 )**
<br/>

✅ 부모 프로세스 > 자식 프로세스
- 부모 프로세스가 죽지 않은 경우 ( 블로킹 )
- 자식 프로세스가 먼저 다 죽은 경우
---
```
shpark0+ 2999518 99.3  0.0  13784  1776 pts/6    R+   16:08   4:06 ./echo_storeserv_v2
shpark0+ 2999519  0.0  0.0      0     0 pts/6    Z+   16:08   0:00 [echo_storeserv_] <defunct>
shpark0+ 2999520  0.0  0.0      0     0 pts/6    Z+   16:08   0:00 [echo_storeserv_] <defunct>
```
- 자식 프로세스는 죽지 않고 좀비 프로세스가 된다.
<br/>

✅ 부모 프로세스 < 자식 프로세스
- 부모 프로세스가 먼저 죽고
- 자식 프로세스가 늦게 죽는 경우
---
```
shpark0+ 3003177  0.0  0.0  13784   180 pts/6    S    16:19   0:00 ./echo_storeserv_v3
shpark0+ 3003178  0.0  0.0  13784   180 pts/6    S    16:19   0:00 ./echo_storeserv_v3
```
- 자식 프로세스는 쉘 프롬프트 창에, 부모프로세스 끝나고 남은 실행작업을 출력한다.
- 이 경우, **자식 프로세스는 좀비 프로세스가 되지 않는다**
<br/>

### Ⅲ. 프로세스 통신 ( IPC )
#### 1️⃣ MsgQ
✅ 개념
- 다른 프로세스 간의 단방향 통신
- 단방향
- 메모리 | 구조체
- 동일 시스템

✅ 예제
``` cpp
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

int msg_id = msgget(IPC_KEY, IPC_CREAT | 0666);
```
<br/>

#### 2️⃣ IPC
✅ 메모리 영역

(1). 프로세스에 속하는 자원

(2). 운영체제에 속하는 자원
- 프로세스 간의 공유
- IPC : Pipe, Msg, 소켓 등 ( **운영체제 자원** )
- 운영체제가 마련해주는 메모리 영역
- **fork() 를 통해 복사하는 대상이 아니다**
  - 프로세스의 독립된 메모리 자원이 아니기 때문이다.
<br/>

#### 3️⃣ Pipe
✅ 개념
- [부모] → [자식] 프로세스 간의 단방향 통신
- 단방향
- **파일 | 스트림**
- 동일 시스템

🔯 중요 개념
- Pipe 는 **부모 프로세스**가 생성 ( 파이프 생성 주체 )

✅ Pipe 함수

(1). pipe 생성
``` cpp
#include <unistd.h>

int fds[2];
int pipe(int fds[2]);

// pipe(fds);
```
- int fds[2] 파일 디스크립터를 사용하여, 파이프 이용
  - **fds[0]** : 수신 (read)  , 파이프 출구
  - **fds[1]** : 송신 (write) , 파이프 입구
- pipe

✅ 예제
- sigaction
```
#include <signal.h>
#include <wait.h>

void exit_proc(int flag)
{
  int status;
  pid_t pid = wait(&status);
}

struct sigaction sig_struct;
sig_struct.sa_handler = exit_proc;
sig_struct.sa_flags = 0;
sigemptyset(&sig_struct.sa_mask);

sigaction(SIGCHLD, &sig_struct, 0);
```
- exit_proc() : 자식 프로세스가 종료되면, 해당 시그널을 받고 실행됨
- SIGCHLD : 자식 프로세스 종료 조건


✅ 주의 사항 ⚠

(1). Recv ( Read ) 2군데
```cpp
// 부모 프로세스
int w_size = write(fds[1], msg, strlen(msg));
int r_size = read(fds[0], msg, BUF_SIZE);

// 자식 프로세스
int r_size = read(fds[0], msg, BUF_SIZE);
```
- 부모 프로세스에서 보낸 write(fds[1], ... ) 가 [ 부모 ] or [ 자식 ] 중, 어느 프로세스로 read 할 지 모른다. ( 모호하다 )
  - [ 부모 ] 프로세스가 read 할 수 있다.
  - [ 자식 ] 프로세스가 read 할 수도 있다.
 
(2). Read > Write
```cpp
pid_t pid = fork();
if (pid == 0) {        // 자식 프로세스
  read(fds[0], msg, BUF_SIZE);
  printf("(recv): %s",msg);
}

else  {                // 부모 프로세스
  sleep(100);
  write(fds[1], msg, strlen(msg));
  printf("(send): %s", msg);
}
```
- read(fds[0]) 를 write(fds[1]) 보다 먼저 하는 경우
  - 아무 것도 read 하지 못하게 되는 것인지 ❌
  - fds[1] 이 있을 때까지, **블로킹** 되어 있다 &nbsp; ✔
<br/>

### Ⅳ. 기타
#### 1️⃣ 헤더 파일
✅ 구조체 정의
- 잘못된 예시 ❌
```cpp
// echo_store.h

#include <arpa.inet.h>

sturct sockaddr_in serv_addr;

// serv_addr.sin_family = AF_INET;
// serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
// serv_addr.sin_port = htons(8088);
```
- 구조체 멤버의 초기화는 **전역 스코프** 에서 직접 수행할 수 없다.
- 구조체 멤버에 값을 대입하는 것은, 함수 또는 블록 내에서 이루어져야 한다.
---
- 옳은 방식 ✔
```cpp
// echo_store.h

#include <arpa/inet.h>

struct sockaddr_in serv_addr;
void initialize_sockaddr()
{
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(8088);
}
```
```cpp
// echo_store.cpp

int main()
{
  initialize_sockaddr();
}
```
- 이런식으로, cpp 의 main() 함수에서 initialize_sock() 함수를 호출하여, 구조체 멤버 변수를 초기화 해야한다.
<br/>

#### 2️⃣ 참고 사이트
✅ 사이트
- [ Sigaction ] (https://velog.io/@oio337a/sigaction) 

#### 3️⃣ netstat
✅ netstat 포트 확인
```bash
$ netstat -ano | grep "8088"

tcp        0      0 0.0.0.0:8088            0.0.0.0:*               LISTEN      off (0.00/0/0)
tcp        0      0 127.0.0.1:8088          127.0.0.1:52212         ESTABLISHED off (0.00/0/0)
tcp        0      0 127.0.0.1:52212         127.0.0.1:8088          ESTABLISHED off (0.00/0/0)
```
