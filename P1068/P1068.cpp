#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int n, m;
struct vo {
    int k, s;
}a[5050];

bool cmp(vo a, vo b) {
    if (a.s != b.s) return a.s > b.s;
    return a.k < b.k;
}
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &a[i].k, &a[i].s);
    }
    sort(a + 1, a + n + 1, cmp);
    int line = m * 1.5;
    while (a[line].s == a[line + 1].s) {
        line++;
    }
    printf("%d %d\n", a[line].s, line);
    for (int i = 1; i <= line; i++) {
        printf("%d %d\n", a[i].k, a[i].s);
    }
    return 0;
}