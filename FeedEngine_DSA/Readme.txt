FeedEngine - DSA Version (CT-159 CCA)

New to the code? Read HOW_IT_WORKS.txt first: it explains everything in simple words.

The OOP FeedEngine project rebuilt on our own data structures.
Every structure and algorithm comes from the CT-159 course plan, and no
STL containers are used (std::string only).


1. How to Compile and Run

Open a terminal in the FeedEngine_DSA folder:

    g++ main.cpp -o FeedEngine
    FeedEngine

Tests (every line should say PASS):

    g++ tests/ds_tests.cpp -o ds_tests
    ds_tests

    g++ tests/feed_tests.cpp -o feed_tests
    feed_tests

Data is saved to posts.txt and users.txt in the same folder.
With no saved posts, the program starts with the 13 posts from the OOP version.

One-file versions (same code, for Dev-C++ or an online compiler) are in single_file/:

    g++ single_file/FeedEngine.cpp -o FeedEngine
    g++ single_file/FeedEngine_Tests.cpp -o FeedEngine_Tests

Dev-C++: open the .cpp file and press F11. If it reports errors about
nullptr or enum class, add -std=c++11 under Tools > Compiler Options.


2. Folder Layout

FeedEngine_DSA/
    main.cpp            account menu + main menu
    FeedEngine.h        the engine (Singleton): all the logic, no printing
    Screens.h           everything the user sees
    FileManager.h       save / load posts.txt and users.txt
    Input.h             input validation
    Post.h, User.h, Content.h, Functions.h
    DataStructures/     LinkedList, Stack, Queue, PriorityQueue, MaxHeap,
                        HashTable, Graph, AVLTree, HigherFirst
    Algorithms/         Sorting (6 sorts), Searching (linear + binary)
    tests/              ds_tests.cpp, feed_tests.cpp


3. Data Structures and Algorithms

Week  Structure / Algorithm        Used for                                 Cost
----  ---------------------------  ---------------------------------------  -----------------
1     Linear search                Search by keyword, compare with binary   O(n)
1     Binary search (+ recursive)  Search by ID on posts[] sorted by ID     O(log n)
2-3   Doubly linked list           Feed: scroll Next / Previous             O(1) per step
4     Stack                        Recently liked + Undo Like               O(1) push / pop
5     Circular queue               Notifications (newest 10 kept)           O(1)
5     Priority queue (sorted list) Recommendations, People you may know     O(n) insert
6     Selection / Bubble /         Sort Posts menu (compare all six)        O(n^2)
      Insertion sort
7     Merge sort, Quick sort       Sort Posts menu                          O(n log n)
10    Binary max-heap              Top Trending (build O(n), take top 5)    O(n + k log n)
10    Heap sort                    Sort Posts menu                          O(n log n)
9,11  BST: in/pre/post/level-     Title Index: all posts A-Z,              O(log n) search
      order traversal, insert,     search by start of title                 O(log n + k) prefix
      search, delete (3 cases)
12    AVL tree (LL, RR, LR, RL     Keeps the title index balanced; the      height ~ log2(n)
      rotations)                   screen compares it with a plain BST
13    Graph (adjacency list)       Follow / unfollow, followers             O(degree)
13    BFS                          People you may know (2 steps away)       O(V + E)
13    BFS shortest path            "How are you connected?"                 O(V + E)
13    DFS                          Graph traversal (in Graph.h + tests)     O(V + E)
15    Hash table (chaining,        Find post by ID, find user by name       O(1) average
      rehash at load 0.75)


4. Bugs Fixed From the OOP Version

- "Recently liked" showed only one post         -> a Stack keeps every like
- Trending reordered the real post list         -> a max-heap; sorting uses a copy
- posts.txt was saved but never loaded          -> FileManager loads everything
- Liking searched the whole list                -> hash table lookup
- Interest "a" matched almost every category    -> needs 3+ letters to match a prefix


5. Main Menu

 1 Profile            7 Recommended         13 Notifications
 2 All Posts          8 Top Trending        14 Create a Post
 3 Scroll Feed        9 Search Posts        15 Logout
 4 Like a Post       10 Sort Posts          16 Save & Exit
 5 Undo Last Like    11 Title Index (A-Z, AVL tree: browse, search, show tree)
 6 Recently Liked    12 Friends (follow, suggestions, path)


6. Possible Next Steps

- Minimum spanning tree / topological sort (week 14)
- Project report (week 15)
