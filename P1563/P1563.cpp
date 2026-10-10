#include <cstdio>
struct xxx {
    int y;
    char x[15];
}a[150000];
int n, m, i, j, p = 1, x, y;
int main() {
    scanf("%d %d", &n, &m);
    for (i = 1; i <= n; i++)
        scanf("%d %s", &a[i].y, a[i].x);
    for (i = 1; i <= m; i++) {
        scanf("%d %d", &x, &y);
        if ((a[p].y + x) % 2 == 1) {
            p += y;
            if (p > n) p -= n;
        }
        else {
            p -= y;
            if (p < 1) p += n;
        }
    }
    printf("%s", a[p].x);
    return 0;
}