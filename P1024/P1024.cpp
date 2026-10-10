#include <cstdio>
#include <cmath>
using namespace std;
double a, b, c, d;
int cnt;
double f(double x) {
    return a * pow(x, 3) + b * pow(x, 2) + c * x + d;
}
void find(double l, double r) {
    cnt++;
    double mid;
    while (r - l >= 0.001) {
        mid = (r + l) / 2;
        if (f(l) * f(mid) <= 0) r = mid;
        else l = mid;
    }
    printf("%.2lf ", mid);
}

int main() {
    double le = -100.001, ri = -99.001;
    scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
    while (cnt < 3 && ri <= 100) {
        if (f(le) * f(ri) <= 0) {
            find(le, ri);
        }
        le += 1;
        ri += 1;
    }

    return 0;
}
