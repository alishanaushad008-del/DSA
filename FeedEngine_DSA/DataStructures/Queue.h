#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

using namespace std;



template <class T>
class Queue {

private:

    T* arr;
    int capacity;
    int front;
    int rear;
    int count;

public:

    explicit Queue(int maxSize = 10) : arr(nullptr), capacity(maxSize > 0 ? maxSize : 1), front(0), rear(-1), count(0) {
        arr = new T[capacity];
    }

    Queue(const Queue& other) : arr(nullptr), capacity(other.capacity), front(other.front), rear(other.rear), count(other.count) {

        arr = new T[capacity];

        for (int i = 0; i < count; i++) {
            int index = (front + i) % capacity;
            arr[index] = other.arr[index];
        }
    }

    Queue& operator=(const Queue& other) {

        if (this != &other) {

            T* fresh = new T[other.capacity];

            for (int i = 0; i < other.count; i++) {
                int index = (other.front + i) % other.capacity;
                fresh[index] = other.arr[index];
            }

            delete[] arr;

            arr = fresh;
            capacity = other.capacity;
            front = other.front;
            rear = other.rear;
            count = other.count;
        }

        return *this;
    }

    ~Queue() {
        delete[] arr;
    }

    void enqueue(const T& value) {

        if (isFull())
            throw overflow_error("Queue overflow: enqueue() on full queue");

        rear = (rear + 1) % capacity;
        arr[rear] = value;
        count++;
    }

    T dequeue() {

        if (isEmpty())
            throw underflow_error("Queue underflow: dequeue() on empty queue");

        T value = arr[front];
        front = (front + 1) % capacity;
        count--;

        return value;
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Queue underflow: peek() on empty queue");

        return arr[front];
    }

    // i-th item counted from the front (0 = oldest), to show the
    // whole queue without removing anything
    T at(int i) const {

        if (i < 0 || i >= count)
            throw out_of_range("Queue index out of range");

        return arr[(front + i) % capacity];
    }

    bool isEmpty() const { return count == 0; }

    bool isFull() const { return count == capacity; }

    int size() const { return count; }

    int getCapacity() const { return capacity; }

    void clear() {
        front = 0;
        rear = -1;
        count = 0;
    }
};

#endif
