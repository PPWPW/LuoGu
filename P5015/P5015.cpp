#include <cstdio>
using namespace std;
char s;
int cnt = 0;
int main() {
    while (true) {
        scanf("%c", &s);
        if (s != ' ' && s != '\n') cnt++;
        else if (s == '\n') break;
    }
    printf("%d", cnt);
    return 0;
}