// Tests for Post, User, FeedEngine and FileManager.
//
// From the FeedEngine_DSA folder:
//   g++ tests/feed_tests.cpp -o feed_tests
//   feed_tests
//
// Nothing here touches the disk: FileManager is tested on text.

#include <iostream>
#include <string>
#include <stdexcept>

#include "../Functions.h"
#include "../FeedEngine.h"
#include "../FileManager.h"

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

// True when the list holds exactly these post IDs, in order
bool idsAre(const DoublyLinkedList<Post*>& list, const int* ids, int n) {

    if (list.size() != n)
        return false;

    int i = 0;

    for (DoublyLinkedList<Post*>::Node* curr = list.getHead(); curr != nullptr; curr = curr->next)
        if (curr->data->getId() != ids[i++])
            return false;

    return true;
}

bool namesAre(const DoublyLinkedList<string>& list, const string* names, int n) {

    if (list.size() != n)
        return false;

    int i = 0;

    for (DoublyLinkedList<string>::Node* curr = list.getHead(); curr != nullptr; curr = curr->next)
        if (curr->data != names[i++])
            return false;

    return true;
}

// ===== POST AND USER =====

bool testPostAndUser() {

    Post post(1, "Football Highlights", "Sports", "feedengine", -5);
    CHECK(post.getLikes() == 0);     // negative likes are not allowed

    post.like();
    post.like();
    CHECK(post.getLikes() == 2);

    post.unlike();
    post.unlike();
    post.unlike();
    CHECK(post.getLikes() == 0);

    User user("Ali_K", "Ali Khan", 20);
    CHECK(user.getUsername() == "ali_k");    // usernames are stored in lowercase

    user.addInterest("Tech");
    user.addInterest("sports");
    user.addInterest("TECH");                // already there, ignored
    CHECK(user.getInterests().size() == 2);

    CHECK(user.isInterestedIn("Technology"));
    CHECK(user.isInterestedIn("SPORTS"));
    CHECK(!user.isInterestedIn("Makeup"));

    // old bug: the interest "a" matched almost every category
    User picky("sara", "Sara", 19);
    picky.addInterest("a");
    CHECK(!picky.isInterestedIn("Fashion"));

    user.recordLike(3);
    user.recordLike(7);
    CHECK(user.hasLiked(3) && user.hasLiked(7) && !user.hasLiked(5));
    CHECK(user.undoLastLike() == 7);
    CHECK(!user.hasLiked(7));

    // only the newest 10 notifications are kept
    for (int k = 0; k < 12; k++)
        user.notify(string("note ") + static_cast<char>('a' + k));

    CHECK(user.getNotifications().size() == User::MAX_NOTIFICATIONS);
    CHECK(user.getNotifications().peek() == "note c");
    CHECK(user.getNotifications().at(9) == "note l");

    user.clearNotifications();
    CHECK(user.getNotifications().isEmpty());

    return true;
}

// ===== POSTS AND SEARCH =====

bool testPostsAndSearch() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    CHECK(engine->getPostCount() == 13);
    CHECK(engine->findPost(1)->getTitle() == "Football Highlights");
    CHECK(engine->findPost(13)->getTitle() == "News Update");
    CHECK(engine->findPost(14) == nullptr && engine->findPost(0) == nullptr);

    // the feed is newest first
    CHECK(engine->getFeed().getHead()->data->getId() == 13);
    CHECK(engine->getFeed().getTail()->data->getId() == 1);

    // every search finds the same post
    for (int id = 1; id <= 13; id++) {
        CHECK(engine->searchByIdBinary(id).index == id - 1);
        CHECK(engine->searchByIdRecursive(id).index == id - 1);
        CHECK(engine->searchByIdLinear(id).index == id - 1);
        CHECK(engine->searchByIdLinear(id).steps == id);
        CHECK(engine->searchByIdBinary(id).steps <= 4);
        CHECK(engine->hashStepsForId(id) >= 1);
    }

    CHECK(engine->searchByIdBinary(99).index == -1);
    CHECK(engine->hashStepsForId(99) <= 1);

    // keyword search, newest first
    int tech[] = {9, 2};
    CHECK(idsAre(engine->searchByKeyword("tech"), tech, 2));

    int tips[] = {5};
    CHECK(idsAre(engine->searchByKeyword("TIPS"), tips, 1));
    CHECK(engine->searchByKeyword("zzz").isEmpty());

    // only a logged-in user can post
    CHECK(engine->createPost("Hello", "News") == Result::NotLoggedIn);

    // posts[] only accepts growing IDs, so it stays sorted
    engine->reset();
    CHECK(engine->addPost(5, "Five", "News", "ali", 0));
    CHECK(!engine->addPost(3, "Three", "News", "ali", 0));
    CHECK(!engine->addPost(5, "Five again", "News", "ali", 0));
    CHECK(engine->addPost(9, "Nine", "News", "ali", 0));

    engine->signUp("ali", "Ali", 20);
    CHECK(engine->createPost("Ten", "News") == Result::Done);
    CHECK(engine->getPostAt(2)->getId() == 10);
    CHECK(engine->getPostAt(3) == nullptr && engine->getPostAt(-1) == nullptr);

    return true;
}

