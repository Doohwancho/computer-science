#include <iostream>
#include <unordered_map>
#include <tuple>

int main() {
    // Unordered map
    std::unordered_map<std::string, int> ages;
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages["Charlie"] = 35;

    for (const auto& pair : ages) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Tuple
    std::tuple<std::string, int, double> person("John", 30, 5.8);
    std::string name = std::get<0>(person);
    int age = std::get<1>(person);
    double height = std::get<2>(person);

    std::cout << name << " is " << age << " years old and " << height << " feet tall." << std::endl;

    return 0;
}