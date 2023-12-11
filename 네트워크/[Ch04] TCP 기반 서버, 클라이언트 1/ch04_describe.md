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
