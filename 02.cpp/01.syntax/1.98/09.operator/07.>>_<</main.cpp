#include <iostream>
#include <string>

class Student {
private:
    std::string name_;
    int id_;
    double gpa_;
    
public:
    Student() : name_(""), id_(0), gpa_(0.0) {}
    Student(const std::string& name, int id, double gpa)
        : name_(name), id_(id), gpa_(gpa) {}
    
    // << 연산자 오버로딩 (출력)
    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Name: " << student.name_ << ", ID: " << student.id_
           << ", GPA: " << student.gpa_;
        return os;
    }
    
    // >> 연산자 오버로딩 (입력)
    friend std::istream& operator>>(std::istream& is, Student& student) {
        std::cout << "Enter name: ";
        is >> student.name_;
        std::cout << "Enter ID: ";
        is >> student.id_;
        std::cout << "Enter GPA: ";
        is >> student.gpa_;
        return is;
    }
};

int main() {
    // 객체 생성
    Student s1("Kim", 12345, 3.8);
    
    // << 연산자 사용 (출력)
    std::cout << "Student Info: " << s1 << std::endl;
    
    // >> 연산자 사용 (입력)
    Student s2;
    std::cout << "\nEnter student information:\n";
    std::cin >> s2;
    
    // 입력받은 정보 출력
    std::cout << "\nYou entered:\n" << s2 << std::endl;
    
    return 0;
}
