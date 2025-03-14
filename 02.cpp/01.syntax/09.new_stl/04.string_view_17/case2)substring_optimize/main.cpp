#include <string>
#include <string_view>

int main() {
    std::string large_string = "This is a very large string with a lot of characters";

    // 이전 방식 - 메모리 할당 및 복사 발생
    std::string sub1 = large_string.substr(10, 10);

    // string_view 사용 - 메모리 할당 없이 참조만 생성
    std::string_view sv = large_string;
    std::string_view sub2 = sv.substr(10, 10);  // 복사 없음, 매우 빠름
    return 0;
}
