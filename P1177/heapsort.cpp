#include <cstdio>
#include <algorithm>
using namespace std;
void sift_down(int arr[], int start, int end) {
    int parent = start;
    int child = parent * 2 + 1;
    while (child <= end) {
        if (child + 1 <= end && arr[child] < arr[child + 1]) child++;
        if (arr[parent] >= arr[child])
            return;
        else {
            swap(arr[parent], arr[child]);
            parent = child;
            child = parent * 2 + 1;
        }
    }
}

void heapsort(int arr[], int len) {
    for (int i = (len - 1 - 1) / 2; i >= 0; i--) sift_down(arr, i, len - 1);
    for (int i = len - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        sift_down(arr, 0, i - 1);
    }
}
int main() {
    int n, a[500050];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    heapsort(a, n);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}