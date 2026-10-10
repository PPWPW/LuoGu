#include <cstdio>
#include <cstring>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    char bestName[21];
    int bestAward = -1, totalAward = 0;
    for (int i = 0; i < n; ++i) {
        char name[21], cadre, western;
        int score, review, papers;
        if (scanf("%20s %d %d %c %c %d", name, &score, &review,
                  &cadre, &western, &papers) != 6) return 0;

        int award = 0;
        if (score > 80 && papers >= 1) award += 8000;
        if (score > 85 && review > 80) award += 4000;
        if (score > 90) award += 2000;
        if (score > 85 && western == 'Y') award += 1000;
        if (review > 80 && cadre == 'Y') award += 850;

        totalAward += award;
        if (award > bestAward) {
            bestAward = award;
            strcpy(bestName, name);
        }
    }

    printf("%s\n%d\n%d\n", bestName, bestAward, totalAward);
    return 0;
}
