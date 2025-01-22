#include <stdio.h>
#include <stdlib.h>

// Class definition
typedef struct {
    int x;
    int y;
    //Point *(*new_point)(int x, int y); //error! cannot declare function pointer in struct. Pointer가 아직 declared되지 않았기 때문에.
    //void (*print_point)(struct Point *p); // warning: declaration of 'struct Point' will not be visible outside of this function
    //void print(); //error: field 'print' declared as a function
} Point;

// Constructor
Point *new_point(int x, int y) {
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

// Method
void print_point(Point *p) {
    printf("Point at (%d, %d)\n", p->x, p->y);
}

//print hello World
void print() {
    printf("Hello World");
}

int main() {
    Point *p1 = new_point(0, 0);
    Point *p2 = new_point(10, 10);
    print_point(p1);
    print_point(p2);

    //call print() method inside *p1
    //p1->print(); //error: field 'print' declared as a function

    return 0;
}



// 신기한 점


// 1. class를 정의할 때는 struct를 사용한다.
// 2. struct의 생성자는 struct 밖에서* new_클래스명()으로 정의한다.
//      -> *new_point() 함수 안 malloc() 으로 대체
//      -> struct의 생성자는 struct안에 들어가지 않는다. 생성자는 struct를 정의하는 것이 아니라, struct를 생성. 생성자는 struct의 포인터를 return한다.
//      -> Q. 생성자를 안에 선언하냐 밖에 선언하냐의 차이가 struct와 class의 차이인건가?

// 3. struct의 method는 struct안에 선언한다. method는 struct를 생성하는 것이 아니라, struct를 조작하는 것이다.
//      method는 struct의 포인터를 parameter로 받는다. 그러므로 method를 호출할 때는 struct의 포인터를 parameter로 넘겨준다.



/******************************************************/
//Q. is class just syntatic sugar of struct? what are the differences?


//A. No, classes and structs are different.



//1. Classes are reference types, and structs are value types.
//   This means that when a variable of a class type is assigned to another variable, they both refer to the same object in memory.
//   With structs, a new copy of the object is created and assigned to the second variable.

//2. Classes can have constructors, destructors, and other methods, but structs cannot.

//3. Classes can inherit from other classes, but structs cannot.

//4. Classes can be declared as abstract, but structs cannot.

//5. Classes can have static members, but structs cannot.

//6. Classes can have explicit access modifiers, but structs cannot.
//      -> public, private, protected, internal, protected internal, private protected

//7. Classes can have explicit interface implementations, but structs cannot.
//
//8. class can be sealed, but structs cannot
//      -> sealed class: A sealed class cannot be inherited.

