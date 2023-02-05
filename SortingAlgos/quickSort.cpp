//  Quick Sort: O(nlog(n)) - worst is O(n^2)
    // Recursive, just not here;
    // Pick a pivot;
    // Swap itemLeft and itemRight:
    // itemLeft: starting left, item smaller than pivot;
    // itemRight: starting Right, item larger than pivot;
    // Swap itemLeft with pivot if itemLeft_index < itemRight_index;

// The iterative version had to be used to be compitable with this Coroutines implementation
inline static
bool quickSort(int arrayToSort[], int count, coroutine_t* co) 
{
    static int lower, upper;
    static bool sorted;
    static std::vector<int> stack(count, 0);

    static int top, i, j, pivot_index;
    static unsigned pivot;

    sorted = false;
    COROUTINE_CHECK_ACTIVE(co);
    COROUTINE_START(co);
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

            // Update cursor position
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

