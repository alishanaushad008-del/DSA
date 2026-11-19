// Tests for the data structures in DataStructures/.
//
// From the FeedEngine_DSA folder:
//   g++ tests/ds_tests.cpp -o ds_tests
//   ds_tests

#include <iostream>
#include <string>
#include <stdexcept>

#include "../Functions.h"
#include "../DataStructures/LinkedList.h"
#include "../DataStructures/Stack.h"
#include "../DataStructures/Queue.h"
#include "../DataStructures/PriorityQueue.h"
#include "../DataStructures/MaxHeap.h"
#include "../DataStructures/HashTable.h"
#include "../DataStructures/Graph.h"
#include "../DataStructures/AVLTree.h"
#include "../Algorithms/Sorting.h"
#include "../Algorithms/Searching.h"

using namespace std;

// Stops the current test and prints which check failed
#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            cout << RED << "    failed: " << #condition \
                 << " (line " << __LINE__ << ")" << RESET << endl; \
            return false; \
        } \
    } while (0)

// Passes only if the expression throws this exception type
#define CHECK_THROWS(expression, errorType) \
    do { \
        bool thrown = false; \
        try { expression; } catch (const errorType&) { thrown = true; } \
        CHECK(thrown); \
    } while (0)

// True when the list holds exactly expected[0..n-1],
// walking forwards with next and backwards with prev
template <class T>
bool listEquals(const DoublyLinkedList<T>& list, const T* expected, int n) {

    if (list.size() != n)
        return false;

    int i = 0;

    for (typename DoublyLinkedList<T>::Node* curr = list.getHead(); curr != nullptr; curr = curr->next) {
        if (i >= n || !(curr->data == expected[i]))
            return false;
        i++;
    }

    if (i != n)
        return false;

    for (typename DoublyLinkedList<T>::Node* curr = list.getTail(); curr != nullptr; curr = curr->prev) {
        i--;
        if (i < 0 || !(curr->data == expected[i]))
            return false;
    }

    return i == 0;
}

// ===== DOUBLY LINKED LIST =====

bool testLinkedList() {

    DoublyLinkedList<int> list;

    CHECK(list.isEmpty());
    CHECK(list.getHead() == nullptr && list.getTail() == nullptr);

    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtHead(1);

    int a[] = {1, 2, 3};
    CHECK(listEquals(list, a, 3));

    // insert in the middle and at the head with insertBefore
    list.insertBefore(list.search(3), 25);
    list.insertBefore(list.getHead(), 0);

    int b[] = {0, 1, 2, 25, 3};
    CHECK(listEquals(list, b, 5));

    CHECK(list.search(25) != nullptr);
    CHECK(list.search(99) == nullptr);

    CHECK(list.deleteValue(25));
    CHECK(!list.deleteValue(99));
    CHECK(list.deleteFromHead() == 0);
    CHECK(list.deleteFromTail() == 3);

    int c[] = {1, 2};
    CHECK(listEquals(list, c, 2));

    // delete a node we already hold, then the last node
    list.deleteNode(list.getTail());
    CHECK(list.size() == 1 && list.getHead() == list.getTail());
    CHECK(list.deleteFromTail() == 1);
    CHECK(list.isEmpty() && list.getHead() == nullptr && list.getTail() == nullptr);

    for (int i = 1; i <= 5; i++)
        list.insertAtTail(i);

    list.reverse();

    int d[] = {5, 4, 3, 2, 1};
    CHECK(listEquals(list, d, 5));

    // deep copy: changing the copy must not change the original
    DoublyLinkedList<int> copy = list;
    copy.insertAtTail(0);
    CHECK(list.size() == 5 && copy.size() == 6);

    list = copy;
    DoublyLinkedList<int>& same = list;
    list = same;

    int e[] = {5, 4, 3, 2, 1, 0};
    CHECK(listEquals(list, e, 6));

    // post titles
    DoublyLinkedList<string> titles;
    titles.insertAtTail("Football Highlights");
    titles.insertAtTail("AI Revolution");
    titles.insertAtHead("Study Tips");

    string t[] = {"Study Tips", "Football Highlights", "AI Revolution"};
    CHECK(listEquals(titles, t, 3));
    CHECK(titles.search("AI Revolution") == titles.getTail());

    list.clear();
    CHECK(list.isEmpty() && list.size() == 0);

    return true;
}

// ===== STACK =====

