#include <iostream>
#include <string>
using namespace std;

struct Node {
    string url;
    Node* prev;
    Node* next;
    
    Node(string u) {
        url = u;
        prev = nullptr;
        next = nullptr;
    }
};

class BrowserHistory {
private:
    Node* curr; 

public:
   
    BrowserHistory(string homepage) {
        curr = new Node(homepage);
    }

 
    void visit(string url) {
        Node* newNode = new Node(url);
        newNode->prev = curr;

    
        Node* temp = curr->next;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        curr->next = newNode;
        curr = newNode;
    }

  
    string back(int steps) {
        while (steps > 0 && curr->prev != nullptr) {
            curr = curr->prev;
            steps--;
        }
        return curr->url;
    }

    
    string forward(int steps) {
        while (steps > 0 && curr->next != nullptr) {
            curr = curr->next;
            steps--;
        }
        return curr->url;
    }
};

int main() {
    BrowserHistory* browserHistory = new BrowserHistory("leetcode.com");

    browserHistory->visit("google.com");     
    browserHistory->visit("facebook.com");  
    browserHistory->visit("youtube.com");   

    cout << "back(1): " << browserHistory->back(1) << endl;       
    cout << "back(1): " << browserHistory->back(1) << endl;      
    cout << "forward(1): " << browserHistory->forward(1) << endl; 

    browserHistory->visit("linkedin.com");   

    cout << "forward(2): " << browserHistory->forward(2) << endl; 
    cout << "back(2): " << browserHistory->back(2) << endl;    
    cout << "back(7): " << browserHistory->back(7) << endl;      

    return 0;
}