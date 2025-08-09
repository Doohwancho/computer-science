# summary 
## A. 순차 처리 서버 (Sequential Server)
- 개념: 한 번에 클라이언트 하나만 순서대로 처리하는 가장 기본적인 서버 모델입니다.
- 테스트 결과: 클라이언트 5개가 동시에 접속을 요청하자, conn0의 작업이 완전히 끝나야 conn3가 연결되는 등 순차적으로 처리되어 전체 작업 시간이 매우 길어졌습니다.
- 문제점: 하나의 요청 처리가 끝날 때까지 다른 모든 요청은 대기해야 하므로 효율이 매우 낮습니다.

## B. 클라이언트당 스레드 1개 할당 (Multi-Thread)
- 개념: 클라이언트가 연결될 때마다 전담 스레드를 하나씩 새로 만들어주는 방식입니다.
- 개선점: 여러 클라이언트의 요청이 동시에 처리되어 순차 모델보다 훨씬 빨라졌습니다.
- 단점:
    - 리소스 낭비: 클라이언트가 많아질수록 스레드도 늘어나 메모리 사용량과 컨텍스트 스위칭 비용이 급증합니다.
    - DoS 공격 취약: 악의적 클라이언트가 연결만 유지해도 스레드 자원이 고갈될 수 있습니다.

## C. 블로킹 I/O (Blocking I/O)
- 개념: recv() 같은 함수를 호출하면, 데이터가 들어올 때까지 스레드가 잠들어(blocked) 대기하는 방식입니다.
- 특징: 스레드가 대기하는 동안에는 CPU 자원을 OS에 반납하므로 효율적이지만, 해당 스레드는 다른 어떤 작업도 할 수 없습니다.
- 문제점: 한 클라이언트의 데이터를 기다리는 동안 다른 클라이언트를 전혀 처리할 수 없어 순차 처리 서버와 같은 비효율이 발생합니다.

## D. 논블로킹 I/O (Non-Blocking I/O) + 폴링
- 개념: 소켓을 논블로킹 모드로 설정해 recv()를 호출해도 즉시 리턴되게 하는 방식입니다. 데이터가 없으면 "데이터 없음(EAGAIN)"이라는 에러를 받고 넘어갑니다.
- 동작: "데이터 왔어?"라고 주기적으로 계속 물어보는(polling) 방식으로 동작합니다.
- 단점:
    - CPU 낭비: 대부분의 폴링은 "데이터 없음"이라는 응답만 받게 되므로 CPU 자원을 낭비합니다.
    - 비효율적 확장: 클라이언트 수가 늘어나면(scale out) 물어봐야 할 소켓도 늘어나 낭비가 극심해집니다.
    - 딜레마: 폴링 주기를 짧게 하면 CPU 사용량이 폭증하고, 길게 하면 응답이 늦어집니다.

## E. I/O 멀티플렉싱 (I/O Multiplexing) - select
- 개념: 폴링의 비효율을 해결하기 위해, 여러 소켓을 OS에 등록하고 **"이 중에 데이터 오면 알려줘"** 라고 요청하는 방식입니다.
- 동작: select() API를 사용하며, 이벤트가 발생한 소켓만 처리할 수 있어 효율적입니다. 싱글 스레드로 동시성(concurrency)을 구현하는 이벤트 기반(event-driven) 구조의 원형입니다.
- select의 한계:
    - 개수 제한: 감시할 수 있는 소켓 개수가 1024개로 제한됩니다.
    - 성능 저하: 감시하는 소켓이 많아질수록 성능이 선형적으로(O(N)) 느려집니다. (딱 1명 끝나도 명단에 1024개 다 훑어봄)
    - 반복 작업: 호출할 때마다 매번 소켓 목록을 다시 설정해야 합니다. (한번 os가 스레드한테 일끝났다 알려줄 때마다, 저 1024개 명단 재작성해서 줘야 함) 
- 이런 select 한계 극복하기 위해 나온게 epoll(linux) / kqueue(mac)임 
- 그러니까 클라이언트로부터 온 요청 024개가 있으면, 쓰레드 한개가 요청 받아서 소켓 1024개 on os 레벨 과 소통한다는거지? 이 소켓 1024개 명단을 os한테 주고, 일 끝나면 나한테 (single thread)알려줘! 근데 단점은 os가 요청 끝나서 알려줄 때마다 저 1024개 명단 재작성해야 해서 비효율적인거고? ㅇㅇ 

