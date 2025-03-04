---\
Goal


1. cpu-bound app은 multi thread 구조가 유리하고
2. io-bound app은 single thread event loop 구조가 유리하다던데,

실제로 벤치마크 떠보자.


---\
What


1. cpu-bound/ <--- 소수 카운트 (cpu-bound)
2. cpu-bound2/ <--- read large file(io-bound) + word count(cpu-bound)
3. io-bound/
    - case1) 여러 개의 작은 파일을 비동기적으로 읽기 (파일 사이즈만 읽기)
    - case2) 1번의 request마다 5개의 task를 10-30ms의 랜덤 지연 한 후 반환하기 


---\
How

step1) create test data 

1. mkdir data
2. node create-files.js
3. mkdir data/small-files
4. small-file.txt을 file1,2,3,4,5로 바꿔서 data/small-files/ 에 넣기 (for io-bound test)



step2) how to cpu-intensive-app benchmark?
```
node cpu-bound/single-thread.js
node cpu-bound/multi-thread.js
node cpu-bound2/single-thread.js
node cpu-bound2/multi-thread.js

node cpu-benchmark.js
```

step3) how to io-intensive-app benchmark?
```
node io-bound/single-thread.js
node io-bound/multi-thread.js

node io-benchmark.js
```



---\
benchmark 


case1) cpu-benchmark 

```
CPU-BOUND TESTS 1
case1) single-thread event loop
Benchmarking http://localhost:3000/prime/10000 with 100 iterations and 10 concurrent requests
Total time: 185.09ms
Average request time: 1.85ms
Requests per second: 540.26
-----------------------------------
case2) multi-thread
Benchmarking http://localhost:3001/prime/10000 with 100 iterations and 10 concurrent requests
Total time: 61.93ms
Average request time: 0.62ms
Requests per second: 1614.72
-----------------------------------
CPU-BOUND TESTS 2
case1) single-thread event loop
Benchmarking http://localhost:3002/file/large with 100 iterations and 10 concurrent requests
Total time: 4353.71ms
Average request time: 43.54ms
Requests per second: 22.97
-----------------------------------
case2) multi-thread
Benchmarking http://localhost:3003/file/large with 100 iterations and 10 concurrent requests
Total time: 1101.25ms
Average request time: 11.01ms
Requests per second: 90.81
-----------------------------------
COMPARISON RESULTS:
CPU-BOUND1:
Single-thread RPS: 540.26
Multi-thread RPS: 1614.72
Performance improvement: 198.87%
CPU-BOUND2:
Single-thread RPS: 22.97
Multi-thread RPS: 90.81
Performance difference: 295.34%
```


당연하게 cpu-bound app은\
single-thread event loop 구조보다\
multi thread 구조가 

더 성능이 잘나온다. 



case2) io-bound
```
PURE IO-BOUND TESTS
--------------------------------
case1) Single-thread event loop - 파일 IO
Benchmarking http://localhost:4000/io-test with 100 iterations and 10 concurrent requests
Total time: 61.00ms
Average request time: 3.84ms
Requests per second: 1639.34
-----------------------------------
case2) Multi-thread - 파일 IO
Benchmarking http://localhost:4001/io-test with 100 iterations and 10 concurrent requests
Total time: 50.00ms
Average request time: 3.26ms
Requests per second: 2000.00
-----------------------------------
case3) Single-thread event loop - 네트워크 IO
Benchmarking http://localhost:4000/network-io with 100 iterations and 10 concurrent requests
Total time: 311.00ms
Average request time: 27.74ms
Requests per second: 321.54
-----------------------------------
case4) Multi-thread - 네트워크 IO
Benchmarking http://localhost:4001/network-io with 100 iterations and 10 concurrent requests
Total time: 87.00ms
Average request time: 5.10ms
Requests per second: 1149.43
-----------------------------------
COMPARISON RESULTS:
파일 IO:
Single-thread RPS: 1639.34
Multi-thread RPS: 2000.00
Performance difference: +22.00%
네트워크 IO:
Single-thread RPS: 321.54
Multi-thread RPS: 1149.43
Performance difference: +257.47%
```

근데 예상을 빗나가는건,\
io-bound 역시,\
multi thread > single thread 더 성능이 좋았다.

Q. 왜지?

A. 
- 지금 이 벤치마크는 동시에 10개 요청을 날리고 있음
- 싱글 스레드는 10개 요청이 전부 한 줄로 서 있는거고
- 멀티 스레드는 4개 워커에 나눠서 처리하니까 당연히 빠름

1) 싱글 스레드의 콜백 지옥:

파일 IO 자체는 비동기로 별도 스레드에서 처리되지만\
콜백은 싱글 스레드 큐에서 순차 처리됨\
동시 요청이 많아지면 콜백 큐에서 대기 시간 발생\
10개 요청의 콜백이 줄 서있는데 당연히 느림


2) 워커 스레드의 꿀:

각 워커마다 독립적인 이벤트 루프가 있음 (이게 핵심)\
즉 4개의 이벤트 루프가 병렬로 콜백 처리함\
싱글 스레드처럼 한 줄로 서서 기다리는게 아니라 여러 줄로 나눠짐


결론)

"IO-bound는 싱글 스레드가 빠르다"는 말은 단일 요청 기준임\
동시에 여러 요청 처리하면? 멀티 스레드가 이김ㅋㅋ\
실전에서는 동시에 여러 클라이언트 접속하니까...


파생 결론1)

Q. 그러면 실전에서는 webapp같은 경우 동시에 여러 요청이 들어오는데, 대부분 멀티쓰레드가 성능이 더 좋겠네?\
그러면 멀티스레드 기반 스프링으로 webapp 만드는게 single thread event loop 기반인 nodejs보다 더 동시성 요청이 많은 환경에서는 더 유리하겠네?

A. llm은 그렇다던데?

보통 single thread event loop 기반인 node.js 하다가\
multi thread 기반인 spring으로 webapp 갈아타는 이유가 이 때문인가?
