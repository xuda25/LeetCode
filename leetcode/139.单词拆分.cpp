/*
 * @lc app=leetcode.cn id=139 lang=cpp
 *
 * [139] 单词拆分
 */

// @lc code=start
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set wordSet(wordDict.begin(), wordDict.end());
        vector<bool> dp(s.size() + 1, false);
        dp[0] = true;

        // 先背包 再物品
        for (int i = 1; i <= s.size(); ++i)
            for (int j = 0; j < i; ++j)
            {
                string word = s.substr(j, i-j);
                if (wordSet.find(word) != wordSet.end() && dp[j])
                    dp[i] = true;
            }

        return dp[s.size()];
    }
};
// @lc code=end

