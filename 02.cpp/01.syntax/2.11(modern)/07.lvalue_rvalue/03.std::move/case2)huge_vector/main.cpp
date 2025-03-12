#include <vector>


std::vector<int> makeHugeVector() {
    std::vector<int> huge(1000000); // 백만 개 원소 벡터
    // ... 채우기 ...
    return huge; // 이 값은 함수 종료 후 사라질 임시 객체 (rvalue)
}

// 사용 1: 복사로 받기 (성능 개구림)
void badWay() {
    std::vector<int> v1 = makeHugeVector(); // 백만 개 원소 전부 복사함 (근데 사실 최신 컴파일러는 최적화해줌)
}

// 사용 2: 이동으로 받기 (훨씬 나음)
void goodWay() {
    std::vector<int> v1 = std::move(makeHugeVector()); // 내부 포인터만 가져옴
}
