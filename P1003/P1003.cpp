#include <cstdio>
using namespace std;
int a[10005], b[10005], g[10005], k[10005];

int main() {
    int n, x, y;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &a[i], &b[i], &g[i], &k[i]);
    }
    scanf("%d %d", &x, &y);
    int ans = -1;
    for (int i = 0; i < n; i++) {
        if (x >= a[i] && y >= b[i] && x <= a[i] + g[i] && y <= b[i] + k[i]) {
            ans = i + 1;
        }
    }
    printf("%d\n", ans);
    return 0;
}
