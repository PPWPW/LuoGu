#include <cstdio>
#include <algorithm>
using namespace std;

int largest[505], secondLargest[505];

void update(int u, int value) {
    if(value > largest[u]) {
        secondLargest[u] = largest[u];
        largest[u] = value;
    }
    else if(value > secondLargest[u]) {
        secondLargest[u] = value;
    }
}

int main() {
    int n;
    scanf("%d", &n);

    for(int i = 1; i < n; i++) {
        for(int j = i + 1; j <= n; j++) {
            int value;
            scanf("%d", &value);
            update(i, value);
            update(j, value);
        }
    }

    int ans = 0;
    for(int i = 1; i <= n; i++) {
        ans = max(ans, secondLargest[i]);
    }

    printf("1\n%d\n", ans);
    return 0;
}
