### Ⅰ. 예제
#### 1️⃣ threadFunc
✅ threadFunc
```cpp
int sharedData = 0;

void* threadFunc(void*arg) {
  for (int i=0; i<1000; i++ ) {
    add_sharedData();
  }
}

void add_sharedData() {
  sharedData++; // 2987 (O) 3000 (X)
}

pthread_create(pthread1, nullptr, threadFunc, nullptr); // thread1
pthread_create(pthread2, nullptr, threadFunc, nullptr); // thread2
pthread_create(pthread3, nullptr, threadFunc, nullptr); // thread3
```
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/55cdb68c-874a-40a7-85b8-16d727d85d17)

- add_sharedData() 또한 **순차적으로** 진행되는 것이 아니라, 각 스레드에 의해서 **병렬적** 으로 실행
- sharedData = 3000 이 아니라 2987 값으로 동기화가 되지 않은 결과 값이 나옴



### Ⅱ. TCP 연결 종료
#### 1️⃣ 4-way Handshake
✅ 4-way Handshake
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/11771453-0f8b-48d7-be2d-dc1a80d8beef)

✅ TIME-WAIT

(1). FIN-WAIT
(2). CLOSE-WAIT
(3). TIME-WAIT

✅ 특이점

(1). 연결 종료 ( 4 way handshake ) 는 연결 ( 3 way handshake ) 와 다르게
- 잔여 패킷 전송을 위하여 일단 FIN 신호에 대해 ACK 를 보내주어야 하기 때문에, **4번의 연결 과정**이 필요하다

(2). 잔여 패킷이, FIN 플래그 이후에 도착하는 경우
- 만약에, [클라이언트] ← (( FIN )) ← [서버] 전송하기 전, 잔여 패킷을 전송과정에서 보낸 패킷이
- **라우팅 지연**, **패킷 유실**로 인한 재전송으로 FIN 패킷 이후에 늦게 도착하는 경우,
- [ 클라이언트 ] 는 이런 상황을 대비하여, TIME WAIT에서, [서버]로부터 FIN 플래그를 받더라도
- **일정 시간동안 세선을 남겨 놓고, 잉여 패킷을 기다리는 과정**을 거친다.
- --> 우리 코드에는 이러한 과정이 어디에 있는지

(3). 초기 Sequence Number인 ISN ( Initial Sequence Number ) 는 **0부터** 시작하지 않고, 난수를 생성하여 시작한다
- Connection 을 맺을 때, 사용한 포트 ( Port ) 가 유한 범위 내에서 사용되고, 시간이 지남에 따라, **재사용** 될 수 있는데,
- [난수] 가 아닌, 순차적인 Number 로 ISN 을 전송하면, 과거에 사용한 포트의 Connection 으로부터 오는 패킷으로 인식할 수 있기 떄문에,
- 이러한 가능성을 줄이고자, **난수** 로 설정하여 전송한다.


 
### Ⅲ. 기타
#### 1️⃣ 참조 사이트
✅ 사이트
- [ TCP 3/4 way handshake ] (https://jeongkyun-it.tistory.com/180)
