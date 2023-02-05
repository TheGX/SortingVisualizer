// Heap Sort: O(nlog(n))
    // Place original array into heap;
    // Remove heap root (max element), resulting in sorted array;
    // Update heap structure along the way;
    // Best case is O(n) -> array already sorted;
inline static
bool heapSort(int arrayToSort[], int count, coroutine_t* co) 
{
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
                
                // Update cursor position
                co->cursorPosition = j;
                COROUTINE_WAIT(co, co->time, co->dt);
                
            }
        }

        // Update cursor position
        co->cursorPosition = i;
        COROUTINE_WAIT(co, co->time, co->dt);
    }
    
    //Swap root node to the end, update the heap structure along the way 
    for( i = count - 1; i > 0; i--)
    {
        // Update cursor position
        co->cursorPosition = i;
        COROUTINE_WAIT(co, co->time, co->dt);
        
        swap(&arrayToSort[0], &arrayToSort[i]);

        // Run through the heap, jumping from child to child
        for (index = 1; index < i; index = (index*2) + 1) 
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
            //index = (index*2) + 1;
            
            // Update cursor position
            co->cursorPosition = index;
            COROUTINE_WAIT(co, co->time, co->dt);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
