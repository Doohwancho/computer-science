#include <stdio.h>
#include <stdlib.h>


/*********************************************************************/
// Q1. what is double pointer?

//Double pointers, also known as multiple pointers, are pointers that point to another pointer.
//This allows them to store the address of another pointer, which can be used to indirectly access the data that the second pointer is pointing to.


int main()
{
  int x = 10;
  int* ptr1;
  int** ptr2;

  // ptr1 stores the address of x
  ptr1 = &x;

  // ptr2 stores the address of ptr1
  ptr2 = &ptr1;

  printf("Value of x is: %d\n", x);
  printf("Value of x using ptr1: %d\n", *ptr1);
  printf("Value of x using ptr2: %d\n", **ptr2);

  return 0;
}

/* Output
Value of x is: 10
Value of x using ptr1: 10
Value of x using ptr2: 10
*/




/*********************************************************************/
// Q2. does double pointer have relationship with virtual memory?

// A. Yes, double pointers can be used to access data in virtual memory.
//    Double pointers are often used to access data stored in virtual memory and can even be used to create dynamic memory allocation in C,
//    which is useful for tasks such as creating linked lists and arrays.


// Q. how to use double pointer to access data in virtual memory?

// A. double pointers are used to access data stored in virtual memory. This is done by first allocating a certain amount of memory to a pointer using the malloc() function,
//    then using a double pointer to reference the allocated space.
//    This allocates a certain amount of memory to the pointer, and the double pointer can then reference the allocated space.
//    The data in the virtual memory can then be accessed through the double pointer.


int main2()
{
  int n;
  printf("Enter the size of array: ");
  scanf("%d", &n);

  // Allocate memory for the array
  int* ptr1 = (int*)malloc(n * sizeof(int));

  // Use a double pointer to reference the allocated space
  int** ptr2 = &ptr1;

  int i;
  for (i = 0; i < n; i++)
  {
    printf("Enter element %d: ", i);
    scanf("%d", &(*ptr2)[i]);
        //(*ptr2) == ptr1
        //(*ptr2)[i] == ptr1[i]
        //&(*ptr2)[i] == &ptr1[i]
  }

  printf("The elements of the array are:\n");
  for (i = 0; i < n; i++)
  {
    printf("%d\t", (*ptr2)[i]);
  }

  // Free the allocated memory
  free(ptr1);

  return 0;
}

/* Output
Enter the size of array: 5
Enter element 0: 10
Enter element 1: 20
Enter element 2: 30
Enter element 3: 40
Enter element 4: 50
The elements of the array are:
10      20      30      40      50




/*********************************************************************/
// Q3. how to use double pointer to create dynamic memory allocation in C?


//A. Double pointers can be used to create dynamic memory allocation in C by using the malloc() and free() functions.
//   The malloc() function can be used to allocate a block of memory of a specified size
//   and the free() function can be used to deallocate a block of memory that is no longer needed.

//   To use double pointers for dynamic memory allocation, the pointer is first set to NULL to indicate that it does not point to any location in memory.
//   Then the malloc() function is called with the desired size as an argument and the address of the pointer is passed as a parameter.
//   This will cause the pointer to point to a block of memory of the specified size allocated from the heap.
//   The double pointer can then be used to access and modify the allocated memory.
//   When the memory is no longer needed, the free() function should be called with the pointer as an argument to deallocate the memory.


// example - dynamic allocation of memory using double pointers

// Declare a double pointer
int **ptr = NULL;

// Allocate a block of memory
ptr = (int **)malloc(sizeof(int*));

// Store a value in the allocated memory
**ptr = 5;

// Print the stored value
printf("Value stored in ptr: %d\n", **ptr); // 5

// Deallocate the memory
free(ptr);





/*********************************************************************/
// Q4. explain double pointer in assembly code

//A. In assembly code, a double pointer is a type of pointer that stores the address of another pointer.
//   It is used to refer to a location in memory that holds the address of a variable or an array.
//   A double pointer is often used when passing pointers as parameters to functions.
//   This allows a function to modify the pointer passed in, which can be used to modify variables or arrays that are outside the scope of the function.

/*
```assembly

; Declare a variable
var: db 0 ; Declare a pointer to the variable
ptr: dd var ; Declare a double pointer
dptr: dd ptr ; Call a function that modifies the double pointer
call modify_dptr, dptr ; Update the value of the variable using the double pointer
mov [dptr], 0Ah ;

modify_dptr: ; Parameter: double pointer
push ebp ; Save the base pointer
mov ebp, esp ; Set the base pointer
mov eax, [ebp + 8] ; Get the double pointer
mov [eax], 0Fh ; Update the double pointer
pop ebp ; Restore the base pointer
ret ; Return
```
*/
