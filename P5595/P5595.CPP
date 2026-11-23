#include <cstdio>
using namespace std;

const int MAX_N = 1000000;
char s[MAX_N + 1], x[MAX_N + 1], y[MAX_N + 1];

int main() {
    scanf("%s", s);

    for(int i = 0; s[i] != '\0'; i++) {
        if(i > 0 && s[i - 1] == 'Z' && s[i] != 'Z') {
            printf("-1\n");
            return 0;
        }
        x[i] = s[i] == 'X' ? '1' : '0';
        y[i] = s[i] == 'Y' ? '1' : '0';
    }

    printf("%s\n%s\n", x, y);
    return 0;
}
