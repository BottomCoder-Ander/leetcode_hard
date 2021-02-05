
/*
 * 1707. Maximum XOR With an Element From Array
 *  
 * 題意：給定一個非負數組nums,以及一個查詢數組queries，queries[i] = [xi, mi]。每個查詢答案是xi和nums中不超過mi的一個元素按位異或的最大值。
 * 題解：Trie樹。先把nums數組的所有數字存進Trie樹中，同時每個節點保存通過這個節點的數字中的最小值。
 *  對於一個查詢，從最高位開始（第31位），優先選擇和當前位不同的分支走，這樣才會最大化。
 *  所以先判斷和當前位不同的分支的最小值是否小於mi，如果小於等  則可以走，否則走和當前位相同的。一直到葉子，然後取異或就得到答案
 *
 */

class Solution {
    struct TreeNode{
        int min_val = INT_MAX;
        int ch[2] = {0};
    } nodes[900000];//采用預分配，而不是指針

    int cnt;

    void insert(const int x){
        int cur = 0;
        for(int i = 30; i >= 0; --i){
            nodes[cur].min_val = min(nodes[cur].min_val, x);
            int id = !!(x & (1 << i));
            if(!nodes[cur].ch[id]){
                nodes[cur].ch[id] = cnt++;
            }
            cur = nodes[cur].ch[id];
        }
        nodes[cur].min_val = x;
    }

    int find(const int x, const int mi) const {
        if(nodes[0].min_val > mi) return -1;
        int cur = 0;
        for(int i = 30; i >= 0; --i){
            bool one_bit = !!(x & (1 << i));

            if(one_bit && nodes[cur].ch[0] && nodes[nodes[cur].ch[0]].min_val <= mi ||
                !nodes[cur].ch[1] || !one_bit && nodes[nodes[cur].ch[1]].min_val > mi){
                cur = nodes[cur].ch[0];
            } else{
                cur = nodes[cur].ch[1];
            }
        }
        return x ^ nodes[cur].min_val;
    }

public:
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        cnt = 1;

        for(int num: nums){
            insert(num);
        }

        vector<int> ans(queries.size());

        for(int i = 0; i < queries.size(); ++i){
            ans[i] = find(queries[i][0], queries[i][1]);
        }
        return ans;
    }
};