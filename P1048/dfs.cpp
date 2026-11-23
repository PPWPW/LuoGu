#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
int n, t;
int cost[103], va[103];
int mem[103][1003];
int dfs(int num, int left) {
    if (mem[num][left] != -1)
        return mem[num][left];
    if (num == n + 1) return mem[num][left] = 0;
    int dfs1, dfs2 = -1E9;
    dfs1 = dfs(num + 1, left);
    if (left >= cost[num])
        dfs2 = dfs(num + 1, left - cost[num]) + va[num];
    return mem[num][left] = max(dfs1, dfs2);
}
int main() {
    memset(mem, -1, sizeof(mem));
    scanf("%d %d", &t, &n);
    for (int i = 1; i <= n; i++)
        scanf("%d %d", &cost[i], &va[i]);
    printf("%d", dfs(1, t));
    return 0;
}