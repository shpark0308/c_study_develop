### Ⅰ. TCP 연결
#### 0️⃣ 배경 지식
✅ TCP
- [ 연결 지향성 | 신뢰성 | 순서 보장 | 독립된 별도의 전송 라인 ]

✅ 순서 보장

🔯 순서 보장 ❌ ( 상황 )

(1). 흐름 문제 ( flow )

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/1a14683a-6f99-4451-9f7d-8cfafc4b0375)
- 송수신 간의 데이터 처리 속도 **차이**로 발생
- 수신자(서버) 가 처리할 수 있는 용량이 초과 되어 데이터가 수신이 안되고 (( **누락** ))
- 서버가 감당할 수 있는 데이처 처리 양보다, 네트워크 속도가 더 **빨라서** 발생하는 문제

(2). 흐름 제어 ( Congestion )
- 네트워크가 너무 복잡해서, 제시간에 데이터가 도착하지 않아서 발생하는 문제

🔯 해결 방법
- [발신자] ← ( ACK ) ← [수신자]
- 수신자가 ACK 신호를 보내에 데이터를 정상적으로 전달 받았다는 신호를 보냄

⇨ 그러기 위해, 사전에, **정확한 전송** 을 위해, 두 네트워크 사이에 세션을 수립하는 과정이 필요 ( **3-Way HandShake** )
<br/>

#### 1️⃣ 송신 / 수신
✅ 연결 ( 3-Way HandShake )
- 정확한 전송을 보장하기 위해서, 상대방 컴퓨터와 사전에 세션을 수립하는 과정
- 연결 요청 : **[Client] → [서버]**

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/72e2bc66-09e3-4bb1-ba36-475ea09f7d75)

✅ 데이터 전송

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/59bb1397-a06c-454f-8d50-c2078aa30567)

(1). [발신자] : Packet 전송 <br/>
(2). [수신자] : ACK 전송 ( 잘 전달 받았다는 SIGNAL 전달 ) <br/>
(3). [발신자] : 일정 시간 이내에 ACK 가 안오면, **재전송** <br/>

✅ 연결 해제 ( 4-Way HandShake )
- 연결 해제
  - (1). **[Client] → ( 연결 해제 요청 ) → [서버]**
  - (2). **[Client] ← ( 연결 해제 ) ← [서버]**
 
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/0709f871-77d4-407f-8677-5a483db9d8f3)
<br/>

#### 2️⃣ TCP 문제
✅ 문제점
- <b>데이터가 1개라도 손실 되면, 재전송 해야하기 때문에, 오래 걸린다.</b>
<br/>

#### 3️⃣ TCP Header
✅ Header
- TCP : segmentation
  - Application 계층에서 [ STREAM ] 으로 전송되는 것을 [ Segmentation ] 으로 쪼갬
  - MSS ( Max segmentation size )

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/08740764-203a-4daa-ba6e-4bc2dc8a91af)

(1). Segmentation : <b>[TCP Header]</b><b>[Data]</b> <br/>
(2). TCP Header : [Source Port] + [Destination Port] <br/>
(3). Sequence Number : 순서 보장 <br/>
&nbsp; &nbsp; &nbsp; Acknowledge Number : 수신자가 받을 다음 byte 정보 <br/>
&nbsp; &nbsp; &nbsp; Flag bit : TCP 연결 제어 및 데이터 관리
<br/>

### Ⅱ. UDP
#### 0️⃣ 배경 지식
✅ UDP
- [ 비연결 지향성 | 비신뢰성 | 순서 보장 X | 독립된 각각의 패킷 ]
- [ 흐름 제어 X, 혼잡 제어 X ] ( **순서 보장 X** )
- 빠른 전송을 보장
- 독립된 각각의 패킷
  - MultiCast
  - BroadCast

#### 1️⃣ 송신 / 수신
✅ 연결

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/87bc3f57-9c88-43c9-acbf-c7863838cacc)
- <b>Connection 과정이 존재하지 않는다</b>

✅ 오류
- ACK 신호가 없다
- **checksum** 을 통해서, 오류를 잡는다.

🔯 checksum
- UDP 는 비연결 지향성, 비신뢰성 데이터 전송
- 하지만, 데이터 손상 및 오류에 대한 복구 수단 ( <b>checksum</b> )
- checksum field : 16bit ( 2byte )
- [송신자] : 데이터 전송하기 전, checksum 계산
  - checksum : [ UDP Header ] + [ PayLoad ]
- [수신자] : 데이터의 checksum을 계산 및 송신 측에서 전송한 checksum값과 비교하여 오류 감지
  - 데이터 무결성 검증
⇨ UDP 는 데이터 손실에 대한 감지 기능만 있지, 이를 위한 **복구 기능은 없다**
⇨ 따라서, TCP 와 다르게, 해당 패킷을 재전송 과정이 없다.
<br/>

