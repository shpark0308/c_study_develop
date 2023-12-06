![image](https://github.com/shpark0308/c_study_develop/assets/60208434/79be927b-814c-4390-abd3-f6521816aa50)### Ⅰ. I/O Mulitiplexing
#### 0️⃣ Multiplexing
✅ Multiplexing<br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/216a6800-c917-4713-8064-bd069f937405)

(1). Multiplexing <br/>
- (( 여러 개의 신호 )) ⇨ (( 하나의 전송 매체 / 통신 채널 )) 을 통해 **전송**하는 기술

(2). DeMultiplexing <br/>
- (( 하나의 신호 )) ⇨ (( 여러개의 신호 )) 로 **분리**하는 기술

✅ 종류

(1). 통신 Multiplexing <br/>
- Time Division Multiplexing ( TDM ) : [시간] 을 여러 개의 슬롯으로 나누고, 각 슬롯에 다른 데이터를 할당하여 전송
- Frequency Division Multiplexing ( FDM ) : [주파수]를 여러 개의 밴드로 나누고, 각 밴드에 다른 데이터를 할당하여 전송
- Code Division Multiplexing ( CDM ) : 각 사용자에게 고유한 [코드]를 부여하고, 이 코드를 사용하여 데이터를 전송

(2). 컴퓨터 Multiplexing <br/>
- I/O Multiplexing : 여러개의 입출력 작업을 (( 동시에 )) **관리**하고 / **처리** 하는 기술
- ( select / poll / epoll ) 과 같은 시스템 콜이 사용됨

(3). 전자기기 및 신호처리 Multiplexing <br/>
- Analog Multiplexing
- Digital Multiplexing

✅ TDM

▪ TDM ( Time Division Multiplexing ) <br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/dce4f7c8-c30a-45e4-8a95-38128851955a)
- 각 [Time] 슬롯이 돌아가면서, 데이터 전송
- 분리
  - (1). 동기화 TDM ( Synchronize )
  - (2). 비동기화 TDM ( Asynchronize )
<br/>

(1). 동기화 TDM <br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/77da61f1-3471-4776-a1ce-894cfa735524)

- 각 Digital Source 의 T ( 한번에 전송하는 데이터 bit dutration ) 과 출력 frame Duration ( T ) 가 같아야 한다
- 입력 채널의 [전송 데이터]가 없을 경우, empty slock 이 할당된다.
- 위와 같은 상황에서 synchronization 문제가 발생할 수 있기 때문에, 이를 위한 bit 를 프레임에 추가한다. <br/>

(2). 비동기화 TDM ( ATDM ) <br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/ca3be98f-1387-4d0e-aa21-1354e62f382e)
- 기본 TDM 과 비교할 때, 채널 사용 효율성이 높음<br/>

✅ TDM 문제

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/5fa05357-775a-455a-9cca-777c446715a8)

```
다음 그림은 동기 TDM ( Time-Division Multiplexing) 방식의 전송 과정을 나타낸 것이다.
동기 TDM 에서 디지털 데이터를 32개의 입력 링크에서 1개의 출력 링크로 <조건> 에 따라 다중화를 수행한다고 가정한다.
<작성 방법> 에 따라 서술하시오 (단, 그림은 참조만 할 것)
```
```
[조건]
* 각 입력링크는 매초 500개의 워드 (16bit)를 전송한다.
* 끼워넣기(interleaving)는 하나의 워드 단위로 수행된다. // 동기화 TDM 에서 빈 슬롯이 있을 경우, 이를 interleaving 으로 하나의 워드 단위만큼 끼워넣는다.
* 동기 비트는 '00','01,'10','11' 의 순서로 변한다. // 동기화 문제 때문에 추가로 동기 비트를 삽입한다 ( 2bit )
```
```
[작성 방법]
(1) 입력 링크 별 데이터 전송률을 구하여 쓸 것. (단, 단위는 kbps로 쓴다)
(2) 개별 출력 프레임의 크기를 구하여 쓸 것. (단, 단위는 bit 로 쓴다)
(3) 출력 프레임의 기간을 구하여 쓸 것. (단, 단위는 ms 로 쓴다)
(4) 출력 링크의 전송률을 구하여 쓸 것. (단, 단위는 kbps로 쓴다)
```
(1). 하나의 입력 링크에서 500 ( 워드 갯수 ) * 16 bit = 8000bit/sec = **8kbps** <br/>
(2). [ #32th ] [ #31th ] [ ... ] [ #1st(16bit) ][ 동기 비트 ] = 16 * 32 + 2bit = **514bit** <br/>
(3). (1초)[ 500th ][ 499th] [ 1th ] ( 1초에 각 입력링크에서 500개를 보내니까 1초에 500프레임 발생 ) <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 1프레임 = 1/500 sec = 0.002 sec = **2ms** <br/>
(4). 1프레임 당 : 514bit / 2ms 에 전달, 514bit * 500 = 257,000bit/sec = **257kbps**<br/>

<br/>

#### 1️⃣ I/O Multiplexing ( 네트워크 Multiplexing )
✅ I/O Multiplexing
- 여러 개의 I/O 작업을 (( 동시에 )) **관리**하고 / **감시** 하는 기술
- 여러 (( 소켓 )) 이나 (( 파일 디스크립터 ))의 상태를 감시하면서, 입출력 작업이 발생하는 것을 효과적으로 처리
- ( select / poll / epoll ) 과 같은 시스템 콜이 이러한 목적으로 사용 <br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/b869c86a-8e8f-4024-8aab-4d5cbc724d2f)

