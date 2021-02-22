/*
 *  1728. Cat and Mouse II
 *  题意：有一个rows *cols的grid。每个位置可能是'.','#','C','M','F'中的一个。'.'表示空地，'#'表示墙。'C','M','F'分别表示猫，老鼠，食物
 *      猫老鼠和食物只有一个。猫和老鼠轮流在grid中移动，如果猫先到达食物或者猫和老鼠在同一个位置或者1000步内老鼠没有到达食物位置，则猫赢。
 *      老鼠先走然后猫后走，每次可以沿上下左右4个方向移动，猫和老鼠有最大移动距离，每次猫不能超过catJump,老鼠不能超过mouseJump。且移动不能翻过墙
 *      问老鼠能否赢得比赛
 *  题解：博弈论。dp或者bfs。动态规划的相关题解见https://leetcode-cn.com/problems/cat-and-mouse-ii/solution/。
 *      bfs思路如下。首先设step[0][i][j][k][p]为当前为mouse turn时，mouse位于i,j位置，cat位于k,p位置时赢得比赛的最少步骤。
 *            而step[1][i][j][k][p] 为cat turn时， mouse位于i,j位置，cat位于k,p位置时mouse赢得比赛的最多步骤（大于等于一千就是mouse输）
 *          全部初始化为大于等于1000的值，然后由终止状态往前推。可以用队列进行优化，像spfs那样。
 */

class Solution {
    int n, m;
    int step[2][8][8][8][8];
    vector<array<int,2>> dirs = {{1, 0},{-1, 0},{0, 1},{0, -1}};

public:
    bool canMouseWin(vector<string>& grid, int cat_jump, int mouse_jump) {
        n = grid.size();
        m = grid[0].size();

        array<int,2> mouse_init, cat_init;
        memset(step, 0x3f, sizeof(step));
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j){
                switch(grid[i][j]){
                    case 'M': mouse_init = {i, j}; break;
                    case 'C': cat_init = {i, j}; break;
                }
                if(grid[i][j] == '#') continue;

                for(int k = 0; k < n; ++k){
                    for(int p = 0; p < m; ++p){
                        if(grid[k][p] == '#') continue;
                        if(grid[i][j] == 'F' && grid[k][p] != 'F'){
                            step[1][i][j][k][p] = 0;
                        } 
                    }
                }
            }
        }

        bool updated = true;
        for(int t = 1; updated; t^=1) { //最多一千次
            updated = false;
            for(int i = 0; i < n; ++i){
                for(int j = 0; j < m ; ++j){
                    if(grid[i][j] == '#') continue;
                    for(int k = 0; k < n; ++k){
                        for(int p = 0; p < m; ++p){
                            if(grid[k][p] == '#') continue;

                            if(grid[i][j] == 'F' || grid[k][p] == 'F' || (i == k && j == p)){
                                continue;
                            }
                            
                            if(t & 1){ //mouse turn
                                int min_step = 1000;
                                for(int d = 0; d < 4; ++d){
                                    for(int s = 1; s <= mouse_jump; ++s){
                                        int next_i = i + dirs[d][0] * s;
                                        int next_j = j + dirs[d][1] * s;

                                        if(next_i < 0 || next_j < 0 || next_i >= n || next_j >= m || grid[next_i][next_j] == '#') break;
                                        min_step = min(min_step, step[1][next_i][next_j][k][p] + 1);
                                    }
                                }
                                if(min_step < 1000 && min_step < step[0][i][j][k][p]) {
                                    step[0][i][j][k][p] = min_step;
                                    updated = true;
                                }
                            } else{
                                int max_step = step[0][i][j][k][p] + 1;
                                for(int d = 0; d < 4 && max_step < 1000; ++d){
                                    for(int s = 1; s <= cat_jump && max_step < 1000; ++s){
                                        int next_k = k + dirs[d][0] * s;
                                        int next_p = p + dirs[d][1] * s;
                                        if(next_k < 0 || next_p < 0 || next_k >= n || next_p >= m || grid[next_k][next_p] == '#') break;
                                        max_step = max(max_step, step[0][i][j][next_k][next_p] + 1) ;
                                    }
                                }
                                if(max_step < 1000 && step[1][i][j][k][p] != max_step) {
                                    updated = true;
                                    step[1][i][j][k][p] = max_step;
                                }
                            }


                        }
                    }

                }
            }
       }

        return step[0][mouse_init[0]][mouse_init[1]][cat_init[0]][cat_init[1]] < 1000;

    }
};