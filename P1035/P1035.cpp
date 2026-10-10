#include <cstdio>
using namespace std;
double k, cnt;
int main() {
    scanf("%lf", &k);
    for (int i = 1;; i++) {
        cnt += (1.0 / i);
        if (cnt > k) {
            printf("%d", i);
            return 0;
        }
    }
    return 0;
}