bool testStack() {

    Stack<int> likes;   // recently liked post IDs

    CHECK(likes.isEmpty());

    likes.push(3);
    likes.push(7);
    likes.push(5);

    CHECK(likes.size() == 3);
    CHECK(likes.peek() == 5);

    // walk top -> bottom without popping
    int expected[] = {5, 7, 3};
    int i = 0;

    for (Stack<int>::Node* curr = likes.getTop(); curr != nullptr; curr = curr->next) {
        CHECK(i < 3 && curr->data == expected[i]);
        i++;
    }

    CHECK(i == 3);

    // undo last like
    CHECK(likes.pop() == 5);
    CHECK(likes.peek() == 7);
    CHECK(likes.size() == 2);

    // deep copy keeps the same order
    Stack<int> copy = likes;
    copy.push(9);
    CHECK(likes.size() == 2 && likes.peek() == 7);
    CHECK(copy.pop() == 9 && copy.pop() == 7 && copy.pop() == 3);
    CHECK(copy.isEmpty());

    likes = copy;
    CHECK(likes.isEmpty());

    for (int k = 0; k < 100; k++)
        likes.push(k);

    for (int k = 99; k >= 0; k--)
        CHECK(likes.pop() == k);

    CHECK(likes.isEmpty() && likes.size() == 0);

    likes.push(1);
    likes.clear();
    CHECK(likes.isEmpty() && likes.getTop() == nullptr);

    return true;
}

// ===== CIRCULAR QUEUE =====

bool testQueue() {

    Queue<int> q(3);

    CHECK(q.isEmpty() && !q.isFull() && q.getCapacity() == 3);

    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    CHECK(q.isFull() && q.size() == 3);
    CHECK(q.dequeue() == 1);

    // circular: the slot freed at the front is reused by the rear
    q.enqueue(4);
    CHECK(q.isFull());
    CHECK(q.at(0) == 2 && q.at(1) == 3 && q.at(2) == 4);
    CHECK(q.peek() == 2);

    // deep copy
    Queue<int> copy = q;
    CHECK(copy.dequeue() == 2);
    CHECK(q.size() == 3 && copy.size() == 2);

    CHECK(q.dequeue() == 2);
    CHECK(q.dequeue() == 3);
    CHECK(q.dequeue() == 4);
    CHECK(q.isEmpty());

    // go round the array many times
    int next = 0;
    int expected = 0;

    for (int round = 0; round < 50; round++) {
        q.enqueue(next++);
        q.enqueue(next++);
        CHECK(q.dequeue() == expected++);
        CHECK(q.dequeue() == expected++);
    }

    CHECK(q.isEmpty());

    // notifications: keep only the newest 3
    Queue<string> notifications(3);

    string messages[] = {
        "Sara liked your post",
        "Omar liked your post",
        "Hina liked your post",
        "Bilal liked your post"
    };

    for (int k = 0; k < 4; k++) {
        if (notifications.isFull())
            notifications.dequeue();   // drop the oldest

        notifications.enqueue(messages[k]);
    }

    CHECK(notifications.size() == 3);
    CHECK(notifications.peek() == messages[1]);
    CHECK(notifications.at(2) == messages[3]);

    // assignment between queues of different capacity
    Queue<int> big(8);
    big.enqueue(42);
    copy = big;
    CHECK(copy.getCapacity() == 8 && copy.size() == 1 && copy.peek() == 42);
    CHECK(big.dequeue() == 42 && copy.size() == 1);

    q.clear();
    CHECK(q.isEmpty() && q.size() == 0);

    return true;
}

// ===== PRIORITY QUEUE =====

struct FakePost {
    int id;
    int likes;
};

// More likes = higher priority (Trending)
struct FakeMoreLikes {

    bool operator()(const FakePost& a, const FakePost& b) const {
        return a.likes > b.likes;
    }
};

