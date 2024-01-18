
ⅠⅡⅢⅣ1️⃣2️⃣3️⃣4️⃣✅🔯

### Ⅰ. 프로젝트 개발
#### 1️⃣ 개발 명령어
✅ 프로세스 번호
``` bash
$ pidof pandora    // 2363721
$ cd /proc/$(pidof pandora)
```

(1). cwd ( current working directory )
``` bash
[shpark0308@localhost 2363721]$ ls -al cwd
lrwxrwxrwx 1 shpark0308 shpark0308 0  1월 18 10:06 cwd -> /home/shpark0308/work/output
```
- 디렉터리 위치 : /proc/<PID>/cwd
- 해당 프로세스가 현재 작업 중인 디렉터리를 **심볼릭 링크**로 나타냄

(2). fd
```
$ cd /proc/<PID>/fd
$ ls -al

// lrwx------ 1 shpark0308 shpark0308 64  1월 18 10:10 15 -> 'anon_inode:[eventpoll]'
// lrwx------ 1 shpark0308 shpark0308 64  1월 18 10:10 16 -> 'socket:[709414025]'
// lrwx------ 1 shpark0308 shpark0308 64  1월 18 10:10 17 -> /dev/nvidiactl
```
- 프로세스가 사용하는 ( 운영체제 )의 파일 디스크립터 리스트들 출력
- [17] fd -> 'socket[709414025]' 로 해당 파일 디스크립터가 사용하고 있는 자원이 무엇인지 출력

### Ⅱ. 모니터링 툴
#### 1️⃣ 윈도우
✅ pch.h
```bash
# include "pch.h"
```
- precompiled header
- 전처리된 헤더 파일을 나타낸다
- C++ 프로그램에서 컴파일 시간을 단축하기 위해 사용
- 주로 프로젝트 내에서 자주 사용되는 헤더 파일들을 포함

🔯 배경 지식
- [ 컴파일러 ] : ( 헤더 ) 파일을 읽어들여 **컴파일** 수행
- 하지만, 대다수의 헤더 파일은 거의 변경되지 않음
- 따라서, 이러한 헤더 파일을 미리 컴파일 한 후, 다시 컴파일 할 때, 재사용할 수 있도록 사용

✅ pch.h VS stdafx.h

(1). stdafx.h
- Microsoft Visual Studio 에서 사용
- precompiled header 를 지원하는 프로젝트에서 **미리 컴파일된 헤더를 포함**

(2). pch.h
- 좀 더 범용적으로 사용되는 용어
- 특정 컴파일러나 특정 IDE 에 국한되지 않고, 더 넓은 의미로 사용

----
🔯 공통점
- 미리 컴파일된 헤더를 사용하는 개넘

🔯 차이점
- 특정 프로젝트나 환경에 따라 사용방법이 다르다
