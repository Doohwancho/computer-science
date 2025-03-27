
# A. step1_sequential_server

## a-1. how to test?
step1) build c
```
g++ -c *.c
g++ echo_server.o utils.o -o main
./main
```


step2) run python test script
```
 ✘ cho-cho  ~/dev/tree/computer-science/05.network/echo-server/step1_sequential_server   main ±  python simple-client.py localhost 8080
INFO:2025-03-26 17:34:15,783:conn0 connected...
INFO:2025-03-26 17:34:15,783:conn0 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:34:15,784:conn0 received b'b'
INFO:2025-03-26 17:34:15,784:conn0 received b'cdbcuf'
INFO:2025-03-26 17:34:16,789:conn0 sending b'xyz^123'
INFO:2025-03-26 17:34:16,790:conn0 received b'234'
INFO:2025-03-26 17:34:17,795:conn0 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:34:17,795:conn0 received b'3'
INFO:2025-03-26 17:34:17,796:conn0 received b'6bc1111'
INFO:2025-03-26 17:34:18,001:conn0 disconnecting
Elapsed: 2.2252049446105957
```

Q. 여러 클라이언트를 동시에 연결하고 싶다면?
```
python simple-client.py localhost 8080 -n 5

python simple-client.py localhost 8080 -n 5
INFO:2025-03-26 17:37:02,629:conn0 connected...
INFO:2025-03-26 17:37:02,629:conn0 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:37:02,629:conn0 received b'b'
INFO:2025-03-26 17:37:02,629:conn0 received b'cdbcuf'
INFO:2025-03-26 17:37:03,635:conn0 sending b'xyz^123'
INFO:2025-03-26 17:37:03,635:conn0 received b'234'
INFO:2025-03-26 17:37:04,640:conn0 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:37:04,641:conn0 received b'36bc11'
INFO:2025-03-26 17:37:04,641:conn0 received b'11'
INFO:2025-03-26 17:37:04,844:conn0 disconnecting
INFO:2025-03-26 17:37:04,855:conn3 connected...
INFO:2025-03-26 17:37:04,855:conn3 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:37:04,855:conn3 received b'bcdbcuf'
INFO:2025-03-26 17:37:05,860:conn3 sending b'xyz^123'
INFO:2025-03-26 17:37:05,861:conn3 received b'234'
INFO:2025-03-26 17:37:06,864:conn3 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:37:06,864:conn3 received b'36bc1111'
INFO:2025-03-26 17:37:07,069:conn3 disconnecting
INFO:2025-03-26 17:37:07,077:conn4 connected...
INFO:2025-03-26 17:37:07,078:conn4 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:37:07,078:conn4 received b'bcdbcuf'
INFO:2025-03-26 17:37:08,083:conn4 sending b'xyz^123'
INFO:2025-03-26 17:37:08,083:conn4 received b'234'
INFO:2025-03-26 17:37:09,088:conn4 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:37:09,089:conn4 received b'36bc1111'
INFO:2025-03-26 17:37:09,292:conn4 disconnecting
INFO:2025-03-26 17:37:09,304:conn1 connected...
INFO:2025-03-26 17:37:09,304:conn1 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:37:09,305:conn1 received b'b'
INFO:2025-03-26 17:37:09,305:conn1 received b'cdbcuf'
INFO:2025-03-26 17:37:10,305:conn1 sending b'xyz^123'
INFO:2025-03-26 17:37:10,306:conn1 received b'234'
INFO:2025-03-26 17:37:11,308:conn1 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:37:11,308:conn1 received b'3'
INFO:2025-03-26 17:37:11,309:conn1 received b'6bc1111'
INFO:2025-03-26 17:37:11,508:conn1 disconnecting
INFO:2025-03-26 17:37:11,519:conn2 connected...
INFO:2025-03-26 17:37:11,520:conn2 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:37:11,520:conn2 received b'bcdbcuf'
INFO:2025-03-26 17:37:12,525:conn2 sending b'xyz^123'
INFO:2025-03-26 17:37:12,525:conn2 received b'234'
INFO:2025-03-26 17:37:13,527:conn2 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:37:13,528:conn2 received b'36bc'
INFO:2025-03-26 17:37:13,528:conn2 received b'1111'
INFO:2025-03-26 17:37:13,733:conn2 disconnecting
Elapsed: 11.109666109085083
```

