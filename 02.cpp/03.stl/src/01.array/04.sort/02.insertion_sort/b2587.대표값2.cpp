#include <iostream>

using namespace std;

void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int arr[5];
    int total = 0;

    for(int i = 0; i < 5; i++) {
        cin >> arr[i];
    }

    insertionSort(arr, 5);

    for(int i = 0; i < 5; i++) {
        total += arr[i];
    }

    cout << total / 5 << endl;
    cout << arr[2] << endl;

    return 0;
}