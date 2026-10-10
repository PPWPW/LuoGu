#include <cstdio>
using namespace std;
int n, a, b, p[205], q[205], x, y;
int k[5][5] = { { 0, 0, 1, 1, 0 },
                { 1, 0, 0, 1, 0 },
                { 0, 1, 0, 0, 1 },
                { 0, 0, 1, 0, 1 },
                { 1, 1, 0, 0, 0 } };
int main() {
    scanf("%d %d %d", &n, &a, &b);
    for (int i = 0; i < a; i++)
        scanf("%d", &p[i]);
    for (int i = 0; i < b; i++)
        scanf("%d", &q[i]);
    for (int i = 0; i < n; i++) {
        x += k[p[i % a]][q[i % b]];
        y += k[q[i % b]][p[i % a]];
    }
    printf("%d %d", x, y);
    return 0;
}
