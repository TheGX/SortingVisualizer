
// Selection Sort: O(n^2):
    // Run across array, finding min, swap min with the 1st value
    // Run again, swap with 2nd, etc....
    // [12, 6, 3, 7]
    // [6, 12, 3, 7]
    // [3, 12, 6, 7]
inline static
bool selectionSort(int* arrayToSort, int count, coroutine_t* co)
{

    static bool sorted;
    static int i=0, j=0;

    sorted = false;
    COROUTINE_CHECK_ACTIVE(co);
    COROUTINE_START(co);
    for (i = 0; i < count; i++) {

        for (j=i+1; j < count; j++) {

            if ( arrayToSort[j] < arrayToSort[i] ) {
                swap(&arrayToSort[j], &arrayToSort[i]);
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
