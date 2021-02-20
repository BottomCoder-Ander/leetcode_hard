/* 1751. Maximum Number of Events That Can Be Attended II
 *  题意：给定一个实践数组，每个事件有格式events[i] = [startDayi, endDayi, valuei]，表示事件i从startDayi开始，到endDayi结束（包括endDayi）
 *      并且完成事件获得收益valuei。给定一个值k，问在这些事件中，至多选择k个没有时间重叠的事件，最多获益多少   
 *  题解：动态规划,设dp[i][j]为前i个事件，选择j个没有时间重叠的事件的最大收益。
 *      由当前事件是否取，两种情况进行转移
 */

class Solution {
    
public:
    int maxValue(vector<vector<int>>& events, int k) {
        int n = events.size();

        long long dp[n + 1][k + 1];
        memset(dp, 0, sizeof(dp));

        sort(events.begin(), events.end(), 
            [](vector<int>&a, vector<int>&b) {
                return a[1] < b[1];        
            }  
        );

        long long ans = 0;
        for(int i = 1; i <= n; ++i){
            int p = lower_bound(
                events.begin(), events.begin() + i - 1, events[i - 1][0],
                [](vector<int>&a, int t) {
                        return a[1] < t;        
                }) - events.begin();
                
            if(p && events[p - 1][1] == events[i - 1][0]) --p;
            for(int j = 1; j <= min(k, i); ++j){
                dp[i][j] = max(dp[i - 1][j], dp[p][j - 1] + events[i - 1][2]);
                ans = max(ans, dp[i][j]);
            }
        }

        return ans;
    }
};