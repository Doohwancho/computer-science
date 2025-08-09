# What 

## diff socket websocket 

둘다 TCP protocol 기반이고, continuous data exchange에 쓰는데, 

socket은 

outcome 
```
Sent WebSocket upgrade request
Received server response:
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: Bt870eqNUsVHU3Z1Nfxt77+XWH0=


WebSocket connection established
Sent message: Hello, WebSocket Server!
Received echo: Hello, WebSocket Server!

```
# Q. how to buid & run on mac?

step1. compile & run server
```
brew install openssl

gcc -o websocket_server websocket_server.c -I$(brew --prefix openssl)/include -L$(brew --prefix openssl)/lib -lssl -lcrypto

./websocket_server
```


step2. compile & run client
```
gcc -o websocket_client websocket_client.c -I$(brew --prefix openssl)/include -L$(brew --prefix openssl)/lib -lssl -lcrypto

./websocket_client
```


