#include <cstdio>
using namespace std;
int n, ans, a[5050], f[5050];
int max(int a, int b) {
    return a > b ? a : b;
}
int  main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    f[1] = 1;
    for (int i = 2; i <= n; i++) {
        f[i] = 1;
        for (int j = 1; j < i; j++) {
            if (a[j] <= a[i]) {
                f[i] = max(f[i], f[j] + 1);
                ans = max(ans, f[i]);
            }
        }
    }
    printf("%d", ans);
    return 0;
}