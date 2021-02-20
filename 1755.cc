/*
 * 1755. Closest Subsequence Sum
 *  题意：给定一个数组nums，和一个数goal。求最小化abs(sum - goal)。sum是nums子序列的和
 *  题解：由于nums至多只有40个数，可以将其分为两组，每组至多20个。两组分别枚举计算子集的和（包括空集）。
 *      问题就成了，在这两组子集和分别取一个数，加起来和goal的绝对值差最小。而这个可以通过双指针解决
 */

class Solution {
public:
    int minAbsDifference(vector<int>& nums, int goal) {
        if(nums.size() == 1) return min(abs(goal), abs(nums[0] - goal));

        int n = nums.size() / 2;
        int m = nums.size() - n;

        unordered_map<long long, int> p2id;
        for(int i = 0; i < m; ++i){
            p2id[1 << i] = i;
        }

        vector<long long> sum1(1 << n, 0), sum2(1 << m, 0);

        for(int i = 1; i < (1 << n); ++i){
            sum1[i] = sum1[i & (i - 1)] + nums[p2id[i & (-i)]];
        }

        for(int i = 1; i < (1 << m); ++i){
            sum2[i] = sum2[i & (i - 1)] + nums[p2id[i & (-i)] + n];
        }

        sort(sum1.begin(), sum1.end());
        sort(sum2.begin(), sum2.end());

        int left = 0, right = (1 << m) - 1;
        long long ans = abs(goal);
        int cur_sum = 0;


        while(left < (1 << n) && right >= 0 && ans != 0){
            ans = min(ans, abs(goal - (sum1[left] + sum2[right])));
            if(left == (1 << n) - 1) {
                right--;
            } else if (right == 0) {
                left++;
            } else if(sum1[left] + sum2[right] > goal){
                right--;
            } else{
                left++;
            }
        }
        return ans;
    }
};