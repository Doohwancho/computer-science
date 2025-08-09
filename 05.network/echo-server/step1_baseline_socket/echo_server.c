#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

/*
```
gcc -o echo_server echo_server.c
./echo_server

curl http://localhost:8080/

ab -n 100 -c 1 http://localhost:8080/
```

*/


int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    
    // HTTP 응답 형식: 상태 라인, 헤더, 빈 라인, 본문
    char *http_response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/plain\r\n"
                          "Content-Length: 13\r\n"
                          "Connection: close\r\n"
                          "\r\n"
                          "Hello, World!";
    
    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // 소켓 옵션 설정 (재사용 활성화)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 모든 인터페이스에서 연결 허용
    server_addr.sin_port = htons(PORT);        // 포트 설정
    
    // 소켓에 주소 바인딩
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 연결 대기 시작
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    // 무한 루프로 클라이언트 요청 처리
    while (1) {
        // 클라이언트 연결 수락
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }
        
        // 클라이언트 IP 주소 출력
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connected: %s\n", client_ip);
        
        // 클라이언트로부터 데이터 수신
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Received request: %.50s...\n", buffer);
        
        // HTTP GET 요청 확인 (간단한 확인)
        if (strncmp(buffer, "GET /", 5) == 0) {
            // 응답 전송
            write(client_fd, http_response, strlen(http_response));
            printf("Response sent\n");
        }
        
        // 연결 닫기
        close(client_fd);
    }
    
    // 메인 소켓 닫기 (실제로는 여기까지 실행되지 않음)
    close(server_fd);
    return 0;
}
