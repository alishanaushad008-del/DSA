#include <iostream>
#include <vector>
using namespace std;

bool carPooling(vector<vector<int>>& trips, int capacity) {
    int maxLocation = 0;
    for (int i = 0; i < trips.size(); i++) {
        if (trips[i][2] > maxLocation) maxLocation = trips[i][2];
    }

    vector<int> change(maxLocation + 1, 0);

    for (int i = 0; i < trips.size(); i++) {
        change[trips[i][1]] += trips[i][0];
        change[trips[i][2]] -= trips[i][0];
    }

    int currentPassengers = 0;
    for (int i = 0; i <= maxLocation; i++) {
        currentPassengers += change[i];
        if (currentPassengers > capacity) return false;
    }
    return true;
}

int main() {
    vector<vector<int>> trips1 = {{2, 1, 5}, {3, 3, 7}};
    int capacity1 = 4;

    vector<vector<int>> trips2 = {{2, 1, 5}, {3, 3, 7}};
    int capacity2 = 5;

    cout << "Example 1 Output: " << (carPooling(trips1, capacity1) ? "true" : "false") << endl;
    cout << "Example 2 Output: " << (carPooling(trips2, capacity2) ? "true" : "false") << endl;

    return 0;
}