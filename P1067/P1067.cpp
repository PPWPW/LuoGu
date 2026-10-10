#include <cstdio>
#include <cmath>
using namespace std;
int n, a;
int main() {
    scanf("%d", &n);
	for(int i=n; i>=0; i--){
        scanf("%d", &a);
		if(a){
            if (i < n && a>0) printf("+");
            if (abs(a) > 1 || i == 0) printf("%d", a);
            if (a == -1 && i) printf("-");
            if (i > 0) printf("x");
            if (i > 1) printf("^%d", i);
		}
	}
    return 0;
}

