/* 1745. Palindrome Partitioning IV
 * 题意：给定一个字符串，问是否能够分割成三个非空回文串
 * 题解：O(n^2)的解法比较容易想到，先用manacher算法先处理一下。然后枚举第一个回文串的阶数位置和第三个回文串的起始位置，然后判断中间是否为回文串即可
 *      也有更优的算法O(n)解决。
 *      基于以下事实：如果一个串能分解成两个非空回文串，那么要么第一个回文串可以是最长回文前缀，要么第二个串是最长回文后缀。
 *      具体见https://leetcode-cn.com/problems/palindrome-partitioning-iv/solution/manacherxian-xing-shi-jian-fu-za-du-by-h-sj24/
 *      https://www.luogu.com.cn/blog/user25308/proof-cf1081h
 * */

class Solution {
    char s_new[4004];//存添加字符后的字符串
    int s_new_len; 
    int p[4003], right[4003];

    void init(string &s) {
        s_new[0]='$';
        s_new[1]='#';
        s_new_len = 2; 
        for(char c: s) {
            s_new[s_new_len++] = c;
            s_new[s_new_len++] = '#';
        } 
        s_new[s_new_len] = '\0';
    }

    void ManacherAlgorithm(string &s) {
        init(s);
        int id;
        int mx = 0;
        memset(right, 0, sizeof(right));
        for(int i = 1; i <= s_new_len; ++i) {
            if(i < mx) {
                p[i] = min(p[2 * id - i], mx - i);
            }else{ 
                p[i] = 1;
            }
            while(s_new[i - p[i]] == s_new[i + p[i]]) p[i]++;

            if(mx < i + p[i]) {
                id = i;
                mx = i + p[i];
            }

            right[i - p[i] + 1] = max(right[i - p[i] + 1], p[i] - 1);

        }

    }


 
public:
    bool checkPartitioning(string s) {
        ManacherAlgorithm(s);
        int n = s_new_len;
        
        auto check = [&](int left, int right){
            int m = (left + right) / 2;
            return m + p[m] > right;
        };
        
        for(int i = 3; i < n; i += 2) right[i] = max(right[i], right[i - 2] - 2);
        stack<int> palindromic_suffix;
        for(int i = n - 2; i > 1; i -= 2) if(check(i, n - 2)) palindromic_suffix.push(i);
        for(int i = 2; i < n; i += 2) {
            if(!check(2, i)) continue;
            while(!palindromic_suffix.empty() && palindromic_suffix.top() <= i + 2) palindromic_suffix.pop(); 
            //後綴
            if(!palindromic_suffix.empty() && check(i + 2, palindromic_suffix.top() - 2)) return true;
            int j = i + right[i + 1] * 2;      
            
            //前綴     
            if(j < n - 2 && check(j + 2, n - 2)) return true;
        }
        
        return false;


    }
};