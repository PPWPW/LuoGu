#include <cstdio>
using namespace std;
int main() {
    int a, b, max=0, ans=0;
    for (int i = 1; i <= 7; i++) {
        scanf("%d %d", &a, &b);
        if (a + b > 8) {
            if (a + b > max) {
                max = a + b;
                ans = i;
            }
        }
    }
    printf("%d", ans);
    return 0;
}