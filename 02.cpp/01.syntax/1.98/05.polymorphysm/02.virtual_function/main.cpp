#include <iostream>

// Base class Shape
class Shape {
public:
    //case1) Virtual function draw
    //이렇게 하면, 모든 파생클래스에서 draw를 오버라이딩 할 필요가 없다.
    virtual void draw() const {
        std::cout << "Drawing a shape" << std::endl;
    }
    //case2) virtual function을 안쓰는 경우
    //이렇게 하면, 모든 파생클래스에서 draw를 오버라이딩 해줘야 한다.
    // void draw() const {
    //     std::cout << "Drawing a shape" << std::endl;
    // }
};

// Derived class Circle, inherits from Shape
class Circle : public Shape {
public:
    // Override the draw function for Circle
    void draw() const {
        std::cout << "Drawing a Circle" << std::endl;
    }
};

// Derived class Rectangle, inherits from Shape
class Rectangle : public Shape {
public:
    // Override the draw function for Rectangle
    void draw() const {
        std::cout << "Drawing a Rectangle" << std::endl;
    }
};

// Function that accepts a Shape reference and calls its draw function
void drawShape(const Shape& shape) {
    shape.draw(); // Calls the appropriate draw function based on the actual type of the object
}

int main() {
    Circle circle;
    Rectangle rectangle;

    // Call the drawShape function with objects of different types
    drawShape(circle);    // drawing a circle with virtual function. if without, drawing a shape
    drawShape(rectangle); // drawing a rectangle with virtual function. if without, drawing a shape

    return 0;
}
