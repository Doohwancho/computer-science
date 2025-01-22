#include <iostream>

using namespace std;

void bubbleSort(int arr[], int size) {
    int temp;

    for(int i = 0; i < size - 1; i++) {
        for(int j = i+1; j < size; j++) {
            if(arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {
    int T;
    int* arr;
    std::cin >> T;
    arr = new int[T];

    for(int i = 0; i < T; i++) {
        std::cin >> arr[i];
    }

    bubbleSort(arr, T); //Q. &arr이어야 하는거 아닌가? A. ㄴㄴ arr가 포인터라 넘겨주고 *arr이나 arr[]로 접근하기 떄문에 원본수정되는게 맞다.
    
    for(int i = 0; i < T; i++) {
        std::cout << arr[i] << std::endl;
    }
    
    return 0;
}