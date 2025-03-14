#include <iostream>

class Vector2D {
private:
    double x_;
    double y_;
    
public:
    Vector2D(double x = 0, double y = 0) : x_(x), y_(y) {}
    
    // + 연산자 오버로딩
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x_ + other.x_, y_ + other.y_);
    }
    
    // - 연산자 오버로딩
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x_ - other.x_, y_ - other.y_);
    }
    
    // * 연산자 오버로딩 (스칼라 곱)
    Vector2D operator*(double scalar) const {
        return Vector2D(x_ * scalar, y_ * scalar);
    }
    
    // / 연산자 오버로딩
    Vector2D operator/(double scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return Vector2D(x_ / scalar, y_ / scalar);
    }
    
    // 출력용
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x_ << ", " << v.y_ << ")";
        return os;
    }
};

int main() {
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);
    
    // 연산자 오버로딩 사용
    Vector2D v3 = v1 + v2;  // v3는 (4, 6)
    Vector2D v4 = v1 - v2;  // v4는 (2, 2)
    Vector2D v5 = v1 * 2.0; // v5는 (6, 8)
    Vector2D v6 = v1 / 2.0; // v6는 (1.5, 2)
    
    // 출력 연산자 사용
    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;
    std::cout << "v1 + v2 = " << v3 << std::endl;
    std::cout << "v1 - v2 = " << v4 << std::endl;
    std::cout << "v1 * 2.0 = " << v5 << std::endl;
    std::cout << "v1 / 2.0 = " << v6 << std::endl;
    
    return 0;
}
