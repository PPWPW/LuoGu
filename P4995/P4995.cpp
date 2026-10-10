#include <cstdio>
#include <algorithm>
using namespace std;

int h[305];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &h[i]);
    }
    sort(h, h + n);

    int left = 0, right = n - 1;
    int last = 0;
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        int current;
        if (i % 2 == 0) current = h[right--];
        else current = h[left++];

        long long diff = current - last;
        ans += diff * diff;
        last = current;
    }

    printf("%lld\n", ans);
    return 0;
}