## F. epoll(linux) / kqueue(mac) / icop(window)
- 개념: select의 단점들을 해결한 리눅스의 고성능 I/O 멀티플렉싱 API입니다. (macOS에서는 kqueue가 동일한 역할을 수행합니다.)
- 스레드는 자신이 직접 모든 소켓을 감시하는 게 아니라, 감시 업무를 OS 커널에 위임하고 결과만 통보받기 때문에 혼자서도 수많은 소켓을 효율적으로 처리할 수 있습니다.
- in linux, everything is file. 
    - 리눅스는 키보드, 모니터, 파일, 네트워크 연결 등 모든 자원을 일관된 방식으로 다루고 싶어 합니다. 그 방식이 바로 **'파일'** 처럼 취급하는 것
    - 프로그램을 실행해서 파일 열기, 네트워크 연결(소켓 생성) 등을 요청하면, 리눅스 커널은 그 자원에 정수 번호표를 하나 붙여줍니다. 이것이 **파일 디스크립터(FD)** 입니다. 0번(표준 입력), 1번(표준 출력), 2번(표준 에러)는 기본으로 할당되고, 이후로는 3, 4, 5... 순서로 번호가 붙습니다.
        - 프로그램(스레드) 입장에서는 이게 진짜 파일인지, 네트워크 연결(소켓)인지, 아니면 프린터인지 구분할 필요가 없습니다. 그냥 OS로부터 받은 3번 FD에 데이터를 쓰거나(write), 4번 FD에서 데이터를 읽어오는(read) 작업을 할 뿐입니다. "3번 손님, 4번 손님" 하고 번호표로 부르는 것과 같습니다.
        - 스레드 입장에서는 `read_from_file()`이나 `read_from_socket()`같은 구분지어서 나눠처리하는게 아니라 그냥 심플하게 `read()` `write()`로 `syscall`하면, 운영체제 kernel이 저 요청을 받아 file_descriptor_id 까보고 아 이건 하드디스크에 있는 test.txt 읽기구나, 혹은 1.2.3.4 ip랑 연결된 tcp 소켓이네? 그럼 네트워크 카드 버퍼에 있는 데이터를 가져와서 전달해야겠다. 하고 처리함. 
- epoll의 장점:
    - 개수 제한 없음: 메모리가 허용하는 한 무제한으로 소켓을 감시할 수 있습니다. (select처럼 소켓 1024개 감시 제한 없음)
    - O(1) 성능: 소켓 개수가 늘어나도 성능이 거의 일정합니다. (select처럼 소켓 매번 1024개 O(N)작업 안함)
    - 효율적 관리: 소켓을 한 번만 등록하면 되며, 이벤트가 발생한 소켓만 정확히 알려줘 불필요한 반복이 없습니다. (select처럼 매번 일 끝났다고 os가 알려줄 떄마다 소켓 1024개 명단 재작성해서 주는거 없음) 

### f-1. in linux, everything is file 
#### case1) hello.txt 파일에서 데이터 읽기
```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    char buffer[100];

    // 1. 파일을 열고, OS로부터 파일 디스크립터(번호표)를 받음
    int file_fd = open("hello.txt", O_RDONLY);
    if (file_fd == -1) {
        perror("open");
        return 1;
    }
    printf("hello.txt opened. File Descriptor: %d\n", file_fd);

    // 2. 받은 번호표를 이용해 데이터를 읽어달라고 OS에 요청
    read(file_fd, buffer, sizeof(buffer));

    printf("Read from file: %s\n", buffer);
    close(file_fd);
    return 0;
}
```


#### case2) 네트워크 소켓에서 데이터 읽기 
```c
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
// ... (accept를 위한 다른 헤더들 포함)

void handle_client(int socket_fd) {
    char buffer[100];
    
    printf("Client connected. Socket Descriptor: %d\n", socket_fd);

    // 2. 받은 번호표를 이용해 데이터를 읽어달라고 OS에 요청
    read(socket_fd, buffer, sizeof(buffer));

    printf("Read from socket: %s\n", buffer);
    close(socket_fd);
}
```
두 코드의 read() 함수 호출 부분을 보세요.

- read(file_fd, buffer, sizeof(buffer));
- read(socket_fd, buffer, sizeof(buffer));

완벽하게 똑같습니다.

### f-2. multiplexing 
멀티플렉싱(Multiplexing)은 아주 간단히 말해 **'여러 개를 하나로 합쳐서 보내고, 받은 쪽에서 다시 여러 개로 나누는 기술'** 을 말합니다 

핵심은 **'하나의 통로(채널)를 여러 사용자가 시간 차를 두고 함께 써서 효율을 높이는 것'** 입니다. 고속도로의 여러 차선에 차들이 동시에 달리는 모습을 생각하면 쉽습니다.

지금 보신 I/O 멀티플렉싱과 예전에 들으셨던 HTTP/2 멀티플렉싱은 이 기본 원리를 각기 다른 레벨에 적용한 것입니다.

여기서의 멀티플렉싱은 **'하나의 스레드가 여러 클라이언트(소켓)를 동시에 처리하는 기술'** 을 의미합니다.

- 통로(채널): 서버의 프로세스 또는 스레드 (일을 하는 주체)
- 여러 데이터: 수많은 클라이언트의 연결(소켓)

- http1.1 -> http2.0 multiplexing도 같은 원리.
    - http1.1은 패킷단위로 쪼갬(L3), 1.5kb 그리고 받는쪽에서 재정렬해서 씀 
    - http2.0은 패킷보다 더 세밀한 단위인 frame 단위로 쪼갬(L2) 
    - 요청(css, js, image file, etc)을 frame단위로 분해하고 stream_id 붙이고, 이건 거대한 tcp 연결에 실어 보냄. 
받는 쪽은 이 frame 단위로 쪼개진 stream_id를 보고 순서대로 다시 재정렬함. 