bool testPriorityQueue() {

    PriorityQueue<int> pq;   // default: bigger number first

    pq.enqueue(5);
    pq.enqueue(1);
    pq.enqueue(9);
    pq.enqueue(3);
    pq.enqueue(7);

    CHECK(pq.size() == 5 && pq.peek() == 9);
    CHECK(pq.dequeue() == 9);
    CHECK(pq.dequeue() == 7);
    CHECK(pq.dequeue() == 5);
    CHECK(pq.dequeue() == 3);
    CHECK(pq.dequeue() == 1);
    CHECK(pq.isEmpty());

    // posts with equal likes keep their arrival order (FIFO)
    PriorityQueue<FakePost, FakeMoreLikes> trending;

    FakePost posts[] = { {1, 4}, {2, 9}, {3, 4}, {4, 0}, {5, 9} };

    for (int k = 0; k < 5; k++)
        trending.enqueue(posts[k]);

    int expectedIds[] = {2, 5, 1, 3, 4};
    int i = 0;

    for (DoublyLinkedList<FakePost>::Node* curr = trending.getHead(); curr != nullptr; curr = curr->next) {
        CHECK(i < 5 && curr->data.id == expectedIds[i]);
        i++;
    }

    CHECK(i == 5);
    CHECK(trending.dequeue().id == 2);
    CHECK(trending.peek().id == 5);

    // many pseudo-random values must come out in non-increasing order
    PriorityQueue<int> many;
    unsigned int seed = 12345;

    for (int k = 0; k < 60; k++) {
        seed = seed * 1103515245u + 12345u;
        many.enqueue(static_cast<int>((seed >> 16) % 1000));
    }

    int previous = many.dequeue();
    int taken = 1;

    while (!many.isEmpty()) {
        int value = many.dequeue();
        CHECK(value <= previous);
        previous = value;
        taken++;
    }

    CHECK(taken == 60);

    many.enqueue(1);
    many.clear();
    CHECK(many.isEmpty() && many.size() == 0);

    // take out an item from the middle
    PriorityQueue<int> small;
    small.enqueue(4);
    small.enqueue(8);
    small.enqueue(6);
    CHECK(small.remove(8));
    CHECK(!small.remove(99));
    CHECK(small.size() == 2 && small.peek() == 6);

    return true;
}

// ===== MAX-HEAP =====

// Most likes first; equal likes -> smaller ID first
struct MoreLikesThenOlder {

    bool operator()(const FakePost& a, const FakePost& b) const {
        return a.likes > b.likes || (a.likes == b.likes && a.id < b.id);
    }
};

// Every parent must come out before (or equal to) its children
bool isMaxHeap(const MaxHeap<int>& heap) {

    for (int i = 1; i < heap.size(); i++)
        if (heap.at((i - 1) / 2) < heap.at(i))
            return false;

    return true;
}

bool testMaxHeap() {

    MaxHeap<int> heap(2);   // small, so it has to grow

    CHECK(heap.isEmpty());

    int values[] = {5, 1, 9, 3, 7, 9, 2};

    for (int i = 0; i < 7; i++) {
        heap.insert(values[i]);
        CHECK(isMaxHeap(heap));
    }

    CHECK(heap.size() == 7 && heap.peek() == 9);

    int expected[] = {9, 9, 7, 5, 3, 2, 1};

    for (int i = 0; i < 7; i++) {
        CHECK(heap.extractMax() == expected[i]);
        CHECK(isMaxHeap(heap));
    }

    CHECK(heap.isEmpty());

    // Floyd's build-heap
    int data[] = {4, 10, 3, 5, 1, 8, 6};
    heap.buildFrom(data, 7);
    CHECK(heap.size() == 7 && heap.peek() == 10);
    CHECK(isMaxHeap(heap));

    int previous = heap.extractMax();

    while (!heap.isEmpty()) {
        int value = heap.extractMax();
        CHECK(value <= previous);
        previous = value;
    }

    // deep copy
    heap.insert(3);
    MaxHeap<int> copy = heap;
    copy.insert(50);
    CHECK(heap.size() == 1 && heap.peek() == 3);
    CHECK(copy.size() == 2 && copy.peek() == 50);

    heap = copy;
    CHECK(heap.size() == 2 && heap.peek() == 50);

    // trending: most likes first, ties broken by the older post
    MaxHeap<FakePost, MoreLikesThenOlder> trending;
    FakePost posts[] = { {1, 4}, {2, 9}, {3, 4}, {4, 0}, {5, 9} };
    trending.buildFrom(posts, 5);

    int expectedIds[] = {2, 5, 1, 3, 4};

    for (int i = 0; i < 5; i++)
        CHECK(trending.extractMax().id == expectedIds[i]);

    // many pseudo-random values
    unsigned int seed = 777;

    for (int k = 0; k < 100; k++) {
        seed = seed * 1103515245u + 12345u;
        heap.insert(static_cast<int>((seed >> 16) % 1000));
    }

    CHECK(heap.size() == 102 && isMaxHeap(heap));

    previous = heap.extractMax();

    while (!heap.isEmpty()) {
        int value = heap.extractMax();
        CHECK(value <= previous);
        previous = value;
    }

    heap.insert(1);
    heap.clear();
    CHECK(heap.isEmpty());

    return true;
}

// ===== HASH TABLE =====

