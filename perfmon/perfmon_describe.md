### Ⅰ. Perfmon ( Performance Monitor )
#### 0️⃣ Perfmon
✅ Perfmon
- Performance Monitor
- Windows 운영 체제에서 시스템 성능을 모니터링하기 위한 도구

✅ 성능 측정
- CPU 사용률
- 메모리 사용량
- 디스크 활동
- 네트워크 트래픽

<br/>

### Ⅱ. 리눅스 모니터링 시스템
📌 : CCU Test 시, 주로 사용 <br/>

#### 1️⃣ GPU / CPU 명령어
✅ GPU
- nvidia-smi
```bash
$ watch -n 0.5 nvidia-smi // ( 0.5 초 간격으로 모니터링 )
```
```bash
$ nvidia-smi dmon -s pucvmet -o T -f "smi-gpu.txt"

// dmon : 디지털 센서 데이터 모니터링 ( power, temperature ... )
// -s pucvmet ( v(전압), u(전력), c(전류) 등 )
// -o T ( 시간 출력 )
// -f "smi-gpu.txt" ( 파일로 output 생성 )
```
✅ CPU
```bash
$ dstat -tcmn 1 // t (time) c (cpu) m (memory) n (network) 1 (1초)
```
<br/>

#### 2️⃣ CPU 사용률
✅ top
```bash
$ top
$ top -H -p [pid] // 특정 프로세스의 -H (스레드) 모두 보기
```
- **[요약 영역]**
  - **Tasks**: 811 total,   1 running, 810 sleeping,   0 stopped,   0 zombie
    - Tasks : 현재 프로세스들의 상태를 나타내주는 영역
    - Total : 전체 프로세스
    - 프로세스 상태 : ( running / sleep / stopped / zombie )
    - 프로세스
      - [ I/O 기반의 일 ] + [ CPU 기반의 일 ] ( ⇔ 번갈아 수행 )
        - ▫ I/O 기반의 일을 하면, (( CPU : Idle )) 상태로 들어감
  - **%Cpu(s)**:  0.4 us,  1.4 sy,  0.8 ni, 95.8 id,  0.0 wa,  1.2 hi,  0.3 si,  0.0 st
    - us (user) sy (system) **id(idle)**
  - **MiB Mem** : 128041.0 total,  88959.5 free,  19634.4 used,  19447.2 buff/cache ( RAM 메모리 영역 )<br/>
    **MiB Swap**:   4096.0 total,   4087.5 free,      8.5 used.  59795.6 avail Mem ( 디스크 메모리 )
    - 일반적으로 Mem 의 사용량이 가득 차면, Swap 메모리 영역을 사용한다.
    - Swap 은 디스크 영역이기 때문에, Mem 보다는 속도가 느리다
    - buff/cache : I/O 관련하여 사용되는 버퍼 메모리의 크기
    - avail Mem : Swap 메모리를 사용하지 않고 사용할 수 있는 메모리 영역
- Command
  - k (kill process)
  - sorting
    - M ( memory sort )
    - P ( cpu usage sort )
    - N ( process ID sort )
    - T ( running time sort )
    - R ( 오름차순 / 내림차순 )
  - ESC ( 나가기 )
  - H ( Thread 정보 제공 )
<br/>

✅ htop 📌
```bash
$ htop
```
- top 의 확장 버전
  - top : 하나의 core 를 하나의 cpu로 계산
  - htop : core 갯수를 확인해서 각 프로세스 정보를 더 디테일 하게 모니터링
- Command
  - F2 : Setup ( thread 보기 설정 )
  - F5 : 트리구조로 설명
  - **F6** : 정렬
  - F9 : Kill
  - F10 : htop 종료
<br/>  

✅ perftop

<br/>  

✅ sar
```bash
$ sar
```
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/4a325fe8-4b3d-48d5-948b-99d8c7773934)

<br/>

#### 3️⃣ 메모리 사용률
✅ free -h
```bash
$ free -h // -h ( 인간이 보는 형식으로 출력 ex. KB, KiB )
```
<br/>

✅ df -h
```
$ df -h
```
- Disk Free ( df )
- 디스크 공간 사용
- 파일 시스템 전체 상태를 보여줌<br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/ef583420-16a4-42a8-9fc0-b80da3125257)

<br/>
  
✅ du -h
```bash
$ du -h
```
- Disk Usage ( du )
- 특정 디렉터리나 파일 용량 확인<br/>
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/56c782d9-3a61-4b0c-bf7b-31f1c7296f93)

<br/>

#### 4️⃣ 디스크 활동
✅ vmstat 📌
```bash
$ vmstat -w [간격] [횟수] // -w ( 출력 간격의 너비 맞춤 )
```
- 가상 메모리 통계, 프로세스 / I/O / memory, system 에 대한 분석
- [설명]
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/65e2c66b-0641-4ed0-b7bd-05969badb5ac)
  - procs : 현재 CPU 내에 존재하는 Queue 에 적재된 Command 수
    - r (run queue) : 작업 수행을 위해, CPU 를 기다리는 Command 수
      - ▫ CPU 자원을 기다리는 (r) 의 영역이 더 중요함
      - ▫ (r) 의 단위가 많을 수록 OS 자체의 bottleneck 을 의미함
    - b (blocked queue) : 메모리나 기타 I/O 등에 대해 자원을 기다리는 Command 수
  - 메모리
    - free : OS 에 실제 남은 메모리 ( 물리적 메모리를 의미 )
  - swap ( 디스크 스왑 메모리 )
    - si : Amount of memory **swapped in** from disk ( 디스크에서 RAM 으로 데이터를 가지고 오는 작업 )
    - so : Amount of memory **swapped out** from disk ( RAM에서 디스크로 데이터를 옮기는 작업 )
      - ▫ Swap 은 주로 운영체제에서 RAM ( 물리적 메모리 ) 부족 시 사용
      - ▫ Swap In / Out 이 많을 경우, 성능 저하 발생
  - system
    - sy ( system call ) : OS 의 시스템 영역에서 수행하는 system call 갯수 (ex. file read/write, socket )
    - cs ( context-switching )
