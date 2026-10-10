#include <cstdio>
using namespace std;
int n, s;
int main() {
    scanf("%d", &n);
    while (n) {
        s = s * 10 + n % 10;
        n /= 10;
    }
    printf("%d", s);
    return 0;
}