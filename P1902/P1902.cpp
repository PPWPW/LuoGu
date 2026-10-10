#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#include <utility>
using namespace std;
int a[1010][1010];
bool vis[1010][1010];
int n, m, ma;
const int dx[4] = { 1, -1, 0, 0 };
const int dy[4] = { 0, 0, 1, -1 };

bool check(int mid) {
    memset(vis, 0, sizeof(vis));
    queue<pair<int, int>> q;

    for (int j = 1; j <= m; j++) {
        vis[1][j] = true;
        q.push({ 1, j });
    }

    while (!q.empty()) {
        int x = q.front().first, y = q.front().second;
        q.pop();

        if (x == n) return true;

        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (vis[nx][ny] || a[nx][ny] > mid) continue;
            vis[nx][ny] = true;
            q.push({ nx, ny });
        }
    }
    return false;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
            ma = max(ma, a[i][j]);
        }
    }
    int l = 0, r = ma;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (check(mid)) {
            r = mid;
        }
        else {
            l = mid + 1;
        }
    }
    printf("%d\n", l);
    return 0;
}
