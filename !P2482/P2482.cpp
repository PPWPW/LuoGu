#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Role { MP, ZP, FP };

struct Pig {
    Role role = MP;
    int hp = 4;
    bool alive = true;
    bool crossbow = false;
    bool revealed = false;
    bool suspected = false;  // 只有主猪会根据类反猪标记选目标。
    vector<char> hand;
};

int n, rebelCount = 0;
vector<Pig> pigs;
vector<char> deck;
size_t deckPos = 0;
bool finished = false;
string winner;

bool checkWinner() {
    if (!pigs[1].alive) winner = "FP";
    else if (rebelCount == 0) winner = "MP";
    else return false;
    return finished = true;
}

void draw(int p, int count) {
    while (count--) {
        pigs[p].hand.push_back(deck[deckPos]);
        // 牌堆耗尽后，反复摸最后一张牌。
        if (deckPos + 1 < deck.size()) ++deckPos;
    }
}

bool take(int p, char card) {
    auto& hand = pigs[p].hand;
    auto it = find(hand.begin(), hand.end(), card);
    if (it == hand.end()) return false;
    hand.erase(it);
    return true;
}

int nextAlive(int p) {
    do {
        p = p % n + 1;
    } while (!pigs[p].alive);
    return p;
}

void reveal(int p) {
    pigs[p].revealed = true;
    pigs[p].suspected = false;
}

bool hostile(int p, int target) {
    const Pig& other = pigs[target];
    if (pigs[p].role == MP) {
        return other.revealed ? other.role == FP : other.suspected;
    }
    if (pigs[p].role == ZP) {
        return other.revealed && other.role == FP;
    }
    return other.role == MP || (other.revealed && other.role == ZP);
}

int chooseTarget(int p, char card) {
    if (card == 'K') {
        int target = nextAlive(p);
        return target != p && hostile(p, target) ? target : -1;
    }
    // 决斗没有距离限制，反猪优先对主猪使用。
    if (pigs[p].role == FP && pigs[1].alive) return 1;
    for (int offset = 1; offset < n; ++offset) {
        int target = (p - 1 + offset) % n + 1;
        if (pigs[target].alive && hostile(p, target)) return target;
    }
    return -1;
}

// 返回这张锦囊对 target 的效果是否被抵消。
bool nullified(int source, int target) {
    // 未公开身份的玩家不能被献殷勤，包括对自己。
    if (!pigs[target].revealed) return false;

    bool wantRebel = pigs[target].role == FP;
    bool canceled = false;
    int start = source;
    while (true) {
        int responder = -1;
        for (int offset = 0; offset < n; ++offset) {
            int p = (start - 1 + offset) % n + 1;
            if (pigs[p].alive && (pigs[p].role == FP) == wantRebel && take(p, 'J')) {
                responder = p;
                break;
            }
        }
        if (responder == -1) return canceled;

        reveal(responder);
        canceled = !canceled;
        wantRebel = !wantRebel;
        start = responder;  // 下一张无懈从当前无懈的使用者开始结算。
    }
}

void damage(int source, int target) {
    Pig& victim = pigs[target];
    --victim.hp;
    while (victim.hp <= 0 && take(target, 'P')) ++victim.hp;
    if (victim.hp > 0) return;

    victim.alive = false;
    victim.crossbow = false;
    victim.hand.clear();
    if (victim.role == FP) --rebelCount;

    // 游戏结束时立即停止，不能继续执行奖励或惩罚。
    if (checkWinner()) return;
    if (victim.role == FP) {
        draw(source, 3);
    } else if (victim.role == ZP && pigs[source].role == MP) {
        pigs[source].hand.clear();
        pigs[source].crossbow = false;
    }
}

void duel(int source, int target) {
    if (nullified(source, target)) return;

    int attacker = source, defender = target;
    while (true) {
        // 忠猪在与主猪决斗时不会弃置杀。
        bool refuse = pigs[defender].role == ZP && pigs[attacker].role == MP;
        if (refuse || !take(defender, 'K')) {
            damage(attacker, defender);
            return;
        }
        swap(attacker, defender);
    }
}

void areaAttack(int source, char response) {
    for (int offset = 1; offset < n && !finished; ++offset) {
        int target = (source - 1 + offset) % n + 1;
        if (!pigs[target].alive || nullified(source, target)) continue;
        if (take(target, response)) continue;

        if (target == 1 && !pigs[source].revealed) pigs[source].suspected = true;
        damage(source, target);
    }
}

void playTurn(int p) {
    bool usedKill = false;
    while (pigs[p].alive && !finished) {
        bool played = false;
        for (size_t i = 0; i < pigs[p].hand.size(); ++i) {
            char card = pigs[p].hand[i];
            int target = -1;
            if (card == 'P') {
                if (pigs[p].hp == 4) continue;
            } else if (card == 'K' || card == 'F') {
                if (card == 'K' && usedKill && !pigs[p].crossbow) continue;
                target = chooseTarget(p, card);
                if (target == -1) continue;
            } else if (card != 'Z' && card != 'N' && card != 'W') {
                continue;  // 闪和无懈可击只用于响应。
            }

            // 先消耗牌，再结算；结算可能导致手牌被清空或新增。
            pigs[p].hand.erase(pigs[p].hand.begin() + i);
            played = true;
            switch (card) {
            case 'P':
                ++pigs[p].hp;
                break;
            case 'Z':
                pigs[p].crossbow = true;
                break;
            case 'K':
                usedKill = true;
                reveal(p);
                if (!take(target, 'D')) damage(p, target);
                break;
            case 'F':
                reveal(p);
                duel(p, target);
                break;
            case 'N':
                areaAttack(p, 'K');
                break;
            case 'W':
                areaAttack(p, 'D');
                break;
            }
            break;  // 状态已变化，重新寻找最左边能使用的牌。
        }
        if (!played) break;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    if (!(cin >> n >> m)) return 0;
    pigs.resize(n + 1);
    for (int p = 1; p <= n; ++p) {
        string role;
        cin >> role;
        pigs[p].role = role == "MP" ? MP : role == "ZP" ? ZP : FP;
        if (pigs[p].role == FP) ++rebelCount;
        for (int i = 0; i < 4; ++i) {
            char card;
            cin >> card;
            pigs[p].hand.push_back(card);
        }
    }
    pigs[1].revealed = true;
    deck.resize(m);
    for (char& card : deck) cin >> card;

    checkWinner();
    int current = 1;
    while (!finished) {
        draw(current, 2);
        playTurn(current);
        if (!finished) current = nextAlive(current);
    }

    cout << winner << '\n';
    for (int p = 1; p <= n; ++p) {
        if (!pigs[p].alive) {
            cout << "DEAD\n";
            continue;
        }
        for (size_t i = 0; i < pigs[p].hand.size(); ++i) {
            if (i) cout << ' ';
            cout << pigs[p].hand[i];
        }
        cout << '\n';
    }
    return 0;
}
