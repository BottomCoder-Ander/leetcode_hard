/*
 * 1735. Count Ways to Make Array With Product
 * 题意：有一个数组queries，queries[i] = [ni,ki]表示一个查询，计算能够构造多少个长度为ni的正整数序列，使得序列的乘积为ki。将所有查询结果进行返回
 * 题解：素因子分解+组合数
 *      序列乘积为ki说明每一项是ki的因子。而不同的序列就是乘积为ki的因子的排列数。    
 *   将ki进行素因子分解，排列总数就是每个素因子排列组合数的乘积。设因子x的个数为num。则将这num个x分配到ni个位置的组合数为C[ni + num - 1][num]
 *     其实就是ni个有序的盒子，放num个相同的球的组合数，采用挡板法可以得到。
 *   所以问题就是如何计算组合数。由于num会很小，所以可以采用预处理，或者边用边计算。计算时，可以采用递推式，也可采用公式以及乘法逆元计算
 */
class Solution {
#define M 1000000007
#define PRIME_NUM 25
//int C[10014][14];
int inv[14];//, mul[14]; 

int getInv(int n){
    int res = 1;
    int x = M - 2;

    do{
        if(x & 1){
            res  = (long long)res * n % M;
        }
        n = (long long)n * n % M;
    }while(x >>= 1);
    return res;
}

void init(){
    inv[1] = 1; //= mul[1] = 1;
    for(int i = 2; i <= 13; ++i){
        //mul[i] = (long long)mul[i - 1] * i % M;
        inv[i] = (long long)inv[i - 1] * getInv(i) % M;
    }
}

inline int Comb(int n, int k){
    //使用递归计算组合数，也可以预先处理
    // int &res = C[n][k];
    // if(res != -1) return res;
    // if(n == k) return res = 1;
    // if(k == 1 || k == n - 1) return res = n;
    // return res = (Comb(n - 1, k) + Comb(n - 1, k - 1)) % M;

    //直接使用公式和逆元。比上面还慢
    //return (((long long)mul[n] * inv[n - k]) % M) * inv[k] % M;

    //使用公式+部分使用逆元。评判系统运行最快
    int res = inv[k];
    for(int i = n - k + 1; i <= n; ++i){
        res = (long long)res * i % M;
    }
    return res;
}

public:
    vector<int> waysToFillArray(vector<vector<int>>& queries) {
        init();
        int prime[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
        vector<int> ans;
        //memset(C, -1, sizeof(C));
        for(vector<int> &q : queries){
            if(q[0] == 1) {
                ans.push_back(1);
                continue;
            }
            
            int res = 1;

            for(int i = 0; i < PRIME_NUM; ++i){
                if(q[1] % prime[i]) continue;
                int count = 0;
                do{
                    q[1] /= prime[i];
                    ++count;
                }while(q[1] % prime[i] == 0);
                res = ((long long)res * Comb(q[0] + count - 1, count)) % M;
            }

            if(q[1] != 1) res = ((long long)res * q[0]) % M;
            ans.push_back(res);
        }
        return ans;
    }
};