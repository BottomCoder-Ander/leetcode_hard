/*  https://leetcode-cn.com/problems/cat-and-mouse-ii/
 *  1728. Cat and Mouse II
 *  题意：有一个rows *cols的grid。每个位置可能是'.','#','C','M','F'中的一个。'.'表示空地，'#'表示墙。'C','M','F'分别表示猫，老鼠，食物
 *      猫老鼠和食物只有一个。猫和老鼠轮流在grid中移动，如果猫先到达食物或者猫和老鼠在同一个位置或者1000步内老鼠没有到达食物位置，则猫赢。
 *      老鼠先走然后猫后走，每次可以沿上下左右4个方向移动，猫和老鼠有最大移动距离，每次猫不能超过catJump,老鼠不能超过mouseJump。且移动不能翻过墙
 *      问老鼠能否赢得比赛
 *  题解：
 */

class Solution {
    #define idx(arr,c,m) arr[c[0]][c[1]][m[0]][m[1]]
    #define idx2(arr, u) arr[u[0]][u[1]][u[2]][u[3]]

    int n,m;
    int mouse_win[8][8][8][8], degree[8][8][8][8];
    vector<array<int,2>> dirs = {{1, 0},{-1, 0},{0, 1},{0, -1}};

    bool valid(array<int,2> pos, vector<string>&grid){
        if(pos[0] < 0 || pos[1] < 0 || pos[0] >= n || pos[1] >= m || grid[pos[0]][pos[1]] == '#')
            return false;
        return true;
    }

    int canWin(vector<string> &grid, array<int,2> &cat_idx, array<int,2> &mouse_idx, int cat_jump, int mouse_jump){
        
        int uncertain = 0;
        for(int d = 0; d < 4; ++d){
            for(int i = 0; i <= mouse_jump; ++i){
                array<int,2> mouse_pos = {mouse_idx[0] + dirs[d][0] * i, mouse_idx[1] + dirs[d][1] * i};
                if(!valid(mouse_pos, grid)) break;
                
                int lost = 0;
                int win = 1;

                for(int d2 = 0; d2 < 4; ++d2){
                    for(int j = 0; j <= cat_jump; ++j){
                        array<int,2> cat_pos = {cat_idx[0] + dirs[d2][0] * j, cat_idx[1] + dirs[d2][1] * j};
                        if(!valid(cat_pos, grid)) break;
                        if(idx(mouse_win, cat_pos, mouse_pos) != 1){
                            win = 0;
                            if(idx(mouse_win, cat_pos, mouse_pos) == 0){
                                lost = 1;
                            }
                        }
                    }
                }
                if(win) return 1;
                if(!uncertain && !lost)
                    uncertain = 1;
            }
        }
        return uncertain ? -1 : 0;
    }

public:
    bool canMouseWin(vector<string>& grid, int cat_jump, int mouse_jump) {
        n = grid.size();
        m = grid[0].size();

        array<int,2> mouse_init, cat_init, target;

        queue<array<int,4>> que;
        memset(mouse_win, -1, sizeof(mouse_win));

        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j){
                switch(grid[i][j]){
                    case 'M': mouse_init = {i, j}; break;
                    case 'C': cat_init = {i, j}; break;
                    case 'F': target = {i, j}; break;
                }
                if(grid[i][j] == '#') continue;

                int count1 = 1;
                for(int d = 0; d < 4; ++d){
                    for(int cj = 1; cj <= cat_jump; ++cj){
                        int ni = i + dirs[d][0] * cj, nj = j + dirs[d][1] * cj;
                        if(!valid({ni, nj}, grid)) break;
                        ++count1;
                    }
                }

                for(int k = 0; k < n; ++k){
                    for(int p = 0; p < m; ++p){
                        if(grid[k][p] == '#') continue;

                        int count2 = 1;
                        for(int d2 = 0; d2 < 4; ++d2){
                            for(int mj = 1; mj <= mouse_jump; ++mj){
                                int nk = k + dirs[d2][0] * mj, np = p + dirs[d2][1] * mj;
                                if(!valid({nk, np}, grid)) break;
                                ++count2;
                            }
                        }
                        degree[i][j][k][p] = count1 * count2 - 1; 
                    }
                }
            }
        }


        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j){
                
                for(int k = 0; k < n; ++k){
                    for(int p = 0; p < m; ++p){
                        if(grid[i][j] == '#' || grid[k][p] == '#') {
                            mouse_win[i][j][k][p] = 0; 
                            continue;
                        }

                        if(i == target[0] && j == target[1] || k == target[0] && p == target[1]){
                            mouse_win[i][j][k][p] = (k == target[0] && p == target[1]);
                            if(mouse_win[i][j][k][p]){
                                que.push({i, j, k, p});
                            }
                        }else if(i == k && j == p) {
                            mouse_win[i][j][k][p] = 0;
                        }
                    }                                                                                                                                          
                }
            }
        }

        if(idx(mouse_win, cat_init, mouse_init) == 1) return true;

        while(!que.empty()){
            array<int, 4> u = que.front(); que.pop();
            if(u[0] == cat_init[0] && u[1] == cat_init[1] &&
                u[2] == mouse_init[0] && u[3] == mouse_init[1]){
                return true;
            }

            for(int d = 0; d < 4; ++d){
                for(int i = 0; i <= cat_jump; ++i){
                    array<int,2> cat_pos = {u[0] + dirs[d][0] * i, u[1] + dirs[d][1] * i};
                    if(!valid(cat_pos, grid)) break;

                    for(int d2 = 0; d2 < 4; ++d2){
                        for(int j = 0; j <= mouse_jump; ++j){
                            array<int,2> mouse_pos = {u[2] + dirs[d2][0] * j, u[3] + dirs[d2][1] * j};
                            if(!valid(mouse_pos, grid)) break;
                            if(idx(mouse_win, cat_pos, mouse_pos) != -1 || i == 0 && j == 0 && (d!= 0 || d2 != 0)) continue;

                            if(--idx(degree, cat_pos, mouse_pos) != 0) continue;

                            idx(mouse_win, cat_pos, mouse_pos) = canWin(grid, cat_pos, mouse_pos, cat_jump, mouse_jump);
                            if(idx(mouse_win, cat_pos, mouse_pos) == 1)
                                que.push({cat_pos[0], cat_pos[1], mouse_pos[0], mouse_pos[1]});
                            
                        }
                    }
                }
            }
        }


        return false;

    }
};