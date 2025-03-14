#include <iostream>
#include <cstring>

class ShallowString {
public:
    ShallowString(const char* str) {
        length_ = strlen(str);
        data_ = new char[length_ + 1];
        strcpy(data_, str);
    }
    
    // 얕은 복사 (그냥 포인터만 복사) - 이러면 개망함
    ShallowString(const ShallowString& other) {
        length_ = other.length_;
        data_ = other.data_;  // 그냥 포인터 주소만 복사 ㅋㅋ (두 객체가 같은 메모리 가리킴)
    }
    
    ~ShallowString() {
        delete[] data_;  // 두 객체가 같은 메모리 해제하면? 더블 프리 버그 ㅅㅌㅊ
    }
    
    void print() const {
        std::cout << data_ << std::endl;
    }
    
private:
    char* data_;
    size_t length_;
};

class DeepString {
public:
    DeepString(const char* str) {
        length_ = strlen(str);
        data_ = new char[length_ + 1];
        strcpy(data_, str);
    }
    
    // 깊은 복사 (진짜 데이터까지 복사) - 안전하지만 비용 ㅈㄴ 큼
    DeepString(const DeepString& other) {
        length_ = other.length_;
        data_ = new char[length_ + 1];  // 새 메모리 할당
        strcpy(data_, other.data_);     // 실제 내용 복사
    }
    
    ~DeepString() {
        delete[] data_;  // 각자 자기 메모리 해제하니까 안전
    }
    
    void print() const {
        std::cout << data_ << std::endl;
    }
    
private:
    char* data_;
    size_t length_;
};
