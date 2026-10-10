#include <cstdio>
#include <iostream>
#include <stack>
using namespace std;
stack<int> cyc;
int t, ccnt, maxo, preo;
char c;
char x[100], y[100];
int xnum, ynum, isxnum, isynum;
char cheet[1000];
int geto(char* s) {
    if (s[3] != '^') return 0;
    else {
        int cnt = 4, sum=0;
        while (s[cnt] != ')') {
            sum *= 10;
            sum += s[cnt] - '0';
            cnt++;
        }
        return sum;
    }
}
void getxy(char* x, char* y) {
    if (x[0] == 'n') {
        xnum = 0;
        isxnum = 0;
    }
    else {
        int cnt = 0, sum = 0;
        while (x[cnt] != '\0') {
            sum *= 10;
            sum += x[cnt] - '0';
            cnt++;
        }
        isxnum = 1;
        xnum = sum;
    }
    if (y[0] == 'n') {
        ynum = 0;
        isynum = 0;
    }
    else {
        int cnt = 0, sum = 0;
        while (y[cnt] != '\0') {
            sum *= 10;
            sum += y[cnt] - '0';
            cnt++;
        }
        isynum = 1;
        ynum = sum;
    }

    return ;
}
bool check(char c) {
    for (int i = 0; i < ccnt; i++) {
        if (cheet[i] == c) return false;
    }
    cheet[ccnt++] = c;
    return true;
}
void push() {
    if ((isxnum == 0 && isynum == 1) || (isxnum == 1 && isynum == 1 && xnum > ynum)) {
        cyc.push(-1000);
    }
    else if ((isxnum == 1 && isynum == 1 && xnum <= ynum) || (isxnum==0 && isynum==0)) {
        if (cyc.empty()) cyc.push(0);
        else cyc.push(cyc.top());
    }
    else {
        if (cyc.empty()) cyc.push(1);
        else cyc.push(cyc.top() + 1);
    }
    maxo = max(maxo, cyc.top());
}
void pop(int *err) {
    if (cyc.empty()) {
        *err = 1;
        printf("stack is already empty\n");
    }
    else {
        cyc.pop();
        cheet[ccnt--] = '\0';
    }
}
void print(int err) {
    if (err == 1) printf("ERR\n");
    else if (maxo == preo) printf("Yes\n");
    else printf("No\n");
}
void del() {
    xnum = ynum = isxnum = isynum = 0;
    maxo = preo = 0;
    while (!cyc.empty()) cyc.pop();
    for (int i = 0; i < ccnt; i++) cheet[i] = '\0';
    ccnt = 0;
}
int main() {
    scanf("%d", &t);
    while (t--) {
        int l, err = 0;
        char s[20], r;
        scanf("%d %s", &l, s);
        preo = geto(s);
        //printf("%d", preo);
        for (int i = 1; i <= l; i++) {
            char op[5];
            scanf("%s", op);
            if (op[0] == 'F') {
                scanf(" %c %s %s", &c, x, y);
                getxy(x, y);
                //printf("%d %d %d %d\n", isxnum, xnum, isynum, ynum);
                if (!check(c)) {
                    err = 1;
                    //printf("have the same i\n");
                }
                push();
            }
            else if (op[0] == 'E') {
                pop(&err);
            }
        }
        if (!cyc.empty()) {
            err = 1;
            //printf("stack is not empty\n");
        }
        print(err);
        del();
    }
    return 0;
}
