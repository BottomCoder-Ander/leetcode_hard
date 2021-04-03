/*
 *
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