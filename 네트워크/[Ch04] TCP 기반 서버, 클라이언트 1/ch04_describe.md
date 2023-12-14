### Ⅰ. 다중 클라이언트 구현
#### 1️⃣ epoll
✅ 코드

(1). epoll_wait
```cpp
#define MAX_EPOLL_EVENTS (20)

epoll_event* observ_ev = (epoll_event*) malloc(sizeof(epoll_event)*MAX_EPOLL_EVENTS);
int fd_num = epoll_wait(epoll_fd, observ_ev, MAX_EPOLL_EVENTS, -1 );
for (int i=0; i<fd_num; i++)
{
  // 모든 이벤트에 대한 처리
  // observ_ev[i].data.fd 에 대한 처리 구현
}
```
- observ_ev : 관찰된 이벤트들을 받아오는 epoll_event
- fd_num : 관찰된 이벤트 갯수
- epoll_wait(epoll_fd, observ_ev, MAX_EPOLL_EVENTS, -1 ) : -1는 무한대기를 의미

(2). print_accept_info
```cpp
char clnt_ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &clnt_addr.sin_addr, clnt_ip, INET_ADDRSTRLEN);

int clnt_port = ntohs(clnt_addr.sin_port);

printf("[clnt] %s:%d", clnt_ip, clnt_port); // [clnt] 127.0.0.1:8088
```

(3). epoll
```cpp
// 1. epoll 저장소 생성
int epoll_fd = epoll_create(1);

// 2. 관찰 이벤트 등록
epoll_event serv_ev;
serv_ev.events = EPOLLIN;
serv_ev.data.fd = serv_sock;

epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &serv_ev);

// 3. 관찰 이벤트 대기
int fd_num = epoll_wait(epoll_fd, observ_ev, MAX_EPOLL_EVENTS, TIME_OUT);
```
- epoll_create(int size)
  - size 는 epoll 크기를 의미
  - -1 일 경우는 안된다, 일반적으로 1로 설정
 
(4). STDIN_FILENO
```cpp
epoll_event event;
event.events = EPOLLIN;
event.data.fd = STDIN_FILENO;

cpoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FIILENO, &event);
```
- 콘솔 입력에 대한 이벤트 처리
- 콘솔 입력 : **STDIN_FILENO**
<br/>

#### 2️⃣ poll
✅ 코드

(1). pollfd 구조체
```cpp
#include <poll.h>

struct pollfd
{
  int fd;           // 감시할 파일디스크립터
  short events;     // 감시할 이벤트 유형 (POLLIN, POLLOUT)
  short revents;    // 실제 발생한 이벤트 
};
```
- short events : 감시할 이벤트 유형 ( POLLIN, POLLOUT )
- short revents : 실제 발생한 이벤트 유형

(2). poll 초기화
```cpp
struct pollfd poll_fds[MAX_CLIENT+1];

// serv_sock
poll_fds[0].fd = serv_sock;
poll_fds[0].events = POLLIN;

// 나머지
for (int i=1; i<=MAX_CLIENT; i++) poll_fds[i].fd = -1;
```
- struct pollfd poll_fds[MAX_CLIENT+1]
  - 배열 선언, struct pollfd *
  - +1 : serv_sock 까지 감시하기 위햄 ( serv_sock + MAX_CLIENT )
 
(3). poll 실행
```cpp
int poll(struct pollfd*, int poll_count, timeout);

for (int i=0; i<=MAX_CLIENT; i++)
{
  if (poll_fds[i].revents == POLLIN)
  {
    // poll_fds[i].fd 에 대한 처리
  }
}
```
- int poll_count
  - pollfd* poll 로 감시해야 할 파일 디스크립터의 갯수
  - 이 코드에서, MAX_CLIENT + 1 가 된다.
- for (int i=0; i<=MAX_CLIENT; i++) : for 문을 돌면서 모든 파일 디스크립터에 대해서 이벤트 발생 여부를 확인
- if (poll_fds[i].revents == POLLIN) : 이벤트 발생 여부 확인

(4). poll 끝
```cpp
close(poll_fds[i].fd);
memset(&poll_fds[i], 0, sizeof(struct pollfd));

poll_fds[i] = -1;
```
- poll_fds[i] 를 모두 0 으로 초기화
- poll_fds[i].fd 파일 디스크립터를 -1 로 할당

