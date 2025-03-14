#include <iostream>
#include <string>
#include <string_view>

// 이전 방식 - 여러 오버로드 필요
void process(const char* str);
void process(const std::string& str);

// string_view 사용 - 하나의 함수로 통합
void process(std::string_view str) {
    // 문자열 처리 (복사 없음)
    std::cout << "Processing: " << str << std::endl;
}

int main() {
    process("C-style string");  // C 스타일 문자열
    
    std::string cpp_str = "C++ string";
    process(cpp_str);  // std::string
    
    char buffer[10] = {'B', 'u', 'f', 'f', 'e', 'r'};
    process(std::string_view(buffer, 6));  // 문자 배열
    
    return 0;
}
