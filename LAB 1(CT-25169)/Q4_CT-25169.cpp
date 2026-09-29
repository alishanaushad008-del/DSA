/*Given an array of integers nums which is sorted in ascending order, and an integer target, write a function to search target in nums. 
If target exists, then return its index. Otherwise, return -1. You must write an algorithm with O(log n) runtime complexity.
Example 1: Input: nums = [-1,0,3,5,9,12], target = 9, Output: 4 Explanation: 9 exists in nums and its index is 4
 Example 2: Input: nums = [-1,0,3,5,9,12], target = 2, Output: -1 
 Explanation: 2 does not exist in nums so return -1 Constraints: 1 <= nums.length <= -104 < nums[i], target < 104 All the integers in nums are unique.*/

 #include <iostream>
#include <vector>

using namespace std;

int search(vector<int>&nums,int target){
    int l=0;
int r=nums.size()-1;

while(l<=r){
    int mid=l+(r-l)/2;

    if(nums[mid]==target){
        return mid;
    }
    else if(nums[mid]<target){
    l=mid+1;
}
else{
    r=mid-1;
}
}
return -1;


}
int main(){
  
    vector<int> nums = {-1, 0, 3, 5, 9, 12};
    int target1= 9;

    int result = search(nums, target1);
    cout << "Index: " << result << endl; // Output: 4


    int target2 = 2;
    int result2 = search(nums, target2);
    cout << "Index: " << result2 << endl; // Output: -1

    return 0;
}  
