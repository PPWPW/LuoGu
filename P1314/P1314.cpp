#include <cstdio>
#include <algorithm>
#include <iostream>
using namespace std;
const long long inf = 1e15;
long long n, m, s, ma = -inf, mi = inf;
long long w[200010], v[200010], a[200010], b[200010];
long long r[200010], l[200010];
int main() {
    scanf("%lld %lld %lld", &n, &m, &s);
    for (int i = 1; i <= n; i++) {
        scanf("%lld %lld", &w[i], &v[i]);
        ma = max(ma, w[i]);
        mi = min(mi, w[i]);
    }
    for (int i = 1; i <= m; i++) {
        scanf("%lld %lld", &l[i], &r[i]);
    }
    long long left = mi, right = ma, ans = inf, mid, loss, cur;
    while (left <= right) {
        cur = 0;
        mid = (left + right) / 2;
        for (int i = 1; i <= n; i++) {
            if (w[i] > mid) a[i] = a[i - 1] + 1, b[i] = b[i - 1] + v[i];
            else a[i] = a[i - 1], b[i] = b[i - 1];
        }
        for (int i = 1; i <= m; i++) {
            cur += (a[r[i]] - a[l[i] - 1]) * (b[r[i]] - b[l[i] - 1]);
        }
        loss = s - cur;
        if (loss < 0) left = mid + 1;
        else right = mid - 1;
        ans = min(ans, abs(loss));
    }
    printf("%lld", ans);
    return 0;
}
