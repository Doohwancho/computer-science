#include <iostream>
#include <vector>

int main(){
    int N, M;
    int i,j,k;
    std::cin >> N >> M;
    std::vector<int> arr(N);

    for(int a = 0; a < M; a++) {
        std::cin >> i >> j >> k;
        for(int b = i-1; b <= j-1; b++) {
            arr[b] = k;
        }
    }

    for(int c = 0; c < N; c++) {
        std::cout << arr[c] << " ";
    }
    return 0;
}