bool testHashTable() {

    HashTable<int, int> squares(3);     // only 3 buckets, so it must rehash

    CHECK(squares.isEmpty() && squares.getBucketCount() == 3);

    for (int i = 0; i < 100; i++)
        squares.insert(i, i * i);

    CHECK(squares.size() == 100);
    CHECK(squares.size() * 4 <= squares.getBucketCount() * 3);    // load factor <= 0.75

    for (int i = 0; i < 100; i++) {
        const int* value = squares.find(i);
        CHECK(value != nullptr && *value == i * i);
    }

    CHECK(squares.find(100) == nullptr);
    CHECK(!squares.contains(-1));

    // insert on an existing key updates it
    squares.insert(5, -1);
    CHECK(squares.size() == 100 && squares.get(5, 0) == -1);

    CHECK(squares.remove(5));
    CHECK(!squares.remove(5));
    CHECK(!squares.contains(5) && squares.size() == 99);

    squares.insert(-7, 49);
    CHECK(squares.get(-7, 0) == 49);

    // rehashing must keep keys that share a bucket:
    // multiples of 7 collide a lot once there are 63 buckets
    HashTable<int, int> multiples(3);

    for (int i = 0; i < 60; i++)
        multiples.insert(i * 7, i);

    CHECK(multiples.size() == 60);

    for (int i = 0; i < 60; i++)
        CHECK(multiples.get(i * 7, -1) == i);

    // many string keys: "useraa", "userba", ...
    HashTable<string, int> manyUsers(2);

    for (int i = 0; i < 60; i++) {
        string name = "user";
        name += static_cast<char>('a' + i % 26);
        name += static_cast<char>('a' + i / 26);
        manyUsers.insert(name, i);
    }

    CHECK(manyUsers.size() == 60);
    CHECK(manyUsers.get("useraa", -1) == 0 && manyUsers.get("userhc", -1) == 59);

    // every entry sits in exactly one bucket
    int total = 0;

    for (int b = 0; b < squares.getBucketCount(); b++)
        total += squares.bucketLength(b);

    CHECK(total == squares.size());
    CHECK(squares.keys().size() == squares.size());

    // collisions: with 7 buckets, 1 and 8 both land in bucket 1
    HashTable<int, int> small(7);
    small.insert(1, 10);
    small.insert(8, 80);
    CHECK(small.getBucketCount() == 7 && small.bucketLength(1) == 2);
    CHECK(small.get(1, 0) == 10 && small.get(8, 0) == 80);
    CHECK(small.stepsToFind(8) == 1);     // newest entry is at the front of the chain
    CHECK(small.stepsToFind(1) == 2);
    CHECK(small.stepsToFind(15) == 2);    // missing key: whole chain checked
    CHECK(small.stepsToFind(3) == 0);     // empty bucket

    // string keys (usernames)
    HashTable<string, int> users;
    users.insert("ali", 1);
    users.insert("sara", 2);
    users.insert("omar", 3);

    CHECK(users.get("sara", 0) == 2 && users.get("hina", 0) == 0);
    CHECK(hashFunction(string("ab")) != hashFunction(string("ba")));

    // operator[] adds missing keys (counting words)
    HashTable<string, int> counts;
    string words[] = {"tech", "sports", "tech", "news", "tech"};

    for (int i = 0; i < 5; i++)
        counts[words[i]]++;

    CHECK(counts.size() == 3);
    CHECK(counts.get("tech", 0) == 3 && counts.get("sports", 0) == 1);

    // values can be lists: category -> post IDs
    HashTable<string, DoublyLinkedList<int> > byCategory;
    byCategory["Sports"].insertAtTail(1);
    byCategory["Sports"].insertAtTail(7);
    byCategory["News"].insertAtTail(13);
    CHECK(byCategory.size() == 2 && byCategory["Sports"].size() == 2);

    // deep copy
    HashTable<string, int> copy = users;
    copy.insert("bilal", 4);
    copy.remove("ali");
    CHECK(users.size() == 3 && users.contains("ali"));
    CHECK(copy.size() == 3 && !copy.contains("ali"));

    users = copy;
    CHECK(users.contains("bilal") && !users.contains("ali"));

    users.clear();
    CHECK(users.isEmpty() && !users.contains("sara"));

    return true;
}

// ===== GRAPH =====

