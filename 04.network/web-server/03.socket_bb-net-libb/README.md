# A. what 
simple TCP/IP server (web sockets included)

reference: https://github.com/Bixkitts/bb-net-lib 

## a. console.log
1. client 쪽 
```
./testClient

Welcome to the test client!
-------------------------------
```


2. server 쪽
```
./testServer

Welcome to the test server!
-----------------------------------

Received data:Opening Connection!AAAAA

Received data:BBBBB

Received data:AAAAA

Received data:BBBBB

Received data:AAAAA

Received data:BBBBB

Received data:AAAAA

Received data:BBBBB

Received data:AAAAA

Received data:BBBBB

Received data:Huh?

Received data:end

Received data:

Connection shutdown triggered by recv()...
```


## b. 서버 구조

src/tests/server.c
```c
localhost = createHost("0.0.0.0", 80);
listenForTCP(localhost, testPacketHandler);
```

1. createHost() from clientObject.c 
    - host에 IPv4, TCP, 80 port, client host별 고유 아이디 부여
    - clientObject.h에서 typedef struct Host를 봐서 host에 뭐있는지 파악하자. 
2. listenForTCP() from listen.c
    1. threadpool 만들고
    2. socket 만들고 
    3. socket 만든걸 host에 bind하고
    4. client에서 받은 데이터를 packetReceptionArgs 구조체로 받아서, 만든 threadpool에 addTask()로 추가해준다. 
3. createThreadPool() & addTaskToThreadPool() & executeTask() from threadPool.c 
    1. 코어 갯수 8개에 맞게 쓰레드 8개 생성해주고,
    2. mutex lock을 걸 공유자원, task queue의 사이즈는 1000으로 함.
    3. 각 task는 function과 argument를 받는 구조체
    4. task queue에 task를 추가하거나 execute할 때마다 lock을 걸고, wait를 날림.
4. create, bind, setTime for sockets from socketsNetLib.c
    1. socketfd는 socket file descriptor의 약자로, unix에서는 socket = file.
    2. socketfd는 custom type(int -> socketfd). 특정 int value를 socketfd 이라고 나타내는 것
    3. socket.c에서 제공하는 bind() 함수로 특정 host:port와 socketfd를 묶는다.
    4. non-blocking mode로 소켓을 설정한다. 



## c. 클라이언트 구조

src/tests/client.c

1. connectToTCP() from send.c
    - socket.h에서 제공하는 connect() 함수로 socketfd와 remoteAddress를 연결
2. sendDataTCP()
    - 보낼 때도 socketfd(int)마다 mutex lock을 걸어서, 여러 socketfd들을 TCP data send() 할 수 있지만, 각각 socket은 순차적으로 보내게 됨. 
    - send_variants[packetSenderType](&sendArgs) 으로 분기 처리해서 데이터 보내고, 보내는 데이터는 &sendArgs



## d. takeaway 
VSC extension: TODO만 모아보는 걸로,
TODO - takeaway: 에 적은 주석 보기


# B. how to build on mac? 
## step1. build
```
cd bb-net-lib
mkdir build 
cmake ..
make
```

## step2. test
```
cd bb-net-lib/build
./testServer
./testClient
```



----------------------------------------------------------------------------------------------
# Networking For Simpletons
No weird complicated parameters to configure, no 
confusing function names, no bullcrap.

The working man's transport layer IP communications.

## How Do I Use This Library?
You can either:
- Add it as a git submodule
- Install it and include it
- Download a release build
- Build it yourself

All of these are easy and convenient options.

### Git Submodule
- Enter the directory of your own git project
- git submodule add https://github.com/Bixkitts/bb-net-lib.git path/to/submodule
- git submodule update --init --recursive

And then to update this library while it's a submodule:
- git submodule update --remote path/to/submodule
(After being updated, you need to commit and push the changes to your remote)

### Install and Include It
- git clone https://github.com/Bixkitts/bb-net-lib.git
- cd bb-net-lib
- mkdir build
- cd build
- cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
- cmake --build . --target install

