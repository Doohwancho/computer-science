#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000000

void shuffle(int *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

int main() {
    int *numbers = (int*)malloc(SIZE * sizeof(int));
    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array with numbers from 1 to SIZE-1
    for (int i = 0; i < SIZE; i++) {
        numbers[i] = i + 1;
    }

    // Seed the random number generator
    srand((unsigned)time(NULL));

    // Shuffle the array
    shuffle(numbers, SIZE);

    // Open a file for writing
    FILE *file = fopen("shuffled_numbers.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        free(numbers); // Free the allocated memory before returning
        return 1;
    }

    // Write the shuffled array to the file, each number on a new line
    for (int i = 0; i < SIZE; i++) {
        fprintf(file, "%d\n", numbers[i]);
    }

    // close file
    fclose(file);

    // Free the allocated memory
    free(numbers);

    return 0;
}

