// Buble Sort: O(n^2):
    // Compare element 2 by 2;
    // Swap if need;
    // Best case is O(n) -> array already sorted
    // [12, 6, 3, 7]
    // [6, 12, 3, 7]
    // [6, 3, 12, 7]

inline static
bool bubleSort(int* arrayToSort, int count, coroutine_t* co)
{
    static bool sorted;
    static int i=0, j=0;

    sorted = false;
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
            //Update cursor position
            co->cursorPosition = j;
            COROUTINE_WAIT(co, co->time, co->dt);
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
