#include <iostream>
#include <string> 

int main() {
    std::string str; 
    std::cin >> str; 
    // int arr[26] = {-1}; //Q. 이렇게 하면 맨 처음 애만 -1이고 나머지는 -1로 initialize 안되나? A. 맞음. 
    int arr[26];
    for (int i = 0; i < 26; ++i) {
        arr[i] = -1;
    }

    for(int i = 0; i < (int)str.length(); i++){
        int index = str[i]-'a';
        if(arr[index] == -1) {
            arr[index] = i;
        }
    }

    for(int i = 0; i < 26; i++) {
        std::cout << arr[i] << " ";
    }
    return 0;
}