#include <cstdio>
using namespace std;
int n, m, k, t;
int a[1111][1111];
int f(int i) {
    int sum = 0;
    for (int j = 1; j <= m; j++) {
        sum += a[i][j];
    }
    return sum;
}
int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &t);
            a[t][j]=1;
        }
    }
    for (int i = 1; i <= k; i++) {
        printf("%d ", f(i));
    }
    return 0;
}