#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <stdexcept>

#include "HigherFirst.h"

using namespace std;



template <class T, class Compare = HigherFirst<T> >
class MaxHeap {

private:

    T* arr;
    int capacity;
    int count;
    Compare higherPriority;

    static int parentOf(int i) { return (i - 1) / 2; }

    static int leftChild(int i) { return 2 * i + 1; }

    static int rightChild(int i) { return 2 * i + 2; }

    // Doubles the array when it is full
    void grow() {

        int newCapacity = capacity * 2;
        T* bigger = new T[newCapacity];

        for (int i = 0; i < count; i++)
            bigger[i] = arr[i];

        delete[] arr;
        arr = bigger;
        capacity = newCapacity;
    }

    void swapItems(int a, int b) {
        T temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }

    // Move a new item up while it beats its parent
    void heapifyUp(int i) {

        while (i > 0 && higherPriority(arr[i], arr[parentOf(i)])) {
            swapItems(i, parentOf(i));
            i = parentOf(i);
        }
    }

    // Move an item down while one of its children beats it
    void heapifyDown(int i) {

        while (true) {

            int best = i;
            int left = leftChild(i);
            int right = rightChild(i);

            if (left < count && higherPriority(arr[left], arr[best]))
                best = left;

            if (right < count && higherPriority(arr[right], arr[best]))
                best = right;

            if (best == i)
                return;

            swapItems(i, best);
            i = best;
        }
    }

public:

    explicit MaxHeap(int initialCapacity = 16) : arr(nullptr), capacity(initialCapacity > 0 ? initialCapacity : 1), count(0), higherPriority() {
        arr = new T[capacity];
    }

    MaxHeap(const MaxHeap& other) : arr(nullptr), capacity(other.capacity), count(other.count), higherPriority(other.higherPriority) {

        arr = new T[capacity];

        for (int i = 0; i < count; i++)
            arr[i] = other.arr[i];
    }

    MaxHeap& operator=(const MaxHeap& other) {

        if (this != &other) {

            T* fresh = new T[other.capacity];

            for (int i = 0; i < other.count; i++)
                fresh[i] = other.arr[i];

            delete[] arr;

            arr = fresh;
            capacity = other.capacity;
            count = other.count;
            higherPriority = other.higherPriority;
        }

        return *this;
    }

    ~MaxHeap() {
        delete[] arr;
    }

    void insert(const T& value) {

        if (count == capacity)
            grow();

        arr[count] = value;
        count++;
        heapifyUp(count - 1);
    }

    // Removes and returns the root, then moves the last item to the root
    // and heapifies it down
    T extractMax() {

        if (isEmpty())
            throw underflow_error("Heap underflow: extractMax() on empty heap");

        T top = arr[0];
        count--;

        if (count > 0) {
            arr[0] = arr[count];
            heapifyDown(0);
        }

        return top;
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Heap underflow: peek() on empty heap");

        return arr[0];
    }

    // Floyd's build-heap: copy the items, then heapify down every
    // parent from the last one up to the root. O(n), faster than n inserts.
    void buildFrom(const T* values, int n) {

        count = 0;

        while (capacity < n)
            grow();

        for (int i = 0; i < n; i++)
            arr[i] = values[i];

        count = n;

        for (int i = n / 2 - 1; i >= 0; i--)
            heapifyDown(i);
    }

    // Item at array index i (index 0 is the root), to show the heap as a tree
    T at(int i) const {

        if (i < 0 || i >= count)
            throw out_of_range("Heap index out of range");

        return arr[i];
    }

    bool isEmpty() const { return count == 0; }

    int size() const { return count; }

    void clear() { count = 0; }
};

#endif
