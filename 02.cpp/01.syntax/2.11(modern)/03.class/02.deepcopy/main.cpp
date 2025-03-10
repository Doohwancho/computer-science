#include <iostream>
#include <vector>

class MyString {
private:
    char* data;
    size_t length;
    
public:
    // 일반 생성자 ㅇㅇ
    MyString(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        std::cout << "일반 생성자: " << data << std::endl;
    }
    
    // 복사 생성자 - 깊은 복사 수행 ㅇㅇ
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        std::cout << "복사 생성자: " << data << std::endl;
    }
    
    // 이동 생성자 - 리소스 훔쳐옴 ㅇㅇ
    MyString(MyString&& other) noexcept {
        // 다른 객체의 자원 가져오기
        data = other.data;
        length = other.length;
        
        // 원본 객체는 자원 포기 (이제 소유권 없음)
        other.data = nullptr;
        other.length = 0;
        
        std::cout << "이동 생성자: " << data << std::endl;
    }
    
    // 소멸자 ㅇㅇ
    ~MyString() {
        std::cout << "소멸자 호출: ";
        if (data) {
            std::cout << data << std::endl;
            delete[] data;
        } else {
            std::cout << "빈 문자열" << std::endl;
        }
    }
    
    // 복사 대입 연산자 ㅇㅇ
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            std::cout << "복사 대입: " << data << std::endl;
        }
        return *this;
    }
    
    // 이동 대입 연산자 ㅇㅇ
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            
            // 자원 훔쳐오기
            data = other.data;
            length = other.length;
            
            // 원본 객체는 자원 포기
            other.data = nullptr;
            other.length = 0;
            
            std::cout << "이동 대입: " << data << std::endl;
        }
        return *this;
    }
    
    size_t size() const {
        return length;
    }
};

// 값 리턴하는 함수 ㅇㅇ
MyString createString(const char* str) {
    MyString temp(str);
    return temp;  // 여기서 이동 생성자 쓰임 (RVO 없을 때) ㅇㅇ
}

int main() {
    std::cout << "=== 기본 생성자 ===" << std::endl;
    MyString s1("안녕");
    
    std::cout << "\n=== 복사 생성자 ===" << std::endl;
    MyString s2 = s1;  // 복사 생성자 호출 ㅇㅇ
    
    std::cout << "\n=== 이동 생성자 ===" << std::endl;
    MyString s3 = std::move(s1);  // 이동 생성자 호출 (s1은 이제 비어있음) ㅇㅇ
    
    std::cout << "\n=== 함수 리턴값 ===" << std::endl;
    MyString s4 = createString("Hello");  // 이동 생성자 쓰일 수 있음 (컴파일러 최적화에 따라 다름) ㅇㅇ
    
    std::cout << "\n=== vector에 추가 ===" << std::endl;
    std::vector<MyString> vec;
    vec.push_back(MyString("벡터에"));  // 임시 객체라 이동 생성자 사용 ㅇㅇ
    vec.push_back(s2);  // 복사 생성자 사용 ㅇㅇ
    vec.push_back(std::move(s2));  // 이동 생성자 사용 (s2는 이제 비어있음) ㅇㅇ
    
    std::cout << "\n=== 프로그램 종료 ===" << std::endl;
    return 0;
}