### Ⅱ. TCP/UDP 복습
#### 0️⃣. 프로토콜 체계
✅ 프로토콜
- 통신 규약
- TCP / IP 통신 규약
- 프로토콜 <부류> / <체계> 로 나눠짐 ( int domain )

✅ 프로토콜 체계
- PF_INET(IPV4) : IPV4 주소 체계 // 제일 많이 사용
- PF_INET6(IPV6) : IPV6 주소 체계
- PF_LOCAL : LOCAL 통신을 위한 UNIX 프로토콜 체계
- PF_PROTOCOL : Low-Level 소켓을 위한 프로토콜 체계
- PF_IPX : IPX 노벨 프로토콜 체계

✅ 프로토콜 타입 ( PROTOCOL TYPE )
- 데이터 전송 방식 (2가지)
  - [ SOCK_STREAM / SOCK_DGRAM ]
- 같은 프로토콜 체계(IPV4) 에도 여러 데이터 전송 방식이 존재한다. ( TCP / UDP )
- TCP / UDP 는 애초에 데이터 전송 방식이 다르기 때문에, (( 포트 번호가 같아도 구별 ))이 가능하다.
<br/>

#### 1️⃣ TCP
✅ 연결 지향형 소켓 ( SOCK_STREAM )
- [ TCP | 신뢰성 | 순서 보장 | 독립된 별도의 전송 라인 ]
- (( 데이터 유실 없이 )) 목적지까지 데이터 전송
- 순서를 보장
- 데이터의 경계가 없다
  - stream 데이터
  - read / write 호출 횟수가 중요하지 않다.
  - socket buffer ( byte 배열 )
    - 최대 socket buffer 크기 : 8GB ( 8096 KB )
    ```cpp
    #include <sys/socket.h>

    int getsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen);
    ```
    - getsockopt : 소켓 옵션 값을 조회
    - #include <sys/socket.h> 헤더에 포함
    - int level : 소켓 종류 지정 ( ex. SOL_SOCKET, 일반적인 소켓 옵션 프로토콜 )
    - int optname : 조회하고자 하는 옵션의 이름 (SO_RCVBUF : 수신할 버퍼의 크기, SO_SNDBUF : 송신할 버퍼의 크기 )
    - void* optval : 조회한 옵션 값을 저장할 버퍼의 포인터
    - socklen_t* optlen : optval 버퍼의 크기
    - 반홬값 (int) : 성공 (0), 실패 (-1)
    ```cpp
    #include <sys/socket.h>

    int recv_len, send_len;
    socklen_t recv_opt = sizeof(recv_len);
    socklen_t send_opt = sizeof(send_len);

    int rRet = getsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, &recv_len, &recv_opt);
    int sRet = getsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, &send_len, &send_opt);
    printf("recv_size : (%d) send_size : (%d)", recv_len, send_len);
    ```
    - send 할 수 있는 데이터가 꽉 차면, 전송을 하지 않기 때문에, 데이터 유실 위험이 없다.
- socket : socket 이 1:1 매칭이어야 한다.

✅ 특성
- recv / send 가 (( stream )) 처럼 전달 되기 때문에, Packet 의 전체 길이를 **[연동 규격서]** 에 명시해야한다.
<br/>

#### 2️⃣ UDP
✅ 비연결 지향형 소켓 ( SOCK_DGRAM )
- [ UDP | 비신뢰성 | 순서 보장 X | (( 독립된 데이터 패킷 )) | 연결 X ]
- 데이터 유실 가능성이 크고, (( **빠른 전송** )) 을 목적으로 한다.
- 데이터 순서를 보장하지 않는다.
- 데이터의 경계가 있다.
  - 독립된 데이터 패킷
  - 한번에 보낼 수 있는 데이터 패킷의 크기가 제한적이다.
  - (송신) : 두 번의 함수 호출 ( 송신과 수신의 함수 호출 횟수가 동일해야한다.)
  - (수신) : 두 번의 함수 호출
- 1:1 / 1:N 통신이 가능하다.
  - 독립된 데이터 패킷
  - 데이터를 (( 패킷 )) 단위로 전송
  - Multicast, 브로드 캐스트에 사용
