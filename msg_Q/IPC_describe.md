### Ⅰ. IPC
#### 0️⃣ 메모리 풀
✅ 배경


#### 1️⃣ IPC 종류

### Ⅱ. IPC 종류 ( 6가지 )
(1). PIPE &nbsp;&nbsp;&nbsp;&nbsp; (2). Msg_Q &nbsp;&nbsp;&nbsp;&nbsp; (3). Shared Memory &nbsp;&nbsp;&nbsp;&nbsp; (4). Socket &nbsp;&nbsp;&nbsp;&nbsp; (5). Memory Map
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/6059e28e-94a9-4bc5-ad6b-1f30c47763ff)

#### 1️⃣ PIPE
✅ 기본 정보
1. 사용 시기 : 부모 → 자식 간 **단방향** 통신
2. 공유 매개체 : 파일
3. 통신 단위 : stream
4. 통신 방향 : 단방향
5. 통신 가능 범위 : 동일 시스템


#### 2️⃣ Named PIPE
✅ 기본 정보
- [ 양방향 통신 | 파일 | stream | 동일 시스템 ]
- 파일을 사용한 PIPE

#### 3️⃣ Message Queue
✅ 기본 정보
- [ **단방향 통신** | 메모리 | (( 구조체 )) | 동일 시스템 ]
- 메모리를 사용한 PIPE
- FIFO 구조
- msgType 에 따라 다른 구조체를 가져올 수 있다.
- 주의 사항 : Queue Full 로 인한 제한

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/d8d31954-a74c-43c8-ba2b-3e8e08aaee34)

#### 4️⃣ Shared Memory ( 양방향 )
✅ 기본 정보
1. 사용 시기 : 다른 프로세스와 **양방향 통신**
2. 공유 매개체 : 메모리
3. 통신 단위 : 구조체
4. 통신 방향 : **양방향**
5. 통신 가능 범위 : 동일 시스템

#### 5️⃣ Memory Map ( 양방향 )
✅ 기본 정보
1. 사용 시기 : 다른 프로세스와 양방향 통신
2. 공유 매개체 : 파일 + 메모리
3. 통신 단위 : 페이지
4. 통신 방향 : 양방향
5. 통신 가능 범위 : 동일 시스템

#### 6️⃣ Socket ( 양방향 )
✅ 기본 정보
1. 사용 시기 : 다른 프로세스와 양방향 통신
2. 공유 매개체 : 소켓
3. 통신 단위 : stream
4. 통신 방향 : 양방향
5. 통신 가능 범위 : 동일 & 외부 시스템


### Ⅲ. 기타
#### 1️⃣ 참조 사이트
✅ 사이트
- https://doitnow-man.tistory.com/110
