#include <cstdio>
#include <algorithm>
using namespace std;

struct Farmer {
    int price, amount;
} farmers[5005];

bool cmp(Farmer &a, Farmer &b) {
    return a.price < b.price;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    for (int i = 0; i < m; i++) {
        scanf("%d %d", &farmers[i].price, &farmers[i].amount);
    }
    sort(farmers, farmers + m, cmp);

    long long ans = 0;
    for (int i = 0; i < m && n > 0; i++) {
        int buy = min(n, farmers[i].amount);
        ans += 1LL * buy * farmers[i].price;
        n -= buy;
    }

    printf("%lld\n", ans);
    return 0;
}
