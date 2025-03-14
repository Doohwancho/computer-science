#include <iostream>
#include <string>

class Percentage {
private:
    double value_; // 0.0 ~ 1.0 사이 값
    
public:
    Percentage(double value = 0.0) {
        if (value < 0.0 || value > 1.0) {
            throw std::invalid_argument("Percentage must be between 0.0 and 1.0");
        }
        value_ = value;
    }
    
    // double로 변환 연산자
    operator double() const {
        return value_;
    }
    
    // int로 변환 연산자 (퍼센트 값으로 반환)
    explicit operator int() const {
        return static_cast<int>(value_ * 100);
    }
    
    // string으로 변환 연산자
    operator std::string() const {
        return std::to_string(static_cast<int>(value_ * 100)) + "%";
    }
};

int main() {
    Percentage p(0.75); // 75%
    
    // double 변환 사용
    double d = p; // 암시적 변환 허용
    std::cout << "As double: " << d << std::endl; // 0.75 출력
    
    // int 변환 사용
    // int i = p; // 에러: explicit 때문에 암시적 변환 불가
    int i = static_cast<int>(p); // 명시적 변환 필요
    std::cout << "As int: " << i << std::endl; // 75 출력
    
    // string 변환 사용
    std::string s = p; // 암시적 변환 허용
    std::cout << "As string: " << s << std::endl; // "75%" 출력
    
    // 수학 연산에서 암시적 변환
    double result = p * 200; // p가 double로 변환되고 계산됨
    std::cout << "p * 200 = " << result << std::endl; // 150 출력
    
    return 0;
}
