#include <iostream>

void insertSort(std::string& str) {
    for(int i = 1; i < (int)str.size(); i++) {
        int key = str[i];
        int j = i - 1;
        while(j >= 0 && str[j] < key) {
            str[j + 1] = str[j];
            j--;
        }
        str[j + 1] = key;
    }
}

int main(){
    std::string str;
    long a;
    std::cin >> a;

    //int -> string 
    str = std::to_string(a);

    //insert sort
    insertSort(str);

    //str to int
    a = std::stoi(str);

    std::cout << a << std::endl;
    
    return 0;
}