### Ⅰ. 네트워크 통신
#### 1️⃣ 캐스트
- [네트워크] 에서 (( 출발지 )) → (( 목적지 )) 까지 데이터 전송 방식 ( 4가지 )
![image](https://github.com/shpark0308/c_study_develop/assets/60208434/b7b61040-3f81-40c8-a8ea-47036a147206)

✅ 유니 캐스트 ( Unicast )
- 1:1 통신, 출발지와 목적지가 1:1 통신
- ex. tcp 통신, Http 통신

✅ 브로드 캐스트 ( Broadcast )
- 1:**ALL** 모든 통신, 동일 네트워크에 존재하는 **모든 호스트**가 목적지
- [사용]
  - 유니 캐스트 통신 전, 목적지의 정확한 위치 파악을 위해 사용
  - Local Network 에서, 모든 호스트에게 패킷 전달
  - ex. ARP

✅ 멀티 캐스트 ( Multicast )
- 1:N 통신, 하나의 출발지에서, (( 특정 )) 다수의 목적지로 통신
- [사용]
  - IPTV 와 같은 실시간 방송을 볼 때, 주로 사용
  - 단방향으로 **다수** 에게 동시에 같은 내용을 전달 할 때 사용
  - ex. 방송

✅ 애니 캐스트 ( Anycast )
- 1:1 통신, 하나의 출발지에서, 동일 그룹 내 가장 가까운 1개의 호스트로 통신
- IPV4 일부 구현 가능, IPV6 모두 가능

### Ⅲ. 기타
#### 1️⃣ 참조 사이트
✅ 사이트
- [캐스트] (https://rooftoproom-whale.tistory.com/14)
<br/>
