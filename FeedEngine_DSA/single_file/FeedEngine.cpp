// FeedEngine - DSA Version (CT-159 CCA), all in one file.
//
// The same code as the FeedEngine_DSA folder, joined in order so it can be
// opened and run on its own (Dev-C++: Execute -> Compile & Run).
//
// Terminal:  g++ FeedEngine.cpp -o FeedEngine
//            FeedEngine
//
// Data is saved to posts.txt and users.txt next to the program.

// ==================================================================
// Functions.h
// ==================================================================
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

inline void printBox(string title) {

    cout << CYAN
         << "\n====================================\n";

    cout << "   " << title << endl;

    cout << "====================================\n"
         << RESET;
}

inline void printError(string msg) {
    cout << RED << msg << RESET << endl;
}

inline void printSuccess(string msg) {
    cout << GREEN << msg << RESET << endl;
}

// ===== TEXT HELPERS =====

// "TeCh" -> "tech"
inline string toLowerCase(string text) {

    for (int i = 0; i < static_cast<int>(text.length()); i++)
        if (text[i] >= 'A' && text[i] <= 'Z')
            text[i] = static_cast<char>(text[i] - 'A' + 'a');

    return text;
}

inline bool startsWith(const string& text, const string& start) {
    return text.length() >= start.length() && text.compare(0, start.length(), start) == 0;
}

// Does text contain word anywhere? Ignores capital letters.
inline bool containsText(const string& text, const string& word) {
    return toLowerCase(text).find(toLowerCase(word)) != string::npos;
}

// Cuts long text so table columns stay straight: "Beauty Skincare Ro..."
inline string shorten(const string& text, int maxLength) {

    if (static_cast<int>(text.length()) <= maxLength)
        return text;

    return text.substr(0, maxLength - 3) + "...";
}

#endif

// ==================================================================
// Content.h
// ==================================================================
#ifndef CONTENT_H
#define CONTENT_H

// The base class from our OOP project: "anything that can be shown on screen".
// display() has no body here (= 0), so every child class, like Post,
// must write its own. This is abstraction + inheritance from last semester.
class Content {

public:
    virtual void display() const = 0;

    virtual ~Content() {}
};

#endif

// ==================================================================
// DataStructures/HigherFirst.h
// ==================================================================
#ifndef HIGHERFIRST_H
#define HIGHERFIRST_H

// The default "rule" for PriorityQueue and MaxHeap: the bigger value goes first.
//
// A rule is just a tiny struct with an operator(), so we can call it like
// a function: HigherFirst<int> rule; then rule(9, 4) is true (9 goes first).
// FeedEngine has its own rule, MoreLikes, which puts the most-liked post first.

template <class T>
struct HigherFirst {

    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

#endif

// ==================================================================
// DataStructures/LinkedList.h
// ==================================================================
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

using namespace std;

// ===== DOUBLY LINKED LIST (Weeks 2-3) =====
//
// Think of a chain of people holding hands: each person holds the hand of
// the person before them (prev) and the person after them (next).
// We can start at the first person (head) or the last person (tail) and walk.
//
// Used in FeedEngine for: the feed (Next / Previous), each user's interests,
// and inside other structures (the priority queue and the graph).
//
// How fast:
//   add or remove at the front, at the back, or next to a node we hold   O(1)
//   search for a value (we have to walk along the chain)                  O(n)
//
// "template <class T>" means T can be any type (int, string, Post*, ...).
// We write the list once and use it for everything.

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

    // ===== ACCESS =====

    // Start a traversal: for (Node* curr = list.getHead(); curr != nullptr; curr = curr->next)
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

// ==================================================================
// DataStructures/Stack.h
// ==================================================================
#ifndef STACK_H
#define STACK_H

#include <stdexcept>

using namespace std;

// ===== STACK (Week 4) =====
//
// Think of a pile of plates: you can only put a plate on top (push)
// and take the top plate off (pop). The last plate in is the first one out
// (LIFO: last in, first out).
//
// We build it from linked nodes: "top" points to the plate on top,
// and every plate points to the plate under it.
//
// Used in FeedEngine for: Like (push), Undo Like (pop), Recently Liked
// (walk from the top down), and to flip the likes around when saving.
//
// How fast: push, pop and peek are all O(1).
// The pile can always grow, so it never overflows.
// Pop or peek on an empty pile is called "underflow" and throws an error.

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

// ==================================================================
// DataStructures/Queue.h
// ==================================================================
#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

using namespace std;

// ===== CIRCULAR QUEUE (Week 5) =====
//
// Think of a line at the bank: people join at the back (enqueue) and
// leave from the front (dequeue). First in, first out (FIFO).
//
// The line lives in an array. When "rear" reaches the end of the array,
// rear = (rear + 1) % capacity sends it back to slot 0, so the empty
// slots at the front are used again. That is what "circular" means.
//
// Used in FeedEngine for: notifications (10 slots per user),
// and inside the graph's BFS search.
//
// How fast: enqueue, dequeue and peek are all O(1).
// Joining a full line is "overflow"; leaving an empty line is "underflow".

template <class T>
class Queue {

private:

    T* arr;
    int capacity;
    int front;
    int rear;
    int count;

public:

    explicit Queue(int maxSize = 10) : arr(nullptr), capacity(maxSize > 0 ? maxSize : 1), front(0), rear(-1), count(0) {
        arr = new T[capacity];
    }

    Queue(const Queue& other) : arr(nullptr), capacity(other.capacity), front(other.front), rear(other.rear), count(other.count) {

        arr = new T[capacity];

        for (int i = 0; i < count; i++) {
            int index = (front + i) % capacity;
            arr[index] = other.arr[index];
        }
    }

    Queue& operator=(const Queue& other) {

        if (this != &other) {

            T* fresh = new T[other.capacity];

            for (int i = 0; i < other.count; i++) {
                int index = (other.front + i) % other.capacity;
                fresh[index] = other.arr[index];
            }

            delete[] arr;

            arr = fresh;
            capacity = other.capacity;
            front = other.front;
            rear = other.rear;
            count = other.count;
        }

        return *this;
    }

    ~Queue() {
        delete[] arr;
    }

    void enqueue(const T& value) {

        if (isFull())
            throw overflow_error("Queue overflow: enqueue() on full queue");

        rear = (rear + 1) % capacity;
        arr[rear] = value;
        count++;
    }

    T dequeue() {

        if (isEmpty())
            throw underflow_error("Queue underflow: dequeue() on empty queue");

        T value = arr[front];
        front = (front + 1) % capacity;
        count--;

        return value;
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Queue underflow: peek() on empty queue");

        return arr[front];
    }

    // i-th item counted from the front (0 = oldest), to show the
    // whole queue without removing anything
    T at(int i) const {

        if (i < 0 || i >= count)
            throw out_of_range("Queue index out of range");

        return arr[(front + i) % capacity];
    }

    bool isEmpty() const { return count == 0; }

    bool isFull() const { return count == capacity; }

    int size() const { return count; }

    int getCapacity() const { return capacity; }

    void clear() {
        front = 0;
        rear = -1;
        count = 0;
    }
};

#endif

// ==================================================================
// DataStructures/PriorityQueue.h
// ==================================================================
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdexcept>


using namespace std;

// ===== PRIORITY QUEUE ON A SORTED LIST (Week 5) =====
//
// Think of a hospital waiting room: the most urgent patient goes first,
// not the one who came first. Here "urgent" means "highest priority".
//
// We keep a linked list that is always in order (highest first), so the
// next one out is simply the head. A new item walks down the list until it
// finds its place, like "insert a value into a sorted list" from Week 6.
//
// Used in FeedEngine for: Recommendations and "People you may know".
// (Trending uses MaxHeap.h, the faster heap version of a priority queue.)
//
// How fast: enqueue O(n) (walk to the right place), dequeue and peek O(1).
// Items with the same priority keep the order they arrived in.

// Compare is a small "rule": higherPriority(a, b) is true when a should
// go before b. The default rule, HigherFirst, puts bigger values first.
template <class T, class Compare = HigherFirst<T> >
class PriorityQueue {

private:

    DoublyLinkedList<T> items;
    Compare higherPriority;

public:

    PriorityQueue() : items(), higherPriority() {}

    explicit PriorityQueue(Compare compare) : items(), higherPriority(compare) {}

    void enqueue(const T& value) {

        typename DoublyLinkedList<T>::Node* curr = items.getHead();

        // Skip every item that should still leave before the new one
        // (equal priority stays in front, which keeps FIFO order)
        while (curr != nullptr && !higherPriority(value, curr->data))
            curr = curr->next;

        if (curr == nullptr)
            items.insertAtTail(value);
        else
            items.insertBefore(curr, value);
    }

    T dequeue() {

        if (isEmpty())
            throw underflow_error("Priority queue underflow: dequeue() on empty queue");

        return items.deleteFromHead();
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Priority queue underflow: peek() on empty queue");

        return items.getHead()->data;
    }

    // Takes one item out wherever it is. O(n)
    bool remove(const T& value) {
        return items.deleteValue(value);
    }

    // Walk from highest to lowest priority without removing anything
    // (e.g. show the top 5 trending posts)
    typename DoublyLinkedList<T>::Node* getHead() const { return items.getHead(); }

    bool isEmpty() const { return items.isEmpty(); }

    int size() const { return items.size(); }

    void clear() { items.clear(); }
};

#endif

// ==================================================================
// DataStructures/MaxHeap.h
// ==================================================================
#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <stdexcept>


using namespace std;

// ===== MAX-HEAP (Week 10) =====
//
// Think of a league table: the champion is always at the top.
// A heap is a tree where every parent is at least as big as its children,
// so the biggest item is always at the root (index 0).
//
// The tree is stored in a plain array, with no pointers at all:
//   the parent of index i is (i - 1) / 2
//   its two children are 2i + 1 and 2i + 2
//
// Used in FeedEngine for: the Top 5 trending posts.
//
// How fast:
//   insert       O(log n)  add at the end, then swap upwards (heapify up)
//   extractMax   O(log n)  take the root, move the last item up, swap down
//   peek         O(1)      the root is always the biggest
//   buildFrom    O(n)      build a whole heap at once (Floyd's method)
//
// higherPriority(a, b) is the rule: true when a should come out before b.

template <class T, class Compare = HigherFirst<T> >
class MaxHeap {

private:

    T* arr;
    int capacity;
    int count;
    Compare higherPriority;

    static int parentOf(int i) { return (i - 1) / 2; }

    static int leftChild(int i) { return 2 * i + 1; }

    static int rightChild(int i) { return 2 * i + 2; }

    // Doubles the array when it is full
    void grow() {

        int newCapacity = capacity * 2;
        T* bigger = new T[newCapacity];

        for (int i = 0; i < count; i++)
            bigger[i] = arr[i];

        delete[] arr;
        arr = bigger;
        capacity = newCapacity;
    }

    void swapItems(int a, int b) {
        T temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }

    // Move a new item up while it beats its parent
    void heapifyUp(int i) {

        while (i > 0 && higherPriority(arr[i], arr[parentOf(i)])) {
            swapItems(i, parentOf(i));
            i = parentOf(i);
        }
    }

    // Move an item down while one of its children beats it
    void heapifyDown(int i) {

        while (true) {

            int best = i;
            int left = leftChild(i);
            int right = rightChild(i);

            if (left < count && higherPriority(arr[left], arr[best]))
                best = left;

            if (right < count && higherPriority(arr[right], arr[best]))
                best = right;

            if (best == i)
                return;

            swapItems(i, best);
            i = best;
        }
    }

public:

    explicit MaxHeap(int initialCapacity = 16) : arr(nullptr), capacity(initialCapacity > 0 ? initialCapacity : 1), count(0), higherPriority() {
        arr = new T[capacity];
    }

    MaxHeap(const MaxHeap& other) : arr(nullptr), capacity(other.capacity), count(other.count), higherPriority(other.higherPriority) {

        arr = new T[capacity];

        for (int i = 0; i < count; i++)
            arr[i] = other.arr[i];
    }

    MaxHeap& operator=(const MaxHeap& other) {

        if (this != &other) {

            T* fresh = new T[other.capacity];

            for (int i = 0; i < other.count; i++)
                fresh[i] = other.arr[i];

            delete[] arr;

            arr = fresh;
            capacity = other.capacity;
            count = other.count;
            higherPriority = other.higherPriority;
        }

        return *this;
    }

    ~MaxHeap() {
        delete[] arr;
    }

    void insert(const T& value) {

        if (count == capacity)
            grow();

        arr[count] = value;
        count++;
        heapifyUp(count - 1);
    }

    // Removes and returns the root, then moves the last item to the root
    // and heapifies it down
    T extractMax() {

        if (isEmpty())
            throw underflow_error("Heap underflow: extractMax() on empty heap");

        T top = arr[0];
        count--;

        if (count > 0) {
            arr[0] = arr[count];
            heapifyDown(0);
        }

        return top;
    }

    T peek() const {

        if (isEmpty())
            throw underflow_error("Heap underflow: peek() on empty heap");

        return arr[0];
    }

