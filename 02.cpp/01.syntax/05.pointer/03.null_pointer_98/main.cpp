#include <iostream>

int main(){
    /**
     * case1) initialize null pointer
     */
    int* ptr1 = NULL;

    if(ptr1 == NULL) {
        std::cout << "ptr1 is NULL" << std::endl;
    } else {
        std::cout << "ptr1 is not NULL" << std::endl;
    }
    
    /**
     * case2) initialize pointer as empty or invalid
     */
    int* ptr2 = NULL;

    /**
     * case3) checking existing pointer is valid;
     */

     int ptr3[] = {10, 20, 30};
     if(ptr3 == ptr2) {
        std::cout << "ptr3 is NULL" << std::endl;
     } else {
         std::cout << "ptr3 is not NULL" << std::endl;
     }

    //  if(ptr3 != NULL) { //warning: comparison of array 'ptr3' not equal to a null pointer is always tru
    //     std::cout << ptr3[0] << std::endl; //10
    //  }

    /**
     * case4) representing start or end of data structure
     */
     struct Node {
        int value;
        Node* next;
     };

    //  Node* head = NULL;
    return 0;
}