
ⅠⅡⅢ2️⃣3️⃣4️⃣✅1️⃣

### Ⅰ. 동시셩
#### 1️⃣ 동시성
✅ 스레드
- 프로세스의 **가장 작은 실행단위**

✅ 멀티 스레드
- 한 프로세스 내에서, 여러 스레드가 **동시성**을 갖고 실행
- 여러 CPU 를 나누어, **병렬 처리**
- 메모리를 공유하기 때문에, **동기화** 작업이 필요함
<br/>

#### 2️⃣ 문제점
- 동시성 문제점
- 동기화

✅ 경쟁 조건 ( Race Condition )
- 여러 프로세스 / 스레드가 동시에 같은 데이터를 변경하고자 할때, 잘못된 데이터를 쓸 수 있음

✅ Dead Lock
<br/>

#### 3️⃣ 해결 방법
- 동기화
- 여러 프로세스 / 스레드가 동시에 실행하는 메모리의 (( 일관성 )) 을 유지

✅ 상호 배제 ( Mutual Exclusion )
```bash
while(True)
{
  lock()
  // critical section
  unlock()
}
```
- 임계 영역을 하나의 프로세스 / 스레드만 진입해서 사용할 수 있도록 함


#### 4️⃣ Lock 의 필요성
✅ 폴링
``` cpp
while(IsSet(Lock) == true)
{
  // critical section
}
```
- 계속 IsSet(Lock) 이 true 값인지 확인
- CPU 가 계속 낭비되는 현상

✅ 인터럽트
- 폴링에서 계속 while 문으로 불필요하게 CPU 가 낭비되는 현상을 방지
- Lock 이 준비되면, 시스템 호출로 깨움

🔯 폴링 방식 > 인터럽트
- 멀티 코어 환경
- critical section 에서의 작업이, context-switching 보다 더 빨리 끝날 경우
---
▪ &nbsp; &nbsp; Lock 을 불필요하게 많이 걸어서, context-switching 하는 것보다

▪ &nbsp; &nbsp; 계속 while 문을 돌면서, critical-section 이 끝나기를 기다리는 것이 더 효율적

### Ⅱ. 동기화
#### 1️⃣ Mutex
✅ 뮤텍스
- Lock() / unLock() 을 통해, **하나의** 프로세스 또는 스레드가 임계영역을 진입할 수 있도록 함
- 이진 세마포어
- Lock() 을 가진 존재 ( 프로세스 / 스레드 ) 만이, unLock() 을 할 수 있다.
- priority inheritence 속성을 가진다
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/1ef50450-af59-45e3-a5a9-8c348538583d)

<br/>

#### 2️⃣ Semaphre
✅ 세마포어
- signal mechanism 을 가진, **하나 이상의** 프로세스 / 스레드가 critical section 에 접근 가능하도록 함
- count 세마포어
- 다른 프로세스에서 시그널을 통해, Lock() 을 해제할 수 있다
- priority inheritance 속성을 가지지 않는다

🔯 Mutex vs Semaphore

(1). Mutex
- 상호 배제만 필요하면, 뮤텍스 사용

(2). Semaphore
- 작업 간의 실행 순서 동기화가 필요하면, 세마포어 권장
