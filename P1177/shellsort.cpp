#include <cstdio>
#include <algorithm>
using namespace std;
void shellsort(int *a, int length) {
  int h = 1;
  while (h < length / 3) {
    h = 3 * h + 1;
  }
  while (h >= 1) {
    for (int i = h; i < length; i++) {
      for (int j = i; j >= h && a[j] < a[j - h]; j -= h) {
        swap(a[j], a[j - h]);
      }
    }
    h = h / 3;
  }
}
int main() {
    int n, a[500050];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    shellsort(a, n);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}