    // Floyd's build-heap: copy the items, then heapify down every
    // parent from the last one up to the root. O(n), faster than n inserts.
    void buildFrom(const T* values, int n) {

        count = 0;

        while (capacity < n)
            grow();

        for (int i = 0; i < n; i++)
            arr[i] = values[i];

        count = n;

        for (int i = n / 2 - 1; i >= 0; i--)
            heapifyDown(i);
    }

    // Item at array index i (index 0 is the root), to show the heap as a tree
    T at(int i) const {

        if (i < 0 || i >= count)
            throw out_of_range("Heap index out of range");

        return arr[i];
    }

    bool isEmpty() const { return count == 0; }

    int size() const { return count; }

    void clear() { count = 0; }
};

#endif

// ==================================================================
// DataStructures/HashTable.h
// ==================================================================
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>


using namespace std;

// ===== HASH FUNCTIONS (Week 15) =====
// A hash function turns a key into a number. The table then picks a
// bucket with: number % bucketCount (the "division method").

// Post IDs are already numbers, so the number is just the ID itself
inline unsigned int hashFunction(int key) {
    return static_cast<unsigned int>(key);
}

// Usernames: start at 5381, then for every letter do hash = hash * 33 + letter
// (the well-known "djb2" hash). The order of letters matters, so even
// "ab" and "ba" get different numbers.
inline unsigned int hashFunction(const string& key) {

    unsigned int hash = 5381;

    for (int i = 0; i < static_cast<int>(key.length()); i++)
        hash = hash * 33 + static_cast<unsigned char>(key[i]);

    return hash;
}

// ===== HASH TABLE (Week 15) =====
//
// Think of a phone book with tabs A to Z: instead of reading every page,
// you jump straight to the right tab. The hash function tells us which
// "tab" (bucket) a key belongs in.
//
// Two keys can land in the same bucket: that is a "collision". We handle
// it with chaining: each bucket holds a small linked chain of entries.
//
// If the table gets more than 75% full (the "load factor"), the chains
// would get long, so we make the table about twice as big and put every
// entry into its new bucket. That is "rehashing".
//
// Used in FeedEngine for: finding a post by its ID and a user by username.
//
// How fast: insert, find and remove are O(1) on average
// (only O(n) in the very unlucky case where every key collides).

template <class K, class V>
class HashTable {

public:

    struct Entry {
        K key;
        V value;
        Entry* next;

        Entry(const K& k, const V& v, Entry* n) : key(k), value(v), next(n) {}
    };

private:

    Entry** buckets;
    int bucketCount;
    int count;

    int indexFor(const K& key) const {
        return static_cast<int>(hashFunction(key) % static_cast<unsigned int>(bucketCount));
    }

    Entry* findEntry(const K& key) const {

        for (Entry* curr = buckets[indexFor(key)]; curr != nullptr; curr = curr->next)
            if (curr->key == key)
                return curr;

        return nullptr;
    }

    void makeBuckets(int n) {

        bucketCount = n;
        buckets = new Entry*[bucketCount];

        for (int i = 0; i < bucketCount; i++)
            buckets[i] = nullptr;
    }

    // Moves every entry into a bigger bucket array (no entries are copied)
    void rehash() {

        Entry** oldBuckets = buckets;
        int oldCount = bucketCount;

        makeBuckets(oldCount * 2 + 1);

        for (int i = 0; i < oldCount; i++) {

            Entry* curr = oldBuckets[i];

            while (curr != nullptr) {
                Entry* next = curr->next;
                int index = indexFor(curr->key);

                curr->next = buckets[index];
                buckets[index] = curr;

                curr = next;
            }
        }

        delete[] oldBuckets;
    }

    // Adds a key that is not in the table yet, at the front of its chain
    Entry* addEntry(const K& key, const V& value) {

        if ((count + 1) * 4 > bucketCount * 3)     // load factor would pass 0.75
            rehash();

        int index = indexFor(key);
        buckets[index] = new Entry(key, value, buckets[index]);
        count++;

        return buckets[index];
    }

    void copyFrom(const HashTable& other) {

        makeBuckets(other.bucketCount);

        for (int i = 0; i < bucketCount; i++)
            for (Entry* curr = other.buckets[i]; curr != nullptr; curr = curr->next)
                buckets[i] = new Entry(curr->key, curr->value, buckets[i]);

        count = other.count;
    }

public:

    explicit HashTable(int initialBuckets = 17) : buckets(nullptr), bucketCount(0), count(0) {
        makeBuckets(initialBuckets > 0 ? initialBuckets : 1);
    }

    HashTable(const HashTable& other) : buckets(nullptr), bucketCount(0), count(0) {
        copyFrom(other);
    }

    HashTable& operator=(const HashTable& other) {

        if (this != &other) {
            clear();
            delete[] buckets;
            copyFrom(other);
        }

        return *this;
    }

    ~HashTable() {
        clear();
        delete[] buckets;
    }

    // Adds a new key, or updates the value if the key is already there
    void insert(const K& key, const V& value) {

        Entry* existing = findEntry(key);

        if (existing != nullptr)
            existing->value = value;
        else
            addEntry(key, value);
    }

    // Pointer to the stored value, or nullptr if the key is missing
    V* find(const K& key) {
        Entry* entry = findEntry(key);
        return entry != nullptr ? &entry->value : nullptr;
    }

    const V* find(const K& key) const {
        Entry* entry = findEntry(key);
        return entry != nullptr ? &entry->value : nullptr;
    }

    // Copy of the value, or fallback if the key is missing
    V get(const K& key, const V& fallback) const {
        Entry* entry = findEntry(key);
        return entry != nullptr ? entry->value : fallback;
    }

    // Value for the key; adds the key with V() first if it is missing
    V& operator[](const K& key) {

        Entry* entry = findEntry(key);

        if (entry == nullptr)
            entry = addEntry(key, V());

        return entry->value;
    }

    bool contains(const K& key) const {
        return findEntry(key) != nullptr;
    }

    bool remove(const K& key) {

        int index = indexFor(key);
        Entry* prev = nullptr;
        Entry* curr = buckets[index];

        while (curr != nullptr) {

            if (curr->key == key) {

                if (prev == nullptr)
                    buckets[index] = curr->next;
                else
                    prev->next = curr->next;

                delete curr;
                count--;
                return true;
            }

            prev = curr;
            curr = curr->next;
        }

        return false;
    }

    // How many entries a lookup compares before it finds the key
    // (or the whole chain if the key is missing). Shows collision cost.
    int stepsToFind(const K& key) const {

        int steps = 0;

        for (Entry* curr = buckets[indexFor(key)]; curr != nullptr; curr = curr->next) {
            steps++;

            if (curr->key == key)
                break;
        }

        return steps;
    }

    // Every key, bucket by bucket
    DoublyLinkedList<K> keys() const {

        DoublyLinkedList<K> result;

        for (int i = 0; i < bucketCount; i++)
            for (Entry* curr = buckets[i]; curr != nullptr; curr = curr->next)
                result.insertAtTail(curr->key);

        return result;
    }

    // Number of entries in bucket i (more than 1 = collisions)
    int bucketLength(int i) const {

        int length = 0;

        if (i >= 0 && i < bucketCount)
            for (Entry* curr = buckets[i]; curr != nullptr; curr = curr->next)
                length++;

        return length;
    }

    int getBucketCount() const { return bucketCount; }

    int size() const { return count; }

    bool isEmpty() const { return count == 0; }

    void clear() {

        for (int i = 0; i < bucketCount; i++) {

            Entry* curr = buckets[i];

            while (curr != nullptr) {
                Entry* next = curr->next;
                delete curr;
                curr = next;
            }

            buckets[i] = nullptr;
        }

        count = 0;
    }
};

#endif

// ==================================================================
// DataStructures/Graph.h
// ==================================================================
#ifndef GRAPH_H
#define GRAPH_H


using namespace std;

// ===== GRAPH (Week 13) =====
//
// Think of a map of who follows whom: every user is a dot (a vertex) and
// every follow is an arrow (an edge). "ali -> sara" means ali follows sara.
//
// We store it as an adjacency list: every user has a linked list of the
// users they follow. A hash table turns a username into its number
// (its index), so we can find that user's list quickly.
//
// Used in FeedEngine for: follow / unfollow, followers, "People you may
// know" and "How are you connected?".
//
// The two searches:
//   BFS (breadth-first search) checks people level by level using a queue:
//   first everyone 1 step away, then 2 steps away, and so on. Every follow
//   counts as 1 step, so BFS also finds the shortest path.
//   DFS (depth-first search) goes as deep as it can first, using a stack.
//
// How fast: BFS and DFS look at every user and every follow once: O(V + E).
// Adding or checking one follow walks one user's list: O(their follows).

template <class T>
class Graph {

private:

    bool directed;
    T* vertices;                        // index -> vertex name
    DoublyLinkedList<int>* adjacency;   // index -> indexes it points to
    int vertexCount;
    int capacity;
    HashTable<T, int> indexOf;          // vertex name -> index

    // Doubles both arrays when they are full
    void grow() {

        int newCapacity = capacity * 2;
        T* newVertices = new T[newCapacity];
        DoublyLinkedList<int>* newAdjacency = new DoublyLinkedList<int>[newCapacity];

        for (int i = 0; i < vertexCount; i++) {
            newVertices[i] = vertices[i];
            newAdjacency[i] = adjacency[i];     // deep copy of the list
        }

        delete[] vertices;
        delete[] adjacency;

        vertices = newVertices;
        adjacency = newAdjacency;
        capacity = newCapacity;
    }

    // Index of a vertex, or -1 if it is not in the graph
    int indexFor(const T& vertex) const {
        return indexOf.get(vertex, -1);
    }

    // Breadth-first search from start, using a queue.
    // Fills distance[] (edges from start, -1 = cannot reach), parent[]
    // (the vertex we came from) and order[] (visit order).
    // Returns how many vertices were reached.
    int bfs(int start, int* distance, int* parent, int* order) const {

        for (int i = 0; i < vertexCount; i++) {
            distance[i] = -1;
            parent[i] = -1;
        }

        Queue<int> queue(vertexCount);     // every vertex is enqueued at most once
        int visited = 0;

        distance[start] = 0;
        queue.enqueue(start);

        while (!queue.isEmpty()) {

            int u = queue.dequeue();
            order[visited++] = u;

            for (DoublyLinkedList<int>::Node* curr = adjacency[u].getHead(); curr != nullptr; curr = curr->next) {

                int w = curr->data;

                if (distance[w] == -1) {
                    distance[w] = distance[u] + 1;
                    parent[w] = u;
                    queue.enqueue(w);
                }
            }
        }

        return visited;
    }

public:

    explicit Graph(bool isDirected = true, int initialCapacity = 8) : directed(isDirected), vertices(nullptr), adjacency(nullptr), vertexCount(0), capacity(initialCapacity > 0 ? initialCapacity : 1), indexOf() {
        vertices = new T[capacity];
        adjacency = new DoublyLinkedList<int>[capacity];
    }

    // FeedEngine has exactly one graph, so copying is not needed
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    ~Graph() {
        delete[] vertices;
        delete[] adjacency;
    }

    // ===== VERTICES AND EDGES =====

    bool addVertex(const T& vertex) {

        if (hasVertex(vertex))
            return false;

        if (vertexCount == capacity)
            grow();

        vertices[vertexCount] = vertex;
        indexOf.insert(vertex, vertexCount);
        vertexCount++;

        return true;
    }

    bool hasVertex(const T& vertex) const {
        return indexOf.contains(vertex);
    }

    // Removes every vertex and edge
    void clear() {

        for (int i = 0; i < vertexCount; i++)
            adjacency[i].clear();

        vertexCount = 0;
        indexOf.clear();
    }

    // false if a vertex is missing, the edge already exists, or from == to
    bool addEdge(const T& from, const T& to) {

        int a = indexFor(from);
        int b = indexFor(to);

        if (a == -1 || b == -1 || a == b || adjacency[a].search(b) != nullptr)
            return false;

        adjacency[a].insertAtTail(b);

        if (!directed)
            adjacency[b].insertAtTail(a);

        return true;
    }

    bool removeEdge(const T& from, const T& to) {

        int a = indexFor(from);
        int b = indexFor(to);

        if (a == -1 || b == -1)
            return false;

        bool removed = adjacency[a].deleteValue(b);

        if (removed && !directed)
            adjacency[b].deleteValue(a);

        return removed;
    }

    bool hasEdge(const T& from, const T& to) const {

        int a = indexFor(from);
        int b = indexFor(to);

        return a != -1 && b != -1 && adjacency[a].search(b) != nullptr;
    }

    // Vertices this one points to (who ali follows)
    DoublyLinkedList<T> neighbours(const T& vertex) const {

        DoublyLinkedList<T> result;
        int a = indexFor(vertex);

        if (a != -1)
            for (DoublyLinkedList<int>::Node* curr = adjacency[a].getHead(); curr != nullptr; curr = curr->next)
                result.insertAtTail(vertices[curr->data]);

        return result;
    }

