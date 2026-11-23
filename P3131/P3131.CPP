#include <cstdio>
#include <iostream>
using namespace std;
int n, ans;
int a[50050], sum[50050], l[8], r[8];
int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        sum[i] = (sum[i - 1] + a[i]) % 7;
    }
    for(int i = 0; i < 7; i++) {
        l[i] = 50050;
        r[i] = -1;
    }
    for(int i = 0; i <= n; i++) {
        l[sum[i]] = min(l[sum[i]], i);
        r[sum[i]] = max(r[sum[i]], i);
    }

    for(int i = 0; i < 7; i++) {
        if(l[i] != 50050 && r[i] != -1) {
            ans = max(ans, r[i] - l[i]);
            if(l[i] == r[i] && i == 0) ans = max(ans, 1);
        }
    }
    printf("%d", ans);
    return 0;
}
