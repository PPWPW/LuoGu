#include <cstdio>
using namespace std;
int len, n, m, a[50005], ans;

int check(int x) {
    int last = 0, cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (a[i] - last < x)
            cnt++;
        else
            last = a[i];
    }
    return cnt;
}

int main() {
    scanf("%d %d %d", &len, &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    a[++n] = len;
    int l = 1, r = len, mid;
    while (l <= r) {
        mid = (l + r) / 2;
        if (check(mid) <= m)
            ans = mid, l = mid + 1;
        else
            r = mid - 1;
    }
    printf("%d", ans);
    return 0;
}
