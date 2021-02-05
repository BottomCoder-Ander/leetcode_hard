/*
 * 1703. Minimum Adjacent Swaps for K Consecutive Ones
 * 題意：给你一个01数组 nums 和一个整数 k . 每一次移动，你可以选择相邻两个数字并将它们交换。計算使 nums 中包含 k 个 连续 1 的 最少 交换次数
 * 
 * 題解：k個不同位置的1，將他們聚集在一起的最小代價就是將他們往中間那個1移動（同郵局選址，如果中間有兩個，則兩個之一以及之間的位置都可以）。
 *      設n2為左邊的1的數量，n1為右邊的1的數量（n1 = k /2 ,n2 = (k - 1) / 2）。
 *        枚舉中間1的位置，計算其他位置到這個位置的移動距離。
 *       移動距離的計算：將i左邊的lpos1,lpos2...,lpost位置的1移動到跟i連續的位置，所需的移動次數為
 *         (i - lpost - 1) + (i - lpos_{t-1} - 2) + ... + (i - lpos1 - t) = t * i -  \sum lpos - (1 + t) t / 2
 *       同樣的，右邊rpos1, rpos2....,rposx 移動到i連續位置所需移動次數為
 *         (rpos1 - i - 1) + ... + (rposx - i - x) = \sum rpos - x * i - (1 + x) x / 2
 *      可以預先求位置的前綴和sum，則儅枚舉中間位置為pos[i]時，
 *                      左邊的移動次數為n2 * pos[i] - (sum[i - 1] - sum[i - n2 - 1]) - (1 + n2) * n2 / 2
 *                      右邊的移動次數為(sum[i + n1] - sum[i]) - n1 * pos[i] - (1 + n1) * n1 / 2 
 *                  總的移動次數為 sum[i + n1] - sum[i] - sum[i - 1] + sum[i - n2 - 1]  + pos[i] * (n2 - n1) - n1(n1 + 1)/2 - n2(n2+1)/2
 *                  其中 n1(n1 + 1)/2 - n2(n2+1)/2和位置無關，可以留在最後再加上
 * 由於儅包括第一個1時，上面下標可能為-1，所以實現中將sum整體向右移動一位，令sum[0] = 0。
 */


class Solution {
public:
    int minMoves(vector<int>& nums, int k) {

        //保存所有1的位置
        vector<int> pos;
        for(int i = 0; i < nums.size(); ++i){
            if(nums[i]) pos.push_back(i);
        }

        if(k == 1) return 0;

        
        //前綴和
        long long sum[pos.size() + 1];
        sum[0] = 0;
        for(int i = 1; i <= pos.size(); ++i){
            sum[i] = sum[i - 1] + pos[i - 1];
        }

        long long res = 1000000000000000LL;
        int n1 = k / 2, n2 = (k - 1) / 2;

        //枚舉中間位置
        for(int i = pos.size() - n1 - 1; i >= n2; --i){
            res = min(res, sum[i + n1 + 1] - sum[i + 1] - sum[i] + sum[i - n2]  + pos[i] * (n2 - n1));
        }

        return res - n1 * (n1 + 1LL) /2 - n2 * (n2 + 1LL) / 2;
    }
};