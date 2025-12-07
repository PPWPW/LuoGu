#include <cstdio>
#include <iostream>
using namespace std;
char s[1000010];
int len, pi[1000010];
void cal_pi() {
    for (int i = 1; i < len; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return;
}
int main() {
    scanf("%d", &len);
    scanf("%s", s);
    cal_pi();
    printf("%d", len - pi[len - 1]);
    return 0;
}