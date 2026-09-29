#include <iostream>
#include <stdexcept>
using namespace std;
template <typename T>
class Queue {
private:
T* data;
size_t capacity;
size_t front;
size_t rear;
size_t size;
void resize() {
size_t newCapacity = capacity * 2;
T* newData = new T[newCapacity];
for (size_t i = 0; i < size; ++i) {
newData[i] = data[(front + i) % capacity];
}
delete[] data;
data = newData;
capacity = newCapacity;
front = 0;
rear = size;
}
public:
Queue(size_t initialCapacity = 4) : capacity(initialCapacity), front(0), rear(0), size(0) {

4
Data Structure Algorithms & Applications (CT-159)
Lab 03
data = new T[capacity];
}
~Queue() {
delete[] data;
}
void enqueue(const T& value) {
if (size == capacity) {
resize();
}
data[rear] = value;
rear = (rear + 1) % capacity;
++size;
}
void dequeue() {
if (isEmpty()) {
throw out_of_range("Queue is empty");
}
front = (front + 1) % capacity;
--size;
}
T& peek() const {
if (isEmpty()) {
throw out_of_range("Queue is empty");
}
return data[front];
}
bool isEmpty() const {
return size == 0;
}
size_t getSize() const {
return size;
}
};
int main() {
Queue<int> q;
q.enqueue(1);
q.enqueue(2);
q.enqueue(3);
cout << "Front element: " << q.peek() << endl;
q.dequeue();
cout << "Front element after dequeue: " << q.peek() << endl;
cout << "Queue size: " << q.getSize() << endl;
return 0;
}