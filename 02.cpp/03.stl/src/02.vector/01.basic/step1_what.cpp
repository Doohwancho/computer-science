#include <iostream>
#include <vector>

//Q. what is vector? how is it different from list?
// vector: java의 arraylist
// list: java의 linkedlist
// vector는 myVector[1]; 같이 random access 지원, 
// list는 myList.get(1); 같이 random access 지원하지 않음.
int main() {
    std::vector<int> myVector;

    myVector.push_back(0);
    myVector.push_back(1);
    myVector.push_back(2);

    for(int i = 0; i < myVector.size(); i++) {
        std::cout << "myVector elements: " << myVector[i] << std::endl;
    }

    for(std::vector<int>::iterator it = myVector.begin(); it!= myVector.end(); ++it) {
        std::cout << "myVector elements: " << *it << std::endl;
    }
    myVector.clear();
}