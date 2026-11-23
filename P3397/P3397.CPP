#include <cstdio>
using namespace std;
int n, m;
int a[10010][10010];
int sum(int x, int y) {
    int cnt = 0;
    for(int i = 1; i <= x; i++) {
        for(int j = 1; j <= y; j++) {
            cnt += a[i][j];
        }
    }
    return cnt;
}
int main() {
    scanf("%d %d", &n, &m);
    while(m--) {
        int x1, x2, y1, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        a[x1][y1]++;
        a[x1][y2+1]--;
        a[x2+1][y1]--;
        a[x2 + 1][y2 + 1]++;
    }
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}