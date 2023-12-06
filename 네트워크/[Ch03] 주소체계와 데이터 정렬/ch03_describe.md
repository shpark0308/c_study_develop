### Ⅰ. 네트워크
#### 0️⃣ socket 생성
✅ int socket(int domain, int type, int protocol )
- 반환자 (int) : 성공 ( 파일 디스크립터 ) 실패 ( -1 )
- int domain : 프로토콜 체계
- int type : 데이터 전송 방식
- int protocol : 프로토콜 정보

✅ 라우터

#### 1️⃣ 게이트 웨이 ( Gateway )
✅ Gateway
- 한 네트워크 ( segment ) → 다른 네트워크로 이동하기 위한 지점
- 서로 다른 네트워크를 연결

✅ 설명
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/7fedca29-4dee-41df-baba-8b71b70dc002)
- [집] → [인터넷] : (집) → (공유기) → (인터넷 제공 회사 라우터(ISP)) → (인터넷 망)
  - (1). 공유기
  - (2). 인터넷 제공 회사 라우터
  ⇒ Gateway 라고 부름
- 이때, 거치는 게이트웨이 수, 홉 수 ( hop count ) 라고 부른다
  - 그림에서 hop count (2) 이다
<br/>

#### 2️⃣ 라우터
✅ 라우터
- 서로 다른 [ 네트워크 주소 #1 ] ⇿ [ 네트워크 주소 #2 ] 일 때, 서로 통신하도록 도와주는 장치
- IP 주소를 식별하여, 스위치 해주기 때문에 **[L3]** 계층 스위치이다.

✅ 라우팅 VS Gateway
- 라우팅 : 서로 다른 네트워크가 통신하기 위해 필요한 (( 장비 )), 물리적 개념
- Gateway : 서로 다른 네트워크를 연결할 때, 거쳐가는 것 ( hop count ), 논리적 개념

✅ 라우팅 테이블
```bash
$ route print
```
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/b5999196-e691-41d8-8e17-27aedcac7d98)
- metric
  - 네트워크 우선순위
  - 해당 metric 을 참고하여, 최적의 경로를 찾아냄

✅ 라우팅 테이블 설정
- 라우팅 테이블도 (( **설정** )) 이 가능하다

- 추가
``` bash
// route add [네트워크 대상] mask [네트워크 마스크] [게이트웨이] if [인터페이스] [옵션]
// -p : 영구설정. 재부팅 시 해당 설정 유지.

$ route add 10.10.10.0 mask 255.255.255.0 192.168.0.1
```
- 수정
```bash
// route change [네트워크 대상] mask [네트워크 마스크] [게이트웨이] if [인터페이스] [옵션]
// -p 등록된 라우팅일 경우 옵션 필수 기재

$ route change 10.10.10.0 mask 255.255.255.0 192.168.0.7 -p
```
<br/>

#### 2️⃣ WSL
- Windows SysSystem for Linux
- 리눅스 용 윈도우 하위 시스템

✅ 배경
- [ 윈도우 ] → (사용) → [ 리눅스 ] : [ Virtual Machine / VMWare ] 같은 도구 / 듀얼 부팅
  
✅ WSL
- 듀얼 부팅 또는 VM 과 같은 느린 환경이 아니더라도, 윈도우에서 PowerShell 을 Bash 처럼 사용 가능
- 윈도우에서 리눅스 환경처럼 사용 가능
- Linux 명령어 사용 가능

✅ 설치
``` bash
$ wsl --list --verbose
$ wsl --set-verbose Ubuntu2
$ wsl
```
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/a2230f92-384d-4bb7-8a46-6fee3b2bfc2e)
- [윈도우]를 리눅스 환경처럼 사용이 가능함

참조 사이트 : https://melonicedlatte.com/network/2020/04/28/201100.html
