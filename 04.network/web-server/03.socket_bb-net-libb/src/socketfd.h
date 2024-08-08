#ifndef SOCKETFD
#define SOCKETFD  //socketfd is a custom type definition for a socket file descriptor

typedef int socketfd; //fd stands for 'file descriptor', b/c in unix system, sockets are treated as file descriptors. 
//socketfd 가 int의 alias로 만드는 것 
//It clearly indicates that a particular integer is being used as a socket file descriptor, rather than just any integer value.

#endif
