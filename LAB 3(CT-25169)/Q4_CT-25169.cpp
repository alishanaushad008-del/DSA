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

    void sortDescending() {
        if (head == nullptr) return;

        bool swapped;
        Node* ptr1;
        Node* lptr = nullptr;

        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
 
                if (ptr1->data < ptr1->next->data) {
                    int temp = ptr1->data;
                    ptr1->data = ptr1->next->data;
                    ptr1->next->data = temp;
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
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
    DoublyLinkedList L, M;

   
    for (int i = 2; i <= 10; i += 2) L.insertEnd(i);

  
    for (int i = 1; i <= 9; i += 2) M.insertEnd(i);

  
    DoublyLinkedList N = concatenate(L, M);

    cout << "List N before sorting: ";
    N.display();

 
    N.sortDescending();

    cout << "List N after Descending Sort: ";
    N.display();

    return 0;
}