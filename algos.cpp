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

//  Quick Sort: O(nlog(n)) - worst is O(n^2)
    // Recursive;
    // Pick a pivot;
    // Swap itemLeft and itemRight:
    // itemLeft: starting left, item smaller than pivot;
    // itemRight: starting Right, item larger than pivot;
    // Swap itemLeft with pivot if itemLeft_index < itemRight_index;


inline static
int partition(int arr[], int low, int high, coroutine_t* co) {

	int pivot = arr[high];  // pivot
	int i = (low - 1);  // Index of smaller element
    int j;
    COROUTINE_START(co);
	for (j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot
		if (arr[j] < pivot)
		{
			i++;  // increment index of smaller element
			swap(&arr[i], &arr[j]);
        
            //PUT cursor here
            //co->cursorPosition = i;
            COROUTINE_WAIT(co, 10, 5.0f);
        }
	}
	swap(&arr[i + 1], &arr[high]);
    COROUTINE_END(co);
	return (i + 1);
}

inline static
bool quickSort(int arr[], int low, int high, coroutine_t* co) {

    if (low < high) {
	/* pi is partitioning index, arr[p] is now
	   at right place */
        int pi = partition(arr, low, high, co);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1, co);
        quickSort(arr, pi + 1, high, co);
        return true;
    }
        return false;
}

inline static
bool quickSortCaller(int* arrayToSort, int count, coroutine_t* co){

    return quickSort(arrayToSort, 0, count - 1, co);

}