(1). select : 가장 오래된 I/O Multiplexing 매커니즘 중 하나로, 여러 개의 소켓을 감시하고, 그 중 어떤 소켓에서 입출력 가능한지 선택 <br/>
(2). poll : seleoct의 대안으로 간단하고 직관적이다, 파일 디스크립터의 상태를 감시하고 이벤트 발생 시, 알려줌 <br/>
(3). epoll : 리눅스에서 사용되는 I/O Multiplexing 매커니즘 중 하나로, select 와 poll 의 단점을 극복하고 더 효율적으로 동작 <br/>

✅ select() (**동기식**)

✅ poll()


✅ epoll()

#### 2️⃣ select()
✅ select ( **동기식** )
- 가장 오래된 I/O 매커니즘, 여러 개의 (( 소켓 )) 을 감시, 어떤 소켓에서 입출력이 가능한지 선택
- [원리]
  - 파일 디스크립터 상태 감시
  - 해당 상태가 변경 시, (( **블로킹** )) // → 블로킹이 되었다는 것 자체가, 동기화
  - 이후, 변한 상태를 확인하고 작업 수행
- 다중 클라이언트를 동시에 관리
- 비동기적인 입출력 작업을 수행하는데 사용
``` cpp
#include <sys/select.h>

int select(int nfds, fd_set* <b>read_fds</b>, fd_set* <b>write_fds</b>,fd_set* <b>except_fds</b>, struct timeval* timeout);

// 예제
int fd_num = select(**fd_max+1**, &reads, 0, 0, &timeout);
```
- fd_max + 1 : 가장 큰 파일 디스크립터 값 + 1 ( 0 <=  X  <=  가장 큰 파일 디스크립터 값 ) 를 참조하기 위해

✅ 관련 코드
```cpp
#include <sys/types.h>

fd_set read_fds; // fd_set = 파일 디스크립터의 ((집합)) 으로, 파일 디스크립터의 상태를 확인하기 위함

FD_ZERO(&read_fds); // 초기화
FD_SET(STDIN_FILENO, &read_fds); // 표준 입출력 예제로 사용
FD_CLR(STDIN_FILENO, &read_fds); // read_fds 에서 해당 파일 디스크립터 제거

if (FD_ISSET(STDIN_FILENO, &read_fds)) {
  // 파일 디스크립터 상태 확인
}
```

✅ poll()

#### 4️⃣ 기타
✅ struct timeval
``` cpp
#include <sys/time.h>

struct timeval
{
  time_t tv_sec; // 초 ( seconds )
  suseconds_t tv_usec;  // 마이크로초 ( Microseconds )
}

struct timeval timeout;
timeout.tv_sec = 5; // timeout 5초 설정
timeout.tv_usec = 0;
```
- [설명]
  - 네트워크에서 **타임아웃** 할 때, 사용
  - suseconds_t : 1 마이크로초 ( 1/1,000,000 sec ) 단위
  - select(), poll() 에서 얼마나 대기 할지, 또는 시간 차이를 계산 할 때, 사용
- [자료형]
  - time_t ( 4byte, 8byte ( = int 형 ))
  - suseconds_t ( 정수형 ( = int 형 ))
✅ epoll()


✅ 참고 사이트
- [TDM] (https://neuro.tistory.com/59)
