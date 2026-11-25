#include <iostream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class BinaryTree {
public:
    TreeNode* search(TreeNode* root, int val) {
        if (root == NULL || root->val == val) {
            return root;
        }
        TreeNode* leftResult = search(root->left, val);
        if (leftResult != NULL) {
            return leftResult;
        }
        return search(root->right, val);
    }

    void preOrder(TreeNode* root) {
        if (root == NULL) return;
        cout << root->val << " ";
        preOrder(root->left);
        preOrder(root->right);
    }

    void postOrder(TreeNode* root) {
        if (root == NULL) return;
        postOrder(root->left);
        postOrder(root->right);
        cout << root->val << " ";
    }
};

int main() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    
    BinaryTree tree;
    tree.preOrder(root);
    cout << endl;
    
    tree.postOrder(root);
    cout << endl;
    
    TreeNode* found = tree.search(root, 2);
    if(found) cout << found->val << endl;

    return 0;
}