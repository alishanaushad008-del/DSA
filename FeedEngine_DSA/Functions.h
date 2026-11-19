#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

inline void printBox(string title) {

    cout << CYAN
         << "\n====================================\n";

    cout << "   " << title << endl;

    cout << "====================================\n"
         << RESET;
}

inline void printError(string msg) {
    cout << RED << msg << RESET << endl;
}

inline void printSuccess(string msg) {
    cout << GREEN << msg << RESET << endl;
}

// ===== TEXT HELPERS =====

// "TeCh" -> "tech"
inline string toLowerCase(string text) {

    for (int i = 0; i < static_cast<int>(text.length()); i++)
        if (text[i] >= 'A' && text[i] <= 'Z')
            text[i] = static_cast<char>(text[i] - 'A' + 'a');

    return text;
}

inline bool startsWith(const string& text, const string& start) {
    return text.length() >= start.length() && text.compare(0, start.length(), start) == 0;
}

// Does text contain word anywhere? Ignores capital letters.
inline bool containsText(const string& text, const string& word) {
    return toLowerCase(text).find(toLowerCase(word)) != string::npos;
}

// Cuts long text so table columns stay straight: "Beauty Skincare Ro..."
inline string shorten(const string& text, int maxLength) {

    if (static_cast<int>(text.length()) <= maxLength)
        return text;

    return text.substr(0, maxLength - 3) + "...";
}

#endif