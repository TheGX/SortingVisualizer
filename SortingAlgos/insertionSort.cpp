// Insertion Sort: 
    // Compares each unsorted element with the max of partial sorted;
    // Creates a sorted partial array at beggingg of array;
    // Inserts element where it should in partial array;
    // Best case is O(n) -> array already sorted
inline static
bool insertionSort(int arrayToSort[], int count, coroutine_t* co) 
{
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
            
           // Update cursor position
           co->cursorPosition = j;
           COROUTINE_WAIT(co, co->time, co->dt);
           
           j--;
       }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
