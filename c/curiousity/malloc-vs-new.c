#include <stdio.h>
#include <stdlib.h>


/*******************************************/
//Q1. What is the difference between malloc and new?


//case1)
//they both allocate memory from heap,
//but malloc requires the user to specify the size of the memory block to be allocated,
//while new does not require the user to specify the size of the memory block to be allocated. -> new is more convenient.

//malloc
int *a = (int*)malloc(sizeof(int));
*a = 5;

// same example using new:
int *b = new int;
*b = 5;




//case2)
//malloc returns a void pointer, while new returns a pointer of the type of the object being allocated. -> new is more convenient.
//show me an example in code
int main()
{
    int *p = (int *)malloc(sizeof(int)); //malloc returns a void pointer, so we need to cast it to int pointer
    int *q = new int; //new returns a pointer of the type of the object being allocated, so we don't need to cast it to int pointer
    return 0;
}





//case3)
//malloc does not call the constructor of the object being allocated, while new does.
//show me an example in code

class Foo
{
public:
    Foo(int x): m_x(x) {}
    int m_x;
};

int main()
{
    Foo *p = (Foo *)malloc(sizeof(Foo)); //malloc does not call the constructor of the object being allocated. 아 malloc은 걍 heap에서 메모리를 input size만큼 만 가져오는 일을 하는거군. 딱 거기까지.
                                         //메모리만 가져오고 그 메모리를 초기화해주는 일은 하지 않음. 그래서 Foo의 constructor를 호출하지 않음
                                         //그래서 Foo의 constructor를 호출해주려면, malloc으로 메모리를 가져온 다음에, 그 메모리를 Foo의 constructor에 넣어주면 됨.
                                         //메모리만 덜렁 가져오면 이게 뭔질 모르니까 Foo로 캐스팅을 해준 상태이고, 종종 뭘 넣을지 안정해진 상황에서는 void pointer 넣는 것 같음.
                                         // foo->m_x is not initialized //foo->m_x는 초기화가 안되어있음. m_x는 garbage value
    Foo *q = new Foo(5); //new calls the constructor of the object being allocated -> 아 메모리 Foo 사이즈 만큼 할당받고 초기화 하고 Foo의 Constructor까지 넣어주는게 new 구나.
    return 0;
}






/*******************************************/
//Q2. What is the difference between free and delete?


//free is a function that frees the memory allocated by malloc,
//while delete is a function that frees the memory allocated by new.

//free requires the user to specify the memory block to be freed,
//while delete does not require the user to specify the memory block to be freed. -> delete is more convenient.



//malloc
int *a = (int*)malloc(sizeof(int));
*a = 5;
free(a);

// same example using new:
int *b = new int;
*b = 5;
delete b;


