#include <cstdio>
using namespace std;
long long a[10000002];
long long m;
long long f(long long i) {
    if (a[i]) return a[i];
    if (i == 1 || i == 2) return a[i] = 1 % m;
    else return a[i] = (f(i - 1) + f(i - 2)) % m;
}
int main() {
    scanf("%lld", &m);
    int i;
    for (i = 1; f(i) != 0 || f(i + 1) != 1; i++) {}
    printf("%lld", i);

    return 0;
}
