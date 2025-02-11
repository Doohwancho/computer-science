#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int age;
    float gpa;
} Student;

Student* createStudent(const char* name, int age, float gpa) { //Q. why const? -> 원본 name값 안바꾸려고?
    Student* student = (Student*)malloc(sizeof(Student));
    if(student == NULL){
        printf("Memory Allocation Failed.\n");
        exit(1);
    }

    strcpy(student->name, name); //Q. 왜 -> 이지? .가 아니라? A. struct/step1 참조. struct Student로 만든 instance의 포인터가 student인데, 포인터에서 variable 참조하려면 ->를 쓰고, 포인터가 아닌 instance에서 값을 참조할 땐 .을 써서 접근함.
    student->age = age;
    student->gpa = gpa;

    return student;
}

void printStudent(const Student* student) { //Q. why const? -> 이 데이터 변경 불가능 하다는 뜻. 이 구조체 내용 변경 불가능. read only. good coding practice.
    printf("name: %s \n", student->name);
    printf("age: %d \n", student->age);
    printf("gpa: %.2f \n", student->gpa);
}

int main() {
    char name[10] = "Foo"; //malloc()으로 안하고 바로 "Foo" 입력 가능하구만. 
    // char* name = "John";
    int age = 10;
    float gpa = 4.0;

    Student* student = createStudent(name, age, gpa); 

    printStudent(student);

    free(student);

    return 0;
}
