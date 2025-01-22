#include <stdio.h>
#include <stdlib.h>

// Function to allocate memory for a matrix of size rows x cols
int** createMatrix(int rows, int cols) {//size of pointer is 8byte in c. 
    int** head = (int**)malloc(rows * sizeof(int*));
    if(head == NULL) {
        printf("Memory Allocation Failed.\n");
        exit(1);
    }
    for(int i = 0; i < rows; i++) {
        int* row = (int*)malloc(cols * sizeof(int));
        if(row == NULL) {
            printf("Memory Allocation Failed.\n");
            exit(1);
        }
        head[i] = row;
    }
    return head;
}

// Function to initialize a matrix with random values
void initializeMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Generate random numbers between 0 and 9
        }
    }
}

// Function to print a matrix
void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to perform matrix multiplication
int** multiplyMatrices(int** matrix1, int rows1, int cols1, int** matrix2, int rows2, int cols2) {
    if (cols1 != rows2) {
        printf("Matrix multiplication is not possible.\n");
        exit(1);
    }
    int** result = createMatrix(rows1, cols2);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

// Function to free memory allocated for a matrix
void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]); //array도 최앞단 head를 free()하듯, 2차원 배열의 각 row의 head만 free()함.
    }
    free(matrix);
}

int main() {
    int rows1 = 2;
    int cols1 = 2;
    int rows2 = 2;
    int cols2 = 2;

    // Check if matrix multiplication is possible
    if (cols1 != rows2) {
        printf("Matrix multiplication is not possible.\n");
        return 0;
    }
    
    // Create and initialize matrices with random values
    int** matrix1 = createMatrix(rows1, cols1);
    initializeMatrix(matrix1, rows1, cols1);
    printf("Matrix 1:\n");
    printMatrix(matrix1, rows1, cols1);
    
    int** matrix2 = createMatrix(rows2, cols2);
    initializeMatrix(matrix2, rows2, cols2);
    printf("Matrix 2:\n");
    printMatrix(matrix2, rows2, cols2);
    
    // Multiply matrices and print the result
    int** result = multiplyMatrices(matrix1, rows1, cols1, matrix2, rows2, cols2);
    printf("Resultant Matrix:\n");
    printMatrix(result, rows1, cols2);
    
    // Free dynamically allocated memory
    freeMatrix(matrix1, rows1);
    freeMatrix(matrix2, rows2);
    freeMatrix(result, rows1);
    
    return 0;
}
