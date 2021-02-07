#include <stdio.h>

struct C1 {
    char c;
    int i;
    char d;
}c1;

struct C2 {
    int i;
    char c;
    char d;
}c2;

union U1 {
    int i;
    char c;
    char d;
}u1;

int main() {
    printf("%d, %d, %d", sizeof(c1), sizeof(c2), sizeof(u1));
}