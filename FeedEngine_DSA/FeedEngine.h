#ifndef FEEDENGINE_H
#define FEEDENGINE_H

#include <string>

#include "Post.h"
#include "User.h"
#include "Functions.h"
#include "DataStructures/LinkedList.h"
#include "DataStructures/PriorityQueue.h"
#include "DataStructures/MaxHeap.h"
#include "DataStructures/HashTable.h"
#include "DataStructures/Graph.h"
#include "DataStructures/AVLTree.h"
#include "Algorithms/Sorting.h"
#include "Algorithms/Searching.h"

using namespace std;


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