<br/>
     
✅ dstat 📌
```bash
$ dstat -tcmn 1 // 1 ( 1초 간격 )
```
-  ipc, file lock, socket, disk, cpu, load, 메모리 등 모니터링 가능
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/3484caf2-e68c-4a17-a883-dd8f28156b79)
<br/>

✅ iostat
```bash
$ iostat [간격] [횟수]
```
- 디스크 I/O 통계 및 CPU 사용율
- 통계 방법
  - Linux 커널은 I/O 발생 시, CPU & device 사용률 정보를 /proc/diskstats. sys/block/디바이스/stat 에 저장
  - 이를 바탕으로 값을 계산하여, iostat 지표로 찍음
- 예시
  -     $ iostat -dxm 1 -p ALL // d (디스크 사용량 정보), x(확장 정보) m(초당 정보 MB/s로 출력)
<br/>

#### 5️⃣ 네트워크 사용량
✅ nload
```bash
$ nload
```
- 실시간으로 네트워크 사용량을 그래픽으로 표시
- Device 에 따라서 (eno1, eno2, eno3 .. ) 분석해야 함
<br/>

✅ iftop 📌
```bash
$ sudo iftop
$ sudo iftop -i eth0 -f "dst port 22" // -i ( interface ) 지정, -f ( filter )
$ sudo iftop -i eth0 -f "dst host google.com"
```
- 설명
  - TX ( transmit ) : 전송량 
  - RX ( Receive ) : 수신량
  - Total : 총량
- Command
  - 참고 [https://dev.plusblog.co.kr/9]
  - Host : ( n | s | d | t )
  - Port : ( N | S | D | P )
  - Sort : ( 1 | 2 | 3 ) ( column ) / ( **<** ( source ) | **>** ( dest ) | **o** ( freeze ) ) 

✅ bmon 📌
```bash
$ bmon
```
- Command
  - 네트워크 인터페이스 (eth2) 로 이동
  - RX / TX 에 초 당 전송되는 패킷의 수가 표시
  - < / > 로 그래프 추가
  - D : detail 정보 출력

<br/>

### Ⅲ. 리눅스 명령어
#### 1️⃣ 명령어
✅ 명령어
- $who
  - 현재 몇 명, 누가 접속했는지 확인 가능
<br/>

#### 2️⃣ 기타
✅ 기타
- CPU Idle
  - Idle 상태 ( 유휴 상태 )
    - 컴퓨터 시스템이 (( 어떠한 작업도 수행하지 않고 )), (( 대기 하고 있는 상태 ))
    - 어떠한 자원 / 장치도 사용하지 않은 상태
    - 놀거나 쉬고 있는 시간
  - CPU Idle
    - 90% Idle
      - **10%** : CPU 사용, **90%** : 유휴 상태
    - CPU Idle 상태
      - 부하가 낮거나, 사용 가능한 자원이 많다.
     
- 프로세스
  - [ I/O 기반의 일 ] + [ CPU 기반의 일 ] ( ⇔ 번갈아 수행 )
    - ▫ I/O 기반의 일을 하면, (( CPU : Idle )) 상태로 들어감
  - (( CPU )) : ( 프로세스 스케쥴링 알고리즘 ) → 프로세스 번갈아 실행
  - 프로세스 상태
    - (1). 실행 (Runnable) : CPU 에 의해서 명령어가 실행중인 프로세스
    - (2). 준비 (Ready) : CPU 명령어 실행을 기다리는 프로세스
    - (3). 대기 (Waiting) : **I/O Operation 이 끝나기를 기다리는 프로세스**
    - (4). 종료 (Terminated) : Ctrl-Z 등의 시그널로 종료된 프로세스
  - 프로세스 순서
    -     [ New ] → ( admitted ) → [ Ready ] → ( scheduler dispatch ) → [ Running ] ( cpu 할당 ) → ( exit ) → [ Terminated ]
    -                              [ Ready ] ← ( I/O event completion ) ← [ Waiting ] ← ( I/O interrupt / event ) ← [ Running ]
    -                              [ Ready ] ← ( interrupt ) ← [ Running ]
    - ![image](https://github.com/shpark0308/c_study_develop/assets/60208434/5e7d4eb4-9f18-4707-8f53-0e833c42bf6a)
  - Zombie 프로세스
    - 프로세스는 ( 부모 프로세스 (root 프로세스) - 자식 프로세스 ) 의 형식의 **트리 구조** 이다
    - 부모 프로세스가 먼저 종료되면, root 프로세스로부터 닿을 수 없는 프로세스가 생기고, 이를 좀비 프로세스라고 부른다.
    - ( 그림 )
<br/>

#### 3️⃣ 메모리 단위
✅ 메모리 단위
- 기본 단위
  - bit < byte < KB (KiloByte) < MB < GB
    - 8bit = 1byte
    - 1000byte = 1KB
    - 1000KB = 1MB
- KiB VS KB
  - KiB ( KibiByte )
    - 1024 byte (2^10) = 1KiB
  - KB ( KiloByte )
    - 1000 byte = 1KB
- 예시
  - 1MiB = 1024KiB, 1MB = 1000KB
  - 1KiB = 1024 byte, 1KB = 1000 byte
