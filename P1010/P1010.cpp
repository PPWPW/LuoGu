#include <cstdio>
#include <cmath>
using namespace std;

void print(int x) {
    int a[1000], cnt = 0;
    while (x) {
        a[cnt++] = x % 2;
        x /= 2;
    }
    cnt--;
    if (cnt == 1) {
        printf("2");
    }
    else if (cnt == 0) {
        printf("2(0)");
    }
    else {
        printf("2(");
        print(cnt);
        printf(")");
    }

    for (int i = cnt - 1; i >= 0; i--) {
        if (a[i] == 1) {
            printf("+");
            if (i == 1) {
                printf("2");
            }
            else if (i == 0) {
                printf("2(0)");
            }
            else {
                printf("2(");
                print(i);
                printf(")");
            }
        }
    }
}
int main() {
    int n;
    scanf("%d", &n);
    print(n);
    return 0;
}