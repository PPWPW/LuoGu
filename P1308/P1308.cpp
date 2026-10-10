#include <bits/stdc++.h>
using namespace std;


int main() {
    string s1, s2;
    getline(cin, s1);
    getline(cin, s2);
    for (int i = 0; i < s1.size(); i++) if ('A' <= s1[i] && 'Z' >= s1[i]) s1[i] = s1[i] - 'A' + 'a';
    for (int i = 0; i < s2.size(); i++) if ('A' <= s2[i] && 'Z' >= s2[i]) s2[i] = s2[i] - 'A' + 'a';
    s1 = s1 + " "; s1 = " " + s1;
    s2 = s2 + " "; s2 = " " + s2;
    int len = s1.size(), ans = 0, x;
    bool f = 0;
    for (int i = 0; i < s2.size(); i++) {
        string s = s2.substr(i, len);
        if (s == s1) {
            if (!f) x = i, f = 1;
            ans++;
        }
    }
    if (ans) printf("%d %d", ans, x);
    else printf("-1");
    return 0;
}
