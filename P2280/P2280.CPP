#include <cstdio>
#include <iostream>
using namespace std;
int n, m, ans, v[5010][5010], a[5010][5010];
int main() {
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; i++) {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        v[x + 1][y + 1] += z;
    }
    for(int i = 1; i <= 5001; i++) {
        for(int j = 1; j <= 5001; j++) {
            a[i][j] = a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1] + v[i][j];
        }
    }
    for(int i = m; i <= 5001; i++) {
        for(int j = m; j <= 5001; j++) {
            int cnt = a[i][j] - a[i - m][j] - a[i][j - m] + a[i - m][j - m];
            ans = max(ans, cnt);
        }
    }
    printf("%d", ans);
    return 0;
}