#ifndef STACK_H
#define STACK_H

#include <stdexcept>

using namespace std;



template <class T>
class Stack {

public:

    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* below) : data(value), next(below) {}
    };

private:

    Node* top;
    int count;

    // Copies the other stack keeping the same top-to-bottom order
    void copyFrom(const Stack& other) {

        Node* last = nullptr;

        for (Node* curr = other.top; curr != nullptr; curr = curr->next) {

            Node* newNode = new Node(curr->data, nullptr);

            if (last == nullptr)
                top = newNode;
            else
                last->next = newNode;

            last = newNode;
        }

        count = other.count;
    }

public:

    Stack() : top(nullptr), count(0) {}

    Stack(const Stack& other) : top(nullptr), count(0) {
        copyFrom(other);
    }

    Stack& operator=(const Stack& other) {

        if (this != &other) {
            clear();
            copyFrom(other);
        }

        return *this;
    }

    ~Stack() {
        clear();
    }

    void push(const T& value) {
        top = new Node(value, top);
        count++;
    }

    T pop() {

        if (isEmpty())
            throw underflow_error("Stack underflow: pop() on empty stack");

        Node* temp = top;
        T value = temp->data;

        top = top->next;
        delete temp;
        count--;

        return value;
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Stack underflow: peek() on empty stack");

        return top->data;
    }

    // Walk top -> bottom without popping:
    // for (Stack<T>::Node* curr = s.getTop(); curr != nullptr; curr = curr->next)
    Node* getTop() const { return top; }

    bool isEmpty() const { return top == nullptr; }

    int size() const { return count; }

    void clear() {

        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }

        count = 0;
    }
};

#endif
