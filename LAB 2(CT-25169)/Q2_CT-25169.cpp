/*Given the head of a sorted linked list, delete all duplicates such that each element appears only once. 
Return the linked list sorted as well. */
#include<iostream>
 using namespace std;

struct Node {
    int data;
    Node* next;
    
    Node(int val) {
        data = val;
        next = nullptr;
    }
};
Node* deleteDuplicates(Node* head) {

    if (head == nullptr) {
        return head;
    }

    Node* current = head;

  
    while (current != nullptr && current->next != nullptr) {
        if (current->data == current->next->data) {
             current->next = current->next->next;
        } else {
          
            current = current->next;
        }
    }

    return head;
}

void printlist(Node* head) {
    while (head != nullptr) {
        cout << head->data << " -> ";
        head = head->next;
    }
    cout << "NULL" << endl;
}


int main() {

    Node* head = new Node(1);
    head->next = new Node(1);
    head->next->next = new Node(2);
    head->next->next->next = new Node(3);
    head->next->next->next->next = new Node(3);

    cout << "Original List:" << endl;
    printlist(head);

   
    head = deleteDuplicates(head);

    cout << "\nList After Removing Duplicates:" << endl;
    printlist(head);

    return 0;
}
