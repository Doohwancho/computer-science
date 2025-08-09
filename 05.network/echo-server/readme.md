---\
how to test?

```
python stress_test.py --url http://localhost:8080/ --duration 5 --concurrency 10 --timeout 1 --rate 100
```

---\
parameter 

```
--url: The target URL to test (default: http://localhost:8080/)
--duration: How long to run the test in seconds (default: 10)
--concurrency: How many concurrent connections to maintain (default: 100)
    - concurrency 파라미터는 동시에 연결할 수 있는 최대 연결 수를 설정하는 것이지, 초당 요청 수를 제한하는 것이 아닙니다
    - 테스트는 초당 10개의 요청이 아니라, 10개의 동시 연결을 사용해 시스템이 처리할 수 있는 최대 속도로 요청을 보냅니다.
--timeout: Maximum time to wait for each response in seconds (default: 5)
--rate: 초당 request 보내는 수량 제한함
    - --rate 0(기본값)으로 설정하면 이전처럼 최대 속도로 요청을 생성함 
```

# B. step1_baseline

초당 900개 처리함. 

```
python stress_test.py --url http://localhost:8080/ --duration 5 --concurrency 10 --timeout 1 --rate 10000
Starting stress test on http://localhost:8080/
Duration: 5 seconds, Concurrency: 10, Timeout: 1.0 seconds
Rate limited to 10000 requests per second

=== Stress Test Results ===
Total requests: 4526
Requests per second: 905.10
Successful requests: 4526 (100.00%)
Failed requests: 0
Average latency: 0.70 ms
Min latency: 0.35 ms
Max latency: 40.34 ms
Median latency: 0.64 ms
95th percentile latency: 0.92 ms
99th percentile latency: 1.52 ms
```
