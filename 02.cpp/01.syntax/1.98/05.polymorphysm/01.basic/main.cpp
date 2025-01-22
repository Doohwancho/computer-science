#include <iostream>

class Shape {
public:
    //Q. why const?
    //A. 이 함수는 내부 상태를 안바꾼다는 약속. 좋은 practice
    void draw() const{ 
        std::cout << "Drawing a shape" << std::endl;
    }
};

class Circle : public Shape{
public:
    void draw() const{
        std::cout << "Drawing a Circle" << std::endl;
    }
};

class Rectangle : public Shape{
public:
    void draw() const{
        std::cout << "Drawing a Rectangle" << std::endl;
    }
};

void drawShape1(const Shape* shape){ //accepts a pointer that points to a Shape
    shape->draw();
}

//function using polymorphism
//Q. is Shape& passing of pointer witha type of SHape?
//A. no, passing reference to Shape
//Q. what is reference?
//A. A reference is an alias or an alternative name for an existing object.
//   When you pass an object by reference, you are essentially passing the object itself, not a pointer to it. 
// . This allows the function to directly access and work with the original object without the need for dereferencing
void drawShape2(const Shape& shape) { //takes an object by reference
    shape.draw(); // Calls the appropriate draw function based on the actual type of the object
}


int main() {
    Circle* circle1 = new Circle();
    Shape* circle2 = new Circle();

    circle1->draw(); //drawing a circle
    circle2->draw(); //drawing a shape

    std::cout << circle1 << std::endl; //Circle Object의 주솟값을 담고있는 포인터
    // std::cout << *circle1 << std::endl;  //error!, *circle1은 Circle Object that circle1 pointer points to.

    drawShape2(*circle1); //drawing a shape, circle1객체 자체를 넘김
    // drawShape2(circle1); //error! - 객체를 넘겨야 하는데 포인터를 넘김
    drawShape1(circle1); //drawing a shape, 포인터를 받는곳에 포인터를 넘김. 

    Circle circle3;
    // Rectangle rectangle;

    // std::cout << circle3 << std::endl; //error! - 포인터가 아님. 객체이다. 객체의 주소를 가르키는 reference다.
    // drawShape1(circle3); //error! -> Q. why? A. not a pointer object.
    drawShape2(circle3); //drawing a shape <-- Q. why not draw circle? A. object slicing. 
    //Q. why not error? A. const Shape& shape <-- Shape을 가르키는 object의 주솟값인 reference만 넘겨준다.

    delete circle1;
    delete circle2;

    return 0;
}