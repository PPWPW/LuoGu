#include <cstdio>
#include <cmath>
#include <iostream>
using namespace std;
long long n, a[100010], s[100010], x, y;
int main() {
    scanf("%lld", &n);
    for(int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        s[i] = a[i] - a[i - 1];
        if(i >= 2) {
            if(s[i] >= 0) x += s[i];
            else y -= s[i];
        }
    }
    printf("%lld\n%lld", max(x, y), abs(x - y) + 1);
    return 0;
}