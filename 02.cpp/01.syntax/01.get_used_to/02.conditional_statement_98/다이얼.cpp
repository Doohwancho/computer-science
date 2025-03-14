#include <iostream>
#include <string>

int main() {
    int total = 0;
    std::string str;
    std::cin >> str;
    
    //ABC = 3
    //DEF = 4
    //GHI = 5
    //JKL = 6
    //MNO = 7
    //PQRS = 8
    //TUV = 9
    //WXYZ = 10

    //loop str
    for(int i = 0; i < str.size(); i++) {
        char ch = str[i];
        if (ch >= 'A' && ch <= 'C') {
            total += 3;
        } else if (ch >= 'D' && ch <= 'F') {
            total += 4;
        } else if (ch >= 'G' && ch <= 'I') {
            total += 5;
        } else if (ch >= 'J' && ch <= 'L') {
            total += 6;
        } else if (ch >= 'M' && ch <= 'O') {
            total += 7;
        } else if (ch >= 'P' && ch <= 'S') {
            total += 8;
        } else if (ch >= 'T' && ch <= 'V') {
            total += 9;
        } else if (ch >= 'W' && ch <= 'Z') {
            total += 10;
        }
    }

    std::cout << total;

    return 0;
}