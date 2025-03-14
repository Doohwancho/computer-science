// 함수가 절대 예외를 던지지 않음을 선언
void func1() noexcept;

// 조건부 noexcept (표현식이 true면 예외를 던지지 않음)
template <class T>
void func2(T param) noexcept(noexcept(T())); 

// 함수 선언과 정의 모두에 noexcept를 붙일 수 있음
void func3() noexcept {
    // 구현
}