    // Vertices that point to this one (ali's followers). O(V + E)
    DoublyLinkedList<T> incoming(const T& vertex) const {

        DoublyLinkedList<T> result;
        int b = indexFor(vertex);

        if (b != -1)
            for (int i = 0; i < vertexCount; i++)
                if (adjacency[i].search(b) != nullptr)
                    result.insertAtTail(vertices[i]);

        return result;
    }

    // ===== TRAVERSALS =====

    // Breadth-first: nearest vertices first, using a queue
    DoublyLinkedList<T> breadthFirst(const T& start) const {

        DoublyLinkedList<T> result;
        int s = indexFor(start);

        if (s == -1)
            return result;

        int* distance = new int[vertexCount];
        int* parent = new int[vertexCount];
        int* order = new int[vertexCount];

        int visited = bfs(s, distance, parent, order);

        for (int i = 0; i < visited; i++)
            result.insertAtTail(vertices[order[i]]);

        delete[] distance;
        delete[] parent;
        delete[] order;

        return result;
    }

    // Depth-first: go as deep as possible before backtracking, using a stack
    DoublyLinkedList<T> depthFirst(const T& start) const {

        DoublyLinkedList<T> result;
        int s = indexFor(start);

        if (s == -1)
            return result;

        bool* visited = new bool[vertexCount];

        for (int i = 0; i < vertexCount; i++)
            visited[i] = false;

        Stack<int> stack;
        stack.push(s);

        while (!stack.isEmpty()) {

            int u = stack.pop();

            if (visited[u])
                continue;

            visited[u] = true;
            result.insertAtTail(vertices[u]);

            // push neighbours last-to-first, so the first neighbour is visited first
            for (DoublyLinkedList<int>::Node* curr = adjacency[u].getTail(); curr != nullptr; curr = curr->prev)
                if (!visited[curr->data])
                    stack.push(curr->data);
        }

        delete[] visited;

        return result;
    }

    // ===== SHORTEST PATH (BFS) =====

    // The fewest edges from -> to, e.g. ali -> sara -> omar.
    // Empty if there is no path.
    DoublyLinkedList<T> shortestPath(const T& from, const T& to) const {

        DoublyLinkedList<T> path;
        int a = indexFor(from);
        int b = indexFor(to);

        if (a == -1 || b == -1)
            return path;

        int* distance = new int[vertexCount];
        int* parent = new int[vertexCount];
        int* order = new int[vertexCount];

        bfs(a, distance, parent, order);

        // walk back from the end using parent[], adding each vertex at the front
        if (distance[b] != -1)
            for (int v = b; v != -1; v = parent[v])
                path.insertAtHead(vertices[v]);

        delete[] distance;
        delete[] parent;
        delete[] order;

        return path;
    }

    // Number of edges on the shortest path, or -1 if there is none
    int distance(const T& from, const T& to) const {
        return shortestPath(from, to).size() - 1;
    }

    // Vertices exactly k edges away, in BFS order
    // (k = 2 gives "friends of friends")
    DoublyLinkedList<T> verticesAtDistance(const T& start, int k) const {

        DoublyLinkedList<T> result;
        int s = indexFor(start);

        if (s == -1)
            return result;

        int* distance = new int[vertexCount];
        int* parent = new int[vertexCount];
        int* order = new int[vertexCount];

        int visited = bfs(s, distance, parent, order);

        for (int i = 0; i < visited; i++)
            if (distance[order[i]] == k)
                result.insertAtTail(vertices[order[i]]);

        delete[] distance;
        delete[] parent;
        delete[] order;

        return result;
    }

    // ===== INFO =====

    int getVertexCount() const { return vertexCount; }

    int getEdgeCount() const {

        int total = 0;

        for (int i = 0; i < vertexCount; i++)
            total += adjacency[i].size();

        return directed ? total : total / 2;
    }

    bool isDirected() const { return directed; }
};

#endif

// ==================================================================
// DataStructures/AVLTree.h
// ==================================================================
#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdexcept>


using namespace std;

// ===== BINARY SEARCH TREE WITH AVL BALANCING (Weeks 9, 11, 12) =====
//
// A binary search tree (BST) follows one simple rule:
//   smaller keys go to the LEFT, bigger keys go to the RIGHT.
// So reading the tree left, then node, then right (in-order) gives A to Z.
//
// The problem: if the keys arrive already in A-Z order, every new key goes
// to the right, and the "tree" becomes one long chain, as slow as a list.
//
// The AVL fix: after every insert or remove, each node on the way back up
// checks "is one side more than 1 level taller than the other?". If yes,
// it turns (rotates) that part of the tree to make it short again.
// There are 4 cases: left-left, right-right, left-right and right-left.
//
// Used in FeedEngine for: the A-Z title index (lowercase title -> its posts).
//
// How fast:
//   insert / find / remove   O(log n)   (a plain BST can be O(n))
//   walking the whole tree   O(n)
//   titles starting with...  O(log n + k), k = how many match
//
// Pass false to the constructor to turn balancing off and get a plain BST
// (we use this to show how tall a BST grows without balancing).

template <class K, class V>
class AVLTree {

public:

    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;         // a leaf has height 1

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

private:

    Node* root;
    int count;
    bool selfBalancing;
    int rotations;

    static int heightOf(const Node* node) {
        return node != nullptr ? node->height : 0;
    }

    // left height - right height: +2 means too tall on the left, -2 on the right
    static int balanceOf(const Node* node) {
        return node != nullptr ? heightOf(node->left) - heightOf(node->right) : 0;
    }

    static void updateHeight(Node* node) {

        int left = heightOf(node->left);
        int right = heightOf(node->right);

        node->height = 1 + (left > right ? left : right);
    }

    // Smallest key of a subtree: keep going left
    static Node* minNode(Node* node) {

        while (node->left != nullptr)
            node = node->left;

        return node;
    }

    // ===== ROTATIONS =====

    // Left-left case: y's left child x moves up and becomes the subtree root,
    // y becomes x's right child, and x's old right subtree becomes y's left
    Node* rotateRight(Node* y) {

        Node* x = y->left;

        y->left = x->right;
        x->right = y;

        updateHeight(y);
        updateHeight(x);
        rotations++;

        return x;
    }

    // Right-right case: the mirror image of rotateRight
    Node* rotateLeft(Node* x) {

        Node* y = x->right;

        x->right = y->left;
        y->left = x;

        updateHeight(x);
        updateHeight(y);
        rotations++;

        return y;
    }

    // Called on every node on the way back up after an insert or remove.
    // Returns the root of this subtree after any rotation.
    Node* rebalance(Node* node) {

        updateHeight(node);

        if (!selfBalancing)
            return node;

        int balance = balanceOf(node);

        if (balance > 1) {                              // too tall on the left
            if (balanceOf(node->left) < 0)              // left-right case: turn it into left-left
                node->left = rotateLeft(node->left);

            return rotateRight(node);                   // left-left case
        }

        if (balance < -1) {                             // too tall on the right
            if (balanceOf(node->right) > 0)             // right-left case: turn it into right-right
                node->right = rotateRight(node->right);

            return rotateLeft(node);                    // right-right case
        }

        return node;
    }

    // ===== INSERT AND REMOVE (recursive) =====

    // found ends up pointing at the node that holds key (new or old)
    Node* insertAt(Node* node, const K& key, const V& value, Node*& found) {

        if (node == nullptr) {
            found = new Node(key, value);
            count++;
            return found;
        }

        if (key < node->key) {
            node->left = insertAt(node->left, key, value, found);
        }
        else if (node->key < key) {
            node->right = insertAt(node->right, key, value, found);
        }
        else {
            found = node;       // key is already in the tree
            return node;
        }

        return rebalance(node);
    }

    Node* removeAt(Node* node, const K& key, bool& removed) {

        if (node == nullptr)        // key is not in the tree
            return nullptr;

        if (key < node->key) {
            node->left = removeAt(node->left, key, removed);
        }
        else if (node->key < key) {
            node->right = removeAt(node->right, key, removed);
        }
        else {
            removed = true;

            // no child or one child: the child (or nullptr) takes its place
            if (node->left == nullptr || node->right == nullptr) {

                Node* child = node->left != nullptr ? node->left : node->right;

                delete node;
                count--;

                return child;
            }

            // two children: copy the in-order successor (smallest key on the
            // right) into this node, then remove the successor from the right
            Node* successor = minNode(node->right);

            node->key = successor->key;
            node->value = successor->value;
            node->right = removeAt(node->right, node->key, removed);
        }

        return rebalance(node);
    }

    Node* findNode(const K& key) const {

        Node* curr = root;

        while (curr != nullptr) {

            if (key < curr->key)
                curr = curr->left;
            else if (curr->key < key)
                curr = curr->right;
            else
                return curr;
        }

        return nullptr;
    }

    // ===== TRAVERSALS (recursive) =====

    void inOrderAt(const Node* node, DoublyLinkedList<K>& out) const {

        if (node == nullptr)
            return;

        inOrderAt(node->left, out);
        out.insertAtTail(node->key);
        inOrderAt(node->right, out);
    }

    void preOrderAt(const Node* node, DoublyLinkedList<K>& out) const {

        if (node == nullptr)
            return;

        out.insertAtTail(node->key);
        preOrderAt(node->left, out);
        preOrderAt(node->right, out);
    }

    void postOrderAt(const Node* node, DoublyLinkedList<K>& out) const {

        if (node == nullptr)
            return;

        postOrderAt(node->left, out);
        postOrderAt(node->right, out);
        out.insertAtTail(node->key);
    }

    // Keys that start with prefix sit next to each other in sorted order,
    // so whole subtrees that cannot hold them are skipped
    void prefixAt(const Node* node, const K& prefix, DoublyLinkedList<K>& out) const {

        if (node == nullptr)
            return;

        if (node->key.compare(0, prefix.length(), prefix) == 0) {
            prefixAt(node->left, prefix, out);
            out.insertAtTail(node->key);
            prefixAt(node->right, prefix, out);
        }
        else if (node->key < prefix) {
            prefixAt(node->right, prefix, out);     // the whole left side is even smaller
        }
        else {
            prefixAt(node->left, prefix, out);      // the whole right side is even bigger
        }
    }

    // Stored heights are right and, with balancing on, every node keeps the AVL rule
    bool heightsValid(const Node* node) const {

        if (node == nullptr)
            return true;

        if (!heightsValid(node->left) || !heightsValid(node->right))
            return false;

        int left = heightOf(node->left);
        int right = heightOf(node->right);

        if (node->height != 1 + (left > right ? left : right))
            return false;

        return !selfBalancing || (left - right <= 1 && right - left <= 1);
    }

    static Node* copyOf(const Node* node) {

        if (node == nullptr)
            return nullptr;

        Node* copy = new Node(node->key, node->value);

        copy->height = node->height;
        copy->left = copyOf(node->left);
        copy->right = copyOf(node->right);

        return copy;
    }

