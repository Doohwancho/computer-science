#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define BACKLOG 128
#define MAX_EVENTS 1024
#define BUFFER_SIZE 4096

// HTTP 응답 템플릿
static const char* http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";

// 클라이언트 연결 정보를 관리하는 구조체
typedef struct {
    int fd;
    char buffer[BUFFER_SIZE];
    size_t buffer_len;
} client_state;

// 전역 변수
static client_state *clients[FD_SETSIZE];
static int server_socket;
static int kq;
static long long total_requests = 0;
static long long current_requests = 0;
static time_t last_report_time = 0;

// 소켓을 논블로킹 모드로 설정하는 함수
int set_nonblock(int fd) {
    int flags;
    
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    
    return 0;
}

// 클라이언트 상태 생성 함수
client_state* create_client(int fd) {
    client_state* client = malloc(sizeof(client_state));
    if (!client) {
        perror("malloc");
        return NULL;
    }
    
    client->fd = fd;
    client->buffer_len = 0;
    memset(client->buffer, 0, BUFFER_SIZE);
    
    return client;
}

// 클라이언트 상태 해제 함수
void free_client(client_state* client) {
    if (client) {
        close(client->fd);
        free(client);
    }
}

// 새 클라이언트 연결 처리 함수
void handle_new_connection() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;
    
    // 새 연결 수락
    client_fd = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept");
        }
        return;
    }
    
    // 논블로킹 모드로 설정
    if (set_nonblock(client_fd) == -1) {
        close(client_fd);
        return;
    }
    
    // 클라이언트 상태 생성
    client_state *client = create_client(client_fd);
    if (!client) {
        close(client_fd);
        return;
    }
    
    // 클라이언트 상태 저장
    if (client_fd >= FD_SETSIZE) {
        fprintf(stderr, "Client fd (%d) exceeds limit\n", client_fd);
        free_client(client);
        return;
    }
    clients[client_fd] = client;
    
    // kqueue에 클라이언트 소켓 이벤트 등록
    struct kevent kev;
    EV_SET(&kev, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        free_client(client);
        clients[client_fd] = NULL;
        return;
    }
    
    printf("Accepted connection from %s:%d (fd: %d)\n", 
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);
}

// 클라이언트로부터 데이터 처리 함수
void handle_client_data(int fd) {
    client_state *client = clients[fd];
    if (!client) return;
    
    char buffer[BUFFER_SIZE];
    ssize_t n = read(fd, buffer, BUFFER_SIZE - 1);
    
    if (n <= 0) {
        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            return;
        }
        
        // 연결 종료 또는 오류
        struct kevent kev;
        EV_SET(&kev, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(kq, &kev, 1, NULL, 0, NULL);
        
        free_client(client);
        clients[fd] = NULL;
        return;
    }
    
    buffer[n] = '\0';
    
    // HTTP 요청 처리 (단순화: 요청 내용 검사 없이 바로 응답)
    if (write(fd, http_response, strlen(http_response)) < 0) {
        perror("write");
    }
    
    // 연결 종료
    struct kevent kev;
    EV_SET(&kev, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kq, &kev, 1, NULL, 0, NULL);
    
    free_client(client);
    clients[fd] = NULL;
    
    // 요청 카운터 증가
    total_requests++;
    current_requests++;
    
    // 초당 요청 수 출력
    time_t now = time(NULL);
    if (now != last_report_time) {
        printf("Requests per second: %lld, Total: %lld\n", current_requests, total_requests);
        current_requests = 0;
        last_report_time = now;
    }
}

// 서버 초기화
int init_server() {
    struct sockaddr_in server_addr;
    
    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return -1;
    }
    
    // 소켓 옵션 설정 (주소 재사용)
    int yes = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        close(server_socket);
        return -1;
    }
    
    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    
    // 소켓에 주소 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        return -1;
    }
    
    // 논블로킹 모드로 설정
    if (set_nonblock(server_socket) == -1) {
        close(server_socket);
        return -1;
    }
    
    // 연결 대기열 설정
    if (listen(server_socket, BACKLOG) == -1) {
        perror("listen");
        close(server_socket);
        return -1;
    }
    
    // kqueue 생성
    kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        close(server_socket);
        return -1;
    }
    
    // kqueue에 서버 소켓 이벤트 등록
    struct kevent kev;
    EV_SET(&kev, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        close(kq);
        close(server_socket);
        return -1;
    }
    
    printf("Server listening on port %d\n", SERVER_PORT);
    return 0;
}

int main() {
    // 클라이언트 배열 초기화
    memset(clients, 0, sizeof(clients));
    
    // 서버 초기화
    if (init_server() == -1) {
        return 1;
    }
    
    // 이벤트 루프
    struct kevent events[MAX_EVENTS];
    
    last_report_time = time(NULL);
    
    while (1) {
        int nevents = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (nevents == -1) {
            perror("kevent wait");
            break;
        }
        
        for (int i = 0; i < nevents; i++) {
            int fd = events[i].ident;
            
            if (fd == server_socket) {
                // 새 연결 처리
                handle_new_connection();
            } else {
                // 클라이언트 데이터 처리
                handle_client_data(fd);
            }
        }
    }
    
    // 정리
    close(kq);
    close(server_socket);
    
    return 0;
}
