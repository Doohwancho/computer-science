#include <iostream>
#include <string>

int main() {
    int T, total = 0;
    std::string str;

    std::cin >> T;
    for(int i = 0; i < T; i++) {
        int arr[26] = {};
        bool isGroupedWord = true;

        for(int p = 0; p < 26; p++) {
            arr[p] = 0;
        }

        std::cin >> str;
        for(int j = 0; j < (int)str.length(); j++) {
            char cur = str[j];
            if(arr[cur-'a'] == 0) {
                arr[cur-'a']++;
            } else {
                if(j == 0 || cur != str[j-1]) {
                    isGroupedWord = false;
                    break;
                }
            }
        }
        if(isGroupedWord) {
            total++;
        }
    }
    std::cout << total << std::endl;
    
    return 0;
}