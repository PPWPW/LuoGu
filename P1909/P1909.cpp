#include <cstdio>
#include <iostream>
using namespace std;
int main() {
    int n, ans = 100000000;
    scanf("%d", &n);
    for (int i = 1; i <= 3; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (n <= a) ans = min(ans, b);
        else if (n % a == 0) ans = min(ans, (n / a) * b);
        else ans = min(ans, (n / a + 1) * b);
    }
    printf("%d", ans);
    return 0;
}
