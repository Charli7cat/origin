#include <stdio.h>
#include <stdlib.h>

void count_sort(int* arr, int size) {
    const int MIN_VALUE = 10;
    const int MAX_VALUE = 24;
    const int RANGE = MAX_VALUE - MIN_VALUE + 1;

    int* count = (int*)calloc(RANGE, sizeof(int));

    for (int i = 0; i < size; i++) {
        count[arr[i] - MIN_VALUE]++;
    }

    int index = 0;
    for (int i = 0; i < RANGE; i++) {
        while (count[i] > 0) {
            arr[index++] = i + MIN_VALUE;
            count[i]--;
        }
    }

    free(count);
}

void print_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

void test_sort(int* arr, int size, const char* test_name) {
    printf("The original array %s: ", test_name);
    print_array(arr, size);

    count_sort(arr, size);

    printf("Sorted array %s: ", test_name);
    print_array(arr, size);
    printf("\n");
}

int main() {
    int arr1[] = { 19, 14, 22, 22, 17, 22, 13, 21, 20, 24,
                  18, 10, 17, 16, 17, 20, 22, 11, 20, 16,
                  14, 13, 10, 22, 18, 14, 16, 24, 19, 17 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    int arr2[] = { 16, 17, 14, 20, 22, 20, 17, 22, 16, 19,
                  23, 24, 20, 22, 21, 18, 14, 16, 17, 21,
                  10, 11, 19, 23, 11, 11, 17, 17, 11, 21,
                  17, 11, 17, 16, 12, 11, 16, 22, 23, 16 };
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    int arr3[] = { 21, 15, 19, 18, 23, 12, 18, 18, 19, 23,
                  12, 20, 15, 22, 21, 18, 19, 20, 12, 16,
                  20, 14, 17, 13, 10, 23, 19, 14, 10, 22,
                  19, 12, 24, 23, 22, 15, 13, 22, 18, 18,
                  11, 23, 24, 17, 10 };
    int size3 = sizeof(arr3) / sizeof(arr3[0]);

    test_sort(arr1, size1, "");
    test_sort(arr2, size2, "");
    test_sort(arr3, size3, "");

    return 0;
}