






ⅠⅡⅢ1️⃣2️⃣3️⃣4️⃣✅







### Ⅳ. 기타
#### 1️⃣ dstat 설치 오류
✅ Python.h 에러
- 에러
```
pmapi.c:27:10: fatal error: Python.h: No such file or directory
#include <Python.h>
          ^~~~~~~~~~
compilation terminated.
error: command 'gcc' failed with exit status 1
```
- 해결 방법
```bash
$ sudo yum install python3-devel    # python3
$ dnf install -y python38-devel // ( 실제 해결 방법 )
```
- /usr/include/python3.8 에 Python.h 와 같은 기본적인 헤더가 없어서 발생한 문제
- dnf install -y python38-devel 이후, **python3.8 파일에 필요한 헤더들이 설치**

✅ pcp 에러
- 헤결 방법
``` bash
$ pip3 install pcp
$ sudo yum install pcp
```
