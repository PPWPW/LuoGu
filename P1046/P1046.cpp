#include <cstdio>
using namespace std;
int a[11], x, cnt;
int main() {
    for (int i = 1; i <= 10; i++) {
        scanf("%d", &a[i]);
    }
    scanf("%d", &x);
    for (int i = 1; i <= 10; i++) {
        if (x + 30 >= a[i]) cnt++;
    }
    printf("%d", cnt);
    return 0;
}