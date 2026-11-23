#include <cstdio>
#include <iostream>
using namespace std;
int n, a[1010][1010], ans;
int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    for(int i = 2; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            a[i][j] += max(a[i - 1][j - 1], a[i - 1] [j] );
            ans = max(ans, a[i][j]);
        }
    }
    printf("%d", ans);
    return 0;
}