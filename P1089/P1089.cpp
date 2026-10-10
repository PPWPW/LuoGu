#include <cstdio>
using namespace std;
int cnt, yue, cur;
int main() {
    for (int i = 1; i <= 12; i++) {
        scanf("%d", &cur);
        yue += 300;
        if (yue < cur) {
            printf("%d", i*-1);
            return 0;
        }
        else {
            yue -= cur;
            cnt += ((yue / 100) * 100);
            yue %= 100;
        }
        //printf("%d\n", cnt);
    }
    printf("%d", int(cnt * 1.2) + yue);
    return 0;
}