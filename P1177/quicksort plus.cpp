#include <cstdio>
#include <algorithm>
using namespace std;

const int INSERTION_SORT_THRESHOLD = 15;

void getpivot(int left, int right, int a[]) {
    int mid = left + (right - left) / 2;
    if (a[left] > a[right]) swap(a[left], a[right]);
    if (a[mid] > a[right]) swap(a[mid], a[right]);
    if (a[left] < a[mid]) swap(a[left], a[mid]);
}

void insertionsort(int left, int right, int a[]) {
    for (int i = left + 1; i <= right; i++) {
        int temp = a[i];
        int j = i - 1;
        while (j >= left && a[j] > temp) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

void quicksort(int left, int right, int a[]) {
    if (left >= right) return;
    if (right - left <= INSERTION_SORT_THRESHOLD) {
        insertionsort(left, right, a);
        return;
    }
    getpivot(left, right, a);
    int pivot = a[left], lt = left, gt = right, i = left + 1;
    while (i <= gt) {
        if (a[i] < pivot) {
            swap(a[i], a[lt]);
            lt++;
            i++;
        }
        else if (a[i] > pivot) {
            swap(a[i], a[gt]);
            gt--;
        }
        else {
            i++;
        }
    }
    quicksort(left, lt - 1, a);
    quicksort(gt + 1, right, a);
}

int main() {
    int n, a[500050];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    quicksort(0, n - 1, a);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}