    // Post-order delete: children first, then the node
    static void destroy(Node* node) {

        if (node == nullptr)
            return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:

    explicit AVLTree(bool balancing = true) : root(nullptr), count(0), selfBalancing(balancing), rotations(0) {}

    AVLTree(const AVLTree& other) : root(copyOf(other.root)), count(other.count), selfBalancing(other.selfBalancing), rotations(other.rotations) {}

    AVLTree& operator=(const AVLTree& other) {

        if (this != &other) {
            clear();
            root = copyOf(other.root);
            count = other.count;
            selfBalancing = other.selfBalancing;
            rotations = other.rotations;
        }

        return *this;
    }

    ~AVLTree() {
        destroy(root);
    }

    // Adds a new key, or updates the value if the key is already there
    void insert(const K& key, const V& value) {

        Node* found = nullptr;
        root = insertAt(root, key, value, found);
        found->value = value;
    }

    // Value for the key; adds the key with V() first if it is missing
    V& operator[](const K& key) {

        Node* found = nullptr;
        root = insertAt(root, key, V(), found);
        return found->value;
    }

    bool remove(const K& key) {

        bool removed = false;
        root = removeAt(root, key, removed);
        return removed;
    }

    // Pointer to the stored value, or nullptr if the key is missing
    V* find(const K& key) {
        Node* node = findNode(key);
        return node != nullptr ? &node->value : nullptr;
    }

    const V* find(const K& key) const {
        Node* node = findNode(key);
        return node != nullptr ? &node->value : nullptr;
    }

    V get(const K& key, const V& fallback) const {
        Node* node = findNode(key);
        return node != nullptr ? node->value : fallback;
    }

    bool contains(const K& key) const {
        return findNode(key) != nullptr;
    }

    // Nodes visited to find the key (or to learn it is missing)
    int searchSteps(const K& key) const {

        int steps = 0;
        Node* curr = root;

        while (curr != nullptr) {
            steps++;

            if (key < curr->key)
                curr = curr->left;
            else if (curr->key < key)
                curr = curr->right;
            else
                break;
        }

        return steps;
    }

    K minKey() const {

        if (root == nullptr)
            throw underflow_error("Tree underflow: minKey() on empty tree");

        return minNode(root)->key;
    }

    K maxKey() const {

        if (root == nullptr)
            throw underflow_error("Tree underflow: maxKey() on empty tree");

        Node* curr = root;

        while (curr->right != nullptr)
            curr = curr->right;

        return curr->key;
    }

    // ===== TRAVERSALS =====

    // Left, node, right: the keys in sorted order (A-Z)
    DoublyLinkedList<K> keysInOrder() const {
        DoublyLinkedList<K> out;
        inOrderAt(root, out);
        return out;
    }

    // Node, left, right
    DoublyLinkedList<K> keysPreOrder() const {
        DoublyLinkedList<K> out;
        preOrderAt(root, out);
        return out;
    }

    // Left, right, node
    DoublyLinkedList<K> keysPostOrder() const {
        DoublyLinkedList<K> out;
        postOrderAt(root, out);
        return out;
    }

    // Level by level from the root, using a queue
    DoublyLinkedList<K> keysLevelOrder() const {

        DoublyLinkedList<K> out;

        if (root == nullptr)
            return out;

        Queue<const Node*> queue(count);
        queue.enqueue(root);

        while (!queue.isEmpty()) {

            const Node* node = queue.dequeue();
            out.insertAtTail(node->key);

            if (node->left != nullptr)
                queue.enqueue(node->left);

            if (node->right != nullptr)
                queue.enqueue(node->right);
        }

        return out;
    }

    // Keys starting with prefix, in sorted order (string keys only)
    DoublyLinkedList<K> keysWithPrefix(const K& prefix) const {
        DoublyLinkedList<K> out;
        prefixAt(root, prefix, out);
        return out;
    }

    // ===== INFO =====

    // Every BST and AVL rule holds (used by the tests after each change)
    bool isValid() const {

        DoublyLinkedList<K> keys = keysInOrder();

        for (typename DoublyLinkedList<K>::Node* curr = keys.getHead(); curr != nullptr && curr->next != nullptr; curr = curr->next)
            if (!(curr->data < curr->next->data))
                return false;

        return keys.size() == count && heightsValid(root);
    }

    // For drawing the tree
    const Node* getRoot() const { return root; }

    // Empty tree: 0, one node: 1
    int height() const { return heightOf(root); }

    int size() const { return count; }

    bool isEmpty() const { return count == 0; }

    bool isSelfBalancing() const { return selfBalancing; }

    // Single rotations done so far (a double rotation counts as 2)
    int getRotationCount() const { return rotations; }

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }
};

#endif

// ==================================================================
// Algorithms/Sorting.h
// ==================================================================
#ifndef SORTING_H
#define SORTING_H

// ===== SORTING (Weeks 6, 7 and 10) =====
//
// Six ways to put an array in order. Each one counts its work
// (comparisons, swaps, passes), so the Sort menu can compare them.
//
//   Selection: find the smallest, put it first, repeat with the rest.
//   Bubble:    swap neighbours that are in the wrong order; the biggest
//              "bubbles" to the end each pass.
//   Insertion: take the next item and slide it into its place, like
//              sorting playing cards in your hand.
//   Merge:     cut the array in half, sort each half, zip them together.
//   Quick:     pick a "pivot", put smaller items on its left and bigger
//              on its right, then do the same on each side.
//   Heap:      turn the array into a heap, then keep moving the biggest
//              item to the end.
//
//              best        worst       extra space   stable
//   selection  O(n^2)      O(n^2)      O(1)          no    (at most n-1 swaps)
//   bubble     O(n)        O(n^2)      O(1)          yes   (stops when a pass makes no swap)
//   insertion  O(n)        O(n^2)      O(1)          yes
//   merge      O(n log n)  O(n log n)  O(n)          yes
//   quick      O(n log n)  O(n^2)      O(log n)      no    (worst case: data already sorted)
//   heap       O(n log n)  O(n log n)  O(1)          no
//
// comesBefore(a, b) returns true when a must be placed before b.

struct SortStats {
    int comparisons;
    int swaps;          // insertion sort: each shift, merge sort: each item copied back
    int passes;         // merge / quick sort: each merge / partition step
};

// Default order: smallest first
template <class T>
struct Ascending {

    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <class T>
void swapValues(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// ===== SELECTION SORT =====
// Find the smallest item of the unsorted part and swap it to the front.

template <class T, class Compare>
SortStats selectionSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 0; i < n - 1; i++) {

        stats.passes++;
        int best = i;

        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;

            if (comesBefore(arr[j], arr[best]))
                best = j;
        }

        if (best != i) {
            swapValues(arr[i], arr[best]);
            stats.swaps++;
        }
    }

    return stats;
}

// ===== BUBBLE SORT =====
// Swap neighbours that are out of order; the largest bubbles to the end.
// Stops early when a whole pass makes no swap.

template <class T, class Compare>
SortStats bubbleSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 0; i < n - 1; i++) {

        bool swapped = false;
        stats.passes++;

        // the last i items are already in place
        for (int j = 0; j < n - i - 1; j++) {
            stats.comparisons++;

            if (comesBefore(arr[j + 1], arr[j])) {
                swapValues(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return stats;
}

// ===== INSERTION SORT =====
// Take each key and insert it into the sorted left part,
// shifting bigger items one place to the right.

template <class T, class Compare>
SortStats insertionSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    for (int i = 1; i < n; i++) {

        stats.passes++;
        T key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (!comesBefore(key, arr[j]))
                break;

            arr[j + 1] = arr[j];
            stats.swaps++;
            j--;
        }

        arr[j + 1] = key;
    }

    return stats;
}

// ===== MERGE SORT (Week 7, divide and conquer) =====
// Split the array in half, sort each half, then merge the two sorted halves.

// Merges arr[low..mid] and arr[mid+1..high], which are already sorted
template <class T, class Compare>
void mergeHalves(T* arr, T* temp, int low, int mid, int high, Compare comesBefore, SortStats& stats) {

    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high) {
        stats.comparisons++;

        // take from the right half only if it is strictly first (keeps it stable)
        if (comesBefore(arr[j], arr[i]))
            temp[k++] = arr[j++];
        else
            temp[k++] = arr[i++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= high)
        temp[k++] = arr[j++];

    for (k = low; k <= high; k++) {
        arr[k] = temp[k];
        stats.swaps++;
    }
}

template <class T, class Compare>
void mergeSortRange(T* arr, T* temp, int low, int high, Compare comesBefore, SortStats& stats) {

    if (low >= high)        // base case: 0 or 1 item is already sorted
        return;

    int mid = low + (high - low) / 2;

    mergeSortRange(arr, temp, low, mid, comesBefore, stats);
    mergeSortRange(arr, temp, mid + 1, high, comesBefore, stats);

    stats.passes++;
    mergeHalves(arr, temp, low, mid, high, comesBefore, stats);
}

template <class T, class Compare>
SortStats mergeSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    if (n < 2)
        return stats;

    T* temp = new T[n];
    mergeSortRange(arr, temp, 0, n - 1, comesBefore, stats);
    delete[] temp;

    return stats;
}

// ===== QUICK SORT (Week 7, divide and conquer) =====
// Pick the last item as the pivot, move everything that comes before it
// to its left (partition), then quick sort both sides.
// If the data is already sorted, every pivot splits off just one item,
// which is the O(n^2) worst case.

// Lomuto partition: returns the pivot's final index
template <class T, class Compare>
int quickPartition(T* arr, int low, int high, Compare comesBefore, SortStats& stats) {

    T pivot = arr[high];
    int i = low - 1;        // last index of the "before pivot" part

    for (int j = low; j < high; j++) {
        stats.comparisons++;

        if (comesBefore(arr[j], pivot)) {
            i++;

            if (i != j) {
                swapValues(arr[i], arr[j]);
                stats.swaps++;
            }
        }
    }

    if (i + 1 != high) {
        swapValues(arr[i + 1], arr[high]);
        stats.swaps++;
    }

    return i + 1;
}

template <class T, class Compare>
void quickSortRange(T* arr, int low, int high, Compare comesBefore, SortStats& stats) {

    if (low >= high)        // base case: 0 or 1 item
        return;

    stats.passes++;
    int pivotIndex = quickPartition(arr, low, high, comesBefore, stats);

    quickSortRange(arr, low, pivotIndex - 1, comesBefore, stats);
    quickSortRange(arr, pivotIndex + 1, high, comesBefore, stats);
}

template <class T, class Compare>
SortStats quickSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};
    quickSortRange(arr, 0, n - 1, comesBefore, stats);
    return stats;
}

// ===== HEAP SORT (Week 10) =====
// Build a max-heap inside the array (the item that comes last is at the
// root), then swap the root to the end and fix the heap, n - 1 times.

// Moves arr[i] down until both children come before it (heap of size n)
template <class T, class Compare>
void heapSiftDown(T* arr, int n, int i, Compare comesBefore, SortStats& stats) {

    while (true) {

        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n) {
            stats.comparisons++;

            if (comesBefore(arr[largest], arr[left]))
                largest = left;
        }

        if (right < n) {
            stats.comparisons++;

            if (comesBefore(arr[largest], arr[right]))
                largest = right;
        }

        if (largest == i)
            return;

        swapValues(arr[i], arr[largest]);
        stats.swaps++;
        i = largest;
    }
}

template <class T, class Compare>
SortStats heapSort(T* arr, int n, Compare comesBefore) {

    SortStats stats = {0, 0, 0};

    // build the heap: heapify every parent, last one first. O(n)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapSiftDown(arr, n, i, comesBefore, stats);

    // move the root (the last item in sorted order) to the end, shrink the heap
    for (int end = n - 1; end > 0; end--) {
        stats.passes++;

        swapValues(arr[0], arr[end]);
        stats.swaps++;

        heapSiftDown(arr, end, 0, comesBefore, stats);
    }

    return stats;
}

// Same sorts with the default order (smallest first)

template <class T>
SortStats selectionSort(T* arr, int n) {
    return selectionSort(arr, n, Ascending<T>());
}

template <class T>
SortStats bubbleSort(T* arr, int n) {
    return bubbleSort(arr, n, Ascending<T>());
}

template <class T>
SortStats insertionSort(T* arr, int n) {
    return insertionSort(arr, n, Ascending<T>());
}

template <class T>
SortStats mergeSort(T* arr, int n) {
    return mergeSort(arr, n, Ascending<T>());
}

template <class T>
SortStats quickSort(T* arr, int n) {
    return quickSort(arr, n, Ascending<T>());
}

template <class T>
SortStats heapSort(T* arr, int n) {
    return heapSort(arr, n, Ascending<T>());
}

#endif

// ==================================================================
// Algorithms/Searching.h
// ==================================================================
#ifndef SEARCHING_H
#define SEARCHING_H

// ===== SEARCHING (Week 1) =====
//
//   Linear search: check the items one by one from the start.       O(n)
//                  Works on any order.
//
//   Binary search: look at the middle item. If the key is bigger,
//                  throw away the left half, else the right half.
//                  Repeat until found.                               O(log n)
//                  Only works on SORTED data in an ARRAY (we need to
//                  jump to the middle, which a linked list can't do).
//
// Every search returns where it found the key (-1 if not found) and how
// many items it checked, so the Search menu can show the difference.
//
// keyOf(item) says what to compare: for posts it gives the post's ID.

struct SearchResult {
    int index;
    int steps;
};

// Default key: the item itself
template <class T>
struct ItemItself {

    const T& operator()(const T& item) const {
        return item;
    }
};

// ===== LINEAR SEARCH =====

template <class T, class K, class KeyOf>
SearchResult linearSearch(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};

    for (int i = 0; i < n; i++) {
        result.steps++;

        if (keyOf(arr[i]) == key) {
            result.index = i;
            return result;
        }
    }

    return result;
}

// ===== BINARY SEARCH (iterative) =====
// arr must be sorted by keyOf, smallest first.

template <class T, class K, class KeyOf>
SearchResult binarySearch(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};
    int low = 0;
    int high = n - 1;

    while (low <= high) {

        int mid = low + (high - low) / 2;   // same as (low + high) / 2, but cannot overflow
        result.steps++;

        if (keyOf(arr[mid]) == key) {
            result.index = mid;
            return result;
        }

        if (keyOf(arr[mid]) < key)
            low = mid + 1;      // key is in the right half
        else
            high = mid - 1;     // key is in the left half
    }

    return result;
}

// ===== BINARY SEARCH (recursive, Week 4) =====
// Base case: low > high means the range is empty, so the key is not there.

template <class T, class K, class KeyOf>
int binarySearchRange(const T* arr, int low, int high, const K& key, KeyOf keyOf, int& steps) {

    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;
    steps++;

    if (keyOf(arr[mid]) == key)
        return mid;

    if (keyOf(arr[mid]) < key)
        return binarySearchRange(arr, mid + 1, high, key, keyOf, steps);

    return binarySearchRange(arr, low, mid - 1, key, keyOf, steps);
}

template <class T, class K, class KeyOf>
SearchResult binarySearchRecursive(const T* arr, int n, const K& key, KeyOf keyOf) {

    SearchResult result = {-1, 0};
    result.index = binarySearchRange(arr, 0, n - 1, key, keyOf, result.steps);
    return result;
}

