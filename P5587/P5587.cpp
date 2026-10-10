#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;
string ch1[10005], ch2;
int t, ans, top, top2;
char sta[100005], sta2[100005];
int main() {
    int cnt = 1, tot = 1;
    for (;; cnt++) {
        getline(cin, ch1[cnt]);
        if (ch1[cnt][0] == 'E') {
            cnt--;
            break;
        }
    }
    for (;; tot++) {
        getline(cin, ch2);
        top = -1; top2 = -1;
        if (ch2[0] == 'E')break;
        for (int i = 0; i < ch2.size(); ++i) {
            if (ch2[i] == '<') {
                if (top >= 0)top--;
            }
            else sta[++top] = ch2[i];
        }
        for (int i = 0; i < ch1[tot].size(); ++i) {
            if (ch1[tot][i] == '<') {
                if (top2 >= 0)top2--;
            }
            else sta2[++top2] = ch1[tot][i];
        }
        for (int i = 0; i < min(top + 1, top2 + 1); ++i)
            if (sta[i] == sta2[i])ans++;
    }
    scanf("%d", &t);
    printf("%d", ans * 60 / t);
    return 0;
}
