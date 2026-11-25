#include <iostream>
#include <algorithm>
#include <utility>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

pair<int, int> robSub(TreeNode* root) {
    if (root == NULL) return {0, 0};

    pair<int, int> left = robSub(root->left);
    pair<int, int> right = robSub(root->right);

    int robCurrent = root->val + left.second + right.second;
    int notRobCurrent = max(left.first, left.second) + max(right.first, right.second);

    return {robCurrent, notRobCurrent};
}

int rob(TreeNode* root) {
    pair<int, int> result = robSub(root);
    return max(result.first, result.second);
}

int main() {
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(3);
    root->right->right = new TreeNode(1);

    cout << rob(root) << endl;

    return 0;
}