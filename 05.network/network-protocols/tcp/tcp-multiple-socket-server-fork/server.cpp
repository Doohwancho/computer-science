//server.cpp

# include "header.h"

# define LISTEN_QUEUE_SIZE 5 //연결 대기 큐 이용

void childHandler(int signal)
{

    int status;
    pid_t spid;

    // -1 :: 어떠한 child process모두 다 받는다. WNOHANG :: 자식 프로세스가 종료되지 않아도 부모는 자신할 일 한다.
    while((spid = waitpid(-1, &status, WNOHANG)) > 0) //자식 프로세스가 종료되면 실행
    {
        printf("자식프로세스 wait한 결과 \n");
        printf("================================\n");
        printf("PID         : %d\n", spid);
        printf("Exit Value  : %d\n", WEXITSTATUS(status));
        printf("Exit Stat   : %d\n", WIFEXITED(status));
    }
}

int main() {

    //childHandler 함수가 SIGCHLD 시그널을 처리할 수 있도록 시그널 설치
    signal(SIGCHLD, (void (*)(int))childHandler);

    struct sockaddr_in listenSocket; //listen socket 생성

    memset(&listenSocket, 0, sizeof(listenSocket));

    listenSocket.sin_family = AF_INET;
    listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
    listenSocket.sin_port = htons(PORT);

    int listenFD = socket(AF_INET, SOCK_STREAM, 0);
    int connectFD;

    ssize_t receivedBytes;
    char readBuff[BUFFER_SIZE];
    char sendBuff[BUFFER_SIZE];
    pid_t pid;


    if (bind(listenFD, (struct sockaddr *) &listenSocket, sizeof(listenSocket)) == -1) {
        printf("Can not bind.\n");
        return -1;
    }

    if (listen(listenFD, LISTEN_QUEUE_SIZE) == -1) { //queue를 이용해서 fork()된 n개의 자식서버들이
        printf("Listen fail.\n");
        return -1;
    }

    printf("Waiting for clients...\n");

    while (1)
    {
        struct sockaddr_in connectSocket, peerSocket;

        socklen_t connectSocketLength = sizeof(connectSocket);

        while((connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, (socklen_t *)&connectSocketLength)) >= 0)
        {
            getpeername(connectFD, (struct sockaddr*)&peerSocket, &connectSocketLength);

            char peerName[sizeof(peerSocket.sin_addr) + 1] = { 0 };
            sprintf(peerName, "%s", inet_ntoa(peerSocket.sin_addr));

            // 접속이 안되었을 때는 출력 x
            if(strcmp(peerName,"0.0.0.0") != 0)
                printf("Client : %s\n", peerName);


            if (connectFD < 0)
            {
                printf("Server: accept failed\n");
                exit(0);
            }
            // 클라이언트가 접속할 때마다 fork를 통해 child process를 생성해 echo를 발생.
            pid = fork();

            // 자식 서버일 때
            if(pid == 0)
            {
                // 리스닝 소켓은 닫아준다. connect socket만 사용해서 client의 요청을 받는다.
                close(listenFD);

                ssize_t receivedBytes;

                // read할 값이 있다면 계속 읽어들인다.
                while((receivedBytes = read(connectFD, readBuff, BUFF_SIZE)) > 0)
                {

                    printf("%lu bytes read\n", receivedBytes);
                    readBuff[receivedBytes] = '\0';
                    fputs(readBuff, stdout);
                    fflush(stdout);

                    sprintf(sendBuff,"%s",readBuff);
                    write(connectFD, sendBuff, strlen(sendBuff)); //connect socket에 client에서 날아온 메세지를 버퍼에 담아 보냄.
                }

                // 클라이언트가 종료되면 해당 자식 프로세스의 커넥팅 소켓도 종료
                close(connectFD);
                return 0; // 혹은 exit(0); 이용

            }

            // 부모 서버라면 커넥팅 소켓을 닫아준다.
            else
                close(connectFD);
        }

    }
    close(listenFD);

    return 0;
}

//console.log


//부모 프로세스에서 fork()하면 부모 프로세스의 모든 내용을 자식 프로세스의 복사하는 것 + 자식프로세스가 종료되면 부모 프로세스에게 SIGCHLD 시그널을 보내는걸 활용해서 만든 서버.
//socket은 2종류.
//client에서 오는 요청을 듣고 socket을 연결해주는 connect socket
//다른 곳에서 요청오는걸 듣는 listen socket

//부모가 자식 fork()로 3개 만들었다면, 자식은 client에서 오는 요청을 connect socket으로 듣고 버퍼에 담아 listen socket으로 보냄.
//listen socket은 부모 프로세스에서만 듣고 있음. 자식 프로세스는 listen socket을 듣지 않음.
//부모가 listen socket으로 온 버퍼에 메시지를 받아 출력.



//실행

//client 3개 실행해서 응답 받음.
//client가 종료하면, 자식 프로세스가 종료되고, 부모 프로세스에서도 종료되는 것을 확인할 수 있다.
//client1 -> server에 말한게, client2, client3에게 전달되지는 않는다.


/* Waiting for clients... */
/* Client : 127.0.0.1 */
/* Client : 127.0.0.1 */
/* Client : 127.0.0.1 */
/* 9 bytes read */
/* what up? */
/* 7 bytes read */
/* hello? */
/* 8 bytes read */
/* Im good */
/* 9 bytes read */
/* are you? */
/* 13 bytes read */
/* what up bro? */
/* 7 bytes read */
/* hello? */
/* 17 bytes read */
/* can you hear me? */
/* 자식프로세스 wait한 결과 */
/* ================================ */
/* PID         : 14233 */
/* Exit Value  : 0 */
/* Exit Stat   : 1 */
/* 자식프로세스 wait한 결과 */
/* ================================ */
/* PID         : 14195 */
/* Exit Value  : 0 */
/* Exit Stat   : 1 */
/* 자식프로세스 wait한 결과 */
/* ================================ */
/* PID         : 14156 */
/* Exit Value  : 0 */
/* Exit Stat   : 1 */




