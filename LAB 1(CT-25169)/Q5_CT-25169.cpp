/*nums is sorted in ascending order.You are given an m x n integer matrix with the following two properties: Each row is sorted in non-decreasing order. The first integer of each row is greater than the last integer of the previous row. Given an integer target, return true if target is in matrix or false otherwise. You must write a solution in O(log(m * n)) time complexity. Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3, Output: true Constraints: m == matrix.length, n == matrix[i].length, 1 <= m, n <= 100, -104 <= matrix[i][j], target <= 104 */

#include <iostream>
#include <vector>

using namespace std;

bool searchMatrix(vector<vector<int>>&matrix,int target){
    int m=matrix.size();
    int n=matrix[0].size();
    int l = 0;
    int r = (m * n) - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        int row = mid / n;
        int col = mid % n;

        if (matrix[row][col] == target) {
            return true;
        } 
        else if (matrix[row][col] < target) {
            l = mid + 1;
        } 
        else {
            r = mid - 1;
        }
    }

    return false;
}
int main() {
    vector<vector<int>> matrix = {
        {1, 3, 5, 7},
        {10, 11, 16, 20},
        {23, 30, 34, 60}
    };

    int target1 = 3;
    cout << (searchMatrix(matrix, target1) ? "true" : "false") << endl; // Output: true

    int target2 = 13;
    cout << (searchMatrix(matrix, target2) ? "true" : "false") << endl; // Output: false

    return 0;
}