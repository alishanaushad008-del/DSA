#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>

#include "LinkedList.h"

using namespace std;

inline unsigned int hashFunction(int key) {
    return static_cast<unsigned int>(key);
}

inline unsigned int hashFunction(const string& key) {

    unsigned int hash = 5381;

    for (int i = 0; i < static_cast<int>(key.length()); i++)
        hash = hash * 33 + static_cast<unsigned char>(key[i]);

    return hash;
}



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
