#include <cstdio>
using namespace std;
int s[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
int cnt;
bool isdata(int i) {
    int month = i / 100 % 100;
    int day = i % 100;
    if (month < 1 || month>12 || day == 0 || day > s[month]) return false;
    return true;
}
bool iscrc(int i) {
    if (i / 10000 == ((i % 10 * 1000) + (i / 10 % 10 * 100) + (i / 100 % 10 * 10) + (i / 1000 % 10))) return true;
    return false;
}
int main() {
    int da1, da2;
    scanf("%d %d", &da1, &da2);
    for (int i = da1; i <= da2; i++) {
        if (!isdata(i)) continue;
        if (iscrc(i)) cnt++;
    }
    printf("%d", cnt);
    return 0;
}
