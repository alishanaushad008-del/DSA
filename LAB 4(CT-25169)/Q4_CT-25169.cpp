#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool findWord(vector<vector<char>>& board, string word, int r, int c, int i) {
    if (i == word.length()) {
        return true;
    }
    
    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size() || board[r][c] != word[i]) {
        return false;
    }
    
    char temp = board[r][c];
    board[r][c] = '#';
    
    bool found = findWord(board, word, r + 1, c, i + 1) ||
                 findWord(board, word, r - 1, c, i + 1) ||
                 findWord(board, word, r, c + 1, i + 1) ||
                 findWord(board, word, r, c - 1, i + 1);
                 
    board[r][c] = temp;
    return found;
}

bool exist(vector<vector<char>>& board, string word) {
    int rows = board.size();
    int cols = board[0].size();
    
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (findWord(board, word, r, c, 0)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int m, n;
    if (!(cin >> m >> n)) return 0;
    
    vector<vector<char>> board(m, vector<char>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }
    
    string word;
    cin >> word;
    
    if (exist(board, word)) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }
    
    return 0;
}