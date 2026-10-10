#include <cstdio>
#include <cstring>
using namespace std;
bool check(int i, int j, int k) {
    int a[10];
    memset(a, 0, sizeof(a));
    if (k > 999) return false;
    a[i % 10] = 1; a[(i / 10) % 10] = 1; a[i / 100] = 1;
    a[j % 10] = 1; a[(j / 10) % 10] = 1; a[j / 100] = 1;
    a[k % 10] = 1; a[(k / 10) % 10] = 1; a[k / 100] = 1;
    for (int t = 1; t <= 9; t++)
        if (a[t] == 0) return false;
    return true;
}
int main() {
    for (int i = 102; i <= 333; i++) {
        int j = 2 * i, k = 3 * i;
        if (check(i, j, k))printf("%d %d %d\n", i, j, k);
    }
    return 0;
}