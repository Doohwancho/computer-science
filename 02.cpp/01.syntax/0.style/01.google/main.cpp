#include <iostream>
#include <string>

//Q. 구글 스타일 c++?
//A. 멤버변수 뒤에 _를 붙여서 함수 안에 파라미터/로컬 별수랑 구분을 둔다. 


class Person {
private:
    std::string name_;
    int age_;
    const int id_;  // const 멤버는 초기화 리스트로만 초기화 가능
    
public:
    // 초기화 리스트 사용 (콜론 뒤에 나오는 부분)
    Person(std::string name, int age, int id)
        : name_(std::move(name)), age_(age), id_(id)  // 초기화 리스트. std::move()가 이 때 쓰이는구나. 원래 string name 없애야 하니까. 
    {
        // 생성자 본문
        std::cout << "Person 생성됨: " << name_ << std::endl;
        
        // 여기서 id_ = id; 하면 컴파일 에러 (const 변수라서)
    }
    
    // 멤버 변수 많으면 이렇게 포맷팅하기도 함
    Person(int id)
        : name_("Unknown")  // 문자열 초기화
        , age_(0)           // 정수 초기화
        , id_(id)           // const 멤버 초기화
    {
        // 생성자 본문
        std::cout << "Person 생성됨: " << name_ << std::endl;
        
    }

    void printPerson() {
        std::cout << name_ << std::endl;
        std::cout << age_ << std::endl;
        std::cout << id_ << std::endl;
    }
};


int main () {
    Person p1 = Person("hello", 10, 1);
    Person p2 = Person(2);

    p1.printPerson();
    p2.printPerson();

    return 0;
}
