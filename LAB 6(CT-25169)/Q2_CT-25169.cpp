#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxToys(vector<int>& arr, int K) {
    sort(arr.begin(), arr.end());
    int count = 0;
    for (int cost : arr) {
        if (K >= cost) {
            K -= cost;
            count++;
        } else {
            break;
        }
    }
    return count;
}

int main() {
    int N, K;
    if (!(cin >> N >> K)) return 0;
    
    vector<int> arr(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    cout << maxToys(arr, K) << endl;

    return 0;
}