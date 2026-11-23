#include <algorithm>
#include <cstdio>
using namespace std;

struct House {
    int s, a;
};

const int MAX_N = 100000;
House h[MAX_N + 1];
int sum[MAX_N + 1], maxS[MAX_N + 1], best[MAX_N + 2];

bool compareByScore(const House& lhs, const House& rhs) {
    return lhs.a > rhs.a;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 1; i <= n; ++i) scanf("%d", &h[i].s);
    for (int i = 1; i <= n; ++i) scanf("%d", &h[i].a);

    sort(h + 1, h + n + 1, compareByScore);

    for (int i = 1; i <= n; ++i) {
        sum[i] = sum[i - 1] + h[i].a;
        maxS[i] = max(maxS[i - 1], h[i].s);
    }
    for (int i = n; i >= 1; --i) {
        best[i] = max(best[i + 1], h[i].a + 2 * h[i].s);
    }

    for (int x = 1; x <= n; ++x) {
        int ans = sum[x] + 2 * maxS[x];
        if (x < n) {
            ans = max(ans, sum[x - 1] + best[x + 1]);
        }
        printf("%d\n", ans);
    }
}
