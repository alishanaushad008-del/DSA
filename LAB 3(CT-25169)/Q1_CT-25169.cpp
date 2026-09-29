#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) {
        data = val;
        next = nullptr;
    }
};

class CircularQueue {
private:
    Node* rear; 

public:
    CircularQueue() {
        rear = nullptr;
    }

    bool isEmpty() {
        return rear == nullptr;
    }

    void enqueue(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            newNode->next = newNode; 
            rear = newNode;
        } else {
            newNode->next = rear->next; 
            rear->next = newNode;      
            rear = newNode;            
        }
        cout << val << " enqueued" << endl;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue Underflow!" << endl;
            return;
        }

        Node* front = rear->next;
        
      
        if (front == rear) {
            cout << front->data << " dequeued" << endl;
            delete front;
            rear = nullptr;
        } else {
            cout << front->data << " dequeued" << endl;
            rear->next = front->next; 
            delete front;
        }
    }

    void getFront() {
        if (isEmpty()) {
            cout << "Queue is Empty!" << endl;
        } else {
            cout << "Front element: " << rear->next->data << endl;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Queue is Empty!" << endl;
            return;
        }

        Node* temp = rear->next; 
        cout << "Queue elements: ";
        do {
            cout << temp->data << " ";
            temp = temp->next;
        } while (temp != rear->next);
        cout << endl;
    }

    ~CircularQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

int main() {
    CircularQueue q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    q.display();
    q.getFront();

    q.dequeue();
    q.display();

    q.enqueue(40);
    q.display();

    return 0;
}