// Same searches when the item itself is the key

template <class T>
SearchResult linearSearch(const T* arr, int n, const T& key) {
    return linearSearch(arr, n, key, ItemItself<T>());
}

template <class T>
SearchResult binarySearch(const T* arr, int n, const T& key) {
    return binarySearch(arr, n, key, ItemItself<T>());
}

template <class T>
SearchResult binarySearchRecursive(const T* arr, int n, const T& key) {
    return binarySearchRecursive(arr, n, key, ItemItself<T>());
}

#endif

// ==================================================================
// Post.h
// ==================================================================
#ifndef POST_H
#define POST_H

#include <iostream>
#include <iomanip>
#include <string>


using namespace std;

// One post in the feed: its ID, title, category, who wrote it and its likes.
// A Post "is a" Content, so it must have a display() function.
class Post : public Content {

private:
    int id;
    string title;
    string category;
    string author;      // username of the user who posted it
    int likes;

public:

    Post(int i, string t, string c, string a, int l) : id(i), title(t), category(c), author(a), likes(l > 0 ? l : 0) {}

    int getId() const { return id; }

    string getTitle() const { return title; }

    string getCategory() const { return category; }

    string getAuthor() const { return author; }

    int getLikes() const { return likes; }

    void like() {
        likes++;
    }

    void unlike() {
        if (likes > 0)
            likes--;
    }

    // One table row: ID, title, category, likes, author
    void display() const override {

        cout << CYAN << left << setw(5) << id << RESET
             << setw(28) << shorten(title, 26)
             << setw(13) << shorten(category, 12)
             << GREEN << setw(7) << likes << RESET
             << "@" << author;
    }

    // Bigger view used while scrolling the feed
    void displayCard() const {

        cout << CYAN << "  #" << id << "  " << RESET << title << endl;
        cout << "  Category : " << YELLOW << category << RESET << endl;
        cout << "  Posted by: @" << author << endl;
        cout << "  Likes    : " << GREEN << likes << RESET << endl;
    }
};

#endif

// ==================================================================
// User.h
// ==================================================================
#ifndef USER_H
#define USER_H

#include <string>


using namespace std;

// One account. Besides a name and an age, every user carries three small
// data structures of their own:
//   interests      a linked list  (what they like: tech, sports, ...)
//   likeHistory    a stack        (the posts they liked, newest on top: Undo)
//   notifications  a queue        (newest 10 messages, oldest shown first)
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

// ==================================================================
// FeedEngine.h
// ==================================================================
#ifndef FEEDENGINE_H
#define FEEDENGINE_H

#include <string>


using namespace std;

// ===== RULES FOR ORDERING =====
// Each rule returns true when a must come before b.

// Trending: more likes first; equal likes -> older post (smaller ID) first
struct MoreLikes {

    bool operator()(Post* a, Post* b) const {
        return a->getLikes() > b->getLikes() || (a->getLikes() == b->getLikes() && a->getId() < b->getId());
    }
};

// Title A-Z, ignoring capital letters
struct TitleAZ {

    bool operator()(Post* a, Post* b) const {
        return toLowerCase(a->getTitle()) < toLowerCase(b->getTitle());
    }
};

struct SmallerId {

    bool operator()(Post* a, Post* b) const {
        return a->getId() < b->getId();
    }
};

// Search key: the post's ID
struct PostId {

    int operator()(Post* post) const {
        return post->getId();
    }
};

// "People you may know": a user you don't follow yet, and how many
// of the people you follow already follow them
struct Suggestion {
    string username;
    int mutual;
};

struct MoreMutual {

    bool operator()(const Suggestion& a, const Suggestion& b) const {
        return a.mutual > b.mutual || (a.mutual == b.mutual && a.username < b.username);
    }
};

// What happened, so the screens can show the right message
enum class Result { Done, NotLoggedIn, PostNotFound, AlreadyLiked, NothingToUndo, FeedFull,
                    UsernameTaken, UserNotFound, CannotFollowSelf, AlreadyFollowing, NotFollowing };

// A-Z title index: lowercase title -> the posts with that title (in ID order)
typedef AVLTree<string, DoublyLinkedList<Post*> > TitleIndex;

enum class SortKey { Likes, Title, Id };

enum class SortAlgorithm { Selection, Bubble, Insertion, Merge, Quick, Heap };

// Runs the chosen sort with the chosen order
template <class Compare>
SortStats runSort(Post** arr, int n, SortAlgorithm algorithm, Compare comesBefore) {

    switch (algorithm) {
        case SortAlgorithm::Selection:
            return selectionSort(arr, n, comesBefore);
        case SortAlgorithm::Bubble:
            return bubbleSort(arr, n, comesBefore);
        case SortAlgorithm::Insertion:
            return insertionSort(arr, n, comesBefore);
        case SortAlgorithm::Merge:
            return mergeSort(arr, n, comesBefore);
        case SortAlgorithm::Quick:
            return quickSort(arr, n, comesBefore);
        default:
            return heapSort(arr, n, comesBefore);
    }
}

// ===== FEED ENGINE: THE BRAIN (Singleton) =====
//
// This class does all the real work. It never prints anything: every
// action returns a Result (Done, AlreadyLiked, ...) and Screens.h decides
// what message to show. That keeps each file simple.
//
// "Singleton" means there is only ONE FeedEngine in the whole program:
// the constructor is private, and getInstance() always hands back the same one.
//
// It keeps every post and user in the structure that suits each job:
//
//   posts[]     array sorted by ID          binary search demo, O(log n)
//   postIndex   hash table  ID -> post      find a post, O(1)
//   feed        doubly linked list          scroll Next / Prev, newest first
//   titleIndex  AVL tree  title -> posts    A-Z list, title search, O(log n)
//   users       doubly linked list          every account, in sign-up order
//   userIndex   hash table  name -> user    log in, O(1)
//   follows     directed graph              follow, suggestions, shortest path
//   (trending)  max-heap built on demand    top posts, O(n + k log n)
//
// It never prints anything; Screens.h shows the results.

class FeedEngine {

public:

    static const int MAX_POSTS = 200;

private:

    Post* posts[MAX_POSTS];
    int postCount;
    int nextId;

    HashTable<int, Post*> postIndex;
    DoublyLinkedList<Post*> feed;
    TitleIndex titleIndex;

    DoublyLinkedList<User*> users;
    HashTable<string, User*> userIndex;
    Graph<string> follows;

    User* currentUser;

    FeedEngine() : postCount(0), nextId(1), postIndex(), feed(), titleIndex(true), users(), userIndex(), follows(true), currentUser(nullptr) {}

    FeedEngine(const FeedEngine&) = delete;
    FeedEngine& operator=(const FeedEngine&) = delete;

    // All posts of the given titles, keeping the titles' order
    DoublyLinkedList<Post*> postsForTitles(const DoublyLinkedList<string>& titles) const {

        DoublyLinkedList<Post*> result;

        for (DoublyLinkedList<string>::Node* t = titles.getHead(); t != nullptr; t = t->next) {

            const DoublyLinkedList<Post*>* posts = titleIndex.find(t->data);

            if (posts != nullptr)
                for (DoublyLinkedList<Post*>::Node* p = posts->getHead(); p != nullptr; p = p->next)
                    result.insertAtTail(p->data);
        }

        return result;
    }

public:

    static FeedEngine* getInstance() {
        static FeedEngine instance;     // created once, destroyed when the program ends
        return &instance;
    }

    ~FeedEngine() {
        reset();
    }

    // Deletes every post, user and follow
    void reset() {

        for (int i = 0; i < postCount; i++)
            delete posts[i];

        postCount = 0;
        nextId = 1;
        postIndex.clear();
        feed.clear();
        titleIndex.clear();

        for (DoublyLinkedList<User*>::Node* curr = users.getHead(); curr != nullptr; curr = curr->next)
            delete curr->data;

        users.clear();
        userIndex.clear();
        follows.clear();

        currentUser = nullptr;
    }

    // ===== POSTS =====

    // Adds a post loaded from the file or created by a user.
    // IDs only grow, so posts[] stays sorted by ID without any sorting.
    bool addPost(int id, const string& title, const string& category, const string& author, int likes) {

        if (postCount == MAX_POSTS)
            return false;

        if (postCount > 0 && id <= posts[postCount - 1]->getId())
            return false;

        Post* post = new Post(id, title, category, author, likes);

        posts[postCount++] = post;
        postIndex.insert(id, post);
        feed.insertAtHead(post);                                // newest first
        titleIndex[toLowerCase(title)].insertAtTail(post);      // same title: ID order
        nextId = id + 1;

        return true;
    }

    // The 13 starting posts from the OOP version
    void seedDefaultPosts() {

        string titles[] = {
            "Football Highlights", "AI Revolution", "Makeup Tutorial", "Beauty Skincare Routine",
            "Study Tips", "Healthy Diet", "Gym Workout", "Travel Vlog", "Coding Basics",
            "Fashion Trends", "Gaming Review", "Science Facts", "News Update"
        };

        string categories[] = {
            "Sports", "Technology", "Makeup", "Beauty", "Education", "Health", "Fitness",
            "Travel", "Technology", "Fashion", "Gaming", "Education", "News"
        };

        for (int i = 0; i < 13; i++)
            addPost(nextId, titles[i], categories[i], "feedengine", 0);
    }

    Result createPost(const string& title, const string& category) {

        if (currentUser == nullptr)
            return Result::NotLoggedIn;

        if (!addPost(nextId, title, category, currentUser->getUsername(), 0))
            return Result::FeedFull;

        return Result::Done;
    }

    // Hash table lookup. O(1) on average
    Post* findPost(int id) const {
        return postIndex.get(id, nullptr);
    }

    // The Search menu runs the same ID lookup four ways to compare their steps
    SearchResult searchByIdLinear(int id) const {
        return linearSearch(posts, postCount, id, PostId());
    }

    SearchResult searchByIdBinary(int id) const {
        return binarySearch(posts, postCount, id, PostId());
    }

    SearchResult searchByIdRecursive(int id) const {
        return binarySearchRecursive(posts, postCount, id, PostId());
    }

    int hashStepsForId(int id) const {
        return postIndex.stepsToFind(id);
    }

    // Linear search through the feed: title or category contains the keyword
    DoublyLinkedList<Post*> searchByKeyword(const string& keyword) const {

        DoublyLinkedList<Post*> matches;

        for (DoublyLinkedList<Post*>::Node* curr = feed.getHead(); curr != nullptr; curr = curr->next)
            if (containsText(curr->data->getTitle(), keyword) || containsText(curr->data->getCategory(), keyword))
                matches.insertAtTail(curr->data);

        return matches;
    }

    // ===== TITLE INDEX (AVL tree) =====

    // Every post A-Z by title: in-order traversal of the title tree
    DoublyLinkedList<Post*> postsByTitle() const {
        return postsForTitles(titleIndex.keysInOrder());
    }

    // Titles that start with the prefix, A-Z. Ignores capital letters.
    // Only the part of the tree that can hold them is visited: O(log n + k)
    DoublyLinkedList<Post*> searchByTitlePrefix(const string& prefix) const {
        return postsForTitles(titleIndex.keysWithPrefix(toLowerCase(prefix)));
    }

    // Tree nodes visited to look up one exact title
    int titleSearchSteps(const string& title) const {
        return titleIndex.searchSteps(toLowerCase(title));
    }

    const TitleIndex& getTitleIndex() const { return titleIndex; }

    // Sorts a COPY of posts[] into out[] (size MAX_POSTS), so posts[] stays
    // sorted by ID and binary search keeps working
    SortStats sortedCopy(SortKey key, SortAlgorithm algorithm, Post** out) const {

        for (int i = 0; i < postCount; i++)
            out[i] = posts[i];

        if (key == SortKey::Likes)
            return runSort(out, postCount, algorithm, MoreLikes());

        if (key == SortKey::Title)
            return runSort(out, postCount, algorithm, TitleAZ());

        return runSort(out, postCount, algorithm, SmallerId());
    }

    // Builds a max-heap of all posts (O(n)), then takes the top k out
    // (O(k log n)). Returns how many were written to out[].
    int trendingTop(int k, Post** out) const {

        MaxHeap<Post*, MoreLikes> heap(postCount > 0 ? postCount : 1);
        heap.buildFrom(posts, postCount);

        int taken = 0;

        while (taken < k && !heap.isEmpty())
            out[taken++] = heap.extractMax();

        return taken;
    }

    // Posts in the user's interests (not their own), most liked first
    PriorityQueue<Post*, MoreLikes> recommendationsFor(const User* user) const {

        PriorityQueue<Post*, MoreLikes> picks;

        for (int i = 0; i < postCount; i++)
            if (posts[i]->getAuthor() != user->getUsername() && user->isInterestedIn(posts[i]->getCategory()))
                picks.enqueue(posts[i]);

        return picks;
    }

    // ===== LIKES =====