## G. libuv
- 개념: epoll, kqueue 등을 추상화하여 어떤 OS에서든 동일한 코드로 동작하게 해주는 고수준 비동기 I/O 라이브러리입니다. (Node.js의 핵심)
- 핵심 개선점: epoll만으로는 해결이 어려운 파일 I/O나 무거운 계산 작업(heavy task)을 내부적인 스레드 풀(Thread Pool)로 보내 처리합니다. 이를 통해 메인 이벤트 루프가 멈추는 현상을 방지하고 서버 전체의 응답성을 유지합니다.
    - epoll은 가벼운 socket통신같은거 감시는 양이 많아도 기가막히게 잘하는데, 갑자기 cpu-intensive task 오면 이벤트루프에서 하던거 못하고 저 일하게 되는데 이걸 'event loop blocking'이라고 함 
    - 그래서 저런 cpu-intensive-request 오면 미리 준비한 thread pool에 여러 쓰레드한테 위임하는 것. 

## H. ae
- 개념: libuv의 경량 버전으로 볼 수 있는 이벤트 루프 라이브러리입니다. (Redis에서 사용)
- libuv처럼 cpu-intensive 작업만 뽑아서 스레드풀에게 위임하는 기능같은건 없고, 오직 핵심인 이벤트 처리기능에 몰빵한 놈. 
- ae는 library, epoll/kqueue는 시스템 호출 부분임. 딱 그 차이. 
- 특징: libuv처럼 OS에 따라 epoll, kqueue 등을 알아서 선택해주지만, 훨씬 가볍습니다. Redis처럼 핵심적인 이벤트 처리 기능에 집중한 라이브러리입니다.

### h-1. os별로 epoll, kqueue, select 쓸지 분기처리 

```c
/* ae.c - A simple event loop implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * ... (라이선스 정보) ...
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "ae.h"

/* OS에 따라 사용할 I/O 멀티플렉싱 기술을 여기서 결정합니다. */
#ifdef HAVE_EVPORT
    #include "ae_evport.c" // 솔라리스의 evport
#else
    #ifdef HAVE_EPOLL
        #include "ae_epoll.c" // 리눅스의 epoll
    #else
        #ifdef HAVE_KQUEUE
            #include "ae_kqueue.c" // macOS/BSD의 kqueue
        #else
            #include "ae_select.c" // 위 기술이 모두 없을 때 사용하는 select
        #endif
    #endif
#endif

/* --- 이하 공통 API 함수들 --- */

// ... aeCreateEventLoop, aeDeleteEventLoop 등의 함수 구현 ...
```

OS에 가장 효율적인 I/O 멀티플렉싱 기술을 선택

### h-2. ae.c 주요함수 
- aeCreateEventLoop(): 이벤트 루프 구조체를 생성하고 초기화합니다. 이 과정에서 위에 설명한 OS별 최적의 I/O 멀티플렉싱 메커니즘(epoll_create, kqueue 등)을 준비합니다.
- aeDeleteEventLoop(): 이벤트 루프를 삭제하고 자원을 해제합니다.
- aeCreateFileEvent(): 가장 중요한 함수 중 하나로, 특정 파일 디스크립터(소켓)에 대해 어떤 이벤트(읽기 가능, 쓰기 가능)를 감시할지 등록합니다.
- aeDeleteFileEvent(): 등록했던 파일 이벤트 감시를 해제합니다.
- aeMain(): 이벤트 루프를 시작합니다. 내부적으로 무한 루프를 돌면서 aeProcessEvents를 계속 호출합니다.
- aeProcessEvents(): 실제 심장 역할을 하는 함수로, 등록된 이벤트 중 발생한 것이 있는지 OS에 물어보고(예: epoll_wait 호출), 발생한 이벤트가 있다면 그에 맞는 콜백 함수를 실행합니다.


### h-3. epoll에 핵심 함수 
ae_epoll.c 파일 안에 있는 aeApiPoll 함수는 aeProcessEvents의 핵심 로직입니다. OS에 "이벤트 온 거 있니?"라고 물어보는 부분이죠.

```c
// ae_epoll.c 파일의 내용

// ... (다른 함수들) ...

static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) {
    aeApiState *state = eventLoop->apidata; // epoll 관련 정보 (epoll fd 등)
    int retval, numevents = 0;

    // 1. epoll_wait()을 호출해서 이벤트가 발생할 때까지 대기합니다.
    //    tvp 시간만큼만 기다리거나, NULL이면 영원히 기다립니다.
    retval = epoll_wait(state->epfd, state->events, eventLoop->setsize,
            tvp ? (tvp->tv_sec*1000 + tvp->tv_usec/1000) : -1);

    // 2. 발생한 이벤트가 있다면(retval > 0) 처리합니다.
    if (retval > 0) {
        numevents = retval;
        for (int j = 0; j < numevents; j++) {
            int mask = 0;
            struct epoll_event *e = state->events+j;

            // 3. 발생한 이벤트의 종류(읽기/쓰기)를 확인합니다.
            if (e->events & EPOLLIN) mask |= AE_READABLE;
            if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
            
            // 4. 처리할 이벤트 목록(fired)에 파일 디스크립터와 이벤트 종류를 저장합니다.
            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }

    // 5. 발생한 이벤트의 개수를 반환합니다.
    return numevents;
}
```

