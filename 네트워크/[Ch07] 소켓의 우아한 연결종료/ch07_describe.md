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


