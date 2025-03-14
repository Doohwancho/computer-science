#include <iostream>
#include <unordered_map>
#include <tuple>

int main() {
    // Tuple
    std::tuple<std::string, int, double> person("John", 30, 5.8);
    std::string name = std::get<0>(person);
    int age = std::get<1>(person);
    double height = std::get<2>(person);

    std::cout << name << " is " << age << " years old and " << height << " feet tall." << std::endl;

    return 0;
}
