#include <termios.h> // For terminal control functions
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // For UNIX standard functions like write()
#include <sys/ioctl.h> // For window size operations
#include <string.h>
#include <ctype.h> 

/*
---
New Features


1. 텍스트 입력기능 추가. look into struct erow 
2. 커서 이동 기능 추가. look into editorMoveCursor();. E.cx, E.cy ++, --으로 이동한다.


---
Takeaway


1. realloc()을 이럴 때 쓰는구나~
    - 기존에 E.row에 char[]을 덮어쓰기 할 때, 먼저 메모리 사이즈 공간을 재할당 받아야 하니까, realloc()을 쓴다. 

*/


// Data structure for a single line of text
typedef struct erow {
    int size;
    char *chars;
} erow;

// Editor configuration and state
struct editorConfig {
    int cx, cy;          // 커서 움직임. <-, -> 같은걸로 움직일 수 있음
    int rowoff;          // Row offset for scrolling - Q. 어케씀? A. 진짜는 20줄인데 화면은 10줄만 표시할 수 있을 때, rowoff = 5이면, 5번째부터 15까지 보여줌 
    int screenrows;      // window size x
    int screencols;      // window size y
    int numrows;         // Number of rows in file. ex) 아무글 안써져있으면 0, 5줄 써져있으면 4, 이런식. ex) E.row[numrows].변수 이런식으로 씀.
    erow *row;          // Array of rows
    struct termios orig_termios; //원래 terminal setting을 이 변수에 저장해둠.
};

//global variable for terminal
struct editorConfig E;


enum editorKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};


void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4); //clear screen (VT100)
    write(STDOUT_FILENO, "\x1b[H", 3); //move cursor to top left screen (VT100)
    perror(s);
    exit(1);
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
        die("tcsetattr");
}

//same from step1
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    
    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

// Read a key press, handling escape sequences
int editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1) die("read");
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return ARROW_UP;
                case 'B': return ARROW_DOWN;
                case 'C': return ARROW_RIGHT;
                case 'D': return ARROW_LEFT;
            }
        }
        return '\x1b';
    }
    return c;
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

// Add a row to the text buffer
void editorAppendRow(char *s, size_t len) {
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1)); //아! 이 때 realloc()을 쓰는구나~
    
    int at = E.numrows;
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len); //파라미터로 받은 s를 E.row[at]에 len 길이 만큼, insert
    E.row[at].chars[len] = '\0'; //string에 맨 마지막에 \0 입력은 국룰
    E.numrows++; //엔터 누르기
}

// Initialize the editor
void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.rowoff = 0;
    E.numrows = 0;
    E.row = NULL;
    
    //same from step2
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}

//모든 줄을 다시 그리기 
void editorDrawRows() {
    //터미널의 각 행을 순회한다.
    for (int y = 0; y < E.screenrows; y++) { 
        // 현재 화면상의 y위치에 표시될 실제 파일에서의 행 번호를 계산
        // rowoff는 스크롤 오프셋으로, 화면이 스크롤되면 증가함
        int filerow = y + E.rowoff; 
        
        // 만약 파일의 실제 행 수보다 현재 그리려는 행 번호가 크다면
        // (즉, 파일의 끝을 넘어섰다면)
        if (filerow >= E.numrows) { 
            // 파일이 비어있고(numrows=0), 화면의 1/3 지점이라면
            if (E.numrows == 0 && y == E.screenrows / 3) {
                // 환영 메시지를 표시
                char welcome[80];
                int welcomelen = snprintf(welcome, sizeof(welcome),
                    "Mini editor -- version 0.0.1");
                
                // 환영 메시지가 화면 너비보다 길다면 자름
                if (welcomelen > E.screencols) welcomelen = E.screencols;

                // 환영 메시지를 화면 중앙에 표시하기 위한 패딩 계산
                int padding = (E.screencols - welcomelen) / 2;

                // 첫 번째 '~' 표시
                if (padding) {
                    write(STDOUT_FILENO, "~", 1);
                    padding--;
                }

                // 패딩만큼 공백 추가
                while (padding--) write(STDOUT_FILENO, " ", 1);

                // 환영 메시지 출력
                write(STDOUT_FILENO, welcome, welcomelen);
            } else {
                // 파일의 끝 이후의 빈 줄에는 '~' 표시 (vim 스타일)
                write(STDOUT_FILENO, "~", 1);
            }
        } else {
            // 실제 파일 내용을 표시하는 부분
            int len = E.row[filerow].size;
            // 화면 너비보다 길다면 자름
            if (len > E.screencols) len = E.screencols;
            // 해당 줄의 내용을 출력
            write(STDOUT_FILENO, E.row[filerow].chars, len);
        }

        // 현재 줄의 나머지 부분을 지움 (VT100 이스케이프 시퀀스)
        write(STDOUT_FILENO, "\x1b[K", 3); 
        
        // 마지막 줄이 아닐 때만 줄바꿈을 출력
        if (y < E.screenrows - 1) {
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

//Q.? 왜함?
void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[?25l", 6); // Hide cursor (VT100)
    write(STDOUT_FILENO, "\x1b[H", 3);    // Move cursor to top-left (VT100)
    
    editorDrawRows(); //모든 줄을 다시 그리기 
    
    // Position cursor
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1); //커서를 올바른 위치에 이동시키기 (VT100)
    write(STDOUT_FILENO, buf, strlen(buf)); 
    
    write(STDOUT_FILENO, "\x1b[?25h", 6); // 커서 이동 후, 커서를 다시 보이게 하기 (VT100)
}

void editorMoveCursor(int key) {
    switch (key) {
        case ARROW_LEFT:
            if (E.cx != 0) E.cx--;
            break;
        case ARROW_RIGHT:
            if (E.cx != E.screencols - 1) E.cx++;
            break;
        case ARROW_UP:
            if (E.cy != 0) E.cy--;
            break;
        case ARROW_DOWN:
            if (E.cy < E.numrows) E.cy++;
            break;
    }
}

void editorProcessKeypress() {
    int c = editorReadKey();
    
    switch (c) {
        case 'q':
            write(STDOUT_FILENO, "\x1b[2J", 4);  //아마 clear terminal 
            write(STDOUT_FILENO, "\x1b[H", 3);  //아마 move cursor to top left 
            exit(0);
            break;
            
        //arrow movement
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
    }
}

int main() {
    enableRawMode();
    initEditor();
    
    // Add some test rows
    editorAppendRow("Welcome to the editor!", 21); //Q. ? - 왜 첫번째 줄 print 안되지?
    editorAppendRow("Use arrow keys to move cursor.", 29);
    editorAppendRow("Press 'q' to quit.", 17);
    
    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }
    
    return 0;
}
