#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
char s[100000];
long long n, len, l;
long long f(long long x, long long len) {
    if (x <= l) return x;
    long long half = len / 2;
    if (x <= half) return f(x, half);
    if (x == half + 1) return f(half, half);
    return f(x - half - 1, half);
}
int main() {
    scanf("%s %lld", s, &n);
    l = strlen(s);
    len = l;
    while (len < n) {
        len *= 2;
    }
    printf("%c", s[f(n, len) - 1]);
}
