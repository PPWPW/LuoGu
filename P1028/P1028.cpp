#include <cstdio>
using namespace std;
int f[10010] = { 0, 1 };
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 2; i <= n; i++) {
        for (int j = 1; j <= i / 2; j++) {
            f[i] += f[j];
        }
        f[i]++;
    }
    printf("%d", f[n]);
    return 0;
}