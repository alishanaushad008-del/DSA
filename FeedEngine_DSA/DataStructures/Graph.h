#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h"
#include "HashTable.h"
#include "Queue.h"
#include "Stack.h"

using namespace std;



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
