#include <iostream>

// explicit 없는 클래스
class NonExplicitClass {
public:
    NonExplicitClass(int x) {
        std::cout << "정수 " << x << "로 객체 생성됨" << std::endl;
    }
};

// explicit 있는 클래스
class ExplicitClass {
public:
    explicit ExplicitClass(int x) {
        std::cout << "정수 " << x << "로 객체 생성됨" << std::endl;
    }
};

// 함수들
void takeNonExplicit(NonExplicitClass obj) {
    std::cout << "NonExplicitClass 객체 받음" << std::endl;
}

void takeExplicit(ExplicitClass obj) {
    std::cout << "ExplicitClass 객체 받음" << std::endl;
}

int main() {
    // explicit 없는 경우: 암시적 변환 허용
    takeNonExplicit(42);  // 컴파일러가 int 42를 객체인 NonExplicitClass(42)로 자동 변환해줌. 
                          // 42를 넣었는데 함수 파라미터가 객체면 좀 이상하잖아? 
                          // 이런 컴파일러의 '암시적 변환' 있는 코드가 읽을 때 헤깔리고, 의도치 않은 버그를 발생할 수 있으니까, 
                          // explicit으로 명시하는 것 
    
    // explicit 있는 경우: 암시적 변환 막힘
    // takeExplicit(42);  // 컴파일 에러!
    
    // 명시적으로 변환하면 가능
    takeExplicit(ExplicitClass(42));  // OK
    
    return 0;
}
