
ⅠⅡⅢ1️⃣2️⃣3️⃣✅▪🔯

### Ⅰ. GPU
#### 1️⃣ GPU 메모리
```
Every 0.5s: nvidia-smi dmon -s pum -c 1
# gpu    pwr  gtemp  mtemp     sm    mem    enc    dec    jpg    ofa     fb   bar1   ccpm
# Idx      W      C      C      %      %      %      %      %      %     MB     MB     MB
    0     29     46      -     32     13     29      0      -      -   4297     11      0
    1     31     45      -     31     13     28      0      -      -   4276     11      0
    2     25     44      -     28     12     29      0      -      -   4281     11      0
    3     29     50      -     27     13     27      0      -      -   4289     10      0
```

✅ FB
> ▪ Frame Buffer
> ▪ 디스플레이
- 디스플레이에 표시되는 이미지를 저장 및 업데이트 하는 **버퍼**
- 각 픽셀의 색상 정보를 저장
- 디스플레이에 표시되는 이미지는, 해당 **프레임 버퍼**에 저장된 이미지 데이터를 기반으로 생성

✅ VRAM
> ▪ Video RAM
- 그래픽 카드에 탑재된 메모리의 일종
- 디스플레이에 표시되는 이미지
- **높은 대역폭과 빠른 속도**를 가지는 특별한 형태의 **RAM**
- 그래픽 처리에 필요한 **대용량의 데이터를 빠르게 읽고 쓸 수 있도록 설계**

✅ Memory
> ▪ 그래픽 메모리
> ▪ 그래픽 처리
- GPU 에서 사용되는 메모리
- FB / VRAM 을 포함하여, 그래픽 처리 작업 ( Texture, Vertex 정보, Shader ) 에 필요한 모든 데이터를 저장
- 일반적으로, Graphic Memory 와 VRAM 을 같은 것을 가리키는 용어로 사용

🔯 선후 관계
- FB ∈ VRAM ∈ Graphic Memory 

