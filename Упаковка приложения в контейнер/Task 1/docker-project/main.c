#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sum_array(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    printf("=== Docker Multi-Compiler Project ===\n");
    printf("Compiled with: ");
    
    #ifdef __clang__
        printf("CLANG\n");
    #elif __GNUC__
        printf("GCC\n");
    #else
        printf("UNKNOWN\n");
    #endif
    
    srand(time(NULL));
    int numbers[5];
    
    printf("\nGenerated numbers: ");
    for (int i = 0; i < 5; i++) {
        numbers[i] = rand() % 100;
        printf("%d ", numbers[i]);
    }
    
    int total = sum_array(numbers, 5);
    printf("\nSum of numbers: %d\n", total);
    
    printf("\nHello from Docker container!\n");
    return 0;
}