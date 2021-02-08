/*
 * 1723. Find Minimum Time to Finish All Jobs
 * 题意：有一系列工作，jobs[i]表示第i项工作需要花费的时间。现在有k个工人。
 *  把工作分配给这k个人，每项工作只能分配给一个人，问工人的最大工作时间最短是多少？
 * 题解1：直接使用状态压缩动态规划。设dp[i][s]为前i个工人，完成的工作s 最大工作时间最短时长。
 *          通过枚举第i个人完成的工作进行状态转移
 * 
 * 解法2：二分最大时长，检验能否能把工作安排完使得每个工人的最大工作时间不超过限制。
 *        问题转化为，能否把工作安排给k个工人，使得每个工人工作时间不超过limit。
 *        设dp[s]为完成工作集s最少需要多少个工人，则可以通过枚举下一个工人的工作集递推。
 *        但是这样需要枚举子集，可以进一步优化。dp[s]个工人，完成工作s，存在一个次序安排工作，使得最后一个人工作时长最少。
 *        所以可以设dp[s][0]为完成工作集s最少需要多少个工人。
 *        进一步，设dp[s][1]为dp[s][0]完成工作集s时，最后一个人最短的工作时长。
 *        则dp[s][0]以及dp[s][1]可以通过枚举最后一个安排的工作进行转移，而不是枚举子集
 * 
 *        由于答案一定是jobs子集的和，所以二分时，可以只二分子集的和。
 * 解法3：dfs + 剪枝
 *      这个有各样的写法。见如下两份代码
 *
 * 解法4：模拟退火
 *      
 */



/* 解法1代码 */
class Solution {
public:
    int minimumTimeRequired(vector<int>& jobs, int k) {
        int n = jobs.size();
        int dp[k + 1][1 << n];

        for(int i = 0; i < (1 << n); ++i){
            int sum = 0;
            for(int j = 0; j < n; ++j){
                if(i & (1 << j)){
                    sum += jobs[j];
                }
            }
            dp[1][i] = sum;
        }

        
        for(int i = 2; i <= k; ++i){
            for(int s = 0; s < (1 << n); ++s){
                dp[i][s] = dp[1][s];
                for(int j = s; j > 0; j = (j - 1) & s){
                    dp[i][s] = min(dp[i][s], max(dp[i - 1][s ^ j], dp[1][j]));
                }
            }
        }

        return dp[k][(1 << n) - 1];
    }
};

/* 解法2 代码 */
class Solution {
public:
    int minimumTimeRequired(vector<int>& jobs, int k) {
        int n = jobs.size();
        int mask = 1 << n;
        vector<int> valid_subset;
        unordered_set<int> vis;
        
        int max_ele = *max_element(jobs.begin(), jobs.end());
        for (int i = 0; i < mask; ++i){
            int sum = 0;
            for (int j = 0; j < n; ++j){
                if (i & (1 << j))
                    sum += jobs[j];
            }
            if(sum >= max_ele && vis.count(sum) == 0){
                vis.insert(sum);
                valid_subset.push_back(sum);
            }
        }
        
        sort(valid_subset.begin(), valid_subset.end());
        
        auto canDo = [&](int limit) {
            vector<array<int, 2>> dp(mask, {k + 1, 0});
            dp[0] = {0, limit};
            for (int i = 0; i < mask; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i & (1 << j))
                        continue;
                    array<int,2> nxt;
                    if(dp[i][1] + jobs[j] <= limit)
                        nxt[0] = dp[i][0], nxt[1] = dp[i][1] + jobs[j];
                    else 
                        nxt[0] = dp[i][0] + 1, nxt[1] = jobs[j];
                    dp[i ^ (1 << j)] = min(dp[i ^ (1 << j)], nxt);
                }
            }
            return dp[mask - 1][0] <= k;
        };

        int lo = 0, hi = valid_subset.size() - 1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            if (canDo(valid_subset[mid]))
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return valid_subset[lo];
    }
};


