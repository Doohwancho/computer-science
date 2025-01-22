#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>

//trial1- memory out (이 문제는 메모리가 아주 작게 설정되어 있다)
// int main() {
//     int T, a;
//     std::cin >> T;
//     std::vector<int> vec(T);
//     for(int i = 0; i < T; i++){
//         std::cin >> vec[i];
//     }
//     std::sort(vec.begin(), vec.end());

//     for(int i = 0; i < T; i++){
//         std::cout << vec[i] << "\n";
//     }
//     return 0;
// }

//solution1 - 계수정렬
//arr의 인덱스로 숫자 정렬값 담는 것 

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, a;
    cin >> N;

    int input[10001] = {0};

    for(int i = 0; i < N; i++) {
        cin >> a;
        input[a]++;
    }

    for(int i = 1; i < 10001; i++) {
        for(int j = 0; j < input[i]; j++) {
            cout << i << "\n";
        }
    }

    return 0;
}