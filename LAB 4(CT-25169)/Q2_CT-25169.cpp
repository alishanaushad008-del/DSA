#include <iostream>
#include <string>
#include <stack>

using namespace std;

string processString(string str) {
    stack<char> st;
    for (char c : str) {
        if (c == '#') {
            if (!st.empty()) {
                st.pop();
            }
        } else {
            st.push(c);
        }
    }
    string result = "";
    while (!st.empty()) {
        result = st.top() + result;
        st.pop();
    }
    return result;
}

bool backspaceCompare(string s, string t) {
    return processString(s) == processString(t);
}

int main() {
    string s, t;
    cin >> s >> t;
    if (backspaceCompare(s, t)) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }
    return 0;
}