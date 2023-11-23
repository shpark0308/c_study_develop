### Ⅰ. 메모리 풀 (Memory Pool)
#### 0️⃣ 메모리 풀
✅ 배경
- 동적 할당
  - 종류
    - ( malloc / free ) : struct 구조체 사용 --> malloc + memset + (( free ))
    - ( new / delete )  : 클래스 사용 --> (( 클래스는 memset 하지 않기 ))
  - (( 다양한 Block Size )) ( 동적 할당 ) 으로 인하여, 단편화 유발
  - 파편화된 메모리들은 Performance 때문에 실시간 시스템에서 사용할 수 없음
  - 성능을 저하 시킴
    - [할당]  (malloc/new) → 운영체제 커널 요청 → 메모리 할당
    - [해제] (free/delete) → 운영체제 커널 요청 → 메모리 해제
    - ▫ 반복적인 동적 할당은 (( 사용자 영역 코드 )) ↔ (( 커널 영역 )) 을 전환 (시스템 전환) 이 많아지기 때문에 성능이 저하된다.
    - ▫ ( free / delete ) 를 못할 경우, 메모리 릭이 발생하여, 점유하는 메모리 영역이 증가해, 성능 저하를 발생
    - ▫ ( free / delete ) 를 못할 경우, 점유하는 메모리 영역이 증가하여, 메모리가 귀해져서, 하드디스크를 참조하게 되고, 이는 성능 저하를 발생시킴
 
✅ 메모리 풀
- 고정된 크기의 Block 을 미리 할당 하여, malloc / new 연산을 통해 유사한 메모리 동적 할당을 가능하게 해줌
  - 미리 Block 을 할당 해줌으로써, 계속적으로 커널 시스템 모드로 전환을 할 필요가 없다
    - 고정된 크기의 Block 할당 : ( 고정된 크기, 메모리 재사용, 파편화 X ) → Cache 효율성이 좋다
  - 유사한 메모리 동적 할당 기능을 가능하게 하여, 실행 시간 ( Run-time ) 에 handle 에 의해서 표현되는 블록들을 할당하고 접근한다.
 
✅ 배경 지식
- 메모리 종류
  - (1). 고정 메모리
    - 고정된 크기   : 고정 크기의 Block 을 갖는 메모리 관리자
    - 메모리 재사용 : 반환된 메모리의 (( 효율적인 재사용 ))
    - 파편화 X     : (( 동일한 크기의 Block )) 을 요청하기 때문에 파편화 발생 X
      - ▫ 내부 단편화 X : 고정된 크기 할당으로 내부 단편화 발생 안함
      - ▫ 외부 단편화 X : 미리 외부에서 할당 받은 공간에 메모리 할당 / 해제를 하기 때문에 외부 단변화 발생 안함
    - Cache 효율성 : 고정된 크기의 Block 을 갖는 메모리 관리자는 (( Cache 효율 )) 이 좋다.
   
  - (2). 가변 메모리

✅ 목표
- 고정된 크기
- 메모리 재사용
- 파편화 X
- Cache 효율성
- 속력 향상
  - 컴파일러가 제공하는 할당자보다 속력이 더 빨라야 한다
  - 미리 할당해놓고 가져다가 사용하기 때문에, 잦은 시스템 전환을 막을 수 있어 속력 향상
- 안정성
  - 프로그램 종료 이전, 메모리 누수 방지  

#### 3️⃣ 기타
✅ 문자열
```cpp
char* temp = new char[10];
memcpy(temp, "1234567890", 10);
temp[5] = 0

printf("결과: %s\n", temp); // 12345
```
- temp[5] = **0** || temp[5] = **'\0'** : 문자의 끝을 의미
- 따라서, memset(temp, 0, 10); 할 때, 주의 → 모든 char 문자열들을 ( 문자의 끝 )으로 만들고 시작하는 것을 뜻함

✅ 변수
- Class 멤버 젼수
  - 기본적으로 초기화 되어서, (( 0 / nullptr )) 로 값이 저장된다.
  - Garbage 값으로 저장 X
  - Class 의 멤버 변수는 일반적으로 힙 영역에 저장됩니다.
    - Class 는 일반적으로 (( 동적 할당 ))
    - Stack 은 일반적으로 (( 정적 할당 ))

✅ 구조체 스스로 할당
```cpp
struct Block {
  struct Block* self;
};

// struct Block block; (X)
struct Block* block = (struct Block*) malloc(sizeof(struct Block)); // (O)
```
- (X): struct Block block;
  - 자기 자신 ( self ) 가 있는 구조체는 정적 할당이 안된다.
  - 자기 자신의 (( 구조체의 크기 )) 를 가늠할 수 없으니까
