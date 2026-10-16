#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> pancakeSort(vector<int>& arr) {
    vector<int> result;
    int n = arr.size();
    for (int curr_size = n; curr_size > 1; curr_size--) {
        int max_idx = 0;
        for (int i = 1; i < curr_size; i++) {
            if (arr[i] > arr[max_idx]) {
                max_idx = i;
            }
        }
        if (max_idx != curr_size - 1) {
            if (max_idx != 0) {
                result.push_back(max_idx + 1);
                reverse(arr.begin(), arr.begin() + max_idx + 1);
            }
            result.push_back(curr_size);
            reverse(arr.begin(), arr.begin() + curr_size);
        }
    }
    return result;
}

int main() {
    int n;
    if (!(cin >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> flips = pancakeSort(arr);

    cout << "[";
    for (size_t i = 0; i < flips.size(); i++) {
        cout << flips[i] << (i + 1 < flips.size() ? "," : "");
    }
    cout << "]" << endl;

    return 0;
}