/*
 * @lc app=leetcode.cn id=273 lang=cpp
 *
 * [273] 整数转换英文表示
 */

// @lc code=start
class Solution {
public:
    vector<string> singles = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    vector<string> teens = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    vector<string> tens = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    vector<string> thousands = {"", "Thousand", "Million", "Billion"};


    string numberToWords(int num) {
        if (num == 0)
            return "Zero";
        string res;
        for (int i = 3, unit = 1000000000; i >= 0; --i, unit /= 1000)
        {
            int curnum = num / unit;
            if (curnum != 0)
            {   
                num -= unit * curnum;
                res = res + toEnglish(curnum) + thousands[i] + " ";
            }


        }

        while (res.back() == ' ')
            res.pop_back();
        return res;
    }

    string toEnglish(int num)
    {
        string curr;
        int hundred = num / 100;
        num %= 100;
        if (hundred != 0)
        {
            curr = curr + singles[hundred] + " Hundred ";
        }

        int ten = num / 10;
        
        if (ten >= 2)
        {
            num %= 10;
            curr = curr + tens[ten] + " ";
        }

        if (num > 0 && num < 10)
            curr = curr + singles[num] + " ";
        else if (num >= 10)
            curr = curr + teens[num-10] + " ";    

        return curr;
    }
};
// @lc code=end

