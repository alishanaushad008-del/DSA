#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
    vector<vector<int>> result;
    if (intervals.empty()) return result;

    sort(intervals.begin(), intervals.end());

    result.push_back(intervals[0]);

    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] <= result.back()[1]) {
            result.back()[1] = max(result.back()[1], intervals[i][1]);
        } else {
            result.push_back(intervals[i]);
        }
    }
    return result;
}

void printIntervals(const vector<vector<int>>& v) {
    cout << "[";
    for (int i = 0; i < v.size(); i++) {
        cout << "[" << v[i][0] << "," << v[i][1] << "]";
        if (i != v.size() - 1) cout << ",";
    }
    cout << "]" << endl;
}

int main() {
    vector<vector<int>> intervals1 = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    vector<vector<int>> intervals2 = {{1, 4}, {4, 5}};

    cout << "Example 1 Output: ";
    printIntervals(mergeIntervals(intervals1));

    cout << "Example 2 Output: ";
    printIntervals(mergeIntervals(intervals2));

    return 0;
}