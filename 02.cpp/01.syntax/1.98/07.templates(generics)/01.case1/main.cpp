#include <iostream>
#include "my_template.h"

//java의 generic인 듯 
int main() {
    MyTemplate<int> intTemplate(42);
    std::cout << intTemplate.getData() << std::endl;    

    MyTemplate<double> doubleTemplate(42.42);
    std::cout << doubleTemplate.getData() << std::endl;
}