Add this to your CMakeLists.txt
- target_link_libraries(${PROJECT_NAME} PRIVATE bbnetlib)

Now you can include <b><bbnetlib.h></b> in your own project and get to coding.


If you're feeling fancy:
- find_package(bbnetlib 0.1 CONFIG REQUIRED)

If you <b>don't have cmake</b>, well then link the library via your own methods.

### Download a Release Build
- Download a release through the browser or wget/curl
- But the library and include header into a place your own project can use it
- include <b>"bbnetlib.h"</b> and link libbbnetlib.

### Build it Yourself
Follow the steps to <b>Install and Include It</b>,
except you do
- cmake --build .

without --target install.
Then do with the resultant libbbnetlib.a and include header as you see fit.

## No, How Do I Write With It?
Here's TCP:
```c
// We want to store the localhost.
// I've used a global, but you can
// use accessors or a database or anything.
Host localHost = NULL;

void yourTCPpacketHandler (char* receivedPacketData, 
                           ssize_t sizeOfPacket, 
                           Host hostThatSentThePacket)
{
    // Enable TLS encryption
    enableEncryption();
    Host remotehost = hostThatSentThePacket;
    // Send some data back
    const char data[] = "Hello there!";
    ssize_t    dataSize = 13;
    sendDataTCP(data, datasize, remotehost);
    // We can cache this host
    // in up to 16 caches for
    // later multicasts!
    int cacheIndex = 0;
    cacheHost(remotehost, cacheIndex);
    // Send the same packet to a cache full of hosts
    multicastTCP(data, datasize, cacheIndex);
    // TCP:
    // We've decided we're completely done with this host,
    // Stop communicating with them.
    closeConnections(remotehost);
    // We just need to close connections on the localhost
    // and then we escape the top level listenTCP()
    // function!
    if (WERE_BORED) {
        closeConnections(localhost)
    }
}
int main() 
{
    localhost = createHost("YOUR.IP", 1234);
    listenTCP (localhost, yourTCPpacketHandler);
    return 0;
}
```
Here's UDP, it's almost the same:
```c
// We want to store the localhost.
// I've used a global, but you can
// use accessors or a database or anything.
Host localHost = NULL;

Host cachedRemoteHost;

void yourUDPpacketHandler (char* receivedPacketData, 
                           ssize_t sizeOfPacket, 
                           Host hostThatSentThePacket)
{
    Host remotehost = hostThatSentThePacket;
    // Get the IP of the remotehost:
    char ip[IP_ADDR_LEN] = getIp(remotehost);
    // Send some data back
    const char data[] = "Hello there!";
    ssize_t    dataSize = 13;
    // UDP is connectionless, we just send a packet
    // back to a remote host.
    sendDataUDP(data, 13, remotehost);
    // We just need to close connections on the localhost
    // and then we escape the top level listenUDP()
    // function!
    if (WERE_BORED) {
        closeConnections(localhost)
    }
}
int main() 
{
    localhost = createHost("YOUR.IP", 1234);
    listenUDP (localhost, yourUDPpacketHandler);
    return 0;
}
```

Maximum packet size is <b>1024 bytes</b>, so you know it's a full packet when that's the received size.

Receiving a <b>packet size of 0</b> in the packetHandler means the remote host negotiated a disconnect,

and <b>-1 packet size</b> means an error (typically a timeout).

<b>TCP connections time out after 5 seconds</b> (this is compiled into the binary, you can change it if necessary).

## How Do I Help?
Hit me up and help me write it.

Request features by opening an issue.

## Encryption?
Yes, just enableEncryption() for TLS and have a certificate and key for it next to the binary.

## Planned Features
- Nothing really right now, I'm just using this to power my own webservers
  so I'll adjust it to fit those needs

## Known Issues, Beware!
I haven't touched UDP code in months,
it probably doesn't work but I'm actively using TCP
and TLS in another project so that should work fine
as described here.
