/*
 * 1766. Tree of Coprimes
 * 题意：给定一棵树，0为根节点，节点编号0到n，每个节点有对应一个数字nums[i]（范围1到50）。
 *  对于每个节点i，求最近的祖先x满足nums[x] 和nums[i]互素。结果输出为一个数组。
 * 
 * 题解：由于nums的数字只有1到50，所以可以在dfs时，用一个栈保存每个数对应的节点和编号
 *    后进的在栈顶，所以只要对比所有在栈顶且和nums[i]互素的数中，对应节点深度最大的即为答案
 *  
 * 扩展：本题中nums比较小，如果nums范围更大一点，比如1e5的范围，那么可以用栈+素数分解法
 *  
 */


class Solution {
    vector<int> coprime[51];
    int gcd(int a, int b){
        return b ? gcd(b, a % b) : a;
    }

    stack<pair<int,int>> st[51];
    void dfs(int x, int fa, int depth, vector<int> graph[], vector<int> &nums, vector<int> &ans){
        
        int cur_dep = -1;
        for(int cp: coprime[nums[x]]){
            if(!st[cp].empty() && st[cp].top().second > cur_dep){
                cur_dep = st[cp].top().second;
                ans[x] = st[cp].top().first;
            }
        }

        st[nums[x]].push({x, depth});
        for(int v: graph[x]){
            if(v != fa){
                dfs(v, x, depth + 1, graph, nums, ans);
            }
        }
        
        st[nums[x]].pop();
    }


public:
    vector<int> getCoprimes(vector<int>& nums, vector<vector<int>>& edges) {
        
        int n = nums.size();
        vector<int> graph[n];
        vector<int> ans(n, -1);

        for(int i = 1; i <= 50; ++i){
            for(int j = 1; j <= 50; ++j){
                if(gcd(i, j) == 1){
                    coprime[i].push_back(j);
                }
            }
        }
        
        for(vector<int> &e : edges){
            graph[e[0]].push_back(e[1]);
            graph[e[1]].push_back(e[0]);
        }
        
        dfs(0, -1, 0, graph, nums, ans);

        return ans;
    }
};