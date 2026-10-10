#include <cstdio>
#include <algorithm>
#include <iostream>
using namespace std;
int n, k, a[100010];
long long ma = -1, mi = -1;
long long acnum(long long x) {
    long long cnt=0, sum=0;
    for (int i = 1; i <= n; i++) {
        sum = max(sum + a[i], 0ll);
        if (sum >= x) sum = 0, cnt++;
    }
    return cnt;
}
int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    long long l, r;
    l = 1, r = 1e18;
    while (l <= r) {
        long long mid = (l + r) / 2;
        if (acnum(mid) <= k) {
            r = mid - 1;
            if (acnum(mid) == k) mi = mid;
        }
        else l = mid + 1;
    }
    l = 1, r = 1e18;
    while (l <= r) {
        long long mid = (l + r) / 2;
        if (acnum(mid) >= k) {
            l = mid + 1;
            if (acnum(mid) == k) ma = mid;
        }
        else r = mid - 1;
    }
    if (mi == -1) printf("-1");
    else printf("%lld %lld", mi, ma);
    return 0;
}
