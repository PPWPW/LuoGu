#include <cstdio>
using namespace std;
int n, x, cnt;
int f(int n, int x) {
    int cnt = 0;
    while (n) {
        if (n % 10 == x)cnt++;
        n /= 10;
    }
    return cnt;
}
int main() {
    scanf("%d %d", &n, &x);
    for (int i = 1; i <= n; i++) {
        cnt += f(i, x);
    }
    printf("%d", cnt);
    return 0;
}