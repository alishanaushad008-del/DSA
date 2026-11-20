#include <iostream>
#include <vector>
#include <map>
using namespace std;

int findLHS(vector<int>& nums) {
    map<int, int> freq;
    for (int i = 0; i < nums.size(); i++) {
        freq[nums[i]]++;
    }

    int longest = 0;
    for (map<int, int>::iterator it = freq.begin(); it != freq.end(); it++) {
        int num = it->first;
        if (freq.count(num + 1)) {
            int length = it->second + freq[num + 1];
            if (length > longest) longest = length;
        }
    }
    return longest;
}

int main() {
    vector<int> nums1 = {1, 3, 2, 2, 5, 2, 3, 7};
    vector<int> nums2 = {1, 2, 3, 4};
    vector<int> nums3 = {1, 1, 1, 1};

    cout << "Example 1 Output: " << findLHS(nums1) << endl;
    cout << "Example 2 Output: " << findLHS(nums2) << endl;
    cout << "Example 3 Output: " << findLHS(nums3) << endl;

    return 0;
}