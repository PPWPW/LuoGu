#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int n;
string s[22];
bool cmp(string a, string b) {
    return a + b > b + a;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        cin >> s[i];
    sort(s + 1, s + 1 + n, cmp);
    for (int i = 1; i <= n; i++)
        cout << s[i];
    return 0;
}
