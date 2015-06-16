#include <iostream>

#include "CustomSort.h"
 

void sort(int* array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 1; j < size; j++) {
            if (array[j - 1] > array[j]) {
                int temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
            }
        }
    } 
}

bool isSorted(int* array, int size) {
    for (int i = 1; i < size; i++) {
        if (array[i - 1] > array[i]) {
            return false;
        }
    }
    return true;
}

void print(int* array, int size) {
    for (int i = 0; i < size; i++) {
        std :: cout << "a[" << i << "] = " << array[i] << std :: endl;
    }
}


int main() {
    int array[] = { 1, 2, -1 };
    int size = sizeof(array) / sizeof(int);
    
    std :: cout << isSorted(array, size) << std :: endl;
    sort(array, size);
    std :: cout << isSorted(array, size) << std :: endl;
    print(array, size);
}
