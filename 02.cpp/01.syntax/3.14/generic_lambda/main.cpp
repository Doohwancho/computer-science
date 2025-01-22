#include <iostream>

int main() {
    auto compare = [](const auto& a, const auto& b) { return a < b; };
    int result1 = compare(3, 5);     // result1 = true
    bool result2 = compare(3.14, 2.71);  // result2 = false

    return 0;
}