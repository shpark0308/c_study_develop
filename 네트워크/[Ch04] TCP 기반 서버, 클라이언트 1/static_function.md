### Ⅰ. static 함수
#### 1️⃣. static 함수
✅ 클래스 멤버 함수 ( static )
- 특정 인스턴스 에 국한되지 않고, 클래스 자체에 속하는 함수를 의미
- 일반적으로 Class 는 헤더 파일에 구현되기 때문에, 다른 파일에서도 해당 함수를 사용할 수 있다.
- **[링킹] 과정에서, 포함**
```cpp
class MyClass
{
        public:
        static void printClass()
        {
                printf("===== ClassFunc() =====");
        }
};

// (1).
MyClass::printClass();

// (2).
MyClass myClass; // 객체 생성
myClass.printClass(); // 인스턴스에서 클래스 static 함수 호출
```
(1). MyClass::printClass()
- 인스턴스 (객체) 를 생성하지 않고도 사용

(2). myClass.printClass()
- 인스턴스 생성 후, 객체를 통해서 static 함수 호출
<br/>

✅ 파일 범위
- 해당 static 함수가 정의된 파일 내에서만 사용이 가능
- 따라서, 다른 file2.cpp 에서도 같은 이름의 함수를 정의하여도, 충돌되지 않음
- **[링킹] 과정에서, 배제** ( 해당 파일 내에서만 유효하도록 함 )
```cpp
// file1.cpp
static void printFunction()
{
        printf("===== ClassFunc() =====");
}
```
- 해당 static 함수가 정의된 파일 내에서만 사용이 가능
- 따라서, 다른 file2.cpp 에서도 같은 이름의 함수를 정의하여도, 충돌되지 않음

### Ⅱ. static 변수
#### 1️⃣ ( 클래스 ) 정적 멤버 변수
✅ 선언

🔯 클래스 멤버 변수 (일반적 변수)
- [선언]
  - 해당 클래스에 다음과 같은 변수가 있다고 알림 
- [할당]
  - 클래스 선언 시, 멤버 변수는 메모리를 할당하는 것 아니다
  - 객체를 생성할때, 인스턴스의 멤버 변수 메모리를 할당
    - 따라서, 클래스 외부에서, 객체를 생성할 때, 멤버 변수의 메모리를 할당

🔯 클래스 정적 멤버 변수
```cpp
class MyClass
{
        public:
                static int num;
};

int MyClass::num = 1;
```
(1). 초기화
- 정적 멤버 변수의 초기화는 객체 생성 시, ❌
  - 따라서, static int num = 0; 으로 초기화 할 수 없다.
  - static int num = 0; 으로 할 경우, 인스턴스가 생성될 때마다, **중복하여 초기화** 하기 떄문이다.
- 컴파일 타임에서 이루어 진다 ✔
  - int MyClass::num = 1;
  ```cpp
  int main()
  {
        int MyClass::num = 1; // 잘못된 코드
        return 0;
  }
  ```
    - 컴파일 타임 시, 초기화 되는 것이 아니라, main() 함수 호출 시, 초기화 하는 거여서, 에러가 발생한다.
    - 클래스가 헤더 파일에서 선언될 경우, 여러 소스 파일의 main() 함수에서 호출하게 된다.
      - 이러한 경우, 클래스의 정적 멤버 변수가 중복되어 초기화 될 수 있기 때문에, 이를 방지하기 위함이다.


### Ⅲ. 코드 작업
#### 1️⃣ 멤버 정적 함수, 변수
✅ 멤버 정적 함수, 변수
```cpp
class MyClass
{
        public:
                int a = 100;
                static int b;

        public:
                static void printClass()
                {
                        printf("===== ClassFunc() =====\n");
                }
                void increment_A()
                {
                        a++;
                }
                static void increment_B()
                {
                        b++;
                }
                int getCount_A()
                {
                        return a;
                }
                static int getCount_B()
                {
                        return b;
                }
};

int MyClass::b = 100;
```
- static void increment 정적 멤버 함수 내에서는 정적 멤버 변수만 사용 할 수 있다.
- // num2++; 이렇게 사용할 경우, 잘못된 코드이다. num2 는 static 변수가 아니기 때문이다.
----
- static 멤버 변수는 static 멤버 함수
- 멤버 변수는 멤버 함수로 사용하는 것이 가장 깔끔하다
