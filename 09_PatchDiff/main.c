#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 6

void gen(int h, int w, char maze[h][w]) {
    for (int i = 0; i < h; i++) 
        for (int j = 0; j < w; j++)
            maze[i][j] = (i%2 && j%2) ? '.' : '#';
    
    int n = (h-1)/2;
    int v[n][n]; 
    memset(v, 0, sizeof(v));
    
    int d[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
    int sx = rand()%n, sy = rand()%n;
    int stack[n*n][2], top = 0;
    
    stack[top][0] = sx; 
    stack[top][1] = sy; 
    v[sx][sy] = 1;
    
    while (top >= 0) {
        int x = stack[top][0], y = stack[top][1];
        int nd[4] = {0,1,2,3};
        
        for (int i = 3; i > 0; i--) {
            int j = rand()%(i+1);
            int t = nd[j];
            nd[j] = nd[i]; 
            nd[i] = t;
        }
        
        int moved = 0;
        for (int k = 0; k < 4; k++) {
            int nx = x + d[nd[k]][0];
            int ny = y + d[nd[k]][1];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && !v[nx][ny]) {
                maze[2*x+1 + d[nd[k]][0]][2*y+1 + d[nd[k]][1]] = '.';
                top++;
                stack[top][0] = nx; 
                stack[top][1] = ny;
                v[nx][ny] = 1;
                moved = 1;
                break;
            }
        }
        if (!moved) top--;
    }
}

int main() {
    srand(time(NULL));
    int h = 2*N+1, w = 2*N+1;
    char maze[h][w];
    
    gen(h, w, maze);
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) 
            putchar(maze[i][j]);
        putchar('\n');
    }
    return 0;
}
