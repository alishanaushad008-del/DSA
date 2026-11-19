// FeedEngine - DSA Version (CT-159 CCA)
// Alisha Naushad (CT-169), Syeda Zainab (CT-171), Abdul Raheem Sheikh (CT-192)
//
// START READING HERE.
// main() loads the saved files, then shows one of two menus in a loop:
//   - the account menu when nobody is logged in (Login / Sign Up / Exit)
//   - the main menu when someone is logged in (16 options)
// Each option calls one screen from Screens.h, and the screen asks
// FeedEngine (the brain) to do the work.

#include "FeedEngine.h"
#include "FileManager.h"
#include "Screens.h"

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
