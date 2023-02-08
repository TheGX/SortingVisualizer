#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "coroutine.h"

#define _countOf(_Array) (sizeof(_Array) / sizeof(_Array[0]))

typedef bool (*sortingFunction) (int* arrayToSort, int count, coroutine_t* co);

typedef struct dynamicArray
{
    void* data;
    size_t capacity;
    size_t size;
} Vector;

inline static
void freeVector(Vector *a) 
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
void copyVector(Vector* src, Vector*& dest) 
{
    //TODO: only do realloc on dynamic condition that dest.capacity is smaller than src.capacity
    dest->size = src->size;
    dest->capacity = src->capacity;
   
    dest->data = (int *)realloc(dest->data, dest->capacity*sizeof(int));
    memcpy(dest->data, src->data, dest->capacity*sizeof(int));
}

inline static
void generateNRandomInts(Vector* vec, int n)
{
    if (n >= vec->capacity)
    {
        //TODO: make this error safe?
        vec->data = (int *)realloc(vec->data, n*sizeof(int));
        vec->capacity = n;
        vec->size = n;
    }
    
    srand(time(NULL));
    for (int i=0; i < vec->size; i++) 
    {
        ((int*)(vec->data))[i] = rand()%1000;
    }
}

inline static
void updateSequence(Vector* randomInts, Vector*& vectorToSort, 
                    coroutine_t* co, bool& sorted)
{
    generateNRandomInts(randomInts, randomInts->capacity);

    //"Reset" array to be sorted back to random
    copyVector(randomInts, vectorToSort);
    co->active = false;
    sorted = false;
}