보시다시피, 코드 자체는 매우 직설적입니다. epoll_wait을 호출해서 OS로부터 이벤트 목록을 받고, 그 목록을 순회하며 eventLoop->fired 배열에 채워 넣는 것이 전부입니다. 이 fired 배열은 나중에 등록된 콜백 함수를 호출하는 데 사용됩니다. 복잡한 로직 없이 OS의 기능을 그대로 활용하기 때문에 가볍고 빠를 수밖에 없는 구조입니다.

### h-4. single main thread event queue 구조에서 오래걸리는 요청이 오면 병목이 어떻게 발생하는가?

#### case1) 일반적인 상황  
```c
// Redis의 메인 이벤트 루프. 서버가 켜져 있는 동안 계속 반복됩니다.
void aeMain(aeEventLoop *eventLoop) {
    while (!eventLoop->stop) {
        // 1. OS에게 "처리할 이벤트 있니?"라고 물어보고 대기합니다. (epoll_wait)
        //    - 새로운 클라이언트 접속
        //    - 클라이언트가 보낸 데이터 도착 등
        //    이벤트가 없으면 여기서 잠시 대기합니다. (CPU 낭비 없음)
        aeProcessEvents(eventLoop);
    }
}

// 발생한 이벤트를 실제로 처리하는 함수
void aeProcessEvents(aeEventLoop *eventLoop) {
    // ... epoll_wait()으로 이벤트가 발생한 소켓(fd) 목록을 가져옴 ...

    for (int j = 0; j < numevents; j++) {
        int fd = eventLoop->fired[j].fd;

        // 2. 발생한 이벤트의 종류에 따라 등록된 콜백 함수를 실행합니다.
        //    예: 클라이언트가 데이터를 보냈다면 'readQueryFromClient' 함수 실행
        if (eventLoop->fired[j].mask & AE_READABLE) {
            readQueryFromClient(fd);
        }
    }
}

// 클라이언트의 요청을 읽고 커맨드를 처리하는 콜백 함수
void readQueryFromClient(int fd) {
    // 1. 소켓(fd)에서 "GET mykey" 같은 명령어를 읽음
    // 2. 읽은 명령어를 파싱해서 실행할 함수를 찾음 (예: getCommand)
    // 3. 해당 커맨드 함수를 실행
    getCommand(client); // 이 함수는 매우 빠릅니다. (보통 0.0001초 미만)
    
    // 4. 처리 결과를 클라이언트에게 전송
}
```
정상 상황의 흐름:

1. aeMain 루프가 aeProcessEvents를 호출합니다.
2. epoll_wait이 깨어나 "A 클라이언트가 GET 명령 보냄!"이라고 알려줍니다.
3. readQueryFromClient 콜백 함수가 실행됩니다.
4. getCommand는 메모리에서 값을 찾아 바로 리턴하므로 순식간에 끝납니다.

루프는 즉시 다음으로 돌아가 다른 클라이언트의 요청을 처리할 준비를 합니다.\
모든 과정이 마이크로초(µs) 단위로 끝나기 때문에, 싱글 스레드임에도 수만 개의 요청을 빠르게 처리할 수 있습니다.


#### case2) 병목 발생: CPU 연산이 긴 명령어가 들어왔을 때

```c
// ... 위와 동일 ...

// readQueryFromClient에서 호출되는 커맨드 처리기
void processCommand(client *c) {
    // ... 명령어 파싱 ...

    // 사용자가 보낸 명령어가 CPU를 많이 잡아먹는 'EVAL_SLOW_SCRIPT'라고 가정
    if (strcmp(c->argv[0]->ptr, "EVAL_SLOW_SCRIPT") == 0) {
        
        // =========================================================
        // === 여기가 바로 병목 지점 (BOTTLENECK) ===
        // =========================================================
        
        // 이 함수가 5초 동안 CPU를 100% 사용하며 복잡한 연산을 수행합니다.
        execute_slow_script(c); 
        
        // *** 이 함수가 끝나기 전까지, 아래 코드는 절대 실행되지 않습니다. ***
        // *** 더 중요한 것은, aeMain()의 while 루프가 다음으로 넘어가지 못하고 ***
        // *** 여기서 5초 동안 완벽하게 '정지'합니다. ***
        
    } else {
        // SET, GET 등 다른 빠른 명령어 처리
        ...
    }
}

// 5초 동안 CPU를 사용하는 가상의 함수
void execute_slow_script(client *c) {
    // 예: for (long i = 0; i < 999,999,999; i++) { ... 복잡한 계산 ... }
    long long startTime = ustime();
    while(1) {
        if (ustime() - startTime > 5000000) { // 5초 경과 체크
            break;
        }
    }
    printf("5초 걸리는 작업 완료!\n");
}
```

병목 발생 시나리오:

