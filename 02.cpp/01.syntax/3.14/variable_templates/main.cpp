#include <iostream>

template <typename T>
constexpr T pi = T(3.14159265358979323846);

int main() {
    double area = 2 * pi<double> * 5;  // area = 2 * 3.14159... * 5

    return 0;
}