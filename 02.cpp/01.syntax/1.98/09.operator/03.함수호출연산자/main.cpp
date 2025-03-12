#include <iostream>
#include <vector>
#include <functional>

// 함수 객체 (Functor) 만들기
class Adder {
private:
    int initial_;
    
public:
    Adder(int initial = 0) : initial_(initial) {}
    
    // 함수 호출 연산자 () 오버로딩
    int operator()(int a, int b) const {
        return initial_ + a + b;
    }
};

// 람다식과 함수 객체 비교
int main() {
    // 1. 함수 객체 사용
    Adder add5(5);
    std::cout << "add5(10, 20): " << add5(10, 20) << std::endl; // 35 출력
    
    // 2. 다른 함수 객체 인스턴스
    Adder add10(10);
    std::cout << "add10(10, 20): " << add10(10, 20) << std::endl; // 40 출력
    
    // 3. STL 알고리즘에서 함수 객체 사용
    std::vector<int> nums = {1, 2, 3, 4, 5};
    int sum = 0;
    
    // 함수 객체를 std::for_each에 전달
    std::for_each(nums.begin(), nums.end(), [&sum](int n) {
        sum += n;
    });
    
    std::cout << "Sum: " << sum << std::endl; // 15 출력
    
    return 0;
}
