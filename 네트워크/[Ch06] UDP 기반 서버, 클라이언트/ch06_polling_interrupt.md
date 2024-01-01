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