    // What happens when you press Like, in 5 steps
    Result likePost(int id) {

        if (currentUser == nullptr)          // nobody logged in?
            return Result::NotLoggedIn;

        Post* post = findPost(id);           // 1. find it (hash table)

        if (post == nullptr)
            return Result::PostNotFound;

        if (currentUser->hasLiked(id))       // 2. liked already?
            return Result::AlreadyLiked;

        post->like();                        // 3. +1 like
        currentUser->recordLike(id);         // 4. onto your pile (stack)

        // 5. tell the author (their notification queue)
        User* author = findUser(post->getAuthor());

        if (author != nullptr && author != currentUser)
            author->notify("@" + currentUser->getUsername() + " liked your post \"" + post->getTitle() + "\"");

        return Result::Done;
    }

    // Pops the most recent like off the user's stack and removes it
    Result undoLastLike(Post*& undone) {

        undone = nullptr;

        if (currentUser == nullptr)
            return Result::NotLoggedIn;

        if (currentUser->getLikeHistory().isEmpty())
            return Result::NothingToUndo;

        undone = findPost(currentUser->undoLastLike());

        if (undone != nullptr)
            undone->unlike();

        return Result::Done;
    }

    // ===== USERS =====

    // Hash table lookup. O(1) on average
    User* findUser(const string& username) const {
        return userIndex.get(toLowerCase(username), nullptr);
    }

    // Adds a user loaded from the file. false if the username is taken.
    bool addUser(User* user) {

        if (user == nullptr || findUser(user->getUsername()) != nullptr)
            return false;

        users.insertAtTail(user);
        userIndex.insert(user->getUsername(), user);
        follows.addVertex(user->getUsername());

        return true;
    }

    // Creates the account and logs it in
    Result signUp(const string& username, const string& name, int age) {

        User* user = new User(username, name, age);

        if (!addUser(user)) {
            delete user;
            return Result::UsernameTaken;
        }

        currentUser = user;
        return Result::Done;
    }

    Result login(const string& username) {

        User* user = findUser(username);

        if (user == nullptr)
            return Result::UserNotFound;

        currentUser = user;
        return Result::Done;
    }

    void logout() {
        currentUser = nullptr;
    }

    // ===== FOLLOWS (Graph) =====

    // Adds the edge from -> to (used when loading the file, no notification)
    bool addFollow(const string& from, const string& to) {
        return follows.addEdge(toLowerCase(from), toLowerCase(to));
    }

    Result follow(const string& username) {

        if (currentUser == nullptr)
            return Result::NotLoggedIn;

        User* target = findUser(username);

        if (target == nullptr)
            return Result::UserNotFound;

        if (target == currentUser)
            return Result::CannotFollowSelf;

        if (!follows.addEdge(currentUser->getUsername(), target->getUsername()))
            return Result::AlreadyFollowing;

        target->notify("@" + currentUser->getUsername() + " started following you");
        return Result::Done;
    }

    Result unfollow(const string& username) {

        if (currentUser == nullptr)
            return Result::NotLoggedIn;

        User* target = findUser(username);

        if (target == nullptr)
            return Result::UserNotFound;

        if (!follows.removeEdge(currentUser->getUsername(), target->getUsername()))
            return Result::NotFollowing;

        return Result::Done;
    }

    bool isFollowing(const string& from, const string& to) const {
        return follows.hasEdge(toLowerCase(from), toLowerCase(to));
    }

    DoublyLinkedList<string> followingOf(const string& username) const {
        return follows.neighbours(toLowerCase(username));
    }

    DoublyLinkedList<string> followersOf(const string& username) const {
        return follows.incoming(toLowerCase(username));
    }

    // Posts by people the user follows, newest first
    DoublyLinkedList<Post*> followingFeed(const string& username) const {

        DoublyLinkedList<Post*> result;

        for (DoublyLinkedList<Post*>::Node* curr = feed.getHead(); curr != nullptr; curr = curr->next)
            if (isFollowing(username, curr->data->getAuthor()))
                result.insertAtTail(curr->data);

        return result;
    }

    // Friends of friends (BFS distance 2), most mutual follows first
    PriorityQueue<Suggestion, MoreMutual> suggestionsFor(const string& username) const {

        PriorityQueue<Suggestion, MoreMutual> result;
        string me = toLowerCase(username);
        DoublyLinkedList<string> candidates = follows.verticesAtDistance(me, 2);
        DoublyLinkedList<string> myFollowing = follows.neighbours(me);

        for (DoublyLinkedList<string>::Node* c = candidates.getHead(); c != nullptr; c = c->next) {

            Suggestion suggestion = {c->data, 0};

            for (DoublyLinkedList<string>::Node* f = myFollowing.getHead(); f != nullptr; f = f->next)
                if (follows.hasEdge(f->data, c->data))
                    suggestion.mutual++;

            result.enqueue(suggestion);
        }

        return result;
    }

    // Shortest follow chain from one user to another (BFS), e.g. ali -> sara -> omar
    DoublyLinkedList<string> connectionPath(const string& from, const string& to) const {
        return follows.shortestPath(toLowerCase(from), toLowerCase(to));
    }

    // ===== GETTERS =====

    User* getCurrentUser() const { return currentUser; }

    int getPostCount() const { return postCount; }

    // Post at index i of posts[] (sorted by ID), or nullptr
    Post* getPostAt(int index) const {

        if (index < 0 || index >= postCount)
            return nullptr;

        return posts[index];
    }

    const DoublyLinkedList<Post*>& getFeed() const { return feed; }

    const DoublyLinkedList<User*>& getUsers() const { return users; }

    int getFollowCount() const { return follows.getEdgeCount(); }
};

#endif

// ==================================================================
// FileManager.h
// ==================================================================
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>


using namespace std;

// ===== FILE MANAGER =====
//
// Saves everything to two text files and loads it back on the next run,
// so likes, accounts, follows and notifications are not lost.
//
// posts.txt (in ID order):
//   POST|id|likes|author|category|title
//
// users.txt (lines after a USER line belong to that user):
//   USER|username|name|age
//   INTEREST|tech
//   LIKE|3               (oldest like first)
//   NOTE|message         (oldest notification first)
//   FOLLOW|sara          (this user follows sara)
//
// Converting to and from text is kept apart from reading and writing
// the files, so the tests can check it without touching the disk.

class FileManager {

private:

    // Splits "a|b|c" into parts[]. The last part keeps any extra '|',
    // so a title is never cut. Returns the number of parts.
    static int splitLine(const string& line, string parts[], int maxParts) {

        int count = 0;
        string current;

        for (int i = 0; i < static_cast<int>(line.length()); i++) {

            if (line[i] == '|' && count < maxParts - 1) {
                parts[count++] = current;
                current = "";
            }
            else {
                current += line[i];
            }
        }

        parts[count++] = current;
        return count;
    }

    // Reads the line that starts at index start and moves start past it.
    // A '\r' (Windows line ending) is dropped.
    static string nextLine(const string& text, int& start) {

        string line;

        while (start < static_cast<int>(text.length()) && text[start] != '\n') {
            if (text[start] != '\r')
                line += text[start];
            start++;
        }

        start++;    // skip the '\n'
        return line;
    }

    // "123" -> 123. Digits only, at most 9 so it fits in an int.
    static bool parseNumber(const string& text, int& value) {

        if (text.empty() || text.length() > 9)
            return false;

        value = 0;

        for (int i = 0; i < static_cast<int>(text.length()); i++) {

            if (text[i] < '0' || text[i] > '9')
                return false;

            value = value * 10 + (text[i] - '0');
        }

        return true;
    }

    // 123 -> "123" (for numbers >= 0)
    static string numberToText(int value) {

        if (value <= 0)
            return "0";

        string digits;

        while (value > 0) {
            digits = static_cast<char>('0' + value % 10) + digits;
            value /= 10;
        }

        return digits;
    }

public:

    // ===== ENGINE -> TEXT =====

    static string postsToText(const FeedEngine& engine) {

        string text;

        for (int i = 0; i < engine.getPostCount(); i++) {

            Post* post = engine.getPostAt(i);

            text += "POST|" + numberToText(post->getId()) + "|" + numberToText(post->getLikes()) + "|"
                  + post->getAuthor() + "|" + post->getCategory() + "|" + post->getTitle() + "\n";
        }

        return text;
    }

    static string usersToText(const FeedEngine& engine) {

        string text;

        for (DoublyLinkedList<User*>::Node* u = engine.getUsers().getHead(); u != nullptr; u = u->next) {

            User* user = u->data;

            text += "USER|" + user->getUsername() + "|" + user->getName() + "|" + numberToText(user->getAge()) + "\n";

            for (DoublyLinkedList<string>::Node* curr = user->getInterests().getHead(); curr != nullptr; curr = curr->next)
                text += "INTEREST|" + curr->data + "\n";

            // The stack gives the newest like first. Pushing it into a second
            // stack reverses it, so the oldest like is written first.
            Stack<int> reversed;

            for (Stack<int>::Node* curr = user->getLikeHistory().getTop(); curr != nullptr; curr = curr->next)
                reversed.push(curr->data);

            while (!reversed.isEmpty())
                text += "LIKE|" + numberToText(reversed.pop()) + "\n";

            const Queue<string>& notes = user->getNotifications();

            for (int i = 0; i < notes.size(); i++)
                text += "NOTE|" + notes.at(i) + "\n";

            DoublyLinkedList<string> following = engine.followingOf(user->getUsername());

            for (DoublyLinkedList<string>::Node* curr = following.getHead(); curr != nullptr; curr = curr->next)
                text += "FOLLOW|" + curr->data + "\n";
        }

        return text;
    }

    // ===== TEXT -> ENGINE =====
    // Bad or unknown lines are skipped. Returns how many were loaded.

    static int loadPostsFromText(FeedEngine& engine, const string& text) {

        int loaded = 0;
        int start = 0;

        while (start < static_cast<int>(text.length())) {

            string line = nextLine(text, start);
            string parts[6];
            int id = 0;
            int likes = 0;

            if (splitLine(line, parts, 6) != 6 || parts[0] != "POST")
                continue;

            if (!parseNumber(parts[1], id) || !parseNumber(parts[2], likes) || parts[5].empty())
                continue;

            if (engine.addPost(id, parts[5], parts[4], parts[3], likes))
                loaded++;
        }

        return loaded;
    }

    // Load posts first: LIKE lines are only kept if the post exists
    static int loadUsersFromText(FeedEngine& engine, const string& text) {

        int loaded = 0;
        int start = 0;
        User* current = nullptr;    // the user the next lines belong to

        // Pass 1: users, interests, likes and notifications
        while (start < static_cast<int>(text.length())) {

            string line = nextLine(text, start);
            string head[2];

            if (splitLine(line, head, 2) != 2)
                continue;

            string tag = head[0];
            string rest = head[1];

            if (tag == "USER") {

                string parts[3];
                int age = 0;
                current = nullptr;

                if (splitLine(rest, parts, 3) == 3 && !parts[0].empty() && parseNumber(parts[2], age)) {

                    User* user = new User(parts[0], parts[1], age);

                    if (engine.addUser(user)) {
                        current = user;
                        loaded++;
                    }
                    else {
                        delete user;
                    }
                }
            }
            else if (current != nullptr) {

                int id = 0;

                if (tag == "INTEREST" && !rest.empty())
                    current->addInterest(rest);
                else if (tag == "LIKE" && parseNumber(rest, id) && engine.findPost(id) != nullptr && !current->hasLiked(id))
                    current->recordLike(id);
                else if (tag == "NOTE" && !rest.empty())
                    current->notify(rest);
            }
        }

        // Pass 2: follows, now that every user exists
        start = 0;
        string from;

        while (start < static_cast<int>(text.length())) {

            string line = nextLine(text, start);
            string head[2];

            if (splitLine(line, head, 2) != 2)
                continue;

            if (head[0] == "USER") {
                string parts[3];
                splitLine(head[1], parts, 3);
                from = parts[0];
            }
            else if (head[0] == "FOLLOW" && !from.empty()) {
                engine.addFollow(from, head[1]);
            }
        }

        return loaded;
    }

    // ===== FILES =====

    static bool readFile(const string& fileName, string& text) {

        ifstream file(fileName.c_str());

        if (!file)
            return false;

        text = "";
        string line;

        while (getline(file, line))
            text += line + "\n";

        return true;
    }

    static bool writeFile(const string& fileName, const string& text) {

        ofstream file(fileName.c_str());

        if (!file)
            return false;

        file << text;
        return static_cast<bool>(file);
    }

    // Loads posts.txt and users.txt. With no saved posts, starts
    // with the 13 default posts.
    static void loadAll(FeedEngine& engine) {

        string text;
        int postsLoaded = 0;

        if (readFile("posts.txt", text))
            postsLoaded = loadPostsFromText(engine, text);

        if (postsLoaded == 0)
            engine.seedDefaultPosts();

        if (readFile("users.txt", text))
            loadUsersFromText(engine, text);
    }

    static bool saveAll(const FeedEngine& engine) {
        return writeFile("posts.txt", postsToText(engine)) && writeFile("users.txt", usersToText(engine));
    }
};

#endif

// ==================================================================
// Input.h
// ==================================================================
#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>


using namespace std;

