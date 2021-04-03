/*
 * 1771. Maximize Palindrome Length From Subsequences
 * 题意：给两个字符串word1,word2.从word1选取一个非空子序列s1，从word2选取非空一个子序列s2
 *    将s1,s2拼接，问拼接后的字符串是回文串时，最大的回文串长度
 * 题解：动态规划。先将两个串拼接成s。则问题就是s子序列中，两个端点在两个串中的最长回文序列长度。
 *      可以设dp[i][j]为子串i到j的最长回文序列，然后端点在两个子串中的那些序列取最大值
 *      也可以设dp[i][j]为串0到i和串j到s.length的最长偶数长度且两个端点在两个串中的回文子序列长度
 */
class Solution {
public:
    int longestPalindrome(string word1, string word2) {
        string s = word1 + word2;

        int n = s.length();
        int dp[n][n];
        int idx[126];
        memset(dp, 0, sizeof(dp));
        memset(idx, -1, sizeof(idx));

        for(int i = 0; i < s.length(); ++i){
            idx[s[i]] = i;
            if(s[i] == s[n - 1] && i < word1.length()) {
                dp[i][n - 1] = 2;
            } else if(i != 0){
                dp[i][n - 1] = dp[i - 1][n - 1];
            }

            for(int j = n - 2; j > i; --j){
                dp[i][j] = dp[i][j + 1];
                if(i > 0) dp[i][j] = max(dp[i][j], dp[i - 1][j]);

                if(idx[s[j]] > 0) {
                    if(idx[s[j]] < word1.length() && j >= word1.length() || dp[idx[s[j]] - 1][j + 1] > 0)
                        dp[i][j] = max(dp[i][j], dp[idx[s[j]] - 1][j + 1] + 2);

                } else if(idx[s[j]] == 0 && j >= word1.length() && dp[i][j] == 0){
                    dp[i][j] = 2;
                }


            }
        }

        int ans = 0;

        for(int i = 0; i < n - 1; ++i){
            ans = max(ans, dp[i][i + 1]);

             if(i < n - 2 && dp[i][i + 2])
                 ans = max(ans, dp[i][i + 2] + 1);

        }

        return ans;
    }
};