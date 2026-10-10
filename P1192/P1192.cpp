#include <cstdio>
using namespace std;
int a[100010] = { 1, 1 };
int f(int n, int k) {
    if (a[n]) return a[n];
    else {
        int sum = 0;
        for (int i = n - 1; i >= n - k && i >= 0; i--) {
            sum += f(i, k);
            sum %= 100003;
        }
        a[n] = sum;
        return a[n];
    }
}
int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    printf("%d", f(n, k));
    return 0;
}
