#include <iostream>
#include <list>

int main() {
    std::list<int> my_list;

    my_list.push_back(0);
    my_list.push_back(1);
    my_list.push_back(2);

    std::cout << "my_list elements: " << std::endl;
    for(std::list<int>::iterator it = my_list.begin(); it != my_list.end(); ++it) {
        std::cout << "elements: " << *it << std::endl;
    }
}