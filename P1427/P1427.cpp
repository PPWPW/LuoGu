#include <cstdio>
using namespace std;
int a[150];
int cnt ,t;
int main() {
    while (true) {
        scanf("%d", &t);
        if (t != 0) {
            cnt++;
            a[cnt] = t;
        }
        else break;
    }
    for (int i = cnt; i >= 1; i--) printf("%d ", a[i]);
    return 0;
}