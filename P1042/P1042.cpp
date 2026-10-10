#include <cstdio>
char a[1000000];
int w, l, x, n, g, t, m, i;
using namespace std;
int main() {
    int p[10000], q[10000];
    while (scanf("%c", &a[t]) && a[t] != 'E') {
        if (a[t] == 'W') {
            w++;
            x++;
        }
        if (a[t] == 'L') {
            l++;
            n++;
        }
        if ((w > 10 && w - l > 1) || (l > 10 && l - w > 1)) {
            q[m++] = w;
            q[m++] = l;
            w = 0;
            l = 0;
        }
        if ((x > 20 && x - n > 1) || (n > 20 && n - x > 1)) {
            p[g++] = x;
            p[g++] = n;
            x = 0;
            n = 0;
        }
        t++;
    }
    p[g++] = x;
    p[g++] = n;
    q[m++] = w;
    q[m++] = l;
    for (i = 0; i < m; i += 2) {
        if (i == 0) {
            printf("%d:%d", q[i], q[i + 1]);
        }
        else printf("\n%d:%d", q[i], q[i + 1]);
    }
    printf("\n");
    for (i = 0; i < g; i += 2)
        printf("\n%d:%d", p[i], p[i + 1]);
    return 0;
}