#include <cstdio>
using namespace std;
int n[22][22][22];
int f(long long a, long long b, long long c) {
    if (a <= 0 || b <= 0 || c <= 0) return 1;
    if (a > 20 || b > 20 || c > 20) {
        return f(20, 20, 20);
    }
    if (n[a][b][c]) return n[a][b][c];
    if (a < b && b < c) {
        n[a][b][c] = f(a, b, c - 1) + f(a, b - 1, c - 1) - f(a, b - 1, c);
        return n[a][b][c];
    }
    n[a][b][c] = f(a - 1, b, c) + f(a - 1, b - 1, c) + f(a - 1, b, c - 1) - f(a - 1, b - 1, c - 1);
    return n[a][b][c];
}


int main() {
    long long a, b, c;
    while (true) {
        scanf("%lld %lld %lld", &a, &b, &c);
        if (a == -1 && b == -1 && c == -1) return 0;
        else printf("w(%lld, %lld, %lld) = %d\n", a, b, c, f(a, b, c));
    }

    return 0;
}