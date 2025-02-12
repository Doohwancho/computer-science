#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Common VT100 escape sequences
#define CLEAR_SCREEN "\x1b[2J"
#define CURSOR_HOME "\x1b[H"
#define CURSOR_UP "\x1b[1A"
#define CURSOR_DOWN "\x1b[1B"
#define CURSOR_RIGHT "\x1b[1C"
#define CURSOR_LEFT "\x1b[1D"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"

void clearScreen() {
    // Clear screen and position cursor at top-left
    write(STDOUT_FILENO, CLEAR_SCREEN, 4);
    write(STDOUT_FILENO, CURSOR_HOME, 3);
}

void moveCursor(int row, int col) {
    // Move cursor to specific position
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
    write(STDOUT_FILENO, buf, strlen(buf));
}

void drawBox(int startRow, int startCol, int width, int height) {
    // Hide cursor while drawing
    write(STDOUT_FILENO, HIDE_CURSOR, 6);
    
    // Draw top border
    moveCursor(startRow, startCol);
    printf("┌");
    for(int i = 0; i < width-2; i++) printf("─");
    printf("┐");
    
    // Draw sides
    for(int i = 1; i < height-1; i++) {
        moveCursor(startRow + i, startCol);
        printf("│");
        moveCursor(startRow + i, startCol + width-1);
        printf("│");
    }
    
    // Draw bottom border
    moveCursor(startRow + height-1, startCol);
    printf("└");
    for(int i = 0; i < width-2; i++) printf("─");
    printf("┘");
    
    // Show cursor again
    write(STDOUT_FILENO, SHOW_CURSOR, 6);
}

int main() {
    clearScreen();
    
    // Draw a box in the middle of the screen
    drawBox(5, 10, 20, 10);
    
    // Move cursor below the box
    moveCursor(16, 1);
    printf("Press Enter to exit...");
    
    getchar();
    clearScreen();
    return 0;
}
