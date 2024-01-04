ⅠⅡⅢ1️⃣2️⃣3️⃣4️⃣✅
### Ⅰ. DNS
#### 1️⃣ DNS
✅ DNS
- Domain Name System
- **[IP주소]** (실제 주소) ↔ **[Domain 주소]** (가상 주소) 변환
  - Domain 주소
    - 표현하기도 쉬운 형태
    - ex. www.naver.com

✅ DNS 서버

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/46d3ca1e-4457-4ac1-b9f7-946545f0d077)

- 모든 컴퓨터는 Local **DNS 서버 주소**를 가지고 있다
- Local DNS 서버로부터 IP 주소를 찾지 못하는 경우 [다른 DNS 서버] 에게 물어본다
- 다른 DNS 서버에도 없을 경우, [root DNS 서버] 에게 물어본 후, 다시 반대 방향으로 돌아와, 해당 Domain 주소의 IP 주소를 반환한다.
---
- Root DNS 서버
  - 계층적 분산 데이터베이스 시스템 ( HDBMS (H:Hierarchical) )

✅ 접속 과정

(1). [브라우저] → Domain 주소 → DNS 서버

(2). DNS 서버 → (( IP 주소 응답 )) → 해당 IP 주소 사이트로 접속 → [브라우저]
<br/>

### 2️⃣ DNS 명령어
✅ nslookup
```bash
$ nslookup

// Default Server:  UnKnown
// Address:  10.40.29.172
```
- Local DNS 서버의 IP 주소를 반환

 ```bash
$ nslookup naver.com

// Name:    naver.com
// Addresses:  223.130.200.104
//             223.130.195.200
```
- nslookup [도메인 주소] 입력 시, DNS 서버로부터 IP 주소 반환

### 3️⃣ 코드
✅ struct hostent 구조체
```cpp
struct hostent
{
  char* h_name          // Official name
  char** h_aliases;     // 다른 도메인 이름
  int h_addrtype;       // AF_INET (IPV4)
  int h_length;         // IPV4 ( 4byte ) IPV6 ( 16byte )
  char** h_addr_list;   // IP 주소
};
```
- 하나의 도메인 이름에도, **(( 서버 부하 분산 ))** 을 위해, 여러 IP 주소를 두어, 이중화 처리를 할 수 있다.
- 따라서, **하나의 도메인 주소에도, 여러 IP 주소**가 있을 수 있다

✅ gethostbyname
```cpp
struct hostent* gethostbyname(const char* hostname);

printf("IP Addr : %s", inet_ntoa(*(struct in_addr*) hostent->h_addr_list[i]);
```
- Domain 주소 → IP 주소
- inet_ntoa(\*(struct in_addr*) addr) 주소 변환해주기

 ✅ gethostbyaddr
 ```cpp
struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family );

host = gethostbyaddr("127.0.0.1", 4, AF_INET);
```
-  addr 이 **(( char\* ))** 인 이유는, IPV4 또는 IPV6 일 수 있기 때문이다.
- socklen_t len : IPV4 ( 4byte ), IPV6 ( 16byte ) 로 설정
- int family : AF_INET

- 