느림.

왜?

5개의 client가 동시에 요청 걸면,\
서버가 하나의 client씩 순차처리 하거든...

로그를 보면 conn0이 처리 완료된 다음,\
conn3 -> 4 -> 1 -> 2 순으로 순차처리함.


## a-2. what

![](./images/sequential-flow.png)

여러 클라이언트에서 요청이 와도,\
하나의 클라이언트와 데이터 송수신 모두 끝날 떄 까지\
다른 클라이언트 요청 처리 안함.\
"순차적"으로 처리한다.


# B. step2_ one_thread_per_client(multi-threads)

## b-1. how to test?
```
g++ -c *.c
g++ *.o -o main
./main
```

```
 cho-cho  ~/dev/tree/computer-science/05.network/echo-server/step1_sequential_server   main ±  python simple-client.py localhost 8080 -n 3
INFO:2025-03-26 17:50:20,218:conn0 connected...
INFO:2025-03-26 17:50:20,218:conn0 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:50:20,218:conn0 received b'b'
INFO:2025-03-26 17:50:20,218:conn0 received b'cdbcuf'
INFO:2025-03-26 17:50:20,221:conn2 connected...
INFO:2025-03-26 17:50:20,221:conn2 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:50:20,221:conn2 received b'b'
INFO:2025-03-26 17:50:20,221:conn2 received b'cdbcuf'
INFO:2025-03-26 17:50:20,224:conn1 connected...
INFO:2025-03-26 17:50:20,224:conn1 sending b'^abc$de^abte$f'
INFO:2025-03-26 17:50:20,224:conn1 received b'b'
INFO:2025-03-26 17:50:20,224:conn1 received b'cdbcuf'
INFO:2025-03-26 17:50:21,223:conn0 sending b'xyz^123'
INFO:2025-03-26 17:50:21,224:conn0 received b'234'
INFO:2025-03-26 17:50:21,226:conn2 sending b'xyz^123'
INFO:2025-03-26 17:50:21,226:conn1 sending b'xyz^123'
INFO:2025-03-26 17:50:21,226:conn2 received b'234'
INFO:2025-03-26 17:50:21,226:conn1 received b'234'
INFO:2025-03-26 17:50:22,229:conn0 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:50:22,229:conn1 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:50:22,229:conn0 received b'36bc1111'
INFO:2025-03-26 17:50:22,230:conn1 received b'36bc1111'
INFO:2025-03-26 17:50:22,230:conn2 sending b'25$^ab0000$abab'
INFO:2025-03-26 17:50:22,230:conn2 received b'36bc1111'
INFO:2025-03-26 17:50:22,434:conn1 disconnecting
INFO:2025-03-26 17:50:22,435:conn0 disconnecting
INFO:2025-03-26 17:50:22,435:conn2 disconnecting
Elapsed: 2.2238399982452393
```


## b-2. what

![](./images/concurrent-flow.png)

client 1개당 쓰레드 1개 할당하는 것.

이전 step1_sequential_server와 비교하면,
![](./images/sequential-flow.png)

더 빨라졌다!

Q. 이전 step1_sequential_server 대비 어느 점이 개선되었지?

1. 여러 클라이언트 동시에 처리 가능함. 한 클라이언트가 데이터 보내는 동안 다른 클라이언트도 처리 가능함
2. 클라이언트 수가 많아져도 서버 성능이 매우 나빠지지 않음. (어느 정도까지는)

Q. cons?

1. client 갯수가 엄청 많아짐 -> thread 많아짐 -> 메모리 많이 먹고 context switching cost 늘어남
2. DoS 공격에 취약함. 악의적인 클라이언트가 연결만 하고 데이터는 안 보내면 쓰레드만 개많이 생성되서 서버 리소스 다 먹음
3. CPU 연산 많은 작업 처리하면 더 쉽게 서버 터짐


# C. step3_blocking io
## c-1. how to test?
```
g++ -c *.c
g++ *.o -o main
./main
```

