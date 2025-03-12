#include <iostream>
#include <stdexcept>
#include <string>

// 간단한 문자열 배열 래퍼 클래스
class StringArray {
private:
    std::string* data_;
    size_t size_;
    
public:
    StringArray(size_t size) : size_(size) {
        data_ = new std::string[size_];
    }
    
    ~StringArray() {
        delete[] data_;
    }
    
    // 복사 생성자
    StringArray(const StringArray& other) : size_(other.size_) {
        data_ = new std::string[size_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    // 복사 대입 연산자
    StringArray& operator=(const StringArray& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new std::string[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    
    // [] 연산자 오버로딩 (읽기/쓰기 가능 버전)
    std::string& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }
    
    // [] 연산자 오버로딩 (const 버전, 읽기 전용)
    const std::string& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }
    
    // 배열 크기 반환
    size_t size() const {
        return size_;
    }
};

int main() {
    StringArray names(3);
    
    // [] 연산자 사용해서 값 설정
    names[0] = "Kim";
    names[1] = "Lee";
    names[2] = "Park";
    
    // [] 연산자 사용해서 값 읽기
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << "names[" << i << "] = " << names[i] << std::endl;
    }
    
    // const 객체에서 [] 연산자 사용
    const StringArray constNames = names;
    // constNames[0] = "Choi"; // 에러: const 객체는 수정 불가
    std::cout << "constNames[0] = " << constNames[0] << std::endl;
    
    // 예외 처리
    try {
        std::cout << names[5] << std::endl; // 범위 초과
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
