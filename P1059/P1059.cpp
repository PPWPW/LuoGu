#include <cstdio>
#include <algorithm>
using namespace std;
int a[1010], n, cnt;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        if (!a[x]) cnt++;
        a[x] = 1;
    }
    printf("%d\n", cnt);
    for (int i = 1; i < 1010; i++) {
        if (a[i] == 1) {
            printf("%d ", i);
        }
    }
    return 0;
}