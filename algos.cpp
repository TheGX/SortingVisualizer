#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "coroutine.h"

#define _countOf(_Array) (sizeof(_Array) / sizeof(_Array[0]))

//TODO: Change this to be a dynamic mem alocation
int randomInts[100];

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
            COROUTINE_WAIT(co, co->time, co->dt);
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
            COROUTINE_WAIT(co, co->time, co->dt);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}

//  Quick Sort: O(nlog(n)) - worst is O(n^2)
    // Recursive;
    // Pick a pivot;
    // Swap itemLeft and itemRight:
    // itemLeft: starting left, item smaller than pivot;
    // itemRight: starting Right, item larger than pivot;
    // Swap itemLeft with pivot if itemLeft_index < itemRight_index;

// The iterative version had to be used to be compitable with this Coroutines implementation
inline static
bool quickSort(int arrayToSort[], int count, coroutine_t* co) {

    static int lower, upper;
    static bool sorted = false;
    static std::vector<int> stack(count, 0);

    static int top, i, j, pivot_index;
    static unsigned pivot;

    COROUTINE_START(co);
    sorted = false;
    lower = 0;
    upper = count-1;

    top = -1;
    stack[++top] = lower;
    stack[++top] = upper;

    while (top >= 0) {
        upper = stack[top--];
        lower = stack[top--];

        pivot_index = lower + (upper - lower) / 2;
        pivot = arrayToSort[upper];
        i = (lower - 1);

        for (j = lower; j <= upper - 1; j++) {
            if (arrayToSort[j] <= pivot) {
                i++;
                swap(&arrayToSort[i], &arrayToSort[j]);
            }

            // Put cursor
            co->cursorPosition = j;
            COROUTINE_WAIT(co, co->time, co->dt);
        }
        // Place the pivot in it's final and correct position
        pivot_index = i + 1;
        std::swap(arrayToSort[pivot_index], arrayToSort[upper]);

        // Add new sub arrays to stack
        if (pivot_index - 1 > lower) {
            stack[++top] = lower;
            stack[++top] = pivot_index - 1;
        }

        if (pivot_index + 1 < upper) {
            stack[++top] = pivot_index + 1;
            stack[++top] = upper;
        }
    }
    
    sorted = true;
    COROUTINE_END(co);
    return sorted;
}   

// Heap Sort: O(nlog(n))
    // Place original array into heap;
    // Remove heap root (max element), resulting in sorted array;
    // Update heap structure along the way;
    // Best case is O(n) -> array already sorted;
inline static
bool heapSort(int arrayToSort[], int count, coroutine_t* co) {

    static int parent, i, j, parentJ, index;
    static bool sorted;
    
    sorted = false;
    COROUTINE_START(co);
    //Make MaxHeap (all parent nodes > than child nodes)
    for( i = 1; i < count ; i++)
    {
        parent = (i-1) / 2;

        // Put cursor
        co->cursorPosition = i;
        COROUTINE_WAIT(co, co->time, co->dt);

        if ( arrayToSort[i] > arrayToSort[parent] )
        {
            j = i;
            parentJ = (j-1) / 2;
            
            //swap child with parent until child smaller than parent
            while ( arrayToSort[j] > arrayToSort[(j-1) / 2] )
            {
                parentJ = (j-1) / 2;
                swap(&arrayToSort[j], &arrayToSort[parentJ]);
                j = parentJ;
            }
        }

    }
    
    //Swap root node to the end, update the heap structure along the way 
    for( i = count - 1; i > 0; i--)
    {
        // Put cursor
        co->cursorPosition = i;
        COROUTINE_WAIT(co, co->time, co->dt);
        
        swap(&arrayToSort[0], &arrayToSort[i]);

        index = 1;
        while ( index < i) 
        {
            // Put cursor
            co->cursorPosition = index;
            COROUTINE_WAIT(co, co->time, co->dt);
            
            parent = (index-1) / 2;

            // If left child < right child, jump to right child
            if ( arrayToSort[index] < arrayToSort[index + 1]  && (index + 1) < i )
            {
                index++;
            }
            
            if ( arrayToSort[index] > arrayToSort[parent] )
            {
                swap(&arrayToSort[index], &arrayToSort[parent]);
            }
            // Move index up to its child
            index = (index*2) + 1;
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
