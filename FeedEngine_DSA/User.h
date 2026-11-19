#ifndef USER_H
#define USER_H

#include <string>

#include "Functions.h"
#include "DataStructures/LinkedList.h"
#include "DataStructures/Stack.h"
#include "DataStructures/Queue.h"

using namespace std;


class User {

public:

    static const int MAX_NOTIFICATIONS = 10;

private:

    string username;                    // always lowercase, used to log in
    string name;
    int age;
    DoublyLinkedList<string> interests;
    Stack<int> likeHistory;             // liked post IDs, most recent on top
    Queue<string> notifications;        // oldest first, keeps the newest 10

public:

    User(string u, string n, int a) : username(toLowerCase(u)), name(n), age(a), interests(), likeHistory(), notifications(MAX_NOTIFICATIONS) {}

    string getUsername() const { return username; }

    string getName() const { return name; }

    int getAge() const { return age; }

    // ===== INTERESTS (Doubly Linked List) =====

    // Adds an interest unless the user already has it. O(n)
    void addInterest(const string& interest) {

        for (DoublyLinkedList<string>::Node* curr = interests.getHead(); curr != nullptr; curr = curr->next)
            if (toLowerCase(curr->data) == toLowerCase(interest))
                return;

        interests.insertAtTail(interest);
    }

    const DoublyLinkedList<string>& getInterests() const { return interests; }

    // Ignores capital letters. 3 or more letters at the start also count,
    // so "tech" matches "Technology", but "a" does not match everything.
    bool isInterestedIn(const string& category) const {

        string b = toLowerCase(category);

        for (DoublyLinkedList<string>::Node* curr = interests.getHead(); curr != nullptr; curr = curr->next) {

            string a = toLowerCase(curr->data);

            if (a == b)
                return true;

            if (a.length() >= 3 && startsWith(b, a))
                return true;

            if (b.length() >= 3 && startsWith(a, b))
                return true;
        }

        return false;
    }

    // ===== LIKES (Stack) =====

    // Walks the stack top -> bottom. O(k), k = posts this user liked
    bool hasLiked(int postId) const {

        for (Stack<int>::Node* curr = likeHistory.getTop(); curr != nullptr; curr = curr->next)
            if (curr->data == postId)
                return true;

        return false;
    }

    void recordLike(int postId) {
        likeHistory.push(postId);
    }

    // Pops the most recent like (Undo). Throws underflow_error if there is none.
    int undoLastLike() {
        return likeHistory.pop();
    }

    const Stack<int>& getLikeHistory() const { return likeHistory; }

    // ===== NOTIFICATIONS (Circular Queue) =====

    // When the queue is full, the oldest notification is dropped first
    void notify(const string& message) {

        if (notifications.isFull())
            notifications.dequeue();

        notifications.enqueue(message);
    }

    const Queue<string>& getNotifications() const { return notifications; }

    void clearNotifications() {
        notifications.clear();
    }
};

#endif
