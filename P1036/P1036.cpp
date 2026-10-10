#include <cstdio>
#include <cmath>
using namespace std;
int n, k, a[22], cnt = 0;
bool isprime(int x) {
    for (int i = 2; i <= sqrt(x); i++) {
        if (x % i == 0) return false;
    }
    return true;
}
bool cal(int i) {
    int t = 1, cur = 0, s = 0;
    while (i) {
        if (i % 2 == 1) s++;
        cur += a[(i % 2) * t];
        i /= 2;
        t++;
    }
    if (s == k && isprime(cur)) return true;
    return false;
}
int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    for (int i = 1; i < 1 << n; i++) {
        if (cal(i)) cnt++;
    }
    printf("%d", cnt);
    return 0;
}