bool testFeedFull() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->signUp("ali", "Ali", 20);

    for (int i = 0; i < FeedEngine::MAX_POSTS; i++)
        CHECK(engine->createPost("Post", "News") == Result::Done);

    CHECK(engine->createPost("One too many", "News") == Result::FeedFull);
    CHECK(engine->getPostCount() == FeedEngine::MAX_POSTS);
    CHECK(engine->searchByIdBinary(150).index == 149);

    return true;
}

// ===== ACCOUNTS =====

bool testSignUpAndLogin() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    CHECK(engine->signUp("Ali", "Ali Khan", 20) == Result::Done);
    CHECK(engine->getCurrentUser() != nullptr);
    CHECK(engine->getCurrentUser()->getUsername() == "ali");

    CHECK(engine->signUp("ALI", "Someone", 30) == Result::UsernameTaken);
    CHECK(engine->getUsers().size() == 1);

    engine->logout();
    CHECK(engine->getCurrentUser() == nullptr);

    CHECK(engine->login("nobody") == Result::UserNotFound);
    CHECK(engine->login("Ali") == Result::Done);
    CHECK(engine->getCurrentUser()->getName() == "Ali Khan");
    CHECK(engine->findUser("ALI") == engine->getCurrentUser());

    return true;
}

// ===== LIKES (Stack) =====

bool testLikesAndUndo() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    CHECK(engine->likePost(3) == Result::NotLoggedIn);

    engine->signUp("ali", "Ali", 20);

    CHECK(engine->likePost(3) == Result::Done);
    CHECK(engine->findPost(3)->getLikes() == 1);
    CHECK(engine->likePost(3) == Result::AlreadyLiked);
    CHECK(engine->findPost(3)->getLikes() == 1);
    CHECK(engine->likePost(99) == Result::PostNotFound);

    engine->likePost(5);
    engine->likePost(7);

    // undo pops the stack: 7, then 5, then 3
    Post* undone = nullptr;

    CHECK(engine->undoLastLike(undone) == Result::Done && undone->getId() == 7);
    CHECK(engine->findPost(7)->getLikes() == 0);
    CHECK(engine->undoLastLike(undone) == Result::Done && undone->getId() == 5);
    CHECK(engine->undoLastLike(undone) == Result::Done && undone->getId() == 3);
    CHECK(engine->undoLastLike(undone) == Result::NothingToUndo && undone == nullptr);

    // after an undo the post can be liked again
    CHECK(engine->likePost(3) == Result::Done);
    CHECK(engine->findPost(3)->getLikes() == 1);

    return true;
}

// ===== TRENDING (Max-Heap) =====

bool testTrending() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    engine->signUp("ali", "Ali", 20);
    engine->likePost(5);
    engine->likePost(9);

    engine->signUp("sara", "Sara", 21);
    engine->likePost(9);
    engine->likePost(2);

    // likes: post 9 -> 2, posts 2 and 5 -> 1, the rest 0
    Post* top[5];
    CHECK(engine->trendingTop(5, top) == 5);

    int expected[] = {9, 2, 5, 1, 3};       // equal likes: older post first

    for (int i = 0; i < 5; i++)
        CHECK(top[i]->getId() == expected[i]);

    Post* all[FeedEngine::MAX_POSTS];
    CHECK(engine->trendingTop(50, all) == 13);

    for (int i = 1; i < 13; i++)
        CHECK(!MoreLikes()(all[i], all[i - 1]));

    // sara undoes her like on post 2
    Post* undone = nullptr;
    engine->undoLastLike(undone);
    engine->trendingTop(5, top);

    int afterUndo[] = {9, 5, 1, 2, 3};

    for (int i = 0; i < 5; i++)
        CHECK(top[i]->getId() == afterUndo[i]);

    return true;
}

// ===== SORTING =====

