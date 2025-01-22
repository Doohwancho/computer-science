#include <iostream>
#include <vector>

int main() {
    int T; 
    std::cin >> T;

    float maxNum = 0, total= 0;
    // float arr[T]; //error -> cpp은 컴파일 타임 때 배열의 사이즈를 반드시 명시하는게 standard.
    std::vector<float> arr(T); //벡터는 컴파일 타임 때 사이즈 안정해줘도 됨. 어짜피 가변길이 배열이라.

    for(int i = 0; i < T; i++) {
        std::cin >> arr[i];
        maxNum = arr[i] > maxNum ? arr[i] : maxNum;
    }
    
    for(int i = 0; i < T; i++) {
       arr[i] = arr[i] / maxNum * 100; 
       total += arr[i];
    }

    std::cout << total / T << std::endl;

    return 0;
}