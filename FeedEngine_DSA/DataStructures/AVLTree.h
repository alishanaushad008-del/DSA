#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdexcept>

#include "LinkedList.h"
#include "Queue.h"

using namespace std;



template <class K, class V>
class AVLTree {

public:

    struct Node {
        K key;//post title
        V value;
        Node* left;
        Node* right;
        int height;         // a leaf has height 1

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

private:

    Node* root;
    int count;//no of nodes
    bool selfBalancing;
    int rotations;

    static int heightOf(const Node* node) {//if node exits its height
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

   
    DoublyLinkedList<K> keysInOrder() const {
        DoublyLinkedList<K> out;
        inOrderAt(root, out);
        return out;
    }

  
    DoublyLinkedList<K> keysPreOrder() const {
        DoublyLinkedList<K> out;
        preOrderAt(root, out);
        return out;
    }

  
    DoublyLinkedList<K> keysPostOrder() const {
        DoublyLinkedList<K> out;
        postOrderAt(root, out);
        return out;
    }

    
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

  
    DoublyLinkedList<K> keysWithPrefix(const K& prefix) const {
        DoublyLinkedList<K> out;
        prefixAt(root, prefix, out);
        return out;
    }

  
    bool isValid() const {

        DoublyLinkedList<K> keys = keysInOrder();

        for (typename DoublyLinkedList<K>::Node* curr = keys.getHead(); curr != nullptr && curr->next != nullptr; curr = curr->next)
            if (!(curr->data < curr->next->data))
                return false;

        return keys.size() == count && heightsValid(root);
    }

    
    const Node* getRoot() const { return root; }

    
    int height() const { return heightOf(root); }

    int size() const { return count; }

    bool isEmpty() const { return count == 0; }

    bool isSelfBalancing() const { return selfBalancing; }

  
    int getRotationCount() const { return rotations; }

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }
};

#endif
