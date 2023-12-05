### Ⅰ. 기본 지식
#### 0️⃣ socket 생성
✅ int socket(int domain, int type, int protocol )
- 반환자 (int) : 성공 ( 파일 디스크립터 ) 실패 ( -1 )
- int domain : 프로토콜 체계
- int type : 데이터 전송 방식
- int protocol : 프로토콜 정보

✅ 프로토콜
- 통신 규약
- TCP / IP 통신 규약
- 프로토콜은 <부류> / <체계> 로 나눠짐 **(int domain)**
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/80552543-c27c-4e75-86b5-214269abf3b6)

✅ 프로토콜 체계 ( Protocol Family ) <br/>
(1). PF_INET(AF_INET) : IPV4 주소 체계 // 제일 많이 사용 <br/>
(2). PF_INET6 : IPV6 주소 체계 <br/>
(3). PF_LOCAL : LOCAL 통신을 위한 UNIX 프로토콜 체계 <br/>
(4). PF_PROTOCOL : Low-Level 소켓을 위한 프로토콜 체계 <br/>
(5). PF_IPX : IPX 노벨 프로토콜 체계<br/>

✅ 프로토콜 타입 ( Protocol Type )
- 데이터 전송 방식 **(2가지)**
- 같은 프로토콜 체계 ( IPV4 ) 에도 여러 데이터 전송 방식이 존재한다 ( TCP / UDP )

(1). 연결 지향성 소켓 ( SOCK_STREAM ) <br/>
- [ TCP | 신뢰성 | 독립된 별도의 전송 라인 ]
- 데이터 손실 없이 목적지에 도착
- 순서를 보장
- 데이터 경계가 없다
  - read / write 호출 횟수가 중요하지 않는다
  - socket buffer ( byte 배열 ) <br/>
     ![image](https://github.com/shpark0308/c_study_develop/assets/60208434/a93e89ac-9e59-45df-a816-e9aa5a231c29)
    - 최대 socket buffer 크기 : 8GB ( 8096 KB )
    - ```cpp
      #include <sys/socket.h>

      int getsockopt(int sockfd, int level, int optname, void* optval, socklen_t *optlen);
      ```
        - getsockopt : 소켓 옵션 값을 조회
        - #include <sys/socket.h> 에 포함
        - int level : 소켓 종류 지정 ( ex. SOL_SOCKET , 일반적인 소켓 옵션 프로토콜 )
        - int optname : 조회하고자 하는 옵션의 이름 ( SO_RCVBUF, 수신 버퍼의 크기, SO_SNDBUF, 송신 버퍼의 크기 )
        - void* optval : 조회한 옵션 값을 저장할 버퍼의 포인터
        - socklen_t *optlen : optval 버퍼의 크기
        - 반환값 (int) : 성공 ( 0 ) 실패 ( -1 )
    - ```cpp
      int recv_size, send_size;
      socklen_t recv_opt = sizeof(recv_size);
      socklen_t send_opt = sizeof(send_size);

      int rRet = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_size, &recv_opt);
      int sRet = getsockopt(sock, SOL_SOCKET, SO__SNDBUF, &send_size, &send_opt);
      printf("recv_size : %d send_size : %d"); // 87380, 16384
      ```
      ![image](https://github.com/shpark0308/c_study_develop/assets/60208434/c8ed112b-3460-49c2-9f6f-39565dee901c)
        - socket buffer 의 최대 크기는 일반적으로 8GB ( 8096 GB )
        - send 할 수 있는 데이터가 꽉 차면, 전송을 하지 않기 때문에, **데이터 유실에 대한 위험은 없다.**
- **소캣 대 소켓이 1:1 이어야 한다.**
      
(2). 비연결 지향성 소켓 ( SOCK_DGRAM ) <br/>
- [ UDP | 비신뢰성 | 독립된 데이터 패킷 | 연결 X ]
- 데이터 유실 가능성이 크고, **빠른 전송**을 목적으로 한다.
- 데이터 순서를 보장하지 않는다
- 한번에 (( 보낼 수 있는 데이터의 크기가 제한 )) 되어 있다
- 데이터의 경계가 있다
  - (송신) : 두 번 함수 호출 ( 송신과 수신이 동일한 횟수로 함수를 호출해야 한다 )
  - (수신) : 두 번 함수 호출
- 1:1 / 1:N 통신도 가능하다
  - 데이터를 패킷 단위로 전송
  - 각 패킷은 독립적적
  - Mulicast, 브로드 캐스트에 사용
- (( 연결 )) 이라는 개념이 없음

✅ 프로토콜 ( int protocol )
- socket 이 (( 최종적으로 )) 사용할 프로토콜 정보
  - 앞서, [ 프로토콜 체계 ], [ 데이터 전송 방식 ] 으로도 충분히 프로토콜 정보를 제공할 수 있지만
  >하나의 프로토콜 체계 안에 동일한 데이터 전송방식을 같은 동일한 프로토콜이 둘 이상 존재할 수 있다.

(1). 연결 지향형 소켓   ( TCP )
```cpp
int tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```
(2). 비연결 지향성 소켓 ( UDP )
```cpp
int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
```
<br/>

#### 1️⃣ TCP 프로토콜
✅ 특성
- recv / send 가 (( **stream** )) 처럼 전달되기 때문에, Packet 의 (( 전체 길이 ))를 [ 연동 규격서 ]에 명시해야한다.
