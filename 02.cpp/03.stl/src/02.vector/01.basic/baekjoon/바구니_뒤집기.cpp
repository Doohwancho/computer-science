#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    int N, M;
    int a, b;

    std::cin >> N >> M;
    std::vector<int> arr(N);

    for(int i = 0; i < N; i++) {
        arr[i] = i+1;
    }

    for(int i = 0; i < M; i++) {
        std::cin >> a >> b;
        //how to reverse vector 'arr' from index a to b?
        std::reverse(arr.begin() + a-1, arr.begin() + b);
    }

    for(int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
    return 0;
}