bool testGraph() {

    Graph<string> g(true, 2);   // directed (a -> b = "a follows b"), small so it grows

    string names[] = {"ali", "sara", "omar", "zara", "hina", "bilal"};

    for (int i = 0; i < 6; i++)
        CHECK(g.addVertex(names[i]));

    CHECK(!g.addVertex("ali"));
    CHECK(g.getVertexCount() == 6 && g.isDirected());

    CHECK(g.addEdge("ali", "sara"));
    CHECK(g.addEdge("ali", "omar"));
    CHECK(g.addEdge("sara", "zara"));
    CHECK(g.addEdge("omar", "zara"));
    CHECK(g.addEdge("omar", "hina"));
    CHECK(g.addEdge("zara", "bilal"));

    CHECK(!g.addEdge("ali", "sara"));      // already there
    CHECK(!g.addEdge("ali", "ali"));       // no self loop
    CHECK(!g.addEdge("ali", "nobody"));    // missing vertex
    CHECK(g.getEdgeCount() == 6);
    CHECK(g.hasEdge("ali", "sara") && !g.hasEdge("sara", "ali"));

    string aliFollows[] = {"sara", "omar"};
    CHECK(listEquals(g.neighbours("ali"), aliFollows, 2));

    string zaraFollowers[] = {"sara", "omar"};
    CHECK(listEquals(g.incoming("zara"), zaraFollowers, 2));

    string bfsOrder[] = {"ali", "sara", "omar", "zara", "hina", "bilal"};
    CHECK(listEquals(g.breadthFirst("ali"), bfsOrder, 6));

    string dfsOrder[] = {"ali", "sara", "zara", "bilal", "omar", "hina"};
    CHECK(listEquals(g.depthFirst("ali"), dfsOrder, 6));

    string path[] = {"ali", "sara", "zara", "bilal"};
    CHECK(listEquals(g.shortestPath("ali", "bilal"), path, 4));
    CHECK(g.distance("ali", "bilal") == 3);
    CHECK(g.distance("bilal", "ali") == -1);
    CHECK(g.distance("ali", "ali") == 0);

    string friendsOfFriends[] = {"zara", "hina"};
    CHECK(listEquals(g.verticesAtDistance("ali", 2), friendsOfFriends, 2));

    CHECK(g.removeEdge("ali", "omar"));
    CHECK(!g.removeEdge("ali", "omar"));

    string afterUnfollow[] = {"zara"};
    CHECK(listEquals(g.verticesAtDistance("ali", 2), afterUnfollow, 1));
    CHECK(g.breadthFirst("nobody").isEmpty());

    // undirected graph
    Graph<int> roads(false);

    for (int i = 1; i <= 4; i++)
        roads.addVertex(i);

    roads.addEdge(1, 2);
    roads.addEdge(2, 3);
    CHECK(roads.hasEdge(2, 1) && roads.getEdgeCount() == 2);
    CHECK(roads.distance(3, 1) == 2 && roads.distance(1, 4) == -1);
    CHECK(roads.removeEdge(2, 1) && !roads.hasEdge(1, 2));

    // a long chain 0 -> 1 -> ... -> 29
    Graph<int> chain;

    for (int i = 0; i < 30; i++)
        chain.addVertex(i);

    for (int i = 0; i < 29; i++)
        chain.addEdge(i, i + 1);

    CHECK(chain.getVertexCount() == 30 && chain.distance(0, 29) == 29);

    g.clear();
    CHECK(g.getVertexCount() == 0 && !g.hasVertex("ali") && g.getEdgeCount() == 0);

    return true;
}

// ===== BST / AVL TREE =====