1. B 클라이언트가 EVAL_SLOW_SCRIPT 명령어를 Redis에 보냅니다.
2. aeMain 루프가 이벤트를 감지하고 readQueryFromClient를 거쳐 processCommand를 호출합니다.
3. processCommand 내부에서 execute_slow_script() 함수가 호출됩니다.
4. 스레드는 이 함수 안에 5초 동안 갇힙니다. CPU는 100% 사용되며 반복문을 실행합니다.
5. 이 5초 동안, aeMain 루프는 완전히 멈춰 있습니다.
    - 새로운 클라이언트가 접속을 시도해도 응답이 없습니다. (accept 불가)
    - 다른 클라이언트가 GET 같은 간단한 명령을 보내도 읽지 못합니다. (read 불가)
    - 모든 클라이언트 입장에서 Redis는 5초 동안 '죽은' 상태가 됩니다.
6. 5초 후 execute_slow_script()가 끝나고 나서야 스레드는 루프로 복귀하고, 그제서야 밀려 있던 다른 요청들을 처리하기 시작합니다.


#### Q. cpu-intensive work는 메인스레드가 아니라 kernel-level thread가 대신 처리하는거 아냐?
os kernel 스레드는 하드웨어와 관련된 느린 작업을 대신 처리하고 결과를 알려주는 심부름꾼에 가깝다. 

- example 
    - I/O 대기: 네트워크로부터 데이터가 도착하거나, 디스크에 파일 쓰기가 완료될 때까지 기다리는 일을 대신 해줍니다. 스레드가 멍하니 기다리며 시간을 낭비하지 않게 해주는 것이죠. (이것이 멀티플렉싱의 핵심입니다.)
    - 데이터 전송: 스레드가 "이 데이터 좀 디스크에 써줘"라고 명령하면, 실제로 하드디스크에 데이터를 기록하는 물리적인 작업을 수행합니다.

하지만, OS 커널은 당신의 프로그램 로직을 전혀 이해하지 못합니다. "클라이언트가 보낸 데이터가 '1'이면 '2'를 더하고, '3'이면 '4'를 곱해라" 같은 계산은 커널의 역할이 아닙니다. 

그래서 user level thread는 이 비즈니스 로직. ex) 파일에서 읽어온 데이터에 2를 더하고 4를 곱해라 같은거를 처리함. 

#### insight) heavy한 데이터 연산은 single main thread event queue 서버에 시키지 마라. 
i/o intensive server랑 cpu intensive server를 msa로 나눠서 관리하라. 

Q. 그러면 redis나 nodejs 같은 single main thread event queue인걸 쓸 땐, 뭔가 데이터를 빠르게 전송하는거, 소켓통신이나 file 입출력같은거만 시키고, 실제 데이터 받아온걸로 뭔가 연산이 필요한건 다른 서버에서 하는게 좋겠네? 
A. 맞음 

node.js / redis 같은 애들한테는 연산작업같은거 맡기지 말고, 실제 cpu 많이쓰는건 다른 db서버가, 연산용 서버에 넘겨서 처리하자


### h-5. user-level-thread & kernel-level-thread 가 cpu에 닿기까지 과정 

1. user-level-thread에 cpu 작업요청  
    - user-level-thread가 데이터 전처리든 뭐든 cpu작업을 해야해서 syscall로 요청함(? 이건 파일 i/o나 socket i/o같은 데이터 불러올 떄나 하는거고, 이 떈 그냥 바로 os kernel한테 물어보나? 어떻게? 이것도 syscall로 통신하는거 아닌가?)
2. cpu scheduling
    - kernel은 수 많은 스레드 요청중에서 지금 당장 cpu에 올릴 스레드를 우선순위 정렬해서 선정함 
3. context switching 
    - 이전 쓰레드가 cpu작업한거 잠깐 옆에 미뤄두고 새로운 스레드 작업 올리는게 context switching 
    - thread안에 program counter, register 값들을 cpu에 로드함 
4. 모드전환 
    - kernel level thread -> user-level-thread 전환이면 모드 전환. 
5. user-level-thread는 제어권을 넘겨받아 cpu에서 연산함(kernel에 개입 없이)
    - os kernel은 해당 user-level-thread에게 cpu 제어권을 넘겨줌 
    - a + b를 계산하거나, for 루프를 돌거나, 문자열을 붙이는 등의 모든 순수 CPU 연산은 무대감독(커널)에게 일일이 허락받지 않고 CPU가 직접 기계어 코드를 실행합니다.
    - 이 단계에서는 커널의 개입이 전혀 없습니다.
6. user-level-thread가 cpu에서 연산하는거 중단하는 2가지 경우 
    1. syscall
        - 유저레벨 스레드의 연산 중, 갑자기 디스크에서 파일읽기같은게 있어서 read() syscall 날리는 경우, 기다릴 수 없으니까 방 뺌. 
        - 이 경우 모드를 유저모드 -> 커널모드 다시 전환 
        - 이 후, 커널은 이 file i/o 요청을 커널 레벨 스레드한테 시키고, 해당 유저레벨 스레드는 대기 상태로 전환시킴. 파일 가져올 때 까지 쉬어~ 
        - 그리고 이 유저 레벨 스레드가 어디까지 연산했는지 컨텍스트를 저장하고 
        - step2로 돌아가서 cpu scheduling부터 다시 반복함 
    2. timer interupt
        - 시간이 다됬어요! 이제 나오세요!

