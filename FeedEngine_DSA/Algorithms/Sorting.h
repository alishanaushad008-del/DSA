#ifndef SORTING_H
#define SORTING_H



struct SortStats {
    int comparisons;
    int swaps;          // insertion sort: each shift, merge sort: each item copied back
    int passes;         // merge / quick sort: each merge / partition step
};


template <class T>
struct Ascending {

    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <class T>
void swapValues(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// ===== SELECTION SORT =====


template <class T, class Compare>
SortStats selectionSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 0; i < n - 1; i++) {

        stats.passes++;
        int best = i;

        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;

            if (comesBefore(arr[j], arr[best]))
                best = j;
        }

        if (best != i) {
            swapValues(arr[i], arr[best]);
            stats.swaps++;
        }
    }

    return stats;
}

// ===== BUBBLE SORT =====
// Swap neighbours that are out of order; the largest bubbles to the end.
// Stops early when a whole pass makes no swap.

template <class T, class Compare>
SortStats bubbleSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 0; i < n - 1; i++) {

        bool swapped = false;
        stats.passes++;

        // the last i items are already in place
        for (int j = 0; j < n - i - 1; j++) {
            stats.comparisons++;

            if (comesBefore(arr[j + 1], arr[j])) {
                swapValues(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return stats;
}

// ===== INSERTION SORT =====
// Take each key and insert it into the sorted left part,
// shifting bigger items one place to the right.

template <class T, class Compare>
SortStats insertionSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 1; i < n; i++) {

        stats.passes++;
        T key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (!comesBefore(key, arr[j]))
                break;

            arr[j + 1] = arr[j];
            stats.swaps++;
            j--;
        }

        arr[j + 1] = key;
    }

    return stats;
}


template <class T, class Compare>
void mergeHalves(T* arr, T* temp, int low, int mid, int high, Compare comesBefore, SortStats& stats) {

    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high) {
        stats.comparisons++;

       
        if (comesBefore(arr[j], arr[i]))
            temp[k++] = arr[j++];
        else
            temp[k++] = arr[i++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= high)
        temp[k++] = arr[j++];

    for (k = low; k <= high; k++) {
        arr[k] = temp[k];
        stats.swaps++;
    }
}

template <class T, class Compare>
void mergeSortRange(T* arr, T* temp, int low, int high, Compare comesBefore, SortStats& stats) {

    if (low >= high)        // base case: 0 or 1 item is already sorted
        return;

    int mid = low + (high - low) / 2;

    mergeSortRange(arr, temp, low, mid, comesBefore, stats);
    mergeSortRange(arr, temp, mid + 1, high, comesBefore, stats);

    stats.passes++;
    mergeHalves(arr, temp, low, mid, high, comesBefore, stats);
}

template <class T, class Compare>
SortStats mergeSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    if (n < 2)
        return stats;

    T* temp = new T[n];
    mergeSortRange(arr, temp, 0, n - 1, comesBefore, stats);
    delete[] temp;

    return stats;
}

// ===== QUICK SORT (Week 7, divide and conquer) =====
// Pick the last item as the pivot, move everything that comes before it
// to its left (partition), then quick sort both sides.
// If the data is already sorted, every pivot splits off just one item,
// which is the O(n^2) worst case.

// Lomuto partition: returns the pivot's final index
template <class T, class Compare>
int quickPartition(T* arr, int low, int high, Compare comesBefore, SortStats& stats) {

    T pivot = arr[high];
    int i = low - 1;        // last index of the "before pivot" part

    for (int j = low; j < high; j++) {
        stats.comparisons++;

        if (comesBefore(arr[j], pivot)) {
            i++;

            if (i != j) {
                swapValues(arr[i], arr[j]);
                stats.swaps++;
            }
        }
    }

    if (i + 1 != high) {
        swapValues(arr[i + 1], arr[high]);
        stats.swaps++;
    }

    return i + 1;
}

template <class T, class Compare>
void quickSortRange(T* arr, int low, int high, Compare comesBefore, SortStats& stats) {

    if (low >= high)        // base case: 0 or 1 item
        return;

    stats.passes++;
    int pivotIndex = quickPartition(arr, low, high, comesBefore, stats);

    quickSortRange(arr, low, pivotIndex - 1, comesBefore, stats);
    quickSortRange(arr, pivotIndex + 1, high, comesBefore, stats);
}

template <class T, class Compare>
SortStats quickSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};
    quickSortRange(arr, 0, n - 1, comesBefore, stats);
    return stats;
}

// ===== HEAP SORT (Week 10) =====
// Build a max-heap inside the array (the item that comes last is at the
// root), then swap the root to the end and fix the heap, n - 1 times.

// Moves arr[i] down until both children come before it (heap of size n)
template <class T, class Compare>
void heapSiftDown(T* arr, int n, int i, Compare comesBefore, SortStats& stats) {

    while (true) {

        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n) {
            stats.comparisons++;

            if (comesBefore(arr[largest], arr[left]))
                largest = left;
        }

        if (right < n) {
            stats.comparisons++;

            if (comesBefore(arr[largest], arr[right]))
                largest = right;
        }

        if (largest == i)
            return;

        swapValues(arr[i], arr[largest]);
        stats.swaps++;
        i = largest;
    }
}

template <class T, class Compare>
SortStats heapSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    // build the heap: heapify every parent, last one first. O(n)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapSiftDown(arr, n, i, comesBefore, stats);

    // move the root (the last item in sorted order) to the end, shrink the heap
    for (int end = n - 1; end > 0; end--) {
        stats.passes++;

        swapValues(arr[0], arr[end]);
        stats.swaps++;

        heapSiftDown(arr, end, 0, comesBefore, stats);
    }

    return stats;
}

// Same sorts with the default order (smallest first)

template <class T>
SortStats selectionSort(T* arr, int n) {
    return selectionSort(arr, n, Ascending<T>());
}

template <class T>
SortStats bubbleSort(T* arr, int n) {
    return bubbleSort(arr, n, Ascending<T>());
}

template <class T>
SortStats insertionSort(T* arr, int n) {
    return insertionSort(arr, n, Ascending<T>());
}

template <class T>
SortStats mergeSort(T* arr, int n) {
    return mergeSort(arr, n, Ascending<T>());
}

template <class T>
SortStats quickSort(T* arr, int n) {
    return quickSort(arr, n, Ascending<T>());
}

template <class T>
SortStats heapSort(T* arr, int n) {
    return heapSort(arr, n, Ascending<T>());
}

#endif
