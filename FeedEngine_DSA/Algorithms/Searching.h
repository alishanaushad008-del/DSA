#ifndef SEARCHING_H
#define SEARCHING_H

// ===== SEARCHING (Week 1) =====
//
//   Linear search: check the items one by one from the start.       O(n)
//                  Works on any order.
//
//   Binary search: look at the middle item. If the key is bigger,
//                  throw away the left half, else the right half.
//                  Repeat until found.                               O(log n)
//                  Only works on SORTED data in an ARRAY (we need to
//                  jump to the middle, which a linked list can't do).
//
// Every search returns where it found the key (-1 if not found) and how
// many items it checked, so the Search menu can show the difference.
//
// keyOf(item) says what to compare: for posts it gives the post's ID.

struct SearchResult {
    int index;
    int steps;
};

// Default key: the item itself
template <class T>
struct ItemItself {

    const T& operator()(const T& item) const {
        return item;
    }
};

// ===== LINEAR SEARCH =====

template <class T, class K, class KeyOf>
SearchResult linearSearch(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};

    for (int i = 0; i < n; i++) {
        result.steps++;

        if (keyOf(arr[i]) == key) {
            result.index = i;
            return result;
        }
    }

    return result;
}

// ===== BINARY SEARCH (iterative) =====
// arr must be sorted by keyOf, smallest first.

template <class T, class K, class KeyOf>
SearchResult binarySearch(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};
    int low = 0;
    int high = n - 1;

    while (low <= high) {

        int mid = low + (high - low) / 2;   // same as (low + high) / 2, but cannot overflow
        result.steps++;

        if (keyOf(arr[mid]) == key) {
            result.index = mid;
            return result;
        }

        if (keyOf(arr[mid]) < key)
            low = mid + 1;      // key is in the right half
        else
            high = mid - 1;     // key is in the left half
    }

    return result;
}

// ===== BINARY SEARCH (recursive, Week 4) =====
// Base case: low > high means the range is empty, so the key is not there.

template <class T, class K, class KeyOf>
int binarySearchRange(const T* arr, int low, int high, const K& key, KeyOf keyOf, int& steps) {

    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;
    steps++;

    if (keyOf(arr[mid]) == key)
        return mid;

    if (keyOf(arr[mid]) < key)
        return binarySearchRange(arr, mid + 1, high, key, keyOf, steps);

    return binarySearchRange(arr, low, mid - 1, key, keyOf, steps);
}

template <class T, class K, class KeyOf>
SearchResult binarySearchRecursive(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};
    result.index = binarySearchRange(arr, 0, n - 1, key, keyOf, result.steps);
    return result;
}

// Same searches when the item itself is the key

template <class T>
SearchResult linearSearch(const T* arr, int n, const T& key) {
    return linearSearch(arr, n, key, ItemItself<T>());
}

template <class T>
SearchResult binarySearch(const T* arr, int n, const T& key) {
    return binarySearch(arr, n, key, ItemItself<T>());
}

template <class T>
SearchResult binarySearchRecursive(const T* arr, int n, const T& key) {
    return binarySearchRecursive(arr, n, key, ItemItself<T>());
}

#endif