### h-6. 경량 스레드(fiber, green thread, virtual thread, etc)
jvm virtual thread(jdk21+) 기준으로 설명하면, 
스레드가 무거우니까 (4mb였나?) 가벼운걸로 쓰자는건데 

핵심 최적화는 I/O 작업으로 스레드가 멈춰야 할 때, OS 레벨의 비싼 컨텍스트 스위칭을 피하는 것 

1. 기존 일반 스레드의 문제점 
    1. 비싼 고용 비용: 배우 한 명(스레드 하나)을 무대에 세우려면 OS(무대감독)가 많은 준비(메모리 할당, 커널 자료구조 생성)를 해야 합니다. new Thread()를 호출하는 것은 비용이 비쌉니다.
    2. 비싼 교체 비용: 배우 A가 연기 도중 "소품이 올 때까지 10초 대기" (I/O 대기) 상태가 되면, 무대감독(OS)은 무대 전체를 암전시키고, 배우 A를 대기실로 보낸 뒤, 배우 B를 위한 무대를 처음부터 다시 설치해야 합니다. 이 과정(컨텍스트 스위칭)이 매우 느리고 비효율적입니다.
    3. 제한된 숫자: 이런 비싼 배우는 수백, 수천 명 고용할 수 없습니다. 시스템 자원이 금방 고갈됩니다.
2. 가상 스레드가 위 문제를 해결한 방법 
    - 구조 
        1. 네이티브 스레드 (캐리어 스레드): OS가 직접 관리하는 소수의 프로 배우. 실제로 무대(CPU) 위에서 연기합니다.
        2. 가상 스레드: 연출가(JVM)가 관리하는 수백만 개의 대본 쪽지. 각 쪽지에는 아주 짧은 대사(작업)가 적혀 있습니다.
        3. JVM 스케줄러: 이 모든 것을 지휘하는 천재 연출가.
3. 최적화 과정 
    1. 연출가(JVM)가 네이티브 스레드에게 **가상 스레드 A** (짧은 일이 적힌 것) 를 주고 일 시킴 .
    2. 네이티브 스레드가 일처리 하다가 I/O 대기 상황이 발생합니다. (파일 읽기 from disk)
    3. 이 떄, 원래라면 os kernel이 네이티브 스레드를 대기상태 만들고 다른 스레드 불러서 context switching이 일어났는데(이게 병목지점), 이 때 jvm이 os kernel 호출 안하고, 가상 스레드A를 unmount하고, 가상 스레드B를 로드시켜 계쏙 일을 시킴 
        - 즉, I/O를 기다리며 노는 시간이 사라지고, CPU 코어를 거의 항상 100% '실제 연산'에 사용하게 됩니다.
        - 그래서 virtual thread가 적용된 코드 실행시키고 시스템 메트릭 보면 cpu usage가 99% 찍힘 
    4. 결과적으로 네이티브 스레드는 cpu에서 unload된 적이 없이, 다른 계속해서 잘게 쪼갠 다른 일을 처리함. 

#### example1)
```java
Thread.startVirtualThread(() -> {
    // --- 이 모든 코드는 '하나'의 가상 스레드에서 순차적으로 실행 ---
    
    System.out.println("Step 1: 데이터베이스에서 사용자 정보 조회 시작");
    // 이 I/O 작업이 100ms 걸린다고 가정 -> 여기서 가상 스레드는 잠시 멈춤 (unmount)
    User user = database.findUser("user123"); 
    
    // 100ms 후 I/O가 끝나면, JVM이 이 가상 스레드를 다시 깨워서 여기서부터 실행 재개 (remount)
    System.out.println("Step 2: 사용자 정보로 인사말 생성 (CPU 작업)");
    String greeting = "Hello, " + user.getName();
    
    System.out.println("Step 3: 외부 날씨 API 호출 시작");
    // 이 I/O 작업이 200ms 걸린다고 가정 -> 여기서 다시 잠시 멈춤 (unmount)
    Weather weather = weatherApi.fetchWeather();
    
    // 200ms 후 I/O가 끝나면, 다시 깨어나서 여기서부터 실행 재개 (remount)
    System.out.println("Step 4: 모든 정보를 합쳐서 최종 결과 생성 (CPU 작업)");
    String result = greeting + " Today's weather is " + weather.getSummary();
    
    System.out.println("최종 결과: " + result);
    // --- 여기까지가 이 가상 스레드의 하나의 작업 단위 ---
});
```

1. 하나의 가상 스레드가 Step 1을 실행하다가 database.findUser()라는 I/O 작업을 만나면 잠시 '일시정지'되고 네이티브 스레드에서 분리됩니다. 
2. 그동안 네이티브 스레드는 다른 가상 스레드의 일을 처리합니다. 
3. DB 조회가 끝나면, JVM은 원래의 가상 스레드를 다시 네이티브 스레드에 얹어서 정확히 멈췄던 다음 지점인 Step 2부터 실행을 이어갑니다.

개발자는 마치 스레드가 한 번도 멈추지 않는 것처럼 순차적인(blocking) 코드를 작성하고, 실제로는 JVM이 알아서 비동기처럼 효율적으로 만들어주는 것이 가상 스레드의 가장 큰 장점입니다.

