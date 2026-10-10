#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
using namespace std;
struct chece {
    int type = -1;
    int camp = -1;
};
//-1:empty  1:captain  2:guard  3:elephant  4:horse  5:car  6:duck  7:soldier
char type[8][20] = { "empty", "captain", "guard", "elephant", "horse", "car", "duck", "soldier" };

chece board[10][10];
int gameover = 0;
void init() {
    board[2][0].type = board[2][8].type = board[7][0].type = board[7][8].type = 6;
    board[0][0].type = board[0][8].type = board[9][0].type = board[9][8].type = 5;
    board[0][1].type = board[0][7].type = board[9][1].type = board[9][7].type = 4;
    board[0][2].type = board[0][6].type = board[9][2].type = board[9][6].type = 3;
    board[0][3].type = board[0][5].type = board[9][3].type = board[9][5].type = 2;
    board[0][4].type = board[9][4].type = 1;
    board[3][0].type = board[3][2].type = board[3][4].type = board[3][6].type = board[3][8].type =
        board[6][0].type = board[6][2].type = board[6][4].type = board[6][6].type = board[6][8].type = 7;
    for (int i = 0; i <= 9; i++) {
        for (int j = 0; j <= 8; j++) {
            if (board[i][j].type != -1) {
                if (i <= 3) board[i][j].camp = 0;
                else board[i][j].camp = 1;
            }
            else {
                board[i][j].camp = board[i][j].type = -1;
            }
        }
    }
}
bool check(int xs, int ys, int xt, int yt, int curcamp) {
    if (xs < 0 || xs > 9 || ys < 0 || ys > 8 ||
        xt < 0 || xt > 9 || yt < 0 || yt > 8) return false;   //超过边界
    if (board[xs][ys].camp != curcamp) return false;  //移动对方阵营棋子或无棋子
    if (board[xs][ys].camp == board[xt][yt].camp) return false;  //目的位置有己方棋子
    if (board[xs][ys].type == 1) {
        if ((abs(xs - xt) == 1 && ys == yt) || (xs == xt && abs(ys - yt) == 1)) return true;
        else return false;
    }
    else if (board[xs][ys].type == 2) {
        if ((abs(xs - xt) == 1) && (abs(ys - yt) == 1)) return true;
        else return false;
    }
    else if (board[xs][ys].type == 3) {
        if ((abs(xs - xt) == 2) && (abs(ys - yt) == 2) && board[(xs + xt) / 2][(ys + yt) / 2].type == -1) return true;
        else return false;
    }
    else if (board[xs][ys].type == 4) {
        if (abs(xs - xt) == 1) {
            if (abs(ys - yt) == 2 && board[xs][(ys + yt) / 2].type == -1) return true;
            else return false;
        }
        if (abs(ys - yt) == 1) {
            if (abs(xs - xt) == 2 && board[(xs + xt) / 2][ys].type == -1) return true;
            else return false;
        }
        return false;
    }
    else if (board[xs][ys].type == 5) {
        if (xs == xt) {
            for (int i = min(ys, yt)+1; i < max(ys, yt); i++) {
                if (board[xs][i].type != -1) return false;
            }
            return true;
        }
        if (ys == yt) {
            for (int i = min(xs, xt)+1; i < max(xs, xt); i++) {
                if (board[i][ys].type != -1) return false;
            }
            return true;
        }
        return false;
    }
    else if (board[xs][ys].type == 6) {
        int dx = xt - xs, dy = yt - ys;
        int sx = (dx > 0 ? 1 : -1);
        int sy = (dy > 0 ? 1 : -1);

        if (abs(dx) == 3 && abs(dy) == 2) {
            return board[xs + sx][ys].type == -1 &&
                  board[xs + 2 * sx][ys + sy].type == -1;
        }
        if (abs(dx) == 2 && abs(dy) == 3) {
          return board[xs][ys + sy].type == -1 &&
                   board[xs + sx][ys + 2 * sy].type == -1;
     }
        return false;
    }
    else if (board[xs][ys].type == 7) {
        if (abs(xs - xt) <= 1 && abs(ys - yt) <= 1) return true;
        return false;
    }
    return false;
}
bool checkgeneral() {
    if (gameover) return false;

    // 将军可能发生在任意一方，因此先记录双方王的位置。
    int captainx[2] = { -1, -1 };
    int captainy[2] = { -1, -1 };
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 9; y++) {
            int camp = board[x][y].camp;
            if (board[x][y].type == 1 && (camp == 0 || camp == 1)) {
                captainx[camp] = x;
                captainy[camp] = y;
            }
        }
    }

    // 一方的王已被吃掉时，游戏结束，不再有将军局面。
    if (captainx[0] == -1 || captainx[1] == -1) return false;

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 9; y++) {
            int camp = board[x][y].camp;
            if (board[x][y].type == -1 || (camp != 0 && camp != 1)) continue;
            if (check(x, y, captainx[!camp], captainy[!camp], camp))
                return true;
        }
    }
    return false;
}
void move(int xs, int ys, int xt, int yt, int curcamp) {
    const char* color[2] = { "red", "blue" };
    chece moved = board[xs][ys];
    chece eaten = board[xt][yt];

    board[xt][yt] = moved;
    board[xs][ys] = chece{};

    // 吃掉王后游戏立即结束，结束的局面不算将军。
    if (eaten.camp == !curcamp && eaten.type == 1) gameover = 1;
    bool general = checkgeneral();

    printf("%s %s;", color[moved.camp], type[moved.type]);
    if (eaten.type == -1)
        printf("NA;");
    else
        printf("%s %s;", color[eaten.camp], type[eaten.type]);
    printf("%s;%s\n", general ? "yes" : "no", gameover ? "yes" : "no");
}
int main() {
    init();
    int q;
    if (scanf("%d", &q) != 1) return 0;
    int curcamp = 0;
    while (q--) {
        int xs, ys, xt, yt;
        if (scanf("%d %d %d %d", &xs, &ys, &xt, &yt) != 4) return 0;
        if (gameover || !check(xs, ys, xt, yt, curcamp)) {
            printf("Invalid command\n");
        }
        else {
            move(xs, ys, xt, yt, curcamp);
            curcamp = !curcamp;
        }
    }
    return 0;
}
