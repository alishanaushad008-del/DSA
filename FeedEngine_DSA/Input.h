#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

#include "Functions.h"

using namespace std;

// ===== INPUT VALIDATION =====
// Every input is read as a whole line, so a wrong entry never leaves
// junk behind in cin and the program never gets stuck in a loop.

// Thrown when input runs out (Ctrl+Z, or a closed input file),
// so main() can still save before it closes
struct InputEnded {};

// Reads one line and trims spaces at both ends
inline string readLine(const string& prompt) {

    cout << prompt;

    string line;

    if (!getline(cin, line))
        throw InputEnded();

    int start = 0;
    int end = static_cast<int>(line.length()) - 1;

    while (start <= end && isspace(static_cast<unsigned char>(line[start])))
        start++;

    while (end >= start && isspace(static_cast<unsigned char>(line[end])))
        end--;

    return line.substr(start, end - start + 1);
}

// A whole number between minValue and maxValue
inline int readInt(const string& prompt, int minValue, int maxValue) {

    while (true) {

        string line = readLine(prompt);
        istringstream in(line);
        int value;
        char extra;

        if (in >> value && !(in >> extra) && value >= minValue && value <= maxValue)
            return value;

        printError("INVALID INPUT! ENTER A NUMBER FROM " + to_string(minValue) + " TO " + to_string(maxValue));
    }
}

// Text of 1-40 characters. lettersOnly blocks digits (names, interests).
// '|' is never allowed because the save files use it to separate fields.
inline string readText(const string& prompt, bool lettersOnly) {

    while (true) {

        string text = readLine(prompt);
        bool valid = !text.empty() && text.length() <= 40;

        for (int i = 0; i < static_cast<int>(text.length()); i++) {

            char c = text[i];

            if (c == '|' || (lettersOnly && isdigit(static_cast<unsigned char>(c))))
                valid = false;
        }

        if (valid)
            return text;

        if (lettersOnly)
            printError("INVALID INPUT! ENTER LETTERS ONLY (1-40 CHARACTERS)");
        else
            printError("INVALID INPUT! ENTER 1-40 CHARACTERS, WITHOUT '|'");
    }
}

// One word of 3-20 letters, digits or _ . Returned in lowercase.
inline string readUsername(const string& prompt) {

    while (true) {

        string name = readLine(prompt);

        if (!name.empty() && name[0] == '@')     // "@ali" is fine too
            name = name.substr(1);

        bool valid = name.length() >= 3 && name.length() <= 20;

        for (int i = 0; i < static_cast<int>(name.length()); i++)
            if (!isalnum(static_cast<unsigned char>(name[i])) && name[i] != '_')
                valid = false;

        if (valid)
            return toLowerCase(name);

        printError("INVALID USERNAME! USE 3-20 LETTERS, DIGITS OR _");
    }
}

#endif
