#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int T, a;
    std::cin >> T;
    std::vector<int> vec(T);

    for(int i = 0; i < T; i++) {
        std::cin >> vec[i];
    }
    std::sort(vec.begin(), vec.end());

    for(int i = 0; i < T; i++) {
        // std::cout << vec[i] << std::endl; //endl; 때문에 시간초과남
        std::cout << vec[i] << "\n";
    }

    return 0;
}