## c-2. what
전체 flow 
1. accept()로 클라이언트 연결 받음
2. recv()로 데이터 기다림
3. 데이터 오면 처리하고 다시 recv() 호출

여기서 핵심은 recv()가 블로킹 함수라는 거임. 

이 함수 호출하면 클라이언트가 데이터 보낼 때까지 그냥 기다림.\
데이터 안 오면 그냥 CPU 반납하고 쓰레드는 잠들어 있음.\
데이터 오면 그때 깨어나서 처리함.

이 방식의 문제점은 한 클라이언트 기다리는 동안 다른 클라이언트는 처리 못한다는 거임.\
그래서 sequential server에서 봤던 것처럼 비효율적임.


## c-3. 테스트 결과 해석
서버 실행하고,\
터미널 2개 켜서\
터미널 1에서 
```
nc localhost 8080
hello
whatup
asdflas;fdkja;sdfjk
```

이렇게 보내면,

서버에서는 
```
./main
Listening on port 8080
peer (localhost, 60158) connected
Calling recv...
recv returned 6 bytes
Calling recv...
recv returned 7 bytes
Calling recv...
recv returned 20 bytes
Calling recv...
```

이렇게 input string bytes를 프린트 함.

근데 두번쨰 터미널을 열어서

```
nc localhost 8080
asdfasdfasdfasdf
asdfasdf
alfdkjasfdkjasfdkjas;dfkj
```

아무리 쳐도, 서버에서는 bytes print 안한다.

왜?

blocking i/o라, 첫번째 터미널에서 보내는게 끊기지 않은 이상,\
두번째 터미널에서 보내는게 안받아짐.\
첫번째 터미널에서 ctrl + c로 나가면,\
`recv returns 0 when the peer has disconnected`\
하면서 서버가 닫힘. 

## c-4. pros 
- blocking I/O is usually very efficient on resource usage while the thread is waiting
- thread is put to sleep by the OS and only wakes up when whatever it was waiting for is available.





# D. step4 non blocking io (polling)

## d-1. how to test?

## d-2. what

두 번째 코드는 non-blocking I/O를 사용한 서버임:

1. accept()로 클라이언트 연결 받음
2. 소켓을 non-blocking 모드로 설정함 (fcntl(newsockfd, F_SETFL, flags | O_NONBLOCK))
3. recv()로 데이터 확인
    - blocking과 다른 점은, always return very fast, even if there's no data to receive.
4. 데이터 없으면 EAGAIN 또는 EWOULDBLOCK 에러 발생하고, 잠깐 쉬었다가 다시 확인
    - 0.2초마다 계속 polling 하는 듯 

여기서 핵심은 recv()가 논블로킹 함수라는 거임. \
데이터 없어도 바로 리턴함. \
대신 특별한 에러 코드(EAGAIN, EWOULDBLOCK)로 "지금은 데이터 없다"고 알려줌.

이 방식의 장점은 데이터 기다리는 동안 다른 일 할 수 있다는 거임. \
근데 이 코드에서는 그냥 usleep(200 * 1000)으로 쉬고 있음. \
이건 그냥 예시용이고, 실제로는 이 시간에 다른 클라이언트 처리할 수 있음.


## d-3. cons of "polling"
근데 이 non-blocking I/O (polling) 방식도 문제가 있음. \
계속 "데이터 왔냐?" 물어보는거임. \
이거 존나 비효율적임 \

1. 클라이언트 1000개 있으면 1000번 recv() 호출해야 함
2. 대부분 "데이터 없음" 응답 받음 (CPU 낭비)
3. 폴링 주기 짧게 하면 CPU 더 낭비하고, 길게 하면 응답 지연됨

특히 scale out했을 때 효율이 별로임.\
소켓 10000만개가 10000client과 통신한다고 하고,\
200ms주기로 10000번씩 polling으로 체크하는게 낭비.

200ms주기보다 더 짧게 잡자니, CPU usage를 너무 많이 쓰고,\
주기를 너무 길게 잡자니, latency가 너무 느려짐.




# E. step5 I/O multi-plexing 

## c-1. how to test?
```
g++ -c *.c
g++ *.o -o main
./main
```

```
python simple-client.py -n 3 localhost 8080
```


## c-2. what

![](./images/multiplexed-flow.png)

