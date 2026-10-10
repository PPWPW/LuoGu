#include <cstdio>
#include <cmath>
using namespace std;
int n;
int main() {
    scanf("%d", &n);
    int k = int(sqrt(n * 2 - 1.75) + 0.5);
    n -= (k - 1) * k / 2;
    if (k % 2 == 1) printf("%d/%d", k - n + 1, n);
    else printf("%d/%d", n, k - n + 1);
    return 0;
}
