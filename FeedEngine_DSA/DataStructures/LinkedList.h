#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

using namespace std;



template <class T>
class DoublyLinkedList {

public:

    // One link of the chain: the data, plus a pointer to each neighbour
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

private:

    Node* head;
    Node* tail;
    int count;

public:

    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Deep copy: builds new nodes instead of sharing the other list's nodes
    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {

        for (Node* curr = other.head; curr != nullptr; curr = curr->next)
            insertAtTail(curr->data);
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {

        if (this != &other) {
            clear();

            for (Node* curr = other.head; curr != nullptr; curr = curr->next)
                insertAtTail(curr->data);
        }

        return *this;
    }

    ~DoublyLinkedList() {
        clear();
    }

    // ===== INSERT =====

    void insertAtHead(const T& value) {

        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }

        count++;
    }

    // O(1) because we keep a tail pointer (no walk to the last node)
    void insertAtTail(const T& value) {

        Node* newNode = new Node(value);

        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        count++;
    }

    // Insert just before a node we already have. Four links change.
    void insertBefore(Node* curr, const T& value) {

        if (curr == nullptr)
            throw invalid_argument("insertBefore(): node is null");

        if (curr == head) {
            insertAtHead(value);
            return;
        }

        Node* newNode = new Node(value);

        newNode->prev = curr->prev;
        newNode->next = curr;
        curr->prev->next = newNode;
        curr->prev = newNode;

        count++;
    }

    // ===== DELETE =====

    // Unlink a node of this list and free it
    void deleteNode(Node* target) {

        if (target == nullptr)
            throw invalid_argument("deleteNode(): node is null");

        if (target->prev != nullptr)
            target->prev->next = target->next;
        else
            head = target->next;

        if (target->next != nullptr)
            target->next->prev = target->prev;
        else
            tail = target->prev;

        delete target;
        count--;
    }

    T deleteFromHead() {

        if (isEmpty())
            throw underflow_error("List underflow: deleteFromHead() on empty list");

        T value = head->data;
        deleteNode(head);
        return value;
    }

    T deleteFromTail() {

        if (isEmpty())
            throw underflow_error("List underflow: deleteFromTail() on empty list");

        T value = tail->data;
        deleteNode(tail);
        return value;
    }

    // Deletes the first node holding this value
    bool deleteValue(const T& value) {

        Node* target = search(value);

        if (target == nullptr)
            return false;

        deleteNode(target);
        return true;
    }

    // ===== SEARCH =====

    // Linear search from the head. Returns nullptr if not found.
    Node* search(const T& value) const {

        for (Node* curr = head; curr != nullptr; curr = curr->next)
            if (curr->data == value)
                return curr;

        return nullptr;
    }

    // ===== REVERSE =====

    // Swap prev and next in every node, then swap head and tail
    void reverse() {

        Node* curr = head;

        while (curr != nullptr) {
            Node* next = curr->next;
            curr->next = curr->prev;
            curr->prev = next;
            curr = next;
        }

        Node* oldHead = head;
        head = tail;
        tail = oldHead;
    }

   
    Node* getHead() const { return head; }

    Node* getTail() const { return tail; }

    int size() const { return count; }

    bool isEmpty() const { return count == 0; }

    void clear() {

        Node* curr = head;

        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }

        head = tail = nullptr;
        count = 0;
    }
};

#endif
