#include <cstdio>
#include <iostream>
using namespace std;
int main() {
    int crc=0, cnt=1;
    char x[14];
    scanf("%13s", x);
    //puts(x);
    for (int i = 0; i < 13; i++) {
        if (x[i] != '-') {
            if (cnt == 10) {
                if ((x[12] == 'X' && crc == 10) || (int(x[12] - '0') == crc)) {
                    printf("Right");
                }
                else {
                    x[12] = (crc == 10) ? 'X' : char(crc + (int)'0');
                    puts(x);
                }
            }
            crc += (cnt) * (int)(x[i] - '0');
            crc = crc % 11;
            cnt++;
        }
    }
    return 0;
}
