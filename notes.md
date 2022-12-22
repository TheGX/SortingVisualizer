
### Selection Sort: O(n^2):
        * Run through array selecting the current overall min and placing in the beggining.

### Buble Sort: O(n^2):
        * Compare element 2 by 2;
        * Swap if need;
        * Best case is O(n) -> array already sorted

### Insertion Sort: O(n^2)
        * Creates a sorted partial array at beggingg of array;
        * Compares each unsorted element with the max of partial sorted;
        * Inserts element where it should in partial array;
        * Best case is O(n) -> array already sorted

### Heap Sort: O(nlog(n))
        * Place original array into heap;
        * Remove heap root (max element), resulting in sorted array;
        * Update heap structure along the way;
        * Best case is O(n) -> array already sorted;

### Quick Sort: O(nlog(n)) - worst is O(n^2)
        * Recursive;
        * Pick a pivot;
        * Swap itemLeft and itemRight:
                ** itemLeft: starting left, item smaller than pivot;
                ** itemRight: starting Right, item larger than pivot;
        * Swap itemLeft with pivot if itemLeft_index < itemRight_index;

### Merge Sort: O(nlog(n)) 
        * Recursive;
        * Keep halfing original array into 2 item arrays;
        * Sort those arrays, and go back up merging them with other sorted arrays;