// ===== INPUT VALIDATION =====
// Every input is read as a whole line, so a wrong entry never leaves
// junk behind in cin and the program never gets stuck in a loop.

// Thrown when input runs out (Ctrl+Z, or a closed input file),
// so main() can still save before it closes
struct InputEnded {};

// Reads one line and trims spaces at both ends
inline string readLine(const string& prompt) {

    cout << prompt;

    string line;

    if (!getline(cin, line))
        throw InputEnded();

    int start = 0;
    int end = static_cast<int>(line.length()) - 1;

    while (start <= end && isspace(static_cast<unsigned char>(line[start])))
        start++;

    while (end >= start && isspace(static_cast<unsigned char>(line[end])))
        end--;

    return line.substr(start, end - start + 1);
}

// A whole number between minValue and maxValue
inline int readInt(const string& prompt, int minValue, int maxValue) {

    while (true) {

        string line = readLine(prompt);
        istringstream in(line);
        int value;
        char extra;

        if (in >> value && !(in >> extra) && value >= minValue && value <= maxValue)
            return value;

        printError("INVALID INPUT! ENTER A NUMBER FROM " + to_string(minValue) + " TO " + to_string(maxValue));
    }
}

// Text of 1-40 characters. lettersOnly blocks digits (names, interests).
// '|' is never allowed because the save files use it to separate fields.
inline string readText(const string& prompt, bool lettersOnly) {

    while (true) {

        string text = readLine(prompt);
        bool valid = !text.empty() && text.length() <= 40;

        for (int i = 0; i < static_cast<int>(text.length()); i++) {

            char c = text[i];

            if (c == '|' || (lettersOnly && isdigit(static_cast<unsigned char>(c))))
                valid = false;
        }

        if (valid)
            return text;

        if (lettersOnly)
            printError("INVALID INPUT! ENTER LETTERS ONLY (1-40 CHARACTERS)");
        else
            printError("INVALID INPUT! ENTER 1-40 CHARACTERS, WITHOUT '|'");
    }
}

// One word of 3-20 letters, digits or _ . Returned in lowercase.
inline string readUsername(const string& prompt) {

    while (true) {

        string name = readLine(prompt);

        if (!name.empty() && name[0] == '@')     // "@ali" is fine too
            name = name.substr(1);

        bool valid = name.length() >= 3 && name.length() <= 20;

        for (int i = 0; i < static_cast<int>(name.length()); i++)
            if (!isalnum(static_cast<unsigned char>(name[i])) && name[i] != '_')
                valid = false;

        if (valid)
            return toLowerCase(name);

        printError("INVALID USERNAME! USE 3-20 LETTERS, DIGITS OR _");
    }
}

#endif

// ==================================================================
// Screens.h
// ==================================================================
#ifndef SCREENS_H
#define SCREENS_H

#include <iostream>
#include <iomanip>
#include <string>


using namespace std;

// ===== SCREENS =====
// Everything the user sees. Each screen reads input (Input.h),
// asks FeedEngine to do the work, and prints the result.

const string CATEGORY_HINT = "Sports, Technology, Makeup, Beauty, Education, Health, Fitness, Travel, Fashion, Gaming, News";

// Prints an error for anything except Result::Done. Returns true on Done.
inline bool reportResult(Result result) {

    switch (result) {
        case Result::Done:             return true;
        case Result::NotLoggedIn:      printError("PLEASE LOG IN FIRST!"); break;
        case Result::PostNotFound:     printError("INVALID POST ID!"); break;
        case Result::AlreadyLiked:     printError("YOU ALREADY LIKED THIS POST!"); break;
        case Result::NothingToUndo:    printError("NOTHING TO UNDO!"); break;
        case Result::FeedFull:         printError("THE FEED IS FULL!"); break;
        case Result::UsernameTaken:    printError("USERNAME ALREADY TAKEN!"); break;
        case Result::UserNotFound:     printError("NO ACCOUNT WITH THAT USERNAME!"); break;
        case Result::CannotFollowSelf: printError("YOU CANNOT FOLLOW YOURSELF!"); break;
        case Result::AlreadyFollowing: printError("YOU ALREADY FOLLOW THIS USER!"); break;
        case Result::NotFollowing:     printError("YOU DO NOT FOLLOW THIS USER!"); break;
    }

    return false;
}

inline void printPostHeader() {

    cout << left << setw(5) << "ID" << setw(28) << "Title" << setw(13) << "Category"
         << setw(7) << "Likes" << "Author" << endl;

    cout << "----------------------------------------------------------------" << endl;
}

// One post per line, tagged for the logged-in user
inline void printPostRow(Post* post, const User* user) {

    post->display();

    if (user != nullptr && user->hasLiked(post->getId()))
        cout << GREEN << "  [LIKED]" << RESET;

    if (user != nullptr && user->isInterestedIn(post->getCategory()))
        cout << YELLOW << "  [INTEREST]" << RESET;

    cout << endl;
}

inline void printPostList(const DoublyLinkedList<Post*>& list, const User* user) {

    printPostHeader();

    for (DoublyLinkedList<Post*>::Node* curr = list.getHead(); curr != nullptr; curr = curr->next)
        printPostRow(curr->data, user);
}

// "ali, sara, omar" or "(none)"
inline string joinNames(const DoublyLinkedList<string>& names) {

    if (names.isEmpty())
        return "(none)";

    string text;

    for (DoublyLinkedList<string>::Node* curr = names.getHead(); curr != nullptr; curr = curr->next) {
        text += "@" + curr->data;

        if (curr->next != nullptr)
            text += ", ";
    }

    return text;
}

// ===== ACCOUNT =====

inline void loginScreen(FeedEngine* engine) {

    printBox("LOGIN");

    string username = readUsername("Username: ");

    if (reportResult(engine->login(username)))
        printSuccess("Welcome back, " + engine->getCurrentUser()->getName() + "!");
}

inline void signUpScreen(FeedEngine* engine) {

    printBox("SIGN UP");

    string username;

    while (true) {
        username = readUsername("Choose a username: ");

        if (engine->findUser(username) == nullptr)
            break;

        printError("USERNAME ALREADY TAKEN!");
    }

    string name = readText("Enter Name: ", true);
    int age = readInt("Enter Age: ", 1, 120);

    if (!reportResult(engine->signUp(username, name, age)))
        return;

    cout << "Categories: " << CATEGORY_HINT << endl;

    int count = readInt("How many interests? (1-10): ", 1, 10);

    for (int i = 0; i < count; i++)
        engine->getCurrentUser()->addInterest(readText("Enter interest " + to_string(i + 1) + ": ", true));

    printSuccess("Account created. Welcome, " + name + "!");
}

inline void profileScreen(FeedEngine* engine) {

    User* user = engine->getCurrentUser();

    printBox("USER PROFILE");

    cout << "Name      : " << GREEN << user->getName() << RESET << endl;
    cout << "Username  : @" << user->getUsername() << endl;
    cout << "Age       : " << user->getAge() << endl;
    cout << "Interests : ";

    for (DoublyLinkedList<string>::Node* curr = user->getInterests().getHead(); curr != nullptr; curr = curr->next)
        cout << YELLOW << curr->data << RESET << "  ";

    cout << endl;
    cout << "Liked     : " << user->getLikeHistory().size() << " posts" << endl;
    cout << "Following : " << engine->followingOf(user->getUsername()).size() << endl;
    cout << "Followers : " << engine->followersOf(user->getUsername()).size() << endl;
}

// ===== POSTS =====

inline void allPostsScreen(FeedEngine* engine) {

    printBox("ALL POSTS (BY ID)");
    printPostHeader();

    for (int i = 0; i < engine->getPostCount(); i++)
        printPostRow(engine->getPostAt(i), engine->getCurrentUser());
}

// Walks the doubly linked list one node at a time: next and prev pointers
inline void scrollFeedScreen(FeedEngine* engine) {

    const DoublyLinkedList<Post*>& feed = engine->getFeed();
    DoublyLinkedList<Post*>::Node* curr = feed.getHead();
    int position = 1;

    if (curr == nullptr) {
        printError("THE FEED IS EMPTY!");
        return;
    }

    while (true) {

        printBox("FEED  (" + to_string(position) + " of " + to_string(feed.size()) + ", newest first)");
        curr->data->displayCard();

        if (engine->getCurrentUser()->hasLiked(curr->data->getId()))
            cout << GREEN << "  You liked this post" << RESET << endl;

        cout << "\n[N] Next   [P] Previous   [L] Like   [Q] Back to menu\n";

        string choice = toLowerCase(readLine("Choice: "));

        if (choice == "n") {
            if (curr->next != nullptr) {
                curr = curr->next;
                position++;
            }
            else {
                printError("THIS IS THE LAST POST!");
            }
        }
        else if (choice == "p") {
            if (curr->prev != nullptr) {
                curr = curr->prev;
                position--;
            }
            else {
                printError("THIS IS THE FIRST POST!");
            }
        }
        else if (choice == "l") {
            if (reportResult(engine->likePost(curr->data->getId())))
                printSuccess("Post Liked!");
        }
        else if (choice == "q") {
            return;
        }
        else {
            printError("INVALID CHOICE! ENTER N, P, L OR Q");
        }
    }
}

inline void likeScreen(FeedEngine* engine) {

    int id = readInt("Enter Post ID: ", 1, 999999);

    if (reportResult(engine->likePost(id)))
        printSuccess("Post Liked!");
}

inline void undoLikeScreen(FeedEngine* engine) {

    Post* undone = nullptr;

    if (reportResult(engine->undoLastLike(undone)) && undone != nullptr)
        printSuccess("Removed your like from \"" + undone->getTitle() + "\"");
}

// Walks the like stack from the top: newest like first
inline void recentlyLikedScreen(FeedEngine* engine) {

    printBox("RECENTLY LIKED (NEWEST FIRST)");

    const Stack<int>& history = engine->getCurrentUser()->getLikeHistory();

    if (history.isEmpty()) {
        cout << "No recently liked posts.\n";
        return;
    }

    printPostHeader();

    for (Stack<int>::Node* curr = history.getTop(); curr != nullptr; curr = curr->next) {

        Post* post = engine->findPost(curr->data);

        if (post != nullptr)
            printPostRow(post, nullptr);
    }
}

inline void recommendationsScreen(FeedEngine* engine) {

    printBox("RECOMMENDED FOR YOU");

    User* user = engine->getCurrentUser();
    PriorityQueue<Post*, MoreLikes> picks = engine->recommendationsFor(user);

    if (picks.isEmpty()) {
        cout << "No recommendations available. Try adding interests like: " << CATEGORY_HINT << endl;
        return;
    }

    printPostHeader();

    for (DoublyLinkedList<Post*>::Node* curr = picks.getHead(); curr != nullptr; curr = curr->next)
        printPostRow(curr->data, user);
}

inline void trendingScreen(FeedEngine* engine) {

    printBox("TOP TRENDING POSTS");

    Post* top[5];
    int count = engine->trendingTop(5, top);

    printPostHeader();

    for (int i = 0; i < count; i++)
        printPostRow(top[i], engine->getCurrentUser());

    cout << YELLOW << "\n(Max-heap built from " << engine->getPostCount()
         << " posts, top " << count << " extracted)" << RESET << endl;
}

inline void createPostScreen(FeedEngine* engine) {

    printBox("CREATE A POST");

    string title = readText("Title: ", false);

    cout << "Categories: " << CATEGORY_HINT << endl;
    string category = readText("Category: ", true);

    if (reportResult(engine->createPost(title, category))) {
        Post* post = engine->getPostAt(engine->getPostCount() - 1);
        printSuccess("Post #" + to_string(post->getId()) + " created! It is now at the top of the feed.");
    }
}

// ===== SEARCH =====

inline void searchScreen(FeedEngine* engine) {

    printBox("SEARCH POSTS");

    cout << "1. By Post ID   (compare hash table, binary and linear search)\n";
    cout << "2. By keyword   (linear search on titles and categories)\n";

    int choice = readInt("Enter choice: ", 1, 2);

    if (choice == 1) {

        int id = readInt("Enter Post ID: ", 1, 999999);
        Post* post = engine->findPost(id);

        if (post == nullptr) {
            printError("No post with ID " + to_string(id));
        }
        else {
            printPostHeader();
            printPostRow(post, engine->getCurrentUser());
        }

        cout << "\nItems checked to " << (post != nullptr ? "find" : "rule out") << " ID " << id
             << " among " << engine->getPostCount() << " posts:\n";

        cout << "  Hash table                : " << engine->hashStepsForId(id) << "   O(1)\n";
        cout << "  Binary search (iterative) : " << engine->searchByIdBinary(id).steps << "   O(log n)\n";
        cout << "  Binary search (recursive) : " << engine->searchByIdRecursive(id).steps << "   O(log n)\n";
        cout << "  Linear search             : " << engine->searchByIdLinear(id).steps << "   O(n)\n";
    }
    else {

        string keyword = readText("Enter keyword: ", false);
        DoublyLinkedList<Post*> matches = engine->searchByKeyword(keyword);

        if (matches.isEmpty()) {
            printError("No posts found.");
            return;
        }

        printPostList(matches, engine->getCurrentUser());
        cout << matches.size() << " post(s) found." << endl;
    }
}

