#include <cstdio>
#include <iostream>
using namespace std;
int a[110][110], dp[110][110];
int n, m, ans;
int main() {
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            if(a[i][j] == 1) dp[i][j] = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
            ans = max(ans, dp[i][j]);
        }
    }
    printf("%d", ans);
    return 0;
}