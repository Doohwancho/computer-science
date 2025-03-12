#include <iostream>

class String {
public:
    String(int size) {  // 크기로 문자열 버퍼 생성
        std::cout << size << " 크기의 문자열 생성" << std::endl;
    }
    
    String(const char* str) {  // 문자열로 초기화
        std::cout << "\"" << str << "\" 문자열로 초기화" << std::endl;
    }
};

class BetterString {
public:
    // 이건 크기로 생성하는건데, 명시적으로만 허용
    explicit BetterString(int size) {
        std::cout << size << " 크기의 문자열 생성" << std::endl;
    }
    
    // 이건 진짜 문자열로 초기화하는거라 암시적 변환 허용해도 됨
    BetterString(const char* str) {
        std::cout << "\"" << str << "\" 문자열로 초기화" << std::endl;
    }
};

void processString(String s) {
    // 문자열 처리
}

void processBetterString(BetterString s) {
    // 문자열 처리
}

int main() {
    //case1) string 
    processString("Hello");  // OK: "Hello" 문자열로 초기화
    processString(10);       // 음?? 10 크기의 빈 문자열 생성됨 (의도한건가?)


    //case2) better string
    processBetterString("Hello");  // OK: 문자열 생성자는 암시적 변환 허용
    // processBetterString(10);    // 컴파일 에러! int->BetterString 암시적 변환 안됨

    // 꼭 필요하면 명시적으로 해야함
    processBetterString(BetterString(10));  // OK: 명시적으로 변환
}

