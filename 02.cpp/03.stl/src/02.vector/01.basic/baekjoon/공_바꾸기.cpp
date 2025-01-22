#include <iostream>
#include <vector>

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
        int tmp = arr[a-1];
        arr[a-1] = arr[b-1];
        arr[b-1] = tmp;
    }

    for(int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
    return 0;
}