#include <cstdio>
using namespace std;
int n, a[111], cnt;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            for (int k = 1; k <= n; k++) {
                if (k == i || k == j || j == i) continue;
                if (a[i] == (a[j] + a[k])) {
                    printf("%d\n", a[i]);
                    cnt++;
                    goto out;
                }
            }
        }
    out:
        continue;
    }
    printf("%d", cnt);
    return 0;
}