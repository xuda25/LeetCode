/*
 * @lc app=leetcode.cn id=143 lang=cpp
 *
 * [143] 重排链表
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
// 方法1： 把节点放入数组

// class Solution {
// public:
//     void reorderList(ListNode* head) {
//         vector<ListNode*> vec;
//         ListNode* cur = head;

//         while (cur)
//         {
//             vec.push_back(cur);
//             cur = cur->next;
//         }
        
//         cur = head;
//         int i = 1, j = vec.size()-1;
//         int count = 0;
//         while (i <= j)
//         {
//             if (count % 2 == 0)
//             {
//                 cur->next = vec[j];
//                 --j;
//             }
//             else
//             {
//                 cur->next = vec[i];
//                 ++i;
//             }
//             cur = cur->next;
//             ++count;
//         }
//         cur->next = nullptr;

//     }
// };

// 方法2
class Solution {
public:
    void reorderList(ListNode* head) {
        deque<ListNode*> que;
        ListNode* cur = head->next;

        while (cur)
        {
            que.push_back(cur);
            cur = cur->next;
        }

        cur = head;
        int count = 0;
        while (!que.empty())
        {
            if (count % 2 == 0)
            {
                cur->next = que.back();
                que.pop_back();
            }
            else
            {
                cur->next = que.front();
                que.pop_front();
            }
            cur = cur->next;
            ++count;
        }
        cur->next = nullptr;
    }
};
// @lc code=end