근데 이 예제는 저 100ms i/o 작업 할 떄 가상스레드를 unmount한 후에 다음 할 다른 가상스레드 코드가 안보여서 잘 이해가 안가니, 다른 예제 

#### example2) 
```java
import java.net.ServerSocket;
import java.net.Socket;

// --- 실제 서버는 이런 무한 루프를 돌고 있습니다 ---
public class VirtualThreadServer {
    public static void main(String[] args) throws Exception {
        ServerSocket serverSocket = new ServerSocket(8080); // 8080 포트에서 대기

        while (true) {
            // 1. 새로운 클라이언트의 접속을 기다립니다. (이 자체가 Blocking I/O)
            Socket clientSocket = serverSocket.accept(); 
            
            // 2. 클라이언트가 접속할 때마다 '새로운' 가상 스레드를 생성해서 작업을 맡깁니다.
            Thread.startVirtualThread(() -> {
                // 이 블록은 방금 접속한 클라이언트 '한 명'만을 위한 것입니다.
                handleClientRequest(clientSocket); 
            });
        } // 다시 루프의 처음으로 돌아가 다음 클라이언트를 기다립니다.
    }

    // ------------------------------------------------------------------

    // handleClientRequest 함수는 이전에 보여드린 예시와 같습니다.
    static void handleClientRequest(Socket socket) {
        // Step 1: DB 조회 (I/O 발생 시 이 가상 스레드는 여기서 '멈춤')
        // User user = database.findUser(...);
        // ...
        // Step 4: 최종 응답 전송
    }
}
```

1. 1,000명의 클라이언트가 거의 동시에 서버에 접속합니다.
2. 서버의 while (true) 루프가 1,000번 빠르게 돌면서 1,000개의 가상 스레드(VT1, VT2, ..., VT1000)를 생성하고, 이들을 JVM 스케줄러에 던져놓습니다.
3. JVM은 소수의 네이티브 스레드(예: 8개, NT1 ~ NT8) 풀을 가지고 있습니다.
4. JVM이 VT1을 NT1에 마운트해서 실행시킵니다. VT1이 database.findUser()에서 I/O 대기에 들어갑니다.
5. JVM은 NT1에서 VT1을 즉시 떼어내고(unmount), 그 자리에 '다른 일'인 VT2를 마운트해서 실행시킵니다.
6. VT2도 곧 I/O 대기에 들어갑니다. JVM은 NT1에서 VT2를 떼어내고, VT3를 마운트합니다.
7. 이 과정이 모든 네이티브 스레드 위에서 빛의 속도로 반복됩니다. 수많은 가상 스레드들이 아주 잠깐씩 네이티브 스레드를 점유했다가 I/O를 만나면 바로 비켜줍니다.
8. 얼마 후, VT1의 DB 조회가 끝났다는 I/O 완료 이벤트가 도착합니다.
9. JVM은 현재 비어있는 네이티브 스레드(예: NT5)에 VT1을 다시 마운트해서 정확히 멈췄던 다음 지점부터 실행을 재개시킵니다.

이처럼 '다른 일'은 미리 묶어서 주는 것이 아니라, 외부로부터 들어오는 동시 요청에 의해 실시간으로 계속 공급되는 것입니다. 가상 스레드는 이러한 수많은 동시 요청을 처리하기 위해 고안된 기술입니다.

#### 단점 
네이티브 스레드 아래 딸린 여러개의 경량스레드(w/ 조각 태스크)가 cpu core 하나씩 붙잡고 계속 연산하는 방식이 
i/o 대기 병목 줄여주고 context switching cost 줄여줘서 좋긴 한데 단점이 발생함 

1. 이건 단점은 아니고 특징?정도인데, 이제 서버 성능이 순수 cpu core 갯수에 따라 달라짐
    - 더 많은 요청 처리하려면, 이제 더 이상 i/o 병목이 문제가 아니라, 더 빠른 cpu를 사거나 cpu core 수를 늘려서 병렬처리 하거나 
2. 소수의 네이티브 스레드 하위에 수많은 수만개의 가상스레드가 소수의 한정된 공유자원을 차지하기위해 경쟁함 
    - ex1. database connection pool) 만약 DB 커넥션 풀의 크기가 30개인데, 수천 개의 가상 스레드가 동시에 DB에 접근하려 한다면 어떻게 될까요? 30개를 제외한 모든 가상 스레드는 커넥션을 얻기 위해 줄을 서서 기다려야 합니다. 병목이 I/O가 아닌 DB 커넥션 풀에서 발생하는 것이죠.
    - ex2. 동기화 블록 (synchronized): 여러 가상 스레드가 동시에 synchronized로 보호된 코드에 접근하려고 하면, 오직 하나만 진입할 수 있습니다. 이때 다른 가상 스레드를 실행해야 할 네이티브 스레드가 이 동기화 블록이 풀리기를 기다리며 **'고정(pinning)'**되어 버릴 수 있어 가상 스레드의 장점이 크게 감소합니다.
