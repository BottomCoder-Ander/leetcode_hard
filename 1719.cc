
/* 1719. Number Of Ways To Reconstruct A Tree
 * 题意：给你一个不重复数组 pairs ，其中 pairs[i] = [xi, yi]， xi < yi。
 *   令 ways 为满足下面条件的有根树的方案数：
 *              树所包含的所有节点值都在 pairs 中。
 *              一个数对 [xi, yi] 出现在 pairs 中 当且仅当 xi 是 yi 的祖先或者 yi 是 xi 的祖先
 * 如果ways <= 1，返回ways，ways > 1返回2
 *
 * 题解：首先，如果一个节点x为根，则其为所有节点的祖先，所以pairs中有x的对有n - 1（n为节点数）。
 * 这些pair构成一棵树，当且仅当每个节点的度数（pair数）为其子节点的个数加上其祖先的个数。
 * 所以如果一个节点是另一个节点的后代，则其度数小于它。并且树的数目大于1，当且仅当存在一个节点和它的儿子度数相同。
 * 可以按照度数排序，从大到小。第一个必须为n - 1。先确定根，然后根的儿子，从顶至下依次确定关系，判断其是否能构成一棵树且是否树唯一。
 * 
 */
class Solution {
    vector<int> to[501], d2n[501];
    bool vis[501];
    int degree[501], par[501];

public:
    int checkWays(vector<vector<int>>& pairs) {

        memset(degree, 0, sizeof(degree));
        memset(vis, 0, sizeof(vis));
        memset(par, -1, sizeof(par));

        for (vector<int> &p : pairs) {
            to[p[0]].push_back(p[1]);
            to[p[1]].push_back(p[0]);
            degree[p[0]]++;
            degree[p[1]]++;
        }

        int cnt = 0, max_degree = 0;
        for(int i = 1; i <= 500; ++i){
            if(degree[i]){
                ++cnt;
                max_degree = max(degree[i], max_degree);
                d2n[degree[i]].push_back(i);
            }
        }

        if(max_degree != cnt - 1) return 0;
        int ans = 1;
        for(int d = max_degree; d >= 1; --d){
            for(int u: d2n[d]){
                vis[u] = true;
                for(int v: to[u]) {
                    if(vis[v]) continue;
                    if(degree[v] == degree[u]) ans = 2;
                    if(par[u] != par[v]) return 0;
                    par[v] = u;
                }
            }
        }
        return ans;
    }
};