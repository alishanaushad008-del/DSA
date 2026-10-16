#include <iostream>
#include <stack>
#include <string>

using namespace std;

bool isPalindrome(string s) {
    stack<char> st;
    for (char c : s) {
        st.push(c);
    }
    for (char c : s) {
        if (c != st.top()) {
            return false;
        }
        st.pop();
    }
    return true;
}

int main() {
    string s;
    cin >> s;
    if (isPalindrome(s)) {
        cout << "Palindrome" << endl;
    } else {
        cout << "Not Palindrome" << endl;
    }
    return 0;
}