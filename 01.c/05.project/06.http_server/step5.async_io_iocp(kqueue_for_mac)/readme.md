---\
What 


1. IOCP는 window 용이라,
2. Linux는 epoll 써야 함
3. Macbook은 kqueue 써야 함.


---\
step4가 이거 아닌가? 싶었는데 아님. 

step4는 동기방식의 multi thread, thread pool에서 여러 쓰레드가 요청을 queue.add()해서 각 쓰레드가 동기적 처리하는 거고,
step5(iocp, kqueue for mac)은 똑같은 kqueue에 http_request를 넣으면, 쓰레드 4개가 각자 비동기로 처리한다. 


step4는 thread 여러개 만들고, threadpool에 담아서, 
incoming http_request를 queue.add() 하면, 
각 쓰레드가 날아온 http_request를 파싱해서 처리하는 거라면, 


step5는 thread 4개가 딱히 pool을 만들진 않았지만(만들어도 무관할 듯), 
각자 incoming http_request를 queue.add() 하면, 
kqueue.h 가 비동기로 처리함 
