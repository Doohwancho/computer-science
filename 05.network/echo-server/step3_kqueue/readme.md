
# how to test?
```
curl -v http://localhost:8080/

*   Trying [::1]:8080...
* connect to ::1 port 8080 failed: Connection refused
*   Trying 127.0.0.1:8080...
* Connected to localhost (127.0.0.1) port 8080
> GET / HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/8.4.0
> Accept: */*
>
< HTTP/1.1 200 OK
< Content-Type: text/plain
< Content-Length: 12
< Connection: close
<
* Closing connection
Hello World!%
```


총 100 GET request를 동시성 10개로 보내고 싶다면?

```
ab -n 100 -c 10 http://127.0.0.1:8080/ 

This is ApacheBench, Version 2.3 <$Revision: 1903618 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient).....done


Server Software:
Server Hostname:        127.0.0.1
Server Port:            8080

Document Path:          /
Document Length:        12 bytes

Concurrency Level:      1
Time taken for tests:   0.010 seconds
Complete requests:      100
Failed requests:        0
Total transferred:      9600 bytes
HTML transferred:       1200 bytes
Requests per second:    10249.05 [#/sec] (mean)
Time per request:       0.098 [ms] (mean)
Time per request:       0.098 [ms] (mean, across all concurrent requests)
Transfer rate:          960.85 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       0
Processing:     0    0   0.0      0       0
Waiting:        0    0   0.0      0       0
Total:          0    0   0.0      0       0

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      0
  99%      0
 100%      0 (longest request)
```

