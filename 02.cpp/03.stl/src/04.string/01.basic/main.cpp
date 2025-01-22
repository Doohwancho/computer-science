#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string myString = "my String!";

    myString += " and more!";

    std::cout << myString << std::endl;
    std::cout << myString.length() << std::endl;
    std::cout << myString[0] << std::endl;

    std::string secondString = "my String! and more!";

    bool isEqualValue = myString == secondString;
    std::cout << isEqualValue << std::endl; //true 

    bool isSameAddress = &myString == &secondString;
    std::cout << isSameAddress << std::endl; //true 

    //reverse a string
    std::reverse(myString.begin(), myString.end()); 

    //string to int cast 변환
    std::string strNum = "456";
    std::stoi(strNum); //456을 int로 변환

    return 0;
}