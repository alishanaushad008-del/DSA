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

class DoublyLinkedList {
public:
    Node* head;
    Node* tail;

    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    void insertEnd(int val) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};


DoublyLinkedList concatenate(DoublyLinkedList& L, DoublyLinkedList& M) {
    DoublyLinkedList N;

   
    Node* temp = L.head;
    while (temp != nullptr) {
        N.insertEnd(temp->data);
        temp = temp->next;
    }

    
    temp = M.head;
    while (temp != nullptr) {
        N.insertEnd(temp->data);
        temp = temp->next;
    }

    return N;
}

int main() {
    DoublyLinkedList L;
    DoublyLinkedList M;

    
    for (int i = 2; i <= 10; i += 2) {
        L.insertEnd(i);
    }

 
    for (int i = 1; i <= 9; i += 2) {
        M.insertEnd(i);
    }

    cout << "List L (Evens): ";
    L.display();

    cout << "List M (Odds): ";
    M.display();

   
    DoublyLinkedList N = concatenate(L, M);

    cout << "List N (Concatenated L + M): ";
    N.display();

    return 0;
}