/*
 *  1713. Minimum Operations to Make a Subsequence
 *  題意：給定一個包含不重複整數的數組target，以及另一個可能重複的整數數組arr
 *      每次操作，可以往arr的任意一個位置插入一個整數。問至少多少次操作可以使得target為arr的一個子序列
 *  题解：转化为最长上升子序列。要使得插入最少，则说明保留的最多，即target子序列最多和arr的子序列相同的最长长度。插入的等于总长度减去原有的。
 *      将arr中不在target中出现的数删掉，而在target中出现的，转化成在target中的位置。则求子序列的最长长度，相当于求这个位置数组的最长上升子序列。
 */
class Solution {
    int g[100002];

    //用lower_bound会慢一些
    inline int search(const int len, const int a){
        int left = 0, right = len - 1, mid;
        while(left <= right){
            mid = (left + right) / 2;
            if(g[mid] < a){
                left = mid + 1;
            } else{
                right = mid - 1;
            }
        }

        return g[left] >= a ? left : left + 1;
    }

    int longestIncresingSubsequence(const vector<int>& arr){
        if(arr.size() == 0) return 0;
        memset(g, 0x3f, arr.size() * sizeof(int));
        int len = 1;
        for(int i = 0; i < arr.size(); ++i){
            int d = search(len, arr[i]);
            g[d] = arr[i];
            if(len == d + 1) ++len;
        }
        return len - 1;
    }

public:
    int minOperations(vector<int>& target, vector<int>& arr) {
        unordered_map<int,int> t2id;

        for(int i = 0; i < target.size(); ++i){
            t2id[target[i]] = i;
        }

        //这么做只是为了看起来更清晰，如果想要更快的效率，则去掉这一步，直接在这里边判断，边求最长序列
        //重复利用arr数组来存位置
        int idx = 0;
        for(int i = 0; i < arr.size(); ++i){
            if(t2id.count(arr[i])){
                arr[idx++] = t2id[arr[i]];
            }
        }

        arr.resize(idx);
        return target.size() - longestIncresingSubsequence(arr);
    }
};