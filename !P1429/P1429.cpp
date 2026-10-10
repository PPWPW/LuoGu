#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Point {
    ll x, y;
};

vector<Point> p, tmp;

bool cmpY(const Point& a, const Point& b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

ll dist2(const Point& a, const Point& b) {
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// 求 p[l, r) 的最近距离平方，返回时该区间按 y 排序
ll solve(int l, int r) {
    if (r - l <= 3) {
        ll best = LLONG_MAX;

        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) {
                best = min(best, dist2(p[i], p[j]));
            }
        }

        sort(p.begin() + l, p.begin() + r, cmpY);
        return best;
    }

    int mid = (l + r) / 2;

    // 必须在递归前保存，递归后点的顺序会发生变化
    ll midX = p[mid].x;

    ll leftBest = solve(l, mid);
    ll rightBest = solve(mid, r);
    ll best = min(leftBest, rightBest);

    // 将两个按 y 排序的区间归并
    merge(p.begin() + l, p.begin() + mid,
          p.begin() + mid, p.begin() + r,
          tmp.begin() + l, cmpY);

    copy(tmp.begin() + l, tmp.begin() + r, p.begin() + l);

    // 归并结果已复制回 p，可以复用 tmp 保存中间带内的点
    int cnt = 0;
    for (int i = l; i < r; ++i) {
        ll dx = p[i].x - midX;
        if (dx * dx < best) {
            tmp[l + cnt++] = p[i];
        }
    }

    // 中间带内的点已经按 y 排序
    for (int i = 0; i < cnt; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            ll dy = tmp[l + j].y - tmp[l + i].y;
            if (dy * dy >= best) break;

            best = min(best, dist2(tmp[l + i], tmp[l + j]));
        }
    }

    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    p.resize(n);
    tmp.resize(n);

    for (auto& point : p) {
        cin >> point.x >> point.y;
    }

    sort(p.begin(), p.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    ll answer = solve(0, n);
    cout << fixed << setprecision(4)
         << sqrtl(static_cast<long double>(answer)) << '\n';

    return 0;
}