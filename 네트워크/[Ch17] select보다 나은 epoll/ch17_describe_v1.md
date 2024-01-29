
ⅠⅡⅢⅣ1️⃣2️⃣3️⃣4️⃣✅🔯

### Ⅰ. I/O 멀티플렉싱
#### 1️⃣ select
✅ 소개
- FD_SET ( fd 집합체 ) 에서 **하나의 fd 라도 변화가 있을 경우**, 블로킹 / 논블로킹 됨
- 싱글 스레드로 다중 I/O 를 처리

🔯 과정

(1) [ 프로세스 ] → [ 커널 ] : 프로세스가 ( 커널의 상황 )을 지속적으로 확인
``` cpp
while(true)
{
  cpy_reads = temp_reads;   // fd_set 복사

  int fd_num = select( fd_max + 1, &cpy_reads, 0, 0, &timeout );
}
```
- FD_SET 에 있는 모든 fd 목록 전체를 체크해야 한다.
- 원본 fd_set 이 아니라, 복사본을 전달해야한다.
  - select() 함수가 실행 ( 블로킹 ) 하는 도중, 파일 디스크럽터 집합 ( 원본 ) 이 변할 수 있음

(2) [ 커널 ] → [ 프로세스 ]
- 이에 대응

(3). 변화 발생
- fd 가 할당 받은 (( 커널 버퍼 )) 에 데이터 복사

✅ 장점
- 모든 운영체제에서 사용 가능 ( OS 에 종속적이지 않음 )
- 싱글 스레드에서 다중 I/O 처리

✅ 단점
- FD_SET 에 있는 모든 Fd 에 대해서 I/O 할 준비가 되었는지 확인
  - 비 효율적
  - 느리다
- fd_set 의 원본이 아니라 복사본을 전달해야하는 번거로움
- 관리할 수 있는 fd 의 갯수 한계 ( 1024개 )

&nbsp; &nbsp; &nbsp;**⇨ epoll ( select() 함수의 한계점 극복 )**
<br/>

#### 2️⃣ poll
✅ 소개
- select() 의 개선 방안
- select 에 비해 관리할 수 있는 fd 의 갯수가 증가함

✅ 단점
- 일부 UNIX ( 리눅스 ) 시스템에서 지원 안함
<br>

#### 3️⃣ epoll
✅ 소개
- select() 함수의 한계점 극복
- 커널 레벨의 멀티 플렉싱 지원

🔯 과정

(1). [ 프로세스 ] → [ 커널 ]
- 관찰 대상의 fd 를 전달 ( epoll_ctl )

(2) [ 커널 ] → [ 프로세스 ]
- 변경 사항이 있는 fd 에 대해서만 알림

✅ 장점
- select() 와 다르게, 모든 fd 가 아닌, 변경사항이 있는 fd 에 대해서만 처리를 하면 된다.
- 관리할 수 있는 fd 의 갯수가 증가하였다.
``` bash
[shpark0308@localhost fd]$ cat /proc/sys/fs/epoll/max_user_watches
26839449
```

✅ 단점
- 운영체제에 종속적
  - 리눅스 : epoll
  - 윈도우 : IOCP
 
🔯 select VS epoll

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/4465d502-e003-44d1-9344-4e1e122775ed)