// ===== TITLE INDEX (AVL TREE) =====

// Draws the tree on its side: the root on the left, the right subtree
// above it and the left subtree below it
inline void printTreeSideways(const TitleIndex::Node* node, int depth) {

    if (node == nullptr)
        return;

    printTreeSideways(node->right, depth + 1);

    cout << string(depth * 4, ' ') << CYAN << shorten(node->key, 24) << RESET
         << " (h=" << node->height << ")" << endl;

    printTreeSideways(node->left, depth + 1);
}

inline void titleIndexScreen(FeedEngine* engine) {

    while (true) {

        printBox("TITLE INDEX (AVL TREE)");

        cout << "1. Browse all posts A-Z       (in-order traversal)\n";
        cout << "2. Search by start of title   (e.g. \"foo\" finds Football Highlights)\n";
        cout << "3. Show the tree\n";
        cout << "4. Back\n";

        int choice = readInt("Enter choice: ", 1, 4);
        const TitleIndex& index = engine->getTitleIndex();

        if (choice == 1) {

            printBox("ALL POSTS A-Z");
            printPostList(engine->postsByTitle(), engine->getCurrentUser());
        }
        else if (choice == 2) {

            string prefix = readText("Title starts with: ", false);
            DoublyLinkedList<Post*> matches = engine->searchByTitlePrefix(prefix);

            if (matches.isEmpty()) {
                printError("No titles start with \"" + prefix + "\"");
            }
            else {
                printPostList(matches, engine->getCurrentUser());
                cout << matches.size() << " post(s) found." << endl;
            }

            cout << YELLOW << "(Tree height " << index.height() << ": one title lookup visits at most "
                 << index.height() << " of the " << index.size() << " titles)" << RESET << endl;
        }
        else if (choice == 3) {

            printBox("TITLE TREE (ROOT ON THE LEFT)");

            if (index.isEmpty()) {
                cout << "The tree is empty.\n";
                continue;
            }

            printTreeSideways(index.getRoot(), 0);

            // The same titles inserted A-Z into a plain BST become one long chain
            AVLTree<string, int> plain(false);
            DoublyLinkedList<string> titles = index.keysInOrder();

            for (DoublyLinkedList<string>::Node* curr = titles.getHead(); curr != nullptr; curr = curr->next)
                plain.insert(curr->data, 0);

            cout << endl;
            cout << "Titles           : " << index.size() << endl;
            cout << "AVL tree height  : " << index.height() << endl;
            cout << "Rotations so far : " << index.getRotationCount() << endl;
            cout << "Plain BST height : " << plain.height() << "   (same titles added A-Z, no balancing)" << endl;
        }
        else {
            return;
        }
    }
}

// ===== SORT =====

inline void sortScreen(FeedEngine* engine) {

    printBox("SORT POSTS");

    cout << "Sort by:\n";
    cout << "1. Likes (most first)\n2. Title (A-Z)\n3. Post ID\n";

    int keyChoice = readInt("Enter choice: ", 1, 3);

    cout << "\nAlgorithm:\n";
    cout << "1. Selection Sort\n2. Bubble Sort\n3. Insertion Sort\n";
    cout << "4. Merge Sort\n5. Quick Sort\n6. Heap Sort\n7. Compare all six\n";

    int algorithmChoice = readInt("Enter choice: ", 1, 7);

    SortKey keys[] = {SortKey::Likes, SortKey::Title, SortKey::Id};

    SortAlgorithm algorithms[] = {
        SortAlgorithm::Selection, SortAlgorithm::Bubble, SortAlgorithm::Insertion,
        SortAlgorithm::Merge, SortAlgorithm::Quick, SortAlgorithm::Heap
    };

    string names[] = {"Selection", "Bubble", "Insertion", "Merge", "Quick", "Heap"};

    int first = algorithmChoice == 7 ? 0 : algorithmChoice - 1;
    int last = algorithmChoice == 7 ? 5 : algorithmChoice - 1;

    Post* sorted[FeedEngine::MAX_POSTS];
    SortStats stats[6];

    for (int a = first; a <= last; a++)
        stats[a] = engine->sortedCopy(keys[keyChoice - 1], algorithms[a], sorted);

    printPostHeader();

    for (int i = 0; i < engine->getPostCount(); i++)
        printPostRow(sorted[i], engine->getCurrentUser());

    cout << endl << left << setw(12) << "Algorithm" << setw(14) << "Comparisons"
         << setw(10) << "Swaps" << "Passes" << endl;

    for (int a = first; a <= last; a++)
        cout << setw(12) << names[a] << setw(14) << stats[a].comparisons
             << setw(10) << stats[a].swaps << stats[a].passes << endl;

    cout << YELLOW << "(Your real post list is not changed; a copy was sorted.)" << RESET << endl;
}

// ===== NOTIFICATIONS =====

inline void notificationsScreen(FeedEngine* engine) {

    printBox("NOTIFICATIONS (OLDEST FIRST)");

    User* user = engine->getCurrentUser();
    const Queue<string>& notes = user->getNotifications();

    if (notes.isEmpty()) {
        cout << "No new notifications.\n";
        return;
    }

    for (int i = 0; i < notes.size(); i++)
        cout << YELLOW << i + 1 << ". " << RESET << notes.at(i) << endl;

    user->clearNotifications();
    printSuccess("All notifications marked as read.");
}

// ===== FRIENDS (GRAPH) =====

inline void followScreen(FeedEngine* engine, bool follow) {

    User* me = engine->getCurrentUser();

    cout << "Users: ";

    for (DoublyLinkedList<User*>::Node* curr = engine->getUsers().getHead(); curr != nullptr; curr = curr->next) {

        if (curr->data == me)
            continue;

        cout << "@" << curr->data->getUsername();

        if (engine->isFollowing(me->getUsername(), curr->data->getUsername()))
            cout << GREEN << " (following)" << RESET;

        cout << "  ";
    }

    cout << endl;

    string username = readUsername(follow ? "Follow username: " : "Unfollow username: ");

    if (follow) {
        if (reportResult(engine->follow(username)))
            printSuccess("You are now following @" + username);
    }
    else {
        if (reportResult(engine->unfollow(username)))
            printSuccess("You unfollowed @" + username);
    }
}

inline void suggestionsScreen(FeedEngine* engine) {

    printBox("PEOPLE YOU MAY KNOW");

    PriorityQueue<Suggestion, MoreMutual> suggestions = engine->suggestionsFor(engine->getCurrentUser()->getUsername());

    if (suggestions.isEmpty()) {
        cout << "No suggestions yet. Follow someone first: their friends will show up here.\n";
        return;
    }

    for (DoublyLinkedList<Suggestion>::Node* curr = suggestions.getHead(); curr != nullptr; curr = curr->next)
        cout << "@" << left << setw(20) << curr->data.username << YELLOW << curr->data.mutual
             << " mutual follow(s)" << RESET << endl;

    cout << YELLOW << "\n(Users exactly 2 steps away in the follow graph, found with BFS)" << RESET << endl;
}

inline void connectionScreen(FeedEngine* engine) {

    printBox("HOW ARE YOU CONNECTED?");

    string me = engine->getCurrentUser()->getUsername();
    string other = readUsername("Other username: ");

    if (engine->findUser(other) == nullptr) {
        printError("NO ACCOUNT WITH THAT USERNAME!");
        return;
    }

    DoublyLinkedList<string> path = engine->connectionPath(me, other);

    if (path.isEmpty()) {
        cout << "No follow chain from @" << me << " to @" << other << ".\n";
        return;
    }

    for (DoublyLinkedList<string>::Node* curr = path.getHead(); curr != nullptr; curr = curr->next) {
        cout << GREEN << "@" << curr->data << RESET;

        if (curr->next != nullptr)
            cout << " -> ";
    }

    cout << endl << path.size() - 1 << " step(s) (shortest path, BFS)" << endl;
}

inline void friendsScreen(FeedEngine* engine) {

    while (true) {

        printBox("FRIENDS (FOLLOW GRAPH)");

        cout << "1. Follow a user\n";
        cout << "2. Unfollow a user\n";
        cout << "3. Following & followers\n";
        cout << "4. Following feed\n";
        cout << "5. People you may know\n";
        cout << "6. How are you connected?\n";
        cout << "7. Back\n";

        int choice = readInt("Enter choice: ", 1, 7);
        string me = engine->getCurrentUser()->getUsername();

        if (choice == 1) {
            followScreen(engine, true);
        }
        else if (choice == 2) {
            followScreen(engine, false);
        }
        else if (choice == 3) {
            cout << "Following : " << joinNames(engine->followingOf(me)) << endl;
            cout << "Followers : " << joinNames(engine->followersOf(me)) << endl;
        }
        else if (choice == 4) {

            printBox("FOLLOWING FEED");
            DoublyLinkedList<Post*> posts = engine->followingFeed(me);

            if (posts.isEmpty())
                cout << "No posts from people you follow yet.\n";
            else
                printPostList(posts, engine->getCurrentUser());
        }
        else if (choice == 5) {
            suggestionsScreen(engine);
        }
        else if (choice == 6) {
            connectionScreen(engine);
        }
        else {
            return;
        }
    }
}

#endif

// ==================================================================
// main.cpp
// ==================================================================
// FeedEngine - DSA Version (CT-159 CCA)
// Alisha Naushad (CT-169), Syeda Zainab (CT-171), Abdul Raheem Sheikh (CT-192)
//
// START READING HERE.
// main() loads the saved files, then shows one of two menus in a loop:
//   - the account menu when nobody is logged in (Login / Sign Up / Exit)
//   - the main menu when someone is logged in (16 options)
// Each option calls one screen from Screens.h, and the screen asks
// FeedEngine (the brain) to do the work.


// Shown when nobody is logged in. Returns false when the user picks Exit.
bool accountMenu(FeedEngine* engine) {

    printBox("FEEDENGINE - WELCOME");

    cout << "1. Login\n";
    cout << "2. Sign Up\n";
    cout << "3. Exit\n";

    int choice = readInt("Enter choice: ", 1, 3);

    if (choice == 1)
        loginScreen(engine);
    else if (choice == 2)
        signUpScreen(engine);
    else
        return false;

    return true;
}

// Shown to a logged-in user. Returns false on Save & Exit.
bool mainMenu(FeedEngine* engine) {

    User* user = engine->getCurrentUser();
    int unread = user->getNotifications().size();

    printBox("MAIN MENU");

    cout << "Logged in as " << GREEN << "@" << user->getUsername() << RESET;

    if (unread > 0)
        cout << YELLOW << "   (" << unread << " new notification(s))" << RESET;

    cout << "\n\n";
    cout << " 1. View Profile\n";
    cout << " 2. View All Posts\n";
    cout << " 3. Scroll Feed (Next / Previous)\n";
    cout << " 4. Like a Post\n";
    cout << " 5. Undo Last Like\n";
    cout << " 6. Recently Liked\n";
    cout << " 7. Recommended For You\n";
    cout << " 8. Top Trending Posts\n";
    cout << " 9. Search Posts\n";
    cout << "10. Sort Posts\n";
    cout << "11. Title Index (A-Z, AVL Tree)\n";
    cout << "12. Friends (Follow / Suggestions)\n";
    cout << "13. Notifications\n";
    cout << "14. Create a Post\n";
    cout << "15. Logout\n";
    cout << "16. Save & Exit\n";

    int choice = readInt("Enter choice: ", 1, 16);

    switch (choice) {
        case 1:  profileScreen(engine); break;
        case 2:  allPostsScreen(engine); break;
        case 3:  scrollFeedScreen(engine); break;
        case 4:  likeScreen(engine); break;
        case 5:  undoLikeScreen(engine); break;
        case 6:  recentlyLikedScreen(engine); break;
        case 7:  recommendationsScreen(engine); break;
        case 8:  trendingScreen(engine); break;
        case 9:  searchScreen(engine); break;
        case 10: sortScreen(engine); break;
        case 11: titleIndexScreen(engine); break;
        case 12: friendsScreen(engine); break;
        case 13: notificationsScreen(engine); break;
        case 14: createPostScreen(engine); break;

        case 15:
            engine->logout();

            if (FileManager::saveAll(*engine))
                printSuccess("Logged out. Data saved.");
            else
                printError("Logged out, but the data could not be saved!");
            break;

        default:
            return false;
    }

    return true;
}

int main() {

    FeedEngine* engine = FeedEngine::getInstance();

    FileManager::loadAll(*engine);

    try {
        bool running = true;

        while (running) {
            if (engine->getCurrentUser() == nullptr)
                running = accountMenu(engine);
            else
                running = mainMenu(engine);
        }
    }
    catch (const InputEnded&) {
        cout << endl;
        printError("Input ended.");
    }

    if (FileManager::saveAll(*engine))
        printSuccess("Data saved to posts.txt and users.txt");
    else
        printError("COULD NOT SAVE DATA!");

    cout << GREEN << "Exiting Program...\n" << RESET;

    return 0;
}
