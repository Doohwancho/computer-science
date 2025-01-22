#include <iostream>
#include <string>

using namespace std;

char arr[5][15]; //전역변수로 배열을 선언하면, 배열에 모든 원소들이 0으로 자동 초기화된다. cpp에서 \0은 널문자이고, 정수값으로 0. 따라서 == NULL같은 비교연산 가능.

int main() {
    // char arr[5][15]; //main 안에 array선언하면 garbage 값이 든 채로 선언됨. 이 값들은 NULL이 아님. 따라서 NULL과 비교하면 false됨.

    for(int i = 0; i < 5; i++) {
        cin >> arr[i]; //이렇게 해도 들어가나?
    }

    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 5; j++) {
            if(arr[j][i] != NULL) {
                cout << arr[j][i];
            }
        }
    }

    return 0;
}