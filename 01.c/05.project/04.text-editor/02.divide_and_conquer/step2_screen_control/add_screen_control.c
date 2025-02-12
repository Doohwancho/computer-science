#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

/*
---
New Features from step1


1. Screen size detection
    - look into getWindowSize();
2. Basic screen clearing and drawing
    - Proper screen refresh: while(1)로 계속, VT100으로 terminal clearing 하고, char[]을 찍어준다.
3. A global editor config structure
    - 글로벌 변수에서 전체 세팅과 어떤 char[]이 들어가는지 관리 
4. Tilde drawing like vim/nano (optional)
    - 각 row가 비어있으면 ~로 채워준다.
*/

//terminal config setting storage 
struct termios orig_termios;

//store window x,y dimension
struct editorConfig { //Q. 어느 라이브러리에서 씀?
    int screenrows;
    int screencols;
};

// global instance of editor configuration
struct editorConfig E;


void die(const char *s) {
    // Clear screen on exit
    write(STDOUT_FILENO, "\x1b[2J", 4); //? -> VT100 'escape' sequence \n1x[2J
    write(STDOUT_FILENO, "\x1b[H", 3);  //? -> VT100 'move cursor to top left' sequence \x1b[H 
    
    perror(s);
    exit(1);
}

//restore original terminal setting
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

//same from step1
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws; //winsize 어느 lib에서 씀?
    //TIOCGWINSZ: gets window size for ioctl 
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) { //Q. sys/ioctl.h -> sys/ttycom.h -> winsize.h 씀 
        return -1;
    } else {
        *cols = ws.ws_col; //window column size
        *rows = ws.ws_row; //window row size
        return 0;
    }
}

void initEditor() {
    //get the window size of terminal 
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}

//매 row 첫번쨰에 ~ 쓰기 
void editorDrawRows() {
    for (int y = 0; y < E.screenrows; y++) {
        //매 row 첫 character에 ~ 쓰기
        write(STDOUT_FILENO, "~\r\n", 3); //Q. 두번째 파라미터 뭐지?
    }
}

void editorRefreshScreen() {
    // Clear screen
    write(STDOUT_FILENO, "\x1b[2J", 4); //Q. 두번째 파라미터 뭐지? A. clear entire screen (VT100)
    write(STDOUT_FILENO, "\x1b[H", 3); //Q. 두번쨰 파라미터 뭐지? A. move cursor to top-left (VT100)
    
    editorDrawRows();
    
    // Move cursor to top-left
    write(STDOUT_FILENO, "\x1b[H", 3); //Q. 두번째 파라미터 뭐지? A. move cursor to top-left (VT100)
}

int main() {
    enableRawMode();
    initEditor();
    
    while (1) {
        editorRefreshScreen();
        
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1) die("read");
        if (c == 'q') break; //quit on pressing 'q'
    }
    
    return 0;
}
