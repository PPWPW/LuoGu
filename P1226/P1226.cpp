#include <cstdio>
using namespace std;
long long a, b, p;
int main() {
    scanf("%lld %lld %lld", &a, &b, &p);
    long long cur = a % p, ans = 1, num=b;
    while (num) {
        if (num % 2 == 1) ans *= cur;
        ans %= p;
        cur *= cur;
        cur %= p;
        num /= 2;
    }
    printf("%lld^%lld mod %lld=%lld", a, b, p, ans);
    return 0;
}