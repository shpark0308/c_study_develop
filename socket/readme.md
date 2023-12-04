### Ⅰ. 
#### 0️⃣ Perfmon
✅ Perfmon
- Performance Monitor
- Windows 운영 체제에서 시스템 성능을 모니터링하기 위한 도구

✅ 성능 측정
- CPU 사용률
- 메모리 사용량
- 디스크 활동
- 네트워크 트래픽

### Ⅱ. 다중 클라이언트 구현
#### 0️⃣ 소스 코드
✅ 소스 코드
- dev_server_v1.cpp
- dev_client_v1.cpp

#### 1️⃣ MultiThread
✅ GPU
- nvidia-smi
✅ CPU
```bash
$ dstat -tcmn 1 // t (time) c (cpu) m (memory) n (network) 1 (1초)
```
<br/>

#### 2️⃣ Socket 구현
✅ sockaddr_in 정보
``` cpp
struct sockaddr_in 
```

✅ accept
``` cpp
int clnt_sock_arr[MAX_CLIENT]; // (1)

void* accept_func(void*arg)
{
  ...
  clnt_sock_arr[i] = accept(*serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_addr_size); // (2)
  ...
}

// 4. accpet
pthread_t accept_thread;

bRet = pthread_create(&accept_thread, NULL, accept_func, (void*)&serv_sock); // (3)
if (bRet!=0) err_handle("(accept) pthread_create failed");
```
(1). 각 **클라이언트의 파일 디스크립터**를 전역변수로 저장
(2). connect 연결 허용 ( accept ) 가 되면, 해당 클라이언트의 파일 디스크립터를 clnt_sock_arr[i] 에 저장
(3). 따로 accept 기능만 하는 thread 를 분리시켜 **언제 들어올지 모르는** 클라이언트 connection 을 담당

#### 3️⃣ 고민 거리
✅ 고민
- 하나의 프로세스 ( 하나의 client ) 창에서 send / recv 를 동시에 어떻게 처리하였는지
- server 에서 여러 client 의 accept 를 어떻게 처리하였는지
  - blocking 되는 accept 를 어떻게 하였는지
  - 언제 어떤 client 가 connection 을 요청할지 모르는 입장에서 ( server ) 계속 blocking 되는 accept 를 기다리는 지 <br/>
  📌 [나의 해결법]
    - accept 만 하는 thread 를 별도로 분리
    - 하지만 END 요청이 들어올 경우, blocking 이 되어있는 accept 스레드를 종료시키기 위해서 별도로 accept_exit_thread 를 구현
