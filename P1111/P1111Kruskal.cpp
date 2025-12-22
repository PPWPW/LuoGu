#include <cstdio>
#include <algorithm>
using namespace std;
int fa[1010];
int n, m;
struct edge {
    int u, v, w;
};
int l;
edge g[100010];
void add(int u, int v, int w) {
    l++;
    g[l].u = u;
    g[l].v = v;
    g[l].w = w;
}
int findroot(int x) { return fa[x] == x ? x : fa[x] = findroot(fa[x]); }
void merge(int x, int y) {
    x = findroot(x);
    y = findroot(y);
    fa[x] = y;
}
bool cmp(edge a, edge b) {
    return a.w < b.w;
}
void kruskal() {
    int tot = 0, ans = 0;
    for (int i = 1; i <= m; i++) {
        int xr = findroot(g[i].u), yr = findroot(g[i].v);
        if (xr != yr) {
            merge(xr, yr);
            tot++;
            ans = max(ans, g[i].w);
        }
    }
    if (tot != n - 1) printf("-1");
    else printf("%d", ans);
}
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++)
        fa[i] = i;
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        add(u, v, w);
    }
    sort(g + 1, g + m + 1, cmp);
    kruskal();
    return 0;
}