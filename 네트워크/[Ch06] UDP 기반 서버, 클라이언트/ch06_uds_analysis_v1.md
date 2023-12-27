### Ⅰ. UDS
#### 1️⃣ UDS 개념
✅ UDS
- Unix Domain Socket
- [프로세스] ↔ [프로세스] 통신을 위한 소켓
- 파일 시스템에 특정한 파일 형태로 존재 ( 파일 디스크립터 )
----
- **Local 내에서만 동작**
- 네트워크를 통한 통신이 아님

#### 2️⃣ IPC
(1). SHM <br/>
(2). UDM ( Unix Domain Socket )

✅ SHM
- [프로세스] ↔ ( 메모리 ) ↔ [프로세스] : 메모리를 직접 공유
- (( 직접 메모리를 공유 ))하기 때문에, 데이터 전송이 빠릅니다
  ⇨ 여러 프로세스에서 ⇾ ( 직접 ) 메모리를 접근할 수 있기 때문에, **(( 동기화 ))** 매커니짐이 필요하다
- (( **메모리 복사 없이** )) 직접 접근하기 떄문에, **Overhead가 적다**

🔯 SHM 사용
- 대량의 데이터를 자주 주고 받아야 할 떄, 메모리 복사가 적은 SHM 적합

✅ Socket
- 여러 차례 (( 메모리 복사 ))를 통해서, 데이터를 전송할 떄, **Overhead가 크다**
- 네트워크 통신이기 때문에, 네트워크 영향을 받는다
- 안전한 프로토콜을 사용해서, 안정성이 높다고 할 수 있다.

🔯 Socket 사용
- 프로세스 간의 독립성을 높이고 싶으면, Socket 이 적합

### Ⅱ. 코드
#### 1️⃣ 코드
- Provider / Consumer : 각 각 CS_TASK_ID 생성
✅ Provider ( Server )

✅ Consumer ( Client )
- consumerTid = CS_Thread_CreateFmt(Test_UDSConsumer)
-----

(1). Test_UDSConsumer
- ipc_consumer_create

(2). CsIPCFactory
- CreateIPCStream()
- 
(3). CsIPCStream
- CSIPC_TYPE_e : SharedMemory / UDS 를 사용할 것인지
  ⇾ 그럼 **socket** 은 어디에 해당하는 것인지 ?
  - SHM ( Shared Memory ) : for window / linux
  - UDS : for linux

#### 2️⃣ cs_ipc.h
✅ 구분

(1). IPC_TYPE
- SHM ( window, linux )
- UDS ( linux )

(2). IPC_MODE
- PROVIDER : Client ( 데이터를 제공하는 주체 )
- CONSUMER : Server ( 데이터를 소비하는 주체 )

✅ 클래스 구성

- (1). CsIPCStream
  - (1)-1. CsSHMBase
    - CS_SHMMEMORY_ID
    - ① CsSHMConsumer : Server ( Data Read )
    - ② CsSHMProvider
  - (1)-2. CsUDSBase
    - CS_UDS_ID
    - ① CsUDSConsumer
    - ② CsUDSProvider

#### 3️⃣ cs_ipc_uds.cpp
✅ 구분
- UDS_MODE
  - SERVER
  - CLIENT
  - ACCEPTEDCLIENT


### Ⅲ. QOS ( Quality of Service )
#### 1️⃣ 네트워크 QOS
✅ QOS ( 서비스 품질 )
- (( 한정된 네트워크 용량 )) 으로  

✅ 트래픽

✅ DSCP

✅ DHCP




