### Ⅰ. 이벤트 처리
- 외부 이벤트 처리 방식 (2가지)
  - (1). 폴링 ( Polling )
  - (2). 인터럽트

#### 1️⃣ 폴링
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/a4391a69-9383-420c-a992-4184e0e34f38)

✅ 폴링
```cpp
bool isReady = false;

void* threadFunc1()
{
  while(!isReady) // 반복문으로 대기, CPU 자원 소모 ( Busy-Waiting )
  {
    printf("thread1 is waiting...");
  }
}

void* threadFunc2()
{
  sleep(10);
  isReady = true;
}

pthread_create(&thread1, nullptr, threadFunc1, nullptr);
pthread_create(&thread2, nullptr, threadFunc2, nullptr); 
```
- isReady = true 가 되기 전까지, thread1 은 계속 while 문을 돌게 됨
---- 
- (( 주기적으로 )) 특정 조건이나 상태를 검사하여, 이벤트가 발생했는지 확인
- 주로 (( **반복문** )) 을 사용하여, 상태를 확인
- [장점]
  - 구현이 간단하다
- [단점]
  - CPU 자원을 지속적으로 소모 ( 무한 루프 ) <br/>
    ⇒ Busy Waiting 을 유발
  - 실시간성이 중요한 시스템에서 적합하지 않음
  - 이벤트가 발생하지 않은 경우에도, 계속적으로 / 주기적으로 상태를 확인해야함
 
✅ 인터럽트 ( Interrupt )
- 외부에서 시스템에게 (( **신호** )) 를 보내어, 현재 수행중인 작업을 중단하고 특정한 처리를 수행하도록 함
- 주로 [하드웨어 장치] 가 → [시스템] 에게 특정 이벤트를 알릴 때, 사용
- (( **callback 함수** ))로 구현
----
- [장점]
  - 실시간성이 뛰어남
  - 특정 이벤트가 발생 시에만, 작업을 수행함으로, CPU 자원을 효율적으로 활용
- [단점]
  - 구현이 복잡함
- [구현]
  - 커널의 **Interrupt Handler** : 핸들러가 필요
    - Interrupt 를 체크
  - **Interrupt Thread** 드라이버로 인터럽트 발생을 알림
  - 인터럽트 발생 이벤트가 들어올 시, 특정한 처리 수행


### Ⅱ. 메모리 복사
#### 1️⃣ 문자열 복사
✅ strcpy
```cpp
#define BUF_SIZE (5)

char msg[BUF_SIZE];

strcpy( msg, "hello");
strncpy( msg, "hello", BUF_SIZE );
```

✅ snprintf()
```cpp
snprintf( msg, BUF_SIZE, "hello my name is shin hyeong");
```
- strcpy, snprintf 는 저장 변수인 msg 의 메모리 공간 크기를 염두하지 않고 처리한다.
- 따라서, **-Wstringop-overflow=** 가 발생할 수 있다.

✅ memcpy
``` cpp
memcpy(msg, "hello", BUF_SIZE);
msg[BUF_SIZE-1] = 0;
```
- 이렇게 해야지, 정확하게 메모리 크기에 맞게 데이터를 저장할 수 있다.

#### 2️⃣ 메모리 참조
✅ auto &iter
```cpp
std::vector<int> vec = {1,2,3,4};
for(auto& iter : vec)
{
  iter = iter + 1; // { 2,3,4,5 }
}
```
- auto &iter : 참조 값을 가지고 오게 되어, 실제 vector 안의 값이 변경됨

✅ auto iter
```cpp
std::vector<int> vec = {1,2,3,4};
for(auto iter : vec)
{
  iter = iter + 1; // { 1,2,3,4 }
}
```
- auto iter : 메모리를 복사해서 가지고 오는 것이기 떄문에, 실제 vector 안의 값이 변경되지 않는다
<br/>

### Ⅲ. UDP
#### 1️⃣ 호출 횟수
✅ TCP
- 데이터 송수신 과정에서 호출하는 [ 입력 호출횟수 ] != [ 출력 호출 횟수 ] : 크게 의미를 가지지 않는다
  - 입력 호출 횟수 : 4번
  - 출력 호출 횟수 : 3번 일 수 있다.
- stream 데이터 이기 떄문이다.
----
```cpp
sleep(10)
int recv_len = recv(serv_sock, msg, BUF_SIZE, 0);

// 클라이언트
for ( int i=0; i<3; i++ )
{
  int send_len = send(serv_sock, msg, BUF_SIZE, 0);
}
```

✅ UDP
- **[ 입력 호출횟수 ] == [ 출력 호출횟수 ] 동일** 해야한다.
  - 입력 호출 횟수 : 3번
  - 출력 호출 횟수 : 3번
-----
```cpp
// 서버
for ( int i=0; i<3; i++ )
{
  sleep[(10);
  int recv_len = recvfrom(serv_sock, msg, BUF_SIZE, 0, &clnt_addr, &clnt_addr_size);
}

// 클라이언트
for ( int i=0; i<3; i++ )
{
  int send_len = sendto(serv_sock, msg, BUF_SIZE, 0, &clnt_addr, clnt_addr_size);
}
```
- recvfrom : ( recv 횟수 : 3번 )
- send_len : ( send 횟수 : 3번 )
- 서버가 sleep(10) 을 하더라도, **늦게라도 클라이언트가 보낸 3번의 메세지는 (( 3번의 recv 함수를 통해서 )) 서버에게 도착**
