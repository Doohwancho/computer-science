#include <iostream>

class Counter {
private:
    int count_;
    
public:
    Counter(int count = 0) : count_(count) {}
    
    // 전위 증가 연산자 (prefix ++)
    Counter& operator++() {
        ++count_;
        return *this;
    }
    
    // 후위 증가 연산자 (postfix ++)
    // 주목: int 매개변수는 전위/후위 구분용
    Counter operator++(int) {
        Counter temp = *this;
        ++count_;
        return temp;
    }
    
    // 전위 감소 연산자 (prefix --)
    Counter& operator--() {
        --count_;
        return *this;
    }
    
    // 후위 감소 연산자 (postfix --)
    Counter operator--(int) {
        Counter temp = *this;
        --count_;
        return temp;
    }
    
    // 현재 값 반환
    int getValue() const {
        return count_;
    }
};

int main() {
    Counter c1(5);
    
    // 전위 증가 연산자
    ++c1;
    std::cout << "After ++c1: " << c1.getValue() << std::endl; // 6 출력
    
    // 후위 증가 연산자
    Counter c2 = c1++;
    std::cout << "c2 value: " << c2.getValue() << std::endl; // 6 출력 (증가 전 값)
    std::cout << "c1 after c1++: " << c1.getValue() << std::endl; // 7 출력
    
    // 전위 감소 연산자
    --c1;
    std::cout << "After --c1: " << c1.getValue() << std::endl; // 6 출력
    
    // 후위 감소 연산자
    Counter c3 = c1--;
    std::cout << "c3 value: " << c3.getValue() << std::endl; // 6 출력 (감소 전 값)
    std::cout << "c1 after c1--: " << c1.getValue() << std::endl; // 5 출력
    
    return 0;
}
