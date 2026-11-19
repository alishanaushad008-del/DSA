#ifndef POST_H
#define POST_H

#include <iostream>
#include <iomanip>
#include <string>

#include "Content.h"
#include "Functions.h"

using namespace std;

class Post : public Content {

private:
    int id;
    string title;
    string category;
    string author;      // username of the user who posted it
    int likes;

public:

    Post(int i, string t, string c, string a, int l) : id(i), title(t), category(c), author(a), likes(l > 0 ? l : 0) {}

    int getId() const { return id; }

    string getTitle() const { return title; }

    string getCategory() const { return category; }

    string getAuthor() const { return author; }

    int getLikes() const { return likes; }

    void like() {
        likes++;
    }

    void unlike() {
        if (likes > 0)
            likes--;
    }

    // One table row: ID, title, category, likes, author
    void display() const override {

        cout << CYAN << left << setw(5) << id << RESET
             << setw(28) << shorten(title, 26)
             << setw(13) << shorten(category, 12)
             << GREEN << setw(7) << likes << RESET
             << "@" << author;
    }

    // Bigger view used while scrolling the feed
    void displayCard() const {

        cout << CYAN << "  #" << id << "  " << RESET << title << endl;
        cout << "  Category : " << YELLOW << category << RESET << endl;
        cout << "  Posted by: @" << author << endl;
        cout << "  Likes    : " << GREEN << likes << RESET << endl;
    }
};

#endif