non-blocking 방식에서 이런 문제점들이 있었잖아?

1. 클라이언트 1000개 있으면 1000번 recv() 호출해야 함
2. 대부분 "데이터 없음" 응답 받음 (CPU 낭비)
3. 폴링 주기 짧게 하면 CPU 더 낭비하고, 길게 하면 응답 지연됨

이 문제 해결하려면 I/O 멀티플렉싱이 필요함.\
이건 OS에게 "이 소켓들 중에 데이터 있는 놈 알려줘"라고 요청하는 방식임.

select는 I/O multiplexing api (select is for synchronous multiplexing)

select()는 여러 소켓을 모니터링하다가 어떤 소켓에 데이터가 도착하면 알려줌. 이러면 폴링처럼 모든 소켓 다 확인할 필요 없이, 데이터 있는 소켓만 처리할 수 있음.

여러 client와 동시에 연결되었지만, 실질적으로는 1번에 1가지씩 처리함 (single-threaded)

event driven 구조의 prototype? 느낌 

1. accept()로 여러 client와 연결한다.
2. 모든 요청은 main loop에서 single thread가 select로 처리한다.

example
```
// select() 사용 예시
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(socket1, &readfds);
FD_SET(socket2, &readfds);
// ... 더 많은 소켓 추가 가능

// 소켓 중 하나라도 데이터 있을 때까지 기다림
select(max_socket + 1, &readfds, NULL, NULL, NULL);

// 이제 readfds에는 데이터 있는 소켓만 남아있음
```

이런 방식으로 하면 한 쓰레드로도 여러 클라이언트 동시에 처리 가능함. 폴링보다 훨씬 효율적이고, 쓰레드 여러개 만드는 것보다 리소스 덜 씀.

결론적으로 다음 스텝은 select(), poll(), epoll() 같은 I/O 멀티플렉싱 API를 사용해서 이벤트 드리븐 방식으로 서버 만드는 거임.\
이게 현대적인 고성능 서버의 기본 아키텍처임.


## c-3. cons 

Q. select의 한계점 
1. FD_SETSIZE 제한 
    - select는 감시할 수 있는 파일 디스크립터 개수가 제한되어 있음. 보통 1024개임.
2. O(N) complexity 
    - 감시하는 파일 디스크립터 개수가 늘어날수록 성능 개떨어짐. scale out해서 연결된 client 수가 늘어날 수록 처리효율이 느려짐.
3. 매번 초기화 해야 함 
    - select 호출할 때마다 감시할 파일 디스크립터 셋을 다시 만들어야 함. = 비효율
4. 반환값 해석 개고생
    - 어떤 파일 디스크립터에 이벤트가 발생했는지 알려면 전체 셋을 다 뒤져봐야 함. 존나 비효율적임


# F. step6 epoll
## f-1. test 
step1) build c
```
g++ -c *.c
g++ *.o -o main
./main
```

step2) run python test script
```
python simple-client.py -n 3 localhost 8080
```

## f-2. what 
select의 단점들을 epoll()은 이렇게 해결했음.

1. 제한 없음:
    - 감시할 수 있는 파일 디스크립터 개수에 제한 없음. 메모리 있는 만큼 넣어도 됨.
2. O(1) 복잡도:
    - 파일 디스크립터 개수 늘어나도 성능 그대로임. 매우 빠름.
3. 한 번만 등록:
    - 파일 디스크립터 한 번만 등록하면 됨. 매번 초기화할 필요 없음.
4. 이벤트 발생한 것만 알려줌:
    - epoll_wait하면 이벤트 발생한 파일 디스크립터만 알려줌. 전체 다 뒤질 필요 없음 ㅇㅇ.
5. 엣지 트리거 지원:
    - 상태 변화 있을 때만 알려주는 엣지 트리거 모드 지원함. 더 효율적으로 처리 가능.


## f-3. 원본에 epoll() -> kqueue()로 변환
arm64 아키텍처는 epoll 못씀. kqueue 써야함.

원본: https://github.com/eliben/code-for-blog/blob/main/2017/async-socket-server/epoll-server.c


# G. step7 libuv

## g-1. test 
step1) install libuv on mac m1
```
brew install libuv
find /opt/homebrew -name "uv.h"
```

