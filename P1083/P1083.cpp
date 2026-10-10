#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cstring>
using namespace std;
int n, m;
int r[1000010], d[1000010], s[1000010], j[1000010];
long long a[1000010], b[1000010];
bool check(int x) {
    memset(a, 0, sizeof(a));
    for (int i = 1; i <= x; i++) {
        a[s[i]] += d[i];
        a[j[i] + 1] -= d[i];
    }
    for (int i = 1; i <= n; i++) {
        b[i] = b[i - 1] + a[i];
        if (b[i] > r[i]) return 0;
    }
    return 1;
}
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &r[i]);
    for (int i = 1; i <= m; i++)
        scanf("%d %d %d", &d[i], &s[i], &j[i]);
    int l = 1, r = m;
    if (check(m)) {
        printf("0");
        return 0;
    }
    while (l < r) {
        int mid = (l + r) / 2;
        if (check(mid)) l = mid + 1;
        else r = mid;
    }
    printf("-1\n%d", l);
    return 0;
}

