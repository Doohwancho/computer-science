---\
How 


1. run server 
2. send http_request to get static file 
```
curl http://localhost:8080/
curl http://localhost:8080/style.css
```


---\
What 


add static file serving feature 


===\
새롭게 추가된 libs 활용사례 

```c
// 파일 정보 얻기
struct stat file_stat;
if (stat(filepath, &file_stat) == -1) {
    // 파일이 없거나 접근 불가
    return;
}

// Content-Length 헤더를 위한 파일 크기
size_t content_length = file_stat.st_size;

// Last-Modified 헤더를 위한 수정 시간
char modified[100];
struct tm* tm_info = gmtime(&file_stat.st_mtime);
strftime(modified, sizeof(modified), 
         "%a, %d %b %Y %H:%M:%S GMT", 
         tm_info);

// 파일 열고 읽기
int fd = open(filepath, O_RDONLY);
if (fd == -1) {
    // 파일 열기 실패
    return;
}

char buffer[BUFFER_SIZE];
ssize_t bytes_read;
while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
    // 클라이언트에게 파일 데이터 전송
    send(client_fd, buffer, bytes_read, 0);
}
```



---\
`<sys/stat.h>` 


파일의 상태와 정보를 다루는 라이브러리

```c
// 주요 함수:
stat(filepath, &file_stat)  // 파일의 메타데이터 가져오기
// file_stat에서 얻을 수 있는 정보들:
file_stat.st_size    // 파일 크기
file_stat.st_mode    // 파일 타입과 권한
file_stat.st_mtime   // 마지막 수정 시간

// 파일 타입 체크 매크로:
S_ISREG(file_stat.st_mode)  // 일반 파일인지 확인
S_ISDIR(file_stat.st_mode)  // 디렉토리인지 확인
```

---\
`<sys/types.h>`

시스템 타입 정의

```c
// 주요 타입들:
off_t     // 파일 오프셋
size_t    // 크기
ssize_t   // 부호있는 크기 (read/write 리턴값)
mode_t    // 파일 모드와 권한
time_t    // 시간값
```


---\
`<fcntl.h>`

파일 제어 옵션과 함수

```c
// 주요 함수:
open(filepath, O_RDONLY)  // 파일 열기
// 주요 플래그:
O_RDONLY   // 읽기 전용
O_WRONLY   // 쓰기 전용
O_RDWR     // 읽기/쓰기
O_CREAT    // 파일 생성
O_APPEND   // 추가 모드
```


---\
`<time.h>`


시간 관련 함수들 

```c
// 주요 함수들:
time()       // 현재 시간 얻기
gmtime()     // time_t를 UTC 시간 구조체로 변환
strftime()   // 시간을 문자열로 포맷팅

// HTTP 응답 헤더에서 사용:
time_t now = time(NULL);
struct tm* tm_info = gmtime(&now);
char date[100];
strftime(date, sizeof(date), 
         "%a, %d %b %Y %H:%M:%S GMT",  // HTTP 날짜 형식
         tm_info);
```
