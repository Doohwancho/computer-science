#include <string>

// 이 함수는 절대 실패하지 않음을 보장
bool is_valid(const std::string& input) noexcept;

// 이 함수는 예외를 던질 수 있음 (명시적으로 지정하지 않으면 기본값)
void process_data(const std::string& data);
