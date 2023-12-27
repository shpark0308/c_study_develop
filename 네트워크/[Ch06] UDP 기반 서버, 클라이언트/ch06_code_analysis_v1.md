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
✅ 서버 구성 ( 소켓 서버를 생성하기 전 (( 준비 )) 단계 )
- [(1), 각 SocketServer 가 생성될 때] → [(2). CASyncSocketServer.**run()**] → [(3). cs_tcpsocket.cpp ]
----
(1). 각 SocketServer 가 생성될 때 <br/>
- 어떤 Parser 를 사용할 것인지 m_Pasrer 를 지정
- 해당 SocketServer->**StartServer**

(2). CASyncSocketServer <br/>
- SocketServer.cpp
- ThinThread 를 상속 받음
- StartServer
  - ① (( TcpSocketOption )) 을 지정해줌 // 옵션을 전부 지정해주면
  - ② CS_TcpSocketServer_Create // TcpSocketServer 를 생성
  - ③ THINTHREAD_CREATE::run()
    - CS_TcpSocket_WaitEvents() // event 가 올 때까지 기다림
      - #define TCPSOCKET_DEFAULT_WAITTIME_MS     (100) // 100ms
      - WaitEvents 로 socket 과 관련한 event 는 100ms 안에 들어와야 한다고 봄
        
        ----
      - CS_SOCKET_WAIT_TIMEOUT 하지만, timeout 이 발생하여도, continue 해서 문제가 없다.
      - 이 구간은, TCPSOCKET_EVENT_ACCEPT 만 수용 [ Event : ACCEPT / READ / WRITE / CLOSE 등
        
(3). CASyncSocketServer::run() - accept
- accept() 하고, 그럼 client session 을 등록하는 곳이 어디있는지
  - 실제로 Client Session 을 받아오는 곳 
  - nClientSock == Invalid 설정
  - 

✅ 1. 소켓 생성 ( **CS_TcpSocketServer_Create** )

(1). CS_SockEvent_Create
- I/O MultiPlexing 으로 들어오는 여러 Socket을 관리하기 위한 SockEvent_Create() 를 생성
  - 이렇게 만든 event 는 **CS_TcpSocket_GetEvent** 일때, event 를 받는데 그럼 이 event 를 (( 언제 )) 받는 것인지 <br/>
    → CSocketSession::run 에서 실행

(2). __tcpsocket_createServer
- socket(PF_INET, SOCK_STREAM, 0) : socket server 생성

(3). __tcpsocket_GarbageCollector_Run(MAX_GC_TIMEOUT); // 60) ( 60sec ) ----->>>>>>>>>>>>> 여기로 다시 돌아오기

(4). CS_TcpSocket_AddEvent( 서버 소켓 )

⇒ 여기까지 [서버 소켓] 생성

✅ 2. Bind 3 listen ( __tcpsocket_createServer //cs_tcpsocket.cpp )

✅ 4. accept ( cs_tcpsocket.cpp )
- 대다수 웬만한 것들은, cs_tcpsocket.cpp 에서 해결
- CS_TcpSocketServer_DoAccept
- CS_TcpSocketServer_CreateClient
  - ① 동일하게, CS_SockEvent_Create // Event 설정
  - ② __tcpsocket_GarbageCollector_Run
    - 주기적으로, 실행 ( while(!gcQueue.isEmpty()) )
  - ③ CS_TcpSocket_AddEvent(client 소켓 fd) 추가
    - AddEvent 를 하면 epoll 에 추가되는것인데, 해당 epoll 은 한개밖에 없는 것인지, 아니면 epoll 을 어떻게 구분하는 것인지
    - type : TCPSOCKET_EVENTTYPE_ACCEPTED_CLIENT 에 따라서, event 생성이 달라진다. 
  - ③ // CreateClient // 863 으로 돌아오기
  - CSocketSession 생성 ⇨ SocketServer::startSocketSession() ⇨ **SockMgr**.InsertSocketSession 에 추가
    - SocketServer() : **Socket Server**
    - SocketSession() : **Socket Client**
- SocketSession::StartSocketSession()
  - SocketSession::run()
    - 해당 Client 세션으로부터 event() 발생 시, Parsing 작업 실행


#### 4️⃣ TCP 클라이언트
✅ SocketSession::run() // Data

(1). StartSocketSession
- run() ⇾ while() 문

(2). run()
- ① SocketSession 에서는 Event 만 발생한 것을 확인하고
- ② Event 를 발생한 것을 m_parser->Parsing() 으로 넘겨줘서 ⇨ 데이터를 읽고 오게 함
-------
- TCPSOCKET_EVENT_READ == tcpEvent
  - m_sessionState = CONNECT; // CONNECT
  - resetWaitTimeoutCount()
  - doParsing() ///// SocketSessiopn.cpp // 221
<br/>

### Ⅱ. 세션 관리
#### 1️⃣ SocketSessionMgr
- SocketSession : Client 마다 세션 맺는 것
✅ InsertSocketSession
```cpp
CriticalSescion_Lock()
socketMap_.insert 에 추가
CriticalSescion_UnLock()
```
- socketMap 은 유일무일하고 공용 변수

### Ⅲ. 파싱
#### 1️⃣ CClientPacketParser
- PacketParser ⇾ cs_socketutil.cpp [RECV/SEND] ⇾ cs_tcpsocket.cpp
✅ CS_TcpSocket_Recv
- RecvInt : CMD 를 보고 Switch 에 따라 다른 작업을 함
(1). ctrlSock_**QOS**Enable
- 


///// Audio Video 데이터 어떻게 전달하는지 보기
///// UDP 로 통신하는 부분은 없는지
////  WireShark 로 Ack / Fin 부분 한번 보기

### Ⅳ. 파일
#### 1️⃣ TCP
✅ cs_tcpsocket.cpp

#### 2️⃣ Socket
✅ cs_sockevent.cpp

(1) int CS_SockEvent_Wait(cs_sockevent_t* pSockEvent, int **nWaitTimeoutMs**) <br/>
- nWaitTimeoutMs : event 를 위한 **Timeout**
  - TCPSOCKET_DEFAULT_WAITTIME_MS (100) // 100mx=s
  - TCPSOCKET_DEFAULT_WAITTIME_MS (100) x MAX_TIMEOUT_COUNT ( 60 ) : 6000 ms = **6초**
- cs_sockevent_t
  - CS_SockEvent_Kind_e_ : [ CS_SOCKEVENT_KIND_SELECT | CS_SOCKEVENT_KIND_EPOLL ]
    ⇒ 해당 (( Event )) 를 위한 Timeout
- [ 어떻게 구현되어있는지? ]
  - Lock() ⇾ (( CS_SockEvent_Select_Wait / CS_SockEvent_Epoll_Wait )) 을 동기화 하기 위함
    - CS_SockEvent_Epoll_Wait : 내부적으로 어떻게 구현되어 있는가
  - UnLock()
 
### Ⅴ. 기타
#### 1️⃣ char
✅ Strncpy
```cpp
char name[CS_SOCK_NAME_LEN];
CS_StrNCpy(pSockEvent->name, name, CS_SOCK_NAME_LEN);
```
- char 배열 값을 
<br/>

#### 2️⃣ thread
✅ cs_thinthread.cpp

✅ cs_thread.cpp

(1). CS_Thread_CreateEx
- ① thread 셋팅
  - pthread_attr
  - cs_task_arg ∈ cs_task
- ② pthread_create 생성
  - (( CThinThread::start )) 함수
  - CThinThread::main() // pMT->main()
    - run() : **ThinThread 를 상속받은 클래스의 run** 을 하게 되는 것
   
#### 3️⃣ msgQ
