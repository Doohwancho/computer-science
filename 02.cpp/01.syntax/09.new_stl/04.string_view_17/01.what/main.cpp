#include <string>
#include <string_view>
#include <iostream>

// Q. what is string_view?
// A.  문자열 데이터를 복사하지 않고 읽기 전용으로 접근할 수 있게 해주는 경량화된 객체
//     문자열의 소유권 없이 문자 시퀀스에 대한 참조를 제공함 

// Q. features?
// 1. 비소유 참조: 문자열 데이터를 직접 소유하지 않고 참조만 함
// 2. 읽기 전용: 참조하는 문자열 데이터를 수정할 수 없음
// 3. 가벼운 복사: 복사 시 실제 문자열 데이터가 아닌 포인터와 길이만 복사됨

int main() {
    // 다양한 초기화 방법
    std::string_view sv1 = "Hello, World!";  // C 스타일 문자열에서 생성
    
    std::string str = "Hello, C++17!";
    std::string_view sv2 = str;  // std::string에서 생성
    
    char array[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    std::string_view sv3(array, 5);  // 문자 배열에서 생성 (길이 지정)
    
    // 기본 연산
    std::cout << "Length: " << sv1.length() << std::endl;
    std::cout << "First char: " << sv1[0] << std::endl;
    std::cout << "Substring: " << sv1.substr(0, 5) << std::endl;
    
    // 문자열 검색
    std::cout << "Found at: " << sv1.find("World") << std::endl;
    
    return 0;
}
