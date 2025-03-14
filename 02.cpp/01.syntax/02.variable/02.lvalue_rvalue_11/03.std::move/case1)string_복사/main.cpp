#include <iostream>

//Q. what is std::move() do?
//A. 기존에 s1_ptr*->data 였다면, free(s1_ptr*) 해주고 데이터만 반환해줌. 
//A. std::move()는 lvalue를 rvalue로 변환해줌 == 데이터 가르키는 포인터 말고 데이터를 반환해줌 

int main() {
    std::string s1 = "long string with lots of characters"; //s1은 lvalue, "long string with lots of characters"는 rvalue임: 임시값이고 문자열 리터럴임
    std::string s2;                                         //s2도 lvalue 

    // 방법 1: 복사 - 모든 문자 하나하나 복사함 (느림)
    s2 = s1;  
                //내부적으로는 이런 일이 벌어짐 (복사 대입 연산자 호출)
                // s2.operator=(s1);  // 전체 내용 복사

    // 방법 2: 이동 - 그냥 내부 포인터만 바꿈 (빠름)
    s2 = std::move(s1);  // s1은 이제 비어있음 (moved-from 상태)
                        // 내부적으로 이런 일이 벌어짐 (이동 대입 연산자 호출)
                        // s2.operator=(static_cast<std::string&&>(s1));  // 포인터만 옮기고 실제 데이터는 복사 안 함

    // s1 출력하면 비어있거나 정의되지 않은 값일 수 있음 (쓰레기값)
    std::cout << "s1 after move: " << s1 << std::endl;  // 빈 문자열이거나 쓰레기값

    // s2는 원래 s1의 내용 가지고 있음
    std::cout << "s2 after move: " << s2 << std::endl;  // "long string with lots of characters"

    return 0;
}

/*
string이 어케 생김?

class string {
private:
    char* data_;  // 실제 문자열 데이터 가리키는 포인터
    size_t size_;
    // ... 기타 멤버 ...

public:
    // 복사 대입 연산자
    string& operator=(const string& other) {
        if (this != &other) {
            delete[] data_;  // 기존 데이터 삭제
            size_ = other.size_;
            data_ = new char[size_ + 1];  // 새 메모리 할당
            std::copy(other.data_, other.data_ + size_ + 1, data_);  // 복사 (비싼 연산)
        }
        return *this;
    }

    // 이동 대입 연산자 (rvalue 참조를 받음)
    string& operator=(string&& other) noexcept {
        if (this != &other) {
            delete[] data_;  // 기존 데이터 삭제
            
            // 아래가 핵심임: 그냥 포인터만 가져오고 복사 안 함 (저렴한 연산)
            data_ = other.data_;  // 포인터만 가져옴 (훔침)
            size_ = other.size_;
            
            // other는 이제 비어있게 만듦 (안 그러면 두 객체가 같은 메모리 가리키고 있음)
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
};

*/
