#include <cstddef>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "coroutine.h"

#define _countOf(_Array) (sizeof(_Array) / sizeof(_Array[0]))

//TODO: Change this to be a dynamic mem alocation
int randomInts[10];

typedef struct dynamicArray
{
    void* data;
    size_t capacity;
    size_t size;
} vector;

inline static
void freeVector(vector *a) 
{
    free(a->data);
    a->data = NULL;
    a->capacity = a->size = 0;
}

inline static
void swap(int* a, int*b) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

inline static
int min(int a, int b) 
{
    return (a < b) ? a : b;
}

inline static
void copyArryInt(int* src, int n, int*& dest) {
    
    dest = (int *)realloc(dest, n*sizeof(int));

    for (int i=0 ; i < n; i++) {
       dest[i] = src[i];
    }
}

inline static
void copyArryIntToFloat(int* src, int n, float*& dest) {
        
    dest = (float *)realloc(dest, n*sizeof(float));
    for (int i=0 ; i < n; i++) {
        dest[i] = src[i];
    }
}

inline static
void generateNRandomInts(vector* vec, int n)
{
    if (n >= vec->capacity)
    {
        //TODO: make this error safe?
        vec->data = (int *)realloc(vec->data, n*sizeof(int));
        vec->capacity = n;
        vec->size = n;
    }
    
    srand(time(NULL));
    for (int i=0; i < vec->size; i++) {
        ((int*)(vec->data))[i] = rand()%1000;
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
    COROUTINE_CHECK_ACTIVE(co);
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
    COROUTINE_CHECK_ACTIVE(co);
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
    // Recursive, just not here;
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

    COROUTINE_CHECK_ACTIVE(co);
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
    COROUTINE_CHECK_ACTIVE(co);
    COROUTINE_START(co);
    //Make MaxHeap (all parent nodes > than child nodes)
    for( i = 1; i < count ; i++)
    {
        parent = (i-1) / 2;

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
                
                // Put cursor
                co->cursorPosition = j;
                COROUTINE_WAIT(co, co->time, co->dt);
                
            }
        }

        // Put cursor
        co->cursorPosition = i;
        COROUTINE_WAIT(co, co->time, co->dt);
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
            
            // Put cursor
            co->cursorPosition = index;
            COROUTINE_WAIT(co, co->time, co->dt);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}

// Insertion Sort: 
    // Compares each unsorted element with the max of partial sorted;
    // Creates a sorted partial array at beggingg of array;
    // Inserts element where it should in partial array;
    // Best case is O(n) -> array already sorted
inline static
bool insertionSort(int arrayToSort[], int count, coroutine_t* co) {

    static bool sorted;
    static int i, key, j;
    
    sorted = false;
    COROUTINE_CHECK_ACTIVE(co);
    COROUTINE_START(co);
    
    for (i = 1; i < count; i++)
    {
       key = arrayToSort[i]; 
       j = i - 1;

       // Move arrayToSort[0...i-1] elements > key
       // 1 up to make space 
       while(j >= 0 && arrayToSort[j] > key)
       {
           swap(&arrayToSort[j+1], &arrayToSort[j]);
            
           // Put cursor
           co->cursorPosition = j;
           COROUTINE_WAIT(co, co->time, co->dt);
           
           j--;
       }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}


#if 0 
// MergeSorts the arrayToSort[left, right] elements into arrayToSort
inline static
void merge(int* arrayToSort, int left, int middle, int right, coroutine_t* co) {

    int leftSize = middle - left+1; 
    int rightSize = right - middle;

    // Temp arrays
    std::vector<int> leftHalf(leftSize), rightHalf(rightSize);

    //Copy value to tmp arrays
    for (int i = 0; i <= leftSize; i++)
    {
        leftHalf[i] = arrayToSort[left + i];
    }
    
    for (int i = 0; i < rightSize; i++)
    {
        rightHalf[i] = arrayToSort[middle + i+1];
    }

    int leftPos = 0, rightPos = 0, arrayToSortPos = left;
    while (leftPos < leftSize && rightPos < rightSize)
    {
        if (leftHalf[leftPos] < rightHalf[rightPos])
        {
            arrayToSort[arrayToSortPos++] = leftHalf[leftPos++];
        }
        else
        {
            arrayToSort[arrayToSortPos++] = rightHalf[rightPos++];
        }
    }

    //Append the rest of the larger half 
    while (leftPos < leftSize)
    {
        arrayToSort[arrayToSortPos++] = leftHalf[leftPos++];
    }
    
    while (rightPos < rightSize)
    {
        arrayToSort[arrayToSortPos++] = rightHalf[rightPos++];
    }
    
    return;
}
#endif  

// Merge Sort: O(nlog(n)) 
    // Recursive, just not here; Not inplace!
    // Keep halfing original array into 2 item arrays;
    // Sort those arrays, and go back up merging them with other sorted arrays;
inline static
bool mergeSort(int arrayToSort[], int count, coroutine_t* co) 
{
    static bool sorted;
    static int m, i, from, middle, to;
    static int right, left;
    static int leftPos = 0, rightPos = 0, arrayToSortPos;
    static int leftSize, rightSize;
    static std::vector<int> leftHalf(0), rightHalf(0);

    sorted = false;
    COROUTINE_CHECK_ACTIVE(co);
    COROUTINE_START(co);

    // divide the array into blocks of size `m`
    // m = [1, 2, 4, 8, 16…]
    for (m = 1; m <= count-1; m = 2*m)
    {
        // Iterate through those blocks
        // for m = 1, i = 0, 2, 4, 6, 8…
        // for m = 2, i = 0, 4, 8…
        for (i = 0; i < count-1; i += 2*m)
        {
            left = i;
            
            // Given m values, the complete array may not have equal m "parts"
            // as such the right part might have less elements than the left
            right = min(i + 2*m - 1, count-1);
            
            middle = i + m - 1;
            if(middle <= right) 
            {

                leftSize = middle - left+1; 
                rightSize = right - middle;

                // Temp arrays: 
                // leftHalf[left, middle] and rightHalf[middle+1, right] 
                leftHalf.clear(), rightHalf.clear();
                leftHalf.resize(leftSize), rightHalf.resize(rightSize);

                //Copy value to tmp arrays
                for (int i = 0; i <= leftSize; i++)
                {
                    leftHalf[i] = arrayToSort[left + i];
                }

                for (int i = 0; i < rightSize; i++)
                {
                    rightHalf[i] = arrayToSort[middle + i+1];
                }

                // Merge the 2 halfs into the sorted array
                leftPos = 0, rightPos = 0, arrayToSortPos = left;
                while (leftPos < leftSize && rightPos < rightSize)
                {
                    if (leftHalf[leftPos] < rightHalf[rightPos])
                    {
                        arrayToSort[arrayToSortPos++] = leftHalf[leftPos++];
                    }
                    else
                    {
                        arrayToSort[arrayToSortPos++] = rightHalf[rightPos++];
                    }
                    //PUT cursor here
                    co->cursorPosition = arrayToSortPos;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

                //Append the rest of the larger half 
                while (leftPos < leftSize)
                {
                    arrayToSort[arrayToSortPos++] = leftHalf[leftPos++];
                    //PUT cursor here
                    co->cursorPosition = arrayToSortPos;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

                while (rightPos < rightSize)
                {
                    arrayToSort[arrayToSortPos++] = rightHalf[rightPos++];
                    //PUT cursor here
                    co->cursorPosition = arrayToSortPos;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

            }
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
