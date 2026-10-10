#include <cstdio>
using namespace std;
int main() {
	long long a1, a2, n;
    scanf("%lld %lld %lld", &a1, &a2, &n);
	long long d = a2 - a1;
	long long an = a1 + (n - 1) * d;
	long long sum = n * (a1 + an) / 2;
    printf("%lld", sum);
	return 0;
}
