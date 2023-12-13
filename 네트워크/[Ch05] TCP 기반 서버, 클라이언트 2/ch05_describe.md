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

print(10);       // Integer : 10
print(NULL);     // Integer : 0
print(nullptr);  // Pointer : 00000000
```
- nullptr 은 (( **포인터** )) 로 인식
<br/>

#### 2️⃣ sleep
✅ sleep 함수
```cpp
#include <unistd.h>

sleep(60); // sec 단위
```
- [Linux] : sleep(60)    // sec 단위
- [Window]: Sleep(60000) // msec 단위 
<br/>

#### 3️⃣ pthread_join

![image](https://github.com/shpark0308/c_study_develop/assets/60208434/7a36b64a-f39d-4935-bb7f-daad141dff92)

✅ main() : pthread_join 2번
- 같은 main() 함수 내에서 순차적인 pthread_join 2번 실행

① ( 실제 동작하는 스레드 ) 를 대기 → 종료 <br/>
② ( 없는 스레드 ( 종료된 스레드 ) ) → 종료 <br/>

✅ main() : pthread_join 1번, join_func() : pthread_join 1번
- main() 함수에서 pthread_join() 실행
- join_func() 함수에서, **병렬적** ( 동시에 ) pthread_join() 실행

① main함수 : ( 실제 동작하는 스레드 ) 를 대기 → 종료 ( main 함수 또는 join_func() )
② join_func() : ( 실제 동작하는 스레드 ) 를 대기 → **대기** <br/>
&nbsp; &nbsp; &nbsp; &nbsp; ▪ 이미 종료된 스레드에 대해서 대기하기 때문에, (( **블로킹** )) 현상
