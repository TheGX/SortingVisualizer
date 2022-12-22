#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "coroutine.h"

#define _countOf(_Array) (sizeof(_Array) / sizeof(_Array[0]))

//TODO: Change this to be a dynamic mem alocation
int randomInts[50];

inline static
void copyArryInt(int* src, int n, int* dest) {
    for (int i=0 ; i < n; i++) {
        dest[i] = src[i];
    }
}

inline static
void copyArryIntToFloat(int* src, int n, float* dest) {
        
    for (int i=0 ; i < n; i++) {
        dest[i] = src[i];
    }
}

inline static
void getNRandomInts(int n){
        
    srand(time(NULL));
    for (int i=0; i < n; i++) {
        randomInts[i] = rand()%1000;
    }
}

inline static
void printArray(int* array, int count) {
    for (int i=0; i < count; i++) {
        printf("array[%d]: %d\n", i, array[i]);
    }
}

inline static
void swap(int* a, int*b) {

    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Selection Sort: O(n^2):
    // Run across array, finding min, swap min with the 1st value
    // Run again, swap with 2nd, etc....
    // [12, 6, 3, 7]
    // [6, 12, 3, 7]
    // [3, 12, 6, 7]
inline static
bool selectionSort(int* arrayToSort, int count, coroutine_t* co){
    
    bool sorted = false;
    static int i=0, j=0;
    COROUTINE_START(co);
    for (i = 0; i < count; i++) {

        for (j=i+1; j < count; j++) {

            if ( arrayToSort[j] < arrayToSort[i] ) {
                swap(&arrayToSort[j], &arrayToSort[i]);
            }
            //PUT cursor here
            co->cursorPosition = j;
            COROUTINE_WAIT(co, 2, 1.0f);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}

// Buble Sort: O(n^2):
    // Compare element 2 by 2;
    // Swap if need;
    // Best case is O(n) -> array already sorted
    // [12, 6, 3, 7]
    // [6, 12, 3, 7]
    // [6, 3, 12, 7]

inline static
bool bubleSort(int* arrayToSort, int count, coroutine_t* co){
        
    bool sorted = false;
    static int i=0, j=0;
    COROUTINE_START(co);
    for (i=0; i < count; i++) {

        // The absolute max will always go to the end at the end of each 
        // outer iteration. So the last i values will be sorted
        // at the end of the cycle
        for (j=0; j < count-1 - i  ; j++) {
            if ( arrayToSort[j] > arrayToSort[j+1] ) {
                swap(&arrayToSort[j], &arrayToSort[j+1]);
            }
            //PUT cursor here
            co->cursorPosition = j;
            //COROUTINE_YIELD(co);
            COROUTINE_WAIT(co, 10, 5.0f);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