3. 해당 서버랑 연결된 다른서버에 과부하를 줄 수 있음 
    - 내 애플리케이션(Node.js/Java)은 이제 너무 효율적이어서, 초당 수천 개의 요청을 처리하여 데이터베이스나 외부 API 같은 하류 시스템에 엄청난 부하를 줄 수 있습니다. 내 서버는 괜찮지만, 내가 의존하는 다른 시스템이 터져나가는 문제가 발생할 수 있습니다. 
4. 시스템 여유 자원 부족 
    - 늘상 cpu-usage가 99%일거라, 로그 수집, 모니터링, 백업, 보안 스캔 등 백그라운드에서 실행되어야 할 다른 중요한 시스템 프로세스들이 실행될 CPU 자원이 부족해집니다.
    - 서버에 SSH로 접속하는 것조차 버벅거릴 수 있으며, 시스템 전체가 불안정해 보일 수 있습니다.

#### use-case
JDK 21의 가상 스레드를 포함한 경량 스레드(파이버)는 '기다리는' 일이 아주 많은 고성능 네트워크 애플리케이션에서 사용할 때 가장 강력한 효과를 발휘합니다.

핵심은 I/O-bound(입출력 위주) 작업을 수백, 수천, 수만 개씩 동시에 처리해야 하는 상황입니다.

##### 가장 이상적인 사용처: '기다리는' 일이 많은 동시성 애플리케이션
이 기술은 본질적으로 I/O 작업을 기다리는 동안 스레드가 노는 시간을 없애기 위해 만들어졌습니다. 따라서 아래와 같은 애플리케이션에 가장 적합합니다.

- example 
    1. 마이크로서비스 및 API 게이트웨이: 수많은 클라이언트 요청을 받아 다른 서비스나 데이터베이스를 호출하고 응답을 기다리는 일이 대부분인 경우.
    2. 대규모 웹 애플리케이션: 수천 명의 사용자가 동시에 접속하여 각자 DB를 조회하고, 캐시를 읽고, 외부 API를 호출하는 등 대부분의 시간을 '대기'로 보내는 경우.
    3. 메시징 시스템 및 실시간 통신 서버: 수많은 클라이언트와 소켓 연결을 유지하며 메시지가 오기를 기다리는 애플리케이션.
- 피해야 할 상황: '계산'만 하는 CPU 집중 작업
    - 경량 스레드는 CPU-bound(계산 위주) 작업에는 거의 이점이 없으며 오히려 약간의 오버헤드가 발생할 수 있습니다. 
    1. 빅데이터 처리 및 대용량 파일 변환: 거대한 데이터를 읽어와 복잡한 통계 계산이나 변환 작업을 수행하는 경우.
    2. 머신러닝 모델 학습 및 추론: 행렬 곱셈 등 순수 CPU 연산이 대부분인 경우.
    3. 비디오 인코딩 및 그래픽 렌더링: 픽셀 단위의 복잡한 수학 연산을 수행하는 경우.

##### single thread non-blocking (nodejs) vs 동기식 코드(java + virtual thread)
1. Node.js는 처음부터 **"모든 I/O는 논블로킹(non-blocking)이어야 한다"** 는 철학으로 설계되었습니다. 
    - 스레드도 메인스레드 1개랑 cpu-intensive 코드 처리하는 스레드 몇개 
    - 비유: 스타벅스 커피 주문 
        1. 당신은 카운터에서 주문(I/O 작업 요청)을 합니다.
        2. 점원은 당신을 그 자리에서 기다리게 하는 대신 **진동벨(Callback 또는 Promise)**을 줍니다.
        3. 당신은 진동벨을 받고 자리에 가서 다른 일(웹서핑 등)을 합니다. 카운터(메인 스레드)는 막히지 않고 다음 손님의 주문을 계속 받습니다.
        4. 커피가 나오면 진동벨이 울리고(이벤트 발생), 당신은 커피를 받아옵니다.
    - 개발 방식: 개발자는 readFile('a.txt', (data) => { ... }) 나 fetch().then(...) 처럼 명시적으로 비동기 스타일의 코드를 작성해야 합니다. 코드의 흐름이 선형적이지 않아 '콜백 지옥' 같은 문제가 발생하기도 합니다.
2. Java의 가상 스레드는 "개발자는 편하게 동기식으로 코딩해라. 성능은 우리가 알아서 비동기처럼 만들어줄게" 라는 철학
    - 개발자는 String data = readFile('a.txt'); 처럼 과거의 간단한 동기식(blocking) 코드를 그대로 작성하면 됩니다. I/O 작업을 만나면 코드가 멈추는 것처럼 보이지만, 실제로는 JVM이 그 스레드를 잠시 재워두고(unmount) 다른 스레드의 일을 처리해주는 '마법'이 일어납니다.




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


# H. step8 ae 

## h-1. what 
libuv의 lightweight한 버전. redis-core에 쓰임.

`brew install libuv`로 깔아보면 알겠지만 27k lines임. 너무 무거움.

libuv처럼 os별로 poll이나 kqueue같은거 내부적으로 처리해주는데,

heavy한 task라 중간에 병목 걸리면 안되니까, 그걸 thread pool만들어서 따로 처리해줌.

근데 light weight함.

