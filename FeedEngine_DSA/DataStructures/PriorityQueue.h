#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdexcept>

#include "LinkedList.h"
#include "HigherFirst.h"

using namespace std;


template <class T, class Compare = HigherFirst<T> >
class PriorityQueue {

private:

    DoublyLinkedList<T> items;
    Compare higherPriority;

public:

    PriorityQueue() : items(), higherPriority() {}

    explicit PriorityQueue(Compare compare) : items(), higherPriority(compare) {}

    void enqueue(const T& value) {

        typename DoublyLinkedList<T>::Node* curr = items.getHead();

        // Skip every item that should still leave before the new one
        // (equal priority stays in front, which keeps FIFO order)
        while (curr != nullptr && !higherPriority(value, curr->data))
            curr = curr->next;

        if (curr == nullptr)
            items.insertAtTail(value);
        else
            items.insertBefore(curr, value);
    }

    T dequeue() {

        if (isEmpty())
            throw underflow_error("Priority queue underflow: dequeue() on empty queue");

        return items.deleteFromHead();
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Priority queue underflow: peek() on empty queue");

        return items.getHead()->data;
    }

    // Takes one item out wherever it is. O(n)
    bool remove(const T& value) {
        return items.deleteValue(value);
    }

    // Walk from highest to lowest priority without removing anything
    // (e.g. show the top 5 trending posts)
    typename DoublyLinkedList<T>::Node* getHead() const { return items.getHead(); }

    bool isEmpty() const { return items.isEmpty(); }

    int size() const { return items.size(); }

    void clear() { items.clear(); }
};

#endif
