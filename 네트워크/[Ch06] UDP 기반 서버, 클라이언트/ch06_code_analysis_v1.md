### Ⅰ. TCP
#### 1️⃣ TcpSockInfo
(1). TcpSocketOption <br/>
(2). TcpSocketStatistic <br/>
(3). TcpSocketEvtType <br/>

✅ TcpSocketOption
- [데이터]
  - sendbuffsize / recvbuffsize
  - **MTU Size** ( 이게 Pandora 에서 어디에 사용되는지 )
  - **sendTimeout / recvTimeout** ( 몇으로 설정해놓았는지 )
  - keepAlive ( keepAlive 는 어떻게 사용하고 있는지 )
-------
- SocketServer.cpp 에서 CASyncSocketServer::StartServer 할 때, 서버의 socket 을 생성할 때, Option 을 지정
  - opt.MTUSize = 1500;
  - opt.nRecvBufferSize = nRecvBuffSize; //
    - #define CSS_SOCKET_DEFAULT_BUFFER_SIZE		<b>(16*1024)</b> = 2^

✅ TcpSocketStatistic
- [데이터]
  - sendRetry / recvRetry
  - sendTotalByte / recvTotalByte
  - ADV_t ( 이것을 실제로 사용하는 곳이 있는지 / 사용한다면 어떻게 사용하고 있는지 확인 )
    - nLost
    - nRTT
    - pacingRate
    - nCwnd
    - 등등

✅ TcpSocketEvtType
- [EvtType]
  - TCPSOCKET_EVENTTYPE_SERVER | Client | Accepted_Client ..
- [Event]
  - ERROR | ACCEPT | ...
- [SocketCloseOption]
  - GRACEFULLY | FORCE ( 종료 시나리오에 따라서 달라지는 것이 있는가 )
<br/>

#### 2️⃣ TcpSockFunction
✅ int CS_TcpSocket_WaitEvents(CS_TCP_ID **hTCP**, int **nTimeoutMs**)
- hTCP
  - **CS_TCP_ID** 로 (( DxTcpSocket* <b>pSockData</b> = (DxTcpSocket*)hTCP; )) 를 하여 TcpSocket 에 접근
  - DxTcpSocket
    - [ eTcpSocketEvtType | TcpSocketOption | bGCDelete | ipaddr | port | statistics ] 등 등이 있음
- 함수
  - 확인 작업
    - (1). NULL == pSockData
    - (2). CS_POOL_Get(s_pool_msg, hTCP ) 가 존재하는지 확인
    - (3). pSockData->isDeleted or bStop 되었는지 확인
   
#### 3️⃣ TCP 서버
✅ 1. 소켓 생성
(1). CS_SockEvent_Create
- I/O MultiPlexing 으로 들어오는 여러 Socket을 관리하기 위한 SockEvent_Create() 를 생성
  - 이것을 언제 사용하는지
  - 
(2). __tcpsocket_createServer
- socket(PF_INET, SOCK_STREAM, 0) : socket 생성
- 

### Ⅱ. 파일
#### 1️⃣ TCP
✅ cs_tcpsocket.cpp

#### 2️⃣ Socket
✅ cs_sockevent.cpp

(1) int CS_SockEvent_Wait(cs_sockevent_t* pSockEvent, int **nWaitTimeoutMs**) <br/>
- nWaitTimeoutMs : event 를 위한 **Timeout**
  - TCPSOCKET_DEFAULT_WAITTIME_MS (100) // 100mx=s
- cs_sockevent_t
  - CS_SockEvent_Kind_e_ : [ CS_SOCKEVENT_KIND_SELECT | CS_SOCKEVENT_KIND_EPOLL ]
    ⇒ 해당 (( Event )) 를 위한 Timeout
- [ 어떻게 구현되어있는지? ]
  - Lock() ⇾ (( CS_SockEvent_Select_Wait / CS_SockEvent_Epoll_Wait )) 을 동기화 하기 위함
    - CS_SockEvent_Epoll_Wait : 내부적으로 어떻게 구현되어 있는가
  - UnLock()
 
### Ⅲ. 기타
#### 1️⃣ char
✅ Strncpy
```cpp
char name[CS_SOCK_NAME_LEN];
CS_StrNCpy(pSockEvent->name, name, CS_SOCK_NAME_LEN);
```
