
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
