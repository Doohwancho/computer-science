#include <vector>
#include <string>

int main() {
    std::vector<std::string> names;

    std::string name = "김철수";
    // 이 이름 다른 곳에선 안 쓸거면
    names.push_back(std::move(name)); // 복사 말고 이동으로 넣음

    // 이 시점에서 name은 비었거나 정의되지 않은 상태
    return 0;
}
