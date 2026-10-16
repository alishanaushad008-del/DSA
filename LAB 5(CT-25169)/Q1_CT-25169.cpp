#include <iostream>
#include <vector>

using namespace std;

class MyCircularDeque {
private:
    vector<int> arr;
    int capacity;
    int front;
    int rear;
    int count;

public:
    MyCircularDeque(int k) {
        capacity = k;
        arr.resize(k);
        front = 0;
        rear = k - 1;
        count = 0;
    }

    bool insertFront(int value) {
        if (isFull()) {
            return false;
        }
        front = (front - 1 + capacity) % capacity;
        arr[front] = value;
        count++;
        return true;
    }

    bool insertLast(int value) {
        if (isFull()) {
            return false;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = value;
        count++;
        return true;
    }

    bool deleteFront() {
        if (isEmpty()) {
            return false;
        }
        front = (front + 1) % capacity;
        count--;
        return true;
    }

    bool deleteLast() {
        if (isEmpty()) {
            return false;
        }
        rear = (rear - 1 + capacity) % capacity;
        count--;
        return true;
    }

    int getFront() {
        if (isEmpty()) {
            return -1;
        }
        return arr[front];
    }

    int getRear() {
        if (isEmpty()) {
            return -1;
        }
        return arr[rear];
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }
};

int main() {
    MyCircularDeque myCircularDeque(3);
    cout << boolalpha << myCircularDeque.insertLast(1) << endl;
    cout << boolalpha << myCircularDeque.insertLast(2) << endl;
    cout << boolalpha << myCircularDeque.insertFront(3) << endl;
    cout << boolalpha << myCircularDeque.insertFront(4) << endl;
    cout << myCircularDeque.getRear() << endl;
    cout << boolalpha << myCircularDeque.isFull() << endl;
    cout << boolalpha << myCircularDeque.deleteLast() << endl;
    cout << boolalpha << myCircularDeque.insertFront(4) << endl;
    cout << myCircularDeque.getFront() << endl;
    return 0;
}