#include <cstdio>
using namespace std;
int n, a[500050], c[500050];
void merge(int* a, int la, int* b, int lb, int* c) {
    int i = 0, j = 0, k = 0;
    while (i < la && j < lb) {
        if (b[j] < a[i]) {
            c[k] = b[j];
            j++;
        }
        else {
            c[k] = a[i];
            i++;
        }
        k++;
    }
    for (; i < la; i++, k++) c[k] = a[i];
    for (; j < lb; j++, k++) c[k] = b[j];
}
void mergesort(int* a, int l, int r) {
    if (r <= l) return;
    int mid = l + ((r - l) >> 1);
    mergesort(a, l, mid);
    mergesort(a, mid + 1, r);
    merge(a + l, mid - l + 1, a + mid + 1, r - mid, c + l);
    for (int i = l; i <= r; i++) a[i] = c[i];
}
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    mergesort(a, 0, n-1);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}