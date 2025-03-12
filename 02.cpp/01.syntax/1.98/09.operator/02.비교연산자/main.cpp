#include <iostream>
#include <string>

class Person {
private:
    std::string name_;
    int age_;
    
public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}
    
    // == 연산자 오버로딩
    bool operator==(const Person& other) const {
        return (name_ == other.name_ && age_ == other.age_);
    }
    
    // != 연산자 오버로딩
    bool operator!=(const Person& other) const {
        return !(*this == other); // == 연산자 재활용
    }
    
    // < 연산자 오버로딩 (이름 알파벳순, 이름 같으면 나이순)
    bool operator<(const Person& other) const {
        if (name_ != other.name_) {
            return name_ < other.name_;
        }
        return age_ < other.age_;
    }
    
    // > 연산자 오버로딩
    bool operator>(const Person& other) const {
        return other < *this; // < 연산자 재활용
    }
    
    // <= 연산자 오버로딩
    bool operator<=(const Person& other) const {
        return !(other < *this); // < 연산자 재활용
    }
    
    // >= 연산자 오버로딩
    bool operator>=(const Person& other) const {
        return !(*this < other); // < 연산자 재활용
    }
    
    // 출력용
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name_ << " (" << p.age_ << ")";
        return os;
    }
};

int main() {
    Person p1("Kim", 25);
    Person p2("Lee", 30);
    Person p3("Kim", 25);
    Person p4("Kim", 28);
    
    // 비교 연산자 사용
    std::cout << "p1 == p3: " << (p1 == p3) << std::endl; // 1 (true)
    std::cout << "p1 != p2: " << (p1 != p2) << std::endl; // 1 (true)
    std::cout << "p1 < p2: " << (p1 < p2) << std::endl;   // 1 (true, K < L)
    std::cout << "p2 > p1: " << (p2 > p1) << std::endl;   // 1 (true, L > K)
    std::cout << "p1 < p4: " << (p1 < p4) << std::endl;   // 1 (true, 같은 이름이지만 25 < 28)
    
    return 0;
}
