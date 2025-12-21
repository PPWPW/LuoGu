#include <cstdio>
#include <algorithm>
using namespace std;

const int INSERTION_SORT = 20;

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

void getpivot(int left, int right, int a[]) {
    int mid = left + (right - left) / 2;
    if (a[left] > a[right]) swap(a[left], a[right]);
    if (a[mid] > a[right]) swap(a[mid], a[right]);
    if (a[left] < a[mid]) swap(a[left], a[mid]);
}

void getradom(int left, int right, int a[]) {
    int ra = rand() % (right - left + 1) + left;
    swap(a[left], a[ra]);
}

int partition(int left, int right, int a[]) {
    getradom(left, right, a);
    getpivot(left, right, a);
    int pivot = a[left];
    while (left < right) {
        while (a[right] >= pivot && left < right) right--;
        a[left] = a[right];
        while (a[left] <= pivot && left < right) left++;
        a[right] = a[left];
    }
    a[left] = pivot;
    return left;
}

void quicksort(int left, int right, int a[]) {
    if (right - left > INSERTION_SORT) {
        if (left < right) {
            int pivot = partition(left, right, a);
            quicksort(left, pivot - 1, a);
            quicksort(pivot + 1, right, a);
        }
    }
    else if (left < right) {
        insertionsort(left, right, a);
    }
}

int main() {
    int n, a[500050];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    quicksort(0, n - 1, a);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}