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
                    //Update cursor position
                    co->cursorPosition = arrayToSortPos - 1;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

                //Append the rest of the larger half 
                while (leftPos < leftSize)
                {
                    arrayToSort[arrayToSortPos++] = leftHalf[leftPos++];
                    //Update cursor position
                    co->cursorPosition = arrayToSortPos - 1;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

                while (rightPos < rightSize)
                {
                    arrayToSort[arrayToSortPos++] = rightHalf[rightPos++];
                    //Update cursor position
                    co->cursorPosition = arrayToSortPos - 1;
                    COROUTINE_WAIT(co, co->time, co->dt);
                }

            }
        }
    }

    sorted = true;
    COROUTINE_END(co);
    return sorted;
}