bool testSortedCopy() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    engine->signUp("ali", "Ali", 20);
    engine->likePost(4);
    engine->likePost(8);
    engine->likePost(11);
    engine->signUp("sara", "Sara", 21);
    engine->likePost(8);
    engine->likePost(4);
    engine->signUp("omar", "Omar", 22);
    engine->likePost(8);

    // likes: post 8 -> 3, post 4 -> 2, post 11 -> 1
    SortAlgorithm algorithms[] = {
        SortAlgorithm::Selection, SortAlgorithm::Bubble, SortAlgorithm::Insertion,
        SortAlgorithm::Merge, SortAlgorithm::Quick, SortAlgorithm::Heap
    };

    Post* sorted[FeedEngine::MAX_POSTS];

    for (int a = 0; a < 6; a++) {

        engine->sortedCopy(SortKey::Likes, algorithms[a], sorted);
        CHECK(sorted[0]->getId() == 8 && sorted[1]->getId() == 4);
        CHECK(sorted[2]->getId() == 11 && sorted[3]->getId() == 1);

        for (int i = 1; i < 13; i++)
            CHECK(!MoreLikes()(sorted[i], sorted[i - 1]));

        engine->sortedCopy(SortKey::Title, algorithms[a], sorted);
        CHECK(sorted[0]->getTitle() == "AI Revolution");
        CHECK(sorted[12]->getTitle() == "Travel Vlog");

        for (int i = 1; i < 13; i++)
            CHECK(!TitleAZ()(sorted[i], sorted[i - 1]));

        engine->sortedCopy(SortKey::Id, algorithms[a], sorted);

        for (int i = 0; i < 13; i++)
            CHECK(sorted[i]->getId() == i + 1);

        // the real list is untouched, so binary search still works
        for (int i = 0; i < 13; i++)
            CHECK(engine->getPostAt(i)->getId() == i + 1);

        CHECK(engine->searchByIdBinary(11).index == 10);
    }

    return true;
}

// ===== TITLE INDEX (AVL tree) =====

bool testTitleIndex() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    const TitleIndex& index = engine->getTitleIndex();
    CHECK(index.size() == 13 && index.isValid());
    CHECK(index.height() <= 5);

    // A-Z by title (in-order traversal)
    int aToZ[] = {2, 4, 9, 10, 1, 11, 7, 6, 3, 13, 12, 5, 8};
    CHECK(idsAre(engine->postsByTitle(), aToZ, 13));

    // search by the start of the title, ignoring capital letters
    int startsWithF[] = {10, 1};
    CHECK(idsAre(engine->searchByTitlePrefix("f"), startsWithF, 2));

    int foot[] = {1};
    CHECK(idsAre(engine->searchByTitlePrefix("FOOT"), foot, 1));

    int startsWithG[] = {11, 7};
    CHECK(idsAre(engine->searchByTitlePrefix("g"), startsWithG, 2));

    int startsWithS[] = {12, 5};
    CHECK(idsAre(engine->searchByTitlePrefix("s"), startsWithS, 2));
    CHECK(engine->searchByTitlePrefix("zz").isEmpty());

    // every title is found by its own first two letters
    for (int i = 0; i < engine->getPostCount(); i++) {
        Post* post = engine->getPostAt(i);
        DoublyLinkedList<Post*> found = engine->searchByTitlePrefix(post->getTitle().substr(0, 2));
        CHECK(found.search(post) != nullptr);
    }

    CHECK(engine->titleSearchSteps("News Update") >= 1);
    CHECK(engine->titleSearchSteps("News Update") <= index.height());

    // a second post with the same title shares that title's tree node
    engine->signUp("ali", "Ali", 20);
    engine->createPost("AI Revolution", "Technology");      // post 14
    CHECK(index.size() == 13 && index.isValid());

    int ai[] = {2, 14};
    CHECK(idsAre(engine->searchByTitlePrefix("ai"), ai, 2));
    CHECK(engine->postsByTitle().size() == 14);

    // new titles keep the tree balanced and in order
    engine->createPost("Zebra Facts", "Education");
    engine->createPost("Apple Pie", "Health");
    CHECK(index.size() == 15 && index.isValid());
    CHECK(engine->postsByTitle().getHead()->data->getTitle() == "AI Revolution");
    CHECK(engine->postsByTitle().getTail()->data->getTitle() == "Zebra Facts");

    engine->reset();
    CHECK(index.isEmpty());

    return true;
}

// ===== RECOMMENDATIONS =====

