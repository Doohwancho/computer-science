#include "vector.hpp"
#include <iostream>

int main() {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    for (std::size_t i = 0; i < vec.get_size(); ++i) {
        std::cout << vec[i] << std::endl;
    }

    return 0;
}
