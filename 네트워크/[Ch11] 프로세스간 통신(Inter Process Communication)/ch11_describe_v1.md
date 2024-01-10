ⅠⅡⅢⅣⅤ1️⃣2️⃣3️⃣✅

### Ⅰ. 프로세스
#### 1️⃣ 프로세스 개념
✅ 배경 지식
- 운영체제의 가장 작은 실행 단위

✅ 메모리 영역
- 각 프로세스는 서로 독립된 메모리 영역을 가진다
- 따라서, 메모리를 공유하기 위해서는, **동시에** 접근이 가능한 메모리 영역이 필요하다 ⇨ [ **메인 메모리** ]
- fork() 를 통한, 부모 ↔ 자식 간의 **프로세스도 메모리를 공유하지 않는다.**


### Ⅳ. 기타
#### 1️⃣ sigaction
✅ sigaction
- signal 함수보다 더 많은 기능을 지원

✅ sigaction 구조체
``` cpp
struct sigaction {
  void (*sa_handler)(int);
  void (*sa_sigaction)(int, sig_info_t *, void*);

  sigset_t sa_mask;
  int sa_flags;
  void (*sa_restorer)(void*)
}
```