#### 2️⃣ UDP 문제
✅ 문제점
- 빠른 전송을 위해, 비신뢰성
  - 패킷 loss 또는 손상에 대한 복구 기능 (ex. 재전송) 이 없다.
  - 연결 / 종료 과정이 별도로 없다.
<br/>

#### 3️⃣ UDP Header
✅ Header
- UDP : 독립된 각각의 패킷

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/4cceea8b-511e-4e4a-85df-4c39ce8e8309)

(1). <b>[UDP Header]</b><b>[Data]</b>
(2). UDP Header : [Source Port] + [Desination Port]
(3). [Length] + [**Checksum**]


<br/>

### Ⅲ. RTP ( Real-Time Protocol )
#### 1️⃣ RTP
✅ RTP
- **RTP** : **[UDP] 패킷 + ( 데이터 )** ( UDP 패킷에 실려서 전송 )
- 실시간 데이터 ( 음성, 비디오 등 )에 사용되는 네트워크 프로토콜
- Multimedia 통신 및 실시간 Streaming 어플리케이션에서 사용

✅ RTP VS UDP 차이

[ 공통점 ] <br/>
(1). RTP 와 UDP 모두, checksum 으로 오류를 감지하지만, 둘 다 **오류 복구 기능이 없다**. <br/>
(2). RTP 는 UDP 의 통신 Port 를 사용한다. <br/>
(3). 둘 다, Mulicast 를 지원 <br/>

[ 차이점 ] <br/>
(1). RTP 는 [시간 정보(Timing)] 를 제공하여 **순서를 보장** ( 실시간 데이터에 중요 역할 ) → UDP 는 순서 보장 X <br/>

✅ Pandora 적용
- 오류 복구에 대한 시나리오
- 실시간 Timing 을 맞추기 위한 노력
- RTP 로 변경하였을 때, TCP 보다 더 (( 성능 )) 이 좋다고 말할 수 있는가?
<br/>

#### 2️⃣ 데이터 송신
✅ 배경
- [송신자] (( 일정하게 데이터를 송신 )) 하지만, 네트워크는 복잡하기 떄문에, **가변적**으로 데이터가 도착할 수 있다.
  - 네트워크 영향 요소 : 대역폭 | 네트워크 구조 | 라우팅 방식 | 전송 프로토콜 종류
- 실시간 데이터 : 수신 프로세스에 도착하는 전송 간격은 **일정** 해야 한다. <br/>
⇨ Buffer ( 버퍼 ) 추가 <br/>

✅ 지연 버퍼
- 가변적인 간격으로 도착하는 데이터를 즉시 서버에게 보내지 않음
- 지연 버퍼를 통해 다시 (( **일정한 간격** )) 으로 보정

✅ 통신 과정

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/d5a57140-78c1-4258-b7de-0e4c3e7798a3)
- [Audio/Video] → (( Codec )) 으로 압축 → RTP Header ( Timestamp 추가 ) → UDP 패킷 → IP 전송

<br/>

#### 3️⃣ RTP Header
✅ Header

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/1837cf99-3053-4673-a047-b4daaf07667c)
- PayLoad (7byte)

#### 4️⃣ RTP, RTCP, RTMP
✅ RTP <b>( UDP )</b>
- Real Time Transport Protocol
- [UDP] + ( 데이터 ) 전송
- ( 시간 정보 ) 전달 (( 순서 보장 ))
- <b>동영상 / 음성 스트리밍에서 사용</b>

✅ RTCP <b>( UDP )</b>
- RTP Control Protocol
- RTP **세션의 제어 / 감시** 를 담당하는 프로토콜
- [RTP] + [RTCP] : RTP 와 함께 사용되어, (( 주기적으로 )) 네트워크 상태, 세션 정보 등의 정보를 수집하고 제공

✅ RTMP <b>( TCP )</b>
- Real-Time Message Protocol
- TCP 기반의 실시간 스트리밍 프롴토콜
- Flash Player 기반으로 사용
  - 현재는 Flash Player 사용이 줄어들어서, HTTP 기반의 HLS을 사용
    - HLS ( HyperText Transport Protocol Live Streaming )
  - HLS 는 HTML5의 일부로 모든 현대 브라우저에서 지원됨
- <b>(( 웹 )) 에서 동영상 스트리밍에서 사용</b>

🔯 HLS
- HyperText Transport Protocol Live Streaming
- HTTP 기반의 실시간 스트리밍 프로토콜
- HTTP 이 TCP 의 기반이기 때문에, 기본적으로 세그먼트 재전송을 통해 (( **신뢰성** )) 을 보장한다.
- TCP 를 기반으로 하기 때문에, 동영상을 세그먼트로 나누어 전송 
- Flash Player 사용이 줄어든 대다수의 브라우저에서 HTML5 의 일부인 HLS 는 현대 모든 브라우저에 지원된다. 
<br/>

### Ⅳ. WebRTC
#### 1️⃣ WebRTC
- Web Real Time Communicatioin
- 많은 회사에서 WebRTC 를 사용

