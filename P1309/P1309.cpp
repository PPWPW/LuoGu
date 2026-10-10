#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
struct player {
    int s, w, n;
}a[200010], temp[200010];
int n, r, q;
bool cmp(player a, player b) {
    return a.s > b.s || (a.s == b.s && a.n < b.n);
}
void copy(player& a, player& b) {
    a.s = b.s;
    a.w = b.w;
    a.n = b.n;
}
void merge() {
    int win = 1, lose = 1, cur = 1;
    while (win * 2 <= n && lose * 2 <= n) {
        int wi = 2 * win - ((a[2 * win - 1].w > a[2 * win].w) ? 1 : 0);
        int lo = 2 * lose - ((a[2 * lose - 1].w < a[2 * lose].w) ? 1 : 0);
        if (cmp(a[wi], a[lo])) {
            copy(temp[cur++], a[wi]);
            win++;
        }
        else {
            copy(temp[cur++], a[lo]);
            lose++;
        }
    }
    while (win * 2 <= n) {
        int wi = 2 * win - ((a[2 * win - 1].w > a[2 * win].w) ? 1 : 0);
        copy(temp[cur++], a[wi]);
        win++;
    }
    while (lose * 2 <= n) {
        int lo = 2 * lose - ((a[2 * lose - 1].w < a[2 * lose].w) ? 1 : 0);
        copy(temp[cur++], a[lo]);
        lose++;
    }
    for (int i = 1; i <= n; i++) {
        copy(a[i], temp[i]);
    }
    return;
}

int main() {

    scanf("%d %d %d", &n, &r, &q);
    n *= 2;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i].s);
        a[i].n = i;
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i].w);
    }
    sort(a + 1, a + n + 1, cmp);
    for (int i = 1; i <= r; i++) {
        for (int i = 1; i < n; i += 2) {
            if (a[i].w > a[i + 1].w) a[i].s++;
            else a[i + 1].s++;
        }
        merge();
    }
    printf("%d", a[q].n);
    return 0;
}
