#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    char *result = "maduiersnfotvbyl";
    // char c = 0;
    // printf("%d, %0x, %0x, %c\n", sizeof(char),'@', '$', c);
    // printf("%c%c%c\n", *result, result[1], result[2]);
    for (int i = 0; i < strlen(result); ++i) {
        printf("%d:%c\n", i, result[i]);
    }
    char c = (char)(96 + 9);
    printf("%c, %c, %c, %c, %c, %c", 105, 111, 110, 101, 102, 103);
}