✅ WebRTC
- 웹 어플리케이션 및 사이트들이 (( 별도의 SW 없이 )), ( 음성 / 영상 미디어 ) 데이터를 Browser 끼리 주고 받을 수 있도록 만든 기술
- [WebRTC 프로그램] ↔ ( ( 음성 / 비디오 ) 미디어 데이터) ↔ [WebRTC 프로그램] : 별도의 SW 및 플러그인 없이 P2P 화상 회의 및 데이터 공유
  - P2P ( Peer to Peer )
- 웹 브라우저 상에서, (( 별도의 SW / 플러그 인 )) 없이, 화상 채팅, 음성 채팅 및 데이터 교환이 가능
- (( RTP )) 기술을 바탕으로 사용
<br/>

### Ⅴ. 기타
#### 1️⃣ 판도라 이해
✅ TCP

- TCP Segmentation 의 MSS 가 일반적으로 65535 (2^16 - 1) 이여서 데이터 값이 이 이상일 경우, <br/> &nbsp;&nbsp;&nbsp;&nbsp;
데이터를 계속 이어서 받도록 한다. <br/>

(1). Application 계층에서는 [Stream] 으로 데이터를 전송하기 때문에, 얼마만큼의 데이터를 Stream 에서 <br/> &nbsp;&nbsp;&nbsp;&nbsp;
세그먼트로 쪼개어 보내는지 **데이터 크기** 를 패킷에 함께 전달해야한다. <br/>
- TCP 는 Connection 맺는 경우,  또는 데이터 손실에 대한 재전송의 경우가 있기 때문에, **느리다**.<br/>
- 반면, UDP는 Connection 과정이 별도로 있지 않아서, **빠르다**.

✅ checksum

(2). checksum <br/>
  - 송신 쪽에서 보내는 데이터가 제대로 된 값인지 **검증** 하기 위한 checksum 이 필요
  - 우리 연동규격서에도 반영
<br/>

✅ RTP VS HLS

⚠ 질문
- 둘 다 실시간 스트리밍 / 멀티 미디어 데이터 전송에 사용
  - RTP (UDP) : 주로 음성 / 동영상 Streaming , 실시간 통신
  - HLS : 웹에서 동영상 Streaming / 비디오 VOD 서비스
- webapp 브라우저 를 Streaming 또는 VOD 서비스를 재생하는 판도라에서는 RTP 보다는 HLS 가 적합하지 않은가?
- 왜 그때, WebRTC 를 사용하지 않고, HLS 를 사용한건가?
  - WebRTC 는 ( RTP ( UDP ))기반이고
  - HLS ( HTTP ( TCP )) 기반이였어서 그런것인지
<br/>

#### 2️⃣ 코드
✅ uint8_t
``` cpp
struct Header
{
  uint8_t version : 2;
};
```
- uint8_t : 8bit 의 부호 없는 정수 의미
- **:2** : 8bit 의 정수를 2bit 로 제한하여 ( 00, 01, 10, 11 ) ( 0,1,2,3 ) 으로 표현
<br/>

#### 3️⃣ Byte Order
✅ 표현 방식 (2가지)
- 시스템 CPU 에 따라서 바이트 오더가 다르다

|Little Endian|Big Endian|
|:-----------:|:--------:|
|Intel x86|IBM|
|AMD|ARM|

(1) Little Endian
- [높은 주소] → [낮은 주소]
- ex. 0x78 0x56 0x34 0x12

(2) Big Endian
- [낮은 주소] → [높은 주소]
- ex. 0x12 0x34 0x56 0x78

✅ 네트워크 & 호스트
- [Host] : CPU (Intel) ⇒ Little Endian
- [네트워크] ⇒ **Big Endian**
----
- h : host byte order
- n : network byte order
- l : long (32bit)
- s : short (16bit)
----
```cpp
short port = 0x1234;   // short (16bit)
int   ip = 0x12345678; // int (32bit)

printf("[port] %x:%x [ip] %x:%x", port, htons(port), ip, htonl(ip));
// [port] 1234:3412 [ip] 12345678:78563412
```
- htonl : host → network, long (32bit) (ip주소)
- htons : host → network, short (16bit) (port)

✅ 메모리 단위
``` cpp
uint8_t num = 1;
printf("size : (%d)", sizeof(num)); // 1
```
- 8bit = 1byte 이여서 크기가 1로 출력됨
<br/>

#### 4️⃣ 참고 사이트
✅ 사이트
- [TCP/UDP HandShake] (https://velog.io/@rlacksals96/%EB%84%A4%ED%8A%B8%EC%9B%8C%ED%81%AC-TCPUDP%EC%99%80-3way-handshake#connection-%ED%95%B4%EC%A0%9C4way-handshake)
- [WebRTC 이해] (https://medium.com/@hyun.sang/webrtc-webrtc%EB%9E%80-43df68cbe511)
