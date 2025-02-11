#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Q. why typedef struct over just struct?
//A. 선언시 계속 struct Person 해야하는 수고를 덜어줌.

struct Person {
    char name[50];
    int age;
    float height;
};

typedef struct Person2 { //그냥 typedef struct {} Person2; 도 됨
    char name[50];
    int age;
    float height;
} Person2;

int main() {
    struct Person p1 = {"John", 25, 1.75};
    Person2 p2 = {"Bar", 25, 1.75}; //struct로 instance를 만든것
    Person2* p3;
    Person2* p4 = (Person2*)malloc(sizeof(Person2)); //pointer to instance made by Person2 struct. 

    printf("%s %d %f\n", p1.name, p1.age, p1.height); //John 25 1.75
    printf("%s %d %f\n", p2.name, p2.age, p2.height); //Bar 25 1.75
    printf("%d \n", p3); //0
    printf("%d \n", p4); //메모리 공간 

    strcpy(p1.name, "Jane");
    p1.age = 30;
    p1.height = 3.24;

    strcpy(p2.name, "Peter");
    p2.age = 50;
    p2.height = 8.24;

    // p4->name = "Jill"; //error - char[50] is not assignable to char*
    strcpy(p4->name, "Jill");
    p4->age = 60;
    p4->height = 120;

    printf("%s %d %f\n", p1.name, p1.age, p1.height); //Jane 30 3.24
    printf("%s %d %f\n", p2.name, p2.age, p2.height); //Peter 50 8.24
    printf("%s %d %f\n", p4->name, p4->age, p4->height); //Jill 60 120

    return 0;
}