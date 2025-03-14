#include <iostream>

class A {
  int *data;

 public:
  A() {
    data = new int[100];
    std::cout << "자원을 획득함!" << std::endl;
  }

  ~A() {
    std::cout << "소멸자 호출!" << std::endl;
    delete[] data;
  }
};

void do_something() { A *pa = new A(); }

int main() {
  do_something();

  // 문제! 할당된 객체가 소멸되지 않음! 
  // delete pa; 가 호출되지 않았기 때문 
  // 즉, 400 바이트 (4 * 100) 만큼의 메모리 누수 발생
}
