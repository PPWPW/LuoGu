#include <cstdio>
#include <algorithm>
    using namespace std;
int n, m, a[500005], ans;

int check(int x) {
    int last = a[1], cnt = 0;
    for (int i = 2; i <= n; i++) {
        if (a[i] - last < x)
            cnt++;
        else
            last = a[i];
    }
    return cnt;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    sort(a + 1, a + n + 1);
    int l = a[1], r = a[n], mid;
    while (l <= r) {
        mid = (l + r) / 2;
        if (check(mid) <= n-m)
            ans = mid, l = mid + 1;
        else
            r = mid - 1;
    }
    printf("%d", ans);
    return 0;
}