step2) compile & run server 
```
gcc -I/opt/homebrew/include -L/opt/homebrew/lib *.c -luv -o main
./main
```

step3) test 
```
python simple-client.py -n 3 localhost 8080
```



## g-2. what 
Q. what is libuv?

A. libuv는 Node.js 만들 때 쓰려고 개발된 라이브러리인데, 이벤트 루프 추상화해서 플랫폼 독립적으로 만든거임. \
내부적으로 최적의 이벤트 루프 구현체인 select, epoll()같은걸 선택해줌. (리눅스면 epoll, 맥이면 kqueue) \
개발자는 콜백만 등록하고 루프 돌리면 됨.

Q. epoll()의 단점을 어떻게 개선했을까?

A. epoll()이 플랫폼 의존적이고 API가 복잡하다는걸 좀 단순하게 바꿨다는 점을 제외하고,\
성능적으로 개선점은 

epoll은 블로킹 연산 만나면 이벤트 루프 전체가 멈추는 크리티컬한 문제가 있는데, \
libuv는 내부적으로 쓰레드풀 써서 블로킹 연산 알아서 처리해줌.\
파일 I/O 같은 블로킹 연산도 비동기 API로 제공함.

libuv의 코드 구조는 대략 다음과 같음. 
```c
// 기본 이벤트 루프 생성
uv_loop_t* loop = uv_default_loop();

// 콜백 등록
uv_listen((uv_stream_t*)&server_stream, N_BACKLOG, on_peer_connected);

// 이벤트 루프 실행
uv_run(loop, UV_RUN_DEFAULT);
```
이런 식으로 콜백 등록하고 이벤트 루프 돌리면 알아서 다 처리해줌. 

libuv는 uv_queue_work() 함수로 블로킹 연산을 쓰레드풀에 던져서 처리함.\
이러면 이벤트 루프 안 멈추고 계속 돌아감.\
예를 들어:

```c
uv_work_t* work_req = malloc(sizeof(*work_req));
uv_queue_work(loop, work_req, on_work, on_after_work);
```
이렇게 하면 on_work는 쓰레드풀에서 실행되고, 끝나면 on_after_work가 메인 쓰레드에서 호출됨.


예를들어,
```c
void on_timer(uv_timer_t* timer) {
    // 이런 식으로 sleep() 호출하면 전체 이벤트 루프 멈춤
    if (random() % 5 == 0) {
        printf("Sleeping...\n");
        sleep(3);
    }
}
```
이런 3초씩 걸리는 task가 event queue에 끼워들면 병목걸려서 전체 성능 내려가는데,\
얘를 on_work에 쓰데르풀에서 따로 처리한다는 것.\
이런식으로 
```c
void on_timer(uv_timer_t* timer) {
    // 이렇게 하면 메인 쓰레드 안 멈춤
    uv_work_t* work_req = malloc(sizeof(*work_req));
    uv_queue_work(loop, work_req, on_work, on_after_work);
}
```
on_work는 쓰레드 풀에서 실행되고, on_after_work는 작업 끝나면 메인 쓰레드에서 호출됨.

## g-3. heavy task(소수 계산) 예시 데모 
소수 판별하는 서버 예제 만들었는데, 큰 수 테스트하면 시간 오래 걸림.\
이걸 메인 쓰레드에서 처리하면 다른 클라이언트 응답 못하고, 쓰레드 풀에서 처리하면 동시에 여러 클라이언트 처리 가능함.


## g-4. libuv가 쓴 최적화 방법?
Q. epoll() 말고 어떤 최적화 방법을 쓴거지?

- libuv는 파일 시스템 작업도 비동기 API로 제공함. 내부적으로 쓰레드 풀 써서 블로킹 POSIX API를 비동기처럼 보이게 만듦.
- uv_fs_read() 같은 함수는 내부적으로 uv__work_submit()을 호출해서 쓰레드 풀에 작업 던짐. 이러면 파일 읽는 동안 메인 쓰레드 안 멈춤.
- Node.js의 fs.readFile이 이 방식으로 구현되어 있음. 그래서 Node.js에서 파일 읽기가 논블로킹으로 동작함.

