/*
 * @lc app=leetcode.cn id=563 lang=cpp
 *
 * [563] 二叉树的坡度
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int res = 0;
    // 后续
    int dfs(TreeNode* root)
    {
        if (!root) return 0;

        int l = dfs(root->left);
        int r = dfs(root->right);

        res += abs(l - r);

        return l + r + root->val;
    }

    int findTilt(TreeNode* root) {
        dfs(root);
        return res;
    }
};
// @lc code=end