bool testRecommendations() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    engine->signUp("ali", "Ali", 20);
    User* ali = engine->getCurrentUser();
    ali->addInterest("tech");
    ali->addInterest("Sports");

    engine->signUp("sara", "Sara", 21);
    engine->likePost(9);

    // Technology (2, 9) and Sports (1); post 9 has a like so it comes first
    PriorityQueue<Post*, MoreLikes> picks = engine->recommendationsFor(ali);
    int expected[] = {9, 1, 2};
    int i = 0;

    CHECK(picks.size() == 3);

    for (DoublyLinkedList<Post*>::Node* curr = picks.getHead(); curr != nullptr; curr = curr->next)
        CHECK(curr->data->getId() == expected[i++]);

    // your own posts are not recommended to you
    engine->login("ali");
    engine->createPost("My Tech Post", "Technology");
    picks = engine->recommendationsFor(ali);
    CHECK(picks.size() == 3);

    return true;
}

// ===== NOTIFICATIONS (Queue) AND FOLLOWS (Graph) =====

bool testNotificationsAndFollows() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    engine->signUp("ali", "Ali", 20);
    engine->createPost("My First Post", "Travel");       // post 14

    engine->signUp("sara", "Sara", 21);
    CHECK(engine->likePost(14) == Result::Done);
    CHECK(engine->likePost(1) == Result::Done);          // author is not a user: no notification
    engine->createPost("Sara Post", "News");             // post 15
    engine->likePost(15);                                // own post: no notification
    CHECK(engine->getCurrentUser()->getNotifications().isEmpty());

    CHECK(engine->follow("ali") == Result::Done);
    CHECK(engine->follow("ali") == Result::AlreadyFollowing);
    CHECK(engine->follow("sara") == Result::CannotFollowSelf);
    CHECK(engine->follow("ghost") == Result::UserNotFound);

    User* ali = engine->findUser("ali");
    CHECK(ali->getNotifications().size() == 2);
    CHECK(ali->getNotifications().at(0) == "@sara liked your post \"My First Post\"");
    CHECK(ali->getNotifications().at(1) == "@sara started following you");

    // omar -> sara -> ali, hina -> ali
    engine->signUp("omar", "Omar", 22);
    engine->follow("sara");
    engine->signUp("hina", "Hina", 23);
    engine->follow("ali");
    engine->login("omar");

    // ali is 2 steps from omar (through sara)
    PriorityQueue<Suggestion, MoreMutual> suggestions = engine->suggestionsFor("omar");
    CHECK(suggestions.size() == 1);
    CHECK(suggestions.peek().username == "ali" && suggestions.peek().mutual == 1);

    // now through hina as well: 2 mutual follows
    engine->follow("hina");
    suggestions = engine->suggestionsFor("omar");
    CHECK(suggestions.size() == 1 && suggestions.peek().mutual == 2);

    string path[] = {"omar", "sara", "ali"};
    CHECK(namesAre(engine->connectionPath("omar", "ali"), path, 3));
    CHECK(engine->connectionPath("ali", "omar").isEmpty());

    string aliFollowers[] = {"sara", "hina"};
    CHECK(namesAre(engine->followersOf("ali"), aliFollowers, 2));

    string omarFollowing[] = {"sara", "hina"};
    CHECK(namesAre(engine->followingOf("omar"), omarFollowing, 2));

    // omar follows sara and hina: only sara has posted
    int feedIds[] = {15};
    CHECK(idsAre(engine->followingFeed("omar"), feedIds, 1));

    CHECK(engine->unfollow("sara") == Result::Done);
    CHECK(engine->unfollow("sara") == Result::NotFollowing);
    CHECK(engine->getFollowCount() == 3);    // sara -> ali, hina -> ali, omar -> hina

    return true;
}

// ===== SAVE AND LOAD (FileManager) =====

