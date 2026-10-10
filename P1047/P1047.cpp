#include <cstdio>
using namespace std;
int l, m, u, v, cnt;
int a[10010];
int main() {
    scanf("%d %d", &l, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &u, &v);
        for (int j = u; j <= v; j++) {
            a[j] = 1;
        }
    }
    for (int i = 0; i <= l; i++) {
        if (a[i] == 0) cnt++;
    }
    printf("%d", cnt);
    return 0;
}