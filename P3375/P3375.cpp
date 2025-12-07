#include <cstdio>
#include <cstring>
using namespace std;
char s1[1000010], s2[1000010];
int pi[1000010], find[1000010], cnt = 0;
void findpi(char* s) {
    int n = strlen(s);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return;
}
void kmp(char* s1, char* s2) {
    int n1 = strlen(s1);
    int n2 = strlen(s2);
    int i = 0, j = 0;
    if (n1 < n2) return;
    while (i < n1 && j < n2) {
        if (s1[i] == s2[j]) {
            i++; j++;
        }
        else {
            if (j != 0) j = pi[j - 1];
            else i++;
        }
        if (j == n2) {
            find[cnt++] = i - j + 1;
            printf("%d\n", i - j + 1);
            j = pi[j - 1];
        }
    }
}
int main() {
    scanf("%s", s1);
    scanf("%s", s2);
    //printf("%s %s", s1, s2);
    findpi(s2);
    kmp(s1, s2);
    int n = strlen(s2);
    for (int i = 0; i < n; i++) {
        printf("%d ", pi[i]);
    }
    return 0;
}
