


ⅠⅡⅢ2️⃣3️⃣4️⃣✅1️⃣

### Ⅰ. 네이글 알고리즘
#### 1️⃣ 네이글 알고리즘 ( Nagle )
✅ 배경
>> Packet : [ Header | 데이터 ]
- 

TCP
- (( 종료 상황 )) 프로그램 강제 종료 )) / (( 정상 종료 )) ==> bind 에러 발생
  - 왜?
    - 종료 ( 입력 스트림, 출력 스트림 ( 스트림 2개 ) 를 모두 끊어내는 = close(sock);
    - 이렇게 하고도, 소켓을 소멸시켜도,
      - 커널은 바로 소멸시키지 X
      - 몇 초 정도 세션을 더 유지 ( 클라이언트와 아직 처리되지 않은 것을을 처리 ) : TIME_WAIT 상태
        - 따라서, 커널에서는 바로 죽지 않기 때문에, 같은 포트로 또 연결 요청하면, 포트 충돌로 인해 에러 발생
``` bash
$ cat /proc/sys/net/ipv4/tcp_fin_timeout // 리눅스 (60초)
```
- 대다수 커널 관련한 정보가 /proc/sys 에 존재
- /proc/sys/net/ipv4 에 tcp 통신과 관련된 커널 정보들이 존재
---
SO_REUSEADDR
- SO_REUSEADDR = TRUE
  - TIME_WAIT 상태에 있는 소켓에 할당된 IP 주소와 포트를 새로 시작하는 소켓에 할당해주게 된다.
 >> 마지막에, 


### Ⅰ. 캡슐화
#### 1️⃣ 캡슐화
✅ 캡슐화
- Header 를 붙혀가는 과정
  - IP 주소 ( Source, Destination )
  - Port 번호 ( Source, Destination )
- 출발지 정보, 목적지 정보 또는 에러 체크 등의 필요 정보를 포함

✅ 캡슐화 과정
- PDU ( Protocol Data Unit )
---
(1). 응용 계층 ( L7 Application )
> **메세지**
- [응용 Header] + [User Data]

(2). 전송 계층 ( L4 Transmission )
> **TCP 세그먼트**
- [TCP Header] + [응용 Header] + [User Data]
- Src Port | Dest Port
- TCP Header : 40 byte
- segmentation 화, stream 데이터를 나누는 과정

(3). 네트워크 계층 ( L3 Network )
> **Header**
- [IP Header] + [TCP Header] + [응용 Header] + [User Data]
- [ Src IP | Dst IP ] + [ Src Port | Dst Port ]

(4). 데이터 링크 계층 ( L2 Data Link )
> **Frame**
- [[ Ethernet Header ] + [IP Header] + [TCP Header] + [응용 Header]] + [User Data]
- [ Src Mac Address | Dst Mac Address] + [ Src IP | Dst IP ] + [ Src Port | Dst Port ]
<br/>

#### 2️⃣ 역캡슐화
✅ 역캡슐화
- Header 를 제거하는 과정

### Ⅱ. 네이글 알고리즘
#### 1️⃣ 배경
✅ 배경
- [[ 여러 패킷 ]] + **[User Data]**
- [ MacAddress | IP | Port | 응용 ] Header
---
- User Data 가 1byte 와 같이 작을 경우, 붙혀야 하는 패킷의 바이트 크기가 더 크게 된다.
  - ex. **( 여러 패킷 )** ( 120 byte ) + **( User Data )** ( 1 byte )
- 작은 크기의 데이터를 보낼 경우, 패킷을 여러 차례 비효율적으로 보내게 된다.
<br/>

### 2️⃣ Nagle On
✅ **네이글 알고리즘**
- ACK 를 받지 못한 경우, [ 버퍼 ] 에 데이터를 차곡 차곡 쌓음
- 이럴 경우, 전달할 수 있는 데이터의 크기를 늘리고 + [패킷 Header] 를 붙혀, 효율적으로 패킷을 전송

🔯 설명
- 예시). Good 데이터

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/8a0a9bde-ee22-4da5-855a-0d1378e06527)

(1). 먼저 G 를 보내고, ACK 를 기다림 <br/>
(2). ACK 를 받을 때까지, o,o,d 를 차곡 차곡 버퍼에 넣음 <br/>
(3). ACK 를 수신하면, 버퍼에 있는 o,o,d 를 전송

---
- 이러한 경우, 여러 차례 패킷을 보내기 보다는, 패킷 전송 횟수를 절감할 수 있다.
- 하지만, 전송 속도가 느리다

✅ Nagle On
``` bash
if there is new data to send then
  if the window_size ⪰ MSS and available data is ⪰ MSS then
    send complete MSS segment now
  else
    if there is unconfirmed data still in the pipe then
      enqueue data in the buffer until an acknowledge is received
    else
      send data is immediately
    end if
  end if
end if
```
- unconfirmed data : [ 전송된 Data ] 에 대한, **ACK** 를 받지 못한 데이터
---
- Data ⪰ MSS 보다 큰 데이터에 대해서는 전송
- Data ⪯ MSS 일 경우
  - ACK 를 받지 못하면, [ 버퍼 ]에 계속 데이터를 차곡 차곡 쌓음
  - ACK 를 받는 경우, 데이터를 즉시 전달

✅ 장점
- 네트워크를 통해 전송해야 하는 **패킷 수를 줄임**
- 데이터 크기를 늘려서 전송하기 때문에, **효율적인 대역폭을 사용**
  - 반면, 작은 데이터 패킷을 전송할 경우, 작은 패킷을 빈번하게 전송하여, 비효율적