/* 解法3的代码 */
class Solution {
    int ans = -1;
    vector<int> time;
    vector<int> jobs;
    int k;
public:
    int minimumTimeRequired(vector<int>& jobs,int k) {
    
        int n = jobs.size();
        time = vector<int>(k, 0);
        this->k = k;
        this->jobs.swap(jobs);
        ans = accumulate(this->jobs.begin(), this->jobs.end(), 0);

        dfs(0,0, -1);
        return ans;
    }
    //第i个任务，第j个工人
    void dfs(int i, int max_time, int min_idx) {
        //cout<<i<<endl;
        if (i == jobs.size()) {                     //到达叶子结点，得到一个可行解
            ans = max_time;        //比较得到一个最优解
            return;
        }
        
        //优先选择工作量最小的安排
        if(min_idx == -1){
            min_idx = 0;
            for(int w = 1; w < min(k, i + 1); ++w){
                if(time[w] < time[min_idx]){
                    min_idx = w;
                }
            }
        } else{
            if(i < k && time[min_idx] > time[i]){
                min_idx = i;
            }
        }

        if(time[min_idx] + jobs[i] >= ans) return;
        time[min_idx] += jobs[i];
        dfs(i + 1, max(max_time, time[min_idx]), -1);
        time[min_idx] -= jobs[i];

        for (int w = 0; w < min(k, i + 1); ++w) {            //job[i]分配给工人w
            if(w == min_idx || time[w] + jobs[i] >= ans)     //剪枝:如果工人w的时间超过答案,不可能是更优答案
                continue;
            time[w] += jobs[i];           
            dfs(i + 1, max(max_time, time[w]), min_idx);
            time[w] -= jobs[i];           //回溯
        }
    }
};

/* 解法3的不同写法（最快） */
class Solution {
    int ans = -1;
    vector<int> time;
    vector<int> jobs;
    int k;
public:
    int minimumTimeRequired(vector<int>& jobs,int k) {
    
        int n = jobs.size();
        time = vector<int>(k, 0);
        this->k = k;
        this->jobs.swap(jobs);
        ans = accumulate(this->jobs.begin(), this->jobs.end(), 0);

        dfs(0,0, -1);
        return ans;
    }
    //第i个任务，第j个工人
    void dfs(int i, int max_time, int min_idx) {
        //cout<<i<<endl;
        if (i == jobs.size()) {                     //到达叶子结点，得到一个可行解
            ans = max_time;        //比较得到一个最优解
            return;
        }
        
        if(min_idx == -1){
            min_idx = 0;
            for(int w = 1; w < min(k, i + 1); ++w){
                if(time[w] < time[min_idx]){
                    min_idx = w;
                }
            }
        } else{
            if(i < k && time[min_idx] > time[i]){
                min_idx = i;
            }
        }

        if(time[min_idx] + jobs[i] >= ans) return;
        time[min_idx] += jobs[i];
        dfs(i + 1, max(max_time, time[min_idx]), -1);
        time[min_idx] -= jobs[i];

        for (int w = 0; w < min(k, i + 1); ++w) {            //job[i]分配给工人w
            if(w == min_idx || time[w] + jobs[i] >= ans)     //剪枝:如果工人w的时间超过答案,不可能是更优答案
                continue;
            time[w] += jobs[i];           
            dfs(i + 1, max(max_time, time[w]), min_idx);
            time[w] -= jobs[i];           //回溯
        }
    }
};

/* 解法4代码 
 *   作者：wangdh15
 *   链接：https://leetcode-cn.com/problems/find-minimum-time-to-finish-all-jobs/solution/mo-ni-tui-huo-sui-ji-hua-pian-fen-fa-tao-ksfz/
 */
 class Solution {
 int kk, n;
    int w[N], s[N];
    int ans = 2e9;

    int calc() {
        for (int i = 0; i < kk; i ++) s[i] = 0;
        for (int i = 0; i < n; i ++) {
            int k = 0;
            for (int j = 1; j < kk; j ++) {
                if (s[j] < s[k]) k = j;
            }
            s[k] += w[i];
        }
        int res = 0;
        for (int i = 0; i < kk; i ++) res = max(res, s[i]);
        ans = min(ans, res);
        return res;
    }

    void simulate_anneal() {


        random_shuffle(w, w + n);
        for (double t = 1e4; t > 1e-4; t *= 0.95) {
            int a = rand() % n, b = rand() % n;
            int resa = calc();
            swap(w[a], w[b]);
            int resb = calc();
            int dt = resb - resa;
            if (exp(-dt / t) > rand() / RAND_MAX) continue;
            swap(w[a], w[b]);
        }


    }

    int minimumTimeRequired(vector<int>& jobs, int k) {
        n = jobs.size();
        kk = k;
        for (int i = 0; i < n; i ++) w[i] = jobs[i];
        for (int i = 0; i < 100; i ++) {
            simulate_anneal();
        }
        return ans;
    }
};

