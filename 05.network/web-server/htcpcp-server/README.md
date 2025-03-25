# HTCPCP Server

## takeaway

1. multi threading
2. thread pool
3. mutex lock (suspend -> wakeup call)


## Getting Started

### Download

```sh
git clone https://github.com/daviskauffmann/htcpcp-server.git
cd htcpcp-server
```

### Install Dependencies

Install the following using the package manager of your choice:

- http-parser
- pthreads
- Winsock


winsock은 window전용..

main.c에 주석 단 하이라이트 부분만 참고.


### Build

```sh
mkdir build
cd build
cmake ..
```

## Resources Used

[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

[Jacob Sorber's multithreaded webserver series](https://www.youtube.com/watch?v=Pg_4Jz8ZIH4)

[HTCPCP/1.0](https://tools.ietf.org/html/rfc2324)


## reference

https://github.com/daviskauffmann/htcpcp-server
