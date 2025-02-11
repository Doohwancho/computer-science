#include <stdio.h>

// takeaway
// 1. call by value: 원본에는 변화 없음.
// 2. call by reference: 원본에 변화 있음.
// 3. movePointByReference(&p, 1, 2); 할 때, Point p에 주솟값을 넘기면, 
//    movePointByReference(Point* p, int dx, int dy); 에서 Point* p라는 포인터 변수를 만들고, 포인터의 값으로 저 주솟값을 할당함. 그 후 그 원본 주솟값을 ->를 이용해 값을 update 함.

typedef struct {
    int x;
    int y;
} Point;

//case1) call by value -> 원본에는 변화 없음.
void movePointByValue(Point p, int dx, int dy) {
    p.x += dx; //Q. 이건 왜 p.x인데, 아래는 왜 p->x지?
               //A. .은 구조체 "변수"에서 접근할 떄 쓰고, ->은 구조체 "포인터"에서 접근할 떄 씀
               //ex. p1*->x 이거나, p1.x 이거나.
    p.y += dy;
}

//case2) call by reference -> 원본에 변화 있음.
void movePointByReference(Point* p, int dx, int dy) { //p에 주솟값을 포인터로 받아와서 해당 원본 주솟값의 값을 바꾸는 것 
    p->x += dx;
    p->y += dy;
}

int main() {
    Point p1 = {0, 0};

    movePointByValue(p1, 1, 2);
    printf("p1.x = %d, p1.y = %d\n", p1.x, p1.y); // p1.x = 0, p1.y = 0
                                                  // Q. 왜 p1->x가 아니라 p1.x지?
                                                  // A. .은 구조체 "변수"에서 접근할 떄 쓰고, ->은 구조체 "포인터"에서 접근할 떄 씀
                                                  // ex. p1*->x 이거나, p1.x 이거나.

    movePointByReference(&p1, 3, 4);
    printf("p1.x = %d, p1.y = %d\n", p1.x, p1.y); // p1.x = 3, p1.y = 4

    return 0;
}
