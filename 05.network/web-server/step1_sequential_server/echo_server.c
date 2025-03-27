// Sequential socket server - accepting one client at a time.
//
// Eli Bendersky [http://eli.thegreenplace.net]
// This code is in the public domain.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

typedef enum { WAIT_FOR_MSG, IN_MSG } ProcessingState;

//serve_connection 함수는 클라이언트랑 통신하는 부분임. 
//먼저 "*" 문자 보내서 클라이언트한테 "ㅇㅇ 연결됐다" 알려줌. 
//그리고 상태 머신으로 프로토콜 처리함.
void serve_connection(int sockfd) {
  //여기가 핵심인데,
  //클라이언트가 연결 시도하고 데이터 보내는 게 서버가 accept()로 받아들이기 전에도 가능하다는거임. 
  //근데 이러면 진짜 블로킹 상황을 시뮬레이션하기 어려움 
  //그래서 개발자가 "*" 문자를 "ack"로 보내는거임. 
  //이게 뭐냐면, 클라이언트가 이 "ack"를 받기 전에는 진짜 데이터를 안 보내게 하는거지. 
  //이러면 한 클라이언트가 서비스 받는 동안 다른 클라이언트들이 기다리는 상황을 정확하게 시뮬레이션할 수 있음. 
  //결국 이 코드는 일부러 비효율적으로 만든거임. 
  //이 코드는 "순차적 처리의 단점"을 더 명확하게 보여주기 위해 일부러 이런 방식으로 설계한 거임
  //이렇게 해야 동시성 없는 서버의 단점을 극명하게 보여줄 수 있으니까.

  //Q. accept()하기도 전에 client의 데이터를 미리 받는다는게 뭔말임?
  // 1. TCP 연결의 특징:
  //    - 클라이언트가 "야 연결하자"(connect()) 하면
  //    - 서버가 "ㅇㅇ 알았다"(accept()) 하기 전에도
  //    - OS가 "ㅇㅇ 연결됐다" 하고 연결을 만들어버림
  // 2. 데이터 전송:
  //    - 클라이언트가 데이터를 보내면
  //    - 서버가 아직 "ㅇㅇ 알았다"(accept()) 안 했어도
  //    - 그 데이터는 서버 쪽에 있는 대기실(버퍼)에 쌓임
  // 3. 서버의 accept():
  //    - 서버가 "ㅇㅇ 알았다"(accept()) 하면
  //    - 대기실(버퍼)에 있던 데이터를 바로 읽을 수 있음
  // 4. 시뮬레이션 문제:
  //    - 여러 클라이언트가 동시에 "야 연결하자" 하고 데이터 보내면
  //    - 서버가 하나씩 "ㅇㅇ 알았다" 할 때
  //    - 이미 모든 클라이언트의 데이터가 대기실에 있을 수 있음
  // 그래서 "한 놈 처리하는 동안 다른 놈들은 기다려야 돼" 이걸 제대로 보여주기 어려움
  // 그래서 이 코드는 일부러 "ㅇㅇ 알았다" 하고 나서 "*" 보내고, 
  // 클라이언트가 그거 받고 나서야 진짜 데이터 보내게 만든거임. 
  // 이러면 한 놈 처리하는 동안 다른 놈들 진짜로 기다리게 되는거지.

  if (send(sockfd, "*", 1, 0) < 1) { //클라이언트와 드디어 연결됬다고 신호 보내는 것. 
    perror_die("send");
  }

  ProcessingState state = WAIT_FOR_MSG;

  while (1) {
    uint8_t buf[1024];
    int len = recv(sockfd, buf, sizeof buf, 0);
    if (len < 0) {
      perror_die("recv");
    } else if (len == 0) {
      break;
    }

    for (int i = 0; i < len; ++i) {
      switch (state) {
      case WAIT_FOR_MSG:
        if (buf[i] == '^') { 
          state = IN_MSG; //IN_MSG 상태: "$" 문자 오면 다시 WAIT_FOR_MSG 상태로 돌아가고, 아니면 받은 문자에 1 더해서 클라이언트한테 다시 보냄
        }
        break;
      case IN_MSG:
        if (buf[i] == '$') {
          state = WAIT_FOR_MSG; //WAIT_FOR_MSG 상태: "^" 문자 올 때까지 무시 
        } else {
          buf[i] += 1;
          if (send(sockfd, &buf[i], 1, 0) < 1) {
            perror("send error");
            close(sockfd);
            return;
          }
        }
        break;
      }
    }
  }

  close(sockfd);
}

int main(int argc, char** argv) {
  setvbuf(stdout, NULL, _IONBF, 0);

  int portnum = 8080;
  if (argc >= 2) {
    portnum = atoi(argv[1]);
  }
  printf("Serving on port %d\n", portnum);

  //step1) 포트 번호로 socket 하나 구해오기 
  int sockfd = listen_inet_socket(portnum);

  while (1) {
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    //step2) 클라이언트 연결 w/ socket 기다림 
    int newsockfd =
        accept(sockfd, (struct sockaddr*)&peer_addr, &peer_addr_len);

    if (newsockfd < 0) {
      perror_die("ERROR on accept");
    }

    //step3) 연결된 클라이언트 정보 출력
    report_peer_connected(&peer_addr, peer_addr_len);

    //step4) 이 함수가 끝날 때까지 다른 클라이언트는 기다려야 함
    serve_connection(newsockfd);

    printf("peer done\n");
  }

  return 0;
}