bool testAVLTree() {

    // 1..7 inserted in order: the AVL tree rotates itself into a perfect tree
    AVLTree<int, int> avl;
    CHECK(avl.isEmpty() && avl.height() == 0);

    for (int i = 1; i <= 7; i++) {
        avl.insert(i, i * 10);
        CHECK(avl.isValid());
    }

    CHECK(avl.size() == 7 && avl.height() == 3);
    CHECK(avl.getRotationCount() == 4);
    CHECK(avl.getRoot()->key == 4);

    int inOrder[] = {1, 2, 3, 4, 5, 6, 7};
    int preOrder[] = {4, 2, 1, 3, 6, 5, 7};
    int postOrder[] = {1, 3, 2, 5, 7, 6, 4};
    int levelOrder[] = {4, 2, 6, 1, 3, 5, 7};

    CHECK(listEquals(avl.keysInOrder(), inOrder, 7));
    CHECK(listEquals(avl.keysPreOrder(), preOrder, 7));
    CHECK(listEquals(avl.keysPostOrder(), postOrder, 7));
    CHECK(listEquals(avl.keysLevelOrder(), levelOrder, 7));

    // the same keys in a plain BST make a chain of height 7
    AVLTree<int, int> bst(false);

    for (int i = 1; i <= 7; i++)
        bst.insert(i, 0);

    CHECK(bst.isValid() && bst.height() == 7 && bst.getRotationCount() == 0);
    CHECK(listEquals(bst.keysPreOrder(), inOrder, 7));

    // searching
    CHECK(avl.get(5, 0) == 50 && avl.get(8, -1) == -1);
    CHECK(avl.contains(1) && !avl.contains(0));
    CHECK(avl.searchSteps(4) == 1 && avl.searchSteps(1) == 3 && avl.searchSteps(8) == 3);
    CHECK(bst.searchSteps(7) == 7);
    CHECK(avl.minKey() == 1 && avl.maxKey() == 7);

    // insert on an existing key updates the value
    avl.insert(3, -3);
    CHECK(avl.size() == 7 && avl.get(3, 0) == -3);

    *avl.find(3) = 30;
    CHECK(avl.get(3, 0) == 30);

    // left-right and right-left cases need a double rotation
    AVLTree<int, int> lr;
    lr.insert(30, 0);
    lr.insert(10, 0);
    lr.insert(20, 0);
    CHECK(lr.getRoot()->key == 20 && lr.height() == 2 && lr.getRotationCount() == 2);

    AVLTree<int, int> rl;
    rl.insert(10, 0);
    rl.insert(30, 0);
    rl.insert(20, 0);
    CHECK(rl.getRoot()->key == 20 && rl.height() == 2 && rl.getRotationCount() == 2);

    // remove a leaf, a node with one child, then the root (two children)
    CHECK(avl.remove(1));
    CHECK(avl.isValid());
    CHECK(avl.remove(2));
    CHECK(avl.isValid());
    CHECK(avl.remove(4));                   // the successor 5 takes its place
    CHECK(avl.isValid() && avl.getRoot()->key == 5);

    int remaining[] = {3, 5, 6, 7};
    CHECK(listEquals(avl.keysInOrder(), remaining, 4));
    CHECK(!avl.remove(4));
    CHECK(avl.size() == 4 && avl.get(5, 0) == 50);

    // 100 sorted inserts stay about log2(100) tall, and removes keep the AVL rule
    AVLTree<int, int> big;

    for (int i = 1; i <= 100; i++)
        big.insert(i, i);

    CHECK(big.isValid() && big.height() == 7);

    for (int i = 2; i <= 100; i += 2) {
        CHECK(big.remove(i));
        CHECK(big.isValid());
    }

    CHECK(big.size() == 50 && big.minKey() == 1 && big.maxKey() == 99);

    // pseudo-random inserts and removes
    AVLTree<int, int> mixed;
    unsigned int seed = 2024;

    for (int k = 0; k < 300; k++) {

        seed = seed * 1103515245u + 12345u;
        int key = static_cast<int>((seed >> 16) % 100);

        if (k % 3 == 2)
            mixed.remove(key);
        else
            mixed.insert(key, k);
    }

    CHECK(mixed.isValid());

    // string keys: search by the start of a title
    AVLTree<string, int> titles;
    string names[] = {"football highlights", "fashion trends", "food", "ai revolution", "news update", "f"};

    for (int i = 0; i < 6; i++)
        titles.insert(names[i], i);

    CHECK(titles.isValid());

    string startsWithF[] = {"f", "fashion trends", "food", "football highlights"};
    CHECK(listEquals(titles.keysWithPrefix("f"), startsWithF, 4));

    string startsWithFo[] = {"food", "football highlights"};
    CHECK(listEquals(titles.keysWithPrefix("fo"), startsWithFo, 2));

    // matches on the right of smaller nodes, and on the left of bigger ones
    for (int i = 0; i < 6; i++) {
        string word = names[i].substr(0, 2);
        DoublyLinkedList<string> found = titles.keysWithPrefix(word);
        CHECK(found.search(names[i]) != nullptr);
    }

    string startsWithN[] = {"news update"};
    CHECK(listEquals(titles.keysWithPrefix("n"), startsWithN, 1));

    string startsWithA[] = {"ai revolution"};
    CHECK(listEquals(titles.keysWithPrefix("a"), startsWithA, 1));

    CHECK(titles.keysWithPrefix("z").isEmpty());
    CHECK(titles.keysWithPrefix("footballs").isEmpty());
    CHECK(titles.keysWithPrefix("").size() == 6);

    // values can be lists: title -> post IDs
    AVLTree<string, DoublyLinkedList<int> > index;
    index["study tips"].insertAtTail(5);
    index["study tips"].insertAtTail(14);
    index["ai revolution"].insertAtTail(2);
    CHECK(index.size() == 2 && index["study tips"].size() == 2);

    // deep copy
    AVLTree<string, int> copy = titles;
    copy.remove("food");
    copy.insert("zebra", 9);
    CHECK(titles.size() == 6 && titles.contains("food") && !titles.contains("zebra"));
    CHECK(copy.isValid() && copy.size() == 6 && !copy.contains("food"));

    titles = copy;
    CHECK(titles.contains("zebra") && titles.isValid());

    titles.clear();
    CHECK(titles.isEmpty() && titles.height() == 0 && titles.keysInOrder().isEmpty());

    return true;
}

// ===== SORTING =====