- (( 연결 )) 이라는 개념이 없음
<br/>

### Ⅲ. TCP / UDP 전송 과정
#### 1️⃣ TCP (신뢰성)
- 신뢰성
- Transmission Control Protocol

✅ 3 Way-Handshake (연결)

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/6a175da2-6a45-41f7-96a6-564143feb678)

✅ 4 Way-Handshake (연결 해제)
<br/>

#### 2️⃣ UDP (비신뢰성 )
- 비신뢰성
- User Datagram Protocol

✅ 전송 방식

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/2d141843-dbf6-4e06-90de-09ffb02b4003)
<br/>

###  Ⅳ. 기타
#### 1️⃣ ICMP 프로토콜
✅ IP 프로토콜 ( L3 계층 )
- 라우터 : 목적지에 도달하기 위해, 중간에 어떤 경로를 거쳐야 하는 가를 선택, 하지만 그 경로는 일정치 않음
  - 목적지까지 갈 수 있는 최적의 경로로 안내
  - 거쳐가야하는 지점 : Gateway
    - 거쳐야 하는 Gateway 수 : Hop 수
  - metrix 을 바탕으로 최적의 경로를 결정
    - 네트워크 간의 최적의 경로를 가기 위해서, Network / Link 의 순위를 숫자로 변환한 것 
- 따라서, **비연결지향성**, **신뢰 할 수 없는 프로토콜**
- 도중에 오류가 발생하여도, 대비가 되어 있지 않다 ⇨ ICMP

✅ ICMP
- Internet Control Message Protocol
- L3 ( IP / Network Layer )
----
- 주로, 네트워크 간의 (( **통신 문제** ))를 진단하고 (( **제어** ))하기 위해 사용되는 프로토콜
- 네트워크에서 발생할 수 있는 **여러 이벤트, 오류**를 다른 네트워크에 전달 ( 주로, 라우터 / 서버 사용 )
- ICMP 는 (( IP(네트워크) )) 계층에서 동작
- [ IP Header ][ ICMP 헤더 ]

✅ 특징
- TCP / UDP 와 달리, L4 ( 전송 계층 ) 과 연결되어 있지 않다
  - 두 네트워크 장비 간의 (( 연결 ))을 전제로 하지 않기 때문이다
  - ICMP 메시지를 보내기 전, 두 장치간의 (( 연결 )) 작업이 필요하지 않다.
  - 데이터 교환 작업 X
- **특정 Port 를 대상으로 연결하는 것을 허용하지 않는다.**

✅ 기능

(1). ping
- Host 또는 네트워크 장비가 다른 호스트나 장비에게 도달하는지 확인 여부

(2). 오류 보고
- "목적지 호스트 도달 불가"와 같은 오류 메시지를 생성하여 발신자에게 전달

(3). MTU Path Discovery
- IP 페킷이 지나가는 경로의 최대 전송 단위( MTU )를 결정하기 위해서, **ICMP 메시지**를 사용

(4). 라우터 간 메시지 통신
- 라우터는, ICMP 메시지를 통해, 네트워크 이상 상태를 감지하고, 해당 정보를 다른 네트워크에 전달 할 수 있도록 함
<br/>

#### 2️⃣ 데이터 단위
✅ 데이터 단위

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/0fed8ec5-99c8-4a3b-b16d-5ba5e22b74e3)
- 패킷 : 데이터 전송의 기본 단위
<br/>

#### 3️⃣ 코드 복습
✅ multi_accept_serv / clnt.cpp
```cpp
typedef struct clnt_info
{
  int fd;
  unsigned long pthread_id;
  pthread_t* recv_pth;
  bool bExit;
} clnt_info_t;

std::map<int,clnt_info_t*> clnt_map;

void* clnt_garbage_func(void* arg)
{
  while(!bExit)
  {
    if (info->bExit == true)
    {
      pthread_join(*info->recv_pth, NULL);

      int clnt_fd = info->fd;
      free(info->recv_pth);
      free(info);

      clnt_map.erase(clnt_fd);
    }
  }
}
```
- clnt 의 recv_thread 가 완전히 종료되면, 해당 clnt_info 의 메모리 공간을 free 하는 방법
