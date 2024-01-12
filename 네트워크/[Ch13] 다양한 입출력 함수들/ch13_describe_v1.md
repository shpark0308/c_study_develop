

ⅠⅡⅢⅣ1️⃣2️⃣3️⃣4️⃣✅


### Ⅰ. send/recv 함수
#### 1️⃣ send / recv
✅ send
```cpp
#include <sys/socket.h>

size_t send(int fd, (const void*) buf, size_t buf_size, int option);
```
- 성공 ( 전송된 바이트 수 ), 실패 ( -1 )

✅ recv
```cpp
size_t recv(int fd, void* buf, size_t buf_size, int option);
```
- 성공 ( 수신한 바이트 수), 실패 -1
- **EOF ( 0 )**

🔯 EOF
- shutdown(serv_sock, SHUT_WR) : EOF 전송을 통해, stream 을 닫을 것을 전송
- shutdown 은 FIN 플래그 전달

#### 2️⃣ 옵션
✅ int option
|인덱스|옵션(Option)|의미|send|recv|
|:---:|:----------:|:--:|:--:|:--:|
|1|MSG_OOB|||












