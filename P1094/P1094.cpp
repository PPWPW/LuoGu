#include <cstdio>
#include <algorithm>
using namespace std;

int p[30005];

int main() {
    int w, n;
    scanf("%d %d", &w, &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
    }
    sort(p, p + n);

    int left = 0, right = n - 1;
    int ans = 0;
    while (left <= right) {
        if (left < right && p[left] + p[right] <= w) {
            left++;
        }
        right--;
        ans++;
    }

    printf("%d\n", ans);
    return 0;
}
