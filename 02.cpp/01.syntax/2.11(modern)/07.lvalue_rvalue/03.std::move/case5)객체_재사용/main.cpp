#include <vector>

int main(){
    std::vector<int> v1(1000000); // 큰 벡터
    // v1 채우기...

    // v1 내용을 v2로 이동하고, v1은 다른 용도로 재사용
    std::vector<int> v2 = std::move(v1);
    // 이 시점에서 v1은 비어있음

    // v1 다시 다른 데이터로 채우기
    v1.resize(500);
    // ... 새 데이터 ...
    return 0;
}