- (O): struct Block* block = (struct Block*) malloc(sizeof(struct Block));

✅ 포인터 복사
```cpp
void* p1 = malloc(sizeof(10)); // 주소값 : 0x7fff54f32140 0x7fff54f32150 데이터값 : 0x1ad5eb0
void* p2 = p1;                 // 주소값 : 0x7fff54f32148
(   ) p3 = &p1;                // 주소값 : 0x7fff54f32150

p1 = (char*)p1 + 10;
printf("p1: [%p:%p]  p2:[%p:%p]  p3:[%p:%p]", &p1, p1, &p2, p2, &p3, *p3);
// 결과
// p1: [(     ):(     )]  p2:[(     ):(     )]  p3:[(     ):(     )]
```
- ( <b>void** </b> ) p3 = &p1;
  - 포인터의 주소값 type은 void** 이어야 한다.
- p1: [(**0x7fff54f32140**):(**0x1ad5eba**)]  p2:[(**0x7fff54f32148**):**0x1ad5eb0**)]  p3:[(**0x7fff54f32150**):(**0x1ad5eba**)]
  ![image](https://github.com/shpark0308/c_study_develop/assets/60208434/6b946c70-b174-467d-a635-3389cc04c34b)
- p1 = (char*)p1 + 10;
  - void* 는 (포인터) 이기 때문에 산술을 할 수 없다.
  - 따라서 다른 유형의 포인터 ( <b>char*</b> ) 캐스팅 후, 산술 해야한다.
 
✅ 메모리 해제
```cpp
// 위에 코드에 이어서 메모리 해제를 시켜주려면...
free(p2);
```
- free(p2);
  - 위 코드에서 malloc 했던 메모리의 시작 주소가 p2 이기 때문에 free(p2) 를 해야한다.
- free(p1) / free(p3) 
  - p1, p3 는 힙 영역에 선언 한 것이 아니기 때문에, free 할 필요가 없다.
  - free 를 할 경우, "free(): invalid pointer 중지됨 (core dumped)" 가 발생

✅ std::vector
```cpp
#include <vector>
#include <algorithm> // std::find, std::distance 사용하기 위함

std::vector<int> vec = { 1,2 3,4,5 }

// find
auto iter = std::find(vec.begin(), vec.end(), 4);
if (iter == vec.end()) {
  printf("not found in vector");
}

// index
int index = std::distance(vec.begin(), iter);
printf("index: %d"); // index = 3 반환
```
- #include <algorithm>
  - std::find / std::distance 를 사용하기 위한 (( header ))
- std::find(vec.begin(), vec.end(), value)
  - (**find O**) auto iter != vec.end()
  - (**find X**) auto iter == vec.end()
- std::distance(vec.begin(), iter)
  - vec.begin() 부터 iter 까지의 distance 를 계산하여 index 값을 반환함
 
✅ pointer
``` cpp
void* double_v1 = (double*) malloc(sizeof(double)); // 0x17a9eb0
void* double_v2 = (double*)double_v1 + sizeof(double);
void* double_v3 = (char*)double_v1 + sizeof(double);
void* double_v4 = (double*)((char*)double_v1+sizeof(double));

printf("[1]. double_v1: %p"); // (        )
printf("[2]. double_v2: %p"); // (        )
printf("[3]. double_v3: %p"); // (        )
printf("[4]. double_v4: %p"); // (        )
```
- [1]. (**0x17a9eb0**)
- [2]. (**0x17a9ef0**)
  - (double*)double_v1 // 단위: double* 여서 8byte 가 되고 sizeof(double) 은 8이기 때문에 8*8 = 64 (10진수) 가 된다.
  - 따라서, double_v2 = double_v1( 0x17a9eb0 ) + 40(16진수, 10진수 : 64 ) = 0x17a9ef0 이 된다.
- [3]. (**0x17a9eb8**)
  - (char*)double_v1 // 단위: char* 여서 1byte가 되고, sizeof(double) 은 8이기 떄문에 1*8 = 8 (10진수) 가 된다.
  - 따라서, double_v3 = double_v1( 0x17a9eb0 ) + 8(16진수, 10진수 : 8 ) = 0x17a9eb8 이 된다.
- [4]. (**0x17a9eb8**)
  - 위에 double_v3와 같은 연산이기 때문에, 포인터의 위치는 0x17a9eb8 이 된다.
  - 하지만 변환은 (double*) 가 된다
