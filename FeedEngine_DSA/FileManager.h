#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>

#include "FeedEngine.h"
#include "DataStructures/Stack.h"

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
