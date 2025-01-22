#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    int T, a, b;
    std::cin >> T >> a;

    std::vector<int> vec;
    for(int i = 0; i < T; i++) {
        std::cin >> b;
        vec.push_back(b);
    }
    //sort vector
    std::sort(vec.begin(), vec.end());

    //pick a-th element from the last
    std::cout << vec.at(T-a) << std::endl;

    return 0;
}