---\
Goal

Modularize Network



---\
Fields

a. protocol: TCP\
b. protocol: UDP\
c. buffer\
d. protocol: gRPC\
e. web server\
f. event loop\
g. multi-thread

x. protocol: http2\
x. protocol: HAProxy

---\
Concepts


a-1. simple client-server socket connection in python :white_check_mark:

b-1. simple client-server socket connection in python :white_check_mark:

c-1. stream splitted by buffer size :white_check_mark:\
c-2. server includes payload size in response header for client to parse easily :white_check_mark:

d-1. simple gRPC using node, express :white_check_mark:

e-1. simple web server in c :white_check_mark:

f-1. event loop from scratch in python :white_check_mark:

g-1. simple web server handling many requests with multi-threads with mutex :white_check_mark:\
g-2. singlethread -> multithread -> multithread with mutex comparsion in c :white_check_mark:\
g-3. binary semaphore :white_check_mark:



---\
Reference


a-1. [simple client-server socket connection in python](https://cumulativebackendstack.blogspot.com/2021/03/tcp-with-socket-programming-in-python-1.html)

b-1. [simple client-server socket connection in python](https://cumulativebackendstack.blogspot.com/2021/03/udp-with-python-udpuser-datagram.html)

c-1,2. [Buffer related to socket.recv()](https://cumulativebackendstack.blogspot.com/2021/03/buffer-related-to-socketrecv.html)

d-1. [simple gRPC using node, express](https://blog.naver.com/jhc9639/222642712063)

e-1. [simple web server in c](https://github.com/infraredCoding/cerveur.git) \
e-2. [multi client server chat](https://github.com/codophobia/Multi-Client-Server-Chat/blob/master/server.c)

f-1. [Explaining event loop in 100 lines of code](https://iximiuz.com/en/posts/explain-event-loop-in-100-lines-of-code/)

g-1. [How to write a multithreaded webserver using condition variables](https://www.youtube.com/watch?v=P6Z5K8zmEmc&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=7) \
g-2. [Safety and Speed Issues with Threads](https://www.youtube.com/watch?v=9axu8CUvOKY&list=PL9IEJIKnBJjFZxuqyJ9JqVYmuFZHr7CFM&index=3)