template <class T>
bool arrayEquals(const T* arr, const T* expected, int n) {

    for (int i = 0; i < n; i++)
        if (!(arr[i] == expected[i]))
            return false;

    return true;
}

bool idsInOrder(const FakePost* posts, const int* ids, int n) {

    for (int i = 0; i < n; i++)
        if (posts[i].id != ids[i])
            return false;

    return true;
}

struct FewerLikes {

    bool operator()(const FakePost& a, const FakePost& b) const {
        return a.likes < b.likes;
    }
};

// Runs sort number `which` (0 = selection ... 5 = heap), smallest first
SortStats runIntSort(int which, int* arr, int n) {

    switch (which) {
        case 0:  return selectionSort(arr, n);
        case 1:  return bubbleSort(arr, n);
        case 2:  return insertionSort(arr, n);
        case 3:  return mergeSort(arr, n);
        case 4:  return quickSort(arr, n);
        default: return heapSort(arr, n);
    }
}

bool testSorting() {

    for (int which = 0; which < 6; which++) {

        int a[] = {5, 1, 4, 2, 8, 0, 9, 3, 7, 6};
        int sortedA[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        runIntSort(which, a, 10);
        CHECK(arrayEquals(a, sortedA, 10));

        int dupes[] = {3, 1, 3, 2, 1};
        int sortedDupes[] = {1, 1, 2, 3, 3};
        runIntSort(which, dupes, 5);
        CHECK(arrayEquals(dupes, sortedDupes, 5));

        int one[] = {42};
        SortStats stats = runIntSort(which, one, 1);
        CHECK(one[0] == 42 && stats.comparisons == 0 && stats.swaps == 0);

        runIntSort(which, one, 0);      // empty array must not crash

        int big[80];
        unsigned int seed = 99 + which;

        for (int i = 0; i < 80; i++) {
            seed = seed * 1103515245u + 12345u;
            big[i] = static_cast<int>((seed >> 16) % 500);
        }

        runIntSort(which, big, 80);

        for (int i = 1; i < 80; i++)
            CHECK(big[i - 1] <= big[i]);
    }

    // already sorted: bubble and insertion are adaptive, selection is not
    int s1[] = {1, 2, 3, 4, 5};
    SortStats bubble = bubbleSort(s1, 5);
    CHECK(bubble.passes == 1 && bubble.comparisons == 4 && bubble.swaps == 0);

    SortStats insertion = insertionSort(s1, 5);
    CHECK(insertion.comparisons == 4 && insertion.swaps == 0);

    SortStats selection = selectionSort(s1, 5);
    CHECK(selection.comparisons == 10 && selection.swaps == 0);

    // quick sort's worst case: sorted input with the last item as pivot
    int s2[] = {1, 2, 3, 4, 5, 6, 7, 8};
    SortStats quick = quickSort(s2, 8);
    CHECK(quick.comparisons == 28 && quick.passes == 7);   // 7 + 6 + ... + 1

    // reversed: selection needs only 2 swaps, bubble swaps every pair
    int r1[] = {5, 4, 3, 2, 1};
    CHECK(selectionSort(r1, 5).swaps == 2);

    int r2[] = {5, 4, 3, 2, 1};
    SortStats reversedBubble = bubbleSort(r2, 5);
    CHECK(reversedBubble.swaps == 10 && reversedBubble.comparisons == 10);

    // stable sorts keep equal items in their original order
    int stableIds[] = {4, 1, 3, 2, 5};

    FakePost p1[] = { {1, 4}, {2, 9}, {3, 4}, {4, 0}, {5, 9} };
    bubbleSort(p1, 5, FewerLikes());
    CHECK(idsInOrder(p1, stableIds, 5));

    FakePost p2[] = { {1, 4}, {2, 9}, {3, 4}, {4, 0}, {5, 9} };
    insertionSort(p2, 5, FewerLikes());
    CHECK(idsInOrder(p2, stableIds, 5));

    FakePost p3[] = { {1, 4}, {2, 9}, {3, 4}, {4, 0}, {5, 9} };
    mergeSort(p3, 5, FewerLikes());
    CHECK(idsInOrder(p3, stableIds, 5));

    // strings and a custom order
    string names[] = {"sara", "Ali", "omar", "bilal"};
    string sortedNames[] = {"Ali", "bilal", "omar", "sara"};
    quickSort(names, 4);
    CHECK(arrayEquals(names, sortedNames, 4));

    int d[] = {3, 9, 1, 7};
    int descending[] = {9, 7, 3, 1};
    heapSort(d, 4, HigherFirst<int>());
    CHECK(arrayEquals(d, descending, 4));

    return true;
}

// ===== SEARCHING =====

struct FakeId {

    int operator()(const FakePost& post) const {
        return post.id;
    }
};

bool testSearching() {

    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    SearchResult binary = binarySearch(arr, 10, 23);
    CHECK(binary.index == 5 && binary.steps == 3);

    SearchResult recursive = binarySearchRecursive(arr, 10, 23);
    CHECK(recursive.index == 5 && recursive.steps == 3);

    SearchResult linear = linearSearch(arr, 10, 23);
    CHECK(linear.index == 5 && linear.steps == 6);

    for (int i = 0; i < 10; i++) {
        CHECK(binarySearch(arr, 10, arr[i]).index == i);
        CHECK(binarySearchRecursive(arr, 10, arr[i]).index == i);
        CHECK(linearSearch(arr, 10, arr[i]).index == i);
    }

    int missing[] = {1, 7, 50, 100};

    for (int i = 0; i < 4; i++) {
        CHECK(binarySearch(arr, 10, missing[i]).index == -1);
        CHECK(binarySearch(arr, 10, missing[i]).steps <= 4);    // log2(10) rounded up
        CHECK(binarySearchRecursive(arr, 10, missing[i]).index == -1);
        CHECK(linearSearch(arr, 10, missing[i]).steps == 10);
    }

    SearchResult empty = binarySearch(arr, 0, 5);
    CHECK(empty.index == -1 && empty.steps == 0);

    // search by a key: posts sorted by ID
    FakePost posts[] = { {3, 1}, {8, 0}, {15, 7}, {21, 2} };
    CHECK(binarySearch(posts, 4, 15, FakeId()).index == 2);
    CHECK(binarySearchRecursive(posts, 4, 4, FakeId()).index == -1);
    CHECK(linearSearch(posts, 4, 21, FakeId()).index == 3);

    return true;
}

// ===== OVERFLOW / UNDERFLOW =====

bool testErrors() {

    DoublyLinkedList<int> list;
    CHECK_THROWS(list.deleteFromHead(), underflow_error);
    CHECK_THROWS(list.deleteFromTail(), underflow_error);
    CHECK_THROWS(list.insertBefore(nullptr, 1), invalid_argument);
    CHECK_THROWS(list.deleteNode(nullptr), invalid_argument);

    Stack<int> stack;
    CHECK_THROWS(stack.pop(), underflow_error);
    CHECK_THROWS(stack.peek(), underflow_error);

    Queue<int> queue(1);
    CHECK_THROWS(queue.dequeue(), underflow_error);
    CHECK_THROWS(queue.peek(), underflow_error);
    queue.enqueue(1);
    CHECK_THROWS(queue.enqueue(2), overflow_error);
    CHECK_THROWS(queue.at(1), out_of_range);

    PriorityQueue<int> pq;
    CHECK_THROWS(pq.dequeue(), underflow_error);
    CHECK_THROWS(pq.peek(), underflow_error);

    MaxHeap<int> heap;
    CHECK_THROWS(heap.extractMax(), underflow_error);
    CHECK_THROWS(heap.peek(), underflow_error);
    CHECK_THROWS(heap.at(0), out_of_range);

    AVLTree<int, int> tree;
    CHECK_THROWS(tree.minKey(), underflow_error);
    CHECK_THROWS(tree.maxKey(), underflow_error);

    return true;
}

// Runs one test and prints PASS / FAIL. Returns 1 if it failed.
int runTest(string name, bool (*test)()) {

    bool passed = false;

    try {
        passed = test();
    }
    catch (const exception& e) {
        cout << RED << "    unexpected exception: " << e.what() << RESET << endl;
    }

    cout << "  " << name;

    for (int i = static_cast<int>(name.length()); i < 24; i++)
        cout << '.';

    if (passed)
        cout << GREEN << " PASS" << RESET << endl;
    else
        cout << RED << " FAIL" << RESET << endl;

    return passed ? 0 : 1;
}

int main() {

    printBox("DATA STRUCTURE TESTS");

    int failed = 0;

    failed += runTest("Doubly Linked List", testLinkedList);
    failed += runTest("Stack", testStack);
    failed += runTest("Circular Queue", testQueue);
    failed += runTest("Priority Queue", testPriorityQueue);
    failed += runTest("Max-Heap", testMaxHeap);
    failed += runTest("Hash Table", testHashTable);
    failed += runTest("Graph", testGraph);
    failed += runTest("BST / AVL Tree", testAVLTree);
    failed += runTest("Sorting (6 algorithms)", testSorting);
    failed += runTest("Searching", testSearching);
    failed += runTest("Overflow / Underflow", testErrors);

    if (failed == 0)
        printSuccess("All tests passed!");
    else
        printError(to_string(failed) + " test(s) failed");

    return failed;
}
