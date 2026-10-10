#include <cstdio>
using namespace std;
int l, r, x, ans;
int main() {
    scanf("%d%d", &l, &r);
    for (int i = l; i <= r; i++){
        x = i;
        while (x > 0){
            if (x % 10 == 2) ans++;
            x /= 10;
        }
    }
    printf("%d", ans);

    return 0;
}
