
1️⃣2️⃣3️⃣4️⃣✅ⅠⅡⅢⅣ

### Ⅰ. 소켓 옵션
#### 1️⃣ 함수
✅ GET
```cpp
int data;
int data_size = sizeof(data);

int bRet = getsockopt(serv_sock, int level, int opt, (void*) &data, (socklen_t*)&data_size);
```
- int level [ SOL_SOCKET | IPPROTO_TCP | IPPROTO_IP ]
- int opt ( ex. SO_SNDBUF, TCP_KEEPALIVE 등 )
- void* data : GET 데이터 버퍼를 저장하는 곳 ( 포인터 )
- socklen_t* data_size : GET 데이터 버퍼의 크기
---
- 성공 (0) 실패 (-1)

✅ SET
```CPP
int bRet = setsockopt(serv_sock, int level, int opt, (const void*) data, (socklen_t) data_size);
```
- 성공 (0) 실패 (-1)

#### 2️⃣ 소켓 옵션 종류
✅ SOL_SOCKET ( 9가지 )
- L5 계층 ( 세션 계층 )

| Idx | 프로토콜 레벨 | 옵션 |
|:---:|:---:|:---:|
|1|SOL_SOCKET|SO_SNDBUF|
|2|SOL_SOCKET|SO_RCVBUF|
|3|SOL_SOCKET|SO_REUSEADDR|
|4|SOL_SOCKET|SO_KEEPALIVE|
|5|SOL_SOCKET|SO_BROADCAST|
|6|SOL_SOCKET|SO_DONTROUTE|
|7|SOL_SOCKET|SO_OOBINLINE|
|8|SOL_SOCKET|SO_TYPE|
|9|SOL_SOCKET|SO_ERROR|
---
(1). SO_SNDBUF

(2). SO_RCVBUF

(3). SO_REUSEADDR

(4). SO_KEEPALIVE

(5). SO_BROADCAST
- 멀티 캐스트

(6). SO_DONTROUTE
- 데이터 전송 시, 라우팅 테이블을 사용하는데, 이 **라우팅 테이블을 사용하지 않음**
- 직접적인 경로 선정

(7). SO_OOBINLINE
- OOB ( Out of Bound ) : 비정상 데이터
- 데이터 수신 시, 비정상 데이터는 수신하지 않고 따로 처리 ( 버리기 )
- SO_OOBINLINE (1)로 설정 시, 비정상 데이터도 정상 데이터와 함께 수신하도록 설정

(8). SO_TYPE

(9). SO_ERROR


✅ IPPROTO_TCP

✅ IPPROTO_IP

### Ⅱ. SOL_SOCKET
#### 1️⃣ 
✅ 

#### 2️⃣ SO_REUSEADDR
✅ 4-way handshake

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/b568a339-5fd7-4dfc-b96a-3ded045e6995)

(1). [ 서버 ] → [ 클라이언트 ] 로 보낸 마지막 ACK 가 클라이언트에게 제대로 전송되지 못한 채, close() 될 경우
- 클라이언트는 서버의 ACK (종료 ACK) 을 받지 못하여, **계속 종료하지 못할 것이다**
- 일정 시간 ( Timeout )만큼, ACK 를 받지 못한 클라이언트는, 패킷을 **재전송** 을 할 것이다.
- 이때, 재전송과 같이 close() 종료 이후, **패킷을 받아오기 위해**
  - 소켓은 닫혔지만 ( close() )
  - **커널** 에서는 세션을 TIME-WAIT 시간만큼 유지한다.

(2). TIME-WAIT
```bash
$ cat /proc/sys/net/ipv4/tcp_fin_timeout      // 60초 (리눅스)
```
- 리눅스에서는 일반적으로, 60초 time-wait 시간을 발생한다.
- 먼저 FIN 을 요청한 쪽에서, TIME-WAIT 이 발생한다.
- 보통, 서버에서 발생한다.
- 이 time-wait 기간동안, 다시 서버를 재시작하면, 그 시간만큼 또 time-wait 이 발생한다.

✅ Bind 에러
- Time-Wait 기간동안, 서버를 Bind() 할 경우, 이전에 사용했던 포트가 아직까지 세션을 유지하고 있기 때문에, <br/>
  bind() 에러가 발생한다.
- 이전에 사용했던 동일한 포트를, 서버에 다시 할당할 수 있게 하는 옵션이, **SO_REUSEADDR** 이다.

✅ SO_REUSEADDR
```cpp
int opt = 1;

int bRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt));
```
- Time-Wait 기간과 무관하게, 서버를 동일한 포트로 bind() 할 수 있다
- default 값으로, SO_REUSEADDR는 (0)이기 떄문에 (1) 로 셋팅해주어야 한다.
<br/>

#### 3️⃣ SO_KEEPALIVE
✅ SO_KEEPALIVE
- L5 세션 계층
- TCP 소켓이 일정 시간동안 데이터 전송이 없을 경우, 시스템은 자동으로 keep-alive를 통해 상대방이 **연결 여부**를 확인
- keep-alive 메시지에 대해, 응답이 없을 경우, 연결 끊김으로 인식하고 연결 종료
- keep-alive [ 메시지 ] 전송

🔯 판도라
```cpp
opt->keepalive_time_ms = 60000; // 60초 설정
```
- 현재, 판도라는 keepalive_time_ms (60초)로 설정

✅ TCP_KEEPALIVE
- L4 전송 계층
- TCP 소켓이 일정 시간동안 데이터 전송이 없을 경우, 상대방에게 재확인 패킷을 전송하여, **연결 여부**를 확인
- keep-alive [ 패킷 ] 전송

🔯 리눅스 TCP_KEEPALIVE
```cpp
$ cd /proc/sys/net/ipv4

$ cat tcp_keepalive_time      // 7200초 (2시간)
$ cat tcp_keepalive_intvl     // 75초
$ cat tcp_keepalive_probes    // 9번
```
- tcp_keepalive_time : 초기에 2시간동안, 아무 데이터 전송이 없을 경우, keep-alive 메시지 전송을 통해 연결 여부를 확인한다.
- tcp_keepalive_intvl : 첫번쨰와, 두번째 keep-alive 패킷 사이의 interval
- tcp_keepalive_probes : keep-alive 패킷을 전송하는 횟수


### Ⅳ. 기타
#### 1️⃣ 판도라
✅ 소켓 옵션
- cs_socketutil.cpp
  - tcp ( **CS_Socket_TcpSetting** )
  - udp ( **CS_Socket_UdpSetting** )
- cs_tcpsocket.cpp
  - tcp 관련 데이터 구조
  - TcpSocketOption ( sendBufSize, recvBufSize, MTU Size, keepalive_time_ms .... )

#### 2️⃣ 연결 종료
✅ Gracefully 종료

(1). close
```cpp
close(clnt_sock);
close(serv_sock);
```
- 양 쪽 모두, 합의하에, 연결을 종료하는 것

(2). shutdown
```cpp
shutdown(serv_sock, SHUT_WR);
```
- 한쪽 스트림만 닫기
- EOF 를 전달하여, 해당 스트림을 닫을 것을 상대편에게 알림

✅ Force 종료
- 소켓 버퍼에 남아있는 데이터들을 버리기 ( **비정상 종료** )
- linger.l_onoff = 1, linger.l_linger = 0 설정
