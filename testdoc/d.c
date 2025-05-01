#include <stdio.h>

int main() {
    union {
        int i;
        char c[sizeof(int)];
    } test;

    test.i = 1;  // 将整数1存入联合体

    if (test.c[0] == 1) {
        printf("Little-Endian (LSB first)\n");
    } else {
        printf("Big-Endian (MSB first)\n");
    }

    return 0;
}
