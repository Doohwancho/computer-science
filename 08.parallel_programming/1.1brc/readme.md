# Takeaway

1. `disk i/o`가 병목지점이라고 늘상 들었는데 오해였다.
    - [컴퓨터가 발전하면서 disk i/o latency가 크게 줄었다.](https://colin-scott.github.io/personal_website/research/interactive_latency.html)
    - [실제 disk i/o benchmark한 것](https://benhoyt.com/writings/io-is-no-longer-the-bottleneck/)을 보면 꽤나 빠르다.
2. 진짜 병목지점은 다음과 같았다.
    1. single thread -> multi threading(context switching cost from switching between user mode and kernel mode) -> green thread
    2. multi thread에 .parallel() 할거면, 각 쓰레드마다 얼마나 큰 혹은 작은 task를 줘야할까? 그 지점.
    3. disk -> jvm heap / jvm native memory space 가져올 때 데이터를 버퍼에 2번씩 복사하는 것이나, (mmap)
    4. parsing을 효율적이게 하는 것. 데이터에 꼭 맞는 자료구조와 parsing 방법
    5. cpu 수준의 최적화: branchless programming이라던지, SWAR로 ';'찾는다던지, char을 long으로 바꿔서 bitmap 연산한다던지
    6. 데이터에 꼭 맞는 최적화된 커스텀 자료구조 사용, 범용자료구조에서의 validaction check단계 skip, 불필요한 메모리 생성 -> gc 해제(공유 객체 사용)

# Concepts
- java
    1. create 1 billion rows text file :white_check_mark:
    2. baseline :white_check_mark:
    3. parallel :white_check_mark:
    4. memory allocation: memory mapping :white_check_mark:
    5. memory allocation: direct buffer :white_check_mark:
    6. chunking :white_check_mark:
    7. parsing: string :white_check_mark:
    8. parsing: double :white_check_mark:
    9. cpu optimization :white_check_mark:

