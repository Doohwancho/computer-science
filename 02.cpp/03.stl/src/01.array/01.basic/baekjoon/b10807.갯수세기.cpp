#include <iostream>

int main() {
    int arr[201] = {0};
    int T, a, b;
    std::cin >> T;
    for(int i = 0; i < T; i++) {
        std::cin >> a;
        arr[a+100]++;
    }
    std::cin >> b;
    std::cout << arr[b+100] << std::endl;
    return 0;
}