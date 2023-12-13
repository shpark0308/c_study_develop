### Ⅰ 

### Ⅱ

### Ⅲ. 기타
#### 1️⃣ nullptr VS NULL
- **pointer : nullptr**
- **int : NULL**

✅ NULL ( Int형 )
```cpp
int Integer, *Pointer;

void print(int ptr)
{
  Integer = ptr;
  std::cout << "Integer : " << ptr << std::endl;
}

void print(int* ptr)
{
  Integer = ptr;
  std::cout << "Pointer : " << ptr << std::endl;
}

Pointer = &Integer;
print(10);      // Integer : 10
print(NULL);    // Integer : 0
```
- print(NULL)
  - Pointer : 0 ❌
  - Integer : 0 ✔
- **NULL** 을 (( **정수** ))로 인식
  - C++ 11에서는 **#define NULL  (0)** 인 상수로 인식

✅ nullptr ( void * )
```cpp
Pointer = &Integer;

#### 2️⃣ sleep
✅ sleep 함수
```cpp
#include <unistd.h>

sleep(60); // sec 단위
```
- [Linux] : sleep(60)    // sec 단위
- [Window]: Sleep(60000) // msec 단위 

print(10);       // Integer : 10
print(NULL);     // Integer : 0
print(nullptr);  // Pointer : 00000000
```
- nullptr 은 (( **포인터** )) 로 인식
