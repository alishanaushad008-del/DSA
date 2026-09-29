#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int val) {
        data = val;
        prev = nullptr;
        next = nullptr;
    }
};

class Deque {
private:
    Node* front;
    Node* rear;

public:
    Deque() {
        front = nullptr;
        rear = nullptr;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void insertFront(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        cout << val << " inserted at front" << endl;
    }

    void insertRear(int val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            newNode->prev = rear;
            rear->next = newNode;
            rear = newNode;
        }
        cout << val << " inserted at rear" << endl;
    }

    void deleteFront() {
        if (isEmpty()) {
            cout << "Deque Underflow!" << endl;
            return;
        }
        Node* temp = front;
        cout << temp->data << " deleted from front" << endl;
        
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        } else {
            front->prev = nullptr;
        }
        delete temp;
    }

    void deleteRear() {
        if (isEmpty()) {
            cout << "Deque Underflow!" << endl;
            return;
        }
        Node* temp = rear;
        cout << temp->data << " deleted from rear" << endl;

        rear = rear->prev;
        if (rear == nullptr) {
            front = nullptr;
        } else {
            rear->next = nullptr;
        }
        delete temp;
    }

    void getFront() {
        if (isEmpty()) {
            cout << "Deque is Empty!" << endl;
        } else {
            cout << "Front element: " << front->data << endl;
        }
    }

    void getRear() {
        if (isEmpty()) {
            cout << "Deque is Empty!" << endl;
        } else {
            cout << "Rear element: " << rear->data << endl;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Deque is Empty!" << endl;
            return;
        }
        Node* temp = front;
        cout << "Deque elements: ";
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~Deque() {
        while (!isEmpty()) {
            deleteFront();
        }
    }
};

int main() {
    Deque dq;

    dq.insertRear(10);
    dq.insertRear(20);
    dq.insertFront(5);
    dq.insertFront(2);

    dq.display();

    dq.getFront();
    dq.getRear();

    dq.deleteFront();
    dq.display();

    dq.deleteRear();
    dq.display();

    return 0;
}