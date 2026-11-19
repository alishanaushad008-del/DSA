#ifndef SCREENS_H
#define SCREENS_H

#include <iostream>
#include <iomanip>
#include <string>

#include "FeedEngine.h"
#include "Input.h"
#include "Functions.h"

using namespace std;


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