- TCP / IP 네트워크의 **효율성을 높임**
  - 네트워크 부하 감소

✅ 단점
- 작은 패킷이 누적되어, 전송 지연을 발생시킬 수 있다.
- 전송 속도가 느리다
<br/>

#### 3️⃣ Nagle Off
✅ Nagle Off
- 잦은 패킷 전송이 있을 수 있으나, **전송 속도가 빠르다**
- 하지만, 빈번하게 패킷을 전송하기 때문에, **혼잡 네트워크**을 유발할 수 있다.
- 네트워크 부하 증가
<br/>

#### 4️⃣ TCP_NODELAY
✅ Nagle On 의 문제점
- 패컷 전송 수를 줄여서, **네트워크 번잡도를 줄임**, **효울성 극대화**
- **전송 속도가 느리다**

✅ TCP_NODELAY

- Nagle 알고리즘 (( 비활성화 ))
- 작은 패킷을 조합하지( 버퍼에 쌓이지 ) 않고, 즉시 전송하도록 하는 옵션
---
⇨ 작은 패킷을 누적시켜, 전송 지연을 유발하지 않고, 즉시 전송

✅ 코드
```cpp
int enable = 1;
int setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, (void*)enable, sizeof(enable));
```
<br/>

### Ⅲ. 소켓 옵션
#### 1️⃣ SO_LINGER
✅ SO_LINGER
- 소켓 close 후, 전송되지 않은 데이터를 어떻게 처리할지에 대한 옵션
  - ex. 즉시 버림
- TIME-WAIT 시간을 조정

✅ TIME-WAIT
- 4 way-handshake 에서 ( 클라이언트 ) → ( 서버 ) 에게 마지막 FIN 에 대한 **ACK** 를 보낸 후, TIME-WAIT 발생
- 일정 시간동안 세선을 유지하며, 데이터를 보장하도록 동작
- 이는, 소켓 close를 하여도, **커널은 time-wait** 을 유지한다.
- 따라서, 이 time-wait 기간동안, 다시 bind() 요청을 할 경우, 에러가 발생

✅ linger 구조체
```cpp
struct linger
{
  int l_onoff;    // Linger 옵션 활셩화 여부
  int l_linger;   // Linger 옵션 활성화 Wait 시간
}
```
- l_onoff ( Linger 옵션 활성화 ) : 지정된 시간까지 버퍼에 남아 있는 데이터를 모두 전송할지 여부

🔯 close 3가지 옵션
- linger 변수에 따른 close 3가지 옵션

(1). l_onoff = 0 ( 정상 종료 )
- Linger 를 비활성화, 소켓의 default 값으로 설정
- 소켓 버퍼에 남아있는 모든 데이터를 전송하는 일반적인 소켓의 정상 종료

(2). l_onoff > 0 && l_linger = 0 ( 비정상 종료 )
- Linger 옵션 활성화
- close() 이후, 즉시 소켓 버퍼에 남아있는 모든 데이터를 버리는 비정상 종료
- TCP 연결 상태일 경우, 상대편 Host 에 Reset 을 위한 RST 패킷 전송

- **Linger 활성화**, close() 이후, 즉시 상태가 종료됨
- 소켓 버퍼에 남아있는 **데이터를 버리는 비정상 종료**
- 마약

(3). l_onoff > 0 && l_linger > 0
<br/>

// 비정상 연결 종료에는 어떠한 경우들이 있는지 보기
### Ⅳ. RST
#### 1️⃣ RST 패킷
✅ TCP [RST 패킷]
- RST (Reset)
- TCP 연결을 **즉시 종료** 하고 초기상태로 돌림
- 양방향에서 동시에 일어나는 중단 작업

✅ RST 패킷 상황

(1). 비정상 연결 종료
- SO_LINGER ( l_onoff = 1, l_linger = 0 )
- close() 이후, 소켓 버퍼에 있는 데이터를 즉시 버림, 비정상 연결 종료

(2). 네트워크 오류
- 네트워크 통신 중 ( 오류 ) 발생 시, RST 패킷을 사용하여, 즉시 종료

(3). 리셋 요청
- 연결을 재설정

(4). 연결 거부
- 서버가 클라이언트의 연결 요청을 거부할 때, 전달

#### 2️⃣ FIN & RST
✅ FIN ( Finish )
- 양
✅ RST ( Reset )
<br/>

#### 3️⃣ Pandora
✅ Wireshark 패킷 분석

✅ 코드 흐름
- [CMD_STBDISCONNECT(2010)] → WM_CLIENT_DISCONNECT_REQ
- WM_CLIENT_DISCONNECT 가 아니라 **WM_CLIENT_DISCONNEC_REQ** 를 호출

✅ DISCONNECT / DISCONNECT_REQ

(1). WM_CLIENT_DISCONNECT
- CSocketSession::onStop() ( thread::onStop() ) 시, closeOption = [ TCPSOCKET_CLOSE_GRACEFULLY / TCPSOCKET_CLOSE_FORCE ] 후, WM_CLIENT_DISCONNECT 전달

(2). WM_CLIENT_DISCONNECT_REQ
- clientDisconnectReq_Impl : CMD_STBDISCONNECT + 1
- 

### Ⅳ. 기타
#### 1️⃣ 참고 사이트
✅ 사이트
- [네이글 알고리즘] (https://code-lab1.tistory.com/144)