bool testSaveAndLoad() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();
    engine->seedDefaultPosts();

    engine->signUp("ali", "Ali Khan", 20);
    engine->getCurrentUser()->addInterest("Tech");
    engine->getCurrentUser()->addInterest("Travel");
    engine->createPost("Trip | Part 1", "Travel");       // post 14, title with a '|'
    engine->likePost(2);
    engine->likePost(9);
    engine->likePost(14);

    engine->signUp("sara", "Sara", 21);
    engine->likePost(14);
    engine->follow("ali");
    engine->logout();

    string postsText = FileManager::postsToText(*engine);
    string usersText = FileManager::usersToText(*engine);

    engine->reset();
    CHECK(engine->getPostCount() == 0 && engine->getUsers().isEmpty());

    CHECK(FileManager::loadPostsFromText(*engine, postsText) == 14);
    CHECK(FileManager::loadUsersFromText(*engine, usersText) == 2);

    // saving again gives exactly the same text: nothing lost, nothing added
    CHECK(FileManager::postsToText(*engine) == postsText);
    CHECK(FileManager::usersToText(*engine) == usersText);

    CHECK(engine->findPost(14)->getTitle() == "Trip | Part 1");
    CHECK(engine->findPost(14)->getAuthor() == "ali");
    CHECK(engine->findPost(14)->getLikes() == 2);
    CHECK(engine->findPost(9)->getLikes() == 1);
    CHECK(engine->getFeed().getHead()->data->getId() == 14);
    CHECK(engine->getTitleIndex().size() == 14 && engine->getTitleIndex().isValid());

    User* ali = engine->findUser("ali");
    CHECK(ali != nullptr && ali->getName() == "Ali Khan" && ali->getAge() == 20);
    CHECK(ali->getInterests().size() == 2 && ali->isInterestedIn("Technology"));
    CHECK(ali->getLikeHistory().size() == 3 && ali->getLikeHistory().peek() == 14);
    CHECK(ali->getNotifications().size() == 2);
    CHECK(ali->getNotifications().at(1) == "@sara started following you");
    CHECK(engine->isFollowing("sara", "ali") && !engine->isFollowing("ali", "sara"));

    // after loading, ali still cannot like a post twice, and undo still works
    CHECK(engine->login("ali") == Result::Done);
    CHECK(engine->likePost(9) == Result::AlreadyLiked);

    Post* undone = nullptr;
    engine->undoLastLike(undone);
    CHECK(undone != nullptr && undone->getId() == 14);
    engine->undoLastLike(undone);
    CHECK(undone != nullptr && undone->getId() == 9);

    return true;
}

bool testLoadBadText() {

    FeedEngine* engine = FeedEngine::getInstance();
    engine->reset();

    string posts =
        "hello world\n"
        "1 0\n"                                 // the old OOP posts.txt format
        "POST|x|0|ali|News|Bad ID\n"
        "POST|20|3|ali|News|Big News\r\n"       // Windows line ending
        "POST|19|0|ali|News|Older ID\n"         // smaller ID after 20
        "POST|21|-4|ali|News|Bad Likes\n"
        "POST|22|1|ali|News|\n";                // empty title

    CHECK(FileManager::loadPostsFromText(*engine, posts) == 1);
    CHECK(engine->findPost(20)->getTitle() == "Big News");
    CHECK(engine->findPost(20)->getLikes() == 3);

    string users =
        "INTEREST|tech\n"                       // before any USER line
        "USER|ali|Ali|20\n"
        "LIKE|20\n"
        "LIKE|99\n"                             // no such post
        "LIKE|20\n"                             // liked twice
        "FOLLOW|ghost\n"                        // no such user
        "USER|ali|Copy|30\n"                    // username taken
        "NOTE|should be skipped\n"
        "USER|sara|Sara|abc\n"                  // bad age
        "FOLLOW|ali\n";

    CHECK(FileManager::loadUsersFromText(*engine, users) == 1);

    User* ali = engine->findUser("ali");
    CHECK(ali != nullptr && ali->getName() == "Ali");
    CHECK(ali->getInterests().isEmpty());
    CHECK(ali->getLikeHistory().size() == 1);
    CHECK(ali->getNotifications().isEmpty());
    CHECK(engine->findUser("sara") == nullptr);
    CHECK(engine->getFollowCount() == 0);

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

    for (int i = static_cast<int>(name.length()); i < 26; i++)
        cout << '.';

    if (passed)
        cout << GREEN << " PASS" << RESET << endl;
    else
        cout << RED << " FAIL" << RESET << endl;

    return passed ? 0 : 1;
}

int main() {

    printBox("FEEDENGINE TESTS");

    int failed = 0;

    failed += runTest("Post and User", testPostAndUser);
    failed += runTest("Posts and Search", testPostsAndSearch);
    failed += runTest("Feed Full", testFeedFull);
    failed += runTest("Sign Up and Login", testSignUpAndLogin);
    failed += runTest("Likes and Undo", testLikesAndUndo);
    failed += runTest("Trending", testTrending);
    failed += runTest("Sorted Copy", testSortedCopy);
    failed += runTest("Title Index (AVL)", testTitleIndex);
    failed += runTest("Recommendations", testRecommendations);
    failed += runTest("Notifications and Follows", testNotificationsAndFollows);
    failed += runTest("Save and Load", testSaveAndLoad);
    failed += runTest("Load Bad Text", testLoadBadText);

    if (failed == 0)
        printSuccess("All tests passed!");
    else
        printError(to_string(failed) + " test(s) failed");

    return failed;
}
