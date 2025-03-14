#ifndef MY_TEMPLATE_H
#define MY_TEMPLATE_H

// 함수 템플릿 
template<typename T>
class MyTemplate {
private:
    T data;

public:
    MyTemplate(const T&data): data(data) {}
    T getData() const { return data; }
};

#endif
