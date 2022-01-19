#include <bits/stdc++.h>
using namespace std;

int add(void) {
    return 1;
}

int mul(void) {
    return 2;
}

 static int (*f[])(void) = {
    [1] add,
    [2] mul,
};

int main() {

    return 0;
}
