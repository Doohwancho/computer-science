---\
How to run?


1. 서버 실행
2. 터미널에서 `nc localhost 8080` 하고 문자열 입력


---\
Features


- Libraries: `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`
- Implement basic socket operations (`socket()`, `bind()`, `listen()`, `accept()`)
- Single-threaded, blocking I/O to start
- This gives you a foundation to handle one connection at a time


---
A. `<sys/socket.h>`


core functions 

- socket() - Creates a new socket
- bind() - Assigns an address to a socket
- listen() - Marks a socket as passive for accepting connections
- accept() - Accepts a connection on a socket
- connect() - Initiates a connection on a socket
- send()/recv() - Send/receive data on a socket
- setsockopt() - Sets socket options (like reuse address, buffer sizes)


example 
```c
// Create socket
socket()
// Setup address
struct sockaddr_in
htons() for port
// Bind and listen
bind()
listen()
// Accept connections
accept()
```

---
B. `<netinet/in.h>`


Internet Address Family

- struct sockaddr_in - IPv4 socket address structure
    - sin_family - Address family (AF_INET)
    - sin_port - Port number
    - sin_addr - IP address
- constants
    1. INADDR_ANY - Bind to all network interfaces
    2. INADDR_LOOPBACK - Localhost address


---
C. `<arpa/inet.h>`


Address Manipulation library 

1. htons()/ntohs() - Host to network / Network to host (short)
2. htonl()/ntohl() - Host to network / Network to host (long)
3. inet_addr()/inet_ntoa() - Convert IP string to binary / binary to string
4. inet_pton()/inet_ntop() - Modern versions of above functions (support IPv6)

